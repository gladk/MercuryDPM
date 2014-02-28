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

#ifndef DELTAMAXSPARTICLE_H
#define DELTAMAXSPARTICLE_H

#include "TangentialSpringParticle.h"
#include "../CDeltaMax.h"

class DeltaMaxsParticle : public TangentialSpringParticle
{
public:
    ///Basic Particle contructors, creates an Particle at (0,0,0) with radius, mass and inertia equal to 1
    DeltaMaxsParticle();
    
    ///Particle copy contstructor, which accepts as input a reference to a Particle. It creates a copy of this Particle and all it's information. Usually it is better to use the copy() function for polymorfism.
    DeltaMaxsParticle(const DeltaMaxsParticle &p);
    
    DeltaMaxsParticle(const TangentialSpringParticle &p);

    ///Particle destructor, needs to be implemented and checked if it removes tangential spring information
    virtual ~DeltaMaxsParticle();
    
    ///Particle copy method. It calls to copy contrustor of this Particle, usefull for polymorfism
    virtual DeltaMaxsParticle* copy() const;
    
	void print(std::ostream& os) const;

	void read(std::istream& is);
    
    CDeltaMaxs& get_DeltaMaxs();
    
private:
    CDeltaMaxs deltaMaxs;                       ///CDeltaMax information
};

#endif
