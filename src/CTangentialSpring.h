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

#ifndef CTANGENTIALSPRING_H
#define CTANGENTIALSPRING_H

#include "Vector.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>
#include <functional>

/** Stores the tangential spring \f$\vec{\delta}_{ij}^t\f$ needed to compute a tangential elastic force between particles PI, PJ.
 */
class CTangentialSpring {
public:
	///constructors
	CTangentialSpring() {
		delta.set_zero(); 
		RollingSpring.set_zero(); 
		TorsionSpring.set_zero(); 
		SlidingForce.set_zero(); 
		pParticle=-1; 
		pWall=-1; 
		time=-1; 
		sliding = false; 
		slidingRolling = false; 
		slidingTorsion = false; 
	}
	
	CTangentialSpring(int pParticle_, int pWall_, Mdouble time_) {	
		delta.set_zero();
		RollingSpring.set_zero();
		TorsionSpring.set_zero();
		SlidingForce.set_zero(); 
		pParticle = pParticle_;
		pWall = pWall_;
		time = time_;
		sliding = false;
		slidingRolling = false; 
		slidingTorsion = false; 
	}
	
	///copy constructor
	CTangentialSpring (const CTangentialSpring &ts) {
		delta = ts.delta;
		RollingSpring = ts.RollingSpring;
		TorsionSpring = ts.TorsionSpring;
		SlidingForce = ts.SlidingForce; 
		pParticle = ts.pParticle;
		pWall = ts.pWall;
		time = ts.time;
		sliding = ts.sliding;
		slidingRolling = ts.slidingRolling;
		slidingTorsion = ts.slidingTorsion;
	}
	
	///outputs tangential spring
	void print(std::ostream& os) {
		os << "delta:" << delta 
			<< ", particle:" << pParticle
			<< ", wall:" << pWall 
			<< ", time:" << time;
	}
	
	///writes spring
	friend inline std::ostream& operator<<(std::ostream& os, const CTangentialSpring &p) 
	{
		os << p.delta << " " << p.RollingSpring << " " << p.TorsionSpring << " " << p.pParticle << " " << p.pWall << " " << p.time;
		return os;
	}	
	
	///reads spring
	friend inline std::istream& operator>>(std::istream& is, CTangentialSpring &p) 
	{
		is >> p.delta >> p.RollingSpring >> p.TorsionSpring >> p.pParticle >> p.pWall >> p.time;
		return is;
	}
	
	void reverse()
	{
		delta=-delta;
		RollingSpring=-RollingSpring;
		TorsionSpring=-TorsionSpring;
		SlidingForce=-SlidingForce;		
	}	
	
	///stores the spring
	Vec3D delta;
	Vec3D RollingSpring;
	Vec3D TorsionSpring;
	///Stores the force (for some non-linear, hysteretic spring models)
	Vec3D SlidingForce;
	///A pointer to the particle in contact; NULL if the contact is with a wall (The other particle is the particle in which CDeltaMax is located)
	int pParticle;
	///A pointer to the wall in contact; NULL if the contact is with a particle (The other particle is the particle in which CDeltaMax is located)
	int pWall;
	///stores the last time the tangential spring was read (if it was not read during the last timestep, the contact is old and will be deleted)
	Mdouble time;

	bool sliding;
	bool slidingRolling;
	bool slidingTorsion;
};

/** Member variable of #Particle storing all tangential springs \f$\vec{\delta}_{ij}^t\f$ of particle PI with contacting particles PJ. 
 * 
 * A tangential spring between PI and PJ can be stored in either 
 * particle depending on which particle is the first particle in 
 * MD::compute_internal_forces.
 * 
 * Geometrically, there can be no more than 13 possible contacts in 3D, so the vector size is limited.
 */
class CTangentialSprings : public std::vector<CTangentialSpring> {
public:

	///Construction required for the erase/remove_if stuff
	struct check_spring_time : public std::binary_function<CTangentialSpring,Mdouble,bool>
	{
		bool operator() (const CTangentialSpring a, const Mdouble b) const {return(a.time<b);}
	};
	
	///Function selects the tangential spring vector for particle-particle interations (also removed not used springs}
    std::vector<int> to_be_erased(Mdouble time) {
    std::vector<int> retval;
		for (unsigned int i=0; i<size(); i++)
			if (operator[](i).time<time) {
				retval.push_back(i);
				//std::cout << "to be erased " << time << " " << operator[](i).pParticle << " " << operator[](i).pWall << " " << size() << std::endl;
			}
		return retval;
	}

	///Function selects the tangential spring vector for particle-particle interations (also removed not used springs}
	void to_erase(Mdouble time) {
		erase(remove_if(begin(),end(),bind2nd(check_spring_time(),time)),end());	
	}

	///Function selects the tangential spring vector for particle-particle interations (also removed not used springs}
	CTangentialSpring* select_particle_spring(int P, Mdouble time_, Mdouble dt) {
		//Remove_if reconstructs the vector with only elements passing the check_spring_time function
		//Erase removes the end of the vector
		erase(remove_if(begin(),end(),bind2nd(check_spring_time(),time_)),end());	
		
		//Loops over all Springs and returns the correct one (if it exists)
		for (CTangentialSprings::iterator it=begin(); it!=end();it++){
			if (it->pParticle==P)
			{
				it->time=time_+dt; 
				return &(*it);
			}
		}	
		return 0;
	}
	
	CTangentialSpring* create_new(int P, Mdouble time_, Mdouble dt)
	{
		//if not, create it
		push_back(CTangentialSpring(P,-1,time_+dt));
		return &back();
	}

	CTangentialSpring* create_new(int P, Mdouble time_, Mdouble dt, CTangentialSpring& CTS)
	{
		//if not, create it
		push_back(CTangentialSpring(CTS));
		back().pParticle=P;
		back().time=time_+dt;
		return &back();
	}
	
	CTangentialSpring* create_new_wall(int W, Mdouble time_, Mdouble dt)
	{
		//if not, create it
        push_back(CTangentialSpring(-1,W,time_+dt));
        return &(back());
	}

    
	bool has_particle_spring(int P, Mdouble time_) {
		//Remove_if reconstructs the vector with only elements passing the check_spring_time function
		//Erase removes the end of the vector
		erase(remove_if(begin(),end(),bind2nd(check_spring_time(),time_)),end());	
		//Loops over all Springs and returns the correct one (if it exists)
		for (CTangentialSprings::iterator it=begin(); it!=end();it++){
			if (it->pParticle==P)
			{
				return true;
			}
		}
		//if not, create it
		return false;
	}


	///Function selects the tangential spring vector for particle-particle interations (also removed not used springs}
	CTangentialSpring* select_wall_spring(int W, Mdouble time_, Mdouble dt) {
		//Remove_if reconstructs the vector with only elements passing the check_spring_time function
		//Erase removes the end of the vector
		erase(remove_if(begin(),end(),bind2nd(check_spring_time(),time_)),end());	
		
		//Loops over all Springs ant returns the correct one (if it exists)
		for (CTangentialSprings::iterator it=begin(); it!=end();it++){
			if (it->pWall==W)
			{
				it->time=time_+dt;
				return &(*it);
			}
		}
		
		//if not, create it
		//~ std::cout << "created pw " << time_ << " " << W << std::endl;
        return 0;
	}

	///Resets the tangential springs
	void reset() {
		clear();
		reserve(13);
	}
	
	///outputs all current active tangential springs
	void print(std::ostream& os, Mdouble time_) {
		os << "Tangential Springs: N=" << size() << std::endl;
		for (CTangentialSprings::iterator it=begin(); it!=end(); it++)
			if (it->time>=time_) {
					it->print(os); os << std::endl; 
			}
	}

public:
	///writes all springs
	friend inline std::ostream& operator<<(std::ostream& os, const CTangentialSprings &p) 
	{
		os << p.size();
		for (unsigned int i=0; i<p.size(); i++) os << " " << p[i];
		return os;
	}	
	
	///reads all springs
	friend inline std::istream& operator>>(std::istream& is, CTangentialSprings &p) 
	{
		//should return empty spring if read from is failed
		int n; is >> n; 
		if (is.fail()) {
			p.resize(0);
		} else {
			p.resize(n);
			for (unsigned int i=0; i<p.size(); i++) is >> p[i];
		}
		return is;
	}

};

#endif
