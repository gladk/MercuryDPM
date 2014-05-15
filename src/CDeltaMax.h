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

#ifndef CDeltaMax_H
#define CDeltaMax_H

#include "Vector.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>

class CWall;
class Particle;

/** Stores the history parameter \f$\delta_{max}\f$ needed for a plastic force.
 */
class CDeltaMax {
public:
	///constructors
	CDeltaMax() {delta=0; pParticle=-1; pWall=-1; time=-1;}
	
	CDeltaMax(Mdouble delta_, int pParticle_, int pWall_, Mdouble time_) {	
		delta = delta_;
		pParticle = pParticle_;
		pWall = pWall_;
		time = time_;
	}
	
	///copy constructor
	CDeltaMax (const CDeltaMax &ts) {
		delta = ts.delta;
		pParticle = ts.pParticle;
		pWall = ts.pWall;
		time = ts.time;
	}
	
	///outputs tangential spring
	void print(std::ostream& os) {
		os << "delta:" << delta 
			<< ", particle:" << pParticle
			<< ", wall:" << pWall 
			<< ", time:" << time;
	}
	
	///writes spring
	friend inline std::ostream& operator<<(std::ostream& os, const CDeltaMax &p) 
	{
		os << p.delta << " " << p.pParticle << " " << p.pWall << " " << p.time;
		return os;
	}	
	
	///reads spring
	friend inline std::istream& operator>>(std::istream& is, CDeltaMax &p) 
	{
		is >> p.delta >> p.pParticle >> p.pWall >> p.time;
		return is;
	}
	
//member variables
	
	///The maximum overlap on which the repellant spring strength is based 
	Mdouble delta;
	///A pointer to the particle in contact; NULL if the contact is with a wall (The other particle is the particle in which CDeltaMax is located)
	int pParticle;
	///A pointer to the wall in contact; NULL if the contact is with a particle (The other particle is the particle in which CDeltaMax is located)
	int pWall;
	///stores the last time the history parameter was read (if it was not read during the last timestep, the contact is old and will be deleted)
	Mdouble time;
};

/** Member variable of #Particle storing all history parameters \f$\delta_{max}\f$ of a particle. 
 * 
 * A tangential spring between PI and PJ can be stored in either 
 * particle depending on which particle is the first particle in 
 * MD::compute_internal_forces.
 * 
 * Geometrically, there can be no more than 13 possible contacts in 3D, so the vector size is limited.
 */
class CDeltaMaxs : public std::vector<CDeltaMax> {
public:
	///Construction required for the erase/remove_if stuff
	struct check_spring_time : public std::binary_function<CDeltaMax,Mdouble,bool>
	{
		bool operator() (const CDeltaMax a, const Mdouble b) const {return(a.time<b);}
	};
	
	///Function selects the tangential spring vector for particle-particle interations (also removed not used springs}
	Mdouble* select_particle(int P, Mdouble time_, Mdouble dt) {
		//Remove_if reconstructs the vector with only elements passing the check_spring_time function
		//Erase removes the end of the vector
		erase(remove_if(begin(),end(),bind2nd(check_spring_time(),time_)),end());	
		
		//Loops over all Springs ant returns the correct one (if it exists)
		for (CDeltaMaxs::iterator it=begin(); it!=end();it++){
			if (it->pParticle==P)
			{
				it->time=time_+dt; 
				return &it->delta;
			}
		}
		
		//if not, create it
		push_back(CDeltaMax(0,P,-1,time_+dt));	
		return &back().delta;
	}

	///Function selects the tangential spring vector for particle-wall interations
	Mdouble* select_wall(int W, Mdouble time_, Mdouble dt) {
		//Remove_if reconstructs the vector with only elements passing the check_spring_time function
		//Erase removes the end of the vector
		erase(remove_if(begin(),end(),bind2nd(check_spring_time(),time_)),end());
		
		//Loops over all Springs ant returns the correct one (if it exists)	
		for (CDeltaMaxs::iterator it=begin(); it!=end(); it++)
		{
			if (it->pWall==W)
			{
				it->time=time_+dt; 
				return &it->delta;
			}
		}
		
		//if not, create it
		push_back(CDeltaMax(0,-1,W,time_+dt));		
		return &back().delta;
	}
	
	///Resets the tangential springs
	void reset() {
		clear();
		reserve(13);
	}
	
	///outputs all current active tangential springs
	void print(std::ostream& os, Mdouble time_) {
		os << "Delta max's: N=" << size() << std::endl;
		for (CDeltaMaxs::iterator it=begin(); it!=end(); it++)
			if (it->time>=time_) {
					it->print(os); os << std::endl; 
			}
	}

public:
	///writes all springs
	friend inline std::ostream& operator<<(std::ostream& os, const CDeltaMaxs &p) 
	{
		os << p.size();
		for (unsigned int i=0; i<p.size(); i++) os <<" "<< p[i];
		return os;
	}	
	
	///reads all springs
	friend inline std::istream& operator>>(std::istream& is, CDeltaMaxs &p) 
	{
		int n; is >> n; p.resize(n);
		for (unsigned int i=0; i<p.size(); i++) is >> p[i];
		return is;
	}

};

#endif
