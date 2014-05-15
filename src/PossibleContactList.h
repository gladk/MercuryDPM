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
