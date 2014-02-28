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

#ifndef PeriodicBoundary_H
#define PeriodicBoundary_H

#include "BaseBoundary.h"
#include "Particles/TangentialSpringParticle.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
///Defines a pair of periodic walls. The particles are in {x: position_left<=normal*x <position_right}, with normal being the outward unit normal vector of the right wall. If a particle moves outside these boundaries, it will be shifted.
////////////////////////////////////////////////////////////////////////////////////////////////////
class PeriodicBoundary : public BaseBoundary{
public:

	virtual PeriodicBoundary* copy() const
	{
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "virtual PeriodicBoundary* copy() const finished" << std::endl;
		#endif				
		return new PeriodicBoundary(*this);
	}
	
	
  ///Defines a periodic wall, given a 
  ///normal vector s.t. all particles are within
  ///{x: position_left<=normal*x<position_right}.
  ///The shift vector is set assuming that the domain 
  ///is rectangular (shift parallel to normal).
  void set(Vec3D normal_, Mdouble position_left_, Mdouble position_right_) {
    // factor is used to set normal to unit length
    factor = 1. / sqrt(Dot(normal_, normal_));
    normal = normal_ * factor;
    position_left = position_left_ * factor;
    position_right = position_right_ * factor;
    shift = normal * (position_right - position_left);
  }

  ///For general parallelogramic domains, the direction 
  ///of the shift vector has to be set manually.
  void set(Vec3D normal_, Mdouble position_left_, Mdouble position_right_, Vec3D shift_direction) {
    // factor is used to set normal to unit length
    factor = 1. / sqrt(Dot(normal_, normal_));
    normal = normal_ * factor;
    position_left = position_left_ * factor;
    position_right = position_right_ * factor;
    // factor is used to set shift vector to correct length
    factor = (position_right - position_left) * Dot(shift_direction, normal);
    shift = shift_direction * factor;
  }
  
  ///Allows the left periodic wall to be moved to a new position and automatically changes its shift value
  void move_left(Mdouble position_)
  {
	  position_left=position_*factor;
	  shift = normal * (position_right - position_left);
  }
  
  ///Allows the right periodic wall to be moved to a new position and automatically changes its shift value
  void move_right(Mdouble position_)
  {
	  position_right=position_*factor;
	  shift = normal * (position_right - position_left);
  }


  ///Returns the distance of the wall to the particle, and 
  ///sets left_wall = true, if the left wall is the wall 
  ///closest to the particle.
  ///Since this function should be called before 
  ///calculating any Particle-Wall interactions, 
  ///it can also be used to set the shift vector 
  ///in case of curved walls.
  Mdouble distance(BaseParticle &P) {
    return distance(P.get_Position());
  }

  Mdouble distance(const Vec3D &P) {
    Mdouble position = Dot(P, normal);

    if (position - position_left < position_right - position) {
      left_wall = true;
      return position - position_left;
    } else {
      left_wall = false;
      return position_right - position;
    }
  }

	///shifts the particle (after distance set the left_wall value)
	void shift_position(BaseParticle* F0) {
		if (left_wall) {
			F0->move(shift);
			left_wall = false;
		}
		else {
			F0->move(-shift);
			left_wall = true;
		}
	}

	///returns the shifted particle w/o actually shifting
	Vec3D get_shifted_position(Vec3D &Position) {
		if (left_wall) {
			return Position + shift;
		}
		else {
			return Position - shift;
		}
	}
	
	///shifts two particles
	void shift_positions(Vec3D &PI, Vec3D &PJ) {
		if (left_wall) {
			PI += shift;
			PJ += shift;
			left_wall = false;
		}
		else {
			PI -= shift;
			PJ -= shift;
			left_wall = true;
		}
	}
		
	///shift P such that it is closest to Q
	void get_close_together(Vec3D &P,Vec3D &Q) {
		Mdouble PQdotn = Dot(P-Q, normal);
		Mdouble shift_norm2 = shift.GetLength2();
		//Check if P is so far from Q that a shift would move it closer
		if (sqr(PQdotn) > .25 * shift_norm2) {
			//Now determine the direction of the shift
			if (PQdotn>0.0) P -= shift;
			else P += shift;
		}
	}
		

	///reads wall
	void read(std::istream& is) { 
    std::string dummy;
		is >> dummy >> normal >> dummy >> position_left >> dummy >> position_right >> dummy >> shift;
	}
	
	///outputs wall
	void print(std::ostream& os) const {
		os << "PeriodicBoundary normal " << normal 
			<< " position_left " << position_left 
			<< " position_right " << position_right
		 	<< " shift " << shift;
	}
		
	Vec3D& get_normal() {return normal;}
	
	int createPeriodicParticles(BaseParticle *P, ParticleHandler &pH)
	{
		if (distance(*P)<P->get_InteractionRadius()+pH.getLargestParticle()->get_InteractionRadius())
		{
			BaseParticle* F0=P->copy();
			shift_position(F0);
            
            //If the Particle includes TangentalSprings reverse them
            TangentialSpringParticle* TSParticle=dynamic_cast<TangentialSpringParticle*>(F0);
			if(TSParticle)
                TSParticle->reverseTangentialSprings();
			
			//If Particle is Mdouble shifted, get correct original particle
			BaseParticle* From=P;
			while(From->get_PeriodicFromParticle()!=NULL)
				From=From->get_PeriodicFromParticle();		
			F0->set_periodicFromParticle(From);

			pH.addObject(F0);
			return 1;
		}
		return 0;
	}		

	bool checkBoundaryAfterParticleMoved(BaseParticle *P, ParticleHandler &pH UNUSED)
	{
		if (distance(*P)<0)
		{
			shift_position(P);
		}
		return false;
	}
	
 private:
  bool left_wall;        ///< true if closest wall is the left wall
  Vec3D normal;          ///< outward unit normal vector for right wall
  Mdouble position_left;  ///< position of left wall, s.t. normal*x=position_left
  Mdouble position_right; ///< position of right wall, s.t. normal*x=position_right
  Mdouble factor;    	 ///< This is the normal to rescale to unit vectoers.  
  Vec3D shift;           ///< shift from left to right boundary
};
#endif
