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

class PossibleContactList
{
public:

    PossibleContactList()
    {
//        std::cout<<"Standard constructor"<<std::endl;
        FirstPossibleContact=0;
    }
    
    void add_PossibleContact(Particle *P1, Particle *P2)
	{
//		std::cout<<"Added new contact between particle "<<P1->get_Index()<<" and "<<P2->get_Index()<<std::endl;
		FirstPossibleContact=new PossibleContact(P1,P2,FirstPossibleContact,P1->get_FirstPossibleContact(),P2->get_FirstPossibleContact());
		P1->set_FirstPossibleContact(FirstPossibleContact);
		P2->set_FirstPossibleContact(FirstPossibleContact);
		if(FirstPossibleContact->get_Next())
			FirstPossibleContact->get_Next()->set_Prev(FirstPossibleContact);
		if(FirstPossibleContact->get_Next1())
			FirstPossibleContact->get_Next1()->set_Prev(P1,FirstPossibleContact);
		if(FirstPossibleContact->get_Next2())
			FirstPossibleContact->get_Next2()->set_Prev(P2,FirstPossibleContact);
	}
	
	/*void remove_AllPosibleContacts()
	{
		//std::cout<<"Removing all contacts"<<std::endl;
		PossibleContact* Next;
		PossibleContact* Curr=FirstPossibleContact;
		while(Curr)
		{
			Next=Curr->get_Next();
			delete Curr;
			Curr=Next;
		}
		FirstPossibleContact=NULL;
		
		for (vector<Particle*>::iterator it = particleHandler.begin(); it!=particleHandler.end(); ++it)
			(*it)->set_FirstPossibleContact(0);
	}*/
	
	void remove_ParticlePosibleContacts(Particle *P)
	{
		//std::cout<<"Removing all contacts of particle "<<P->get_Index()<<std::endl;
		Particle* O;
		PossibleContact* Next;
		PossibleContact* Curr=P->get_FirstPossibleContact();
		while(Curr)
		{
			//std::cout<<"Removing contacts index="<<Curr->get_Index()<<" between particle "<<Curr->get_P1()->get_Index()<<" and "<<Curr->get_P2()->get_Index()<<std::endl;
			Next=Curr->get_Next(P);
			O=Curr->get_OtherParticle(P);
			if(Curr->get_Next())
				Curr->get_Next()->set_Prev(Curr->get_Prev());
			
			if(Curr->get_Prev())
				Curr->get_Prev()->set_Next(Curr->get_Next());
			else
				FirstPossibleContact=Curr->get_Next();
			
			if(Curr->get_Next(O))
				Curr->get_Next(O)->set_Prev(O,Curr->get_Prev(O));
				
			if(Curr->get_Prev(O))
				Curr->get_Prev(O)->set_Next(O,Curr->get_Next(O));
			else
				O->set_FirstPossibleContact(Curr->get_Next(O));
			delete Curr;
			Curr=Next;
		}
		P->set_FirstPossibleContact(0);
	}
    
    PossibleContact* getFirstPossibleContact(){return FirstPossibleContact;}
    
private:
    PossibleContact* FirstPossibleContact;

};

#endif
