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
