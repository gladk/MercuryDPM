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

#include <iomanip>
#include <Species/LinearViscoelasticSpecies.h>

#include "Mercury2D.h"

#include "Boundaries/CubeInsertionBoundary.h"
#include "Boundaries/MaserBoundary.h"
#include "Boundaries/PeriodicBoundary.h"
#include "Particles/BaseParticle.h"
#include "Walls/InfiniteWall.h"
#include "Math/ExtendedMath.h"

class MaserSelfTest : public Mercury2D
{
public:

    void printTime() const
    {
        std::cout << "t=" << std::setprecision(3) << std::left << std::setw(6) << getTime() << ", tmax=" << std::setprecision(3) << std::left << std::setw(6) << getTimeMax() <<std::endl;
    }

    void setupInitialConditions()
    {
        setName("MaserSelfTest");
        auto species =speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());
        species->setDensity(constants::pi / 6.0);
        species->setStiffness(1e5);
        species->setDissipation(10);
        setTimeStep(1e-5);
        setTimeMax(2.0);
        setSaveCount(2000);
        double angle=2.0/180.0*constants::pi;
        setGravity(10.0*Vec3D(sin(angle), -cos(angle), 0.0));
        setHGridMaxLevels(1);

        setXMin(0.0);
        setYMin(0.0);
        setXMax(8.0);
        setYMax(2.0);

        double radMin = 0.1;
        double radMax = 0.15;
        BaseParticle* particleToCopy =new BaseParticle;
        particleToCopy->setSpecies(species);
        CubeInsertionBoundary cubeInsertionBoundary;
        cubeInsertionBoundary.set(particleToCopy, 500, Vec3D(radMax, radMax, 0), Vec3D(1.0 - radMax, getYMax() - radMax, 0.0), Vec3D(0.0, 0.0, 0.0), Vec3D(0.0, 0.0, 0.0), radMin, radMax);
        cubeInsertionBoundary.checkBoundaryBeforeTimeStep(this);
        delete particleToCopy;

        InfiniteWall w0;
        w0.set(Vec3D(0.0, -1.0, 0.0), Vec3D(0.0, 0.0, 0.0));
        wallHandler.copyAndAddObject(w0);

        auto b0 = boundaryHandler.copyAndAddObject(MaserBoundary());
        b0->set(Vec3D(1.0, 0.0, 0.0), 0.0, 1.0);
        for (auto it = particleHandler.begin(); it != particleHandler.end(); ++it)
        {
            b0->addParticleToMaser(*it);
        }

        /*auto b0=boundaryHandler.copyAndAddObject(PeriodicBoundary());
        b0->set(Vec3D(1.0, 0.0, 0.0), 0.0, 1.0);*/
        write(std::cout,false);
    }

};

int main(int argc UNUSED, char *argv[] UNUSED)
{
    MaserSelfTest maserSelfTest;
    maserSelfTest.solve();
}
