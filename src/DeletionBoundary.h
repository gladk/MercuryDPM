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

#ifndef DELETIONBOUNDARY_H
#define DELETIONBOUNDARY_H

#include "ParticleHandler.h"

class DeletionBoundary : public BaseBoundary 
{
	public:
	DeletionBoundary() : BaseBoundary()
	{
		#ifdef CONSTUCTOR_OUTPUT
			std::cout<<"DeletionBoundary () finished"<<std::endl;
		#endif
	}

	DeletionBoundary* copy() const
	{
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "virtual DeletionBoundary* copy() const finished" << std::endl;
		#endif		
		return new DeletionBoundary(*this);
	}
	
	void set(Vec3D normal_, Mdouble position_) 
	{
		factor = 1. / sqrt(Dot(normal_, normal_));
		normal = normal_ * factor;
		position = position_ * factor;		
	}	

	Mdouble get_distance(const Vec3D &Position) {return position - Dot(Position, normal);}	
	
	//Returns true if the particle is deleted
	bool checkBoundaryAfterParticleMoved(BaseParticle *P, ParticleHandler &pH)
	{
		if (get_distance(P->get_Position())<0)
		{
			pH.removeObject(P->get_Index());
			return true;			
		}
		else
			return false;
	};
	
	///reads wall
	void read(std::istream& is)  { 
		std::string dummy;
		is >> dummy >> normal >> dummy >> position;
	}

	///outputs wall
	void print(std::ostream& os) const { 
		os << "DeletionBoundary normal " << normal << " position " << position;
	}

	
private:
	Vec3D normal;     //<outward unit normal vector
	Mdouble position;  //<position n*x=p
	Mdouble factor;    //<This is the normal to rescale to unit vectoers.	
};


#endif
