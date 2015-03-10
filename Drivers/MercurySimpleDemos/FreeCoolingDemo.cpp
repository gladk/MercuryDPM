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

#include "Mercury2D.h"
#include "Particles/BaseParticle.h"
#include "Walls/InfiniteWall.h"

/// In this file 32^2 particles with the same velocity are placed in a bi-axial box. This makes them collide with the

class FreeCoolingDemoProblem : public Mercury2D{
public:
    
    void setupInitialConditions()
    {
        auto species = speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());
        species->setDensity(2000);
        species->setDissipation(0.005);
        species->setStiffness(1e3);

        int N1=static_cast<int>(sqrt(N))+1;
        BaseParticle p0;
        for (int i=0;i<N;i++)
        {
            int ix=static_cast<int>(i%N1);
            int iy=static_cast<int>(i/N1);
            
            double x=(getXMax()-getXMin())*(ix+1)/(N1+1);
            double y=(getYMax()-getYMin())*(iy+1)/(N1+1);
            
            
            p0.setPosition(Vec3D(x,y,0.0));
            p0.setVelocity(Vec3D(0.1,0.1,0.0));
            p0.setRadius(0.0001);
            p0.setSpecies(species);
            particleHandler.copyAndAddObject(p0);
        }
        
        wallHandler.clear();
        InfiniteWall w0;
        w0.set(Vec3D(-1, 0, 0), Vec3D(getXMin(), 0, 0));
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D( 1, 0, 0), Vec3D(getXMax(), 0, 0));
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D( 0,-1, 0), Vec3D(0, getYMin(), 0));
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D( 0, 1, 0), Vec3D(0, getYMax(), 0));
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

 	///Now do the same problem with the HGrid turned on, simple done by create a new problem from the old problem.
	FreeCoolingDemoProblem problem;
	problem.setName("FreeCoolingDemo");

    problem.N=1024;
    problem.setGravity(Vec3D(0.0,0.0,0.0));
    problem.setTimeStep(5e-5);
    problem.setSaveCount(10);
    problem.setTimeMax(0.05);

    problem.setHGridMaxLevels(1);
	problem.setHGridCellOverSizeRatio(1.2);
	problem.setHGridUpdateEachTimeStep(false);
	problem.solve();
}
