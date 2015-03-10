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
#include <iostream>
#include "Mercury2D.h"
#include "Particles/BaseParticle.h"
#include "Species/LinearViscoelasticSlidingFrictionLinearReversibleAdhesiveSpecies.h"
#include "Walls/InfiniteWall.h"

/// In this file 32^2 particles with the same velocity are placed in a
/// bi-axial box. This makes them collide with the walls and each other.
/// Afterwards the same run is performed with hgrid on.
/// It tests the working (and speedup) of the hgrid.

class my_problem_HGRID : public Mercury2D{
public:
    
	void setupInitialConditions()
	{
		int N1=static_cast<int>(ceil(sqrt(N)));
		BaseParticle p0;
        p0.setRadius(0.0001);
		for (int i=0;i<N;i++)
		{
            int ix=static_cast<int>(i%N1);
			int iy=static_cast<int>(i/N1);
            
			double x=(getXMax()-getXMin())*(ix+1)/(N1+1);
			double y=(getYMax()-getYMin())*(iy+1)/(N1+1);
			
            
			p0.setPosition(Vec3D(x,y,0.0));
			Vec3D vel;
            vel.X=random.getRandomNumber(-1,1);
            vel.Y=random.getRandomNumber(-1,1);
            p0.setVelocity(1.0*vel);
			particleHandler.copyAndAddObject(p0);
            p0.setRadius(0.0001*random.getRandomNumber(0.7,1));
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
	}
    
	int N;
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
	std::cout<<"In this file 32^2 particles with the same velocity are placed "
	"in a bi-axial box. This makes them collide with the walls and eachother. "
	"Afterwards the same run is performed with hgrid on. It tests the working "
	"(and speedup) of the hgrid."<<std::endl;
	
	///Start off my solving the default problem
 	my_problem_HGRID problem;
    auto species = problem.speciesHandler.copyAndAddObject(LinearViscoelasticSlidingFrictionLinearReversibleAdhesiveSpecies());
    species->setDensity(2000);
    species->setDissipation(0.01);
    species->setStiffness(1e4);
    species->setSlidingFrictionCoefficient(0.1);
    species->setDensity(2000);
    species->setAdhesionStiffness(0.05* species->getStiffness());
    species->setAdhesionForceMax(1.1e-4*0.05* species->getStiffness());

 	problem.N=25;
 	problem.setName("free_cooling_adhesive");
    problem.setXMax(0.0018);
    problem.setYMax(0.0018);
 	problem.setGravity(Vec3D(0.0,0.0,0.0));
    problem.setTimeStep(3e-6);
	problem.setSaveCount(50);
 	problem.setTimeMax(0.05);
    problem.setHGridMaxLevels(1);
	//problem.setHGridCellOverSizeRatio(1.2);
	problem.setHGridUpdateEachTimeStep(true);

    problem.getDataFile().setFileType(FileType::ONE_FILE);
    problem.getFStatFile().setFileType(FileType::NO_FILE);
    problem.solve();
    std::cout << problem.particleHandler.getObject(0)->getRadius() << std::endl;
    std::cout << problem.particleHandler.getObject(0)->getInteractionRadius() << std::endl;
    return 0;
}
