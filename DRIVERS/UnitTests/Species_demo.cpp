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
