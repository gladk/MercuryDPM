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

#include <iostream>
#include "Species/LinearViscoelasticSlidingFrictionSpecies.h"
#include "DPMBase.h"
#include "Particles/BaseParticle.h"
#include "Logger.h"

/// In this file, the rolling behaviour of the tangential spring is tested. This is done by placing one normal partilce on top of a fixed partilce and letting graviry roll it over the other particle until it losses contact.
class TangentialSpringUnitTest : public DPMBase {
public:
    void setupInitialConditions()
	{
		setXMax(1);
		setYMax(1);
		setZMax(1);
        setSystemDimensions(3);
		setParticleDimensions(3);
		
		BaseParticle P0,P1;
		
		P0.setPosition(Vec3D(0.5,0.5,0.25));
		P1.setPosition(Vec3D(0.5,0.5,0.75));
	
		P0.fixParticle();
		P1.setVelocity(Vec3D(0.1,0.0,0.0));
	
		P0.setRadius(0.25);
		P1.setRadius(0.25);
		
		particleHandler.copyAndAddObject(P0);
		particleHandler.copyAndAddObject(P1);		
	}
};

Logger<Log::ERROR> testLogger("Tangential Spring Unit Test");

int main(int argc, char *argv[])
{

 	TangentialSpringUnitTest tangentialSpringUnitTestProblem;
    auto species = tangentialSpringUnitTestProblem.speciesHandler.copyAndAddObject(LinearViscoelasticSlidingFrictionSpecies());
 	tangentialSpringUnitTestProblem.setName("TangentialSpringUnitTest");
 	species->setDensity(6./constants::pi);
	species->setCollisionTimeAndRestitutionCoefficient(1e-3,.2,1./8);
	species->setSlidingStiffness(species->getStiffness()*2/7);
	species->setSlidingFrictionCoefficient(1e20);
    tangentialSpringUnitTestProblem.setTimeStep(2e-5);
	//tangentialSpringUnitTestProblem.setFileType(FileType::NO_FILE);
 	tangentialSpringUnitTestProblem.setGravity(Vec3D(0,0,-1));
 	tangentialSpringUnitTestProblem.setTimeMax(2.5);
	tangentialSpringUnitTestProblem.solve(argc,argv);
	
	std::vector<BaseParticle*>::iterator pIt = tangentialSpringUnitTestProblem.particleHandler.begin();
    
    if (!(*pIt)->getPosition().isEqualTo(Vec3D(0.5,0.5,0.25), 1e-7))  logger(FATAL, "First particles is in the wrong position. It is %",(*pIt)->getPosition());
    if (!(*pIt)->getVelocity().isEqualTo(Vec3D(0.0,0.0,0.0)  , 1e-7))  logger(FATAL, "First particle has the wrong velocity");
    ++pIt;

    if (!((*pIt)->getPosition().isEqualTo(Vec3D(0.961524052956078, 0.5, 0.450153103106219), 1e-7)))      logger(FATAL, "Second particle has the wrong position. It is % at time %",(*pIt)->getPosition(),tangentialSpringUnitTestProblem.getTime());
    if (!(*pIt)->getVelocity().isEqualTo(Vec3D(0.325869890236916, 0, -0.623251003973752)    , 1e-7))  logger(FATAL, "Second particle has the wrong velocity. It is %",(*pIt)->getVelocity());
    if (!(*pIt)->getAngularVelocity().isEqualTo(Vec3D(0.0,2.11896618998424,0.0)    , 1e-7))  logger(FATAL, "Second particles has the wrong angular velocity. It is %", (*pIt)->getAngularVelocity());

}
