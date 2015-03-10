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


class NewtonsCradleSelftest : public DPMBase {
	
public:
	
	void setupInitialConditions()
	{
		// make sure the number of particles is right
		std::cout << "Creating a cubic packing of " << N << "^3 particles" << std::endl;
		//set_N(mathsFunc::cubic(N));
		double Radius = 1;
		
		//set Particles' position, radius, velocity and bounding box
		setXMin(0);
		setYMin(0);
		setZMin(0);
		
		setXMax(2.*Radius);
		setYMax(2.*Radius);
		setZMax(N*2.*Radius+.5);
		
		BaseParticle P0;
		for (int i=0;i<1;i++)
			for (int j=0;j<1;j++)
				for (int k=0;k<N;k++)
				{
					P0.setRadius(Radius);
					P0.setVelocity(Vec3D(0.0,0.0,0.0));
					P0.setPosition(Radius*Vec3D(1.+2.*i,1.+2.*j,1.+2.*k));
					particleHandler.copyAndAddObject(P0);
				}	
		
		//set walls
		InfiniteWall w;
		w.set(Vec3D(0.0,0.0,-1.0),-getZMin());
        wallHandler.copyAndAddObject(w);
	}
	
	int N;	
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
	NewtonsCradleSelftest problem;
	problem.setName("NewtonsCradleSelftest");
    auto species=problem.speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());
    problem.N=5;	//set the number of particles
    problem.setSystemDimensions(3);
	problem.setParticleDimensions(3);
	species->setDensity(6./constants::pi/8*(2*problem.N+.5)*1.05*1.05);
	problem.setGravity(Vec3D(0.,0.,-1.));
	species->setCollisionTimeAndRestitutionCoefficient(.01,.1,1.);
    problem.setTimeStep(.0001);
	problem.setTimeMax(8.0);
	problem.setSaveCount(1000);
	std::cout << "Relax the packing" << std::endl;
	problem.solve();
	
	std::cout << "Get statistics" << std::endl;	
	std::string str[3];
	str[0]="Gaussian";
	str[1]="HeavisideSphere";
	str[2]="Lucy";
	
	int i=0;
	double w=0.46/3.;
	StatisticsVector<Z> stats1("NewtonsCradleSelftest");
	stats1.setZMinStat(-.5);
	stats1.setZMaxStat(2*problem.N);
	stats1.setXMinStat(0.5);
	stats1.setXMaxStat(1.55);
	stats1.setYMinStat(0.5);
	stats1.setYMaxStat(1.55);
    std::cout << stats1.getName() + "_" + str[i] + "T_XYZ.stat" << std::endl;
	stats1.getStatFile().setName(stats1.getName() + "_" + str[i] + "T_XYZ.stat");
	stats1.set_h(0.0005);
	stats1.set_w(w);
	stats1.set_superexact(false);
	stats1.set_CG_type(str[i].c_str());
	stats1.set_tminStat(problem.getTimeMax()*1.00000999999);
	stats1.setTimeMaxStat(1e20);
	stats1.statistics_from_fstat_and_data();
	
	for (int i=0; i<3; i++)	{
		double w=0.125;
		if (i==1) w*=2;
		if (i==2) w*=2*sqrt(3);
		
		StatisticsVector<Z> stats0("NewtonsCradleSelftest");
		stats0.setZMinStat(-0.5);
	    stats0.getStatFile().setName(stats0.getName() + "_" + str[i] + "T_XYZ.stat");
		stats0.set_h(0.02);
		stats0.set_w(w);
		stats0.set_superexact(true);
		stats0.set_CG_type(str[i].c_str());
		stats0.set_tminStat(problem.getTimeMax()*.999999);
		stats0.setTimeMaxStat(1e20);
		stats0.statistics_from_fstat_and_data();

		StatisticsVector<XYZ> stats2("NewtonsCradleSelftest");
		stats2.setZMinStat(-0.5);
	    stats2.getStatFile().setName(stats2.getName() + "_" + str[i] + "T_XYZ.stat");
		stats2.set_h(0.02);
		stats2.set_ny(1);
		stats2.set_w(w);
		stats2.set_superexact(true);
		stats2.set_CG_type(str[i].c_str());
		stats2.set_tminStat(problem.getTimeMax()*.999999);
		stats2.setTimeMaxStat(1e20);
		stats2.statistics_from_fstat_and_data();
		
		/* Current the 2D statistics is not implement this tets will be added when it is.
		StatisticsVector<XZ> stats1("NewtonsCradleSelftest");
		stats1.setZMinStat(-0.5);
   	        stats1.getStatFile().setName(str[i] + "T_XZ.stat");
		stats1.set_h(0.02);
		stats1.set_w(w);
		stats1.set_superexact(true);
		stats1.set_CG_type(str[i].c_str());
		stats1.set_tminStat(problem.getTimeMax()*.999999);
		stats1.setTimeMaxStat(1e20);
		stats1.statistics_from_fstat_and_data();
		*/
	    // should give you Density 5
	}

	return(0);
}

