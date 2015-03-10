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

#ifndef MIXEDSPECIES_H
#define MIXEDSPECIES_H
#include "Species/FrictionForceSpecies/EmptyFrictionSpecies.h"
#include "Species/AdhesiveForceSpecies/EmptyAdhesiveSpecies.h"
#include "Interactions/Interaction.h"
class BaseInteraction;
//class EmptyFrictionSpecies;
//class EmptyAdhesiveSpecies;
//template<class NormalForceInteraction, class FrictionForceInteraction> class Interaction;

template<class NormalForceSpecies, class FrictionForceSpecies=EmptyFrictionSpecies, class AdhesiveForceSpecies=EmptyAdhesiveSpecies>
class MixedSpecies : public NormalForceSpecies, public FrictionForceSpecies, public AdhesiveForceSpecies
{
public:
    MixedSpecies();
    MixedSpecies(const MixedSpecies &s);
    MixedSpecies(const Species<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies> &s);
    virtual ~MixedSpecies();
    MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>* copy() const;
    /// Called by SpeciesHandler::readObject
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
MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::MixedSpecies()
: BaseSpecies(), NormalForceSpecies(), FrictionForceSpecies(), AdhesiveForceSpecies()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"MixedSpecies::MixedSpecies() finished"<<std::endl;
#endif
}

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::MixedSpecies(const MixedSpecies &s)
: BaseSpecies(s), NormalForceSpecies(s), FrictionForceSpecies(s), AdhesiveForceSpecies(s)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"MixedSpecies::MixedSpecies(const MixedSpecies &p) finished"<<std::endl;
#endif
}

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::MixedSpecies(const Species<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies> &s)
: BaseSpecies(s), NormalForceSpecies(s), FrictionForceSpecies(s), AdhesiveForceSpecies(s)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"MixedSpecies::MixedSpecies(const MixedSpecies &p) finished"<<std::endl;
#endif
}

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::~MixedSpecies()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"MixedSpecies::~MixedSpecies() finished"<<std::endl;
#endif
}

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
void MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::clear()
{
    std::cout << "MixedSpecies::clear(), this function shouldn't be called" << std::endl;
}

///MixedSpecies copy method. It calls to copy constructor of this MixedSpecies, useful for polymorphism
template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>* MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::copy() const
{
    return new MixedSpecies(*this);
}

///MixedSpecies print function, which accepts an os std::stringstream as input. It prints human readable MixedSpecies information to the std::stringstream
template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
void MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::write(std::ostream& os) const
{
    os << getName();
    os << " idA " << BaseObject::getId();
    os << " idB " << BaseObject::getIndex();
    NormalForceSpecies::write(os);
    FrictionForceSpecies::write(os);
    AdhesiveForceSpecies::write(os);
}

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
void MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::read(std::istream& is)
{
    //note: name is already read by SpeciesHandler::readObject
    std::string dummy;
    unsigned int id, index;
    is >> dummy >> id;
    is >> dummy >> index;
    BaseObject::setId(id);
    BaseObject::setIndex(index);
    NormalForceSpecies::read(is);
    FrictionForceSpecies::read(is);
    AdhesiveForceSpecies::read(is);
}

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
std::string MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::getBaseName() const
{
    return NormalForceSpecies::getBaseName()
        + FrictionForceSpecies::getBaseName()
        + AdhesiveForceSpecies::getBaseName();
}

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
std::string MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::getName() const
{
    return getBaseName() + "MixedSpecies";
}

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
BaseInteraction* MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
{
    return new Interaction<typename NormalForceSpecies::InteractionType, typename FrictionForceSpecies::InteractionType, typename AdhesiveForceSpecies::InteractionType>(P, I, timeStamp);
}

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
bool MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::getUseAngularDOFs() const
{
    return FrictionForceSpecies::getUseAngularDOFs();
}

///create values for mixed MixedSpecies
template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
void MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::mix(BaseSpecies* const S, BaseSpecies* const T)
{
    NormalForceSpecies::mix(S, T);
    FrictionForceSpecies::mix(S, T);
    AdhesiveForceSpecies::mix(S, T);
}

///Returns the particle distance below which adhesive forces can occur (needed for contact detection)
template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
Mdouble MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::getInteractionDistance() const
{
    return AdhesiveForceSpecies::getInteractionDistance();
}
#endif
