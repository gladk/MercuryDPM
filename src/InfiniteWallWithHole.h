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

///This is a class defining walls. It defines the 
///interaction of regular walls and periodic walls
///with particles as defined in Particle
///Modifications:

#ifndef INFINITEWALLWITHHOLE_H
#define INFINITEWALLWITHHOLE_H

////////////////////////////////////////////////////////////////////////////////////////////////////
///Definition of a wall (planar or finite). 
///A standard wall is a plane defined  as {x: normal*x=position}, with normal being the outward unit normal vector of the wall.
///A particle touches a standard wall if position-normal*x<=radius.
///A finite wall is convex polygon defined by a set of normals normal_i and positions position_i.
///A particle touches a finite wall if position_i-normal_i*x<=radius for all i.
/// \todo remove access to set Nwalls and wall set etc.. and add function add_wall. This would have stoped a lot of bugs in my own (Anthony's) driver codes.
////////////////////////////////////////////////////////////////////////////////////////////////////
class InfiniteWallWithHole : public BaseWall
{
	public:
	InfiniteWallWithHole() : BaseWall()
	{
		velocity.set_zero();
		#ifdef CONSTUCTOR_OUTPUT
		std::cout<<"InfiniteWall () finished"<<std::endl;
		#endif
	}
	
	InfiniteWallWithHole(Vec3D normal, Mdouble position, Mdouble holeRadius)  : BaseWall()
	{
		this->normal=normal;
		this->position=position;
		this->holeRadius=holeRadius;
		velocity.set_zero();
		#ifdef CONSTUCTOR_OUTPUT
		std::cout<<"InfiniteWallWithHole(Vec3D normal, Mdouble position, Mdouble holeRadius) finished"<<std::endl;
		#endif
	}	
	
	InfiniteWallWithHole (const InfiniteWallWithHole &p) : BaseWall(p)
	{
		normal=p.normal;
		position=p.position;
		factor=p.factor;
		velocity=p.velocity;
		holeRadius=p.holeRadius;
		#ifdef CONSTUCTOR_OUTPUT
		std::cout<<"InfiniteWall (const InfiniteWall &p) finished"<<std::endl;
		#endif
	}	

	///Wall copy method. It calls the copy contrustor of this Wall, usefull for polymorfism
	InfiniteWallWithHole* copy() const
	{
		return new InfiniteWallWithHole(*this);
	} 

	void clear()
	{
		indSpecies = 0;
		velocity.set_zero();
	}	  

	///Defines a standard wall, given an outward normal vector s. t. normal*x=position
	void set(Vec3D normal_, Mdouble position_, Mdouble holeRadius_) 
	{
		//factor is used to set n to unit length 
		factor = 1. / sqrt(Dot(normal_, normal_));
		normal = normal_ * factor;
		position = position_ * factor;
		holeRadius=holeRadius_;
	}

	///Allows the wall to be moved to a new position
	void move(Mdouble position_) {
		position=position_*factor;
	}

	///Allows the wall to be moved to a new position (also orthogonal to the normal), and setting the velocity
	void move(Vec3D velocity_, Mdouble dt) {
		velocity=velocity_;
		position+=Dot(velocity,normal)*dt;
	}
	
	///Allows the wall to be moved with time
	void move_time(Mdouble dt) {
		position+=Dot(velocity,normal)*dt;
	}
		
	///Returns the distance of the wall to the particle. 
	Mdouble get_wallDistance(const Vec3D &Position) {return position - Dot(Position, normal);}
	
	Mdouble get_holeDistance(const Vec3D &Position) {return holeRadius-sqrt(pow(Position.X,2)+pow(Position.Y,2));}
	
	///Since this function should be called before calculating any Particle-Wall interactions, it can also be used to set the normal vector in case of curved walls.
	bool get_distance_and_normal(BaseParticle &P, Mdouble &distance, Vec3D &normal_return)
	{
		double wallDistance = get_wallDistance(P.get_Position());
		if (wallDistance>=P.get_Radius())
			return false;
		
		double holeDistance=get_holeDistance(P.get_Position());
		if (holeDistance>=P.get_Radius())
			return false;
		
		
		if(wallDistance>0&&holeDistance>0)
		{
			distance=sqrt(pow(wallDistance,2)+pow(holeDistance,2));
			Vec3D ContactPoint;
			double alpha=atan2(P.get_Position().Y,P.get_Position().X);
			ContactPoint.X=holeRadius*cos(alpha);
			ContactPoint.Y=holeRadius*sin(alpha);
			ContactPoint.Z=position;
			//std::cout<<"ContactPoint="<<ContactPoint<<" Particle position="<<P.get_Position()<<std::endl;
			normal_return=ContactPoint-P.get_Position();
			normal_return/=normal_return.GetLength();
			//std::cout<<"Corner collision normal="<<normal_return<<std::endl;
			return true;
		}
		else if(wallDistance>holeDistance)
		{
			distance=wallDistance;
			normal_return = normal;
			//std::cout<<"Wall collision normal="<<normal_return<<std::endl;
			return true;
		}
		else
		{
			distance=holeDistance;		
			normal_return.X=P.get_Position().X/(holeRadius-holeDistance);
			normal_return.Y=P.get_Position().Y/(holeRadius-holeDistance);;
			normal_return.Z=0;
			//std::cout<<"Hole collision normal="<<normal_return<<std::endl;
			return true;
		}		
	}

	///reads wall
	void read(std::istream& is)  { 
		std::string dummy;
		is >> dummy >> normal >> dummy >> position >> dummy >> velocity;
	}

	///outputs wall
	void print(std::ostream& os) const{ 
		os << "InfiniteWallWithHole normal " << normal << " position " << position<< " holeRadius "<<holeRadius;
		//optional output
		if (velocity.GetLength2()) os << " velocity " << velocity;
	}

	///access function for normal
	Vec3D get_Normal() {return normal;}
	///access function for position
	Mdouble get_Position() {return position;}
	///access function for velocity
	Vec3D get_Velocity() const {return velocity;}
	///access function for velocity
	void set_Velocity(Vec3D new_) {velocity = new_;}

private:
	Vec3D normal;     //<outward unit normal vector
	Mdouble position;  //<position n*x=p
	Mdouble factor;    //<This is the normal to rescale to unit vectoers.
	Vec3D velocity;        ///< velocity of the wall (used to calculate the relative velocity in the force calculation)
	Mdouble holeRadius;

};

#endif
