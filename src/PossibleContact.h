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

#ifndef POSSIBLECONTACT_H
#define POSSIBLECONTACT_H

class PossibleContact
{
	public:
	PossibleContact()
	{
		P1=0;
		P2=0;
		Next=0;
		Prev=0;
		Next1=0;
		Prev1=0;
		Next2=0;
		Prev2=0;
		index=0;
	}
	PossibleContact(Particle* P1, Particle* P2)
	{
		this->P1=P1;
		this->P2=P2;
	}
	PossibleContact(Particle* P1, Particle* P2, PossibleContact* Next)
	{
		this->P1=P1;
		this->P2=P2;
		this->Next=Next;
		this->Prev=0;
		if(Next)
			index=Next->get_Index()+1;
		else
			index=0;
	}
	PossibleContact(Particle* P1, Particle* P2, PossibleContact* Next, PossibleContact* Next1, PossibleContact* Next2)
	{
		this->P1=P1;
		this->P2=P2;
		this->Next=Next;
		this->Next1=Next1;
		this->Next2=Next2;
		this->Prev=0;
		this->Prev1=0;
		this->Prev2=0;
		if(Next)
			index=Next->get_Index()+1;
		else
			index=0;
	}	
	
	Particle* get_P1() {return P1;}
	Particle* get_P2() {return P2;}
	Particle* get_OtherParticle(Particle *P)
	{
		if(P1==P)
			return P2;
		else if(P2==P)
			return P1;
		else
		{
			std::cout<<"Particle* get_OtherParticle(Particle *P), no match for "<<P->get_Index()<<" posibilities are "<<P1->get_Index()<<" and "<<P2->get_Index()<<std::endl;
			return 0;
		}		
	}
	PossibleContact* get_Next() {return Next;}
	PossibleContact* get_Next(Particle* P)
	{
		if(P1==P)
			return Next1;
		else if(P2==P)
			return Next2;
		else
		{
			std::cout<<"get_Next(Particle* P), no match for "<<P->get_Index()<<" posibilities are "<<P1->get_Index()<<" and "<<P2->get_Index()<<std::endl;
			return 0;
		}
	}	
	PossibleContact* get_Prev() {return Prev;}
	PossibleContact* get_Prev(Particle* P)
	{
		if(P1==P)
			return Prev1;
		else if(P2==P)
			return Prev2;
		else
		{
			std::cout<<"PossibleContact* get_Prev(Particle* P), no match for "<<P->get_Index()<<" posibilities are "<<P1->get_Index()<<" and "<<P2->get_Index()<<std::endl;
			return 0;
		}
	}		
	PossibleContact* get_Next1() {return Next1;}
	PossibleContact* get_Prev1() {return Prev1;}
	PossibleContact* get_Next2() {return Next2;}
	PossibleContact* get_Prev2() {return Prev2;}
	int get_Index() {return index;}
	
	void set_P1(Particle* P1){this->P1=P1;}
	void set_P2(Particle* P2){this->P2=P2;}
	void set_Next(PossibleContact* Next){this->Next=Next;}
	void set_Next(Particle* P,PossibleContact* Next)
	{
		if(P1==P)
			this->Next1=Next;
		else if(P2==P)
			this->Next2=Next;
		else
			std::cout<<"void set_Next(Particle* P,PossibleContact* Next), no match for "<<P->get_Index()<<" posibilities are "<<P1->get_Index()<<" and "<<P2->get_Index()<<std::endl;
	}	
	void set_Prev(PossibleContact* Prev){this->Prev=Prev;}
	void set_Prev(Particle* P,PossibleContact* Prev)
	{
		if(P1==P)
			this->Prev1=Prev;
		else if(P2==P)
			this->Prev2=Prev;
		else
			std::cout<<"void set_Prev(Particle* P,PossibleContact* Prev), no match for "<<P->get_Index()<<" posibilities are "<<P1->get_Index()<<" and "<<P2->get_Index()<<std::endl;
	}
	void set_Next1(PossibleContact* Next1){this->Next1=Next1;}
	void set_Prev1(PossibleContact* Prev1){this->Prev1=Prev1;}
	void set_Next2(PossibleContact* Next2){this->Next2=Next2;}
	void set_Prev2(PossibleContact* Prev2){this->Prev2=Prev2;}
	
	
	private:
	int index;
	Particle* P1;
	Particle* P2;
	PossibleContact* Next;
	PossibleContact* Prev;
	PossibleContact* Next1;
	PossibleContact* Prev1;
	PossibleContact* Next2;
	PossibleContact* Prev2;
	
};

#endif
