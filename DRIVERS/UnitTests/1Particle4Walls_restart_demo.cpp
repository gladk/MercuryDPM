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

#include "MD.h"
#include "writeToFile.h"

/// In this file, 1 Particle and 4 Walls are loaded from files 
/// "1Particle4Walls.ini" and "1Particle4Walls.restart". The particles 
/// are aligned such that the single nonfixed particle rotates 
/// sinusoidally without moving. This is to test the behaviour of the 
/// tangential spring and the file loading routines.
int main(int argc UNUSED, char *argv[] UNUSED)
{
	writeToFile("1Particle4Walls.ini",
		"1 0 0 0 0 .96 1 .96\n"
		"0.48 0 0.48  0 0 0  0.5  0 0 0  0 60 0  0\n"
		);

	writeToFile("1Particle4Walls.restart",
		"restart_version 2\n"
		"name 1Particle4Walls_restart\n"
		"xmin 0 xmax 1 ymin 0 ymax 1 zmin 0 zmax 1\n"
		"dt 1e-4 t 0 tmax .01 save_count 5\n"
		"dim 3 gravity 0 0 0\n"
		"options_fstat 1 options_data 1 options_stat 1\n"
		"Species 1\n"
		"k 200000 disp 0 kt 57142.857 dispt 0 mu 0.5 rho 1.9098593 dim_particle 3\n"
		"Walls 4\n"
		"InfiniteWall normal -1 0 0 position 0\n"
		"InfiniteWall normal  1 0 0 position .96\n"
		"InfiniteWall normal 0 0 -1 position 0\n"
		"InfiniteWall normal 0 0  1 position .96\n"
		"WallsPeriodic 0\n"
		"Particles 0 \n"
		"FixedParticles 0\n"
		"NUM_BUCKETS 16 HGRID_MAX_LEVELS 1 MIN_CELL_SIZE 1 SPHERE_TO_CELL_RATIO 1 CELL_TO_CELL_RATIO 2"
		);
		
 	MD problem;
 	problem.set_name("1Particle4Walls");
	problem.load_restart_data();
	problem.load_from_data_file("1Particle4Walls.ini");
	problem.set_append(false);
	problem.solve();
	problem.save_restart_data();
	
	//Now check the rotational energy in the system
	//gnuplot: p '1Particle4Walls_restart.ene' u 1:4
}
