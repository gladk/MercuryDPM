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

// Tutorial 1

//! [T1:headers]
#include <Species/Species.h>
#include <Species/LinearViscoelasticSpecies.h>
#include "Mercury3D.h"
#include "Particles/BaseParticle.h"
//! [T1:headers]

//! [T1:class]
class Tutorial1 : public Mercury3D
{
public:

void setupInitialConditions()
{	
//! [T1:createParticle]
BaseParticle p0;
p0.setRadius(0.005); // sets particle radius
p0.setPosition(Vec3D(0.1*getXMax(),0.1*getYMax(),0.1*getZMax())); // sets particle position
p0.setVelocity(Vec3D(0.1,0.1,0.1));
particleHandler.copyAndAddObject(p0);
//! [T1:createParticle]
}

};
//! [T1:class]

//! [T1:main]
int main(int argc, char *argv[])
{
// Problem setup
Tutorial1 problem;

//! [T1:problemSetup]
problem.setName("Tutorial1");
problem.setSystemDimensions(3);
problem.setGravity(Vec3D(0.0,0.0,0.0));
problem.setXMax(1.0);
problem.setYMax(1.0);
problem.setZMax(1.0);
problem.setTimeMax(1.0);
//! [T1:problemSetup]

//! [T1:speciesProp]
// The normal spring stiffness and normal dissipation is computed and set as 
// For collision time tc=0.005 and restitution coefficeint rc=1.0,
auto species = problem.speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());
species->setDensity(2500.0); // sets the species type-0 density
species->setStiffness(258.5);// sets the spring stiffness
species->setDissipation(0.0);// sets the dissipation
//! [T1:speciesProp]

//! [T1:output]
problem.setSaveCount(10);
problem.getDataFile().setFileType(FileType::ONE_FILE);
problem.getRestartFile().setFileType(FileType::ONE_FILE);
problem.getFStatFile().setFileType(FileType::NO_FILE);
problem.getEneFile().setFileType(FileType::NO_FILE);
//! [T1:output]

//! [T1:visualOutput] 	
problem.setXBallsAdditionalArguments("-solidf -v0");
//! [T1:visualOutput]

//! [T1:solve]
problem.setTimeStep(.005/50.0); // (collision time)/50.0
problem.solve(argc, argv);
//! [T1:solve]
}
//! [T1:main]
