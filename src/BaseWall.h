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

#ifndef BASEWALL_H
#define BASEWALL_H

#include "Particles/BaseParticle.h"
//Note the get_Velocity can for some walls be dependend on which point on the wall is meant.

class BaseWall 
{	
	public:
 
	int indSpecies;

	BaseWall()
	{
		velocity.set_zero();
		force.set_zero();
		torque.set_zero();
		indSpecies=0;
        _handler=0;
        _index=0;
        _id=0;
        
		#ifdef CONSTUCTOR_OUTPUT
            std::cout<<"BaseWall() finished"<<std::endl;
		#endif
	}

	BaseWall(const BaseWall &p)
	{
		velocity=p.velocity;
		force=p.force;
		torque=p.torque;
		indSpecies=p.indSpecies;
        _handler=p._handler;
        _index=p._index;
        _id=p._id;
        
		#ifdef CONSTUCTOR_OUTPUT
            std::cout<<"BaseWall(const BaseWall &p) finished"<<std::endl;
		#endif
	}
	
	virtual ~BaseWall() {};

	virtual BaseWall* copy() const  = 0;
	
	virtual void clear() {std::cout<<"BaseWall::clear(), this function shouldn't be called"<<std::endl;}
	
	///Allows the wall to be moved to a new position
	virtual void move(Mdouble position_ UNUSED) {std::cout<<"BaseWall:move(Mdouble position_ UNUSED), this function shouldn't be called"<<std::endl;}

	///Allows the wall to be moved to a new position (also orthogonal to the normal), and setting the velocity
	virtual void move(Vec3D velocity_ UNUSED, Mdouble dt UNUSED) {std::cout<<"BaseWall::move(Vec3D velocity_, Mdouble dt UNUSED), this function shouldn't be called"<<std::endl;}
	
	///Allows the wall to be moved to a new position (also orthogonal to the normal), and setting the velocity
	virtual void move_time(Mdouble dt UNUSED) {std::cout<<"BaseWall::move_time(Mdouble dt UNUSED), this function shouldn't be called"<<std::endl;}

	virtual bool get_distance_and_normal(BaseParticle &P UNUSED, Mdouble &distance UNUSED, Vec3D &normal_return UNUSED) = 0;

	///access function for velocity
	void set_Velocity(Vec3D new_) {velocity=new_;}
	
    virtual Vec3D get_Velocity() const = 0;
    
    friend inline std::ostream& operator<<(std::ostream& os, BaseWall &w);

	friend inline std::istream& operator>>(std::istream& is, BaseWall &w);
	
    virtual void read(std::istream& is UNUSED) = 0;
	
    virtual void print(std::ostream& os UNUSED) const = 0;
    
    virtual void moveInHandler(int newPos){_index=newPos;}
    
    int              getIndSpecies() const           {return indSpecies;}
  
    void             setIndSpecies(const int _new)   {indSpecies=_new;}

    const Vec3D& get_Force() const {return force;}
    const Vec3D& get_Torque() const	{return torque;}
    void set_Force(Vec3D _new) {force=_new;}
    void set_Torque(Vec3D _new)	{torque=_new;}
    void set_Index(int index) {_index=index;}
    void set_Id(int id) {_id=id;}    
    void add_Force					(Vec3D _new)		{force+=_new;}
    void add_Torque					(Vec3D _new)		{torque+=_new;}

protected:
	Vec3D velocity;        ///< velocity of the wall (used to calculate the relative velocity in the force calculation)
private:
    Vec3D force;
    Vec3D torque;
    WallHandler* _handler;    
    int _index;
    int _id;

};

inline std::ostream& operator<<(std::ostream& os, BaseWall &w)
{
	w.print(os);
	return os;
}

inline std::istream& operator>>(std::istream& is, BaseWall &w)
{
	w.read(is);
	return(is);
}
#endif
