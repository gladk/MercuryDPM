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

#include "../../src/HGRID_2D.h"
#include <sstream>
#include <iostream>
#include <cstdlib>

class periodic_walls : public HGRID_2D {

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  ///This is were the walls are implemented
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  void compute_external_forces(BaseParticle *CI)
  {
    compute_walls(CI);
  }

	void setup_particles_initial_conditions() 
	{
		set_dissipation(1);
		BaseParticle P0,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P13;
		P0 .set_Position(Vec3D( 0e-3, 2e-3,  0.0));
		P1 .set_Position(Vec3D( 1e-3, 2e-3,  0.0));
		P2 .set_Position(Vec3D(10e-3, 3e-3,  0.0));
		P3 .set_Position(Vec3D( 1e-3, 3e-3,  0.0));
		P4 .set_Position(Vec3D( 0e-3, 4e-3,  0.0));
		P5 .set_Position(Vec3D( 9e-3, 4e-3,  0.0));
		P6 .set_Position(Vec3D(10e-3, 5e-3,  0.0));
		P7 .set_Position(Vec3D( 9e-3, 5e-3,  0.0));
		P8 .set_Position(Vec3D( 1e-3, 6e-3,  0.0));
		P9 .set_Position(Vec3D(10e-3, 6e-3,  0.0));
		P10.set_Position(Vec3D(10e-3,10e-3,  0.0));
		P11.set_Position(Vec3D( 9e-3, 9e-3,  0.0));
		P12.set_Position(Vec3D( 0e-3, 0e-3, 1e-3));
		P13.set_Position(Vec3D( 1e-3, 1e-3, 1e-3));
		
		P1. set_Velocity(Vec3D(-1e-2, 0e-2, 0.0));
		P3. set_Velocity(Vec3D(-1e-2, 0e-2, 0.0));
		P5. set_Velocity(Vec3D( 1e-2, 0e-2, 0.0));
		P7. set_Velocity(Vec3D( 1e-2, 0e-2, 0.0));
		P9. set_Velocity(Vec3D( 1e-2, 0e-2, 0.0));
		P11.set_Velocity(Vec3D( 1e-2, 1e-2, 0.0));
		P13.set_Velocity(Vec3D(-1e-2,-1e-2, 0.0));
		
		P0.set_Velocity(Vec3D(0.0,0.0,0.0));
		P2.set_Velocity(Vec3D(0.0,0.0,0.0));
		P4.set_Velocity(Vec3D(0.0,0.0,0.0));
		P6.set_Velocity(Vec3D(0.0,0.0,0.0));
		P8.set_Velocity(Vec3D(0.0,0.0,0.0));
		P10.set_Velocity(Vec3D(0.0,0.0,0.0));
		P12.set_Velocity(Vec3D(0.0,0.0,0.0));
		
		P0.set_Radius(0.45e-3);
		P1.set_Radius(0.35e-3);
		P2.set_Radius(0.45e-3);
		P3.set_Radius(0.35e-3);
		P4.set_Radius(0.45e-3);
		P5.set_Radius(0.35e-3);
		P6.set_Radius(0.45e-3);
		P7.set_Radius(0.35e-3);
		P8.set_Radius(0.45e-3);
		P9.set_Radius(0.35e-3);
		P10.set_Radius(0.45e-3);
		P11.set_Radius(0.35e-3);
		P12.set_Radius(0.45e-3);
		P13.set_Radius(0.35e-3);
					
		getParticleHandler().copyAndAddObject(P0);
		getParticleHandler().copyAndAddObject(P1);
		getParticleHandler().copyAndAddObject(P2);
		getParticleHandler().copyAndAddObject(P3);
		getParticleHandler().copyAndAddObject(P4);
		getParticleHandler().copyAndAddObject(P5);
		getParticleHandler().copyAndAddObject(P6);
		getParticleHandler().copyAndAddObject(P7);
		getParticleHandler().copyAndAddObject(P8);
		getParticleHandler().copyAndAddObject(P9);
		getParticleHandler().copyAndAddObject(P10);
		getParticleHandler().copyAndAddObject(P11);
		getParticleHandler().copyAndAddObject(P12);
		getParticleHandler().copyAndAddObject(P13);
			
		PeriodicBoundary b0;
		b0.set(Vec3D(1,0,0), get_xmin(), get_xmax());
		getBoundaryHandler().copyAndAddObject(b0);
		b0.set(Vec3D(0,1,0), get_ymin(), get_ymax());
		getBoundaryHandler().copyAndAddObject(b0);
	}

};


int main(int argc UNUSED, char *argv[] UNUSED)
{
    ///Start off my solving the default problem
    periodic_walls problem;
	problem.set_tmax(0.11);
	problem.set_dt(1.1e-05);
	problem.set_number_of_saves(500);
	problem.set_name("periodic_walls");
	problem.solve();
}
