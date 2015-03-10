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
#include "Math/Helpers.h"

/// In this file, 1 Particle and 4 Walls are loaded from files 
/// "1Particle4Walls.ini" and "1Particle4Walls.restart". The particles 
/// are aligned such that the single nonfixed particle rotates 
/// sinusoidally without moving. This is to test the behaviour of the 
/// tangential spring and the file loading routines.
int main(int argc UNUSED, char *argv[] UNUSED)
{
	helpers::writeToFile("1Particle4WallsRestartUnitTest.ini",
		"1 0 0 0 0 .96 1 .96\n"
		"0.48 0 0.48  0 0 0  0.5  0 0 0  0 60 0  0\n"
		);

	helpers::writeToFile("1Particle4WallsRestartUnitTest.restart",
        "restart_version 1.0 name 1Particle4WallsRestartUnitTest_restart\n"
        "dataFile name 1Particle4WallsRestartUnitTest_restart.data fileType ONE_FILE saveCount 5 counter 0 nextSavedTimeStep 0\n"
        "fStatFile name 1Particle4WallsRestartUnitTest_restart.fstat fileType ONE_FILE saveCount 5 counter 0 nextSavedTimeStep 0\n"
        "eneFile name 1Particle4WallsRestartUnitTest_restart.ene fileType ONE_FILE saveCount 5 counter 0 nextSavedTimeStep 0\n"
        "restartFile name 1Particle4WallsRestartUnitTest_restart.restart fileType ONE_FILE saveCount 5 counter 0 nextSavedTimeStep 0\n"
        "statFile name 1Particle4WallsRestartUnitTest_restart.stat fileType ONE_FILE saveCount 5 counter 0 nextSavedTimeStep 0\n"
        "xMin 0 xMax 1 yMin 0 yMax 1 zMin 0 zMax 1\n"
        "timeStep 1e-04 time 0 ntimeSteps 0 timeMax 0.01\n"
        "systemDimensions 3 particleDimensions 3 gravity 0 0 0\n"
		"Species 1\n"
		"LinearViscoelasticSlidingFrictionSpecies id 0 density 1.9098593 stiffness 200000 dissipation 0 slidingStiffness 57142.857 slidingDissipation 0 frictionCoefficient 0.5 frictionCoefficientStatic 0.5\n"
        "Walls 4\n"
		"InfiniteWall id 0 indSpecies 0 position 0 0 0 orientation 0 0 0 1 velocity 0 0 0 angularVelocity 0 0 0 0 force 0 0 0 torque 0 0 0 normal -1 0 0 position 0 factor 1\n"
		"InfiniteWall id 1 indSpecies 0 position 0 0 0 orientation 0 0 0 1 velocity 0 0 0 angularVelocity 0 0 0 0 force 0 0 0 torque 0 0 0 normal  1 0 0 position .96 factor 1\n"
		"InfiniteWall id 2 indSpecies 0 position 0 0 0 orientation 0 0 0 1 velocity 0 0 0 angularVelocity 0 0 0 0 force 0 0 0 torque 0 0 0 normal 0 0 -1 position 0 factor 1\n"
		"InfiniteWall id 3 indSpecies 0 position 0 0 0 orientation 0 0 0 1 velocity 0 0 0 angularVelocity 0 0 0 0 force 0 0 0 torque 0 0 0 normal 0 0  1 position .96 factor 1\n"
		"WallsPeriodic 0\n"
		"Particles 0 \n"
        "NUM_BUCKETS 16 HGRID_MAX_LEVELS 1 MIN_CELL_SIZE 1 SPHERE_TO_CELL_RATIO 1 CELL_TO_CELL_RATIO 2\n"
		);
		
 	DPMBase problem;
    problem.setName("1Particle4WallsRestartUnitTest");
    problem.readRestartFile();
	problem.readDataFile("1Particle4WallsRestartUnitTest.ini");
	problem.setAppend(false);
	problem.solve();
	problem.writeRestartFile();
	
	//Now check the rotational energy in the system; it should loose some rotational energy in the first loop due to
	//sliding, then conserve energy as no tangential dissipation is used.
	//gnuplot:
	// p '1Particle4WallsRestartUnitTest_restart.ene' u 1:4 w l t 'rotational energy', '1Particle4WallsRestartUnitTest_restart.ene' u 1:($3+$4+$5-160) w l t 'total energy - normal elastic energy'
}
