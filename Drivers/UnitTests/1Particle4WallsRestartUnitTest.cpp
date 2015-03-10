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

#include <Logger.h>
#include "DPMBase.h"
#include "Math/Helpers.h"
extern Logger<LOG_MAIN_LEVEL> logger;

/// In this file, 1 Particle and 4 Walls are loaded from files 
/// "1Particle4Walls.ini" and "1Particle4Walls.restart". The particles 
/// are aligned such that the single nonfixed particle rotates 
/// sinusoidally without moving. This is to test the behaviour of the 
/// tangential spring and the file loading routines.
int main(int argc UNUSED, char *argv[] UNUSED)
{
	helpers::writeToFile("1Particle4WallsRestartUnitTest.ini",
		"1 0 0 0 0 .96 1 .96\n"
		"0.48 0 0.48  0 0 0  0.5  0 0 0  0 0 0  0\n"
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
		"InfiniteWall id 0 indSpecies 0 position 0    0 0    orientation 0 0 0 1 velocity 0 0 0 angularVelocity 0 0 0 0 force 0 0 0 torque 0 0 0 normal -1 0 0 factor 1\n"
		"InfiniteWall id 1 indSpecies 0 position 0.96 0 0    orientation 0 0 0 1 velocity 0 0 0 angularVelocity 0 0 0 0 force 0 0 0 torque 0 0 0 normal  1 0 0 factor 1\n"
		"InfiniteWall id 2 indSpecies 0 position 0    0 0    orientation 0 0 0 1 velocity 0 0 0 angularVelocity 0 0 0 0 force 0 0 0 torque 0 0 0 normal 0 0 -1 factor 1\n"
		"InfiniteWall id 3 indSpecies 0 position 0    0 0.96 orientation 0 0 0 1 velocity 0 0 0 angularVelocity 0 0 0 0 force 0 0 0 torque 0 0 0 normal 0 0  1 factor 1\n"
		"Boundaries 0\n"
		"Particles 1\n"
        "BaseParticle id 0 indSpecies 0 position 0.48 0 0.48 orientation 0 0 0 1 velocity 0 0 0 angularVelocity 0 60 0 0 force 0 0 0 torque 0 0 0 radius 0.5 invMass 1 invInertia 10\n"
        "Interactions 4\n"
        "LinearViscoelasticSlidingFrictionInteraction particleWallIds 0 0 timeStamp 0 force 0 0 0 torque 0 0 0 slidingSpring 0 0 0\n"
        "LinearViscoelasticSlidingFrictionInteraction particleWallIds 0 1 timeStamp 0 force 0 0 0 torque 0 0 0 slidingSpring 0 0 0\n"
        "LinearViscoelasticSlidingFrictionInteraction particleWallIds 0 2 timeStamp 0 force 0 0 0 torque 0 0 0 slidingSpring 0 0 0\n"
        "LinearViscoelasticSlidingFrictionInteraction particleWallIds 0 3 timeStamp 0 force 0 0 0 torque 0 0 0 slidingSpring 0 0 0\n"
		);
		
 	DPMBase problem;
    problem.setName("1Particle4WallsRestartUnitTest");
    problem.readRestartFile();
	//problem.readDataFile("1Particle4WallsRestartUnitTest.ini");
	problem.setAppend(false);
    //problem.write(std::cout,true);
    problem.solve();
    //problem.write(std::cout,true);
	problem.writeRestartFile();
	
    BaseParticle* p = problem.particleHandler.getObject(0);
    if (!p->getAngularVelocity().compareTo(Vec3D(0.0,17.7251802668803,0.0), 1e-7))  logger.log(Log::FATAL, "First particles has the wrong angular velocity it is %, while is should be %.",p->getAngularVelocity(),Vec3D(0.0,17.7251802668803,0.0));
    if (!p->getVelocity().compareTo(Vec3D(0.0,0.0,0.0)  , 1e-7))  logger.log(Log::FATAL, "First particle has the wrong velocity, it is %, while it should be %",p->getVelocity(),0.0);

    //Now check the rotational energy in the system; it should loose some rotational energy in the first loop due to
	//sliding, then conserve energy as no tangential dissipation is used.
	//gnuplot:
	// p '1Particle4WallsRestartUnitTest_restart.ene' u 1:4 w l t 'rotational energy', '1Particle4WallsRestartUnitTest_restart.ene' u 1:($3+$4+$5-160) w l t 'total energy - normal elastic energy'

}
