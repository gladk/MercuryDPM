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

#include "EmptyTangentialInteraction.h"
#include "EmptyAdhesiveInteraction.h"
#include "InteractionHandler.h"
#include "BaseInteractable.h"
class BaseInteractable;
template<class NormalForceSpecies, class TangentialForceSpecies, class AdhesiveForceSpecies> class Species;

//this class combines normal and tangential force laws
template<class NormalForceInteraction, class TangentialForceInteraction=EmptyTangentialInteraction, class AdhesiveForceInteraction=EmptyAdhesiveInteraction>
class Interaction : public NormalForceInteraction, public TangentialForceInteraction, public AdhesiveForceInteraction
{
public:
    Interaction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);

    Interaction(const Interaction &p);

    virtual ~Interaction();

    ///Interaction copy method. It calls to copy constructor of this Interaction, useful for polymorphism
    Interaction<NormalForceInteraction, TangentialForceInteraction, AdhesiveForceInteraction>* copy() const;

    //PI->P, PJ->I
    void computeForce();

    ///Interaction read function, which accepts an std::stringstream as input.
    void read(std::istream& is);

    ///Interaction print function, which accepts an std::stringstream as input.
    void write(std::ostream& os) const;

    std::string getName() const;

    Mdouble getElasticEnergy() const;

    //const Species<NormalForceInteraction::SpeciesType, TangentialForceInteraction::SpeciesType>* getSpecies() const;

    void integrate(Mdouble timeStep);

    void reverseHistory();
};

template<class NormalForceInteraction, class TangentialForceInteraction, class AdhesiveForceInteraction>
Interaction<NormalForceInteraction, TangentialForceInteraction, AdhesiveForceInteraction>::Interaction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
: BaseInteraction(P, I, timeStamp), NormalForceInteraction(P, I, timeStamp), TangentialForceInteraction(P, I, timeStamp), AdhesiveForceInteraction(P, I, timeStamp)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"Interaction::Interaction() finished"<<std::endl;
#endif
}

template<class NormalForceInteraction, class TangentialForceInteraction, class AdhesiveForceInteraction>
Interaction<NormalForceInteraction, TangentialForceInteraction, AdhesiveForceInteraction>::Interaction(const Interaction &p)
: BaseInteraction(p), NormalForceInteraction(p), TangentialForceInteraction(p), AdhesiveForceInteraction(p)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"Interaction::Interaction(const Interaction &p finished"<<std::endl;
#endif
}

template<class NormalForceInteraction, class TangentialForceInteraction, class AdhesiveForceInteraction>
Interaction<NormalForceInteraction, TangentialForceInteraction, AdhesiveForceInteraction>::~Interaction()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"Interaction::~Interaction() finished"<<std::endl;
#endif
}

template<class NormalForceInteraction, class TangentialForceInteraction, class AdhesiveForceInteraction>
Interaction<NormalForceInteraction, TangentialForceInteraction, AdhesiveForceInteraction>* Interaction<NormalForceInteraction, TangentialForceInteraction, AdhesiveForceInteraction>::copy() const
{
    return new Interaction(*this);
}

///BaseParticle print function, which accepts an os std::stringstream as input. It prints human readable BaseParticle information to the std::stringstream
template<class NormalForceInteraction, class TangentialForceInteraction, class AdhesiveForceInteraction>
void Interaction<NormalForceInteraction, TangentialForceInteraction, AdhesiveForceInteraction>::write(std::ostream& os) const
{
    NormalForceInteraction::write(os);
    TangentialForceInteraction::write(os);
    AdhesiveForceInteraction::write(os);
}

template<class NormalForceInteraction, class TangentialForceInteraction, class AdhesiveForceInteraction>
void Interaction<NormalForceInteraction, TangentialForceInteraction, AdhesiveForceInteraction>::read(std::istream& is)
{
    NormalForceInteraction::read(is);
    TangentialForceInteraction::read(is);
    AdhesiveForceInteraction::read(is);
}


template<class NormalForceInteraction, class TangentialForceInteraction, class AdhesiveForceInteraction>
void Interaction<NormalForceInteraction, TangentialForceInteraction, AdhesiveForceInteraction>::reverseHistory()
{
    NormalForceInteraction::reverseHistory();
    TangentialForceInteraction::reverseHistory();
    AdhesiveForceInteraction::reverseHistory();
}

template<class NormalForceInteraction, class TangentialForceInteraction, class AdhesiveForceInteraction>
std::string Interaction<NormalForceInteraction, TangentialForceInteraction, AdhesiveForceInteraction>::getName() const
{
    return NormalForceInteraction::getName() + TangentialForceInteraction::getName() + AdhesiveForceInteraction::getName() + "Interaction";
}

template<class NormalForceInteraction, class TangentialForceInteraction, class AdhesiveForceInteraction>
void Interaction<NormalForceInteraction, TangentialForceInteraction, AdhesiveForceInteraction>::integrate(Mdouble timeStep)
{
    TangentialForceInteraction::integrate(timeStep);
}

template<class NormalForceInteraction, class TangentialForceInteraction, class AdhesiveForceInteraction>
void Interaction<NormalForceInteraction, TangentialForceInteraction, AdhesiveForceInteraction>::computeForce()
{
    NormalForceInteraction::computeForce();
    TangentialForceInteraction::computeForce();
    AdhesiveForceInteraction::computeForce();
}

template<class NormalForceInteraction, class TangentialForceInteraction, class AdhesiveForceInteraction>
Mdouble Interaction<NormalForceInteraction, TangentialForceInteraction, AdhesiveForceInteraction>::getElasticEnergy() const
{
    return NormalForceInteraction::getElasticEnergy() + TangentialForceInteraction::getElasticEnergy() + AdhesiveForceInteraction::getElasticEnergy();
}


//template<class NormalForceInteraction, class TangentialForceInteraction, class AdhesiveForceInteraction>
//const Species<NormalForceInteraction::SpeciesType, TangentialForceInteraction::SpeciesType>* Interaction::getSpecies() const
//{
//    return dynamic_cast<const Species<NormalForceInteraction::SpeciesType, TangentialForceInteraction::SpeciesType>*>(getBaseSpecies());
//}
#endif
