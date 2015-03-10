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

// Tutorial 3

//! [T3:headers]
#include <Species/LinearViscoelasticSpecies.h>
#include "Mercury3D.h"
#include "Particles/BaseParticle.h"
#include "Walls/InfiniteWall.h"
//! [T3:headers]

//! [T3:class]
class Tutorial3 : public Mercury3D
{
public:

void setupInitialConditions()
{	
BaseParticle p0;
p0.setRadius(0.005);
p0.setPosition(Vec3D(0.5*getXMax(),0.5*getYMax(),getZMax()));
p0.setVelocity(Vec3D(0.0,0.0,0.0));
particleHandler.copyAndAddObject(p0);

//! [T3:infiniteWall]
InfiniteWall w0;
w0.set(Vec3D(0.0,0.0,-1.0),-getZMin());
wallHandler.copyAndAddObject(w0);
//! [T3:infiniteWall]
}

};
//! [T3:class]

int main(int argc, char *argv[])
{

// Problem setup
Tutorial3 problem;
//

//
problem.setName("Tutorial3");
problem.setSystemDimensions(3);
problem.setGravity(Vec3D(0.0,0.0,-9.81));
problem.setXMax(1.0);
problem.setYMax(1.0);
problem.setZMax(2.0);
problem.setTimeMax(2.0);
//

//! [T3:speciesProp]
// The normal spring stiffness and normal dissipation is computed and set as 
// For collision time tc=0.005 and restitution coefficeint rc=1.0, 
auto species = problem.speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());
species->setDensity(2500.0); // sets the species type-0 density
species->setStiffness(258.5);// sets the spring stiffness
species->setDissipation(0.0);// sets the dissipation
//! [T3:speciesProp]

//
problem.setSaveCount(10);
problem.getDataFile().setFileType(FileType::ONE_FILE);
problem.getRestartFile().setFileType(FileType::ONE_FILE);
problem.getFStatFile().setFileType(FileType::NO_FILE);
problem.getEneFile().setFileType(FileType::NO_FILE);
//

//
problem.setXBallsAdditionalArguments("-solidf -v0");	
//

//
problem.setTimeStep(0.005/50.0); // (collision time)/50.0
problem.solve(argc, argv);
//
}
