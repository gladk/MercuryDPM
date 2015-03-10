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
