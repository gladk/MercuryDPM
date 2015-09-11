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

/*!
 * Constructor of the ParticleHandler class. It creates and empty ParticleHandler.
 */
InteractionHandler::InteractionHandler()
{
    logger(DEBUG, "InteractionHandler::InteractionHandler() finished");
}

/*!
 * \param[in] IH The InteractionHandler that has to be copied.
 * \details This is not a copy constructor! It only clears all variables, since 
 *          by default interactions are not copied.
 * \todo Please check if interactions indeed don't need to be copied.
 */
InteractionHandler::InteractionHandler(const InteractionHandler& IH UNUSED)
: BaseHandler<BaseInteraction>()
{
    //By default interactions are not copied.
    logger(DEBUG, "InteractionHandler::InteractionHandler(const "
           "InteractionHandler &IH) finished, please note that no interactions"
           " have been copied.");
}

/*!
 * \param[in] rhs The BoundaryHandler on the right hand side of the assignment.
 */
InteractionHandler InteractionHandler::operator =(const InteractionHandler& rhs)
{
    if (this != &rhs)
    {
        clear();
    }
    logger(DEBUG, "InteractionHandler::operator =(const InteractionHandler& rhs) finished.");
    return *this;
}

InteractionHandler::~InteractionHandler()
{
    logger(DEBUG, "InteractionHandler::~InteractionHandler() finished");
}

/*!
 * \param[in] P A pointer to the BaseInteraction (or derived class) that has to be added.
 */
void InteractionHandler::addObject(BaseInteraction* I)
{
    //Puts the particle in the Particle list
    BaseHandler<BaseInteraction>::addObject(I);
    //set the particleHandler pointer
    I->setHandler(this);
}

/*!
 * \param[in] P the first BaseInteractable by which the interaction is defined.
 * \param[in] I the first BaseInteractable by which the interaction is defined.
 * \return the Interaction between the BaseInteractable's P and I, if it exists.
 */
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
    return nullptr;
}

/*!
 * \details Returns a pointer to the existing Interaction, if the Interaction 
 * already exists otherwise creates a new Interaction and returns a pointer to it.
 * \param[in] P the first BaseInteractable by which the interaction is defined.
 * \param[in] I the first BaseInteractable by which the interaction is defined.
 * \param[in] timeStamp the current value of DPMBase::time_.
 * \return the Interaction between the BaseInteractable's P and I
 */
BaseInteraction* InteractionHandler::getInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
{
    BaseSpecies* species = getDPMBase()->speciesHandler.getMixedObject(P->getIndSpecies(), I->getIndSpecies());

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

/*!
 * \details Deleting the three periodic interactions between two real particles 
 * is difficult, because its interaction information has to be saved. 
 * If the two real particles interacted (which can be checked by looking at the 
 * time stamp), the interaction between the real particles is kept, and all 
 * interactions that involve ghost particles gets removed; 
 * otherwise, the interaction between the lower-indexed real particle with the 
 * ghost particle of the higher indexed particles is saved (with the ghost 
 * particle replaced by the real particle), and all other interactions removed.
 * 
 * This is what this function is intended for, and it does it in the following way:
 * When an interaction is removed the periodic particle has to be stored in the I pointer
 * So when an interaction is removed where P is normal and I is periodic, 
 * and the information is new it will be transfered when the index of P is 
 * lower than the index of the real particle of I. 
 * \image html Walls/periodicBoundary.pdf
 * \param[in] the id of the Interaction that needs to be deleted.
 */
void InteractionHandler::removeObjectKeepingPeriodics(unsigned const int id)
{
    BaseInteraction* iMain = getObject(id);

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
                    //Here we decide which of the two interactions should be kept:
                    //the interaction between the two real particles (iMain), or 
                    //the interaction between the real and a ghost particle (iOther). 
                    //It picks the one for which a collision has happened, 
                    //i.e. the one with the newer timeStamp. 
                    ///\todo this function will create an error if the timeStamp is in the future! This should not happen (ever), but who knows.
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

/*!
 * \details Each interaction contains a time stamp, which stores the last time 
 * that an interaction object has been called. Thus, one can see that an 
 * interaction has ended by comparing the time stamp with the last value of DPMBase::time_.
 * This function erases all interactions that have ended. 
 * \param[in] lastTimeStep the last used value of DPMBase::time_.
 */
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

/*!
 * \return the string InteractionHandler
 */
std::string InteractionHandler::getName() const
{
    return "InteractionHandler";
}

/*!
 * \param[in] os The output stream where the InteractionHandler must be written
 *  to, usually a restart file.
 */
void InteractionHandler::write(std::ostream& os) const
{
    os << "Interactions " << getNumberOfObjects() << std::endl;
    for (BaseInteraction* i : *this)
        os << (*i) << std::endl;
}

/*!
 * \param[in] is The input stream from which the information is read.
 */
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
    if (idType.compare("particleIds") == 0)
        C = getInteraction(getDPMBase()->particleHandler.getObjectById(id0), getDPMBase()->particleHandler.getObjectById(id1), timeStamp);
    else
        C = getInteraction(getDPMBase()->particleHandler.getObjectById(id0), getDPMBase()->wallHandler.getObjectById(id1), timeStamp);
    line >> (*C);
}
