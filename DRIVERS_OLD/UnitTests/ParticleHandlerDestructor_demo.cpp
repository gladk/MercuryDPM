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

///todo{This code is not working as is wanted}


#include<iostream>

#include "../../src/MD.h"


class my_problem : public MD{
public:

void setup_particles_initial_conditions()
{
	BaseParticle p0;
	for (unsigned int i=0;i<2;i++)
	{
		Mdouble Y = random.get_RN(0.0,get_ymax());
		Mdouble X = random.get_RN(0.0,get_xmax());
		p0.set_Position(Vec3D(X,Y,0.0));
		p0.set_Velocity(Vec3D(0.0,0.0,0.0));
		p0.set_Radius(0.0005);
		p0.compute_particle_mass(Species);
		getParticleHandler().copyAndAddObject(p0);
	}
	
	getWallHandler().clear();
	InfiniteWall w0;
	w0.set(Vec3D(-1, 0, 0), -get_xmin());
	getWallHandler().copyAndAddWall(w0);
	w0.set(Vec3D( 1, 0, 0),  get_xmax());
	getWallHandler().copyAndAddWall(w0);
	w0.set(Vec3D( 0,-1, 0), -get_ymin());
	getWallHandler().copyAndAddWall(w0);
	w0.set(Vec3D( 0, 1, 0),  get_ymax());
	getWallHandler().copyAndAddWall(w0);
}	
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
 	{
 		my_problem problem;
 		problem.set_name("ParticleHandlerDestructorTest");
 		problem.solve();
 		problem.info();
 	}
 	std::cout<<"Ready"<<std::endl;

}
