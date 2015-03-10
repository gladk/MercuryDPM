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

#include "DPMBase.h"
#include "Math/Helpers.h"

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
        "dataFile name 1Particle4WallsRestartUnitTest_restart.data fileType ONE_FILE saveCount 5 counter 0 nextSavedTimeStep 0\n"
        "fStatFile name 1Particle4WallsRestartUnitTest_restart.fstat fileType ONE_FILE saveCount 5 counter 0 nextSavedTimeStep 0\n"
        "eneFile name 1Particle4WallsRestartUnitTest_restart.ene fileType ONE_FILE saveCount 1 counter 0 nextSavedTimeStep 0\n"
        "restartFile name 1Particle4WallsRestartUnitTest_restart.restart fileType ONE_FILE saveCount 5 counter 0 nextSavedTimeStep 0\n"
        "statFile name 1Particle4WallsRestartUnitTest_restart.stat fileType ONE_FILE saveCount 5 counter 0 nextSavedTimeStep 0\n"
        "xMin 0 xMax 1.6 yMin 0 yMax 1 zMin 0 zMax 1.6\n"
        "timeStep 1e-05 time 0 ntimeSteps 0 timeMax 0.01\n"
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
		"Interactions 0\n"
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
