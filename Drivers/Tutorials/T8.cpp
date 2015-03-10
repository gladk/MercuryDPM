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
