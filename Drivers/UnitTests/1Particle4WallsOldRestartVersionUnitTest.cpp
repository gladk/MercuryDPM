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
#include <Math/Helpers.h>
#include "DPMBase.h"

/// This code tests if the old restart format can still be read.
/// The resulting simulation should be equivalent to 1Particle4WallsRestartUnitTest.
int main(int argc UNUSED, char *argv[] UNUSED)
{

	helpers::writeToFile("1Particle4WallsOldRestartVersionUnitTest.restart",
		"restart_version 3\n"
		"name 1Particle4WallsOldRestartVersionUnitTest\n"
		"xmin 0 xmax 1 ymin 0 ymax 1.1 zmin 0 zmax 1\n"
		"dt 0.0001 t 0 tmax 0.01 save_count_data 5 save_count_ene 5 save_count_stat 5 save_count_fstat 5\n"
		"dim 3 gravity 0 0 0\n"
		"options_fstat 1 options_data 1 options_ene 1 options_restart 1\n"
		"Species 2\n"
        "k 200000 disp 50 kt 57142.857 dispt 0 mu 0.5 rho 1.9098593 dim_particle 3\n"
        "k 200000 disp 50 kt 57142.857 dispt 0 mu 0.5 rho 1.9098593 dim_particle 3\n"
        "k 200000 disp 50 kt 57142.857 dispt 0 mu 0.5 (mixed)\n"
		"Walls 4\n"
		"numFiniteWalls 0 normal -1 0 0 position 0\n"
		"numFiniteWalls 0 normal  1 0 0 position .96\n"
		"numFiniteWalls 0 normal 0 0 -1 position 0\n"
		"numFiniteWalls 0 normal 0 0  1 position .96\n"
		"WallsPeriodic 1\n"
		"normal 0 1 0 pos_left 0 pos_right 1.1 shift 0 1.1 0\n"
		"Particles 1\n"
		"0.48 0 0.48  0 0 0  0.5  0 0 0  0 60 0  1 10 0\n"
		"NUM_BUCKETS 16 HGRID_MAX_LEVELS 1 MIN_CELL_SIZE 1 SPHERE_TO_CELL_RATIO 1 CELL_TO_CELL_RATIO 2\n"
		"FixedParticleRadius 0.5 MinInflowParticleRadius 0.5 MaxInflowParticleRadius 0.5\n"
		"RandomizedBottom 2 ChuteAngle 20 max_failed 1 num_created 2000\n"
		"InflowVelocity 0.1 InflowVelocityVariance 0 InflowHeight 10\n"
		);
    DPMBase problem;
    problem.setName("1Particle4WallsOldRestartVersionUnitTest");
    problem.readRestartFile();
	problem.setAppend(false);
    problem.solve();
	problem.writeRestartFile();
	
	//Now check the rotational energy in the system; it should loose some rotational energy in the first loop due to
	//sliding, then conserve energy as no tangential dissipation is used.
	//gnuplot:
	// p '1Particle4WallsRestartUnitTest_restart.ene' u 1:4 w l t 'rotational energy', '1Particle4WallsRestartUnitTest_restart.ene' u 1:($3+$4+$5-160) w l t 'total energy - normal elastic energy'
}
