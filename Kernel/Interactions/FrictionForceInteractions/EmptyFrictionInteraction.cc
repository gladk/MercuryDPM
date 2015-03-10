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

#include "EmptyFrictionInteraction.h"
#include "Species/FrictionForceSpecies/EmptyFrictionSpecies.h"
#include "Particles/BaseParticle.h"
#include "InteractionHandler.h"
#include <iomanip>
#include <fstream>

EmptyFrictionInteraction::EmptyFrictionInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
    : BaseInteraction(P, I, timeStamp)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"EmptyFrictionInteraction::EmptyFrictionInteraction() finished"<<std::endl;
#endif
}

EmptyFrictionInteraction::EmptyFrictionInteraction(const EmptyFrictionInteraction &p)
    : BaseInteraction(p)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"EmptyFrictionInteraction::EmptyFrictionInteraction(const EmptyFrictionInteraction &p finished"<<std::endl;
#endif
}

EmptyFrictionInteraction::~EmptyFrictionInteraction()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"EmptyFrictionInteraction::~EmptyFrictionInteraction() finished"<<std::endl;
#endif
}

void EmptyFrictionInteraction::write(std::ostream& os UNUSED) const
{}

void EmptyFrictionInteraction::read(std::istream& is UNUSED)
{}

void EmptyFrictionInteraction::computeForce()
{}

void EmptyFrictionInteraction::integrate(Mdouble timeStep UNUSED)
{}

Mdouble EmptyFrictionInteraction::getElasticEnergy() const
{
    return 0.0;
}

Mdouble EmptyFrictionInteraction::getTangentialOverlap() const
{
    return 0;
}

const Vec3D EmptyFrictionInteraction::getTangentialForce() const
{
    return Vec3D(0.0,0.0,0.0);
}

const EmptyFrictionSpecies *EmptyFrictionInteraction::getSpecies() const
{
    return dynamic_cast<const EmptyFrictionSpecies *>(getBaseSpecies());
}

std::string EmptyFrictionInteraction::getName() const
{
    return "";
}
