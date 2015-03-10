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


#include "ChuteBottom.h"
#include "Species/LinearViscoelasticSpecies.h"
#include "Species/FrictionForceSpecies/SlidingFrictionSpecies.h"
#include "Boundaries/PeriodicBoundary.h"
#include "Walls/InfiniteWall.h"
#include <Logger.h>

ChuteBottom::ChuteBottom()
{
    constructor();
}

ChuteBottom::ChuteBottom(const DPMBase& other)
: DPMBase(other), Chute(other)
{
    constructor();
}

ChuteBottom::ChuteBottom(const MercuryBase& other)
: DPMBase(other), Chute(other)
{
    constructor();
}

ChuteBottom::ChuteBottom(const Mercury3D& other)
: DPMBase(other), Chute(other)
{
    constructor();
}

ChuteBottom::ChuteBottom(const Chute& other)
: DPMBase(other), Chute(other)
{
    constructor();
}

void ChuteBottom::constructor()
{
    setName("roughbottom");
    getFStatFile().setFileType(FileType::NO_FILE); //set to 0 for no data creation
    getDataFile().setFileType(FileType::ONE_FILE);
    getRestartFile().setFileType(FileType::ONE_FILE);
    setThickness(2.4);
    setIsBottomPeriodic(true);
}

void ChuteBottom::makeRoughBottom(Chute &chute)
{
    // set all parameters that should be different from the original chute
    setChuteAngle(0.0);
    setInflowHeight(25. * getInflowParticleRadius());
    //~ setInflowHeight(45.*getInflowParticleRadius()); note: Changing the Inflow height was an attempt to make the bottom density homogeneous, but it did not have the desired effect
    setRoughBottomType(MONOLAYER_DISORDERED);
    setFixedParticleRadius(getInflowParticleRadius());

    auto species = dynamic_cast<LinearViscoelasticNormalSpecies*>(speciesHandler.getObject(0));
    if (species!=nullptr)
    {
        //increase collision time, lower dissipation, increase timestep
        Mdouble effectiveMass = 0.5*speciesHandler.getObject(0)->getMassFromRadius(0.5 * (getMinInflowParticleRadius() + getMaxInflowParticleRadius()));
        Mdouble collisionTime= helpers::computeCollisionTimeFromKAndDispAndEffectiveMass(species->getStiffness(),speciesHandler.getObject(0)->getDensity(),effectiveMass);
        species->setCollisionTimeAndRestitutionCoefficient(10.0*collisionTime,0.2,effectiveMass);
        setTimeStep(10.0 * 0.02 * helpers::computeCollisionTimeFromKAndDispAndEffectiveMass(species->getStiffness(),speciesHandler.getObject(0)->getDensity(),effectiveMass));
        ///\todo the above should be replaced with sth like this (and setTimeMax(getTimeStep() * 1e4);), but this would break the selftest and old codes
//        Mdouble effectiveMass = 0.5*speciesHandler.getObject(0)->getMassFromRadius(0.5 * (getMinInflowParticleRadius() + getMaxInflowParticleRadius()));
//        Mdouble collisionTime = species->getCollisionTime(2.0*effectiveMass);
//        species->setCollisionTimeAndRestitutionCoefficient(10.0*collisionTime,0.2,2.0*effectiveMass);
//        setTimeStep(0.2*collisionTime);
      }
    else
    {
        logger(WARN,"[ChuteBottom::makeRoughBottom()] species type does not allow setting the parameters.");
    }
    auto species2 = dynamic_cast<SlidingFrictionSpecies*>(speciesHandler.getObject(0));
    if (species2 != nullptr)
        species2->setSlidingFrictionCoefficient(0);

    setTimeMax(getTimeStep() * 2e3);
    //set_number_of_saves(2);
    setSaveCount(100);

    solve();

    //createBottom
    Mdouble height = 0;
    for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it != particleHandler.end(); it++)
    {
        height = std::max(height, (*it)->getPosition().Z);
    }

    std::cout << "Thickness" << thickness_ << std::endl;
    ///todo{Dinant is not a fan of this algorithm (i.e. popping back stuff while in iterator}
    //now cut a slice of width 2*MaxInflowParticleRadius
    for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it != particleHandler.end();)
    {
        if ((*it)->getPosition().Z < height - (1.0 + thickness_) * getMaxInflowParticleRadius() || (*it)->getPosition().Z > height - getMaxInflowParticleRadius())
        {
            //delete particles outside the given range
            //*it = particleHandler.back();
            //particleHandler.removeLastObject();
            particleHandler.removeObject((*it)->getIndex());
        }
        else
        {
            //fix the remaining particles on the floor

            //(*it)->getPosition().Z -= height - MaxInflowParticleRadius;
            (*it)->move(Vec3D(0.0, 0.0, getMaxInflowParticleRadius() - height));

            (*it)->fixParticle();
            it++;
        }
    }

    //copy the rough bottom over
    chute.particleHandler.setStorageCapacity(particleHandler.getNumberOfObjects());
    std::cout << "Chute bottom finished, consisting of " << particleHandler.getNumberOfObjects() << " Particles" << std::endl;

    chute.particleHandler = particleHandler;
    /// \todo{ after copying a particle handler, you have to set the species pointers to the species of the chute problem;
    /// a) do you need to set other things?
    /// b) this should probably be done in ParticleHandler::operator =
    /// }
    for (BaseParticle* p : chute.particleHandler)
    {
        p->setSpecies(chute.speciesHandler.getObject(0));
    }

}

void ChuteBottom::setupInitialConditions()
{

    particleHandler.setStorageCapacity(static_cast<unsigned int>(std::min(3.0 * getXMax() * getYMax() * getZMax() / mathsFunc::cubic(2.0*getInflowParticleRadius()), 1e6)));

    createBottom();

    ///todo{The createBottom() function also creates some walls and boudarys, but at slightly different locations. In this version they are removed and reset, but this is (in my opinion (Dinant)) not the correct way}
    wallHandler.clear();
    boundaryHandler.clear();
    if (isBottomPeriodic_)
    {
        InfiniteWall w0;
        w0.set(Vec3D(0.0, 0.0, -1.0), Vec3D(0, 0, getZMin() - getInflowParticleRadius()));
        wallHandler.copyAndAddObject(w0);
        PeriodicBoundary b0;
        b0.set(Vec3D(1.0, 0.0, 0.0), getXMin(), getXMax());
        boundaryHandler.copyAndAddObject(b0);
        b0.set(Vec3D(0.0, 1.0, 0.0), getYMin(), getYMax());
        boundaryHandler.copyAndAddObject(b0);
    }
    else
    {
        InfiniteWall w0;
        w0.set(Vec3D(0.0, 0.0, -1.0), Vec3D(0, 0, getZMin() - getInflowParticleRadius()));
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D(-1.0, 0.0, 0.0), Vec3D(getXMin(), 0, 0));
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D( 1.0, 0.0, 0.0), Vec3D(getXMax(), 0, 0));
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D(0.0,-1.0, 0.0), Vec3D(0, getYMin(), 0));
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D(0.0, 1.0, 0.0), Vec3D(0, getYMax(), 0));
        wallHandler.copyAndAddObject(w0);
    }

    //add particles
    hGridActionsBeforeTimeLoop();
    int failed = 0, max_failed = 500;
    //try max_failed times to find new insertable particle

    BaseParticle inflowParticle_;
    inflowParticle_.setHandler(&particleHandler);
    inflowParticle_.setOrientation(Vec3D(0.0, 0.0, 0.0));
    inflowParticle_.setAngularVelocity(Vec3D(0.0, 0.0, 0.0));
    while (failed <= max_failed)
    {
        inflowParticle_.setRadius(getFixedParticleRadius());
        //inflowParticle_.computeMass();

        //The position components are first stored in a Vec3D, because if you pass them directly into setPosition the compiler is allowed to change the order in which the numbers are generated
        Vec3D position;
        position.X = random.getRandomNumber(inflowParticle_.getRadius(), getXMax() - inflowParticle_.getRadius());
        position.Y = random.getRandomNumber(inflowParticle_.getRadius(), getYMax() - inflowParticle_.getRadius());
        position.Z = random.getRandomNumber(2 * inflowParticle_.getRadius(), getZMax() - inflowParticle_.getRadius());
        inflowParticle_.setPosition(position);
        inflowParticle_.setVelocity(Vec3D(0.0, 0.0, 0.0));
        
        
        if (checkParticleForInteraction(inflowParticle_))
        {
            particleHandler.copyAndAddObject(inflowParticle_);
            failed = 0;
        }
        else
        {
            failed++;
        }
    }
    //set_Nmax(particleHandler.getNumberOfObjects());
    std::cout << "N=" << particleHandler.getNumberOfObjects() << std::endl;

    //fix hgrid (there is still an issue when particles are polydispersed)
    //assume 1-2 levels are optimal (which is the case for mono and bidispersed) and set the cell size to min and max
    // !this is not optimal for polydispersed
    Mdouble minCell = 2. * std::min(getFixedParticleRadius(), getMinInflowParticleRadius());
    Mdouble maxCell = 2. * std::max(getFixedParticleRadius(), getMaxInflowParticleRadius());
    if ((minCell == maxCell) | (minCell == 0.))
        setHGridMaxLevels(1);
    else
        setHGridMaxLevels(2);
//		set_HGRID_cell_to_cell_ratio (1.0000000001*maxCell/minCell);
    //optimize number of buckets
    //setHGridNumberOfBucketsToPower(particleHandler.getNumberOfObjects() * 1.5);
    //end: fix hgrid

    //~ write(std::cout,false);
}

void ChuteBottom::actionsBeforeTimeStep()
{
}

Mdouble ChuteBottom::getThickness()
{
    return thickness_;
}
void ChuteBottom::setThickness(Mdouble new_)
{
    if (new_ > 0.0)
        thickness_ = new_;
    else
    {
        logger(ERROR,"[ChuteBottom::setThickness()] thickness % negative.", new_);
    }
}
Mdouble ChuteBottom::getIsBottomPeriodic()
{
    return isBottomPeriodic_;
}
void ChuteBottom::setIsBottomPeriodic(bool isBottomPeriodic)
{
    isBottomPeriodic_ = isBottomPeriodic;
}
