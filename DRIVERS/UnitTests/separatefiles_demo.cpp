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
		BaseParticle p0;
		
		//This code is solved three times, so you would get three copies of the particles without this check
		if (getParticleHandler().getNumberOfObjects() < 1)
			{
		
			p0.set_Position(Vec3D(get_xmax()/2,get_ymax()/2,0.0));
			p0.set_Velocity(Vec3D(0.0,0.0,0.0));
			p0.set_Radius(0.0005);
			getParticleHandler().copyAndAddObject(p0);
			
			}
			
		else
			{
				getParticleHandler().getObject(0)->set_Position(Vec3D(get_xmax()/2,get_ymax()/2,0.0));
				getParticleHandler().getObject(0)->set_Velocity(Vec3D(0.0,0.0,0.0));
				
			}
			
		//endif
		
		
		set_dt(1e-6);
		set_tmax(1e-3);
		set_number_of_saves(2);
	}

};

int main(int argc UNUSED, char *argv[] UNUSED)
{
	free_fall problem;
	problem.set_name("nofiles");
	problem.set_options_fstat(0);
	problem.set_options_stat(0);
	problem.set_options_ene(0);
	problem.set_options_data(0);
	problem.set_options_restart(0);
	problem.solve();
	
	problem.set_name("combinedfiles");
	problem.set_options_fstat(1);
	problem.set_options_stat(1);
	problem.set_options_ene(1);
	problem.set_options_data(1);
	problem.set_options_restart(1);
	problem.solve();
	
	problem.set_name("separatefiles");
	problem.set_options_fstat(2);
	problem.set_options_stat(2);
	problem.set_options_ene(2);
	problem.set_options_data(2);
	problem.set_options_restart(2);
	problem.solve();
	
}
