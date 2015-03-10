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

///todo{This code is not working as is wanted}
#include<iostream>
#include <Species/LinearViscoelasticSpecies.h>

#include "Mercury3D.h"
#include "Particles/BaseParticle.h"
#include "Walls/InfiniteWall.h"
#include "MercuryTime.h"

/*!
 * In this file 10^3 particles with the same velocity are placed in a tri-axial box. 
 * This makes them collide with the walls and eachother. 
 * Afterwards the same run is performed with hgrid on. 
 * It tests the working (and speedup) of the hgrid.
 */

class FreeCooling3DDemoProblem : public Mercury3D
{
public:
	
	void setupInitialConditions()
	{
		unsigned int N1 = static_cast<unsigned int>(pow(N, 0.33)) + 1;	
		BaseParticle p0;		
		for (unsigned int i = 0; i < N; i++)
		{
			unsigned int ix = static_cast<unsigned int>(i%N1);
			unsigned int iz = static_cast<unsigned int>(i/N1/N1);
			unsigned int iy = (i - ix - N1 * N1 * iz) / N1;
		
			double x = (getXMax() - getXMin()) * (ix + 1) / (N1 + 1);
			double y = (getYMax() - getYMin()) * (iy+1) / (N1 + 1);
			double z = (getZMax() - getZMin()) * (iz + 1) / (N1 + 1);
			
			p0.setPosition(Vec3D(x,y,z));
			p0.setVelocity(20*Vec3D(0.1,0.1,0.1));
			p0.setRadius(0.002);
			particleHandler.copyAndAddObject(p0);
		}
		
		wallHandler.clear();
		InfiniteWall w0;
		w0.set(Vec3D(-1.0, 0.0, 0.0), Vec3D(getXMin(), 0, 0));
		wallHandler.copyAndAddObject(w0);
		w0.set(Vec3D(1.0, 0.0, 0.0), Vec3D(getXMax(), 0, 0));
		wallHandler.copyAndAddObject(w0);
		w0.set(Vec3D(0.0,-1.0, 0.0), Vec3D(0, getYMin(), 0));
		wallHandler.copyAndAddObject(w0);
		w0.set(Vec3D(0.0, 1.0, 0.0), Vec3D(0, getYMax(), 0));
		wallHandler.copyAndAddObject(w0);
		w0.set(Vec3D(0.0, 0.0,-1.0), Vec3D(0, 0, getZMin()));
		wallHandler.copyAndAddObject(w0);
		w0.set(Vec3D(0.0, 0.0, 1.0), Vec3D(0, 0, getZMax()));
		wallHandler.copyAndAddObject(w0);
	}
	
    ///\brief Number of particles in the system.
	unsigned int N;
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
	std::cout << "In this file 10^3 particles with the same velocity are placed "
	"in a tri-axial box. This makes them collide with the walls and eachother. "
	"Afterwards the same run is performed with hgrid on. It tests the working "
	"(and speedup) of the hgrid." << std::endl;
	
    Time time;
    time.tic();
	///Start off my solving the default problem
 	FreeCooling3DDemoProblem problem;
    auto species = new LinearViscoelasticSpecies;
    problem.speciesHandler.addObject(species);
    species->setDensity(2000);
    species->setDissipation(0.005);
    species->setStiffness(1e3);

    problem.setXMax(0.1);
    problem.setYMax(0.1);
    problem.setZMax(0.1);
    problem.N=1000;
    problem.setName("FreeCooling3DDemo");
    problem.setGravity(Vec3D(0.0,0.0,0.0));
    problem.setTimeStep(5e-5);
    problem.setSaveCount(4);
    problem.setTimeMax(0.08);
    problem.setSystemDimensions(3);
    
    problem.setHGridMaxLevels(1);
    problem.setHGridCellOverSizeRatio(1.2);
    problem.setHGridUpdateEachTimeStep(false);    
    problem.solve();
    
    std::cout << "Total time to run this simulation: " << time.toc() << "s" << std::endl;
}
