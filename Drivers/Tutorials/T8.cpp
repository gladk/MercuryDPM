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

// Tutorial 8

//! [T8:headers]
#include <Species/LinearViscoelasticSpecies.h>
#include "Mercury3D.h"
#include "Particles/BaseParticle.h"
#include "Walls/InfiniteWall.h"
#include "Walls/IntersectionOfWalls.h"
//! [T8:headers]

//! [T8:class]
class Tutorial8 : public Mercury3D
{
public:

void setupInitialConditions()
{	
BaseParticle p0;
p0.setRadius(0.005);
p0.setPosition(Vec3D(0.15*getXMax(),0.3335*getYMax(),0.0));
p0.setVelocity(Vec3D(1.2,0.2,0.0));
particleHandler.copyAndAddObject(p0);

//! [T8:infiniteWalls]
InfiniteWall w0;

w0.set(Vec3D(1.0,0.0,0.0),getXMax());
wallHandler.copyAndAddObject(w0);

w0.set(Vec3D(-1.0,0.0,0.0),-getXMin());
wallHandler.copyAndAddObject(w0);

w0.set(Vec3D(0.0,1.0,0.0),getYMax());
wallHandler.copyAndAddObject(w0);

w0.set(Vec3D(0.0,-1.0,0.0),-getYMin());
wallHandler.copyAndAddObject(w0);
//! [T8:infiniteWalls]

//! [T8:finiteWall]
IntersectionOfWalls w1;
w1.addObject(Vec3D(-1.0,0.0,0.0),Vec3D(0.75*getXMax(),0.0,0.0));
w1.addObject(Vec3D(1.0,0.0,0.0),Vec3D(0.25*getXMax(),0.0,0.0));
w1.addObject(Vec3D(0.0,-1.0,0.0),Vec3D(0.0,0.75*getYMax(),0.0));
w1.addObject(Vec3D(0.0,1.0,0.0),Vec3D(0.0,0.25*getYMax(),0.0));
wallHandler.copyAndAddObject(w1);
//! [T8:finiteWall]
}

};
//! [T8:class]

int main(int argc, char *argv[])
{

// Problem setup
Tutorial8 problem; // instantiate an object of class Tutorial 8
//

//
problem.setName("Tutorial8");
problem.setSystemDimensions(2);
problem.setGravity(Vec3D(0.0,0.0,0.0));
problem.setXMax(0.5);
problem.setYMax(0.5);
problem.setTimeMax(5.0);
//

//! [T8:speciesProp]
// The normal spring stiffness and normal dissipation is computed and set as 
// For collision time tc=0.005 and restitution coefficeint rc=1.0, 
auto species = problem.speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());
species->setDensity(2500.0); // sets the species type-0 density
species->setStiffness(258.5);// sets the spring stiffness
species->setDissipation(0.0);// sets the dissipation
//! [T8:speciesProp]

//
problem.setSaveCount(10);
problem.getDataFile().setFileType(FileType::ONE_FILE);
problem.getRestartFile().setFileType(FileType::ONE_FILE);
problem.getFStatFile().setFileType(FileType::NO_FILE);
problem.getEneFile().setFileType(FileType::NO_FILE);
//

//
problem.setXBallsAdditionalArguments("-solidf -v0 -s .85");	
//

//
problem.setTimeStep(.005/50.0); // (collision time)/50.0
problem.solve(argc, argv);
//
}
