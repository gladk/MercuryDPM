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

#ifndef INTERACTION_H
#define INTERACTION_H

#include "FrictionForceInteractions/EmptyFrictionInteraction.h"
#include "AdhesiveForceInteractions/EmptyAdhesiveInteraction.h"
#include "InteractionHandler.h"
#include "BaseInteractable.h"
class BaseInteractable;
template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies> class Species;

//this class combines normal and tangential force laws
template<class NormalForceInteraction, class FrictionForceInteraction=EmptyFrictionInteraction, class AdhesiveForceInteraction=EmptyAdhesiveInteraction>
class Interaction : public NormalForceInteraction, public FrictionForceInteraction, public AdhesiveForceInteraction
{
public:
    Interaction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);

    Interaction(const Interaction &p);

    virtual ~Interaction();

    ///Interaction copy method. It calls to copy constructor of this Interaction, useful for polymorphism
    Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>* copy() const;

    //PI->P, PJ->I
    void computeForce();

    ///Interaction read function, which accepts an std::stringstream as input.
    void read(std::istream& is);

    ///Interaction print function, which accepts an std::stringstream as input.
    void write(std::ostream& os) const;

    std::string getName() const;

    Mdouble getElasticEnergy() const;

    //const Species<NormalForceInteraction::SpeciesType, FrictionForceInteraction::SpeciesType>* getSpecies() const;

    void integrate(Mdouble timeStep);

    void reverseHistory();
};

template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::Interaction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
: BaseInteraction(P, I, timeStamp), NormalForceInteraction(P, I, timeStamp), FrictionForceInteraction(P, I, timeStamp), AdhesiveForceInteraction(P, I, timeStamp)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"Interaction::Interaction() finished"<<std::endl;
#endif
}

template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::Interaction(const Interaction &p)
: BaseInteraction(p), NormalForceInteraction(p), FrictionForceInteraction(p), AdhesiveForceInteraction(p)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"Interaction::Interaction(const Interaction &p finished"<<std::endl;
#endif
}

template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::~Interaction()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"Interaction::~Interaction() finished"<<std::endl;
#endif
}

template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>* Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::copy() const
{
    return new Interaction(*this);
}

///BaseParticle print function, which accepts an os std::stringstream as input. It prints human readable BaseParticle information to the std::stringstream
template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
void Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::write(std::ostream& os) const
{
    NormalForceInteraction::write(os);
    FrictionForceInteraction::write(os);
    AdhesiveForceInteraction::write(os);
}

template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
void Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::read(std::istream& is)
{
    NormalForceInteraction::read(is);
    FrictionForceInteraction::read(is);
    AdhesiveForceInteraction::read(is);
}


template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
void Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::reverseHistory()
{
    NormalForceInteraction::reverseHistory();
    FrictionForceInteraction::reverseHistory();
    AdhesiveForceInteraction::reverseHistory();
}

template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
std::string Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::getName() const
{
    return NormalForceInteraction::getName() + FrictionForceInteraction::getName() + AdhesiveForceInteraction::getName() + "Interaction";
}

template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
void Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::integrate(Mdouble timeStep)
{
    FrictionForceInteraction::integrate(timeStep);
}

template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
void Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::computeForce()
{
    NormalForceInteraction::computeForce();
    FrictionForceInteraction::computeForce();
    AdhesiveForceInteraction::computeForce();
}

template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
Mdouble Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::getElasticEnergy() const
{
    return NormalForceInteraction::getElasticEnergy() + FrictionForceInteraction::getElasticEnergy() + AdhesiveForceInteraction::getElasticEnergy();
}


//template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
//const Species<NormalForceInteraction::SpeciesType, FrictionForceInteraction::SpeciesType>* Interaction::getSpecies() const
//{
//    return dynamic_cast<const Species<NormalForceInteraction::SpeciesType, FrictionForceInteraction::SpeciesType>*>(getBaseSpecies());
//}
#endif
