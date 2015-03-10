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
#include "Species/LinearViscoelasticSpecies.h"
#include <cmath>
#include <iostream>
#include <iomanip>


///this tests if moving the wall works with CWall::move(Vec3D velocity,Vec3D dt). 
///The wall is moved in normal and tangential direction and the results 
///are compared with a system where the particles are moved instead of 
///the walls, with the same relative velocities
class WallSpecies : public DPMBase {
public:

	void setupInitialConditions()
	{
		setXMax(20);
		setYMax(20);
		setZMax(20);
        setSystemDimensions(3);
        setParticleDimensions(3);
        setGravity(Vec3D(0,0,0));
        setTimeStep(.0002);
		setTimeMax(15);
        setSaveCount(2587);

        //I don't know how to write this in a neat way.
        auto species0 = speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());
        auto species1 = speciesHandler.copyAndAddObject(species0);
        auto species01 = speciesHandler.getMixedObject(species0, species1);
        species0->setDensity(6.0/constants::pi);
        species0->setCollisionTimeAndRestitutionCoefficient(0.01,0.25,2);
        species01->setCollisionTimeAndRestitutionCoefficient(0.01,0.5,2);

        write(std::cout,false);

		//set particles
		BaseParticle P0;
		P0.setRadius(0.5);
		P0.setPosition(Vec3D(5,10,10));
		P0.setVelocity(Vec3D(-1,0,-1));
		particleHandler.copyAndAddObject(P0);
        
		//set walls
		InfiniteWall w0;
		w0.set(Vec3D(0, 0, -1), getZMin());
		wallHandler.copyAndAddObject(w0);
        
        w0.set(Vec3D(-1,0,0),getXMin());
        w0.setIndSpecies(1);
        wallHandler.copyAndAddObject(w0);
        
        std::cout << "X" << std::endl;
	}

    //Write the speed of the particles as the info line.
    double getInfo(const BaseParticle& P0) const
    {
        return sqrt(mathsFunc::square(P0.getVelocity().X)+mathsFunc::square(P0.getVelocity().Z));
    }
};



int main(int argc UNUSED, char *argv[] UNUSED)
{
	std::cout << std::endl << "Simulation of one particle, d=0.1, interacting with walls with different coefficeient of resitution" << std::endl;
	std::cout << std::endl << "The purpose of the test is to check walls species information is correctly picked up" << std::endl;
	WallSpecies problem;
	problem.setName("wallSpecies");
	problem.solve();
	std::cout << "v_z(tmax) = " << std::setprecision(14) << problem.particleHandler.getObject(0)->getVelocity().Z << " (should be 0.25 give or take)" << std::endl;
    std::cout << "v_x(tmax) = " << std::setprecision(14) << problem.particleHandler.getObject(0)->getVelocity().X << " (should be 0.5 give or take)" << std::endl;
}
