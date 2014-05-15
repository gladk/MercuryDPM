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

class free_fall : public MD{
public:
	
	void setup_particles_initial_conditions()
	{
		InfiniteWall w0;
		w0.set(Vec3D(-1,0,0), -get_xmin());
		getWallHandler().copyAndAddWall(w0);
		w0.set(Vec3D( 1,0,0),  get_xmax());
		getWallHandler().copyAndAddWall(w0);
		w0.set(Vec3D(0,-1,0), -get_ymin());
		getWallHandler().copyAndAddWall(w0);
		w0.set(Vec3D(0, 1,0),  get_ymax());
		getWallHandler().copyAndAddWall(w0);
		
		BaseParticle p0;
		p0.set_Position(Vec3D(get_xmax()/2,get_ymax()/2,0.0));
		p0.set_Velocity(Vec3D(0.0,0.0,0.0));
		p0.set_Radius(0.0005);
		getParticleHandler().copyAndAddObject(p0);

	}

};


int run_freefall(int argc, char *argv[])
{
	
	///Start off my solving the default problem
	free_fall free_fall_problem;
	free_fall_problem.set_name("free_fall_restart");
	free_fall_problem.set_savecount(10000);
	free_fall_problem.set_dt(1e-6);
	free_fall_problem.set_ymax(0.1);
	free_fall_problem.set_xmax(0.01);
	free_fall_problem.readArguments(argc, argv);
	free_fall_problem.solve();
	return true;
}

int main(int argc UNUSED, char *argv[] UNUSED)
{
	if (argc==1) {
		std::cout << std::endl << "Case 1: not restarted" << std::endl;
		if (system("./free_fall_restart_demo -tmax 0.4 -name free_fall_no_restart")) std::cerr<< "code did not run" <<std::endl;

		std::cout << std::endl << "Case 2: restarted at t=0.2" << std::endl;
		//restarted at t=0.2

		if (system("./free_fall_restart_demo -tmax 0.2")) std::cerr<< "code did not run" <<std::endl;
		if (system("./free_fall_restart_demo -r -tmax 0.4")) std::cerr<< "code did not run" <<std::endl;

		std::cout << std::endl << "Case 3: restarted at t=0.2; using separate data files" << std::endl;


		if (system("./free_fall_restart_demo -tmax 0.2 -name free_fall_restart_opt2 -options_data 2")) std::cerr<< "code did not run" <<std::endl;
		if (system("./free_fall_restart_demo -r free_fall_restart_opt2 -tmax 0.4 ")) std::cerr<< "code did not run" <<std::endl;
		//the if is to silence the warning about not using the return value of system
	} else run_freefall(argc, argv);
	//to compare use
	//gnuplot> var=3; plot 'free_fall_restart/free_fall_restart.ene' u 1:var w l, 'free_fall_restart/free_fall_no_restart.ene' u 1:var w l
	//../sc/fpdiff.py ./free_fall_restart.fstat ./free_fall_no_restart.fstat

}
