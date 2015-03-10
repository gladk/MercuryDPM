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
#include "Particles/BaseParticle.h"
#include <iostream>
#include <cmath>
#include <Species/LinearViscoelasticSpecies.h>

class SpeciesTest : public Mercury3D
{
public:
    virtual void printTime() const
    {
    }
    
    void setupInitialConditions()
    {
        auto species0 = new LinearViscoelasticSpecies;
        speciesHandler.addObject(species0);
        species0->setDensity(6. / constants::pi);

        double tc = 1e-2, r = 0.4, m = 1;
        //sets k and dissipation_ based on the collision time and restitution coefficient for a collision with effective mass m
        //species0->setStiffnessAndDissipation(helpers::computeKAndDispFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(tc, r, m));
        //setCollisionTimeAndRestitutionCoefficient(tc,r, 1.0,1.0);
        species0->setCollisionTimeAndRestitutionCoefficient(tc, r, m);
        
        setSaveCount(50);
        ///Note: the restitution coefficient converges if the time steps per collision, tc/dt, is increased. Maybe this should be checked here.
        setTimeStep(tc / 50.);
        setTimeMax(3.0);
        setXMax(10);
        setYMax(10);
        setZMax(1);
        setZMin(-1);
        setGravity(Vec3D(0, 0, 0));

        auto species1 = speciesHandler.copyAndAddObject(species0);

        BaseParticle P0, P1, P2, P3, P4, P5;
        //Small particles are species 0 and large particles are species two
        //Collison small - small
        P0.setRadius(.5);
        P0.setIndSpecies(0);
        P0.setPosition(Vec3D(0.0, 0.0, 0.0));
        P0.setVelocity(Vec3D(1.0, 0.0, 0.0));
        
        P1.setRadius(.5);
        P1.setIndSpecies(0);
        P1.setPosition(Vec3D(2.5, 0.0, 0.0));
        P1.setVelocity(Vec3D(0.0, 0.0, 0.0));
        
        //Collision small - large
        P2.setRadius(.5);
        P2.setIndSpecies(0);
        P2.setPosition(Vec3D(0.0, 0.0, 2.0));
        P2.setVelocity(Vec3D(1.0, 0.0, 0.0));
        
        P3.setRadius(1.0);
        P3.setIndSpecies(1);
        P3.setPosition(Vec3D(2.5, 0.0, 2.0));
        P3.setVelocity(Vec3D(0.0, 0.0, 0.0));
        
        //Collision large - large
        P4.setRadius(1.0);
        P4.setIndSpecies(1);
        P4.setPosition(Vec3D(0.0, 0.0, 4.0));
        P4.setVelocity(Vec3D(1.0, 0.0, 0.0));
        
        P5.setRadius(1.0);
        P5.setIndSpecies(1);
        P5.setPosition(Vec3D(2.5, 0.0, 4.0));
        P5.setVelocity(Vec3D(0.0, 0.0, 0.0));
        
        particleHandler.copyAndAddObject(P0);
        particleHandler.copyAndAddObject(P1);
        particleHandler.copyAndAddObject(P2);
        particleHandler.copyAndAddObject(P3);
        particleHandler.copyAndAddObject(P4);
        particleHandler.copyAndAddObject(P5);
        
        species1->setCollisionTimeAndRestitutionCoefficient(tc, r, 8.0, 8.0);
        speciesHandler.getMixedObject(species0, species1)->setCollisionTimeAndRestitutionCoefficient(tc, r, 1.0, 8.0);
        
    }
    
    void actionsAfterSolve()
    {       
        for (unsigned int i = 0; i < 5; i += 2)
        {
            double r= particleHandler.getObject(i + 1)->getVelocity().X - particleHandler.getObject(i)->getVelocity().X;
            std::cout << "Cofficient of resutions for collision " << round(i / 2.0) + 1 << " is : " << r << std::endl;
	    if (std::abs(r-0.4)>1e-2)
        {
            std::cerr << "Wrong restitution" << std::endl;
            exit(-1);
        }
	}
    }
    
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
    std::cout << "Species test problem " << std::endl;
    std::cout << "---------------------" << std::endl;
    std::cout << std::endl;
    std::cout << "This problem set k and gamma for different for different size particles such that the coefficent of restitution is the same" << std::endl;
    std::cout << "Note for each collision this value should be 0.4" << std::endl;
    std::cout << "Also this code demostrates how to use different species in the code" << std::endl;
    SpeciesTest problem;
    problem.setName("SpeciesUnitTest");
    
    problem.solve();
}
