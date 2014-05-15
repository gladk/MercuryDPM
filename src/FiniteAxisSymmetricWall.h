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

#ifndef FINITEAXISSYMETRICWALL_H
#define FINITEAXISSYMETRICWALL_H

#include "FiniteWall.h"

///\todo{Thomas: Can we rename AxisSymmetric with Axisymmetric? Can we rename FiniteWall with PolyhedralWall?}
class FiniteAxisSymmetricWall : public FiniteWall
{
	public:
	FiniteAxisSymmetricWall() : FiniteWall()
	{
		CartesianVelocity.set_zero();
		axisOrigin.set_zero();
		axisOrientation.set_zero();		
		#ifdef CONSTUCTOR_OUTPUT
			std::cout<<"FiniteAxisSymmetricWall() finished"<<std::endl;
		#endif
	}
	
	FiniteAxisSymmetricWall (const FiniteAxisSymmetricWall &p) : FiniteWall(p)
	{
		CartesianVelocity=p.CartesianVelocity;
		axisOrigin=p.axisOrigin;
		axisOrientation=p.axisOrientation;
		#ifdef CONSTUCTOR_OUTPUT
			std::cout<<"FiniteAxisSymmetricWall(const FiniteAxisSymmetricWall &p) finished"<<std::endl;
		#endif
	}	

	///Wall copy method. It calls the copy contrustor of this Wall, usefull for polymorfism
	FiniteAxisSymmetricWall* copy() const
	{
		return new FiniteAxisSymmetricWall(*this);
	} 

	bool get_distance_and_normal(BaseParticle &P, Mdouble &distance, Vec3D &normal_return)
	{ 
		///\todo{Kay found a bug here}
	    //transform to axisymmetric coordinates
	    Vec3D PO = P.get_Position() - axisOrigin; //move the coordinate system to the axis origin, so P0=(xhat,yhat,zhat) 
        Vec3D tangential = PO-P.get_Position().Z*axisOrientation; //tangential is the projection into the (xhat,yhat) plane
        P.set_Position(Vec3D(tangential.GetLength(),0.0,Dot(axisOrientation,PO))); //now P=(r,phi,zhat) is cylindrical
	    tangential /= P.get_Position().X;
	    Vec3D normal_axisymmetric_coordinates;
	    //determine wall distance, normal and contact in axissymmetric coordinates
	    //and transform from axisymmetric coordinates
	    if(!FiniteWall::get_distance_and_normal(P, distance, normal_axisymmetric_coordinates))
	    {
	    	//if not in contact
			P.set_Position(PO + axisOrigin);
			return false;
		} else {
	    	//if in contact
	    	normal_return = normal_axisymmetric_coordinates.Z * axisOrientation + tangential*normal_axisymmetric_coordinates.X;		
		    ///\todo Radius is based on Particle, not wall
		    if (!velocity.is_zero()) { //assuming most walls have zero velocity, this if statement saves time
	    		Vec3D angular = Cross(axisOrientation,tangential);
	    		CartesianVelocity = Vec3D(velocity.Z*axisOrientation + tangential*velocity.X + angular*P.get_Position().X*velocity.Y);
		    }
	    	P.set_Position(PO + axisOrigin);
	    	//std::cout << P.get_Position() << " v " << get_Velocity() << " cv " << velocity << std::endl;
		    return true;
		}
    }

    ///Defines an axisymmetric wall
    void setSymmetryAxis(Vec3D new_axisOrigin, Vec3D new_axisOrientation) {
	    axisOrigin = new_axisOrigin;
	    axisOrientation = new_axisOrientation;
	    axisOrientation.normalize();
    }
    
	///reads wall
	void read(std::istream& is)
	{ 
		std::string dummy;
		is>>axisOrigin>>axisOrientation;
		FiniteWall::read(is);
	}    

	///outputs wall
	void print(std::ostream& os) const
	{
		os << "FiniteAxisSymmetricWall axisOrigin "<<axisOrigin<<" axisOrientation "<<axisOrientation<<" "; 
		FiniteWall::print(os);
	}
	
	///\todo{Velocity is given in (R,phi,zhat) coordinates and has to be translated to (x,y,z)}
	///access function for velocity
	Vec3D get_Velocity() const  {return CartesianVelocity;}

	///Allows the wall to be moved to a new position, and sets the velocity
	void moveAxisOrigin(Vec3D velocity_, Mdouble dt) {
		velocity=velocity_;
		axisOrigin+=velocity*dt;
	}

	Vec3D getAxisOrigin() const  {return axisOrigin;}

	private:
	Vec3D axisOrigin;
	Vec3D axisOrientation;
	Vec3D CartesianVelocity;
};

#endif
