//Copyright (c) 2013-2014, The MercuryDPM Developers Team. All rights reserved.
//For the list of developers, see <http://www.MercuryDPM.org/Team>.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name MercuryDPM nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE MERCURYDPM DEVELOPERS TEAM BE LIABLE FOR ANY
//DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#include "LinearPlasticViscoelasticInteraction.h"
#include "InteractionHandler.h"
#include "Particles/BaseParticle.h"
#include <iomanip>
#include <fstream>
#include <Species/NormalForceSpecies/LinearPlasticViscoelasticNormalSpecies.h>
#include <cmath>    // std::max

LinearPlasticViscoelasticInteraction::LinearPlasticViscoelasticInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
        : BaseInteraction(P, I, timeStamp)
{
    maxOverlap_=0;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LinearPlasticViscoelasticInteraction::LinearPlasticViscoelasticInteraction() finished"<<std::endl;
#endif
}

LinearPlasticViscoelasticInteraction::LinearPlasticViscoelasticInteraction(const LinearPlasticViscoelasticInteraction &p)
        : BaseInteraction(p)
{
    maxOverlap_=p.maxOverlap_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LinearPlasticViscoelasticInteraction::LinearPlasticViscoelasticInteraction(const LinearPlasticViscoelasticInteraction &p finished"<<std::endl;
#endif
}

LinearPlasticViscoelasticInteraction::~LinearPlasticViscoelasticInteraction()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout<<"LinearPlasticViscoelasticInteraction::~LinearPlasticViscoelasticInteraction() finished"<<std::endl;
#endif
}

BaseInteraction* LinearPlasticViscoelasticInteraction::copy() const
{
    return new LinearPlasticViscoelasticInteraction(*this);
}

///BaseParticle print function, which accepts an os std::stringstream as input. It prints human readable BaseParticle information to the std::stringstream
void LinearPlasticViscoelasticInteraction::write(std::ostream& os) const
        {
    BaseInteraction::write(os);
}

void LinearPlasticViscoelasticInteraction::read(std::istream& is)
{
    BaseInteraction::read(is);
}

std::string LinearPlasticViscoelasticInteraction::getBaseName() const
{
    return "LinearPlasticViscoelastic";
}

void LinearPlasticViscoelasticInteraction::computeLinearPlasticViscoelasticForce()
{
    // Compute the relative velocity vector of particle P w.r.t. I
    setRelativeVelocity(getP()->getVelocityAtContact(getContactPoint()) - getI()->getVelocityAtContact(getContactPoint()));
    // Compute the projection of vrel onto the normal (can be negative)
    setNormalRelativeVelocity(Vec3D::dot(getRelativeVelocity(), getNormal()));

    if (getOverlap() > 0) //if contact forces
    {
        const LinearPlasticViscoelasticNormalSpecies* species = getSpecies();
        //Mdouble normalForce = species->getLoadingStiffness() * getOverlap() - species->getDissipation() * getNormalRelativeVelocity();
        ///\todo TWnow this has to be corrected; I think the dissipation force is applied in the wrong direction
        Mdouble normalForce = -species->getDissipation() * getNormalRelativeVelocity();

        BaseParticle* PParticle = dynamic_cast<BaseParticle*>(getP());
        BaseParticle* IParticle = dynamic_cast<BaseParticle*>(getI());
        Mdouble effectiveDiameter;
        if (IParticle == 0)
            effectiveDiameter = 2.0 * PParticle->getRadius();
        else
            effectiveDiameter = ((2.0 * PParticle->getRadius() * IParticle->getRadius()) / (PParticle->getRadius() + IParticle->getRadius()));
        //calculate deltastar, the overlap above which k2max becomes active (the 'fluid branch')
        Mdouble deltaStar = (species->getUnloadingStiffnessMax() / (species->getUnloadingStiffnessMax() - species->getLoadingStiffness())) * species->getPenetrationDepthMax() * effectiveDiameter;
        //2*depth*r_eff is the overlap at which fn=0 on the k2max branch
        //deltastar is the overlap at which the k1 and the k2max branch meet

        //retrieve history parameter deltamax, the max. achieved overlap
        maxOverlap_ = std::min(deltaStar, std::max(maxOverlap_, getOverlap()));

        //calculate k2(deltamax) (only use first case for Walton-Braun spring)
        Mdouble unloadingStiffness;
        if (getOverlap() > deltaStar)
            unloadingStiffness = species->getUnloadingStiffnessMax();
        else
            unloadingStiffness = species->getLoadingStiffness() + (species->getUnloadingStiffnessMax() - species->getLoadingStiffness()) * (maxOverlap_ / deltaStar);

        //calculate delta0(deltamax), the overlap where the force is zero
        Mdouble equilibriumOverlap = (unloadingStiffness - species->getLoadingStiffness()) / unloadingStiffness * maxOverlap_;

        //add elastic force
        //std::cout << k2*(getOverlap()-(delta0))-species->k*getOverlap() << std::endl;
        if (getOverlap() > deltaStar)
        {
            normalForce += species->getUnloadingStiffnessMax() * (getOverlap() - equilibriumOverlap);
        }
        else if (unloadingStiffness * (getOverlap() - (equilibriumOverlap)) >= species->getLoadingStiffness() * getOverlap())
        {
            normalForce += species->getLoadingStiffness() * getOverlap();
        }
        else if (unloadingStiffness * (getOverlap() - equilibriumOverlap) >= -species->getCohesionStiffness() * getOverlap())
        {
            normalForce += unloadingStiffness * (getOverlap() - equilibriumOverlap);
        }
        else
        {
            normalForce += -species->getCohesionStiffness() * getOverlap();
            maxOverlap_ = (unloadingStiffness + species->getCohesionStiffness()) / (unloadingStiffness - species->getLoadingStiffness()) * getOverlap();
        }

        setAbsoluteNormalForce(std::abs(normalForce)); //used for further corce calculations;
        setForce(getNormal() * normalForce);
        setTorque(Vec3D(0.0, 0.0, 0.0));
    }
    else
    {
        setAbsoluteNormalForce(0.0);
        setForce(Vec3D(0.0, 0.0, 0.0));
        setTorque(Vec3D(0.0, 0.0, 0.0));
    }
}

void LinearPlasticViscoelasticInteraction::computeNormalForce()
{
    computeLinearPlasticViscoelasticForce();
}

Mdouble LinearPlasticViscoelasticInteraction::getElasticEnergy() const
{
   if (getOverlap() > 0)
        return 0.5 * (getSpecies()->getLoadingStiffness() * mathsFunc::square(getOverlap()));
    else
        return 0.0;
  ///\todo TW this is not correct; we should count the return energy
}

const LinearPlasticViscoelasticNormalSpecies* LinearPlasticViscoelasticInteraction::getSpecies() const
{
    return dynamic_cast<const LinearPlasticViscoelasticNormalSpecies*>(getBaseSpecies());
}

Mdouble LinearPlasticViscoelasticInteraction::getMaxOverlap() const
{
    return maxOverlap_;
}

void LinearPlasticViscoelasticInteraction::setMaxOverlap(const Mdouble maxOverlap)
{
    maxOverlap_ = maxOverlap;
}

Mdouble LinearPlasticViscoelasticInteraction::getUnloadingStiffness() const
{
    const LinearPlasticViscoelasticNormalSpecies* species = getSpecies();
    Mdouble effectiveDiameter = 2.0*getEffectiveRadius();
    Mdouble deltaMaxFluid = species->getPenetrationDepthMax() * effectiveDiameter / (1.0-species->getLoadingStiffness()/species->getUnloadingStiffnessMax());
    if (getOverlap() > deltaMaxFluid)
        return species->getUnloadingStiffnessMax();
    else
        return species->getLoadingStiffness() + (species->getUnloadingStiffnessMax() - species->getLoadingStiffness()) * getMaxOverlap()/deltaMaxFluid;
}

