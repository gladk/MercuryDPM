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
#include <iostream>

class SpeciesTest : public MD{
public:
	virtual void cout_time() {}
	
	void setup_particles_initial_conditions()
	{
		
		double tc=1e-2, r=0.4, m = 0.5;
		//sets k and disp based on the collision time and restitution coefficient for a collision with effective mass m
		get_Species(0)->set_k_and_disp(helperFunc::computeKAndDispFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(tc, r, m));
		//set_collision_time_and_restitution_coefficient(tc,r, 1.0,1.0);

		set_dim(3);
		set_dim_particle(3);
		set_rho(6./constants::pi);
		set_savecount(50);
		set_dt(tc/50.); 
		set_tmax(5.0);
		set_xmax(10);
		set_ymax(10);
		set_zmax(1);
		set_zmin(-1);
		set_gravity(Vec3D(0,0,0));
		
		add_Species();
		
		BaseParticle P0,P1,P2,P3,P4,P5;
		//Small particles are species 0 and large particles are species two
		//Collison small - small
		P0.set_Radius(.5);
		P0.set_species(0);
		P0.set_Position(Vec3D(0.0,0.0,0.0));
		P0.set_Velocity(Vec3D(1.0,0.0,0.0));
		
		P1.set_Radius(.5);
		P1.set_species(0);
		P1.set_Position(Vec3D(3.0,0.0,0.0));
		P1.set_Velocity(Vec3D(0.0,0.0,0.0));
		
	
		//Collision small - large
		P2.set_Radius(.5);
		P2.set_species(0);
		P2.set_Position(Vec3D(0.0,0.0,2.0));
		P2.set_Velocity(Vec3D(1.0,0.0,0.0));
		
		P3.set_Radius(1.0);
		P3.set_species(1);
		P3.set_Position(Vec3D(3.0,0.0,2.0));
		P3.set_Velocity(Vec3D(0.0,0.0,0.0));
		
		//Collision large - large
		P4.set_Radius(1.0);
		P4.set_species(1);
		P4.set_Position(Vec3D(0.0,0.0,4.0));
		P4.set_Velocity(Vec3D(1.0,0.0,0.0));
		
		P5.set_Radius(1.0);
		P5.set_species(1);
		P5.set_Position(Vec3D(3.0,0.0,4.0));
		P5.set_Velocity(Vec3D(0.0,0.0,0.0));
		
		getParticleHandler().copyAndAddObject(P0);
		getParticleHandler().copyAndAddObject(P1);
		getParticleHandler().copyAndAddObject(P2);
		getParticleHandler().copyAndAddObject(P3);
		getParticleHandler().copyAndAddObject(P4);
		getParticleHandler().copyAndAddObject(P5);		
		
		get_Species(1)->set_collision_time_and_restitution_coefficient(tc,r, 8.0,8.0);
		get_MixedSpecies(1, 0)->set_collision_time_and_restitution_coefficient(tc,r, 1.0,8.0);
		
	}

	void actions_after_solve() 
		{

	
			for (int i=0;i<5;i+=2)
				{
				std::cout << "Cofficient of resutions for collision " <<round(i/2.0)+1<<" is : "<< getParticleHandler().getObject(i+1)->get_Velocity().X-getParticleHandler().getObject(i)->get_Velocity().X <<std::endl;
				}
			
		
			
		}
		
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
	std::cout << "Species test problem " <<std::endl;
	std::cout << "---------------------" <<std::endl;
	std::cout << std::endl;
	std::cout << "This problem set k and gamma for different for different size particles such that the coefficent of restitution is the same" << std::endl;
	std::cout << "Note for each collision this value should be 0.4" << std::endl;
	std::cout << "Also this code demostrates how to use different species in the code" << std::endl;
	SpeciesTest problem;
	problem.set_name("SpeciesTest");

	problem.solve();
	//problem.print(std::cout,true);
}
