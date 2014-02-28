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

#include "MD.h"
#include "writeToFile.h"

/// In this file, 5 Particles (4 fixed) are loaded from files 
/// "5Particles.ini" and "5Particles.restart". The particles are aligned 
/// such that the single nonfixed particle rotates sinusoidally without 
/// moving. This is to test the behaviour of the tangential spring and 
/// the file loading routines.
int main(int argc UNUSED, char *argv[] UNUSED)
{
	
	writeToFile("5Particles.ini",
		"5 0 0 0 0 1.6 1 1.6 \n"
		"0   0 0    0 0 0  0.5  0 0 0  0 0 0  0\n"
		"0   0 1.4  0 0 0  0.5  0 0 0  0 0 0  0\n"
		"1.4 0 0    0 0 0  0.5  0 0 0  0 0 0  0\n"
		"1.4 0 1.4  0 0 0  0.5  0 0 0  0 0 0  0\n"
		"0.7 0 0.7  0 0 0  0.5  0 0 0  0 30 0  0\n"
		);

	writeToFile("5Particles.restart",
		"restart_version 2\n"
		"name 5Particles_restart\n"
		"xmin 0 xmax 1.6 ymin 0 ymax 1 zmin 0 zmax 1.6\n"
		"dt 1e-5 t 0 tmax .01 save_count 5\n"
		"dim 3 gravity 0 0 0\n"
		"options_fstat 1 options_data 1 options_stat 1\n"
		"Species 1\n"
		"k 200000 disp 25 kt 57142.857 dispt 0 mu 0.5 rho 1.9098593 dim_particle 3\n"
		"Walls 0\n"
		"WallsPeriodic 0\n"
		"Particles 0\n"
		"FixedParticles 4\n"
		"NUM_BUCKETS 16 HGRID_MAX_LEVELS 1 MIN_CELL_SIZE 1 SPHERE_TO_CELL_RATIO 1 CELL_TO_CELL_RATIO 2\n"
		);
	
 	MD problem;
 	problem.set_name("5Particles");
	problem.load_restart_data();
	problem.load_from_data_file("5Particles.ini");
	problem.set_append(false);
	problem.solve();
	
	//Now check the rotational energy in the system
	//gnuplot: p '5Particles_restart.ene' u 1:4
}
