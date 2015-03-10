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
#include <Species/LinearViscoelasticSpecies.h>

/// This case does a single elastic particle falling on an infinite plane. The k is chosen so that the maximum overlap with the wall is around 2% of the partcles dimater; whereas, the time is taken to ensure 50 steps with a collision.
class FreeFallSelfTest : public Mercury2D
{
public:
	
	void setupInitialConditions()
	{
		InfiniteWall w0;
		w0.set(Vec3D(0,-1,0), -getYMin());
		wallHandler.copyAndAddObject(w0);
		
		BaseParticle p0;
		p0.setPosition(Vec3D(getXMax()/2,getYMax()*0.95,0.0));
		p0.setVelocity(Vec3D(0.0,0.0,0.0));
		p0.setRadius(0.005);
		particleHandler.copyAndAddObject(p0);
	}

};

int main(int argc, char *argv[])
{
	std::cout<<"Single particle bouncing vertically on the bottom plate"<<std::endl;
	///Start off my solving the default problem
	FreeFallSelfTest freeFallSelfTestProblem;
    auto species = new LinearViscoelasticSpecies;
    freeFallSelfTestProblem.speciesHandler.addObject(species);
    species->setDensity(2000.0);
    freeFallSelfTestProblem.setParticleDimensions(3);
    species->setStiffness(8000000.0);

    freeFallSelfTestProblem.setName("FreeFallSelfTest");
	freeFallSelfTestProblem.setSaveCount(500);
	freeFallSelfTestProblem.getFStatFile().setFileType(FileType::NO_FILE);
    freeFallSelfTestProblem.setTimeStep(1e-6);
	freeFallSelfTestProblem.setYMax(0.1);
	freeFallSelfTestProblem.setXMax(0.01);
    freeFallSelfTestProblem.solve(argc, argv);
}
