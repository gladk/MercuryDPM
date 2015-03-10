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

        double tc = 1e-2, r = 0.4, m = 0.5;
        //sets k and dissipation_ based on the collision time and restitution coefficient for a collision with effective mass m
        species0->setNormalStiffnessAndDissipation(helpers::computeKAndDispFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(tc, r, m));
        //setCollisionTimeAndRestitutionCoefficient(tc,r, 1.0,1.0);
        
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
