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

#include "DPMBase.h"
#include "Particles/BaseParticle.h"
#include "Walls/InfiniteWall.h"
#include "Species/Species.h"
#include "Species/LinearViscoelasticSlidingFrictionSpecies.h"
//#include "Interactions/SlidingFrictionInteraction.h"
//#include "Interactions/LinearViscoelasticInteraction.h"

class particle_particle_collision : public DPMBase{
public:

    particle_particle_collision() 
    {
        species = speciesHandler.copyAndAddObject(LinearViscoelasticSlidingFrictionSpecies());
    }
    
	virtual void setupInitialConditions()
	{
		static int count = -1;
		count++;

		// only do this for the first time
		if (!count) {
            setSystemDimensions(2);
			setParticleDimensions(3);
            species->setDensity(2000.0);

			unsigned int nParticle = 2;
			normal = Vec3D(-1.0,0.0,0.0);
			tangent = Vec3D(0.0,1.0,0.0);
			
			BaseParticle P0;
            for (unsigned int j=0; j< nParticle; j++){
				P0.setRadius(random.getRandomNumber(0.0005,0.001));
            	particleHandler.copyAndAddObject(P0);
                particleHandler.getLastObject()->computeMass();

            }

			//set random relative velocity
			initialNormalRelativeVelocity = random.getRandomNumber(-0.1,-0.05);
			initialTangentialRelativeVelocity = random.getRandomNumber( 0.05, 0.1);
		
			tc = random.getRandomNumber(0.0,1.0e-5);
			en = random.getRandomNumber(0.5,1.0);	
				
			setXMax(0.005);
			setYMax(0.005);
			setZMax(0.005);
		
			m12 = (particleHandler.getObject(0)->getMass()*particleHandler.getObject(1)->getMass())/(particleHandler.getObject(0)->getMass()+particleHandler.getObject(1)->getMass());
            species->setCollisionTimeAndRestitutionCoefficient(tc,en,2.0*m12);
	        setTimeMax(2.0*tc);
            setTimeStep(tc / 200);
			setSaveCount(1);
		}
		
		// do this for all solves
		particleHandler.getObject(0)->setVelocity(normal*initialNormalRelativeVelocity+tangent*initialTangentialRelativeVelocity);
		particleHandler.getObject(1)->setVelocity(Vec3D(0.0,0.0,0.0));
		particleHandler.getObject(0)->setAngularVelocity(Vec3D(0.0,0.0,0.0));
		particleHandler.getObject(1)->setAngularVelocity(Vec3D(0.0,0.0,0.0));
		
		particleHandler.getObject(0)->setPosition(Vec3D(0.0025-particleHandler.getObject(0)->getRadius(),0.0025,0.0025));
		particleHandler.getObject(1)->setPosition(Vec3D(0.0025+particleHandler.getObject(1)->getRadius(),0.0025,0.0025));
		particleHandler.getObject(0)->setOrientation(Vec3D(0.0,0.0,0.0));
		particleHandler.getObject(1)->setOrientation(Vec3D(0.0,0.0,0.0));
		
 	}
	
	Vec3D getRelativeVelocity() {
		return particleHandler.getObject(0)->getVelocity() - particleHandler.getObject(1)->getVelocity() + Vec3D::cross(normal,particleHandler.getObject(0)->getAngularVelocity()*particleHandler.getObject(0)->getRadius() + particleHandler.getObject(1)->getAngularVelocity()*particleHandler.getObject(1)->getRadius());
	}
	
	void getRelativeVelocityComponents(Mdouble& normalRelativeVelocity, Mdouble& tangentialRelativeVelocity) {
		Vec3D relativeVelocity = getRelativeVelocity();
		normalRelativeVelocity = relativeVelocity.X / normal.X;
		tangentialRelativeVelocity = relativeVelocity.Y / tangent.Y;
	}
	
	///Calculates collision time for two copies of a particle of species 0
	Mdouble getCollisionTime(Mdouble mass){
		return species->getCollisionTime(mass);
	}
	
	
	Mdouble initialNormalRelativeVelocity, initialTangentialRelativeVelocity;
	Vec3D normal, tangent;
	Mdouble tc, en;
	Mdouble m12;
    LinearViscoelasticSlidingFrictionSpecies* species;
};

class wall_particle_collision : public particle_particle_collision{
public:
	
	void setupInitialConditions()
	{
        static int count = -1;
		count++;
		
		// only do this for the first time
		if (!count) {
            setSystemDimensions(2);
			setParticleDimensions(3);
            species->setDensity(2000.0);
			
			
			InfiniteWall w0;
			w0.set(Vec3D(1, 0, 0), 0.0025);
			wallHandler.copyAndAddObject(w0);
			
			BaseParticle P0;
			particleHandler.copyAndAddObject(P0);			
			setXMax(0.005);
			setYMax(0.005);
			setZMax(0.005);
			
			//set random masses

			particleHandler.getObject(0)->setRadius(random.getRandomNumber(0.0005,0.001));
			particleHandler.getObject(0)->computeMass();
			
			//set random relative velocity
			normal = Vec3D(-1.0,0.0,0.0);
			tangent = Vec3D(0.0,1.0,0.0);
			initialNormalRelativeVelocity = random.getRandomNumber(-0.1,-0.05);
			initialTangentialRelativeVelocity = random.getRandomNumber( 0.05, 0.1);
			
			tc = random.getRandomNumber(0.0,1.0e-5);
			en = random.getRandomNumber(0.5,1.0);
			m12 = particleHandler.getObject(0)->getMass(); // wall counts as an infinite mass
            species->setCollisionTimeAndRestitutionCoefficient(tc,en,2.0*m12);
			setTimeMax(2.0*tc);
            setTimeStep(tc / 200);
			setSaveCount(1);
		}
		
		// do this for all solves
		particleHandler.getObject(0)->setVelocity(normal*initialNormalRelativeVelocity+tangent*initialTangentialRelativeVelocity);
		particleHandler.getObject(0)->setAngularVelocity(Vec3D(0.0,0.0,0.0));
		
		particleHandler.getObject(0)->setPosition(Vec3D(0.0025-particleHandler.getObject(0)->getRadius(),0.0025,0.0025));
		particleHandler.getObject(0)->setOrientation(Vec3D(0.0,0.0,0.0));
	}
	
	Vec3D getRelativeVelocity() {
		return particleHandler.getObject(0)->getVelocity() + Vec3D::cross(normal,particleHandler.getObject(0)->getAngularVelocity()*particleHandler.getObject(0)->getRadius());
	}
	void getRelativeVelocityComponents(Mdouble& normalRelativeVelocity, Mdouble& tangentialRelativeVelocity) {
		Vec3D relativeVelocity = getRelativeVelocity();
		normalRelativeVelocity = relativeVelocity.X / normal.X;
		tangentialRelativeVelocity = relativeVelocity.Y / tangent.Y;
	}
	
};

void particle_particle_test()
{
	std::cout << std::endl << "testing particle-particle collisions ..." << std::endl << std::endl;

	particle_particle_collision problem;
    
	problem.random.setRandomSeed(5);

	problem.setupInitialConditions();
	Mdouble normalRelativeVelocity, tangentialRelativeVelocity, analyticTangentialRelativeVelocity;
	
	std::cout << "5: without tangential forces" << std::endl;
	problem.setName("force_test5");
	problem.solve();
	problem.getRelativeVelocityComponents(normalRelativeVelocity, tangentialRelativeVelocity);
	std::cout << "tangentialRelativeVelocity: analytic=" << problem.initialTangentialRelativeVelocity << ", simulation=" << tangentialRelativeVelocity << std::endl;
	std::cout << "normalRelativeVelocity: analytic:" << -problem.en*problem.initialNormalRelativeVelocity << ", simulation=" << normalRelativeVelocity << std::endl;

	//problem.setAppend_to_files(true);

	std::cout << "6: with Coulomb friction" << std::endl;
	Mdouble mu = problem.random.getRandomNumber(0.0,1.0);
    problem.species->setSlidingFrictionCoefficient(mu);
	problem.species->setSlidingDissipation(1e20);
	//problem.species->setSlidingStiffness(0.0);
    problem.setName("force_test6");
	problem.solve();
	problem.getRelativeVelocityComponents(normalRelativeVelocity, tangentialRelativeVelocity);
	analyticTangentialRelativeVelocity = std::max(0.0,problem.initialTangentialRelativeVelocity + mu*3.5*(1+problem.en)*problem.initialNormalRelativeVelocity);
	std::cout << "tangentialRelativeVelocity: analytic=" << analyticTangentialRelativeVelocity << ", simulation=" << tangentialRelativeVelocity << std::endl;
	std::cout << "normalRelativeVelocity: analytic:" << -problem.en*problem.initialNormalRelativeVelocity << ", simulation=" << normalRelativeVelocity << std::endl;
	
	std::cout << "7: with Coulomb friction, spring activated" << std::endl;
	problem.species->setSlidingStiffness(1.0);
	//problem.species->setSlidingDissipation(1);
	problem.setName("force_test7");
	problem.solve();
	std::cout << "tangentialRelativeVelocity: analytic=" << analyticTangentialRelativeVelocity << ", simulation=" << tangentialRelativeVelocity << std::endl;
	std::cout << "normalRelativeVelocity: analytic:" << -problem.en*problem.initialNormalRelativeVelocity << ", simulation=" << normalRelativeVelocity << std::endl;

	std::cout << "8: with tangential viscous force" << std::endl;
	Mdouble et = problem.random.getRandomNumber(-1.0,0.0);
	problem.species->setSlidingFrictionCoefficient(1e20);
	problem.species->setSlidingDissipation(-log(-et)/(2.0*problem.tc) /3.5 * 2.0 * problem.m12);
	problem.species->setSlidingStiffness(0.0);
	problem.setName("force_test8");
	problem.solve();
	problem.getRelativeVelocityComponents(normalRelativeVelocity, tangentialRelativeVelocity);
	analyticTangentialRelativeVelocity = problem.initialTangentialRelativeVelocity * exp(-2.0*3.5*problem.species->getSlidingDissipation()/(2.0*problem.m12) * problem.getCollisionTime(2.0*problem.m12));
	std::cout << "tangentialRelativeVelocity: analytic=" << analyticTangentialRelativeVelocity << ", simulation=" << tangentialRelativeVelocity << std::endl;
	std::cout << "normalRelativeVelocity: analytic:" << -problem.en*problem.initialNormalRelativeVelocity << ", simulation=" << normalRelativeVelocity << std::endl;
	
	std::cout << "9: with tangential elastic force" << std::endl;
	Mdouble et2 = problem.random.getRandomNumber(0.0,1.0);
	problem.species->setSlidingFrictionCoefficient(1e20);
	problem.species->setSlidingDissipation(0.0);
	problem.species->setSlidingStiffness(problem.species->getStiffness()/3.5*mathsFunc::square(acos(-et2)/constants::pi));
	problem.setName("force_test9");
	problem.solve();
	problem.getRelativeVelocityComponents(normalRelativeVelocity, tangentialRelativeVelocity);
	analyticTangentialRelativeVelocity = problem.initialTangentialRelativeVelocity * cos(sqrt(problem.species->getSlidingStiffness()/problem.m12*3.5)* problem.getCollisionTime(2.0*problem.m12));
	std::cout << "tangentialRelativeVelocity: analytic=" << analyticTangentialRelativeVelocity << ", simulation=" << tangentialRelativeVelocity << std::endl;
	std::cout << "normalRelativeVelocity: analytic:" << -problem.en*problem.initialNormalRelativeVelocity << ", simulation=" << normalRelativeVelocity << std::endl;

}

void wall_particle_test()
{
	std::cout << std::endl << "testing wall-particle collisions ..." << std::endl << std::endl;
	
	srand(5);
	
	wall_particle_collision problem;
    problem.setupInitialConditions();

    Mdouble normalRelativeVelocity, tangentialRelativeVelocity, analyticTangentialRelativeVelocity;
	
	//problem.setAppend_to_files(true);
	
	std::cout << "0: without tangential forces" << std::endl;
	problem.setName("force_test0");
	problem.solve();
	
	problem.getRelativeVelocityComponents(normalRelativeVelocity, tangentialRelativeVelocity);
	std::cout << "tangentialRelativeVelocity: analytic=" << problem.initialTangentialRelativeVelocity << ", simulation=" << tangentialRelativeVelocity << std::endl;
	std::cout << "normalRelativeVelocity: analytic:" << -problem.en*problem.initialNormalRelativeVelocity << ", simulation=" << normalRelativeVelocity << std::endl;

	std::cout << "1: with Coulomb friction" << std::endl;
	Mdouble mu = problem.random.getRandomNumber(0.0,1.0);
	problem.species->setSlidingFrictionCoefficient(mu);
	problem.species->setSlidingDissipation(1e20);
	problem.species->setSlidingStiffness(0.0);
    problem.setName("force_test1");
	problem.solve();
	problem.getRelativeVelocityComponents(normalRelativeVelocity, tangentialRelativeVelocity);
	analyticTangentialRelativeVelocity = std::max(0.0,problem.initialTangentialRelativeVelocity + mu*3.5*(1+problem.en)*problem.initialNormalRelativeVelocity);
	std::cout << "tangentialRelativeVelocity: analytic=" << analyticTangentialRelativeVelocity << ", simulation=" << tangentialRelativeVelocity << std::endl;
	std::cout << "normalRelativeVelocity: analytic:" << -problem.en*problem.initialNormalRelativeVelocity << ", simulation=" << normalRelativeVelocity << std::endl;

	std::cout << "2: with Coulomb friction, spring activated" << std::endl;
    problem.species->setSlidingStiffness(1.0);
    problem.setName("force_test2");
	problem.solve();
	std::cout << "tangentialRelativeVelocity: analytic=" << analyticTangentialRelativeVelocity << ", simulation=" << tangentialRelativeVelocity << std::endl;
	std::cout << "normalRelativeVelocity: analytic:" << -problem.en*problem.initialNormalRelativeVelocity << ", simulation=" << normalRelativeVelocity << std::endl;

	std::cout << "3: with tangential viscous force" << std::endl;
	Mdouble et = problem.random.getRandomNumber(-1.0,0.0);
	problem.species->setSlidingFrictionCoefficient(1e20);
	problem.species->setSlidingDissipation(-log(-et)/(2.0*problem.tc) /3.5 * 2.0 * problem.m12);
	problem.species->setSlidingStiffness(0.0);
    problem.setName("force_test3");
	problem.solve();
	problem.getRelativeVelocityComponents(normalRelativeVelocity, tangentialRelativeVelocity);
	analyticTangentialRelativeVelocity = problem.initialTangentialRelativeVelocity * exp(-2.0*3.5*problem.species->getSlidingDissipation()/(2.0*problem.m12) * problem.getCollisionTime(2.0*problem.m12));
	std::cout << "tangentialRelativeVelocity: analytic=" << analyticTangentialRelativeVelocity << ", simulation=" << tangentialRelativeVelocity << std::endl;
	std::cout << "normalRelativeVelocity: analytic:" << -problem.en*problem.initialNormalRelativeVelocity << ", simulation=" << normalRelativeVelocity << std::endl;

	std::cout << "4: with tangential elastic force" << std::endl;
	Mdouble et2 = problem.random.getRandomNumber(0.0,1.0);
	problem.species->setSlidingFrictionCoefficient(1e20);
	problem.species->setSlidingDissipation(0.0);
	problem.species->setSlidingStiffness(problem.species->getStiffness()/3.5*mathsFunc::square(acos(-et2)/constants::pi));
	problem.setName("force_test4");
	problem.solve();
	problem.getRelativeVelocityComponents(normalRelativeVelocity, tangentialRelativeVelocity);
	analyticTangentialRelativeVelocity = problem.initialTangentialRelativeVelocity * cos(sqrt(problem.species->getSlidingStiffness()/problem.m12*3.5)* problem.getCollisionTime(2.0*problem.m12));
	std::cout << "tangentialRelativeVelocity: analytic=" << analyticTangentialRelativeVelocity << ", simulation=" << tangentialRelativeVelocity << std::endl;
	std::cout << "normalRelativeVelocity: analytic:" << -problem.en*problem.initialNormalRelativeVelocity << ", simulation=" << normalRelativeVelocity << std::endl;
}



int main(int argc UNUSED, char *argv[] UNUSED) {
	
	std::cout << std::endl << "note: analytic values are only asymptotically correct as delta->0" << std::endl;
	particle_particle_test();
	wall_particle_test();
	
	return 0;
}

