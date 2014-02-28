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
