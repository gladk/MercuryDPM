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
