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
