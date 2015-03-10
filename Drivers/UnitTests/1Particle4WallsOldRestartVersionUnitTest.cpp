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
