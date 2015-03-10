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

#ifndef BASESPECIES_H
#define BASESPECIES_H
#include "BaseObject.h"
#include "Math/ExtendedMath.h"
#include "Math/Helpers.h"

class SpeciesHandler;
//class BaseParticle;
class BaseParticle;
class BaseInteractable;
class BaseInteraction;

//Note the getVelocity can for some Species be dependent on which point on the Species is meant.
class BaseSpecies : public BaseObject
{
public:
    BaseSpecies();
    BaseSpecies(const BaseSpecies &p);
    virtual ~BaseSpecies();
    virtual BaseSpecies* copy() const=0;
    virtual void read(std::istream& is);
    virtual void write(std::ostream& os) const;
    virtual void clear();
    void setHandler(SpeciesHandler* handler);
    SpeciesHandler* getHandler() const;
    virtual std::string getBaseName() const;
    virtual std::string getName() const;

// Species-specific functions

    Mdouble average(Mdouble a, Mdouble b);

    ///create values for mixed species
    void mix(BaseSpecies* const S, BaseSpecies* const T);

    ///\todo TW: should getMassFromRadius be removed?
    Mdouble getMassFromRadius(const Mdouble radius);

    ///Returns the particle distance below which adhesive forces can occur (needed for contact detection)
    virtual Mdouble getInteractionDistance() const;

//setters and getters

    ///Allows the density to be changed
    ///\todo recalculate masses when setting dim_particle or rho
    void setDensity(Mdouble density);

    ///Allows the density to be accessed
    Mdouble getDensity() const;

    ///Allows the dimension of the particle (f.e. for mass) to be accessed
    virtual bool getUseAngularDOFs() const = 0;

    virtual BaseInteraction* getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)=0;

private:
    SpeciesHandler* handler_;
    Mdouble density_; ///<particle density
};
#endif
