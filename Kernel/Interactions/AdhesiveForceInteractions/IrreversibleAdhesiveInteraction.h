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

#ifndef LINEARIRREVERSIBLEADHESIVEINTERACTION_H
#define LINEARIRREVERSIBLEADHESIVEINTERACTION_H

#include "ReversibleAdhesiveInteraction.h"
#include "Math/Vector.h"
class BaseParticle;
class IrreversibleAdhesiveSpecies;
class BaseInteractable;

class IrreversibleAdhesiveInteraction : public ReversibleAdhesiveInteraction
{
public:
    typedef IrreversibleAdhesiveSpecies SpeciesType;
    IrreversibleAdhesiveInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);
    IrreversibleAdhesiveInteraction(const IrreversibleAdhesiveInteraction &p);
    virtual ~IrreversibleAdhesiveInteraction();

    void computeForce();

    ///Interaction read function, which accepts an std::stringstream as input.
    void read(std::istream& is);

    ///Interaction print function, which accepts an std::stringstream as input.
    void write(std::ostream& os) const;

    const IrreversibleAdhesiveSpecies* getSpecies() const;

    std::string getName() const;

private:
    bool wasInContact_;
};
#endif
