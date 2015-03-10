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
#include "Particles/BaseParticle.h"
#include "Walls/InfiniteWall.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <Species/LinearViscoelasticSlidingFrictionSpecies.h>


///This tests if moving the wall works with CWall::move(Vec3D velocity,Vec3D dt). 
///The wall is moved in normal and tangential direction and the results 
///are compared with a system where the particles are moved instead of 
///the walls, with the same relative velocities
class MovingWalls : public DPMBase {
public:

    MovingWalls ()
    {
        species = speciesHandler.copyAndAddObject(LinearViscoelasticSlidingFrictionSpecies());
    }
	void setupInitialConditions()
	{
        setParticleDimensions(3);
		setXMax(2);
		setYMax(2);
		setZMax(2);
        setSystemDimensions(3);
        species->setDensity(6/constants::pi);
		setGravity(Vec3D(0,0,0));
        species->setCollisionTimeAndRestitutionCoefficient(0.01,1,1);
        setTimeStep(.0002);
		setTimeMax(1);
		setSaveCount(helpers::getSaveCountFromNumberOfSavesAndTimeMaxAndTimestep(20, getTimeMax(), getTimeStep()));

		//set particles
		BaseParticle P0;
		P0.setRadius(0.5);
		P0.setPosition(Vec3D(1,1,1));
		P0.setVelocity(Vec3D(0,0,-1));
		particleHandler.copyAndAddObject(P0);
		
		//set walls
		InfiniteWall w0;
		w0.set(Vec3D(0, 0, -1), getZMin());
		wallHandler.copyAndAddObject(w0);
	}
	
	void actionsBeforeTimeStep() {
		wallHandler.getObject(0)->move(Vec3D(0,0,1), getTimeStep());
	}

protected:
    LinearViscoelasticSlidingFrictionSpecies* species;
};

class OldMovingWalls : public MovingWalls {
	void actionsBeforeTimeStep() {
        ///\todo TW: Ant, are you sure this is right?
	/// Yes, Thoams it is.
		wallHandler.getObject(0)->move(-getTime());
	}
};

class NoMovingWalls : public MovingWalls {
	void setupInitialConditions() {
		MovingWalls::setupInitialConditions();
		particleHandler.getObject(0)->setVelocity(Vec3D(0,0,-2));
	}

	void actionsBeforeTimeStep() {
	}
};

class MovingWallsTangential : public MovingWalls {
public:
	void setupInitialConditions() {
		MovingWalls::setupInitialConditions();
		setGravity(Vec3D(0,0,-1));
        species->setCollisionTimeAndRestitutionCoefficient(0.01,.5,1);
        species->setSlidingFrictionCoefficient(.5);
        species->setSlidingStiffness(species->getStiffness()*2./7.);
        species->setSlidingDissipation(species->getDissipation());
		particleHandler.getObject(0)->setPosition(Vec3D(1,1,particleHandler.getObject(0)->getRadius()));
		particleHandler.getObject(0)->setVelocity(Vec3D(0,0,0));
	}

	void actionsBeforeTimeStep() {
		wallHandler.getObject(0)->move(Vec3D(1,0,0), getTimeStep());
	}
};

class OldMovingWallsTangential : public MovingWallsTangential {
	void actionsBeforeTimeStep() {
	}
};

class NoMovingWallsTangential : public MovingWallsTangential {
	//Compared to MovingWall case the reference is changed i.e. the particle now moves and the wall fixed; show give same answer case
	void setupInitialConditions() {
		MovingWallsTangential::setupInitialConditions();
		particleHandler.getObject(0)->setVelocity(Vec3D(-1,0,0));
	}

	void actionsBeforeTimeStep() {
	}
};


int main(int argc UNUSED, char *argv[] UNUSED)
{
	std::cout << std::endl << "Simulation of one particle, d=1, m=1 interacting with a plane;\n testing the effect of normal plane velocity; normal elastic forces only" << std::endl;
	std::cout << std::endl << "at t=0: particle: r(1,1,1) v(0,0,-1), wall: v(0,0,1), gravity(0,0,0)" << std::endl;
	MovingWalls problem;
	problem.setName("movingWall");
	problem.solve();
	std::cout << "v_z(tmax) = " << std::setprecision(14) << problem.particleHandler.getObject(0)->getVelocity().Z << " (should be 3)" << std::endl;

	std::cout << std::endl << "at t=0: particle: r(1,1,1) v(0,0,-1), wall: v(0,0,1), gravity(0,0,0),\n wall has no velocity, only position, prescribed" << std::endl;
	OldMovingWalls problem2;
	problem2.setName("oldMovingWall");
	problem2.solve();
	std::cout << "v_z(tmax) = " << std::setprecision(14) << problem2.particleHandler.getObject(0)->getVelocity().Z << std::endl;

	std::cout << std::endl << "at t=0: particle: r(1,1,1) v(0,0,-2), wall: v(0,0,0), gravity(0,0,0)" << std::endl;
	NoMovingWalls problem0;
	problem0.setName("noMovingWall");
	problem0.solve();
	std::cout << "v_z(tmax)+1 = " << std::setprecision(14) << problem0.particleHandler.getObject(0)->getVelocity().Z+1 << std::endl;

	std::cout << std::endl << "Simulation of one particle, d=1, m=1 interacting with a plane;\n testing the effect of tangential plane velocity; viscoelastic frictional forces" << std::endl;
	
	std::cout << std::endl << "at t=0: particle: r(1,1,1/2) v(0,0,0), wall: v(1,0,0), gravity(0,0,-1)" << std::endl;
	//note: particle starts rolling, does not reach full velocity of the wall
	MovingWallsTangential problem3;
	problem3.setName("movingWallTangential");
	problem3.solve();
	std::cout << "v_x(tmax) = " << std::setprecision(14) << problem3.particleHandler.getObject(0)->getVelocity().X  << " (should be 2/7)" << std::endl;
	
	std::cout << std::endl << "at t=0: particle: r(1,1,1/2) v(0,0,0), wall: v(1,0,0), gravity(0,0,-1),\n wall has no velocity, only position, prescribed" << std::endl;
	OldMovingWallsTangential problem4;
	problem4.setName("oldMovingWallTangential");
	problem4.solve();
	std::cout << "v_x(tmax) = " << std::setprecision(14) << problem4.particleHandler.getObject(0)->getVelocity().X << std::endl;

	std::cout << std::endl << "at t=0: particle: r(1,1,1/2) v(-1,0,0), wall: v(0,0,0), gravity(0,0,-1)" << std::endl;
	NoMovingWallsTangential problem5;
	problem5.setName("noMovingWallTangential");
	problem5.solve();
	std::cout << "v_x(tmax)+1 = " << std::setprecision(14) << problem5.particleHandler.getObject(0)->getVelocity().X+1 << std::endl;

    return 0;
}
