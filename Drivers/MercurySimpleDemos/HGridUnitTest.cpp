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

#include <Species/LinearViscoelasticSpecies.h>
#include "Mercury3D.h"
#include "DPMBase.h"
#include "Boundaries/PeriodicBoundary.h"
#include "Particles/BaseParticle.h"

class MD_demo : public DPMBase
{
public:

    MD_demo()
    {
        species = new LinearViscoelasticSpecies;
        speciesHandler.addObject(species);
    }

    void setupInitialConditions()
    {
        if (particleHandler.getNumberOfObjects() != N)
        {
            particleHandler.clear();
            boundaryHandler.clear();
            
            double VP = constants::pi * 4.0 / 3.0;
            L = pow(N * VP * DistInt(1, omega) / nu, 1.0 / 3.0);
            
            setXMin(0);
            setYMin(0);
            setZMin(0);
            setXMax(L);
            setYMax(L);
            setZMax(L);
            
            PeriodicBoundary b0;
            b0.set(Vec3D(1, 0, 0), getXMin(), getXMax());
            boundaryHandler.copyAndAddObject(b0);
            b0.set(Vec3D(0, 1, 0), getYMin(), getYMax());
            boundaryHandler.copyAndAddObject(b0);
            b0.set(Vec3D(0, 0, 1), getZMin(), getZMax());
            boundaryHandler.copyAndAddObject(b0);
            
            particleHandler.setStorageCapacity(2 * N);
            BaseParticle p0;
            p0.setVelocity(Vec3D(0.0, 0.0, 0.0));
            
            double V = 0;
            
            //Use at least particles with maximum and minimum size
            p0.setRadius(1.0);
            Vec3D position;
            position.Z = random.getRandomNumber(0, getZMax());
            position.Y = random.getRandomNumber(0, getYMax());
            position.X = random.getRandomNumber(0, getXMax());
            p0.setPosition(position);
            particleHandler.copyAndAddObject(p0);
            V += VP * pow(p0.getRadius(), 3);
            
            p0.setRadius(omega);
            position.Z = random.getRandomNumber(0, getZMax());
            position.Y = random.getRandomNumber(0, getYMax());
            position.X = random.getRandomNumber(0, getXMax());
            p0.setPosition(position);
            particleHandler.copyAndAddObject(p0);
            V += VP * pow(p0.getRadius(), 3);

            //For other particles use a random distribution
            for (unsigned int i = 2; i < N; i++)
            {
                p0.setRadius(RandomRadius());
                position.Z = random.getRandomNumber(0, getZMax());
                position.Y = random.getRandomNumber(0, getYMax());
                position.X = random.getRandomNumber(0, getXMax());
                p0.setPosition(position);
                particleHandler.copyAndAddObject(p0);
                V += VP * pow(p0.getRadius(), 3);
            }
        }
    }
    
    double RandomRadius()
    {
        double rand = random.getRandomNumber(0, 1);
        if (alpha == -1)
        {
            return pow(omega, rand);
        }
        else
        {
            return pow(rand * (pow(omega, 1.0 + alpha) - 1.0) + 1.0, 1.0 / (1.0 + alpha));
        }
    }
    
    double DistInt(double s, double e)
    {
        if (omega == 1)
        {
            return 1;
        }
        double teller;
        double noemer;
        if (alpha == -1)
        {
            noemer = log(omega);
        }
        else
        {
            noemer = (pow(omega, 1.0 + alpha) - 1.0) / (1.0 + alpha);
        }
        
        if (alpha == -4)
        {
            teller = log(e) - log(s);
        }
        else
        {
            teller = (pow(e, 4.0 + alpha) - pow(s, 4.0 + alpha)) / (4.0 + alpha);
        }
        return teller / noemer;
    }
    
    double L;
    double omega;
    double alpha;
    double nu;
    unsigned int N;
    LinearViscoelasticSpecies* species;
};

class HGrid_demo : public Mercury3D
{
public:
    HGrid_demo(DPMBase& other)
            : DPMBase(other)
    {
    }
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
    MD_demo MD_problem;
    MD_problem.species->setDensity(2000);

    MD_problem.omega = 40;
    MD_problem.alpha = -2;
    MD_problem.nu = 0.7;
    MD_problem.N = 1000;
    MD_problem.setSystemDimensions(3);
    MD_problem.setParticleDimensions(3);
    MD_problem.setName("HGridUnitTest_MD");
    MD_problem.setTimeStep(0.1);
    MD_problem.setTimeMax(0.09);
    MD_problem.setSaveCount(1);
    MD_problem.setupInitialConditions();

    HGrid_demo HGrid_problem1(MD_problem);
    HGrid_problem1.setHGridMethod(TOPDOWN);
    HGrid_problem1.setHGridMaxLevels(3);
    HGrid_problem1.setHGridDistribution(EXPONENTIAL);
    HGrid_problem1.setName("HGridUnitTest_HGrid1");
    
    HGrid_demo HGrid_problem2(MD_problem);
    HGrid_problem2.setHGridMethod(BOTTOMUP);
    HGrid_problem2.setHGridMaxLevels(8);
    HGrid_problem2.setHGridDistribution(LINEAR);
    HGrid_problem2.setName("HGridUnitTest_HGrid2");

    std::cout << "Solving the MD problem" << std::endl;
    MD_problem.solve();
    std::cout << "Solving the first HGrid problem" << std::endl;
    HGrid_problem1.solve();
    std::cout << "Solving the second HGrid problem" << std::endl;
    HGrid_problem2.solve();
    
    // Check the particles are in the same place for all three problem i.e. no HGrid and two different HGrid settings 
    std::vector<BaseParticle*>::iterator hGrid1It = HGrid_problem1.particleHandler.begin();
    std::vector<BaseParticle*>::iterator hGrid2It = HGrid_problem2.particleHandler.begin();
    for (std::vector<BaseParticle*>::iterator mdIt = MD_problem.particleHandler.begin(); mdIt != MD_problem.particleHandler.end(); ++mdIt)
    {
        if (!(*mdIt)->getPosition().compareTo((*hGrid1It)->getPosition(),1e-10))
        {
            exit(-1);
        }
        if (!(*mdIt)->getPosition().compareTo((*hGrid2It)->getPosition(), 1e-10))
        {
            exit(-1);
        }
        ++hGrid1It;
        ++hGrid2It;
    }
}
