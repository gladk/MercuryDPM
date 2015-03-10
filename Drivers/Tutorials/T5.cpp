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

// Tutorial 5

//! [T5:headers]
#include <Species/LinearViscoelasticSpecies.h>
#include "Mercury3D.h"
#include "Particles/BaseParticle.h"
//! [T5:headers]

//! [T5:class]
class Tutorial5 : public Mercury3D
{
public:

void setupInitialConditions()
{	
BaseParticle p0;

p0.setRadius(0.005); //particle-1 radius
p0.setPosition(Vec3D(0.25*getXMax(),0.5*getYMax(),0.5*getZMax()));
p0.setVelocity(Vec3D(0.25,0.0,0.0));
particleHandler.copyAndAddObject(p0); // 1st particle created

p0.setRadius(0.005); // particle-2 radius
p0.setPosition(Vec3D(0.75*getXMax(),0.5*getYMax(),0.5*getZMax()));
p0.setVelocity(Vec3D(-0.25,0.0,0.0));
particleHandler.copyAndAddObject(p0); // 2nd particle created
}

};
//! [T5:class]

//! [T5:main]
int main(int argc, char *argv[])
{

// Problem setup
Tutorial5 problem;
//

//        
problem.setName("Tutorial5");
problem.setSystemDimensions(3);
problem.setGravity(Vec3D(0.0,0.0,0.0));
problem.setXMax(0.5);
problem.setYMax(0.25);
problem.setZMax(0.5);
problem.setTimeMax(2.0);
//

//! [T5:speciesProp]
// The normal spring stiffness and normal dissipation is computed and set as 
// For collision time tc=0.005 and restitution coefficeint rc=1.0, 
auto species = problem.speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());
species->setDensity(2500.0); // sets the species type-0 density
species->setStiffness(258.5);// sets the spring stiffness
species->setDissipation(0.0);// sets the dissipation
//! [T5:speciesProp]

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
//! [T5:main]
