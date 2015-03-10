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

#include "Mercury3D.h"
#include "StatisticsVector.h"
#include "Particles/BaseParticle.h"
#include "Walls/InfiniteWall.h"

#include <cmath>
#include <iostream>
#include <iomanip>
#include <Species/LinearViscoelasticSpecies.h>

class SquarePacking : public StatisticsVector<O> {

public:

	void setupInitialConditions()
	{
		// make sure the number of particles is right
		std::cout << "Creating a cubic packing of " << N << "^3 particles" << std::endl;
		double Radius = .5;
		
		//set Particles' position, radius, velocity and bounding box
		setXMin(0); setYMin(0);	setZMin(0);
		
		setXMax(5);
		setYMax(5);
		BaseParticle P0;
		for (int i=0;i<N;i++)
            for (int j=0;j<N;j++)
            {

                P0.setRadius(Radius);
                P0.setVelocity(Vec3D(0.0,0.0,0.0));
                P0.setPosition(Radius*Vec3D(1.+2.*i,1.+2.*j,0.0));
                //setXMax(std::max(getXMax(),P0.getPosition().X+Radius));
                //setYMax(std::max(getYMax(),P0.getPosition().Y+Radius));
                particleHandler.copyAndAddObject(P0);

            }	
		setZMax(0.1);

		//set walls
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
	
	int N;
	
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
	SquarePacking problem;
	problem.setName("SquarePackingSelfTest");
    auto species=problem.speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());
    //set the number of particles
    problem.N=5;
    problem.setSystemDimensions(2);
	problem.setParticleDimensions(2);
    species->setDensity(6.0/constants::pi*2./3.);
    //species->setDensity(1.9098593*2/3);
	problem.setGravity(Vec3D(0.,-1,0.));
	species->setCollisionTimeAndRestitutionCoefficient(.01,.1,1.);
    problem.setTimeStep(.0002);
	problem.setTimeMax(1.0);
	problem.setSaveCount(1000);

    problem.setCGTimeMin(problem.getTimeMax()*.98);
    problem.setTimeMaxStat(1e20);

    std::cout << "Relax the packing" << std::endl;
	problem.solve();

	std::cout << "Get statistics" << std::endl;
	StatisticsVector<Y> stats("SquarePackingSelfTest");
    stats.getStatFile().setName("SquarePackingSelfTest.Y.stat");
	double n = 500;
	stats.setN(n);
	stats.setCGWidth(.1);
	stats.setCGTimeMin(problem.getTimeMax()*.98);
	stats.setTimeMaxStat(1e20);
	//stats.verbose();
	//stats.set_boundedDomain(true);
	stats.statistics_from_fstat_and_data();

	std::cout << "Get fully averaged statistics" << std::endl;
	StatisticsVector<O> statsO("SquarePackingSelfTest");
	statsO.getStatFile().setName("SquarePackingSelfTest.O.stat");
    statsO.setCGTimeMin(problem.getTimeMax()*.98);
    statsO.setTimeMaxStat(1e20);
	statsO.statistics_from_fstat_and_data();
	// should give you Density 1
}

