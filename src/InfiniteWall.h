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

#ifndef INFINITEWALL_H
#define INFINITEWALL_H

////////////////////////////////////////////////////////////////////////////////////////////////////
///Definition of a wall (planar or finite). 
///A standard wall is a plane defined  as {x: normal*x=position}, with normal being the outward unit normal vector of the wall.
///A particle touches a standard wall if position-normal*x<=radius.
///A finite wall is convex polygon defined by a set of normals normal_i and positions position_i.
///A particle touches a finite wall if position_i-normal_i*x<=radius for all i.
/// \todo remove access to set Nwalls and wall set etc.. and add function add_wall. This would have stoped a lot of bugs in my own (Anthony's) driver codes.
////////////////////////////////////////////////////////////////////////////////////////////////////
class InfiniteWall : public BaseWall
{
	public:
	InfiniteWall() : BaseWall()
	{
		#ifdef CONSTUCTOR_OUTPUT
            std::cout<<"InfiniteWall () finished"<<std::endl;
		#endif
	}
	
	InfiniteWall (const InfiniteWall &p) : BaseWall(p)
	{
		normal=p.normal;
		position=p.position;
		factor=p.factor;
		#ifdef CONSTUCTOR_OUTPUT
		std::cout<<"InfiniteWall (const InfiniteWall &p) finished"<<std::endl;
		#endif
	}	

	///Wall copy method. It calls the copy contrustor of this Wall, usefull for polymorfism
	InfiniteWall* copy() const
	{
		return new InfiniteWall(*this);
	} 

	void clear()
	{
		indSpecies = 0;
		velocity.set_zero();
	}	  

	///Defines a standard wall, given an outward normal vector s. t. normal*x=position
	void set(Vec3D normal_, Mdouble position_) 
	{
		//factor is used to set n to unit length 
		factor = 1. / sqrt(Dot(normal_, normal_));
		normal = normal_ * factor;
		position = position_ * factor;		
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
	Mdouble get_distance(const Vec3D &Position) {return position - Dot(Position, normal);}

	///Since this function should be called before calculating any Particle-Wall interactions, it can also be used to set the normal vector in case of curved walls.
	bool get_distance_and_normal(BaseParticle &P, Mdouble &distance, Vec3D &normal_return)
	{
		distance = get_distance(P.get_Position());
		if (distance>=P.get_WallInteractionRadius())
			return false;
		normal_return = normal;
		return true;
	}

	///reads wall
	void read(std::istream& is)  { 
		std::string dummy;
		is >> dummy >> normal >> dummy >> position >> dummy >> velocity;
	}

	///outputs wall
	void print(std::ostream& os) const { 
		os << "InfiniteWall normal " << normal << " position " << position;
		//\todo{species should be written}
		//optional output
		if (velocity.GetLength2()) os << " velocity " << velocity;
	}

	///access function for normal
	Vec3D get_Normal() const {return normal;}
	///access function for position
	Mdouble get_Position() const  {return position;}

	///access function for velocity
	Vec3D get_Velocity() const  {return velocity;}
	
private:
	Vec3D normal;     //<outward unit normal vector
	Mdouble position;  //<position n*x=p
	Mdouble factor;    //<This is the normal to rescale to unit vectoers.
};

#endif
