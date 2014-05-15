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
