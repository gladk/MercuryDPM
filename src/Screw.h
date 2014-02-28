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

#ifndef SCREW_H
#define SCREW_H

#include "BaseWall.h"
#include "ExtendedMath.h"


///This function defines a archimedes screw in the z-direction from a (constant)  starting point, a (constant) length L, a (constant) radius r, a (constant) number or revelations N and a (constant) rotation speed (rev/s)

/// q is a new coordinate going from 0 to 1 and t is the time
/// x=xs+r*cos(2*pi*(offset+N*q))
/// y=ys+r*sin(2*pi*(offset+N*q))
/// z=zs+q*L


class Screw : public BaseWall
{
	public:
	
	Screw() : BaseWall()
	{
		Start.set_zero();
		L=1;
		MaxR=1;
		N=1;
		omega=1;
		offset=0;
		thickness=0;
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "Screw() finished" << std::endl;
		#endif				
	}	
	
	Screw(Vec3D Start, double L, double R, double N, double omega,double thickness) : BaseWall()
	{
		this->Start=Start;
		this->L=L;
		this->MaxR=R;
		this->N=N;
		this->omega=omega;
		this->thickness=thickness;
		this->offset=0.0;
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "Screw() finished" << std::endl;
		#endif				
	}
	
	virtual Screw* copy() const
	{
		return new Screw(*this);
	} 	
 
	bool get_distance_and_normal(BaseParticle &P, Mdouble &distance, Vec3D &normal_return)
	{
		double Rsqr=pow(P.get_Position().X-Start.X,2)+pow(P.get_Position().Y-Start.Y,2);
		if(Rsqr>pow(MaxR+P.get_WallInteractionRadius()+thickness,2)||P.get_Position().Z>L+Start.Z+P.get_WallInteractionRadius()+thickness||P.get_Position().Z<Start.Z-P.get_WallInteractionRadius()-thickness)
		{
			//std::cout<<"Particle is out of first bound checking"<<std::endl;
            //std::cout<<"Rule 1: "<<Rsqr<<"<"<<pow(r+P.get_Radius()+thickness,2)<<std::endl;
            //std::cout<<"Rule 2: "<<Start.Z-P.get_Radius()-thickness<<"<"<<P.get_Position().Z<<"<"<<L+Start.Z+P.get_Radius()+thickness<<std::endl;
			return false;
		}
        double R=sqrt(Rsqr);
		double alpha=atan2(P.get_Position().Y-Start.Y,P.get_Position().X-Start.X);
		double dz=P.get_Position().Z-Start.Z;
		
		///To find the contact point we have to minimize (with respect to r and q)
		///Distance^2=(x-x0-r*cos(2*Pi*(offset+N*q)))^2+(y-y0-r*sin(2*Pi*(offset+N*q)))^2+(z-z0-q*L)^2
        ///Using polar coordinated (i.e. x-x0=R*cos(alpha), y-y0=R*sin(alpha) and dz=z-z0)
		///Distance^2=R^2+r^2-2*R*r*cos(alpha-2*Pi*(offset+N*q))+(dz-q*L)^2
		
		///Differentiation with respect to r and solve for zero:
		///0=2*r-2*R*cos(alpha-2*Pi*(offset+N*q)
		///r=R*cos(alpha-2*Pi*(offset+N*q))
		
		///Substitue back
		///Distance^2=R^2+R^2*cos^2(alpha-2*Pi*(offset+N*q))-2*R^2*cos(alpha-2*Pi*(offset+N*q))*cos(alpha-2*Pi*(offset+N*q))+(dz-q*L)^2
		///Distance^2=R^2*sin^2(alpha-2*Pi*(offset+N*q))+(dz-q*L)^2
		
		///So we have to minimize:
		///Distance^2=R^2*sin^2(alpha-2*Pi*(offset+N*q))^2+(dz-q*L)^2
		///For this we use the Euler algoritm
		
		double q;		//Current guess
		double dd;		//Derivative at current guess
		double ddd;		//Second derivative at current guess
		double q0=dz/L;	//Minimum of the parabolic part
		
		///The initial guess will be in the minimum of the sin closest to the minimum of the parabolic part
		///Minima of the sin are at
		///alpha-2*Pi*(offset+N*q)=k*Pi (k=integer)
        ///q=alpha/(2*Pi*N)-k/(2*N)-offset/N (k=integer)
        
        double k=round(alpha/constants::pi-2.0*(offset+N*q0));
        q=alpha/(2.0*constants::pi*N)-k/(2.0*N)-offset/N;
        
		//Now apply Newton's method
        do
		{
			dd =-2.0*Rsqr*constants::pi*N*sin(2.0*alpha-4.0*constants::pi*(N*q+offset))-2.0*L*(dz-q*L);
			ddd= 8.0*Rsqr*constants::sqr_pi*N*N*cos(2.0*alpha-4.0*constants::pi*(N*q+offset))+2.0*L*L;
			q-=dd/ddd;
		} while(fabs(dd/ddd)>1e-14);
        
        //Calculate r
        double r=R*cos(2.0*constants::pi*(offset+N*q)-alpha);
        
        //Check if the location is actually on the screw:
        //First posibility is that the radius is to large:
        if(fabs(r)>MaxR) //Left boundary of the coil
        {
            r=mathsFunc::sign(r)*MaxR;
            int steps=0;
            //This case reduces to the coil problem
            do
            {
                dd =-4.0*R*r*constants::pi    *N  *sin(alpha-2.0*constants::pi*(N*q+offset))-2.0*L*(dz-q*L);
                ddd= 8.0*R*r*constants::sqr_pi*N*N*cos(alpha-2.0*constants::pi*(N*q+offset))+2.0*L*L;
                q-=dd/ddd;
                steps++;
            } while(fabs(dd/ddd)>1e-14);      
        }
        //Second possibility is that it occured before the start of after the end
        if(q<0)
        {
            q=0;
            r=R*cos(alpha-2.0*constants::pi*(offset+q*N));
            if(fabs(r)>MaxR)
            {
                r=mathsFunc::sign(r)*MaxR;
            }
        }    
        else if(q>1)
        {
            q=1;
            r=R*cos(alpha-2.0*constants::pi*(offset+q*N));
            if(fabs(r)>MaxR)
            {
                r=mathsFunc::sign(r)*MaxR;
            }
        }
                	
		distance=R*R*pow(sin(alpha-2*constants::pi*(offset+N*q)),2)+pow(dz-q*L,2);
		//If distance is to large there is no contact
		if (distance>=(P.get_WallInteractionRadius()+thickness)*(P.get_WallInteractionRadius()+thickness))
		{
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
		is >> dummy >> Start >> dummy >> L >> dummy >> MaxR >> dummy >> N >> dummy >> omega >> dummy >> offset;
	}

	///outputs wall
	void print(std::ostream& os) const
    { 
		os << "Screw start " << Start << " Length "<<L<<" Radius "<<MaxR<<" Revolutions "<<N<<" Omega "<<omega<<" Thickness "<<thickness<<" Offset "<<offset;
	}	
	
	///Todo{Implement this function correctly}
	Vec3D get_Velocity() const {return Vec3D(0.0,0.0,0.0);}
	
	private:
		Vec3D Start;
		double L,MaxR,N,omega,offset,thickness;
};

#endif
