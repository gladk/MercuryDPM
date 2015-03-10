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
#ifdef DEBUG_DESTRUCTOR
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
