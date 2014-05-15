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
#include <cmath>
#include <iostream>
#include <iomanip>


///this tests if moving the wall works with CWall::move(Vec3D velocity,Vec3D dt). 
///The wall is moved in normal and tangential direction and the results 
///are compared with a system where the particles are moved instead of 
///the walls, with the same relative velocities
class MovingWalls : public MD {
public:

	void setup_particles_initial_conditions()
	{
		set_dim_particle(3);
		set_xmax(2);
		set_ymax(2);
		set_zmax(2);
		set_dim(3);
		set_rho(6/constants::pi);
		set_gravity(Vec3D(0,0,0));
		set_collision_time_and_restitution_coefficient(0.01,1,1);
		set_dt(.0002);
		set_tmax(1);
		set_number_of_saves(20);
		
		//set particles
		TangentialSpringParticle P0;
		P0.set_Radius(0.5);
		P0.set_Position(Vec3D(1,1,1));
		P0.set_Velocity(Vec3D(0,0,-1));
		getParticleHandler().copyAndAddObject(P0);
		
		//set walls
		InfiniteWall w0;
		w0.set(Vec3D(0, 0, -1), get_zmin());
		getWallHandler().copyAndAddWall(w0);
	}
	
	void actions_before_time_step() {
		getWallHandler().getWall(0)->move(Vec3D(0,0,1),get_dt());
	}

};

class OldMovingWalls : public MovingWalls {
	void actions_before_time_step() {
		getWallHandler().getWall(0)->move(-get_t());
	}
};

class NoMovingWalls : public MovingWalls {
	void setup_particles_initial_conditions() {
		MovingWalls::setup_particles_initial_conditions();
		getParticleHandler().getObject(0)->set_Velocity(Vec3D(0,0,-2));
	}

	void actions_before_time_step() {
	}
};

class MovingWallsTangential : public MovingWalls {
public:
	void setup_particles_initial_conditions() {
		MovingWalls::setup_particles_initial_conditions();
		set_gravity(Vec3D(0,0,-1));
		set_collision_time_and_restitution_coefficient(0.01,.5,1);
		set_mu(.5);
		set_kt(get_k()*2./7.);
		set_dispt(get_disp());
		getParticleHandler().getObject(0)->set_Position(Vec3D(1,1,getParticleHandler().getObject(0)->get_Radius()));
		getParticleHandler().getObject(0)->set_Velocity(Vec3D(0,0,0));
	}

	void actions_before_time_step() {
		getWallHandler().getWall(0)->move(Vec3D(1,0,0),get_dt());
	}
};

class OldMovingWallsTangential : public MovingWallsTangential {
	void actions_before_time_step() {
	}
};

class NoMovingWallsTangential : public MovingWallsTangential {
	//Compared to MovingWall case the reference is changed i.e. the particle now moves and the wall fixed; show give same answer case
	void setup_particles_initial_conditions() {
		MovingWallsTangential::setup_particles_initial_conditions();
		getParticleHandler().getObject(0)->set_Velocity(Vec3D(-1,0,0));
	}

	void actions_before_time_step() {
	}
};


int main(int argc UNUSED, char *argv[] UNUSED)
{
	std::cout << std::endl << "Simulation of one particle, d=1, m=1 interacting with a plane;\n testing the effect of normal plane velocity; normal elastic forces only" << std::endl;
	std::cout << std::endl << "at t=0: particle: r(1,1,1) v(0,0,-1), wall: v(0,0,1), gravity(0,0,0)" << std::endl;
	MovingWalls problem;
	problem.set_name("movingWall");
	problem.solve();
	std::cout << "v_z(tmax) = " << std::setprecision(14) << problem.getParticleHandler().getObject(0)->get_Velocity().Z << " (should be 3)" << std::endl;

	std::cout << std::endl << "at t=0: particle: r(1,1,1) v(0,0,-1), wall: v(0,0,1), gravity(0,0,0),\n wall has no velocity, only position, prescribed" << std::endl;
	OldMovingWalls problem2;
	problem2.set_name("oldMovingWall");
	problem2.solve();
	std::cout << "v_z(tmax) = " << std::setprecision(14) << problem2.getParticleHandler().getObject(0)->get_Velocity().Z << std::endl;

	std::cout << std::endl << "at t=0: particle: r(1,1,1) v(0,0,-2), wall: v(0,0,0), gravity(0,0,0)" << std::endl;
	NoMovingWalls problem0;
	problem0.set_name("noMovingWall");
	problem0.solve();
	std::cout << "v_z(tmax)+1 = " << std::setprecision(14) << problem0.getParticleHandler().getObject(0)->get_Velocity().Z+1 << std::endl;

	std::cout << std::endl << "Simulation of one particle, d=1, m=1 interacting with a plane;\n testing the effect of tangential plane velocity; viscoelastic frictional forces" << std::endl;
	
	std::cout << std::endl << "at t=0: particle: r(1,1,1/2) v(0,0,0), wall: v(1,0,0), gravity(0,0,-1)" << std::endl;
	//note: particle starts rolling, does not reach full velocity of the wall
	MovingWallsTangential problem3;
	problem3.set_name("movingWallTangential");
	problem3.solve();
	std::cout << "v_x(tmax) = " << std::setprecision(14) << problem3.getParticleHandler().getObject(0)->get_Velocity().X  << " (should be 2/7)" << std::endl;
	
	std::cout << std::endl << "at t=0: particle: r(1,1,1/2) v(0,0,0), wall: v(1,0,0), gravity(0,0,-1),\n wall has no velocity, only position, prescribed" << std::endl;
	OldMovingWallsTangential problem4;
	problem4.set_name("oldMovingWallTangential");
	problem4.solve();
	std::cout << "v_x(tmax) = " << std::setprecision(14) << problem4.getParticleHandler().getObject(0)->get_Velocity().X << std::endl;

	std::cout << std::endl << "at t=0: particle: r(1,1,1/2) v(-1,0,0), wall: v(0,0,0), gravity(0,0,-1)" << std::endl;
	NoMovingWallsTangential problem5;
	problem5.set_name("noMovingWallTangential");
	problem5.solve();
	std::cout << "v_x(tmax)+1 = " << std::setprecision(14) << problem5.getParticleHandler().getObject(0)->get_Velocity().X+1 << std::endl;

}
