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

#ifndef TANGENTIALSPRINGPARTICLE_H
#define TANGENTIALSPRINGPARTICLE_H


#include "BaseParticle.h"
#include "../ExtendedMath.h"
#include "../CTangentialSpring.h"

class TangentialSpringParticle : public BaseParticle
{
public:
    ///Basic Particle contructors, creates an Particle at (0,0,0) with radius, mass and inertia equal to 1
    TangentialSpringParticle();
    
    ///Particle copy contstructor, which accepts as input a reference to a Particle. It creates a copy of this Particle and all it's information. Usually it is better to use the copy() function for polymorfism.
    TangentialSpringParticle(const TangentialSpringParticle &p);
    
    ///Particle destructor, needs to be implemented and checked if it removes tangential spring information
    virtual ~TangentialSpringParticle();
    
    ///Particle copy method. It calls to copy contrustor of this Particle, usefull for polymorfism
    virtual TangentialSpringParticle* copy() const;
    
    void reverseTangentialSprings();
    
    CTangentialSprings& get_TangentialSprings();
    
	void print(std::ostream& os) const;

	void read(std::istream& is);
    
    void moveInHandler(int newPos);
	
	void oldRead(std::istream& is,std::string& x);
    
private:
    CTangentialSprings tangentialSprings;       ///Tangential spring information
};

#endif
