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

#include "Mercury2D.h"
#include "Particles/BaseParticle.h"
#include "Walls/InfiniteWall.h"
#include <iostream>
#include "Species/Species.h"
#include "Species/LinearViscoelasticSpecies.h"

/// In this file two particles are symmetrically placed in a bi-axial box are allowed to jump around under gravity. It tests walls gravity and symmetry.

class TwoParticleElasticCollision : public Mercury2D {

	void setupInitialConditions()
	{
		BaseParticle P0,P1;
		P0.setPosition(Vec3D(0.006,0.005,0.0));
		P1.setPosition(Vec3D(0.004,0.005,0.0));
	
		P0.setVelocity(Vec3D(-0.1,0.0,0.0));
		P1.setVelocity(Vec3D( 0.1,0.0,0.0));
	
		P0.setRadius(0.0005);
		P1.setRadius(0.0005);
		particleHandler.copyAndAddObject(P0);
		particleHandler.copyAndAddObject(P1);
		
		wallHandler.clear();
		InfiniteWall w0;
		w0.set(Vec3D(-1, 0, 0), -getXMin());
		wallHandler.copyAndAddObject(w0);
		w0.set(Vec3D( 1, 0, 0),  getXMax());
		wallHandler.copyAndAddObject(w0);
		w0.set(Vec3D( 0,-1, 0), -getYMin());
		wallHandler.copyAndAddObject(w0);
		w0.set(Vec3D( 0, 1, 0),  getYMax());
		wallHandler.copyAndAddObject(w0);		
	}

};

int main(int argc UNUSED, char *argv[] UNUSED)
{
	TwoParticleElasticCollision twoParticleElasticCollisionProblem;
    twoParticleElasticCollisionProblem.setName("TwoParticleElasticCollisionSelfTest");

    auto species = new LinearViscoelasticSpecies;
    twoParticleElasticCollisionProblem.speciesHandler.addObject(species);
    species->setDensity(2000);
    species->setStiffness(1e4);

	twoParticleElasticCollisionProblem.setTimeMax(0.25);
	twoParticleElasticCollisionProblem.setSaveCount(10);
    twoParticleElasticCollisionProblem.setTimeStep(2e-5);
	twoParticleElasticCollisionProblem.getFStatFile().setFileType(FileType::NO_FILE);
	twoParticleElasticCollisionProblem.solve();
}
