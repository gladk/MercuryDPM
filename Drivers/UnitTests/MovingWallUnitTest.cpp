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

#include <cmath>
#include <iostream>
#include <iomanip>

#include "Logger.h"
#include "DPMBase.h"
#include "Particles/BaseParticle.h"
#include "Species/LinearViscoelasticSlidingFrictionSpecies.h"
#include "Walls/InfiniteWall.h"
extern Logger<LOG_MAIN_LEVEL> logger;

///This tests if moving the wall works with CWall::move(Vec3D velocity,Vec3D dt). 
///The wall is moved in normal and tangential direction and the results 
///are compared with a system where the particles are moved instead of 
///the walls, with the same relative velocities
class MovingWall : public DPMBase
{
public:

    MovingWall()
    {
        species = speciesHandler.copyAndAddObject(LinearViscoelasticSlidingFrictionSpecies());
        particle = particleHandler.copyAndAddObject(BaseParticle());
        wall = wallHandler.copyAndAddObject(InfiniteWall());
    }

    void setupInitialConditions()
    {
        setParticleDimensions(3);
        setXMax(2);
        setYMax(2);
        setZMax(2);
        setSystemDimensions(3);
        species->setDensity(6.0 / constants::pi);
        setGravity(Vec3D(0.0, 0.0, 0.0));
        species->setCollisionTimeAndRestitutionCoefficient(0.01, 1.0, 1.0);
        setTimeStep(0.0002);
        setTimeMax(1.0);
        setSaveCount(helpers::getSaveCountFromNumberOfSavesAndTimeMaxAndTimestep(20, getTimeMax(), getTimeStep()));

        //set particles
        particle->setRadius(0.5);
        particle->setPosition(Vec3D(1.0, 1.0, 1.0));
        particle->setVelocity(Vec3D(0.0, 0.0, -1.0));

        //set walls
        wall->setNormal(Vec3D(0.0, 0.0, -1.0));

    }

protected:
    LinearViscoelasticSlidingFrictionSpecies* species;
    InfiniteWall* wall;
    BaseParticle* particle;
};

/// In the reference case the particle just moves two times as fast
class MovingWallReference : public MovingWall
{
    void setupInitialConditions()
    {
        MovingWall::setupInitialConditions();
        particle->setVelocity(Vec3D(0.0, 0.0, -2.0));
        wall->setPosition(Vec3D(0.0, 0.0, 0.0));
        wall->setVelocity(Vec3D(0.0, 0.0, 0.0));
    }
};

class MovingWallSimpleIntegration : public MovingWall
{
    void setupInitialConditions()
    {
        MovingWall::setupInitialConditions();
        wall->setPosition(Vec3D(0.0, 0.0, 0.0));
        wall->setVelocity(Vec3D(0.0, 0.0, 1.0));
    }
};

class MovingWallPrescribedPosition : public MovingWall
{
    void setupInitialConditions()
    {
        MovingWall::setupInitialConditions();
        wall->setPrescribedPosition([] (double time)
        {
            return Vec3D(0.0,0.0,time);
        });
    }
};

class MovingWallPrescribedVelocity : public MovingWall
{
    void setupInitialConditions()
    {
        MovingWall::setupInitialConditions();
        wall->setPosition(Vec3D(0.0, 0.0, 0.0));
        wall->setPrescribedVelocity([] (double time)
        {
            return Vec3D(0.0,0.0,1);
        });

    }
};

class MovingWallPrescribedPositionPrescribedVelocity : public MovingWall
{
    void setupInitialConditions()
    {
        MovingWall::setupInitialConditions();
        wall->setPrescribedPosition([] (double time)
        {
            return Vec3D(0.0,0.0,time);
        });
        wall->setPrescribedVelocity([] (double time)
        {
            return Vec3D(0.0,0.0,1);
        });
    }
};

class MovingWallTangential : public MovingWall
{
public:
    void setupInitialConditions()
    {
        MovingWall::setupInitialConditions();
        setGravity(Vec3D(0,0,-1));
        species->setCollisionTimeAndRestitutionCoefficient(0.01, 0.5, 1);
        species->setSlidingFrictionCoefficient(0.5);
        species->setSlidingStiffness(species->getStiffness() * 2.0 / 7.0);
        species->setSlidingDissipation(species->getDissipation());
        particle->setPosition(Vec3D(1.0, 1.0, particleHandler.getObject(0)->getRadius()));
        particle->setVelocity(Vec3D(0.0, 0.0, 0.0));
    }
};

/// In the reference case the particle just moves two times as fast
class MovingWallTangentialReference : public MovingWallTangential
{
    void setupInitialConditions()
    {
        MovingWallTangential::setupInitialConditions();
        particle->setVelocity(Vec3D(-1.0, 0.0, 0.0));
        wall->setPosition(Vec3D(0.0, 0.0, 0.0));
        wall->setVelocity(Vec3D(0.0, 0.0, 0.0));
    }
};

class MovingWallTangentialSimpleIntegration : public MovingWallTangential
{
    void setupInitialConditions()
    {
        MovingWallTangential::setupInitialConditions();
        wall->setPosition(Vec3D(0.0, 0.0, 0.0));
        wall->setVelocity(Vec3D(1.0, 0.0, 0.0));
    }
};

class MovingWallTangentialPrescribedPosition : public MovingWallTangential
{
    void setupInitialConditions()
    {
        MovingWallTangential::setupInitialConditions();
        wall->setPrescribedPosition([] (double time)
        {
            return Vec3D(time,0.0,0.0);
        });
    }
};

class MovingWallTangentialPrescribedVelocity : public MovingWallTangential
{
    void setupInitialConditions()
    {
        MovingWallTangential::setupInitialConditions();
        wall->setPosition(Vec3D(0.0, 0.0, 0.0));
        wall->setPrescribedVelocity([] (double time)
        {
            return Vec3D(1.0,0.0,0.0);
        });

    }
};

class MovingWallTangentialPrescribedPositionPrescribedVelocity : public MovingWallTangential
{
    void setupInitialConditions()
    {
        MovingWallTangential::setupInitialConditions();
        wall->setPrescribedPosition([] (double time)
        {
            return Vec3D(time,0.0,0.0);
        });
        wall->setPrescribedVelocity([] (double time)
        {
            return Vec3D(1.0,0.0,0.0);
        });
    }
};

void compareParticles(BaseParticle* Ptest, BaseParticle* Pref, double absError, Vec3D positionDifference, Vec3D velocityDifference, Vec3D orientationDifference, Vec3D AngularVelocityDifference)
{
    if (mathsFunc::compare(Ptest->getPosition(), Pref->getPosition() + positionDifference, absError))
    {
        logger.log(Log::FATAL, "The particle has the wrong position. It is %, however is should be %, the difference is %", Ptest->getPosition(), Pref->getPosition() + positionDifference, Ptest->getPosition() - Pref->getPosition() - positionDifference);
    }
    if (mathsFunc::compare(Ptest->getVelocity(), Pref->getVelocity() + velocityDifference, absError))
    {
        logger.log(Log::FATAL, "The particle has the wrong velocity. It is %, however is should be %, the difference is %", Ptest->getVelocity(), Pref->getVelocity() + velocityDifference, Ptest->getVelocity() - Pref->getVelocity() - velocityDifference);
    }
    if (mathsFunc::compare(Ptest->getOrientation(), Pref->getOrientation() + orientationDifference, absError))
    {
        logger.log(Log::FATAL, "The particle has the wrong orientation. It is %, however is should be %, the difference is %", Ptest->getOrientation(), Pref->getOrientation() + orientationDifference, Ptest->getOrientation() - Pref->getOrientation() - orientationDifference);
    }
    if (mathsFunc::compare(Ptest->getAngularVelocity(), Pref->getAngularVelocity() + AngularVelocityDifference, absError))
    {
        logger.log(Log::FATAL, "The particle has the wrong angular velocity. It is %, however is should be %, the difference is %", Ptest->getAngularVelocity(), Pref->getAngularVelocity() + AngularVelocityDifference, Ptest->getAngularVelocity() - Pref->getAngularVelocity() - AngularVelocityDifference);
    }
}

int main(int argc UNUSED, char *argv[] UNUSED)
{

    {
        MovingWallReference problem0;
        problem0.setName("MovingWallUnitTest_Normal_Reference");
        problem0.solve();
        MovingWallSimpleIntegration problem1;
        problem1.setName("MovingWallUnitTest_Normal_SimpleIntegration");
        problem1.solve();
        MovingWallPrescribedPosition problem2;
        problem2.setName("MovingWallUnitTest_Normal_PrescribedPosition");
        problem2.solve();
        MovingWallPrescribedVelocity problem3;
        problem3.setName("MovingWallUnitTest_Normal_PrescribedVelocity");
        problem3.solve();
        MovingWallPrescribedPositionPrescribedVelocity problem4;
        problem4.setName("MovingWallUnitTest_Normal_PrescribedPositionPrescribedVelocity");
        problem4.solve();

        Vec3D positionDifference = Vec3D(0.0, 0.0, problem0.getTime());
        Vec3D velocityDifference = Vec3D(0.0, 0.0, 1.0);
        Vec3D orientationDifference = Vec3D(0.0, 0.0, 0.0);
        Vec3D angularVelocityDifference = Vec3D(0.0, 0.0, 0.0);
        compareParticles(problem1.particleHandler.getLastObject(), problem0.particleHandler.getLastObject(), 1e-10, positionDifference, velocityDifference, orientationDifference, angularVelocityDifference);
        compareParticles(problem2.particleHandler.getLastObject(), problem0.particleHandler.getLastObject(), 1e-10, positionDifference, velocityDifference, orientationDifference, angularVelocityDifference);
        compareParticles(problem3.particleHandler.getLastObject(), problem0.particleHandler.getLastObject(), 1e-10, positionDifference, velocityDifference, orientationDifference, angularVelocityDifference);
        compareParticles(problem4.particleHandler.getLastObject(), problem0.particleHandler.getLastObject(), 1e-10, positionDifference, velocityDifference, orientationDifference, angularVelocityDifference);
    }
    {
        MovingWallTangentialReference problem5;
        problem5.setName("MovingWallUnitTest_Tangential_Reference");
        problem5.solve();
        MovingWallTangentialSimpleIntegration problem6;
        problem6.setName("MovingWallUnitTest_Tangential_SimpleIntegration");
        problem6.solve();
        MovingWallTangentialPrescribedPosition problem7;
        problem7.setName("MovingWallUnitTest_Tangential_PrescribedPosition");
        problem7.solve();
        MovingWallTangentialPrescribedVelocity problem8;
        problem8.setName("MovingWallUnitTest_Tangential_PrescribedVelocity");
        problem8.solve();
        MovingWallTangentialPrescribedPositionPrescribedVelocity problem9;
        problem9.setName("MovingWallUnitTest_Tangential_PrescribedPositionPrescribedVelocity");
        problem9.solve();

        Vec3D positionDifference = Vec3D(problem5.getTime(), 0.0, 0.0);
        Vec3D velocityDifference = Vec3D(1.0, 0.0, 0.0);
        Vec3D orientationDifference = Vec3D(0.0, 0.0, 0.0);
        Vec3D angularVelocityDifference = Vec3D(0.0, 0.0, 0.0);
        compareParticles(problem6.particleHandler.getLastObject(), problem5.particleHandler.getLastObject(), 1e-10, positionDifference, velocityDifference, orientationDifference, angularVelocityDifference);
        compareParticles(problem7.particleHandler.getLastObject(), problem5.particleHandler.getLastObject(), 1e-10, positionDifference, velocityDifference, orientationDifference, angularVelocityDifference);
        compareParticles(problem8.particleHandler.getLastObject(), problem5.particleHandler.getLastObject(), 1e-10, positionDifference, velocityDifference, orientationDifference, angularVelocityDifference);
        compareParticles(problem9.particleHandler.getLastObject(), problem5.particleHandler.getLastObject(), 1e-10, positionDifference, velocityDifference, orientationDifference, angularVelocityDifference);
    }

    /*
    diff ../../../Source/Drivers/UnitTests/SelfTestData/movingWall.data MovingWallUnitTest_Normal_PrescribedPosition.data
    diff ../../../Source/Drivers/UnitTests/SelfTestData/movingWall.data MovingWallUnitTest_Normal_PrescribedVelocity.data
    diff ../../../Source/Drivers/UnitTests/SelfTestData/movingWall.data MovingWallUnitTest_Normal_PrescribedPositionPrescribedVelocity.data
    diff ../../../Source/Drivers/UnitTests/SelfTestData/movingWall.data MovingWallUnitTest_Normal_SimpleIntegration.data
    diff ../../../Source/Drivers/UnitTests/SelfTestData/noMovingWall.data MovingWallUnitTest_Normal_Reference.data

    diff ../../../Source/Drivers/UnitTests/SelfTestData/movingWallTangential.data MovingWallUnitTest_Tangential_PrescribedPosition.data
    diff ../../../Source/Drivers/UnitTests/SelfTestData/movingWallTangential.data MovingWallUnitTest_Tangential_PrescribedVelocity.data
    diff ../../../Source/Drivers/UnitTests/SelfTestData/movingWallTangential.data MovingWallUnitTest_Tangential_PrescribedPositionPrescribedVelocity.data
    diff ../../../Source/Drivers/UnitTests/SelfTestData/movingWallTangential.data MovingWallUnitTest_Tangential_SimpleIntegration.data
    diff ../../../Source/Drivers/UnitTests/SelfTestData/noMovingWallTangential.data MovingWallUnitTest_Tangential_Reference.data
    */


    return 0;
}
