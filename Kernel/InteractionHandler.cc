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

#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include "Particles/BaseParticle.h"
#include "InteractionHandler.h"
#include "SpeciesHandler.h"
#include "Species/BaseSpecies.h"
#include "Interactions/BaseInteraction.h"

///Constructor of the ParticleHandler class. It creates and empty ParticleHandler.
InteractionHandler::InteractionHandler()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "InteractionHandler::InteractionHandler() finished" << std::endl;
#endif
}

/// \param[in] BH The InteractionHandler that has to be copied.
InteractionHandler::InteractionHandler(const InteractionHandler &IH UNUSED)
        : BaseHandler()
{
    //By default interactions are not copied.
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "InteractionHandler::InteractionHandler(const InteractionHandler &IH) finished" << std::endl;
#endif
}

/// \param[in] rhs The BoundaryHandler on the right hand side of the assignment.
InteractionHandler InteractionHandler::operator =(const InteractionHandler& rhs)
{
    if (this != &rhs)
    {
        clear();
    }
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "InteractionHandler InteractionHandler::operator =(const InteractionHandler& rhs)" << std::endl;
#endif
    return *this;
}

InteractionHandler::~InteractionHandler()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout << "InteractionHandler::~InteractionHandler() finished" << std::endl;
#endif
}

/// \param[in] P A pointer to the BaseInteraction (or derived class) that has to be added.
void InteractionHandler::addObject(BaseInteraction* I)
{
    //Puts the particle in the Particle list
    BaseHandler<BaseInteraction>::addObject(I);
    //set the particleHandler pointer
    I->setHandler(this);
}

void InteractionHandler::readObject(std::istream& is UNUSED)
{
    std::cerr << "Error in InteractionHandler::readObject" << std::endl;
    exit(-1);
}

BaseInteraction* InteractionHandler::getExistingInteraction(BaseInteractable* P, BaseInteractable* I)
{
    //for particle-particle collision it is assumed BaseInteractable P has a lower index then I, so we only have to check for I, not P
    for (std::list<BaseInteraction*>::const_iterator it = P->getInteractions().begin(); it != P->getInteractions().end(); ++it)
    {
        if ((*it)->getI() == I)
        {
            return *it;
        }
    }
    return 0;
}

BaseInteraction* InteractionHandler::getInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
{
    BaseSpecies* species = speciesHandler_->getMixedObject(P->getIndSpecies(),I->getIndSpecies());

    //std::cout << "Trying to reconnect to BaseInteraction between P=" << P->getId() << " and " << I->getId() << std::endl;
    BaseInteraction* C = getExistingInteraction(P, I);
    if (!C)
    {
        C = species->getNewInteraction(P, I, timeStamp);
        addObject(C);
        //std::cout << "Creating new interaction with index=" << getLastObject()->getIndex() << " id=" << getLastObject()->getId() << std::endl;
    }

    //set timeStamp
    C->setTimeStamp(timeStamp);

    //set species of collision
    C->setSpecies(species);

    return C;
}

void InteractionHandler::removeObjectKeepingPeriodics(unsigned const int id)
{
    BaseInteraction* iMain = getObject(id);
    
    //std::cout<<"InteractionHandler::removeObject("<<id<<") P="<<iMain->getP()->getId()<<" I="<<iMain->getI()->getId()<<std::endl;

    // When deleting an interaction between object where one of them is periodic becomes difficult, because its interaction information has to be saved
    // Now when an interaction is removed the periodic particle has to be the I pointer
    // So when an interaction is removed where P is normal and I is periodic, and the information is new it will be transfered when the index of P is lower then the index of the real particle of I. 
    
    BaseParticle* P = dynamic_cast<BaseParticle*>(iMain->getP());
    BaseParticle* I = dynamic_cast<BaseParticle*>(iMain->getI());
    if (P && I)
    {
        //std::cout<<"Both are particles"<<std::endl;
        BaseInteractable* realI = I->getPeriodicFromParticle();
        if (realI && !P->getPeriodicFromParticle())
        {
            //std::cout << "P is normal and I periodic P=" << P->getId() << " I=" << I->getId() << " I real=" << I->getPeriodicFromParticle()->getId() << std::endl;
            if (P->getIndex() < realI->getIndex())
            {
                //std::cout << "Reconnecting to spring" << std::endl;
                BaseInteraction* iOther = getExistingInteraction(P, realI);
                if (iOther)
                {
                    //std::cout << "Found" << std::endl;
                    if (iOther->getTimeStamp() < iMain->getTimeStamp())
                    {
                        //std::cout << "Switching" << std::endl;
                        iMain->setI(realI);
                        removeObject(iOther->getIndex());
                        return;
                    }
                    else
                    {
                        //std::cout << "Not switching" << std::endl;
                        BaseHandler<BaseInteraction>::removeObject(id);
                        return;
                    }
                }
                else
                {
                    //std::cout << "Not found, so just move" << std::endl;
                    iMain->setI(realI);
                    return;
                }
            }
        }
    }
    BaseHandler<BaseInteraction>::removeObject(id);
}

void InteractionHandler::eraseOldInteractions(Mdouble lastTimeStep)
{
    //std::cout<<"void InteractionHandler::eraseOldInteractions(Mdouble lastTimeStep)"<<std::endl;
    //std::cout<<"Current interactions="<<getNumberOfObjects()<<std::endl;
    //Remove_if reconstructs the vector with only elements passing the check_spring_time function
    //Erase removes the end of the vector
    ///\todo TW: this function has to be sped up with sth like this: erase(remove_if(begin(), end(), bind2nd(checkSpringTime(), lastTimeStep)), end());
    for (unsigned int id = 0; id < getNumberOfObjects(); id++)
    {
        if (getObject(id)->getTimeStamp() < lastTimeStep)
        {
            removeObject(id);
            --id;
        }
    }
}

void InteractionHandler::setSpecies(SpeciesHandler* specieshandler)
{
    speciesHandler_ = specieshandler;
}


std::string InteractionHandler::getName() const
{
    return "InteractionHandler";
}
