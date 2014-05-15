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

#ifndef COIL_H
#define COIL_H

#include "../BaseWall.h"


///This function defines a coil in the z-direction from a (constant) starting point, a (constant) length L, a (constant) radius r, a (constant) number or revelations N and a (constant) rotation speed (rev/s)

/// q is a new coordinate going from 0 to 1 and t is the time
/// x=xs+r*cos(2*pi*(offset+N*q))
/// y=ys+r*sin(2*pi*(offset+N*q))
/// z=zs+q*L


class Coil : public BaseWall
{
	public:
	
	Coil() : BaseWall()
	{
		Start.set_zero();
		L=1;
		r=1;
		N=1;
		omega=1;
		offset=0;
		thickness=0;
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "Coil() finished" << std::endl;
		#endif				
	}	
	
	Coil(Vec3D Start, double L, double r, double N, double omega,double thickness) : BaseWall()
	{
		this->Start=Start;
		this->L=L;
		this->r=r;
		this->N=N;
		this->omega=omega;
		this->thickness=thickness;
		this->offset=0.0;
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "Coil() finished" << std::endl;
		#endif				
	}
	
	virtual Coil* copy() const
	{
		return new Coil(*this);
	} 	
 
	bool get_distance_and_normal(BaseParticle &P, Mdouble &distance, Vec3D &normal_return)
	{
		double Rsqr=pow(P.get_Position().X-Start.X,2)+pow(P.get_Position().Y-Start.Y,2);
		if(Rsqr>pow(r+P.get_WallInteractionRadius()+thickness,2)||Rsqr<pow(r-P.get_WallInteractionRadius()-thickness,2)||P.get_Position().Z>L+Start.Z+P.get_WallInteractionRadius()+thickness||P.get_Position().Z<Start.Z-P.get_WallInteractionRadius()-thickness)
		{
			//std::cout<<"Particle is out of first bound checking"<<std::endl;
            //std::cout<<"Rule 1: "<<pow(r-P.get_Radius()-thickness,2)<<"<"<<Rsqr<<"<"<<pow(r+P.get_Radius()+thickness,2)<<std::endl;
            //std::cout<<"Rule 2: "<<Start.Z-P.get_Radius()-thickness<<"<"<<P.get_Position().Z<<"<"<<L+Start.Z+P.get_Radius()+thickness<<std::endl;
            return false;
		}		
		double R=sqrt(Rsqr);
		double alpha=atan2(P.get_Position().Y-Start.Y,P.get_Position().X-Start.X);
		double dz=P.get_Position().Z-Start.Z;
		
		///To find the contact point we have to minimize (with respect to q)
		///Distance^2=(x-x0-r*cos(2*Pi*(offset+N*q)))^2+(y-y0-r*sin(2*Pi*(offset+N*q)))^2+(z-z0-q*L)^2
        ///Using polar coordinated (i.e. x-x0=R*cos(alpha), y-y0=R*sin(alpha) and dz=z-z0)
		///Distance^2=R^2+r^2-2*R*r*cos(alpha-2*Pi*(offset+N*q))+(dz-q*L)^2
		
		///So we have to minimize:
		///Distance^2=R^2+r^2-2*R*r*cos(alpha-2*Pi*(offset+N*q))+(dz-q*L)^2
		///For this we use the Euler algoritm
		
		double q;		//Current guess
		double dd;		//Derivative at current guess
		double ddd;		//Second derivative at current guess
		double q0=dz/L;	//Minimum of the parabolic part
		
		///The initial guess will be in the maximum of the cos closest to the minimum of the parabolic part
		///Minima of the cos are at
		///alpha-2*Pi*(offset+N*q)=2*k*Pi (k=integer)
        ///q=alpha/(2*Pi*N)-k/N-offset/N (k=integer)
        
        double k=round(alpha/2.0/constants::pi-(offset+N*q0));
        q=alpha/(2*constants::pi*N)-k/N-offset/N;
        
		//Now apply Newton's method
        do
		{
			dd =-4.0*R*r*constants::pi    *N  *sin(alpha-2.0*constants::pi*(N*q+offset))-2.0*L*(dz-q*L);
			ddd= 8.0*R*r*constants::sqr_pi*N*N*cos(alpha-2.0*constants::pi*(N*q+offset))+2.0*L*L;
			q-=dd/ddd;
		} while(fabs(dd/ddd)>1e-14);
        
        
        //Check if the location is actually on the coil, otherwise a point collision with the end of the coil calculated
        if(q<0) //Left boundary of the coil
        {
            q=0;
        }
        else if(q>1) //right boundary of the coil
        {
            q=1;
        }
        	
		distance=R*R+r*r-2*R*r*cos(alpha-2*constants::pi*(offset+N*q))+pow(dz-q*L,2);
		//If distance is to large there is no contact
		if (distance>=(P.get_WallInteractionRadius()+thickness)*(P.get_WallInteractionRadius()+thickness))
		{
			//std::cout<<"Particle is out of second bound checking, distance^2="<<distance<<" max="<<(P.get_Radius()+thickness)*(P.get_Radius()+thickness)<<std::endl;
            return false;
		}

        Vec3D ContactPoint;
        distance=sqrt(distance)-thickness;
        ContactPoint.X=Start.X+r*cos(2.0*constants::pi*(offset+N*q));
        ContactPoint.Y=Start.Y+r*sin(2.0*constants::pi*(offset+N*q));
        ContactPoint.Z=Start.Z+q*L;
        normal_return=ContactPoint-P.get_Position();
        normal_return/=normal_return.GetLength();
		return true;
	}

	///Allows the wall to be moved to a new position (also orthogonal to the normal), and setting the velocity
	void move_time(Mdouble dt)
    {
		offset+=omega*dt;
	}
	
	///reads wall
	void read(std::istream& is)
    {
        std::string dummy;
		is >> dummy >> Start >> dummy >> L >> dummy >> r >> dummy >> N >> dummy >> omega >> dummy >> offset;
	}

	///outputs wall
	void print(std::ostream& os) const
    { 
		os << "Coil Start " << Start << " Length "<<L<<" Radius "<<r<<" Revolutions "<<N<<" Omega "<<omega<<" Thickness "<<thickness<<" Offset "<<offset;
	}	
	
	///Todo{Implement this function correctly}
	Vec3D get_Velocity() const {return Vec3D(0.0,0.0,0.0);}
	
	private:
		Vec3D Start;
		double L,r,N,omega,offset,thickness;
};

#endif
