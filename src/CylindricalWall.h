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
