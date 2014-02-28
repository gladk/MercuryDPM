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

#ifndef BaseBoundary_H
#define BaseBoundary_H

#include "WallHandler.h"
#include "ParticleHandler.h"
#include "RNG.h"


class BaseBoundary 
{
	public:
	///BaseBoundary copy method. It calls the copy constructor of this BaseBoundary, useful for polymorphism
	///todo{Does this work correctly?}
	virtual BaseBoundary* copy() const = 0;
	virtual ~BaseBoundary() {};
	

	virtual int createPeriodicParticles(BaseParticle *P UNUSED, ParticleHandler &pH UNUSED){return 0;};
	
	virtual bool checkBoundaryAfterParticleMoved(BaseParticle *P UNUSED, ParticleHandler &pH UNUSED){return false;};
    
    virtual void checkBoundaryActionsBeforeTimeStep(ParticleHandler &pH UNUSED, WallHandler &wH UNUSED, RNG &random UNUSED){};
    
    void set_Index(int index) {_index=index;}
    void set_Id(int id) {_id=id;}
    void setHandler(BoundaryHandler* handler) {_handler=handler;}
	
	///writes wall
	friend inline std::ostream& operator<<(std::ostream& os, const BaseBoundary &b);
	
	///reads wall
	friend inline std::istream& operator>>(std::istream& is, BaseBoundary &b);

	///reads boundary
	virtual void read(std::istream& is UNUSED) = 0;
	
	///outputs boundary
	virtual void print(std::ostream& os UNUSED) const = 0;
    
    virtual void moveInHandler(int newPos){_index=newPos;}
    
    private:
    BoundaryHandler* _handler;
    int _index;
    int _id;
};

inline std::ostream& operator<<(std::ostream& os, const BaseBoundary &b)
{
	b.print(os);
	return os;
}

inline std::istream& operator>>(std::istream& is, BaseBoundary &b)
{
	b.read(is);
	return(is);
}
#endif
