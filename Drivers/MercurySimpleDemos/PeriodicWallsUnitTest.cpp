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
#include <sstream>
#include <iostream>
#include <cstdlib>

#include "Mercury2D.h"
#include "Boundaries/PeriodicBoundary.h"
#include "Particles/BaseParticle.h"
#include "Species/LinearViscoelasticSpecies.h"
#include "Logger.h"
extern Logger<LOG_MAIN_LEVEL> logger;

class periodic_walls : public Mercury2D {

  /////////////////////////////////////////////////////////////////////////////////////////////////////
  ///This is were the walls are implemented
  /////////////////////////////////////////////////////////////////////////////////////////////////////
  void computeExternalForces(BaseParticle *CI)
  {
    computeWalls(CI);
  }

	void setupInitialConditions() 
	{
		BaseParticle P0,P1,P2,P3,P4,P5,P6,P7,P8,P9,P10,P11,P12,P13;
		P0 .setPosition(Vec3D( 0e-3, 2e-3,  0.0));
		P1 .setPosition(Vec3D( 1e-3, 2e-3,  0.0));
		P2 .setPosition(Vec3D(10e-3, 3e-3,  0.0));
		P3 .setPosition(Vec3D( 1e-3, 3e-3,  0.0));
		P4 .setPosition(Vec3D( 0e-3, 4e-3,  0.0));
		P5 .setPosition(Vec3D( 9e-3, 4e-3,  0.0));
		P6 .setPosition(Vec3D(10e-3, 5e-3,  0.0));
		P7 .setPosition(Vec3D( 9e-3, 5e-3,  0.0));
		P8 .setPosition(Vec3D( 1e-3, 6e-3,  0.0));
		P9 .setPosition(Vec3D(10e-3, 6e-3,  0.0));
		P10.setPosition(Vec3D(10e-3,10e-3,  0.0));
		P11.setPosition(Vec3D( 9e-3, 9e-3,  0.0));
		P12.setPosition(Vec3D( 0e-3, 0e-3, 1e-3));
		P13.setPosition(Vec3D( 1e-3, 1e-3, 1e-3));
		
		P1. setVelocity(Vec3D(-1e-2, 0e-2, 0.0));
		P3. setVelocity(Vec3D(-1e-2, 0e-2, 0.0));
		P5. setVelocity(Vec3D( 1e-2, 0e-2, 0.0));
		P7. setVelocity(Vec3D( 1e-2, 0e-2, 0.0));
		P9. setVelocity(Vec3D( 1e-2, 0e-2, 0.0));
		P11.setVelocity(Vec3D( 1e-2, 1e-2, 0.0));
		P13.setVelocity(Vec3D(-1e-2,-1e-2, 0.0));
		
		P0.setVelocity(Vec3D(0.0,0.0,0.0));
		P2.setVelocity(Vec3D(0.0,0.0,0.0));
		P4.setVelocity(Vec3D(0.0,0.0,0.0));
		P6.setVelocity(Vec3D(0.0,0.0,0.0));
		P8.setVelocity(Vec3D(0.0,0.0,0.0));
		P10.setVelocity(Vec3D(0.0,0.0,0.0));
		P12.setVelocity(Vec3D(0.0,0.0,0.0));
		
		P0.setRadius(0.45e-3);
		P1.setRadius(0.35e-3);
		P2.setRadius(0.45e-3);
		P3.setRadius(0.35e-3);
		P4.setRadius(0.45e-3);
		P5.setRadius(0.35e-3);
		P6.setRadius(0.45e-3);
		P7.setRadius(0.35e-3);
		P8.setRadius(0.45e-3);
		P9.setRadius(0.35e-3);
		P10.setRadius(0.45e-3);
		P11.setRadius(0.35e-3);
		P12.setRadius(0.45e-3);
		P13.setRadius(0.35e-3);
					
		particleHandler.copyAndAddObject(P0);
		particleHandler.copyAndAddObject(P1);
		particleHandler.copyAndAddObject(P2);
		particleHandler.copyAndAddObject(P3);
		particleHandler.copyAndAddObject(P4);
		particleHandler.copyAndAddObject(P5);
		particleHandler.copyAndAddObject(P6);
		particleHandler.copyAndAddObject(P7);
		particleHandler.copyAndAddObject(P8);
		particleHandler.copyAndAddObject(P9);
		particleHandler.copyAndAddObject(P10);
		particleHandler.copyAndAddObject(P11);
		particleHandler.copyAndAddObject(P12);
		particleHandler.copyAndAddObject(P13);
			
		PeriodicBoundary b0;
		b0.set(Vec3D(1,0,0), getXMin(), getXMax());
		boundaryHandler.copyAndAddObject(b0);
		b0.set(Vec3D(0,1,0), getYMin(), getYMax());
		boundaryHandler.copyAndAddObject(b0);
	}

};


int main(int argc UNUSED, char *argv[] UNUSED)
{
    ///Start off my solving the default problem
    periodic_walls problem;
    auto species = new LinearViscoelasticSpecies;
    problem.speciesHandler.addObject(species);
    species->setDensity(2000);
    species->setStiffness(10000);
    species->setDissipation(1);

    problem.setTimeMax(0.11);
    problem.setTimeStep(1.1e-05);
	//problem.setSaveCount(helpers::getSaveCountFromNumberOfSavesAndTimeMaxAndTimestep(1,problem.getTimeMax(),problem.getTimeStep()));
	problem.setSaveCount(helpers::getSaveCountFromNumberOfSavesAndTimeMaxAndTimestep(20, problem.getTimeMax(), problem.getTimeStep()));
	problem.setName("PeriodicWallsUnitTest");

	problem.solve();
	 
	Vec3D goodPos;
	Vec3D goodVel;
    std::vector<BaseParticle*>::iterator pIt = problem.particleHandler.begin();

    goodPos=Vec3D(0.00950076062215577,0.002,0);
    goodVel=Vec3D(-0.005560409816604,0,0);
    if (!(*pIt)->getPosition().compareTo(goodPos, 1e-10))
    {
        logger.log(Log::FATAL, "E0 The particle is in the wrong position. It is %, however is should be %", (*pIt)->getPosition(), goodPos);
    }
    if (!(*pIt)->getVelocity().compareTo(goodVel , 1e-10))
    {
        logger.log(Log::FATAL, "E0 The particle has the wrong velocity. It is %, however is should be %", (*pIt)->getVelocity(), goodVel);
    }
    ++pIt;

    goodPos=Vec3D(0.000725163257251641,0.002,0);
    goodVel=Vec3D(-0.000808302139899,0,0);
    if (!(*pIt)->getPosition().compareTo(goodPos, 1e-10))
    {
        logger.log(Log::FATAL, "E1 The particle is in the wrong position. It is %, however is should be %", (*pIt)->getPosition(), goodPos);
    }
    if (!(*pIt)->getVelocity().compareTo(goodVel , 1e-10))
    {
        logger.log(Log::FATAL, "E1 The particle has the wrong velocity. It is %, however is should be %", (*pIt)->getVelocity(), goodVel);
    }
    ++pIt;
    
    goodPos=Vec3D(0.00950076062215577,0.003,0);
    goodVel=Vec3D(-0.005560409816604,0,0);
    if (!(*pIt)->getPosition().compareTo(goodPos, 1e-10))
    {
        logger.log(Log::FATAL, "E2 The particle is in the wrong position. It is %, however is should be %", (*pIt)->getPosition(), goodPos);
    }
    if (!(*pIt)->getVelocity().compareTo(goodVel , 1e-10))
    {
        logger.log(Log::FATAL, "E2 The particle has the wrong velocity. It is %, however is should be %", (*pIt)->getVelocity(), goodVel);
    }
    ++pIt;

    goodPos=Vec3D(0.000725163257251641,0.003,0);
    goodVel=Vec3D(-0.000808302139899,0,0);
    if (!(*pIt)->getPosition().compareTo(goodPos, 1e-10))
    {
        logger.log(Log::FATAL, "E3 The particle is in the wrong position. It is %, however is should be %", (*pIt)->getPosition(), goodPos);
    }
    if (!(*pIt)->getVelocity().compareTo(goodVel , 1e-10))
    {
        logger.log(Log::FATAL, "E3 The particle has the wrong velocity. It is %, however is should be %", (*pIt)->getVelocity(), goodVel);
    }
    ++pIt;
    
    goodPos=Vec3D(0.0004992393778432442,0.004,0);
    goodVel=Vec3D(0.00556040981661,0,0);
    if (!(*pIt)->getPosition().compareTo(goodPos, 1e-10))
    {
        logger.log(Log::FATAL, "E4 The particle is in the wrong position. It is %, however is should be %", (*pIt)->getPosition(), goodPos);
    }
    if (!(*pIt)->getVelocity().compareTo(goodVel , 1e-10))
    {
        logger.log(Log::FATAL, "E4 The particle has the wrong velocity. It is %, however is should be %", (*pIt)->getVelocity(), goodVel);
    }
    ++pIt;

    goodPos=Vec3D(0.00927483674274562,0.004,0);
    goodVel=Vec3D(0.0008083021398896,0,0);
    if (!(*pIt)->getPosition().compareTo(goodPos, 1e-10))
    {
        logger.log(Log::FATAL, "E5 The particle is in the wrong position. It is %, however is should be %", (*pIt)->getPosition(), goodPos);
    }
    if (!(*pIt)->getVelocity().compareTo(goodVel , 1e-10))
    {
        logger.log(Log::FATAL, "E5 The particle has the wrong velocity. It is %, however is should be %", (*pIt)->getVelocity(), goodVel);
    }
    ++pIt;
    
    goodPos=Vec3D(0.000499239377843248,0.005,0);
    goodVel=Vec3D(0.00556040981661,0,0);
    if (!(*pIt)->getPosition().compareTo(goodPos, 1e-10))
    {
        logger.log(Log::FATAL, "E6 The particle is in the wrong position. It is %, however is should be %", (*pIt)->getPosition(), goodPos);
    }
    if (!(*pIt)->getVelocity().compareTo(goodVel , 1e-10))
    {
        logger.log(Log::FATAL, "E6 The particle has the wrong velocity. It is %, however is should be %", (*pIt)->getVelocity(), goodVel);
    }
    ++pIt;

    goodPos=Vec3D(0.00927483674274562,0.005,0);
    goodVel=Vec3D(0.0008083021398892,0,0);
    if (!(*pIt)->getPosition().compareTo(goodPos, 1e-10))
    {
        logger.log(Log::FATAL, "E7 The particle is in the wrong position. It is %, however is should be %", (*pIt)->getPosition(), goodPos);
    }
    if (!(*pIt)->getVelocity().compareTo(goodVel , 1e-10))
    {
        logger.log(Log::FATAL, "E7 The particle has the wrong velocity. It is %, however is should be %", (*pIt)->getVelocity(), goodVel);
    }
    ++pIt;

    goodPos=Vec3D(0.00149923937784423,0.006,0);
    goodVel=Vec3D(0.005560409816606,0,0);
    if (!(*pIt)->getPosition().compareTo(goodPos, 1e-10))
    {
        logger.log(Log::FATAL, "E8 The particle is in the wrong position. It is %, however is should be %", (*pIt)->getPosition(), goodPos);
    }
    if (!(*pIt)->getVelocity().compareTo(goodVel , 1e-10))
    {
        logger.log(Log::FATAL, "E8 The particle has the wrong velocity. It is %, however is should be %", (*pIt)->getVelocity(), goodVel);
    }
    ++pIt;
    
    goodPos=Vec3D(0.000274836742748357,0.006,0);
    goodVel=Vec3D(0.0008083021398959,0,0);
    if (!(*pIt)->getPosition().compareTo(goodPos, 1e-10))
    {
        logger.log(Log::FATAL, "E9 The particle is in the wrong position. It is %, however is should be %", (*pIt)->getPosition(), goodPos);
    }
    if (!(*pIt)->getVelocity().compareTo(goodVel , 1e-10))
    {
        logger.log(Log::FATAL, "E9 The particle has the wrong velocity. It is %, however is should be %", (*pIt)->getVelocity(), goodVel);
    }
    ++pIt;

    goodPos=Vec3D(0.000368955529803987, 0.000368955529803987,0);
    goodVel=Vec3D(0.005560480643586,0.005560480643586,0);
    if (!(*pIt)->getPosition().compareTo(goodPos, 1e-10))
    {
        logger.log(Log::FATAL, "E10 The particle is in the wrong position. It is %, however is should be %", (*pIt)->getPosition(), goodPos);
    }
    if (!(*pIt)->getVelocity().compareTo(goodVel , 1e-10))
    {
        logger.log(Log::FATAL, "E10 The particle has the wrong velocity. It is %, however is should be %", (*pIt)->getVelocity(), goodVel);
    }
    ++pIt;

    goodPos=Vec3D(0.0094902039201126, 0.0094902039201126 ,0);
    goodVel=Vec3D(0.0008081850585628,0.0008081850585628,0);
    if (!(*pIt)->getPosition().compareTo(goodPos, 1e-10))
    {
        logger.log(Log::FATAL, "E11 The particle is in the wrong position. It is %, however is should be %", (*pIt)->getPosition(), goodPos);
    }
    if (!(*pIt)->getVelocity().compareTo(goodVel , 1e-10))
    {
        logger.log(Log::FATAL, "E11 The particle has the wrong velocity. It is %, however is should be %", (*pIt)->getVelocity(), goodVel);
    }
    ++pIt;

    goodPos=Vec3D(0.00963104447019111, 0.00963104447019111,0.001);
    goodVel=Vec3D(-0.005560480643562,-0.005560480643562,0);
    if (!(*pIt)->getPosition().compareTo(goodPos, 1e-10))
    {
        logger.log(Log::FATAL, "E12 The particle is in the wrong position. It is %, however is should be %", (*pIt)->getPosition(), goodPos);
    }
    if (!(*pIt)->getVelocity().compareTo(goodVel , 1e-10))
    {
        logger.log(Log::FATAL, "E12 The particle has the wrong velocity. It is %, however is should be %", (*pIt)->getVelocity(), goodVel);
    }
    ++pIt;

    goodPos=Vec3D(0.00050979607988006, 0.00050979607988006,0.001);
    goodVel=Vec3D(-0.0008081850586013,-0.0008081850586013,0);
    if (!(*pIt)->getPosition().compareTo(goodPos, 1e-10))
    {
        logger.log(Log::FATAL, "E13 The particle is in the wrong position. It is %, however is should be %", (*pIt)->getPosition(), goodPos);
    }
    if (!(*pIt)->getVelocity().compareTo(goodVel , 1e-10))
    {
        logger.log(Log::FATAL, "E13 The particle has the wrong velocity. It is %, however is should be %", (*pIt)->getVelocity(), goodVel);
    }


}
