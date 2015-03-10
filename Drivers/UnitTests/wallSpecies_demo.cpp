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
        std::cout << "X" << std::endl;
        LinearViscoelasticSpecies* species0 = speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());
        std::cout << "X" << std::endl;
        LinearViscoelasticSpecies* species1 = speciesHandler.copyAndAddObject(species0);
        std::cout << "X" << std::endl;
        LinearViscoelasticMixedSpecies* species01 = speciesHandler.getMixedObject(species0, species1);
        std::cout << "X" << std::endl;
        species0->setDensity(6.0/constants::pi);
        std::cout << "Y" << std::endl;
        species0->setCollisionTimeAndRestitutionCoefficient(0.01,0.25,2);
        std::cout << "Z" << std::endl;
        write(std::cout,false);
        species01->setCollisionTimeAndRestitutionCoefficient(0.01,0.5,2);
        std::cout << "Y" << std::endl;


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
