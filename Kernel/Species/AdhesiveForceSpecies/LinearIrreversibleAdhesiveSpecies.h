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

#ifndef LINEARIRREVERSIBLEADHESIVESPECIES_H
#define LINEARIRREVERSIBLEADHESIVESPECIES_H
#include "Species/BaseSpecies.h"
#include "Math/ExtendedMath.h"
#include "Interactions/AdhesiveForceInteractions/LinearIrreversibleAdhesiveInteraction.h"
#include "LinearReversibleAdhesiveSpecies.h"
class BaseInteractable;
class BaseInteraction;

//Note the getVelocity can for some Species be dependent on which point on the Species is meant.
class LinearIrreversibleAdhesiveSpecies : public LinearReversibleAdhesiveSpecies
{
public:
    typedef LinearIrreversibleAdhesiveInteraction InteractionType;
    LinearIrreversibleAdhesiveSpecies();
    LinearIrreversibleAdhesiveSpecies(const LinearIrreversibleAdhesiveSpecies &s);
    virtual ~LinearIrreversibleAdhesiveSpecies();
    std::string getBaseName() const;
};
#endif
