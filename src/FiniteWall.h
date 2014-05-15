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


#ifndef FINITEWALL_H
#define FINITEWALL_H

#include "BaseWall.h"
#include "InfiniteWall.h"

class FiniteWall : public BaseWall
{
	public:
	FiniteWall() : BaseWall()
	{
		#ifdef CONSTUCTOR_OUTPUT
		std::cout<<"InfiniteWall () finished"<<std::endl;
		#endif	
	}

	///Wall copy method. It calls the copy contrustor of this Wall, usefull for polymorfism
	virtual FiniteWall* copy() const
	{
		return new FiniteWall(*this);
	} 

	void clear()
	{
		indSpecies = 0;
		velocity.set_zero();
		finite_walls.clear();
	}	  

	///Adds a wall to the set of finite walls, given an outward normal vector s.t. normal*x=normal*point
	void add_finite_wall(Vec3D normal, Vec3D point) {
		add_finite_wall(normal,Dot(normal,point));
	}
	///Adds a wall to the set of finite walls, given an outward normal vector s. t. normal*x=position
	void add_finite_wall(Vec3D normal_, Mdouble position_) 
	{
		//n is the index of the new wall
		int n = finite_walls.size();
		finite_walls.resize(n+1);
		finite_walls[n].set(normal_,position_);

		// AB[n*(n-1)/2+m] is the direction of the intersecting line between walls m and n, m<n
		// A[n*(n-1)/2+m] is a point on the intersecting line between walls m and n, m<n
		// See http://www.netcomuk.co.uk/~jenolive/vect18d.html for finding the line where two planes meet
		AB.resize(n*(n+1)/2);
		A.resize (n*(n+1)/2);
		for(int m=0; m<n; m++) 
		{
			int id = (n-1)*n/2+m;
			//first we cross the wall normals and normalize to obtain AB
			AB[id] = Cross(finite_walls[m].get_Normal(), finite_walls[n].get_Normal());
			AB[id] /= sqrt(AB[id].GetLength2());
			//then we find a point A (using AB*x=0 as a third plane)
			Mdouble invdet = 1.0/(	+finite_walls[n].get_Normal().X*(finite_walls[m].get_Normal().Y*AB[id].Z-AB[id].Y*finite_walls[m].get_Normal().Z)
									-finite_walls[n].get_Normal().Y*(finite_walls[m].get_Normal().X*AB[id].Z-finite_walls[m].get_Normal().Z*AB[id].X)
									+finite_walls[n].get_Normal().Z*(finite_walls[m].get_Normal().X*AB[id].Y-finite_walls[m].get_Normal().Y*AB[id].X));
			A[id] = Vec3D(	+(finite_walls[m].get_Normal().Y*AB[id].Z-AB[id].Y*finite_walls[m].get_Normal().Z)*finite_walls[n].get_Position()
							-(finite_walls[n].get_Normal().Y*AB[id].Z-finite_walls[n].get_Normal().Z*AB[id].Y)*finite_walls[m].get_Position()
							+(finite_walls[n].get_Normal().Y*finite_walls[m].get_Normal().Z-finite_walls[n].get_Normal().Z*finite_walls[m].get_Normal().Y)*0.0,
							-(finite_walls[m].get_Normal().X*AB[id].Z-finite_walls[m].get_Normal().Z*AB[id].X)*finite_walls[n].get_Position()
							+(finite_walls[n].get_Normal().X*AB[id].Z-finite_walls[n].get_Normal().Z*AB[id].X)*finite_walls[m].get_Position()
							-(finite_walls[n].get_Normal().X*finite_walls[m].get_Normal().Z-finite_walls[m].get_Normal().X*finite_walls[n].get_Normal().Z)*0.0,
							+(finite_walls[m].get_Normal().X*AB[id].Y-AB[id].X*finite_walls[m].get_Normal().Y)*finite_walls[n].get_Position()
							-(finite_walls[n].get_Normal().X*AB[id].Y-AB[id].X*finite_walls[n].get_Normal().Y)*finite_walls[m].get_Position()
							+(finite_walls[n].get_Normal().X*finite_walls[m].get_Normal().Y-finite_walls[m].get_Normal().X*finite_walls[n].get_Normal().Y)*0.0 ) * invdet;
		}

		// C[(n-2)*(n-1)*n/6+(m-1)*m/2+l] is a point intersecting walls l, m and n, l<m<n
		C.resize((n-1)*n*(n+1)/6);
		for(int m=0; m<n; m++) 
		{
			for(int l=0; l<m; l++) 
			{
				int id = (n-2)*(n-1)*n/6+(m-1)*m/2+l;
				Mdouble invdet = 1.0/(	+finite_walls[n].get_Normal().X*(finite_walls[m].get_Normal().Y*finite_walls[l].get_Normal().Z-finite_walls[l].get_Normal().Y*finite_walls[m].get_Normal().Z)
										-finite_walls[n].get_Normal().Y*(finite_walls[m].get_Normal().X*finite_walls[l].get_Normal().Z-finite_walls[m].get_Normal().Z*finite_walls[l].get_Normal().X)
										+finite_walls[n].get_Normal().Z*(finite_walls[m].get_Normal().X*finite_walls[l].get_Normal().Y-finite_walls[m].get_Normal().Y*finite_walls[l].get_Normal().X));
				C[id] = Vec3D(	+(finite_walls[m].get_Normal().Y*finite_walls[l].get_Normal().Z-finite_walls[l].get_Normal().Y*finite_walls[m].get_Normal().Z)*finite_walls[n].get_Position()
								-(finite_walls[n].get_Normal().Y*finite_walls[l].get_Normal().Z-finite_walls[n].get_Normal().Z*finite_walls[l].get_Normal().Y)*finite_walls[m].get_Position()
								+(finite_walls[n].get_Normal().Y*finite_walls[m].get_Normal().Z-finite_walls[n].get_Normal().Z*finite_walls[m].get_Normal().Y)*finite_walls[l].get_Position(),
								-(finite_walls[m].get_Normal().X*finite_walls[l].get_Normal().Z-finite_walls[m].get_Normal().Z*finite_walls[l].get_Normal().X)*finite_walls[n].get_Position()
								+(finite_walls[n].get_Normal().X*finite_walls[l].get_Normal().Z-finite_walls[n].get_Normal().Z*finite_walls[l].get_Normal().X)*finite_walls[m].get_Position()
								-(finite_walls[n].get_Normal().X*finite_walls[m].get_Normal().Z-finite_walls[m].get_Normal().X*finite_walls[n].get_Normal().Z)*finite_walls[l].get_Position(),
								+(finite_walls[m].get_Normal().X*finite_walls[l].get_Normal().Y-finite_walls[l].get_Normal().X*finite_walls[m].get_Normal().Y)*finite_walls[n].get_Position()
								-(finite_walls[n].get_Normal().X*finite_walls[l].get_Normal().Y-finite_walls[l].get_Normal().X*finite_walls[n].get_Normal().Y)*finite_walls[m].get_Position()
								+(finite_walls[n].get_Normal().X*finite_walls[m].get_Normal().Y-finite_walls[m].get_Normal().X*finite_walls[n].get_Normal().Y)*finite_walls[l].get_Position() ) * invdet;
			}
		}
	}

	void create_open_prism_wall(std::vector<Vec3D> Points, Vec3D PrismAxis) 
	{
		finite_walls.clear();
		for(unsigned int i=0; i<Points.size()-1; i++)
        	add_finite_wall(Cross(Points[i]-Points[i+1],PrismAxis),Points[i]);
	}

	void create_prism_wall(std::vector<Vec3D> Points, Vec3D PrismAxis) 
	{
		create_open_prism_wall(Points, PrismAxis);
        add_finite_wall(Cross(Points.back()-Points.front(),PrismAxis),Points.front());
	}

	void create_open_prism_wall(std::vector<Vec3D> Points) 
	{
		Vec3D PrismAxis = Cross(
			GetUnitVector(Points[1]-Points[0]),
			GetUnitVector(Points[2]-Points[1]));
		finite_walls.clear();
		for(unsigned int i=0; i<Points.size()-1; i++)
        	add_finite_wall(Cross(Points[i]-Points[i+1],PrismAxis),Points[i]);
	}

	void create_prism_wall(std::vector<Vec3D> Points) 
	{
		Vec3D PrismAxis = Cross(
			GetUnitVector(Points[2]-Points[0]),
			GetUnitVector(Points[1]-Points[0]));
		create_open_prism_wall(Points, PrismAxis);
        add_finite_wall(Cross(Points.back()-Points.front(),PrismAxis),Points.front());
	}

	///Since this function should be called before calculating any Particle-Wall interactions, it can also be used to set the normal vector in case of curved walls.
	bool get_distance_and_normal(BaseParticle &P, Mdouble &distance, Vec3D &normal_return)
	{
		static Mdouble distance_new;
		static Mdouble distance2;  
		static Mdouble distance3;
		static int id;  
		static int id2;  
		static int id3;  

		//if we are here, this is a finite wall
		distance = -1e20;
		distance2 = -1e20;
		distance3 = -1e20;
		//for all finite walls
		for (unsigned int i=0; i<finite_walls.size(); i++)
		{
			//calculate the distance to the particle
			distance_new = finite_walls[i].get_distance(P.get_Position());
			//return false if the distance to any one wall is too large (i.e. no contact)
			if (distance_new>=P.get_WallInteractionRadius()) return false;
			//store the minimum distance and the respective wall in "distance" and "id"
			//and store up to two walls (id2, id3) and their distances (distance2, distance3), if the possible contact point is near the intersection between id and id2 (and id3)
			if (distance_new>distance)
			{
				if (distance>-P.get_WallInteractionRadius())
				{
					if (distance2>-P.get_WallInteractionRadius()) {distance3 = distance; id3 = id;}
					else {distance2 = distance; id2 = id;}
				} 
				distance = distance_new; id = i;
			} else if (distance_new>-P.get_WallInteractionRadius())
			{
				if (distance2>-P.get_WallInteractionRadius()) {distance3 = distance_new; id3 = i;}
				else {distance2 = distance_new; id2 = i;}
			}
		}

		//if we are here, the closest wall is id; 
		//if distance2>-P.Radius (and distance3>-P.Radius), the possible contact point is near the intersection between id and id2 (and id3)
		if (distance2>-P.get_WallInteractionRadius())
		{
			//D is the point on wall id closest to P
			Vec3D D = P.get_Position() + finite_walls[id].get_Normal() * distance;
			//If the distance of D to id2 is positive, the contact is with the intersection
			bool intersection_with_id2 = (finite_walls[id2].get_distance(D)>0.0);

			if (distance3>-P.get_WallInteractionRadius()&&(finite_walls[id3].get_distance(D)>0.0))
			{
				if (intersection_with_id2)
				{
						//possible contact is with intersection of id,id2,id3
					//we know id2<id3
					int index = 
					(id<id2)?( (id3-2)*(id3-1)*id3/6+(id2-1)*id2/2+id  ):
					(id<id3)?( (id3-2)*(id3-1)*id3/6+(id -1)*id /2+id2 ):
					( (id -2)*(id -1)*id /6+(id3-1)*id3/2+id2 );
					normal_return = P.get_Position() - C[index];
					distance = sqrt(normal_return.GetLength2());
					if (distance>=P.get_WallInteractionRadius()) return false; //no contact
					normal_return /= -distance; 
					return true; //contact with id,id2,id3
				} else { intersection_with_id2 = true; distance2 = distance3; id2 = id3; }
			}

			if (intersection_with_id2)
			{ //possible contact is with intersection of id,id2
				int index = (id>id2)?((id-1)*id/2+id2):((id2-1)*id2/2+id);
				Vec3D AC = P.get_Position() - A[index];
				normal_return = AC - AB[index] * Dot(AC,AB[index]);
				distance = sqrt(normal_return.GetLength2());
				if (distance>=P.get_WallInteractionRadius()) return false; //no contact
				normal_return /= -distance; 
				return true; //contact with two walls
			}
		}
		//contact is with id
		normal_return = finite_walls[id].get_Normal(); 
		return true;

	}


	///reads wall
	void read(std::istream& is)
	{ 
		std::string dummy;
		int n;
		is >> dummy >> n;

		Vec3D normal;
		Mdouble position;
		for (int i=0; i<n; i++)
		{
			is >> dummy >> normal >> dummy >> position;
			add_finite_wall(normal,position);
		}		
		is >> dummy >> velocity;
	}

	///outputs wall
	void print(std::ostream& os) const
	{
		os << "FiniteWall numFiniteWalls " << finite_walls.size(); 
		for (std::vector<InfiniteWall>::const_iterator it = finite_walls.begin(); it!=finite_walls.end(); ++it)
		{
			os << " normal " << it->get_Normal() << " position " << it->get_Position();
		}
		if (velocity.GetLength2()) os << " velocity " << velocity;
	}

	///access function for velocity
	Vec3D get_Velocity() const  {return velocity;}

	private:
	std::vector<InfiniteWall> finite_walls;
	std::vector<Vec3D> A;  //<A[n*(n-1)/2+m] is a point on the intersecting line between walls m and n, m<n
	std::vector<Vec3D> AB; //<AB[n*(n-1)/2+m] is the direction of the intersecting line between walls m and n, m<n
	std::vector<Vec3D> C;  //<C[(n-2)*(n-1)*n/6+(m-1)*m/2+l] is a point intersecting walls l, m and n, l<m<n

};

#endif
