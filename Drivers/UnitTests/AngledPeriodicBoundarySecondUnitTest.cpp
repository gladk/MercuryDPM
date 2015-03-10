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

#include "Species/Species.h"
#include "Species/LinearViscoelasticFrictionReversibleAdhesiveSpecies.h"
#include "DPMBase.h"
#include "Boundaries/AngledPeriodicBoundary.h"

/*! 
 * This code tests if the contact properties are preserved when a particle pair 
 * crosses a set of AngularPeriodicWalls. It creates a pair of adhesive-elastic 
 * particles that rotate around the center of the domain due to a body force.
 */
class AngledPeriodicBoundarySecondUnitTest : public DPMBase
{
public:

    //sets species and periodic boundary

    AngledPeriodicBoundarySecondUnitTest()
    {
        LinearViscoelasticFrictionReversibleAdhesiveSpecies species;
        //set density such that mass is 1.0 for diameter 1.0 particles
        species.setDensity(6.0 / constants::pi);
        species.setStiffness(1e2);
        species.setAdhesionStiffness(1e2);
        //set such that the overlap for force balance is 10%
        species.setAdhesionForceMax(1e-1 * species.getAdhesionStiffness());
        species.setRollingFrictionCoefficient(1e20);
        species.setRollingStiffness(1.0);
        speciesHandler.copyAndAddObject(species);
        setTimeStep(0.01 * species.getCollisionTime(1.0));

        AngledPeriodicBoundary b;
        Mdouble c = cos(constants::pi / 2.0);
        Mdouble s = sqrt(1.0 - c * c);
        Vec3D normal_left(s, -c, 0.0);
        Vec3D normal_right(0.0, -1.0, 0.0);
        Vec3D origin(0.0, 0.0, 0.0);
        b.set(normal_left, normal_right, origin);
        //comment the line below to see how the system behaves without 
        //the AngledPeriodicBoundary
        boundaryHandler.copyAndAddObject(b);
    }

    //computes body force

    void computeExternalForces(BaseParticle* p)
    {
        if (!p->isFixed())
        {
            // Gravitational force
            Vec3D normal = p->getPosition() / Vec3D::getLength(p->getPosition());
            p->addForce(-normal * p->getMass());
            // Still calls this in compute External Forces.
            computeForcesDueToWalls(p);
        }
    }

    //sets initial particle positions

    void setupInitialConditions()
    {
        //centrifugal acc. a=v^2/R;
        Mdouble velocity = 2.0;
        Mdouble R = 4.0;
        BaseParticle p;
        p.setSpecies(speciesHandler.getObject(0));
        p.setRadius(0.5);
        p.setPosition({R, 0, 0});
        p.setVelocity({0, sqrt(R), 0});
        //p.setAngularVelocity({0,0,1});
        particleHandler.copyAndAddObject(p);

        //http://www.mathsisfun.com/algebra/trig-cosine-law.html
        Mdouble d = 0.9;
        Mdouble c = (2.0 * R * R - d * d) / (2.0 * R * R);
        Mdouble s = sqrt(1.0 - c * c);
        p.setRadius(0.5);
        p.setPosition({R*c, R*s, 0});
        p.setVelocity({-sqrt(R) * s, sqrt(R) * c, 0});
        p.setAngularVelocity({0, 0, 1});
        particleHandler.copyAndAddObject(p);

        setXMin(-R-1);
        setXMax(R+1);
        setYMin(-R-1);
        setYMax(R+1);
        setZMin(-R-1);
        setZMax(R+1);

    }
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
    AngledPeriodicBoundarySecondUnitTest dpm;
    dpm.setName("AngledPeriodicBoundarySecondUnitTest");
    dpm.setTimeMax(4.0 * constants::pi);
    dpm.solve(argc, argv);

    AngledPeriodicBoundarySecondUnitTest dpm2;
    dpm2.setName("AngledPeriodicBoundarySecondUnitTest2");
    dpm2.boundaryHandler.clear();
    dpm2.setTimeMax(4.0 * constants::pi);
    dpm2.solve(argc, argv);

    if (!mathsFunc::isEqual(dpm.particleHandler.getObject(0)->getAngularVelocity(), 
                            dpm2.particleHandler.getObject(0)->getAngularVelocity(), 1e-10))
    {
        logger(ERROR, "angular velocity of the two simulations doesn't match");
    }
    
    //Note:: currently the rolling spring is not conserved!!
    return 0;
}
// now plot
// p 'interactionRadius.fstat' u 7:($9/100) every 4::5 w lp
