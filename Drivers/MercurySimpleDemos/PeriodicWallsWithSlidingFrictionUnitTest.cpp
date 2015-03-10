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
#include "Species/LinearViscoelasticSlidingFrictionSpecies.h"
#include "Species/LinearViscoelasticFrictionSpecies.h"
#include "Mercury3D.h"
#include "Boundaries/PeriodicBoundary.h"
#include "Particles/BaseParticle.h"
#include "Logger.h"
extern Logger<LOG_MAIN_LEVEL> logger;

///This test is a UnitTest for:
///Periodic Particles in combination with HGrid
///Tangential Springs over periodic contacts and during transitions from periodic to normal and reverse
///Removal of Particles while tangential spring information has to be conserved

class PeriodicWallsWithSlidingFrictionUnitTest : public Mercury3D
{
    void setupInitialConditions()
    {
       auto species = speciesHandler.copyAndAddObject(LinearViscoelasticFrictionSpecies());
        species->setDensity(constants::pi / 6.0);

        setName("PeriodicWallsWithSlidingFrictionUnitTest");
        double stiffness = 1e5;
        setParticleDimensions(2);
        species->setDensity(2500);
        species->setStiffness(stiffness);
        species->setSlidingStiffness(2.0 / 7.0 * stiffness);
	    species->setSlidingFrictionCoefficient(0.5);
        species->setRollingStiffness(2.0 / 5.0 * stiffness);
        species->setRollingFrictionCoefficient(0.5);
        species->setTorsionStiffness(2.0 / 5.0 * stiffness);
        species->setTorsionFrictionCoefficient(0.5);

        setGravity(Vec3D(0.0, 0.0, 0.0));
        setTimeMax(8e-2);
        setTimeStep(2.0e-4);
        setFileType(FileType::NO_FILE);

        setXMin(0.0);
        setYMin(0.0);
        setXMax(1.0);
        setYMax(1.0);

        PeriodicBoundary b0;
        b0.set(Vec3D(1, 0, 0), getXMin(), getXMax());
        boundaryHandler.copyAndAddObject(b0);
        b0.set(Vec3D(0, 1, 0), getYMin(), getYMax());
        boundaryHandler.copyAndAddObject(b0);

        BaseParticle P0, P1, P2, P3, P4, P5, P6, P7;

        //Particle to be removed
        P0.setPosition(Vec3D(0.1, 0.1, 0.0));
        P1.setPosition(Vec3D(0.3, 0.3, 0.0));
        //Contacts starts normal becomes periodic
        P2.setPosition(Vec3D(0.6, 0.041, 0.0));
        P2.setAngularVelocity(Vec3D(0.3, 0.6, 0.9));
        P3.setPosition(Vec3D(0.4, 0.001, 0.0));
        //Normal case
        P4.setPosition(Vec3D(0.6, 0.82, 0.0));
        P4.setAngularVelocity(Vec3D(0.3, 0.6, 0.9));
        P5.setPosition(Vec3D(0.4, 0.78, 0.0));
        //Contact starts periodic becomes normal and periodic again
        P6.setPosition(Vec3D(0.02, 0.42, 0.0));
        P6.setAngularVelocity(Vec3D(0.3, 0.6, 0.9));
        P7.setPosition(Vec3D(0.82, 0.38, 0.0));

        P0.setVelocity(Vec3D(0.0, 0.0, 0.0));
        P1.setVelocity(Vec3D(0.0, 0.0, 0.0));
        P2.setVelocity(Vec3D(-1.0, 0.0, 0.0));
        P3.setVelocity(Vec3D(1.0, 0.0, 0.0));
        P4.setVelocity(Vec3D(-1.0, 0.0, 0.0));
        P5.setVelocity(Vec3D(1.0, 0.0, 0.0));
        P6.setVelocity(Vec3D(-1.0, 0.0, 0.0));
        P7.setVelocity(Vec3D(1.0, 0.0, 0.0));

        P0.setRadius(0.1);
        P1.setRadius(0.1);
        P2.setRadius(0.1);
        P3.setRadius(0.1);
        P4.setRadius(0.1);
        P5.setRadius(0.1);
        P6.setRadius(0.1);
        P7.setRadius(0.1);

        particleHandler.copyAndAddObject(P0);
        particleHandler.copyAndAddObject(P1);
        particleHandler.copyAndAddObject(P2);
        particleHandler.copyAndAddObject(P3);
        particleHandler.copyAndAddObject(P4);
        particleHandler.copyAndAddObject(P5);
        particleHandler.copyAndAddObject(P6);
        particleHandler.copyAndAddObject(P7);
    }

protected:

    void actionsBeforeTimeStep()
    {
        static bool FirstParticleRemoved = false;
        static bool SecondParticlRemoved = false;
        if (getTime() > 3.5e-2 && FirstParticleRemoved == false)
        {
            logger.log(Log::VERBOSE, "Just before removing first particle");
            for (auto it = interactionHandler.begin(); it != interactionHandler.end(); ++it)
            {
                logger.log(Log::VERBOSE, "Interaction between particle (id=% index=%) and particle (id=% index=%) delta=%)", (*it)->getP()->getId(), (*it)->getP()->getIndex(), (*it)->getI()->getId(), (*it)->getI()->getIndex(), (*it)->getTimeStamp());
            }

            FirstParticleRemoved = true;
            particleHandler.removeObject(1);

            logger.log(Log::VERBOSE, "Just after removing first particle");
            for (auto it = interactionHandler.begin(); it != interactionHandler.end(); ++it)
            {
                logger.log(Log::VERBOSE, "Interaction between particle (id=% index=%) and particle (id=% index=%) delta=%)", (*it)->getP()->getId(), (*it)->getP()->getIndex(), (*it)->getI()->getId(), (*it)->getI()->getIndex(), (*it)->getTimeStamp());
            }
            logger.log(Log::VERBOSE, "What should have happened:");
            logger.log(Log::VERBOSE, "First the particle (id=1,index=1) is removed");
            logger.log(Log::VERBOSE, "This causes the particle (id=7,index=7) to move to (id=7,index=1)");
            logger.log(Log::VERBOSE, "So the tangential spring witch was between particle (id=7,index=1) and (id=6,index=6) should be move to the last particle and reversed");
        }

        if (getTime() > 4.5e-2 && SecondParticlRemoved == false)
        {
            logger.log(Log::VERBOSE, "Just before removing second particle");
            for (auto it = interactionHandler.begin(); it != interactionHandler.end(); ++it)
            {
                logger.log(Log::VERBOSE, "Interaction between particle (id=% index=%) and particle (id=% index=%) delta=%)", (*it)->getP()->getId(), (*it)->getP()->getIndex(), (*it)->getI()->getId(), (*it)->getI()->getIndex(), (*it)->getTimeStamp());
            }

            SecondParticlRemoved = true;
            particleHandler.removeObject(0);

            logger.log(Log::VERBOSE, "Just after removing second particle");
            for (auto it = interactionHandler.begin(); it != interactionHandler.end(); ++it)
            {
                logger.log(Log::VERBOSE, "Interaction between particle (id=% index=%) and particle (id=% index=%) delta=%)", (*it)->getP()->getId(), (*it)->getP()->getIndex(), (*it)->getI()->getId(), (*it)->getI()->getIndex(), (*it)->getTimeStamp());
            }
            logger.log(Log::VERBOSE, "What should have happened:");
            logger.log(Log::VERBOSE, "First the particle (id=0,index=0) is removed");
            logger.log(Log::VERBOSE, "This causes the particle (id=6,index=6) to move to (id=0,index=0)");
            logger.log(Log::VERBOSE, "So the tangential spring between particle (id=6,index=6) and (id=7,index=1) should be move to the last particle and reversed");
        }
    }

    void printTime() const
    {
    }
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
    ///Start off by solving the default problem
    PeriodicWallsWithSlidingFrictionUnitTest periodicWallsWithSlidingFrictionUnitTest;
    periodicWallsWithSlidingFrictionUnitTest.solve();

    BaseParticle* PNormal;
    BaseParticle* PCase1;
    BaseParticle* PCase2;
    Vec3D shift;
    PNormal = periodicWallsWithSlidingFrictionUnitTest.particleHandler.getObject(4);
    PCase1 = periodicWallsWithSlidingFrictionUnitTest.particleHandler.getObject(0);
    PCase2 = periodicWallsWithSlidingFrictionUnitTest.particleHandler.getObject(2);

    shift = Vec3D(-0.58, -0.4, 0.0);
    if (!PCase1->getPosition().compareTo(PNormal->getPosition() + shift, 1e-10))
    {
        logger.log(Log::FATAL, "E0 The particle is in the wrong position. It is %, however is should be %", PCase1->getPosition(), PNormal->getPosition() + shift);
    }
    if (!PCase1->getVelocity().compareTo(PNormal->getVelocity(), 1e-10))
    {
        logger.log(Log::FATAL, "E1 The particle has the wrong velocity. It is %, however is should be %", PCase1->getVelocity(), PNormal->getVelocity());
    }

    shift = Vec3D(0.0, -0.779, 0.0);
    if (!PCase2->getPosition().compareTo(PNormal->getPosition() + shift, 1e-10))
    {
        logger.log(Log::FATAL, "E2 The particle is in the wrong position. It is %, however is should be %", PCase2->getPosition(), PNormal->getPosition() + shift);
    }
    if (!PCase2->getVelocity().compareTo(PNormal->getVelocity(), 1e-10))
    {
        logger.log(Log::FATAL, "E3 The particle has the wrong velocity. It is %, however is should be %", PCase2->getVelocity(), PNormal->getVelocity());
    }

    PNormal = periodicWallsWithSlidingFrictionUnitTest.particleHandler.getObject(5);
    PCase1 = periodicWallsWithSlidingFrictionUnitTest.particleHandler.getObject(1);
    PCase2 = periodicWallsWithSlidingFrictionUnitTest.particleHandler.getObject(3);

    shift = Vec3D(1.0 - 0.58, -0.4, 0.0);
    if (!PCase1->getPosition().compareTo(PNormal->getPosition() + shift, 1e-10))
    {
        std::cout << PNormal->getPosition() - PCase1->getPosition() << " " << PNormal->getPosition() + PCase1->getPosition() << std::endl;
        logger.log(Log::FATAL, "E4 The particle is in the wrong position. It is %, however is should be %", PCase1->getPosition(), PNormal->getPosition() + shift);
    }
    if (!PCase1->getVelocity().compareTo(PNormal->getVelocity(), 1e-10))
    {
        logger.log(Log::FATAL, "E5 The particle has the wrong velocity. It is %, however is should be %", PCase1->getVelocity(), PNormal->getVelocity());
    }

    shift = Vec3D(0.0, 1.0 - 0.779, 0.0);
    if (!PCase2->getPosition().compareTo(PNormal->getPosition() + shift, 1e-10))
    {
        logger.log(Log::FATAL, "E6 The particle is in the wrong position. It is %, however is should be %", PCase2->getPosition(), PNormal->getPosition() + shift);
    }
    if (!PCase2->getVelocity().compareTo(PNormal->getVelocity(), 1e-10))
    {
        logger.log(Log::FATAL, "E7 The particle has the wrong velocity. It is %, however is should be %", PCase2->getVelocity(), PNormal->getVelocity());
    }
}
