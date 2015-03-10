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

#include "ReversibleAdhesiveInteraction.h"
#include "Species/AdhesiveForceSpecies/ReversibleAdhesiveSpecies.h"
#include "Particles/BaseParticle.h"
#include "InteractionHandler.h"
#include <iomanip>
#include <fstream>

ReversibleAdhesiveInteraction::ReversibleAdhesiveInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
    : BaseInteraction(P, I, timeStamp)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"ReversibleAdhesiveInteraction::ReversibleAdhesiveInteraction() finished"<<std::endl;
#endif
}

ReversibleAdhesiveInteraction::ReversibleAdhesiveInteraction(const ReversibleAdhesiveInteraction &p)
    : BaseInteraction(p)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"ReversibleAdhesiveInteraction::ReversibleAdhesiveInteraction(const ReversibleAdhesiveInteraction &p finished"<<std::endl;
#endif
}

ReversibleAdhesiveInteraction::~ReversibleAdhesiveInteraction()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"ReversibleAdhesiveInteraction::~ReversibleAdhesiveInteraction() finished"<<std::endl;
#endif
}

void ReversibleAdhesiveInteraction::write(std::ostream& os  UNUSED) const
{}

void ReversibleAdhesiveInteraction::read(std::istream& is  UNUSED)
{}

void ReversibleAdhesiveInteraction::computeForce()
{
    std::cout << "ReversibleAdhesiveInteraction::computeForce" << std::endl;
    const ReversibleAdhesiveSpecies* species = getSpecies();

    if (getOverlap()>=0)
        addForce(getNormal() * (-species->getAdhesionForceMax()));
    else
        addForce(getNormal() * (-species->getAdhesionStiffness() *getOverlap() - species->getAdhesionForceMax()));
}

Mdouble ReversibleAdhesiveInteraction::getElasticEnergy() const
{
    ///\todo TW
    return 0.0;
}

const ReversibleAdhesiveSpecies *ReversibleAdhesiveInteraction::getSpecies() const
{
    return dynamic_cast<const ReversibleAdhesiveSpecies *>(getBaseSpecies());
}

std::string ReversibleAdhesiveInteraction::getName() const
{
    return "ReversibleAdhesive";
}
