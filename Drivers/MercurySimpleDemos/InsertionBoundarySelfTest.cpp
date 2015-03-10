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

        species = speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());
        species->setDensity(2000);
        species->setStiffness(10000);
        
        insertionBoundary = boundaryHandler.copyAndAddObject(CubeInsertionBoundary());
        BaseParticle* insertionBoundaryParticle =new BaseParticle;
        insertionBoundaryParticle->setSpecies(species);
        insertionBoundary->set(insertionBoundaryParticle,10,Vec3D(0.25,0.25,0.25),Vec3D(0.75,0.75,0.75),Vec3D(-5,-5,-5),Vec3D(5,5,5),0.1,0.2);
        
        bottomWall= wallHandler.copyAndAddObject(InfiniteWall());
        bottomWall->set(Vec3D(std::pow(1.0/3.0,0.5), std::pow(1.0/3.0,0.5), std::pow(1.0/3.0,0.5)), 1);
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
