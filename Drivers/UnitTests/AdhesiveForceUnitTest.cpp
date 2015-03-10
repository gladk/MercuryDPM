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

#include "Particles/BaseParticle.h"
#include "Species/Species.h"
#include "Species/LinearViscoelasticSpecies.h"
#include "Species/LinearPlasticViscoelasticSpecies.h"
#include "Species/TangentialForceSpecies/SlidingFrictionSpecies.h"
#include "Species/AdhesiveForceSpecies/LinearReversibleAdhesiveSpecies.h"
#include "Species/AdhesiveForceSpecies/LinearIrreversibleAdhesiveSpecies.h"
#include "DPMBase.h"
#include "Walls/InfiniteWall.h"
#include "Logger.h"
extern Logger<LOG_MAIN_LEVEL> logger;

///This code is written to test short-distance non-contact
///forces such as van-der-Waals or liquid bridge forces. 
///A simple reversible adhesion force is combined with a linear-elastic 
///contact force.
class ParticleParticleInteraction : public DPMBase{
public:

    Species<LinearViscoelasticSpecies, SlidingFrictionSpecies,LinearIrreversibleAdhesiveSpecies>* species;

    ParticleParticleInteraction()
    {
        species = new Species<LinearViscoelasticSpecies, SlidingFrictionSpecies,LinearIrreversibleAdhesiveSpecies>;
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
    Species<LinearPlasticViscoelasticSpecies, SlidingFrictionSpecies,LinearIrreversibleAdhesiveSpecies>* species;

    ParticleParticleInteractionWithPlasticForces()
    {
        species = new Species<LinearPlasticViscoelasticSpecies, SlidingFrictionSpecies,LinearIrreversibleAdhesiveSpecies>;
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

    Species<LinearViscoelasticSpecies, SlidingFrictionSpecies,LinearIrreversibleAdhesiveSpecies>* species;

    ParticleWallInteraction()
    {
        species = new Species<LinearViscoelasticSpecies, SlidingFrictionSpecies,LinearIrreversibleAdhesiveSpecies>;
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
		w.set(Vec3D(-1, 0, 0), -getXMin());
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
    
    logger.log(Log::INFO,"Testing particle-particle collision for elastic adhesive forces");
    std::vector<BaseParticle*>::iterator pIt = ParticleParticleInteractionProblem.particleHandler.begin();
    if (!(*pIt)->getPosition().compareTo(Vec3D(-0.00101582359198137,0.0,0.0), 1e-7))
        logger.log(Log::FATAL,"First particle is in the wrong position. It is at %",(*pIt)->getPosition());
    if (!(*pIt)->getVelocity().compareTo(Vec3D(-0.0436824,0.0,0.0)  , 1e-7))
        logger.log(Log::FATAL,"First particle has the wrong velocity. It is at %",(*pIt)->getVelocity());
    ++pIt;
    if (!(*pIt)->getPosition().compareTo(Vec3D(0.00101582359198137,0.0,0.0), 1e-7))
        logger.log(Log::FATAL,"Second particle is in the wrong position. It is at %",(*pIt)->getPosition());
    if (!(*pIt)->getVelocity().compareTo(Vec3D(0.0436824,0.0,0.0)  , 1e-7))
        logger.log(Log::FATAL,"Second particle has the wrong velocity. It is at %",(*pIt)->getVelocity());

    //Second test Particle Particle interaction with a plastic force (non reverseable)
	ParticleParticleInteractionWithPlasticForces ParticleParticleInteractionWithPlasticForcesProblem;
	ParticleParticleInteractionWithPlasticForcesProblem.solve();
    
    logger.log(Log::INFO,"Testing particle particles collision for plastic adhesive forces");
    std::vector<BaseParticle*>::iterator pIt2 = ParticleParticleInteractionWithPlasticForcesProblem.particleHandler.begin();
    if (!(*pIt2)->getPosition().compareTo(Vec3D(-0.00107858,0.0,0.0), 1e-7))
        logger.log(Log::FATAL,"First particle is in the wrong position. It is at %",(*pIt2)->getPosition());
    if (!(*pIt2)->getVelocity().compareTo(Vec3D(-0.0352526,0.0,0.0)  , 1e-7))
        logger.log(Log::FATAL,"First particle has the wrong velocity. It is at %",(*pIt2)->getVelocity());
    ++pIt2;
    if (!(*pIt2)->getPosition().compareTo(Vec3D(0.00107858,0.0,0.0), 1e-7))
        logger.log(Log::FATAL,"Second particle is in the wrong position. It is at %",(*pIt2)->getPosition());
    if (!(*pIt2)->getVelocity().compareTo(Vec3D(0.0352526,0.0,0.0)  , 1e-7))
        logger.log(Log::FATAL,"Second particle has the wrong velocity. It is at %",(*pIt2)->getVelocity());
    
    //Final test Particle-Wall interactions with a plastic force (non reversable)
	ParticleWallInteraction ParticleWallInteraction;
	ParticleWallInteraction.solve();
    
    logger.log(Log::INFO,"Testing particle-wall collision for plastic adhesive forces");
    std::vector<BaseParticle*>::iterator pIt3 = ParticleWallInteraction.particleHandler.begin();
    if (!(*pIt3)->getPosition().compareTo(Vec3D(0.00104381,0.0,0.0), 1e-7))
        logger.log(Log::FATAL,"The particle is in the wrong position. It is at %",(*pIt3)->getPosition());
    if (!(*pIt3)->getVelocity().compareTo(Vec3D(0.0362122,0.0,0.0)  , 1e-7))
        logger.log(Log::FATAL,"The particle has the wrong velocity. It is at %",(*pIt3)->getVelocity());
   
    return 0;
}
// now plot
// p 'interactionRadius.fstat' u 7:($9/100) every 4::5 w lp
