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


#include <iostream>
#include <iomanip>
#include <string>
#include <stdlib.h>
#include "Particles/BaseParticle.h"
#include "InteractionHandler.h"
#include "SpeciesHandler.h"
#include "Species/BaseSpecies.h"
#include "Interactions/BaseInteraction.h"
#include "DPMBase.h"

///Constructor of the ParticleHandler class. It creates and empty ParticleHandler.
InteractionHandler::InteractionHandler()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "InteractionHandler::InteractionHandler() finished" << std::endl;
#endif
}

/// \param[in] BH The InteractionHandler that has to be copied.
InteractionHandler::InteractionHandler(const InteractionHandler &IH UNUSED)
        : BaseHandler()
{
    //By default interactions are not copied.
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "InteractionHandler::InteractionHandler(const InteractionHandler &IH) finished" << std::endl;
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
    std::cout << "InteractionHandler::operator =(const InteractionHandler& rhs)" << std::endl;
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
    BaseSpecies* species = getDPMBase()->speciesHandler.getMixedObject(P->getIndSpecies(),I->getIndSpecies());

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

std::string InteractionHandler::getName() const
{
    return "InteractionHandler";
}

void InteractionHandler::write(std::ostream& os) const
{
    os << "Interactions " << getNumberOfObjects() << std::endl;
    for (std::vector<BaseInteraction*>::const_iterator it = begin(); it != end(); ++it)
        os << (**it) << std::endl;
}

/// \param[in] is The input stream from which the information is read.
void InteractionHandler::readObject(std::istream& is)
{
    std::string type, dummy, idType;
    unsigned int id0, id1;
    Mdouble timeStamp;

    std::stringstream line(std::stringstream::in | std::stringstream::out);
    helpers::getLineFromStringStream(is, line);
    line >> type >> idType >> id0 >> id1 >> dummy >> timeStamp;
    ///\todo TW: Change identifier in restart file from id to index; is there any reason the id should be kept after restarting, once this is done? (Note, the id is set to the old one in the particle handler because interactions store id, not indices; also note id's are slow
    BaseInteraction* C;
    if (idType.compare("particleIds")==0)
        C = getInteraction(getDPMBase()->particleHandler.getObjectById(id0), getDPMBase()->particleHandler.getObjectById(id1), timeStamp);
    else
        C = getInteraction(getDPMBase()->particleHandler.getObjectById(id0), getDPMBase()->wallHandler.getObjectById(id1), timeStamp);
    line >> (*C);
}
