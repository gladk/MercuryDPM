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
        if (FirstPossibleContact->get_Next())
            FirstPossibleContact->get_Next()->set_Prev(FirstPossibleContact);
        if (FirstPossibleContact->get_Next1())
            FirstPossibleContact->get_Next1()->set_Prev(P1, FirstPossibleContact);
        if (FirstPossibleContact->get_Next2())
            FirstPossibleContact->get_Next2()->set_Prev(P2, FirstPossibleContact);
    }

    /*!
     * \brief 
     */
    void remove_ParticlePosibleContacts(BaseParticle *P)
    {
        //std::cout<<"Removing all contacts of particle "<<P->get_Index()<<std::endl;
        BaseParticle* O;
        PossibleContact* Next;
        PossibleContact* Curr = P->getFirstPossibleContact();
        while (Curr)
        {
            //std::cout<<"Removing possible contacts index="<<Curr->get_Index()<<" between particle "<<Curr->get_P1()->getIndex()<<" and "<<Curr->get_P2()->getIndex()<<std::endl;
            Next = Curr->get_Next(P);
            O = Curr->get_OtherParticle(P);
            if (Curr->get_Next())
                Curr->get_Next()->set_Prev(Curr->get_Prev());
            
            if (Curr->get_Prev())
                Curr->get_Prev()->set_Next(Curr->get_Next());
            else
                FirstPossibleContact = Curr->get_Next();
            
            if (Curr->get_Next(O))
                Curr->get_Next(O)->set_Prev(O, Curr->get_Prev(O));
            
            if (Curr->get_Prev(O))
                Curr->get_Prev(O)->set_Next(O, Curr->get_Next(O));
            else
                O->setFirstPossibleContact(Curr->get_Next(O));
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
            it=it->get_Next();
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
