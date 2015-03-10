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

#include <iostream>
#include "Species/LinearViscoelasticSlidingFrictionSpecies.h"
#include "DPMBase.h"
#include "Particles/BaseParticle.h"
#include "Logger.h"

extern Logger<LOG_MAIN_LEVEL> logger;

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
	tangentialSpringUnitTestProblem.setFileType(FileType::NO_FILE);
 	tangentialSpringUnitTestProblem.setGravity(Vec3D(0,0,-1));
 	tangentialSpringUnitTestProblem.setTimeMax(2.5);
	tangentialSpringUnitTestProblem.solve(argc,argv);
	
	std::vector<BaseParticle*>::iterator pIt = tangentialSpringUnitTestProblem.particleHandler.begin();
    
    if (!(*pIt)->getPosition().compareTo(Vec3D(0.5,0.5,0.25), 1e-7))  logger.log(Log::FATAL, "First particles is in the wrong position. It is %",(*pIt)->getPosition());
    if (!(*pIt)->getVelocity().compareTo(Vec3D(0.0,0.0,0.0)  , 1e-7))  logger.log(Log::FATAL, "First particle has the wrong velocity");
    ++pIt;

    if (!((*pIt)->getPosition().compareTo(Vec3D(0.961524052956078, 0.5, 0.450153103106219), 1e-7)))      logger.log(Log::FATAL, "Second particle has the wrong position. It is % at time %",(*pIt)->getPosition(),tangentialSpringUnitTestProblem.getTime());
    if (!(*pIt)->getVelocity().compareTo(Vec3D(0.325869890236916, 0, -0.623251003973752)    , 1e-7))  logger.log(Log::FATAL, "Second particle has the wrong velocity. It is %",(*pIt)->getVelocity());
    if (!(*pIt)->getAngularVelocity().compareTo(Vec3D(0.0,2.11896618998424,0.0)    , 1e-7))  logger.log(Log::FATAL, "Second particles has the wrong angular velocity. It is %", (*pIt)->getAngularVelocity());

}
