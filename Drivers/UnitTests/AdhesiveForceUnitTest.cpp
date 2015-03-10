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

#include "Particles/BaseParticle.h"
#include "Species/Species.h"
#include "Species/NormalForceSpecies/LinearViscoelasticNormalSpecies.h"
#include "Species/NormalForceSpecies/LinearPlasticViscoelasticNormalSpecies.h"
#include "Species/FrictionForceSpecies/SlidingFrictionSpecies.h"
#include "Species/AdhesiveForceSpecies/ReversibleAdhesiveSpecies.h"
#include "Species/AdhesiveForceSpecies/IrreversibleAdhesiveSpecies.h"
#include "DPMBase.h"
#include "Walls/InfiniteWall.h"
#include "Logger.h"

///This code is written to test short-distance non-contact
///forces such as van-der-Waals or liquid bridge forces. 
///A simple reversible adhesion force is combined with a linear-elastic 
///contact force.
class ParticleParticleInteraction : public DPMBase{
public:

    Species<LinearViscoelasticNormalSpecies, SlidingFrictionSpecies,IrreversibleAdhesiveSpecies>* species;

    ParticleParticleInteraction()
    {
        species = new Species<LinearViscoelasticNormalSpecies, SlidingFrictionSpecies,IrreversibleAdhesiveSpecies>;
        speciesHandler.addObject(species);
    }

	void setupInitialConditions()
	{
        relVelocity_ = 1e-1;
        setName("AdhesiveForceUnitTest_ParticleParticleInteraction");

        setParticleDimensions(3);
        species->setDensity(2000.0);
        species->setStiffness(1e2);
	    species->setAdhesionStiffness(1e2);
	    species->setAdhesionForceMax(1e-5*species->getAdhesionStiffness());
        species->setSlidingStiffness(1.2e1);
        species->setSlidingFrictionCoefficient(0.01);

        setSystemDimensions(3);
        setGravity(Vec3D(0,0,0));
        setTimeStep(5e-6 / 2.0);
        Mdouble R = 1e-3;
	    setXMax( 2*R);
		setYMax(   R);
		setZMax(   R);
		setXMin(-2*R);
		setYMin(-R);
		setZMin(-R);
		
        particleHandler.clear();
		BaseParticle P;
		P.setRadius(R);

		P.setPosition(Vec3D(-R-species->getInteractionDistance()/3,0,0));
		P.setVelocity(Vec3D(relVelocity_/2,0,0));
		particleHandler.copyAndAddObject(P);
		P.setPosition(Vec3D(R+species->getInteractionDistance()/3,0,0));
		P.setVelocity(Vec3D(-relVelocity_/2,0,0));
		particleHandler.copyAndAddObject(P);
				
		setTimeMax(getTimeStep()*250*2);
        setFileType(FileType::ONE_FILE);
        setSaveCount(1);

	}
  
private:
	Mdouble relVelocity_;
};

class ParticleParticleInteractionWithPlasticForces : public DPMBase{

public:
    Species<LinearPlasticViscoelasticNormalSpecies, SlidingFrictionSpecies,IrreversibleAdhesiveSpecies>* species;

    ParticleParticleInteractionWithPlasticForces()
    {
        species = new Species<LinearPlasticViscoelasticNormalSpecies, SlidingFrictionSpecies,IrreversibleAdhesiveSpecies>;
        speciesHandler.addObject(species);
    }

    void setupInitialConditions()
	{
        relVelocity_ = 1e-1;
        setSystemDimensions(3);
        setParticleDimensions(3);
        setGravity(Vec3D(0,0,0));
        setTimeStep(5e-6 / 2.0);
        Mdouble R = 1e-3;
        setXMax( 2*R);
        setYMax(   R);
        setZMax(   R);
        setXMin(-2*R);
        setYMin(-R);
        setZMin(-R);

        species->setDensity(2000.0);
        species->setLoadingStiffness(1e2);
        species->setUnloadingStiffnessMax(1.5* species->getLoadingStiffness());
        species->setCohesionStiffness(species->getLoadingStiffness());
        species->setPenetrationDepthMax(0.05);
        species->setAdhesionStiffness(1e2);
        species->setAdhesionForceMax(1e-5*species->getAdhesionStiffness());
        species->setSlidingStiffness(1.2e1);
        species->setSlidingFrictionCoefficient(0.01);

        particleHandler.clear();
		BaseParticle P;
		P.setRadius(R);
        
		P.setPosition(Vec3D(-R-species->getInteractionDistance()/3,0,0));
		P.setVelocity(Vec3D(relVelocity_/2,0,0));
		particleHandler.copyAndAddObject(P);
		P.setPosition(Vec3D(R+species->getInteractionDistance()/3,0,0));
		P.setVelocity(Vec3D(-relVelocity_/2,0,0));
		particleHandler.copyAndAddObject(P);
        setName("AdhesiveForceUnitTest_ParticleParticleInteractionWithPlasticForces");
        setTimeMax(getTimeStep()*250*5);
        setFileType(FileType::ONE_FILE);
        setSaveCount(1);

    }

private:
    Mdouble relVelocity_;
};


class ParticleWallInteraction : public DPMBase{
public:

    Species<LinearViscoelasticNormalSpecies, SlidingFrictionSpecies,IrreversibleAdhesiveSpecies>* species;

    ParticleWallInteraction()
    {
        species = new Species<LinearViscoelasticNormalSpecies, SlidingFrictionSpecies,IrreversibleAdhesiveSpecies>;
        speciesHandler.addObject(species);
    }

protected:
	void setupInitialConditions()
	{
        relVelocity_ = 1e-1;
        setName("AdhesiveForceUnitTest_ParticleWallInteractionWithPlasticForces");

        setSystemDimensions(3);
		setParticleDimensions(3);
		setGravity(Vec3D(0,0,0));

        species->setDensity(2000.0);
        species->setStiffness(1e2);
	    species->setAdhesionStiffness(1e2);
	    species->setAdhesionForceMax(1e-5*species->getAdhesionStiffness());
	    Mdouble R = 1e-3;
        species->setSlidingStiffness(1.2e1);
        species->setSlidingFrictionCoefficient(0.01);
        setTimeStep(5e-6 / 2.0);
        
	    setXMax( 2*R);
		setYMax(   R);
		setZMax(   R);
		setXMin(0);
		setYMin(-R);
		setZMin(-R);
		
        particleHandler.clear();
		BaseParticle P;
		P.setRadius(R);
		P.setPosition(Vec3D(R+species->getInteractionDistance()*1/3,0,0));
		P.setVelocity(Vec3D(-relVelocity_/2,0,0));
		particleHandler.copyAndAddObject(P);
		
   		wallHandler.clear();
		InfiniteWall w;
		w.set(Vec3D(-1, 0, 0), Vec3D(getXMin(), 0, 0));
		wallHandler.copyAndAddObject(w);
     
		setTimeMax(getTimeStep()*250*4);
        setFileType(FileType::ONE_FILE);
		setSaveCount(1);
	}
public:
	Mdouble relVelocity_;
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
    //First test Particle Particle interactions with no plastic forces so reverseable.
	ParticleParticleInteraction ParticleParticleInteractionProblem;
	ParticleParticleInteractionProblem.solve();
    
    logger(INFO,"Testing particle-particle collision for elastic adhesive forces");
    std::vector<BaseParticle*>::iterator pIt = ParticleParticleInteractionProblem.particleHandler.begin();
    if (!(*pIt)->getPosition().isEqualTo(Vec3D(-0.00101582359198137,0.0,0.0), 1e-7))
        logger(FATAL,"First particle is in the wrong position. It is at %",(*pIt)->getPosition());
    if (!(*pIt)->getVelocity().isEqualTo(Vec3D(-0.0436824,0.0,0.0)  , 1e-7))
        logger(FATAL,"First particle has the wrong velocity. It is at %",(*pIt)->getVelocity());
    ++pIt;
    if (!(*pIt)->getPosition().isEqualTo(Vec3D(0.00101582359198137,0.0,0.0), 1e-7))
        logger(FATAL,"Second particle is in the wrong position. It is at %",(*pIt)->getPosition());
    if (!(*pIt)->getVelocity().isEqualTo(Vec3D(0.0436824,0.0,0.0)  , 1e-7))
        logger(FATAL,"Second particle has the wrong velocity. It is at %",(*pIt)->getVelocity());

    //Second test Particle Particle interaction with a plastic force (non reverseable)
	ParticleParticleInteractionWithPlasticForces ParticleParticleInteractionWithPlasticForcesProblem;
	ParticleParticleInteractionWithPlasticForcesProblem.solve();
    
    logger(INFO,"Testing particle particles collision for plastic adhesive forces");
    std::vector<BaseParticle*>::iterator pIt2 = ParticleParticleInteractionWithPlasticForcesProblem.particleHandler.begin();
    if (!(*pIt2)->getPosition().isEqualTo(Vec3D(-0.00107858,0.0,0.0), 1e-7))
        logger(FATAL,"First particle is in the wrong position. It is at %",(*pIt2)->getPosition());
    if (!(*pIt2)->getVelocity().isEqualTo(Vec3D(-0.0352526,0.0,0.0)  , 1e-7))
        logger(FATAL,"First particle has the wrong velocity. It is at %",(*pIt2)->getVelocity());
    ++pIt2;
    if (!(*pIt2)->getPosition().isEqualTo(Vec3D(0.00107858,0.0,0.0), 1e-7))
        logger(FATAL,"Second particle is in the wrong position. It is at %",(*pIt2)->getPosition());
    if (!(*pIt2)->getVelocity().isEqualTo(Vec3D(0.0352526,0.0,0.0)  , 1e-7))
        logger(FATAL,"Second particle has the wrong velocity. It is at %",(*pIt2)->getVelocity());
    
    //Final test Particle-Wall interactions with a plastic force (non reversable)
	ParticleWallInteraction ParticleWallInteraction;
	ParticleWallInteraction.solve();
    
    logger(INFO,"Testing particle-wall collision for plastic adhesive forces");
    std::vector<BaseParticle*>::iterator pIt3 = ParticleWallInteraction.particleHandler.begin();
    if (!(*pIt3)->getPosition().isEqualTo(Vec3D(0.00104381,0.0,0.0), 1e-7))
        logger(FATAL,"The particle is in the wrong position. It is at %",(*pIt3)->getPosition());
    if (!(*pIt3)->getVelocity().isEqualTo(Vec3D(0.0362122,0.0,0.0)  , 1e-7))
        logger(FATAL,"The particle has the wrong velocity. It is at %",(*pIt3)->getVelocity());
   
    return 0;
}
// now plot
// p 'interactionRadius.fstat' u 7:($9/100) every 4::5 w lp
