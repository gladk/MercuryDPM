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
