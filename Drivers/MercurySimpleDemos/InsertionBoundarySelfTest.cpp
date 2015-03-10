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

#include <iostream>
#include "Mercury3D.h"
#include "Boundaries/CubeInsertionBoundary.h"
#include "Particles/BaseParticle.h"
#include "Species/LinearViscoelasticSpecies.h"
#include "Walls/InfiniteWall.h"


class InsertionBoundarySelfTest : public Mercury3D
{
public:
    
    
    void setupInitialConditions()
    {
        setName("InsertionBoundarySelfTest");
        setSystemDimensions(3);
        setGravity(Vec3D(0.0,0.0,0.0));
        setTimeStep(1e-4);
        getDataFile().setSaveCount(500);
        setTimeMax(1);
        setHGridMaxLevels(2);

        setXMin(0.0);
        setYMin(0.0);
        setZMin(0.0);
        setXMax(1.0);
        setYMax(1.0);
        setZMax(1.0);

        
        species = new LinearViscoelasticSpecies; //delete is done in speciesHandler
        speciesHandler.addObject(species);
        species->setDensity(2000);
        species->setStiffness(10000);
                
        insertionBoundary = new CubeInsertionBoundary; //delete is done in boundaryHandler
        boundaryHandler.addObject(insertionBoundary);
        BaseParticle* insertionBoundaryParticle = new BaseParticle;
        insertionBoundaryParticle->setSpecies(species);
        insertionBoundary->set(insertionBoundaryParticle,10,Vec3D(0.25,0.25,0.25),Vec3D(0.75,0.75,0.75),Vec3D(-5,-5,-5),Vec3D(5,5,5),0.1,0.2);        
        delete insertionBoundaryParticle;
                
        bottomWall = new InfiniteWall(); //delete is done in wallHandler
        wallHandler.addObject(bottomWall);
        Vec3D normal = Vec3D(std::pow(1.0/3.0,0.5), std::pow(1.0/3.0,0.5), std::pow(1.0/3.0,0.5));
        bottomWall->set(normal, normal);
    }
    
    LinearViscoelasticSpecies* species;
    CubeInsertionBoundary* insertionBoundary;
    InfiniteWall* bottomWall;

};

int main(int argc UNUSED, char *argv[] UNUSED)
{
    std::cout << "Simple box for creating particles" << std::endl;
    ///Start off my solving the default problem
    InsertionBoundarySelfTest insertionBoundary_problem;

    insertionBoundary_problem.solve();
}
