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

#ifndef SPECIES_H
#define SPECIES_H
#include "MixedSpecies.h"
#include "ParticleSpecies.h"
#include "Species/FrictionForceSpecies/EmptyFrictionSpecies.h"
#include "Species/AdhesiveForceSpecies/EmptyAdhesiveSpecies.h"
#include "Interactions/Interaction.h"
class BaseInteraction;
//class EmptyFrictionSpecies;
//class EmptyAdhesiveSpecies;
//template<class NormalForceInteraction, class FrictionForceInteraction> class Interaction;

template<class NormalForceSpecies, class FrictionForceSpecies=EmptyFrictionSpecies, class AdhesiveForceSpecies=EmptyAdhesiveSpecies>
class Species : public ParticleSpecies, public NormalForceSpecies, public FrictionForceSpecies, public AdhesiveForceSpecies
{
public:
    typedef MixedSpecies<NormalForceSpecies,FrictionForceSpecies,AdhesiveForceSpecies> MixedSpeciesType;

    Species();
    Species(const Species &s);
    virtual ~Species();
    Species<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>* copy() const;
    MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>* copyMixed() const;
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void clear();
    std::string getBaseName() const;
    std::string getName() const;
    BaseInteraction* getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);
    bool getUseAngularDOFs() const;
    void mix(BaseSpecies* const S, BaseSpecies* const T);
    ///Returns the particle distance below which adhesive forces can occur (needed for contact detection)
    Mdouble getInteractionDistance() const;
};

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
Species<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::Species()
: ParticleSpecies(), NormalForceSpecies(), FrictionForceSpecies(), AdhesiveForceSpecies()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"Species::Species() finished"<<std::endl;
#endif
}

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
Species<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::Species(const Species &s)
: ParticleSpecies(s), NormalForceSpecies(s), FrictionForceSpecies(s), AdhesiveForceSpecies(s)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"Species::Species(const Species &p) finished"<<std::endl;
#endif
}

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
Species<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::~Species()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"Species::~Species() finished"<<std::endl;
#endif
}

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
void Species<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::clear()
{
    std::cout << "Species::clear(), this function shouldn't be called" << std::endl;
}

///Species copy method. It calls to copy constructor of this Species, useful for polymorphism
template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
Species<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>* Species<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::copy() const
{
    return new Species(*this);
}

///Species copy method. It calls to copy constructor of this Species, useful for polymorphism
template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>* Species<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::copyMixed() const
{
    return new MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>(*this);
}

///Species print function, which accepts an os std::stringstream as input. It prints human readable Species information to the std::stringstream
template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
void Species<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::write(std::ostream& os) const
{
    ParticleSpecies::write(os);
    NormalForceSpecies::write(os);
    FrictionForceSpecies::write(os);
    AdhesiveForceSpecies::write(os);
}

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
void Species<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::read(std::istream& is)
{
    ParticleSpecies::read(is);
    NormalForceSpecies::read(is);
    FrictionForceSpecies::read(is);
    AdhesiveForceSpecies::read(is);
}

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
std::string Species<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::getBaseName() const
{
    return NormalForceSpecies::getBaseName()
        + FrictionForceSpecies::getBaseName()
        + AdhesiveForceSpecies::getBaseName();
}

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
std::string Species<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::getName() const
{
    return getBaseName() + "Species";
}

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
BaseInteraction* Species<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
{
    return new Interaction<typename NormalForceSpecies::InteractionType, typename FrictionForceSpecies::InteractionType, typename AdhesiveForceSpecies::InteractionType>(P, I, timeStamp);
}

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
bool Species<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::getUseAngularDOFs() const
{
    return FrictionForceSpecies::getUseAngularDOFs();
}

///create values for mixed species
template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
void Species<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::mix(BaseSpecies* const S, BaseSpecies* const T)
{
    NormalForceSpecies::mix(S, T);
    FrictionForceSpecies::mix(S, T);
    AdhesiveForceSpecies::mix(S, T);
}

///Returns the particle distance below which adhesive forces can occur (needed for contact detection)
template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
Mdouble Species<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::getInteractionDistance() const
{
    return AdhesiveForceSpecies::getInteractionDistance();
}
#endif
