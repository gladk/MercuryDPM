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

#include "Mercury3D.h"
#include "StatisticsVector.h"
#include "Particles/BaseParticle.h"
#include "Walls/InfiniteWall.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <Species/LinearViscoelasticSpecies.h>

/// In this file a cubic packing of 5^3 particles in a tri-axial box is created and allowed to settle under small gravity. After that Z statistics are calculated.

class NewtonsCradleSelftest : public Mercury3D
{

public:

    void setupInitialConditions()
    {
        double Radius = .5;

        setXMin(0);
        setYMin(0);
        setZMin(0);
        setXMax(5);
        setYMax(5);
        setZMax(5);

        BaseParticle P0;
        for (int i = 0; i < N; i++)
            for (int j = 0; j < N; j++)
                for (int k = 0; k < N; k++)
                {
                    P0.setRadius(Radius);
                    P0.setVelocity(Vec3D(0.0, 0.0, 0.0));
                    P0.setPosition(Radius * Vec3D(1. + 2. * i, 1. + 2. * j, 1. + 2. * k));
                    particleHandler.copyAndAddObject(P0);
                }

        //set walls
        InfiniteWall w0;
        w0.set(Vec3D(-1, 0, 0), -getXMin());
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D(1, 0, 0), getXMax());
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D(0, -1, 0), -getYMin());
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D(0, 1, 0), getYMax());
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D(0, 0, -1), -getZMin());
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D(0, 0, 1), getZMax());
        wallHandler.copyAndAddObject(w0);
    }

    int N;

};

int main(int argc UNUSED, char *argv[] UNUSED)
{
    NewtonsCradleSelftest problem;
    problem.setName("CubicPackingSelfTest");
    auto species=problem.speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());

    //set the number of particles
    problem.N = 5;
    problem.setSystemDimensions(3);
    problem.speciesHandler.getObject(0)->setDensity(1.9098593);
    problem.setGravity(Vec3D(0., 0., -1.));
    species->setCollisionTimeAndRestitutionCoefficient(.01, .1, 1.);
    problem.setTimeStep(.0002);
    problem.setTimeMax(0.2);
    problem.setSaveCount(1000);
    problem.solve();

    StatisticsVector<Z> stats("CubicPackingSelfTest");
    double n = 500;
    stats.setN(n);
    stats.setCGWidth(.1);
    stats.setSuperExact(false);
    stats.setCGShape("Gaussian");
    stats.setCGTimeMin(problem.getTimeMax() * .999999);
    stats.setTimeMaxStat(1e20);
    stats.setVerbosityLevel(0);
    stats.statistics_from_fstat_and_data();
    /// should give you Density 1

}

