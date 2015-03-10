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

///todo{This code is not working as is wanted}

#include<iostream>
#include <Species/LinearViscoelasticSpecies.h>

#include "DPMBase.h"
#include "Particles/BaseParticle.h"
#include "Walls/InfiniteWall.h"
#include "Boundaries/PeriodicBoundary.h"

class my_problem : public DPMBase
{
public:

    void setupInitialConditions()
    {
        auto species = speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());

        BaseParticle p0;
        particleHandler.copyAndAddObject(p0);
        
        InfiniteWall w0;
        wallHandler.copyAndAddObject(w0);
        
        PeriodicBoundary b0;
        boundaryHandler.copyAndAddObject(b0);
    }
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
    {
        std::cout<<"Creating base problem"<<std::endl;
        my_problem problem;
        problem.setName("ParticleHandlerDestructorTest");
        std::cout<<"Finished creating base problem"<<std::endl<<std::endl;
        
        std::cout<<"Adding a BaseParticle, InfiniteWall and PeriodicBoundary to the base problem"<<std::endl;
        problem.setupInitialConditions();
        std::cout<<"Finished adding a BaseParticle, InfiniteWall and PeriodicBoundary to the base problem"<<std::endl<<std::endl;
        
        std::cout<<"Copying base problem"<<std::endl;
        //my_problem problem2(problem);
        std::cout<<"Finished copying base problem"<<std::endl<<std::endl;
        
        std::cout<<"Starting to destruct everything"<<std::endl;
    }
    std::cout << "Ready" << std::endl;
}
