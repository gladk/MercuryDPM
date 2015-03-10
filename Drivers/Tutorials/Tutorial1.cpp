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

// Tutorial 1

/*
** This file is annotated with DoxyFile comments in order to show the code on
** the documentation - This is not needed for your real drivers.
** Please ignore these comments.
*/

//! [T1:headers]
#include <Species/Species.h>
#include <Species/LinearViscoelasticSpecies.h>
#include <Mercury3D.h>
#include <Particles/BaseParticle.h>
//! [T1:headers]

//! [T1:class]
class Tutorial1 : public Mercury3D 	
{ 

  public:

  void setupInitialConditions()
  {	
//! [T1:createParticle]
    BaseParticle p0;
    p0.setRadius(0.05); // sets particle radius
    p0.setPosition(Vec3D(0.1*getXMax(),0.1*getYMax(),0.1*getZMax())); // sets particle position
    p0.setVelocity(Vec3D(0.5,0.1,0.1));
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
  problem.setTimeMax(2.0);
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
  problem.dataFile.setFileType(FileType::ONE_FILE);
  problem.restartFile.setFileType(FileType::ONE_FILE);
  problem.fStatFile.setFileType(FileType::NO_FILE);
  problem.eneFile.setFileType(FileType::NO_FILE);
  std::cout << problem.dataFile.getCounter() << std::endl;
//! [T1:output]

//! [T1:visualOutput] 	
  problem.setXBallsAdditionalArguments("-solidf -v0");
//! [T1:visualOutput]

//! [T1:solve]
  problem.setTimeStep(.005/50.0); // (collision time)/50.0
  problem.solve(argc, argv);
//! [T1:solve]
  return 0;
}
//! [T1:main]
