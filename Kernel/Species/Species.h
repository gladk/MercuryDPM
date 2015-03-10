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
#include "Species/EmptyTangentialSpecies.h"
#include "Species/EmptyAdhesiveSpecies.h"
#include "Interactions/Interaction.h"
class BaseInteraction;
//template<class NormalForceInteraction, class TangentialForceInteraction> class Interaction;

template<class NormalForceSpecies, class TangentialForceSpecies=EmptyTangentialSpecies, class AdhesiveForceSpecies=EmptyAdhesiveSpecies>
class Species : public NormalForceSpecies, public TangentialForceSpecies, public AdhesiveForceSpecies
{
public:
    Species();
    Species(const Species &s);
    virtual ~Species();
    Species<NormalForceSpecies, TangentialForceSpecies, AdhesiveForceSpecies>* copy() const;
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

template<class NormalForceSpecies, class TangentialForceSpecies, class AdhesiveForceSpecies>
Species<NormalForceSpecies, TangentialForceSpecies, AdhesiveForceSpecies>::Species()
: BaseSpecies(), NormalForceSpecies(), TangentialForceSpecies(), AdhesiveForceSpecies()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"Species::Species() finished"<<std::endl;
#endif
}

template<class NormalForceSpecies, class TangentialForceSpecies, class AdhesiveForceSpecies>
Species<NormalForceSpecies, TangentialForceSpecies, AdhesiveForceSpecies>::Species(const Species &s)
: BaseSpecies(s), NormalForceSpecies(s), TangentialForceSpecies(s), AdhesiveForceSpecies(s)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"Species::Species(const Species &p) finished"<<std::endl;
#endif
}

template<class NormalForceSpecies, class TangentialForceSpecies, class AdhesiveForceSpecies>
Species<NormalForceSpecies, TangentialForceSpecies, AdhesiveForceSpecies>::~Species()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"Species::~Species() finished"<<std::endl;
#endif
}

template<class NormalForceSpecies, class TangentialForceSpecies, class AdhesiveForceSpecies>
void Species<NormalForceSpecies, TangentialForceSpecies, AdhesiveForceSpecies>::clear()
{
    std::cout << "Species::clear(), this function shouldn't be called" << std::endl;
}

///Species copy method. It calls to copy constructor of this Species, useful for polymorphism
template<class NormalForceSpecies, class TangentialForceSpecies, class AdhesiveForceSpecies>
Species<NormalForceSpecies, TangentialForceSpecies, AdhesiveForceSpecies>* Species<NormalForceSpecies, TangentialForceSpecies, AdhesiveForceSpecies>::copy() const
{
    return new Species(*this);
}

///Species print function, which accepts an os std::stringstream as input. It prints human readable Species information to the std::stringstream
template<class NormalForceSpecies, class TangentialForceSpecies, class AdhesiveForceSpecies>
void Species<NormalForceSpecies, TangentialForceSpecies, AdhesiveForceSpecies>::write(std::ostream& os) const
{
    NormalForceSpecies::write(os);
    TangentialForceSpecies::write(os);
    AdhesiveForceSpecies::write(os);
}

template<class NormalForceSpecies, class TangentialForceSpecies, class AdhesiveForceSpecies>
void Species<NormalForceSpecies, TangentialForceSpecies, AdhesiveForceSpecies>::read(std::istream& is)
{
    NormalForceSpecies::read(is);
    TangentialForceSpecies::read(is);
    AdhesiveForceSpecies::read(is);
}

template<class NormalForceSpecies, class TangentialForceSpecies, class AdhesiveForceSpecies>
std::string Species<NormalForceSpecies, TangentialForceSpecies, AdhesiveForceSpecies>::getBaseName() const
{
    return NormalForceSpecies::getBaseName()
        + TangentialForceSpecies::getBaseName()
        + AdhesiveForceSpecies::getBaseName();
}

template<class NormalForceSpecies, class TangentialForceSpecies, class AdhesiveForceSpecies>
std::string Species<NormalForceSpecies, TangentialForceSpecies, AdhesiveForceSpecies>::getName() const
{
    return getBaseName() + "Species";
}

template<class NormalForceSpecies, class TangentialForceSpecies, class AdhesiveForceSpecies>
BaseInteraction* Species<NormalForceSpecies, TangentialForceSpecies, AdhesiveForceSpecies>::getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
{
    return new Interaction<typename NormalForceSpecies::InteractionType, typename TangentialForceSpecies::InteractionType, typename AdhesiveForceSpecies::InteractionType>(P, I, timeStamp);
}

template<class NormalForceSpecies, class TangentialForceSpecies, class AdhesiveForceSpecies>
bool Species<NormalForceSpecies, TangentialForceSpecies, AdhesiveForceSpecies>::getUseAngularDOFs() const
{
    return TangentialForceSpecies::getUseAngularDOFs();
}

///create values for mixed species
template<class NormalForceSpecies, class TangentialForceSpecies, class AdhesiveForceSpecies>
void Species<NormalForceSpecies, TangentialForceSpecies, AdhesiveForceSpecies>::mix(BaseSpecies* const S, BaseSpecies* const T)
{
    NormalForceSpecies::mix(S, T);
    TangentialForceSpecies::mix(S, T);
    AdhesiveForceSpecies::mix(S, T);
}

///Returns the particle distance below which adhesive forces can occur (needed for contact detection)
template<class NormalForceSpecies, class TangentialForceSpecies, class AdhesiveForceSpecies>
Mdouble Species<NormalForceSpecies, TangentialForceSpecies, AdhesiveForceSpecies>::getInteractionDistance() const
{
    return AdhesiveForceSpecies::getInteractionDistance();
}
#endif
