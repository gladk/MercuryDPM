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
        speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());

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
