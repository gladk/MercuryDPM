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

#include "MD.h"
#include <iostream>

class incl_plane : public MD {
public:
	
	void setup_particles_initial_conditions()
	{
		InfiniteWall w0;
		w0.set(Vec3D(0,-1,0),-get_ymin());
		getWallHandler().copyAndAddWall(w0);
		
		TangentialSpringParticle p0; //important
		p0.set_Radius(0.005);
		p0.set_Position(Vec3D((get_xmin()+get_xmax())/2.0,get_ymin()+p0.get_Radius(),0));
		p0.set_Velocity(Vec3D(0.0,0.0,0.0));
		getParticleHandler().copyAndAddObject(p0);
	}

};

int main(int argc UNUSED, char *argv[] UNUSED)
{
	std::cout<<"Single particle rolling on a bottom plate"<<std::endl;
	///Start off my solving the default problem
	incl_plane problem;
	problem.set_name("inclined_plane");
	problem.set_savecount(5000);
	problem.set_dim(2);
	problem.set_dim_particle(3);
	Mdouble theta = 15.0*constants::pi/180.0;
	problem.set_gravity(9.8*Vec3D(std::sin(theta),-std::cos(theta),0.0));
	problem.set_rho(2000);
	problem.set_k(10000);
	problem.set_disp(0);
	problem.set_kt(10000);
	problem.set_dispt(0);
	problem.set_mu(0.5);
	problem.set_krolling(10000);
	problem.set_disprolling(0);
	problem.set_murolling(0.5);
	//tc=7e-5
	problem.set_dt(2e-6);
	problem.set_tmax(1.0);
	problem.set_xmax(0.1);
	problem.set_ymax(0.1);
	problem.set_zmax(0.1);
	problem.readArguments(argc, argv);
	problem.solve();
	problem.print(std::cout, true);
	std::cout << problem.getParticleHandler().getObject(0)->get_Velocity() << std::endl;

}
