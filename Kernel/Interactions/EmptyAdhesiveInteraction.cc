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

#include "EmptyAdhesiveInteraction.h"
#include "Species/EmptyAdhesiveSpecies.h"
#include "Particles/BaseParticle.h"
#include "InteractionHandler.h"
#include <iomanip>
#include <fstream>

EmptyAdhesiveInteraction::EmptyAdhesiveInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
    : BaseInteraction(P, I, timeStamp)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"EmptyAdhesiveInteraction::EmptyAdhesiveInteraction() finished"<<std::endl;
#endif
}

EmptyAdhesiveInteraction::EmptyAdhesiveInteraction(const EmptyAdhesiveInteraction &p)
    : BaseInteraction(p)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"EmptyAdhesiveInteraction::EmptyAdhesiveInteraction(const EmptyAdhesiveInteraction &p finished"<<std::endl;
#endif
}

EmptyAdhesiveInteraction::~EmptyAdhesiveInteraction()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"EmptyAdhesiveInteraction::~EmptyAdhesiveInteraction() finished"<<std::endl;
#endif
}

void EmptyAdhesiveInteraction::write(std::ostream& os UNUSED) const
{}

void EmptyAdhesiveInteraction::read(std::istream& is UNUSED)
{}

void EmptyAdhesiveInteraction::computeForce()
{}

Mdouble EmptyAdhesiveInteraction::getElasticEnergy() const
{
    return 0.0;
}

const EmptyAdhesiveSpecies *EmptyAdhesiveInteraction::getSpecies() const
{
    return dynamic_cast<const EmptyAdhesiveSpecies *>(getBaseSpecies());
}

std::string EmptyAdhesiveInteraction::getName() const
{
    return "";
}
