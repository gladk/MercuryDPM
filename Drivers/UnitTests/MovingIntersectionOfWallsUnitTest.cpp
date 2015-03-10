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
#include "Walls/IntersectionOfWalls.h"
#include "Species/LinearViscoelasticSpecies.h"
extern Logger<LOG_MAIN_LEVEL> logger;

///This tests if moving walls also works with IntersectionOfWalls.
///An intersection of walls in the shape of a triangle is created and from all three directions particles are bounced of on the faces and edges.
///The same situation is performed in a moving reference frame.

class MovingIntersectionOfWallsUnitTest_Basic : public DPMBase
{
public:

    MovingIntersectionOfWallsUnitTest_Basic()
    {
        species = speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());

    }

    void setupInitialConditions()
    {
        setParticleDimensions(2);
        setSystemDimensions(2);
        setXMin(-2.0);
        setXMax(2.0);
        setYMin(-2.0);
        setYMax(2.0);
        species->setDensity(6.0 / constants::pi);
        setGravity(Vec3D(0.0, 0.0, 0.0));
        species->setCollisionTimeAndRestitutionCoefficient(0.01, 1.0, 1.0);
        setTimeStep(0.0002);
        setTimeMax(1.0);
        setSaveCount(helpers::getSaveCountFromNumberOfSavesAndTimeMaxAndTimestep(20, getTimeMax(), getTimeStep()));

        p1f = particleHandler.copyAndAddObject(BaseParticle());
        p2f = particleHandler.copyAndAddObject(BaseParticle());
        p3f = particleHandler.copyAndAddObject(BaseParticle());
        p1e = particleHandler.copyAndAddObject(BaseParticle());
        p2e = particleHandler.copyAndAddObject(BaseParticle());
        p3e = particleHandler.copyAndAddObject(BaseParticle());
        triangle = wallHandler.copyAndAddObject(IntersectionOfWalls());
        std::vector<Vec3D> points={Vec3D(0.5,-std::sqrt(3)/6.0,0.0),Vec3D(0.0,std::sqrt(3)/3.0,0.0),Vec3D(-0.5,-std::sqrt(3)/6.0,0.0),Vec3D(0.5,-std::sqrt(3)/6.0,0.0)};
        triangle->createOpenPrism(points,Vec3D(0.0,0.0,1.0));
                        
        p1f->setRadius(0.1);
        p2f->setRadius(0.1);
        p3f->setRadius(0.1);      
        p1f->setPosition(Vec3D(-0.5, std::sqrt(3)/6.0,0.0));
        p2f->setPosition(Vec3D(-0.0,-std::sqrt(3)/3.0,0.0));
        p3f->setPosition(Vec3D( 0.5, std::sqrt(3)/6.0,0.0));
        p1f->setVelocity(Vec3D( 0.5,-std::sqrt(3)/6.0,0.0));
        p2f->setVelocity(Vec3D( 0.0, std::sqrt(3)/3.0,0.0));
        p3f->setVelocity(Vec3D(-0.5,-std::sqrt(3)/6.0,0.0));
        p1e->setRadius(0.1);
        p2e->setRadius(0.1);
        p3e->setRadius(0.1);      
        p1e->setPosition(1.5*Vec3D( 0.5,-std::sqrt(3)/6.0,0.0));
        p2e->setPosition(1.5*Vec3D( 0.0, std::sqrt(3)/3.0,0.0));
        p3e->setPosition(1.5*Vec3D(-0.5,-std::sqrt(3)/6.0,0.0));
        p1e->setVelocity(Vec3D(-0.5, std::sqrt(3)/6.0,0.0));
        p2e->setVelocity(Vec3D( 0.0,-std::sqrt(3)/3.0,0.0));
        p3e->setVelocity(Vec3D( 0.5, std::sqrt(3)/6.0,0.0));
	}

    BaseParticle* p1f;
    BaseParticle* p2f;
    BaseParticle* p3f;
    BaseParticle* p1e;
    BaseParticle* p2e;
    BaseParticle* p3e;
    IntersectionOfWalls* triangle;

private:
    LinearViscoelasticSpecies* species;
};


class MovingIntersectionOfWallsUnitTest_MovingReferenceFrame : public MovingIntersectionOfWallsUnitTest_Basic
{
public:
	void setupInitialConditions()
	{
		MovingIntersectionOfWallsUnitTest_Basic::setupInitialConditions();
		triangle->setVelocity(velocity);
		p1f->accelerate(velocity);
		p2f->accelerate(velocity);
		p3f->accelerate(velocity);
		p1e->accelerate(velocity);
		p2e->accelerate(velocity);
		p3e->accelerate(velocity);
	}
	Vec3D velocity=Vec3D(1.0,0.0,0.0); 	
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
	MovingIntersectionOfWallsUnitTest_Basic basic;
	MovingIntersectionOfWallsUnitTest_MovingReferenceFrame movingReferenceFrame;
	basic.setName("MovingIntersectionOfWallsUnitTest_Basic");
	movingReferenceFrame.setName("MovingIntersectionOfWallsUnitTest_MovingReferenceFrame");
	basic.solve();
	movingReferenceFrame.solve();
	compareParticles(movingReferenceFrame.p1f,basic.p1f, 1e-10, movingReferenceFrame.velocity*movingReferenceFrame.getTime(), movingReferenceFrame.velocity, Vec3D(0.0,0.0,0.0), Vec3D(0.0,0.0,0.0));
	compareParticles(movingReferenceFrame.p2f,basic.p2f, 1e-10, movingReferenceFrame.velocity*movingReferenceFrame.getTime(), movingReferenceFrame.velocity, Vec3D(0.0,0.0,0.0), Vec3D(0.0,0.0,0.0));
	compareParticles(movingReferenceFrame.p3f,basic.p3f, 1e-10, movingReferenceFrame.velocity*movingReferenceFrame.getTime(), movingReferenceFrame.velocity, Vec3D(0.0,0.0,0.0), Vec3D(0.0,0.0,0.0));
	compareParticles(movingReferenceFrame.p1e,basic.p1e, 1e-10, movingReferenceFrame.velocity*movingReferenceFrame.getTime(), movingReferenceFrame.velocity, Vec3D(0.0,0.0,0.0), Vec3D(0.0,0.0,0.0));
	compareParticles(movingReferenceFrame.p2e,basic.p2e, 1e-10, movingReferenceFrame.velocity*movingReferenceFrame.getTime(), movingReferenceFrame.velocity, Vec3D(0.0,0.0,0.0), Vec3D(0.0,0.0,0.0));
	compareParticles(movingReferenceFrame.p3e,basic.p3e, 1e-10, movingReferenceFrame.velocity*movingReferenceFrame.getTime(), movingReferenceFrame.velocity, Vec3D(0.0,0.0,0.0), Vec3D(0.0,0.0,0.0));
}
