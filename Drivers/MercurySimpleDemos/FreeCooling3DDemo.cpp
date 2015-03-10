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

///todo{This code is not working as is wanted}
#include<iostream>
#include <Species/LinearViscoelasticSpecies.h>

#include "Mercury3D.h"
#include "Particles/BaseParticle.h"
#include "Walls/InfiniteWall.h"

/// In this file 10^3 particles with the same velocity are placed in a tri-axial box. This makes them collide with the walls and eachother. Afterwards the same run is performed with hgrid on. It tests the working (and speedup) of the hgrid.

class FreeCooling3DDemoProblem : public Mercury3D{
public:
	
	void setupInitialConditions()
	{
		int N1=static_cast<int>(pow(N,0.33))+1;	
		BaseParticle p0;		
		for (int i=0;i<N;i++)
		{
			int ix=static_cast<int>(i%N1);
			int iz=static_cast<int>(i/N1/N1);
			int iy=(i-ix-N1*N1*iz)/N1;
		
			double x=(getXMax()-getXMin())*(ix+1)/(N1+1);
			double y=(getYMax()-getYMin())*(iy+1)/(N1+1);
			double z=(getZMax()-getZMin())*(iz+1)/(N1+1);
			
		
			p0.setPosition(Vec3D(x,y,z));
			p0.setVelocity(Vec3D(0.1,0.1,0.1));
			p0.setRadius(0.0001);
			particleHandler.copyAndAddObject(p0);
		}
		
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
		w0.set(Vec3D( 0, 0,-1), -getZMin());
		wallHandler.copyAndAddObject(w0);
		w0.set(Vec3D( 0, 0, 1),  getZMax());
		wallHandler.copyAndAddObject(w0);
	}
	
	int N;
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
	std::cout<<"In this file 10^3 particles with the same velocity are placed "
	"in a tri-axial box. This makes them collide with the walls and eachother. "
	"Afterwards the same run is performed with hgrid on. It tests the working "
	"(and speedup) of the hgrid."<<std::endl;
	
	///Start off my solving the default problem
 	FreeCooling3DDemoProblem problem;
    auto species = new LinearViscoelasticSpecies;
    problem.speciesHandler.addObject(species);
    species->setDensity(2000);
    species->setDissipation(0.005);
    species->setStiffness(1e3);

    problem.N=1000;
 	problem.setName("FreeCooling3DDemo");
	problem.setGravity(Vec3D(0.0,0.0,0.0));
    problem.setTimeStep(5e-5);
	problem.setSaveCount(4);
 	problem.setTimeMax(0.08);
    problem.setSystemDimensions(3);
	problem.setZMax(0.01);
    
        problem.setHGridMaxLevels(1);
	problem.setHGridCellOverSizeRatio(1.2);
	problem.setHGridUpdateEachTimeStep(false);    
	problem.solve();
}

