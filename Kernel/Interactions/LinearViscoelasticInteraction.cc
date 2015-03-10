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

#include "Interactions/LinearViscoelasticInteraction.h"
#include "Species/LinearViscoelasticSpecies.h"
#include "BaseInteractable.h"
#include "InteractionHandler.h"
#include <iomanip>
#include <fstream>

LinearViscoelasticInteraction::LinearViscoelasticInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
        : BaseInteraction(P, I, timeStamp)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LinearViscoelasticInteraction::LinearViscoelasticInteraction() finished"<<std::endl;
#endif
}

LinearViscoelasticInteraction::LinearViscoelasticInteraction(const LinearViscoelasticInteraction &p)
        : BaseInteraction(p)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LinearViscoelasticInteraction::LinearViscoelasticInteraction(const LinearViscoelasticInteraction &p finished"<<std::endl;
#endif
}

LinearViscoelasticInteraction::~LinearViscoelasticInteraction()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LinearViscoelasticInteraction::~LinearViscoelasticInteraction() finished"<<std::endl;
#endif
}

LinearViscoelasticInteraction* LinearViscoelasticInteraction::copy() const
{
    return new LinearViscoelasticInteraction(*this);
}

///BaseParticle print function, which accepts an os std::stringstream as input. It prints human readable BaseParticle information to the std::stringstream
void LinearViscoelasticInteraction::write(std::ostream& os) const
        {
    BaseInteraction::write(os);
}

void LinearViscoelasticInteraction::read(std::istream& is)
{
    BaseInteraction::read(is);
}

std::string LinearViscoelasticInteraction::getName() const
{
    return "LinearViscoelastic";
}

void LinearViscoelasticInteraction::computeForce()
{
    // Compute the relative velocity vector of particle P w.r.t. I
    setRelativeVelocity(getP()->getVelocityAtContact(getContactPoint()) - getI()->getVelocityAtContact(getContactPoint()));
    // Compute the projection of vrel onto the normal (can be negative)
    setNormalRelativeVelocity(Vec3D::dot(getRelativeVelocity(), getNormal()));

    if (getOverlap() > 0) //if contact forces
    {
        const LinearViscoelasticSpecies* species = getSpecies();

        Mdouble normalForce = species->getStiffness() * getOverlap() - species->getDissipation() * getNormalRelativeVelocity();
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

Mdouble LinearViscoelasticInteraction::getElasticEnergy() const
{
    return 0.5 * (getSpecies()->getStiffness() * mathsFunc::square(getOverlap()));
}

const LinearViscoelasticSpecies* LinearViscoelasticInteraction::getSpecies() const
{
    return dynamic_cast<const LinearViscoelasticSpecies*>(getBaseSpecies());
}
