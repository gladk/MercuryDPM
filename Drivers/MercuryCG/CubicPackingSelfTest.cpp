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

