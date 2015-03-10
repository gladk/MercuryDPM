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

#include "IrreversibleAdhesiveInteraction.h"
#include "Species/AdhesiveForceSpecies/IrreversibleAdhesiveSpecies.h"
#include "Particles/BaseParticle.h"
#include "InteractionHandler.h"
#include <iomanip>
#include <fstream>

IrreversibleAdhesiveInteraction::IrreversibleAdhesiveInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
    : BaseInteraction(P, I, timeStamp),  ReversibleAdhesiveInteraction(P, I, timeStamp)
{
    wasInContact_=false;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"IrreversibleAdhesiveInteraction::IrreversibleAdhesiveInteraction() finished"<<std::endl;
#endif
}

IrreversibleAdhesiveInteraction::IrreversibleAdhesiveInteraction(const IrreversibleAdhesiveInteraction &p)
    : BaseInteraction(p),  ReversibleAdhesiveInteraction(p)
{
    wasInContact_=p.wasInContact_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"IrreversibleAdhesiveInteraction::IrreversibleAdhesiveInteraction(const IrreversibleAdhesiveInteraction &p finished"<<std::endl;
#endif
}

IrreversibleAdhesiveInteraction::~IrreversibleAdhesiveInteraction()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"IrreversibleAdhesiveInteraction::~IrreversibleAdhesiveInteraction() finished"<<std::endl;
#endif
}

void IrreversibleAdhesiveInteraction::write(std::ostream& os) const
{
    os << " wasInContact " << wasInContact_;
}

void IrreversibleAdhesiveInteraction::read(std::istream& is)
{
    std::string dummy;
    is >> dummy >> wasInContact_;
}

void IrreversibleAdhesiveInteraction::computeForce()
{
    const IrreversibleAdhesiveSpecies* species = getSpecies();
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

const IrreversibleAdhesiveSpecies *IrreversibleAdhesiveInteraction::getSpecies() const
{
    return dynamic_cast<const IrreversibleAdhesiveSpecies *>(getBaseSpecies());
}

std::string IrreversibleAdhesiveInteraction::getName() const
{
    return "IrreversibleAdhesive";
}
