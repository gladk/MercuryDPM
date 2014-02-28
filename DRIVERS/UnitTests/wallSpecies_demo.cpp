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

#include "../../src/MD.h"
#include <cmath>
#include <iostream>
#include <iomanip>


///this tests if moving the wall works with CWall::move(Vec3D velocity,Vec3D dt). 
///The wall is moved in normal and tangential direction and the results 
///are compared with a system where the particles are moved instead of 
///the walls, with the same relative velocities
class WallSpecies : public MD {
public:

	void setup_particles_initial_conditions()
	{
		set_dim_particle(3);
		set_xmax(20);
		set_ymax(20);
		set_zmax(20);
		set_dim(3);
		set_rho(6/constants::pi);
		set_gravity(Vec3D(0,0,0));
		set_collision_time_and_restitution_coefficient(0.01,0.25,2);
		set_dt(.0002);
		set_tmax(15);
		set_number_of_saves(30);
		
        add_Species();
        
        get_MixedSpecies(0,1)->set_collision_time_and_restitution_coefficient(0.01,0.5,2);
        
        
		//set particles
		BaseParticle P0;
		P0.set_Radius(0.5);
		P0.set_Position(Vec3D(5,10,10));
		P0.set_Velocity(Vec3D(-1,0,-1));
		getParticleHandler().copyAndAddObject(P0);
        
		//set walls
		InfiniteWall w0;
		w0.set(Vec3D(0, 0, -1), get_zmin());
		getWallHandler().copyAndAddWall(w0);
        
        w0.set(Vec3D(-1,0,0),get_xmin());
        w0.setIndSpecies(1);
        getWallHandler().copyAndAddWall(w0);
        
        
	}

    //Write the speed of the particles as the info line.
    double getInfo(BaseParticle& P0)
    {
        return sqrt(sqr(P0.get_Velocity().X)+sqr(P0.get_Velocity().Z));
    }

};



int main(int argc UNUSED, char *argv[] UNUSED)
{
	std::cout << std::endl << "Simulation of one particle, d=0.1, interacting with walls with different coefficeient of resitution" << std::endl;
	std::cout << std::endl << "The purpose of the test is to check walls species information is correctly picked up" << std::endl;
	WallSpecies problem;
	problem.set_name("wallSpecies");
	problem.solve();
	std::cout << "v_z(tmax) = " << std::setprecision(14) << problem.getParticleHandler().getObject(0)->get_Velocity().Z << " (should be 0.25 give or take)" << std::endl;
    std::cout << "v_x(tmax) = " << std::setprecision(14) << problem.getParticleHandler().getObject(0)->get_Velocity().X << " (should be 0.5 give or take)" << std::endl;

    

}
