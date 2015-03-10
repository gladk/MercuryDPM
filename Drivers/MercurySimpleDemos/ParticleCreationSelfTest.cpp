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

//based on /storage2/usr/people/sluding/MDCC/C3DshearXL30/MU0_LONG2
#include "Mercury3D.h"
#include "Species/LinearViscoelasticSpecies.h"
#include "Particles/BaseParticle.h"
#include "Boundaries/PeriodicBoundary.h"
#include "Walls/AxisymmetricIntersectionOfWalls.h"

/** This self test was written to test the speed of particle creation in MercuryDPM.
 * The code below creates 200'000 non-overlapping particles of homogeneous size.
 **/
class ParticleCreation : public Mercury3D
{
public:

    ParticleCreation(Mdouble width, Mdouble length, Mdouble height, Mdouble sizeDistribution) : sizeDistribution_(sizeDistribution)
    {

        setXMin(0);
        setXMax(width);
        setYMin(0);
        setYMax(length);
        setZMin(0);
        setZMax(height);

        std::cout << "Creating flat-wall box of"
            " width [" << getXMin() << ", " << getXMax() << "],"
            " length [" << getYMin() << ", " << getYMax() << "],"
            " height [" << getZMin() << ", " << getZMax() << "]." << std::endl;

        //create new species
        species = speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());
        species->setDensity(6.0 / constants::pi);

        //set inter-particle contact properties
        species->setStiffnessAndRestitutionCoefficient(10, 0.1, 1.0);

        setTimeStep(0.02 * species->getCollisionTime(species->getMassFromRadius(0.5)));
        std::cout << "time step used "
            << getTimeStep() << std::endl;

        //set timestep
        setTimeMax(getTimeStep());

        //set walls
        InfiniteWall w;
        w.set(Vec3D(0, 0, -1), Vec3D(0, 0, getZMin()));
        wallHandler.copyAndAddObject(w);
        w.set(Vec3D(0, -1, 0), Vec3D(0, getYMin(), 0));
        wallHandler.copyAndAddObject(w);
        w.set(Vec3D(-1, 0, 0), Vec3D(getXMin(), 0, 0));
        wallHandler.copyAndAddObject(w);
        w.set(Vec3D(0, 0, 1), Vec3D(0, 0, getZMax()));
        wallHandler.copyAndAddObject(w);
        w.set(Vec3D(0, 1, 0), Vec3D(0, getYMax(), 0));
        wallHandler.copyAndAddObject(w);
        w.set(Vec3D(1, 0, 0), Vec3D(getXMax(), 0, 0));
        wallHandler.copyAndAddObject(w);
    }

    void setupInitialConditions()
    {
        //number of particles to be inserted
        unsigned int n = static_cast<unsigned int>((getXMax() - getXMin())*(getYMax() - getYMin())*(getZMax() - getZMin()));
        //try to find new insertable particles
        unsigned int i = 0;
        BaseParticle p;
        p.setSpecies(species);
        Mdouble s = sizeDistribution_;
        Mdouble rMin = cbrt(0.5 / (s * s + 1.0) / (s + 1.0));
        std::cout << "Inserting a maximum of " << n << " particles with " 
                << 2.0*rMin << "<d<" << 2.0*s*rMin 
                << " (sizeDistribution " << s << ")" << std::endl;
        //this already changes the largest particle (before it gets inserted into DPMBase)
        p.setRadius(s * rMin);
        Vec3D position;
        unsigned int failed =0;
        while (i < n) {
            position.X = random.getRandomNumber(getXMin() + p.getRadius(),
                                                getXMax() - p.getRadius());
            position.Y = random.getRandomNumber(getYMin() + p.getRadius(),
                                                getYMax() - p.getRadius());
            position.Z = random.getRandomNumber(getZMin() + p.getRadius(),
                                                getZMax() - p.getRadius());
            p.setPosition(position);
            if (checkParticleForInteraction(p)) {
//                std::cout << "insert r=" << p.getRadius() 
//                    << ", N=" << particleHandler.getNumberOfObjects() 
//                    << ", R=" << particleHandler.getLargestParticle()->getRadius()
//                    << ", " << particleHandler.getLargestParticle()
//                    << ", " << &p
//                    << std::endl;
                particleHandler.copyAndAddObject(p);
                if (hGridNeedsRebuilding()) {
                    hGridRebuild();
                }
                //std::cout << "done inserting, N=" << particleHandler.getNumberOfObjects() << std::endl;
                p.setRadius(random.getRandomNumber(rMin, s * rMin));
                //std::cout << "next particle r=" << p.getRadius() << std::endl;
                i++;
                if (particleHandler.getNumberOfObjects() % 1000 == 0) {
                    std::cout << " " << particleHandler.getNumberOfObjects() / 1000 << "k";
                    std::flush(std::cout);
                }
                failed=0;
            }
            else
            {
                failed++;
                if (failed>1000)
                {
                    //std::cout << std::endl << "failed to insert 100 particles in a row; aborting" << std::endl;
                    break;
                }
            }
        }
        std::cout << std::endl << "Inserted " << particleHandler.getNumberOfObjects() << " particles" << std::endl;
        //hGridRebuild();
        //hGridInfo();
        //exit(-1);
    }

    LinearViscoelasticSpecies* species;
    Mdouble sizeDistribution_;
};

int main(int argc, char *argv[])
{
    Mdouble width = 20; //in particle diameters
    Mdouble length = 20; //in particle diameters
    Mdouble height = 20; //in particle diameters
    Mdouble sizeDistribution = 4;
    ParticleCreation SC(width, length, height, sizeDistribution);
    ///\todo we should force people to set a name
    SC.setName("ParticleCreationSelfTest");
    SC.solve(argc, argv);
    //SC.hGridInfo();
    return 0;
}
