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

#ifndef BOUNDARIES_INSERTIONBOUNDARY_H
#define BOUNDARIES_INSERTIONBOUNDARY_H

#include "../BaseWall.h"

class InsertionBoundary : public BaseBoundary
{
	public:
	virtual InsertionBoundary* copy() const
	{
		#ifdef CONSTUCTOR_OUTPUT
			cerr << "virtual InsertionBoundary* copy() const finished" << endl;
		#endif		
		return new InsertionBoundary(*this);
	}
    
    void set(Vec3D posMin, Vec3D posMax, Vec3D velMin, Vec3D velMax, Mdouble radMin, Mdouble radMax, int maxFailed)
    {
        posMin_=posMin;
        posMax_=posMax;
        velMin_=velMin;
        velMax_=velMax;
        radMin_=radMin;
        radMax_=radMax;
        maxFailed_=maxFailed;        
    }
      
    virtual void checkBoundaryActionsBeforeTimeStep(ParticleHandler &pH, WallHandler &wH, RNG &random)
    {
        int failed = 0;
        BaseParticle p0;
        bool isInsertable;
        Mdouble distance;
        Vec3D normal;
        //try max_failed times to find new insertable particle
        while (failed<=maxFailed_)
        {
            std::cout << "I" << std::endl;
            //Set random radius, position and velocity
            p0.set_Radius(random.get_RN(radMin_,radMax_));
            p0.set_Mass(1.0);                              
            p0.set_inertia(1.0);
            Vec3D position;
            position.X = random.get_RN(posMin_.X,posMax_.X);
            position.Y = random.get_RN(posMin_.Y,posMax_.Y);
            position.Z = random.get_RN(posMin_.Z,posMax_.Z);
            Vec3D velocity;
            velocity.X = random.get_RN(velMin_.X,velMax_.X);
            velocity.Y = random.get_RN(velMin_.Y,velMax_.Y);
            velocity.Z = random.get_RN(velMin_.Z,velMax_.Z);
            p0.set_Position(position);
            p0.set_Velocity(velocity);
            // p0.set_Position(Vec3D(random.get_RN(posMin_.X,posMax_.X),random.get_RN(posMin_.Y,posMax_.Y),random.get_RN(posMin_.Z,posMax_.Z)));
            // p0.set_Velocity(Vec3D(random.get_RN(velMin_.X,velMax_.X),random.get_RN(velMin_.Y,velMax_.Y),random.get_RN(velMin_.Z,velMax_.Z)));
            
            isInsertable=true;
            
//            cout<<"Particle to insert "<<p0<<endl;
            //Check if it has no collision with walls
            for(std::vector<BaseWall*>::iterator it=wH.begin();isInsertable&&it!=wH.end();it++)
            {
                (*it)->get_distance_and_normal(p0,distance,normal);
                if(distance<p0.get_Radius())
                {
 //                   cout<<"Collision with wall "<<**it<<endl;
                    isInsertable=false;
                }
                else
                {
//                    cout<<"No collision with wall "<<**it<<endl;
                }
            }
            //Check if it has no collision with other particles
            for(std::vector<BaseParticle*>::iterator it=pH.begin();isInsertable&&it!=pH.end();it++)
            {
                if(GetDistance2(p0.get_Position(),(*it)->get_Position())<sqr(p0.get_Radius()+(*it)->get_Radius()))
                {
 //                   cout<<"Collision with particle "<<**it<<endl;
                    isInsertable=false;
                }
                else
                {
//                    cout<<"No collision with particle "<<**it<<endl;
                }
            }
            
            if (isInsertable)
            {
                //Still needs to set a lot of things like mass and HGRID stuff
 //               cout<<"Inserting particle"<<endl;
                pH.copyAndAddObject(p0);
                failed = 0; 
            }
            else
            {
//                cout<<"Not inserting particle"<<endl;
                failed++;
            }
        }      
    };
    
	///reads wall
	void read(std::istream& is)  { 
        std::string dummy;
		is >> dummy >> posMin_ >> dummy >> posMax_ >> dummy >> velMin_ >> dummy >> velMax_ >> dummy >> radMin_ >> dummy >> radMax_ >> dummy >> maxFailed_;
	}
	
	///outputs wall
	void print(std::ostream& os) const {
		os << "InsertionBoundary posMin " << posMin_
			<< " posMax " << posMax_ 
			<< " velMin " << velMin_
		 	<< " velMax " << velMax_
            << " radMin " << radMin_
            << " radMax " << radMax_
            << " maxFailed " << maxFailed_;
	}
    

    Vec3D posMin_,posMax_;
    Vec3D velMin_,velMax_;
    Mdouble radMin_,radMax_;
    int maxFailed_;
};

#endif
