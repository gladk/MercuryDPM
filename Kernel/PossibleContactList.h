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
