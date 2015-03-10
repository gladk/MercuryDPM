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

#ifndef LINEARREVERSIBLEADHESIVESPECIES_H
#define LINEARREVERSIBLEADHESIVESPECIES_H
#include "Species/BaseSpecies.h"
#include "Math/ExtendedMath.h"
#include "Interactions/AdhesiveForceInteractions/ReversibleAdhesiveInteraction.h"
class BaseInteractable;
class BaseInteraction;

//Note the getVelocity can for some Species be dependent on which point on the Species is meant.
class ReversibleAdhesiveSpecies : public virtual BaseSpecies
{
public:
    typedef ReversibleAdhesiveInteraction InteractionType;
    ReversibleAdhesiveSpecies();
    ReversibleAdhesiveSpecies(const ReversibleAdhesiveSpecies &s);
    virtual ~ReversibleAdhesiveSpecies();
    void read(std::istream& is);
    void write(std::ostream& os) const;
    std::string getBaseName() const;
    void mix(ReversibleAdhesiveSpecies* const S, ReversibleAdhesiveSpecies* const T);

//adhesion-specific functions

    ///Returns the particle distance below which adhesive forces can occur (needed for contact detection)
    Mdouble getInteractionDistance() const;

//setters and getters
    ///Allows the spring constant to be changed
    void setAdhesionStiffness(Mdouble new_k0);

    ///Allows the spring constant to be accessed
    Mdouble getAdhesionStiffness() const;

    ///Allows the spring constant to be changed
    void setAdhesionForceMax(Mdouble new_f0);

    ///Allows the spring constant to be accessed
    Mdouble getAdhesionForceMax() const;

private:
    Mdouble adhesionStiffness_; ///<stiffness of linear adhesion force
    Mdouble adhesionForceMax_; ///<adhesion force at zero overlap
    
};
#endif
