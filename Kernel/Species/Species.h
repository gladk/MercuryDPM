//Copyright (c) 2013-2014, The MercuryDPM Developers Team. All rights reserved.
//For the list of developers, see <http://www.MercuryDPM.org/Team>.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name MercuryDPM nor the
//    names of its contributors may be used to endorse or promote products
//    derived from this software without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//DISCLAIMED. IN NO EVENT SHALL THE MERCURYDPM DEVELOPERS TEAM BE LIABLE FOR ANY
//DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
//(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
//ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
//(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

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
