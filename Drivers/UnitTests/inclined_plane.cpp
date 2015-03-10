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
#include <iostream>
#include <Species/LinearViscoelasticSlidingFrictionSpecies.h>

class incl_plane : public DPMBase {
public:
	
	void setupInitialConditions()
	{
		InfiniteWall w0;
		w0.set(Vec3D(0,-1,0),-getYMin());
		wallHandler.copyAndAddObject(w0);
		
		BaseParticle p0; //important
		p0.setRadius(0.005);
		p0.setPosition(Vec3D((getXMin()+getXMax())/2.0,getYMin()+p0.getRadius(),0));
		p0.setVelocity(Vec3D(0.0,0.0,0.0));
		particleHandler.copyAndAddObject(p0);
	}

};

int main(int argc, char *argv[])
{
	std::cout<<"Single particle rolling on a bottom plate"<<std::endl;
	///Start off my solving the default problem
	incl_plane problem;
    
    auto species = problem.speciesHandler.copyAndAddObject(LinearViscoelasticSlidingFrictionSpecies());
    problem.setName("inclined_plane");
	problem.setSaveCount(5000);
    problem.setSystemDimensions(2);
	problem.setParticleDimensions(3);
	Mdouble theta = 15.0*constants::pi/180.0;
	problem.setGravity(9.8*Vec3D(std::sin(theta),-std::cos(theta),0.0));
	species->setDensity(2000);
    species->setStiffness(10000);
    species->setDissipation(0);
	species->setSlidingStiffness(10000);
	species->setSlidingDissipation(0);
	species->setSlidingFrictionCoefficient(0.5);
	//species->setRollingStiffness(10000);
	//species->setRollingDissipation(0);
	//species->setRollingFrictionCoefficient(0.5);
	//tc=7e-5
    problem.setTimeStep(2e-6);
	problem.setTimeMax(1.0);
	problem.setXMax(0.1);
	problem.setYMax(0.1);
	problem.setZMax(0.1);
	problem.solve(argc, argv);
	problem.write(std::cout, true);
	std::cout << problem.particleHandler.getObject(0)->getVelocity() << std::endl;

}
