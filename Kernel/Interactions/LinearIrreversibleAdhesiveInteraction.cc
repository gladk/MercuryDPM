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

#include "LinearIrreversibleAdhesiveInteraction.h"
#include "Species/LinearIrreversibleAdhesiveSpecies.h"
#include "Particles/BaseParticle.h"
#include "InteractionHandler.h"
#include <iomanip>
#include <fstream>

LinearIrreversibleAdhesiveInteraction::LinearIrreversibleAdhesiveInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
    : BaseInteraction(P, I, timeStamp),  LinearReversibleAdhesiveInteraction(P, I, timeStamp)
{
    wasInContact_=false;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LinearIrreversibleAdhesiveInteraction::LinearIrreversibleAdhesiveInteraction() finished"<<std::endl;
#endif
}

LinearIrreversibleAdhesiveInteraction::LinearIrreversibleAdhesiveInteraction(const LinearIrreversibleAdhesiveInteraction &p)
    : BaseInteraction(p),  LinearReversibleAdhesiveInteraction(p)
{
    wasInContact_=p.wasInContact_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LinearIrreversibleAdhesiveInteraction::LinearIrreversibleAdhesiveInteraction(const LinearIrreversibleAdhesiveInteraction &p finished"<<std::endl;
#endif
}

LinearIrreversibleAdhesiveInteraction::~LinearIrreversibleAdhesiveInteraction()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LinearIrreversibleAdhesiveInteraction::~LinearIrreversibleAdhesiveInteraction() finished"<<std::endl;
#endif
}

void LinearIrreversibleAdhesiveInteraction::write(std::ostream& os) const
{
    os << " wasInContact " << wasInContact_;
}

void LinearIrreversibleAdhesiveInteraction::read(std::istream& is)
{
    std::string dummy;
    is >> dummy >> wasInContact_;
}

void LinearIrreversibleAdhesiveInteraction::computeForce()
{
    const LinearIrreversibleAdhesiveSpecies* species = getSpecies();
    if (getOverlap()>=0)
    {
        wasInContact_=true;
        addForce(getNormal() * (-species->getAdhesionForceMax()));
    }
    else if (wasInContact_)
    {
        addForce(getNormal() * (-species->getAdhesionStiffness() *getOverlap() - species->getAdhesionForceMax()));
    }
}

const LinearIrreversibleAdhesiveSpecies *LinearIrreversibleAdhesiveInteraction::getSpecies() const
{
    return dynamic_cast<const LinearIrreversibleAdhesiveSpecies *>(getBaseSpecies());
}

std::string LinearIrreversibleAdhesiveInteraction::getName() const
{
    return "LinearIrreversibleAdhesive";
}
