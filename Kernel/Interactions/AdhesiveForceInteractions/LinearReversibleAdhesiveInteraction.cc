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

#include "LinearReversibleAdhesiveInteraction.h"
#include "Species/AdhesiveForceSpecies/LinearReversibleAdhesiveSpecies.h"
#include "Particles/BaseParticle.h"
#include "InteractionHandler.h"
#include <iomanip>
#include <fstream>

LinearReversibleAdhesiveInteraction::LinearReversibleAdhesiveInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
    : BaseInteraction(P, I, timeStamp)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LinearReversibleAdhesiveInteraction::LinearReversibleAdhesiveInteraction() finished"<<std::endl;
#endif
}

LinearReversibleAdhesiveInteraction::LinearReversibleAdhesiveInteraction(const LinearReversibleAdhesiveInteraction &p)
    : BaseInteraction(p)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LinearReversibleAdhesiveInteraction::LinearReversibleAdhesiveInteraction(const LinearReversibleAdhesiveInteraction &p finished"<<std::endl;
#endif
}

LinearReversibleAdhesiveInteraction::~LinearReversibleAdhesiveInteraction()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LinearReversibleAdhesiveInteraction::~LinearReversibleAdhesiveInteraction() finished"<<std::endl;
#endif
}

void LinearReversibleAdhesiveInteraction::write(std::ostream& os  UNUSED) const
{}

void LinearReversibleAdhesiveInteraction::read(std::istream& is  UNUSED)
{}

void LinearReversibleAdhesiveInteraction::computeForce()
{
    std::cout << "LinearReversibleAdhesiveInteraction::computeForce" << std::endl;
    const LinearReversibleAdhesiveSpecies* species = getSpecies();

    if (getOverlap()>=0)
        addForce(getNormal() * (-species->getAdhesionForceMax()));
    else
        addForce(getNormal() * (-species->getAdhesionStiffness() *getOverlap() - species->getAdhesionForceMax()));
}

Mdouble LinearReversibleAdhesiveInteraction::getElasticEnergy() const
{
    ///\todo TW
    return 0.0;
}

const LinearReversibleAdhesiveSpecies *LinearReversibleAdhesiveInteraction::getSpecies() const
{
    return dynamic_cast<const LinearReversibleAdhesiveSpecies *>(getBaseSpecies());
}

std::string LinearReversibleAdhesiveInteraction::getName() const
{
    return "LinearReversibleAdhesive";
}
