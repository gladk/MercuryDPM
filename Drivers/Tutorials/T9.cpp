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

// Tutorial 9

//! [T9:headers]
#include <Species/LinearViscoelasticFrictionSpecies.h>
#include "Mercury3D.h"
#include "Particles/BaseParticle.h"
#include "Walls/InfiniteWall.h"
//! [T9:headers]

//! [T9:class]
class Tutorial9 : public Mercury3D
{
public:

void setupInitialConditions()
{	
BaseParticle p0;

p0.setIndSpecies(0); // sets the particle to be as species type-1
p0.setRadius(0.005);
p0.setPosition(Vec3D(0.05*getXMax(),0.01*getYMax(),getZMin()+p0.getRadius()));
p0.setVelocity(Vec3D(0.0,0.0,0.0));
particleHandler.copyAndAddObject(p0);

p0.setIndSpecies(1); // sets the particle to species type-2
p0.setRadius(0.005);
p0.setPosition(Vec3D(0.05*getXMax(),0.21*getYMax(),getZMin()+p0.getRadius()));
p0.setVelocity(Vec3D(0.0,0.0,0.0));
particleHandler.copyAndAddObject(p0);

p0.setIndSpecies(2); // sets the particle to species type-3
p0.setRadius(0.005);
p0.setPosition(Vec3D(0.05*getXMax(),0.41*getYMax(),getZMin()+p0.getRadius()));
p0.setVelocity(Vec3D(0.0,0.0,0.0));
particleHandler.copyAndAddObject(p0);


//! [T8:infiniteWalls]
InfiniteWall w0;

w0.set(Vec3D(0.0,0.0,-1.0),Vec3D(0.0,0.0,getZMin()));
wallHandler.copyAndAddObject(w0);
//! [T8:infiniteWalls]

}

};
//! [T8:class]

int main(int argc, char *argv[])
{

// Problem setup
Tutorial9 problem; // instantiate an object of class Tutorial 9
//

double angle = constants::pi/180.0*20.0;
//
problem.setName("Tutorial9");
problem.setSystemDimensions(3);
problem.setGravity(Vec3D(sin(angle),0.0,-cos(angle))*9.81);
problem.setXMax(0.3);
problem.setYMax(0.3);
problem.setZMax(0.05);
problem.setTimeMax(0.5);
//

//
auto species0 = problem.speciesHandler.copyAndAddObject(LinearViscoelasticFrictionSpecies());
auto species1 = problem.speciesHandler.copyAndAddObject(LinearViscoelasticFrictionSpecies());
auto species2 = problem.speciesHandler.copyAndAddObject(LinearViscoelasticFrictionSpecies());
auto species01 = problem.speciesHandler.getMixedObject(species0, species1);
auto species02 = problem.speciesHandler.getMixedObject(species0, species2);
//

// The normal spring stiffness and normal dissipation is computed and set as 
// For collision time tc=0.005 and restitution coefficeint rc=0.88, 

species0->setDensity(2500.0); // sets the species type-0 density
species0->setStiffness(259.018);// sets the spring stiffness
species0->setSlidingStiffness(2.0/7.0*species0->getStiffness());
species0->setRollingStiffness(2.0/5.0*species0->getStiffness());
species0->setDissipation(0.0334);// sets the dissipation
species0->setSlidingFrictionCoefficient(0.0);
species0->setRollingFrictionCoefficient(0.0);

//
species1->setDensity(2500.0); // sets the species type-0 density
species1->setStiffness(259.018);// sets the spring stiffness
species1->setDissipation(0.0334);// sets the dissipation
species1->setSlidingStiffness(2.0/7.0*species1->getStiffness());
species1->setRollingStiffness(2.0/5.0*species1->getStiffness());
species1->setSlidingFrictionCoefficient(0.5);
species1->setRollingFrictionCoefficient(0.0);

species01->setStiffness(259.018); 
species01->setDissipation(0.0334);// sets the dissipation
species01->setSlidingStiffness(2.0/7.0*species01->getStiffness());
species01->setRollingStiffness(2.0/5.0*species01->getStiffness());
species01->setSlidingFrictionCoefficient(0.5);
species01->setRollingFrictionCoefficient(0.0);

species2->setDensity(2500.0); // sets the species type-0 density
species2->setStiffness(258.5);// sets the spring stiffness
species2->setDissipation(0.0);// sets the dissipation
species2->setSlidingStiffness(2.0/7.0*species2->getStiffness());
species2->setRollingStiffness(2.0/5.0*species2->getStiffness());
species2->setSlidingFrictionCoefficient(0.5);
species2->setRollingFrictionCoefficient(0.5);

species02->setStiffness(259.018); 
species02->setDissipation(0.0334);// sets the dissipation
species02->setSlidingStiffness(2.0/7.0*species02->getStiffness());
species02->setRollingStiffness(2.0/5.0*species02->getStiffness());
species02->setSlidingFrictionCoefficient(0.5);
species02->setRollingFrictionCoefficient(0.5);
//

//
problem.setSaveCount(10);
problem.getDataFile().setFileType(FileType::ONE_FILE);
problem.getRestartFile().setFileType(FileType::ONE_FILE);
problem.getFStatFile().setFileType(FileType::ONE_FILE);
problem.getEneFile().setFileType(FileType::NO_FILE);
//

//
problem.setXBallsAdditionalArguments("-solidf -v0 -s 8");	
//

//
problem.setTimeStep(0.005/50.0);
problem.solve(argc, argv);
//
}
