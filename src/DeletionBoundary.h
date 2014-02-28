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
