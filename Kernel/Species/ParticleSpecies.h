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

#ifndef ParticleSpecies_H
#define ParticleSpecies_H
#include "BaseSpecies.h"
#include "Math/ExtendedMath.h"
#include "Interactions/BaseInteraction.h"
//class SpeciesHandler;
class BaseInteractable;
class BaseInteraction;

//Note the getVelocity can for some Species be dependent on which point on the Species is meant.
class ParticleSpecies : public virtual BaseSpecies
{
public:
    typedef BaseInteraction InteractionType;

    ParticleSpecies();
    ParticleSpecies(const ParticleSpecies &p);
    virtual ~ParticleSpecies();
    ParticleSpecies* copy() const=0;
    virtual BaseSpecies* copyMixed() const=0;
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void clear();
    std::string getBaseName() const;
    BaseInteraction* getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);

    ///Allows the density to be changed
    ///\todo recalculate masses when setting dim_particle or rho
    void setDensity(Mdouble density);

    ///\todo TW: should getMassFromRadius be removed?
    Mdouble getMassFromRadius(const Mdouble radius);

    ///Allows the density to be accessed
    Mdouble getDensity() const;

private:
    Mdouble density_;
};
#endif
