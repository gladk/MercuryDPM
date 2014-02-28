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

#include<iostream>
#include "../../src/MD.h"
#include "../../src/HGRID_3D.h"


/// In this file, the rolling behaviour of the tangential spring is tested. This is done by placing 
class TangentialSpringTest : public MD {

	void setup_particles_initial_conditions()
	{
		set_xmax(1);
		set_ymax(1);
		set_zmax(1);
		set_dim(3);
		set_dim_particle(3);
		
		TangentialSpringParticle P0,P1;
		
		P0.set_Position(Vec3D(0.5,0.5,0.25));
		P1.set_Position(Vec3D(0.5,0.5,0.75));
	
		P0.fixParticle();
		P1.set_Velocity(Vec3D(0.1,0.0,0.0));
	
		P0.set_Radius(0.25);
		P1.set_Radius(0.25);
		
		getParticleHandler().copyAndAddObject(P0);
		getParticleHandler().copyAndAddObject(P1);		
	}
};


int main(int argc UNUSED, char *argv[] UNUSED)
{

 	TangentialSpringTest problem;
 	problem.set_name("TangentialSpringTest");
 	problem.set_rho(6./constants::pi);
	problem.set_collision_time_and_restitution_coefficient(1e-3,.2,1./8);
	problem.set_kt(problem.get_k()*2/7);
	problem.set_mu(1e20);
 	problem.set_dt(2e-5);
 	problem.set_savecount(43);
 	problem.set_gravity(Vec3D(0,0,-1));
 	problem.set_tmax(2.5);
	problem.solve(argc,argv);
}
