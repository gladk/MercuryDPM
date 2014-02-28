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


#ifndef CYLINDRICALWALL_H
#define CYLINDRICALWALL_H

#include "BaseWall.h"

class CylindricalWall : public BaseWall
{
	public:
	CylindricalWall() : BaseWall()
	{
		#ifdef CONSTUCTOR_OUTPUT
		std::cout<<"CylindricalWall () finished"<<std::endl;
		#endif
	}
	
	CylindricalWall (const CylindricalWall &p) : BaseWall(p)
	{
		radius=p.radius;
		#ifdef CONSTUCTOR_OUTPUT
		std::cout<<"CylindricalWall (const CylindricalWall &p) finished"<<std::endl;
		#endif
	}
	
	CylindricalWall (double radius) : BaseWall()
	{
		this->radius=radius;
		#ifdef CONSTUCTOR_OUTPUT
		std::cout<<"CylindricalWall (double radius) finished"<<std::endl;
		#endif
	}	

	///Wall copy method. It calls the copy contrustor of this Wall, usefull for polymorfism
	CylindricalWall* copy() const
	{
		return new CylindricalWall(*this);
	} 

	void clear()
	{
		indSpecies = 0;
	}	  

	///Defines a standard wall, given an outward normal vector s. t. normal*x=position
	void set(Mdouble radius) 
	{
		this->radius=radius;
	}

	///Since this function should be called before calculating any Particle-Wall interactions, it can also be used to set the normal vector in case of curved walls.
	bool get_distance_and_normal(BaseParticle &P, Mdouble &distance, Vec3D &normal_return)
	{
		double R=sqrt(pow(P.get_Position().X,2)+pow(P.get_Position().Y,2));
		distance=radius-R;
		if(distance>=P.get_Radius()) return false;
		
		normal_return.X=P.get_Position().X;
		normal_return.Y=P.get_Position().Y;
		normal_return.Z=0.0;
		normal_return/=normal_return.GetLength();
		
		/*std::cout<<"Contact Cylindrical Wall"<<std::endl;
		std::cout<<"Particle index="<<P.get_Index()<<" position="<<P.get_Position()<<" R="<<R<<std::endl;
		std::cout<<"distance="<<distance<<" normal="<<normal_return<<std::endl;
		std::cout<<"Overlap="<<distance-P.get_Radius()<<" velocity="<<P.get_Velocity()<<std::endl;*/
		return true;
	}

	///reads wall
	void read(std::istream& is)  { 
		std::string dummy;
		is >> dummy >> radius;
	}

	///outputs wall
	void print(std::ostream& os) const { 
		os << "CylindricalWall radius " << radius;
	}

	///access function for radius
	double get_Radius() {return radius;}
	Vec3D get_Velocity() const {return Vec3D(0.0,0.0,0.0);}

private:
	Mdouble radius;
};

#endif
