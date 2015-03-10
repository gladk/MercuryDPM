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

#include "ChuteBottom.h"
#include "Species/LinearViscoelasticSpecies.h"
#include "Species/FrictionForceSpecies/SlidingFrictionSpecies.h"
#include "Boundaries/PeriodicBoundary.h"
#include "Walls/InfiniteWall.h"

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

void ChuteBottom::makeRoughBottom(ParticleHandler &ChuteParticleHandler)
{
    // set all parameters that should be different from the original chute
    setChuteAngle(0.0);
    setInflowHeight(25. * getInflowParticleRadius());
    //~ setInflowHeight(45.*getInflowParticleRadius()); note: Changing the Inflow height was an attempt to make the bottom density homogeneous, but it did not have the desired effect
    setRoughBottomType(MONOLAYER_DISORDERED);
    setFixedParticleRadius(getInflowParticleRadius());
  


    auto species = dynamic_cast<LinearViscoelasticSpecies*>(speciesHandler.getObject(0));
    if (species!=nullptr)
    {
        Mdouble collisionTime= helpers::computeCollisionTimeFromKAndDispAndEffectiveMass(species->getStiffness(),species->getDensity(),0.5*species->getMassFromRadius(0.5 * (getMinInflowParticleRadius() + getMaxInflowParticleRadius())));
        species->setCollisionTimeAndRestitutionCoefficient(collisionTime*10.0,0.2,species->getMassFromRadius(0.5 * (getMinInflowParticleRadius() + getMaxInflowParticleRadius())));
        setTimeStep(10.0 * 0.02 * helpers::computeCollisionTimeFromKAndDispAndEffectiveMass(species->getStiffness(),species->getDensity(),species->getMassFromRadius(0.5 * (getMinInflowParticleRadius() + getMaxInflowParticleRadius()))));
    }
    else
    {
        std::cerr << "ChuteBottom::makeRoughBottom: Warning: species type does not allow setting the parameters" << std::endl;
    }
    auto species2 = dynamic_cast<SlidingFrictionSpecies*>(speciesHandler.getObject(0));
    if (species2 != nullptr)
        species2->setSlidingFrictionCoefficient(0);

    setTimeMax(getTimeStep() * 2e3);
    //set_number_of_saves(2);
    setSaveCount(100);

    //solve
    solve();

    //createBottom
    Mdouble height = 0;
    for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it != particleHandler.end(); it++)
    {
        height = std::max(height, (*it)->getPosition().Z);
    }

    std::cout << "Thickness" << thickness_ << std::endl;
    ///todo{Dinant is not a fan of this alogirm (i.e. poping back stuff while in iterator}
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
    ChuteParticleHandler.setStorageCapacity(particleHandler.getNumberOfObjects());
    std::cout << "Chute bottom finished, consisting of " << particleHandler.getNumberOfObjects() << " Particles" << std::endl;

    ChuteParticleHandler = particleHandler;
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
        w0.set(Vec3D(0.0, 0.0, -1.0), -getZMin() + getInflowParticleRadius());
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
        w0.set(Vec3D(0.0, 0.0, -1.0), -getZMin() + getInflowParticleRadius());
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D(-1.0, 0.0, 0.0), -getXMin());
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D(1.0, 0.0, 0.0), getXMax());
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D(0.0, -1.0, 0.0), -getYMin());
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D(0.0, 1.0, 0.0), getYMax());
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
        std::cerr << "Error: thickness " << new_ << " negative" << std::endl;
        exit(-1);
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
