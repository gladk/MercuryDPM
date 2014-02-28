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
