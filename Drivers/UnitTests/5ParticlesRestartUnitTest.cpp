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

#include "DPMBase.h"

/// In this file, 5 Particles (4 fixed) are loaded from files 
/// "5Particles.ini" and "5Particles.restart". The particles are aligned 
/// such that the single nonfixed particle rotates sinusoidally without 
/// moving. This is to test the behaviour of the tangential spring and 
/// the file loading routines.
int main(int argc UNUSED, char *argv[] UNUSED)
{
	
	helpers::writeToFile("5Particles.ini",
		"5 0 0 0 0 1.6 1 1.6 \n"
		"0   0 0    0 0 0  0.5  0 0 0  0 0 0  0\n"
		"0   0 1.4  0 0 0  0.5  0 0 0  0 0 0  0\n"
		"1.4 0 0    0 0 0  0.5  0 0 0  0 0 0  0\n"
		"1.4 0 1.4  0 0 0  0.5  0 0 0  0 0 0  0\n"
		"0.7 0 0.7  0 0 0  0.5  0 0 0  0 30 0  0\n"
		);

	helpers::writeToFile("5Particles.restart",
        "restart_version 1.0 name 1Particle4WallsRestartUnitTest_restart\n"
        "dataFile name 1Particle4WallsRestartUnitTest_restart.data fileType ONE_FILE saveCount 5\n"
        "fStatFile name 1Particle4WallsRestartUnitTest_restart.fstat fileType ONE_FILE saveCount 5\n"
        "eneFile name 1Particle4WallsRestartUnitTest_restart.ene fileType ONE_FILE saveCount 1\n"
        "restartFile name 1Particle4WallsRestartUnitTest_restart.restart fileType ONE_FILE saveCount 5\n"
        "statFile name 1Particle4WallsRestartUnitTest_restart.stat fileType ONE_FILE saveCount 5\n"
        "xMin 0 xMax 1.6 yMin 0 yMax 1 zMin 0 zMax 1.6\n"
        "timeStep 1e-05 time 0 timeMax 0.01\n"
        "systemDimensions 3 particleDimensions 3 gravity 0 0 0\n"
        "Species 1\n"
        "LinearViscoelasticSlidingFrictionSpecies id 0 rho 1.9098593 k 200000 disp 0 slidingStiffness 57142.857 slidingDissipation 0 frictionCoefficient 0.5 frictionCoefficientStatic 0.5\n"
		"Walls 0\n"
		"WallsPeriodic 0\n"
		"Particles 5\n"
        "BP 0 0 0 0 0 0 0.5 0 0 0 0 0 0 0 0 0\n"
        "BP 0 0 0 0 0 0 0.5 0 0 0 0 0 0 0 0 0\n"
        "BP 0 0 0 0 0 0 0.5 0 0 0 0 0 0 0 0 0\n"
        "BP 0 0 0 0 0 0 0.5 0 0 0 0 0 0 0 0 0\n"
        "BP 0 0 0 0 0 0 0.5 0 0 0 0 0 0 0 0 0\n"
		"NUM_BUCKETS 16 HGRID_MAX_LEVELS 1 MIN_CELL_SIZE 1 SPHERE_TO_CELL_RATIO 1 CELL_TO_CELL_RATIO 2\n"
		);
	
 	DPMBase problem;
 	problem.setName("5Particles");
    problem.readRestartFile();
	problem.readDataFile("5Particles.ini");
	problem.setAppend(false);
	problem.solve();
	
	//Now check the rotational energy in the system
	//gnuplot: p '5Particles_restart.ene' u 1:4
}
