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
#include "Boundaries/PeriodicBoundary.h"
#include "Boundaries/AngledPeriodicBoundary.h"

/*! 
 * This code tests if the contact properties are preserved when a particle pair 
 * crosses a set of AngularPeriodicWalls. It creates a pair of adhesive-elastic 
 * particles that rotate around the center of the domain due to a body force.
 */
class AngledPeriodicBoundaryUnitTest : public DPMBase
{
public:

    //sets species and periodic boundary
    AngledPeriodicBoundaryUnitTest()
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
        setTimeStep(1e-5);
        
        PeriodicBoundary b1;
        b1.set({0,1,0},0,4);
        //boundaryHandler.copyAndAddObject(b1);        

        AngledPeriodicBoundary b;
        Mdouble c = cos(constants::pi/2.0);
        Mdouble s = sqrt(1.0-c*c);
        Vec3D normal_left(s,-c,0.0);
        Vec3D normal_right(0.0,-1.0,0.0);
        Vec3D origin(0.0,0.0,0.0);
        b.set(normal_left,normal_right,origin);
        //comment line below to see how the system behaves w/o AngledPeriodicBoundary
        boundaryHandler.copyAndAddObject(b);        
    }

//    void actionsBeforeTimeStep()
//    {
//        std::cout << std::endl << "actionsBeforeTimeStep time " << getNtimeSteps() << std::endl;
//        particleHandler.write(std::cout);
//        interactionHandler.write(std::cout);
//    }
//
//    //computes body force
//    void computeAllForces()
//    {
//        DPMBase::computeAllForces();
//        std::cout << std::endl << "computeAllForces time " << getNtimeSteps() << std::endl;
//        particleHandler.write(std::cout);
//        interactionHandler.write(std::cout);
//    }

    //sets initial particle positions
    void setupInitialConditions()
    {
        //centrifugal acc. a=v^2/R;
        Mdouble R = 4.0;
        BaseParticle p;
        p.setSpecies(speciesHandler.getObject(0));
		p.setRadius(0.5);
		p.setPosition({R,0.5,0});
		particleHandler.copyAndAddObject(p);
		
        p.setRadius(0.5);
		p.setPosition({R,-0.5,0});
		//p.setVelocity({-sqrt(R)*s,sqrt(R)*c,0});
		p.setAngularVelocity({0,0,1});
		particleHandler.copyAndAddObject(p);
    }
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
    AngledPeriodicBoundaryUnitTest dpm;
    dpm.setName("AngledPeriodicBoundaryUnitTest");
    dpm.setTimeMax(4.0*constants::pi);   
    dpm.setTimeMax(9.5*dpm.getTimeStep());   
    dpm.setXMin(-5);   
    dpm.setXMax( 5);   
    dpm.setYMin(-5);   
    dpm.setYMax( 5);   
    dpm.setZMin(-5);   
    dpm.setZMax( 5);
    dpm.setGravity({0,0,0});
    dpm.solve(argc, argv);
    
    //Note:: currently the rolling spring is not conserved!!
    return 0;
}
// now plot
// p 'interactionRadius.fstat' u 7:($9/100) every 4::5 w lp
