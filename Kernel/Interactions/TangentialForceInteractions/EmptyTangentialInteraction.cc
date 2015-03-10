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

#include "EmptyTangentialInteraction.h"
#include "Species/TangentialForceSpecies/EmptyTangentialSpecies.h"
#include "Particles/BaseParticle.h"
#include "InteractionHandler.h"
#include <iomanip>
#include <fstream>

EmptyTangentialInteraction::EmptyTangentialInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
    : BaseInteraction(P, I, timeStamp)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"EmptyTangentialInteraction::EmptyTangentialInteraction() finished"<<std::endl;
#endif
}

EmptyTangentialInteraction::EmptyTangentialInteraction(const EmptyTangentialInteraction &p)
    : BaseInteraction(p)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"EmptyTangentialInteraction::EmptyTangentialInteraction(const EmptyTangentialInteraction &p finished"<<std::endl;
#endif
}

EmptyTangentialInteraction::~EmptyTangentialInteraction()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"EmptyTangentialInteraction::~EmptyTangentialInteraction() finished"<<std::endl;
#endif
}

void EmptyTangentialInteraction::write(std::ostream& os UNUSED) const
{}

void EmptyTangentialInteraction::read(std::istream& is UNUSED)
{}

void EmptyTangentialInteraction::computeForce()
{}

void EmptyTangentialInteraction::integrate(Mdouble timeStep UNUSED)
{}

Mdouble EmptyTangentialInteraction::getElasticEnergy() const
{
    return 0.0;
}

Mdouble EmptyTangentialInteraction::getTangentialOverlap() const
{
    return 0;
}

const Vec3D EmptyTangentialInteraction::getTangentialForce() const
{
    return Vec3D(0.0,0.0,0.0);
}

const EmptyTangentialSpecies *EmptyTangentialInteraction::getSpecies() const
{
    return dynamic_cast<const EmptyTangentialSpecies *>(getBaseSpecies());
}

std::string EmptyTangentialInteraction::getName() const
{
    return "";
}
