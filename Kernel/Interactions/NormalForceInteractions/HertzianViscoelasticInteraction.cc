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

#include "HertzianViscoelasticInteraction.h"
#include "Species/NormalForceSpecies/HertzianViscoelasticNormalSpecies.h"
#include "BaseInteractable.h"
#include "InteractionHandler.h"
#include <iomanip>
#include <fstream>

HertzianViscoelasticInteraction::HertzianViscoelasticInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
        : BaseInteraction(P, I, timeStamp)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"HertzianViscoelasticInteraction::HertzianViscoelasticInteraction() finished"<<std::endl;
#endif
}

HertzianViscoelasticInteraction::HertzianViscoelasticInteraction(const HertzianViscoelasticInteraction &p)
        : BaseInteraction(p)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"HertzianViscoelasticInteraction::HertzianViscoelasticInteraction(const HertzianViscoelasticInteraction &p finished"<<std::endl;
#endif
}

HertzianViscoelasticInteraction::~HertzianViscoelasticInteraction()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"HertzianViscoelasticInteraction::~HertzianViscoelasticInteraction() finished"<<std::endl;
#endif
}

HertzianViscoelasticInteraction* HertzianViscoelasticInteraction::copy() const
{
    return new HertzianViscoelasticInteraction(*this);
}

///BaseParticle print function, which accepts an os std::stringstream as input. It prints human readable BaseParticle information to the std::stringstream
void HertzianViscoelasticInteraction::write(std::ostream& os) const
        {
    BaseInteraction::write(os);
}

void HertzianViscoelasticInteraction::read(std::istream& is)
{
    BaseInteraction::read(is);
}

std::string HertzianViscoelasticInteraction::getName() const
{
    return "HertzianViscoelastic";
}

void HertzianViscoelasticInteraction::computeForce()
{
    ///\todo TWnow
    // Compute the relative velocity vector of particle P w.r.t. I
    setRelativeVelocity(getP()->getVelocityAtContact(getContactPoint()) - getI()->getVelocityAtContact(getContactPoint()));
    // Compute the projection of vrel onto the normal (can be negative)
    setNormalRelativeVelocity(Vec3D::dot(getRelativeVelocity(), getNormal()));

    if (getOverlap() > 0) //if contact forces
    {
        const HertzianViscoelasticNormalSpecies* species = getSpecies();

        //R = 2.0 * PI->getRadius() * PJ->getRadius() / (PI->getRadius() + PJ->getRadius());
        //a = sqrt(R * deltan);
        //Mdouble kn = 4. / 3. * pSpecies->k * a;
        //fdotn += kn * deltan + pSpecies->dissipation_ * vdotn;
        Mdouble effectiveDiameter = 2.0*getEffectiveRadius();
        Mdouble stiffness = 4. / 3. * species->getElasticModulus() * std::sqrt(effectiveDiameter * getOverlap());
        Mdouble normalForce = stiffness * getOverlap() - species->getDissipation() * getNormalRelativeVelocity();

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

Mdouble HertzianViscoelasticInteraction::getElasticEnergy() const
{
    ///\todo TWnow
    return 0.5 * (getSpecies()->getElasticModulus() * mathsFunc::square(getOverlap()));
}

const HertzianViscoelasticNormalSpecies* HertzianViscoelasticInteraction::getSpecies() const
{
    return dynamic_cast<const HertzianViscoelasticNormalSpecies*>(getBaseSpecies());
}
