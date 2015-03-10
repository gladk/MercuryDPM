// This file is part of MercuryDPM.
//
// MercuryDPM is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// MercuryDPM is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with MercuryDPM.  If not, see <http://www.gnu.org/licenses/>.
//
// Copyright 2013 The Mercury Developers Team
// For the list of developers, see <http://www.MercuryDPM.org/Team>

#include "LinearPlasticViscoelasticInteraction.h"
#include "InteractionHandler.h"
#include "Particles/BaseParticle.h"
#include "Species/BaseSpecies.h"
#include <iomanip>
#include <fstream>
#include <Species/LinearPlasticViscoelasticSpecies.h>
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
#ifdef DEBUG_CONSTRUCTOR
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

std::string LinearPlasticViscoelasticInteraction::getName() const
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
        const LinearPlasticViscoelasticSpecies* species = getSpecies();
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
            normalForce += species->getUnloadingStiffnessMax() * (getOverlap() - (equilibriumOverlap));
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

void LinearPlasticViscoelasticInteraction::computeForce()
{
    computeLinearPlasticViscoelasticForce();
}

Mdouble LinearPlasticViscoelasticInteraction::getElasticEnergy() const
{
    return 0;
    //return 0.5 * (getSpecies()->getStiffness() * mathsFunc::square(getOverlap()));
}

const LinearPlasticViscoelasticSpecies* LinearPlasticViscoelasticInteraction::getSpecies() const
{
    return dynamic_cast<const LinearPlasticViscoelasticSpecies*>(getBaseSpecies());
}

Mdouble LinearPlasticViscoelasticInteraction::getMaxOverlap() const
{
    return maxOverlap_;
}

void LinearPlasticViscoelasticInteraction::setMaxOverlap(const Mdouble maxOverlap)
{
    maxOverlap_ = maxOverlap;
}

