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

#ifndef POSSIBLECONTACTLIST_H
#define POSSIBLECONTACTLIST_H

#include "PossibleContact.h"

/*!
 * \brief Manages the linked list of PossibleContact.
 * \details Please note that the PossibleContact at the front of the list is the
 * one that is added last, so firstPossibleContact_ is the PossibleContact that
 * has been added last.
 * \todo Look at the memory management of PossibleContactList. Maybe a destructor
 * that takes out all remaining PossibleContact? Or is there a Handler that calls
 * remove_ParticlePosibleContacts for all particles?
 * \todo Restart-tests are not working with CONTACT_LIST_HGRID turned on, so either
 * finish the ContactList-related code, or get rid of it. If we keep it, clean up
 * the code, in particular the naming-convention.
 */
class PossibleContactList
{
public:
    
    /*!
     * \brief Constructor, sets the firstPossibleContact_ to a nullptr since there are no possible interactions yet.
     */
    PossibleContactList()
    {
        firstPossibleContact_ = nullptr;
        logger(DEBUG, "PossibleContactList() constructor finished.");
    }
    
    /*!
     * \brief Add the possible contact between two given BaseParticle to the linked list.
     * \param[in] P1 A pointer to the first BaseParticle we want to make a PossibleContact with.
     * \param[in] P2 A pointer to the second BaseParticle we want to make a PossibleContact with.
     * \details If there is a PossibleContact between two BaseParticle, make a new
     *          PossibleContact and put it in front of the linked list. Then assign
     *          the newly made PossibleContact to the former front of the linked list.
     */
    void add_PossibleContact(BaseParticle* P1, BaseParticle* P2)
    {
		firstPossibleContact_ = new PossibleContact(P1, P2, firstPossibleContact_, P1->getFirstPossibleContact(), P2->getFirstPossibleContact());
        P1->setFirstPossibleContact(firstPossibleContact_);
        P2->setFirstPossibleContact(firstPossibleContact_);
        if (firstPossibleContact_->getNext())
            firstPossibleContact_->getNext()->setPreviousPosition(firstPossibleContact_);
        if (firstPossibleContact_->getNext1())
            firstPossibleContact_->getNext1()->setPreviousPosition(P1, firstPossibleContact_);
        if (firstPossibleContact_->getNext2())
            firstPossibleContact_->getNext2()->setPreviousPosition(P2, firstPossibleContact_);
        logger(VERBOSE, "Added new possible contact between particles % and % ", P1->getIndex(), P2->getIndex());
    }

    /*!
     * \brief Remove all PossibleContact with given BaseParticle from the linked list.
     * \param[in] P A pointer to the BaseParticle we want to remove all PossibleContact for.
     * \details To remove all PossibleContact with given BaseParticle from the 
     *          linked list of all PossibleContact, go through all PossibleContact
     *          that are given as a list for the given BaseParticle, and then remove
     *          it from the global list by correcting all pointers and then delete
     *          the PossibleContact.
     */
    void remove_ParticlePosibleContacts(BaseParticle* P)
    {
        logger(VERBOSE, "Removing all contacts of particle %.", P->getIndex());
        //The BaseParticle that shares a PossibleContact with P.
        BaseParticle* O;
        //The next possible contact for P.
        PossibleContact* Next;
        //The current possible contact for P.
        PossibleContact* Curr = P->getFirstPossibleContact();
        while (Curr != nullptr)
        {
            logger(VERBOSE, "Removing possible contacts index = % between particles % and %.", Curr->getIndex(), Curr->getP1()->getIndex(), Curr->getP2()->getIndex());
            Next = Curr->getNext(P);
            O = Curr->getOtherParticle(P);
            //Set the pointers of the next global possible contact.
            if (Curr->getNext())
                Curr->getNext()->setPreviousPosition(Curr->getPrevious());
            
            //Set the pointers of the previous possible contact.
            if (Curr->getPrevious())
                Curr->getPrevious()->setNextPosition(Curr->getNext());
            else
                firstPossibleContact_ = Curr->getNext();
            
            //Set the pointers of the other object of the possible contact.
            if (Curr->getNext(O))
                Curr->getNext(O)->setPreviousPosition(O, Curr->getPrevious(O));
            
            if (Curr->getPrevious(O))
                Curr->getPrevious(O)->setNextPosition(O, Curr->getNext(O));
            else
                O->setFirstPossibleContact(Curr->getNext(O));
            
            //Delete the possible contact and update the the pointer Curr to the
            //next possible contact with P.
            delete Curr;
            Curr = Next;
        }
        P->setFirstPossibleContact(nullptr);
    }
    
    /*!
     * \brief Write all PossibleContact to the given ostream.
     * \param[in,out] os The output stream the PossibleContactList must be written to.
     */
    void write(std::ostream& os) const
    {
        os << "Possible contacts are: " << std::endl;
        PossibleContact* it = firstPossibleContact_;
        while(it != nullptr)
        {
            os << *it << std::endl;
            it = it->getNext();
        }
    }

    /*!
     * \brief Get the front of the linked list of PossibleContact.
     * \return A pointer to the first PossibleContact on the linked list.
     */
    PossibleContact* getFirstPossibleContact()
    {
        return firstPossibleContact_;
    }
    
private:
    /*!
     * \brief The pointer to the first PossibleContact on the linked list. 
     * \details Please note that this is the PossibleContact that has been added
     *          last.
     */
    PossibleContact* firstPossibleContact_;///
    
};

#endif
