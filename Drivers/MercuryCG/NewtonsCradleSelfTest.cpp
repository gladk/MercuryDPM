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

#include "Mercury3D.h"
#include "StatisticsVector.h"
#include "Particles/BaseParticle.h"
#include "Walls/InfiniteWall.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <Species/LinearViscoelasticSpecies.h>


class NewtonsCradleSelfTest : public DPMBase {
	
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
	NewtonsCradleSelfTest problem;
	problem.setName("NewtonsCradleSelfTest");
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
	StatisticsVector<Z> stats1("NewtonsCradleSelfTest");
	stats1.setZMinStat(-.5);
	stats1.setZMaxStat(2*problem.N);
	stats1.setXMinStat(0.5);
	stats1.setXMaxStat(1.55);
	stats1.setYMinStat(0.5);
	stats1.setYMaxStat(1.55);
    std::cout << stats1.getName() + "_" + str[i] + "T_XYZ.stat" << std::endl;
	stats1.getStatFile().setName(stats1.getName() + "_" + str[i] + "T_XYZ.stat");
	stats1.set_h(0.0005);
	stats1.setCGWidth(w);
	stats1.setSuperExact(false);
	stats1.setCGShape(str[i].c_str());
	stats1.setCGTimeMin(problem.getTimeMax()*1.00000999999);
	stats1.setTimeMaxStat(1e20);
	stats1.statistics_from_fstat_and_data();
	
	for (int i=0; i<3; i++)	{
		double w=0.125;
		if (i==1) w*=2;
		if (i==2) w*=2*sqrt(3);
		
		StatisticsVector<Z> stats0("NewtonsCradleSelfTest");
		stats0.setZMinStat(-0.5);
	    stats0.getStatFile().setName(stats0.getName() + "_" + str[i] + "T_XYZ.stat");
		stats0.set_h(0.02);
		stats0.setCGWidth(w);
		stats0.setSuperExact(true);
		stats0.setCGShape(str[i].c_str());
		stats0.setCGTimeMin(problem.getTimeMax()*.999999);
		stats0.setTimeMaxStat(1e20);
		stats0.statistics_from_fstat_and_data();

		StatisticsVector<XYZ> stats2("NewtonsCradleSelfTest");
		stats2.setZMinStat(-0.5);
	    stats2.getStatFile().setName(stats2.getName() + "_" + str[i] + "T_XYZ.stat");
		stats2.set_h(0.02);
		stats2.setNY(1);
		stats2.setCGWidth(w);
		stats2.setSuperExact(true);
		stats2.setCGShape(str[i].c_str());
		stats2.setCGTimeMin(problem.getTimeMax()*.999999);
		stats2.setTimeMaxStat(1e20);
		stats2.statistics_from_fstat_and_data();
		
		/* Current the 2D statistics is not implement this tets will be added when it is.
		StatisticsVector<XZ> stats1("NewtonsCradleSelfTest");
		stats1.setZMinStat(-0.5);
   	        stats1.getStatFile().setName(str[i] + "T_XZ.stat");
		stats1.set_h(0.02);
		stats1.setCGWidth(w);
		stats1.setSuperExact(true);
		stats1.setCGShape(str[i].c_str());
		stats1.setCGTimeMin(problem.getTimeMax()*.999999);
		stats1.setTimeMaxStat(1e20);
		stats1.statistics_from_fstat_and_data();
		*/
	    // should give you Density 5
	}

	return(0);
}

