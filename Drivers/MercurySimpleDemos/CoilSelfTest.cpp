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

//! [CST:headers]
#include "Mercury3D.h"
#include "Particles/BaseParticle.h"
#include "Walls/InfiniteWall.h"
#include "Walls/InfiniteWallWithHole.h"
#include "Walls/Coil.h"
#include "Species/LinearViscoelasticSpecies.h"
//! [CST:headers]

//! [CST:class]
class CoilSelfTest : public Mercury3D {
    
private:

    void setupInitialConditions() {
        //! [CST:initial]
        // gravity, particle radius
        setGravity(Vec3D(0.0, -9.8, 0.0));
        particleRadius = 0.2;
        
        // set problem geometry
        setXMax(1.0);
        setYMax(5.0);
        setZMax(2.0);
        setXMin(-1.0);
        setYMin(-1.0);
        //! [CST:initial]
        
        //! [CST:species]
        species = speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());
        species->setDensity(1000);
        species->setCollisionTimeAndRestitutionCoefficient(0.05, 0.8, pow(particleRadius, 3) * constants::pi * 4.0 / 3.0 * 1000);
        //! [CST:species]
        
        //! [CST:walls]
        frontWall = wallHandler.copyAndAddObject(InfiniteWall());
        frontWall->set(Vec3D(-1, 0, 0), Vec3D(getXMin(), 0, 0));
        backWall = wallHandler.copyAndAddObject(InfiniteWall());
        backWall->set(Vec3D(1, 0, 0), Vec3D(getXMax(), 0, 0));
        bottomWall = wallHandler.copyAndAddObject(InfiniteWall());
        bottomWall->set(Vec3D(0, -1, 0), Vec3D(0, getYMin(), 0));
        topWall = wallHandler.copyAndAddObject(InfiniteWall());
        topWall->set(Vec3D(0, 1, 0), Vec3D(0, getYMax(), 0));
        leftWall = wallHandler.copyAndAddObject(InfiniteWall());
        leftWall->set(Vec3D(0, 0, -1), Vec3D(0, 0, getZMin()));

        rightWall = wallHandler.copyAndAddObject(InfiniteWallWithHole());
        rightWall->set(Vec3D(0, 0, 1), getZMax(), 1.0);
        //! [CST:walls]
        
        //! [CST:coil]
        // creation of the coil and setting of its properties
        coil = wallHandler.copyAndAddObject(Coil());
        // set(Start position, Length, Radius, Number of turns, Rotation speed, Thickness)
        coil->set(Vec3D(0, 0, 0), 1.0, 1.0 - particleRadius, 2.0, -1.0, 0.5 * particleRadius);
        //! [CST:coil]
        
        //! [CST:particle]
        particleHandler.clear();
        BaseParticle p0;
        p0.setSpecies(species);
        p0.setVelocity(Vec3D(0.0, 0.0, 0.0));
        p0.setRadius(particleRadius);
        //! [CST:particle]
        
        /*
        //Single test case
        double distance;
                Vec3D normal;
        p0.setPosition(Vec3D(1.0,0.0,0.0));
        if(coil->getDistance_and_normal(p0, distance, normal))
                        std::cout<<"Collision, distance screw="<<distance<<std::endl;
                else
                        std::cout<<"No collision, distance screw="<<distance<<std::endl;
         */
        
        //! [CST:placeparticles]
        // Nx*Ny*Nz particles are created and placed on evenly spaced positions in 
        // the domain [xMin_,xMax_]*[yMin_,yMax_]*[zMin_,zMax_] (unless their position 
        // is already occupied by the coil).
        int Nx = static_cast<int> (std::floor((getXMax() - getXMin()) / (2.1 * particleRadius)));
        int Ny = static_cast<int> (std::floor((getYMax() - getYMin()) / (2.1 * particleRadius)));
        int Nz = static_cast<int> (std::floor((getZMax() - getZMin()) / (2.1 * particleRadius)));
        Mdouble distance;
        Vec3D normal;
        
        // Place particles
        for (int i = 0; i < Nx; i++)
            for (int j = 0; j < Ny; j++)
                for (int k = 0; k < Nz; k++) {
                    p0.setPosition(Vec3D(getXMin()+(getXMax() - getXMin())*(0.5 + i) / Nx, getYMin()+(getYMax() - getYMin())*(0.5 + j) / Ny, getZMin()+(getZMax() - getZMin())*(0.5 + k) / Nz));
                    if (!coil->getDistanceAndNormal(p0, distance, normal)) {
                        particleHandler.copyAndAddObject(p0);
                    } else {
                        //std::cout<<p0.getPosition()<<std::endl;
                    }
                }
        //! [CST:placeparticles]
    }
    
    //! [CST:beforetime]
    void actionsBeforeTimeStep() {
        if (getTime() > 1)
            coil->move_time(getTimeStep());

    }
    //! [CST:beforetime]

    //! [CST:datamembers]
public:
    double particleRadius;
    LinearViscoelasticSpecies* species;
    Coil* coil;
    InfiniteWall* frontWall;
    InfiniteWall* backWall;
    InfiniteWall* topWall;
    InfiniteWall* bottomWall;
    InfiniteWall* leftWall;
    InfiniteWallWithHole* rightWall;
    //! [CST:datamembers]
};
//! [CST:class]

//! [CST:main]
int main(int argc UNUSED, char *argv[] UNUSED) {
    
    // create CoilSelfTest object
    CoilSelfTest problem;
    
    // set some basic problem properties
    problem.setName("CoilSelfTest");
    problem.setSystemDimensions(3);
    problem.setTimeStep(0.02 * 0.05);
    problem.setTimeMax(2.0);
    problem.setSaveCount(helpers::getSaveCountFromNumberOfSavesAndTimeMaxAndTimestep(1000, problem.getTimeMax(), problem.getTimeStep()));
    
    // actually solving the problem
    problem.solve();
    
}
//! [CST:main]
// the end
