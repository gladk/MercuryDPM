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
 * \class PossibleContactList
 * \brief
 */
class PossibleContactList
{
public:
    
    /*!
     * \brief constructor
     */
    PossibleContactList()
    {
//        std::cout<<"Standard constructor"<<std::endl;
        FirstPossibleContact = 0;
    }
    
    /*!
     * \brief 
     */
    void add_PossibleContact(BaseParticle *P1, BaseParticle *P2)
    {
		//std::cout<<"Added new possible contact between particle "<<P1->getIndex()<<" and "<<P2->getIndex()<<std::endl;
        FirstPossibleContact = new PossibleContact(P1, P2, FirstPossibleContact, P1->getFirstPossibleContact(), P2->getFirstPossibleContact());
        P1->setFirstPossibleContact(FirstPossibleContact);
        P2->setFirstPossibleContact(FirstPossibleContact);
        if (FirstPossibleContact->getNext())
            FirstPossibleContact->getNext()->setPreviousPosition(FirstPossibleContact);
        if (FirstPossibleContact->getNext1())
            FirstPossibleContact->getNext1()->setPreviousPosition(P1, FirstPossibleContact);
        if (FirstPossibleContact->getNext2())
            FirstPossibleContact->getNext2()->setPreviousPosition(P2, FirstPossibleContact);
    }

    /*!
     * \brief 
     */
    void remove_ParticlePosibleContacts(BaseParticle *P)
    {
        //std::cout<<"Removing all contacts of particle "<<P->getIndex()<<std::endl;
        BaseParticle* O;
        PossibleContact* Next;
        PossibleContact* Curr = P->getFirstPossibleContact();
        while (Curr)
        {
            //std::cout<<"Removing possible contacts index="<<Curr->getIndex()<<" between particle "<<Curr->getP1()->getIndex()<<" and "<<Curr->getP2()->getIndex()<<std::endl;
            Next = Curr->getNext(P);
            O = Curr->getOtherParticle(P);
            if (Curr->getNext())
                Curr->getNext()->setPreviousPosition(Curr->getPrevious());
            
            if (Curr->getPrevious())
                Curr->getPrevious()->setNextPosition(Curr->getNext());
            else
                FirstPossibleContact = Curr->getNext();
            
            if (Curr->getNext(O))
                Curr->getNext(O)->setPreviousPosition(O, Curr->getPrevious(O));
            
            if (Curr->getPrevious(O))
                Curr->getPrevious(O)->setNextPosition(O, Curr->getNext(O));
            else
                O->setFirstPossibleContact(Curr->getNext(O));
            delete Curr;
            Curr = Next;
        }
        P->setFirstPossibleContact(0);
    }
    
    /*!
     * \brief
     */
    void write(std::ostream& os) const
    {
        os<<"Possible contacts are:"<<std::endl;
        PossibleContact* it=FirstPossibleContact;
        while(it!=nullptr)
        {
            os<<*it<<std::endl;
            it=it->getNext();
        }
    }

    /*!
     * \brief 
     */
    PossibleContact* getFirstPossibleContact()
    {
        return FirstPossibleContact;
    }
    
private:
    PossibleContact* FirstPossibleContact;///
    
};

#endif
