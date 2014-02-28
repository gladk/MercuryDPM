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

/// In this file two particles are symmetrically placed in a bi-axial box are allowed to jump around under gravity. It tests walls gravity and symmetry.

class two_particle_elastic_collision : public MD {

	void setup_particles_initial_conditions()
	{
		BaseParticle P0,P1;
		P0.set_Position(Vec3D(0.006,0.005,0.0));
		P1.set_Position(Vec3D(0.004,0.005,0.0));
	
		P0.set_Velocity(Vec3D(-0.1,0.0,0.0));
		P1.set_Velocity(Vec3D( 0.1,0.0,0.0));
	
		P0.set_Radius(0.0005);
		P1.set_Radius(0.0005);
		getParticleHandler().copyAndAddObject(P0);
		getParticleHandler().copyAndAddObject(P1);
		
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
	two_particle_elastic_collision problem;
	problem.set_name("elastic_collision");
	problem.set_dt(1e-5);
	problem.solve();
}
