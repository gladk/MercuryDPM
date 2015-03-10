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

#include "Chute.h"
#include "ChuteBottom.h"
#include "Particles/BaseParticle.h"
#include "Boundaries/ChuteInsertionBoundary.h"
#include "Boundaries/PeriodicBoundary.h"
#include "Walls/InfiniteWall.h"
#include <string>
#include <string.h>
#include <iomanip>
#include <Species/LinearViscoelasticSlidingFrictionSpecies.h>

#include "Logger.h"
extern Logger<LOG_MAIN_LEVEL> logger;

Chute::Chute()
{
    constructor();
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "Chute() finished" << std::endl;
#endif      
}

//The copy-constructor of DPMBase has to be called because the link from DPMBase to HGRID_base is virtual
Chute::Chute(const DPMBase& other)
        : DPMBase(other), Mercury3D(other)
{
    constructor();
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "Chute(MD& other) finished" << std::endl;
#endif      
}

//The copy-constructor of DPMBase has to be called because the link from DPMBase to HGRID_base is virtual
Chute::Chute(const MercuryBase& other)
        : DPMBase(other), Mercury3D(other)
{
    constructor();
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "Chute(HGRID_base& other)  finished" << std::endl;
#endif      
}

//The copy-constructor of DPMBase has to be called because the link from DPMBase to HGRID_base is virtual
Chute::Chute(const Mercury3D& other)
        : DPMBase(other), Mercury3D(other)
{
    constructor();
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "Chute(Mercury3D& other) finished" << std::endl;
#endif      
}

void Chute::constructor()
{
    isChutePeriodic_ = false;
    setFixedParticleRadius(0.001);
    setRoughBottomType(MONOLAYER_DISORDERED);
    setChuteAngle(0.0);
    
    setMaxFailed(1);
    setInflowParticleRadius(0.001);
    setInflowVelocity(0.1);
    setInflowVelocityVariance(0.0);
    setInflowHeight(0.02);
}

///This function reads all chute data
void Chute::read(std::istream& is)
{
    MercuryBase::read(is);
    //read out the full line first, so if there is an error it does not affect the read of the next line
    std::string line_string;
    getline(is, line_string);
    std::cout << "Chuteline=" << line_string << std::endl;
    std::stringstream line(std::stringstream::in | std::stringstream::out);
    line << line_string;
    
    if (getRestartVersion().compare("1"))
    {
        std::string dummy;
        unsigned int roughBottomType;
        line >> fixedParticleRadius_ >> roughBottomType >> chuteAngle_
                >> minInflowParticleRadius_ >> maxInflowParticleRadius_ >> maxFailed_ >> dummy
                >> inflowVelocity_ >> inflowVelocityVariance_ >> inflowHeight_;
        setRoughBottomType(static_cast<RoughBottomType>(roughBottomType));
    }
    else
    {
        std::string dummy;
        unsigned int roughBottomType;
        line >> dummy >> fixedParticleRadius_ >> dummy >> minInflowParticleRadius_ >> dummy >> maxInflowParticleRadius_
                >> dummy >> roughBottomType >> dummy >> chuteAngle_ >> dummy >> maxFailed_ >> dummy >> dummy
                >> dummy >> inflowVelocity_ >> dummy >> inflowVelocityVariance_ >> dummy >> inflowHeight_;
        setRoughBottomType(static_cast<RoughBottomType>(roughBottomType));
    }
    //if the Chute Angle is given in degrees, move to radians;
    if (chuteAngle_ > 1.0)
        chuteAngle_ *= constants::pi / 180.;
}

///This function writes all chute data
void Chute::write(std::ostream& os) const
        {
    MercuryBase::write(os);
    os << "FixedParticleRadius " << fixedParticleRadius_
            << " MinInflowParticleRadius " << minInflowParticleRadius_
            << " MaxInflowParticleRadius " << maxInflowParticleRadius_
            << std::endl
            << "RandomisedBottom " << roughBottomType_
            << " ChuteAngle " << chuteAngle_ / constants::pi * 180.
            << " max_failed " << maxFailed_;
    if (insertionBoundary_)
    {
        os << " num_created " << insertionBoundary_->getNumberOfParticlesInserted();
    }
    os << std::endl
            << "InflowVelocity " << inflowVelocity_
            << " InflowVelocityVariance " << inflowVelocityVariance_
            << " InflowHeight " << inflowHeight_
            << std::endl;
}

///This function std::couts all chute data
void Chute::write(std::ostream& os, bool writeAllParticles) const
        {
    MercuryBase::write(os, writeAllParticles);
    os << " FixedParticleRadius:" << fixedParticleRadius_ << ", InflowParticleRadius: [" << minInflowParticleRadius_ << "," << maxInflowParticleRadius_ << "]," << std::endl
            << " RandomisedBottom:" << roughBottomType_ << ", ChuteAngle:" << chuteAngle_ / constants::pi * 180. << ", max_failed:" << maxFailed_ << ", num_created:" << std::endl;
    if (insertionBoundary_)
    {
        os << insertionBoundary_->getNumberOfParticlesInserted();
    }
    else
    {
        os << 0;
    }
    os << "," << std::endl
            << " InflowVelocity:" << inflowVelocity_ << ", InflowVelocityVariance:" << inflowVelocityVariance_ << ", InflowHeight:" << inflowHeight_ << std::endl;
}

void Chute::actionsBeforeTimeStep()
{
    cleanChute();
}

void Chute::printTime() const
{
    std::cout << "\rt=" << std::setprecision(3) << std::left << std::setw(6) << getTime()
            << ", tmax=" << std::setprecision(3) << std::left << std::setw(6) << getTimeMax()
            << ", N=" << std::setprecision(3) << std::left << std::setw(6) << particleHandler.getNumberOfObjects()
            << std::endl;
    std::cout.flush();
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///This initially set up the particles///
////////////////////////////////////////////////////////////////////////////////////////////////////
void Chute::setupInitialConditions()
{
    setupSideWalls();
    
    ChuteInsertionBoundary b1;
    b1.set(new BaseParticle, maxFailed_, Vec3D(getXMin(), getYMin(), getZMin()), Vec3D(getXMax(), getYMax(), getZMax()), minInflowParticleRadius_, maxInflowParticleRadius_, fixedParticleRadius_, inflowVelocity_, inflowVelocityVariance_);
    insertionBoundary_ = boundaryHandler.copyAndAddObject(b1);

    //creates the bottom of the chute
    createBottom();
}

void Chute::setupSideWalls()
{
    //set side walls - solid if not a periodic
    if (isChutePeriodic_)
    {
        PeriodicBoundary b0;
        b0.set(Vec3D(0.0, 1.0, 0.0), getYMin(), getYMax());
        boundaryHandler.copyAndAddObject(b0);
    }
    else
    {
        InfiniteWall w0;
        w0.set(Vec3D(0.0, -1.0, 0.0), -getYMin());
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D(0.0, 1.0, 0.0), getYMax());
        wallHandler.copyAndAddObject(w0);
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///Creates the bottom of the chute; either smooth, grid-like or random ///
////////////////////////////////////////////////////////////////////////////////////////////////////
void Chute::createBottom()
{
    /// \bug This is temporary fix and how to do this should be discussed. Thomas how much damping should there be?
    //auto species = new LinearViscoelasticSpecies;
    //speciesHandler.addObject(species);
    //setDimension(3);
    //species->setDensity(2000.0);
    //species->setStiffness(8000000.0);

    
    
    if (fabs(getFixedParticleRadius()) < 1e-12) // smooth bottom
    {
        //bottom wall 
        InfiniteWall w0;
        w0.set(Vec3D(0.0, 0.0, -1.0), -getZMin());
        wallHandler.copyAndAddObject(w0);
    }
    else //rough bottom
    {
        // Define standard fixed particle
        /// \todo Does the bottom we always has to be this particle?
        BaseParticle F0;
        F0.setHandler(&particleHandler);
        F0.setRadius(getFixedParticleRadius());
        F0.setPosition(Vec3D(0.0, 0.0, 0.0));
        
        
        if (roughBottomType_ == MONOLAYER_ORDERED)
        {
            // grid-like fixed-particle bottom
            Mdouble dx = 2.0 * F0.getRadius();
            Mdouble dy = 2.0 * F0.getRadius();
            unsigned int nx = static_cast<unsigned int>(std::max(1, static_cast<int>(std::floor((getXMax() - getXMin()) / dx))));
            unsigned int ny = static_cast<unsigned int>(std::max(1, static_cast<int>(std::floor((getYMax() - getYMin()) / dy))));
            dx = (getXMax() - getXMin()) / nx;
            dy = (getYMax() - getYMin()) / ny;
            for (unsigned int i = 0; i < nx; i++)
            {
                for (unsigned int j = 0; j < ny; j++)
                {
                    F0.setPosition(Vec3D(F0.getRadius() + dx * i, F0.getRadius() + dy * j, 0.0));
                    particleHandler.copyAndAddObject(F0);
                }
            }
        }
        else if (roughBottomType_ == MONOLAYER_DISORDERED)
        { // random fixed-particle bottom
            std::cout << "create rough chute bottom, fixed z" << std::endl;
            
            //bottom wall 
            InfiniteWall w0;
            w0.set(Vec3D(0.0, 0.0, -1.0), -(getZMin() - .5 * F0.getRadius()));
            wallHandler.copyAndAddObject(w0);
            
            //add first particle to initialise HGRID
            
            //The position components are first stored in a Vec3D, because if you pass them directly into setPosition the compiler is allowed to change the order in which the numbers are generated
            Vec3D position;
            position.X = random.getRandomNumber(F0.getRadius(), getXMax() - F0.getRadius());
            position.Y = random.getRandomNumber(getYMin() + F0.getRadius(), getYMax() - F0.getRadius());
            F0.setPosition(position);
            particleHandler.copyAndAddObject(F0);
            
            hGridActionsBeforeTimeLoop();
            hGridActionsBeforeTimeStep();
            
            //now add more particles
            int failed = 0;
            while (failed < 500)
            {
                //The position components are first stored in a Vec3D, because if you pass them directly into setPosition the compiler is allowed to change the order in which the numbers are generated
                position.X = random.getRandomNumber(F0.getRadius(), getXMax() - F0.getRadius());
                position.Y = random.getRandomNumber(getYMin() + F0.getRadius(), getYMax() - F0.getRadius());
                F0.setPosition(position);
                if (checkParticleForInteraction(F0))
                {
                    particleHandler.copyAndAddObject(F0);
                    failed = 0;
                }
                else
                {
                    failed++;
                }
            }
        }
        else //if (roughBottomType_ == MULTILAYER)
        {
            //this pointer is the current MD class, so the bottom is create with the particles properties from the MD class
            ChuteBottom bottom(*this);
            bottom.setInflowParticleRadius(getFixedParticleRadius());
            bottom.makeRoughBottom(particleHandler);
            std::cout << "Starting to destruct ChuteBottom" << std::endl;
        }
        std::cout << "Destructed ChuteBottom" << std::endl;
        //finally, fix particles to the floor
        for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it != particleHandler.end(); ++it)
            (*it)->fixParticle();
        
        std::cout << "but here we get" << std::endl;
        
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///New particles are destroyed at the outflow, subject to criteria the user can set
////////////////////////////////////////////////////////////////////////////////////////////////////
void Chute::cleanChute()
{
    //clean outflow every 100 timesteps
    static int count = 0, maxcount = 100;
    if (count > maxcount)
    {
        count = 0;
        // delete all outflowing particles
        for (unsigned int i = 0; i < particleHandler.getNumberOfObjects();)
        {
            
            if (particleHandler.getObject(i)->getPosition().X > getXMax() || particleHandler.getObject(i)->getPosition().X < getXMin()) //||particleHandler.getObject(i)->Position.Z+particleHandler.getObject(i)->Radius<zMin_)
                    
            {
#ifdef DEBUG_OUTPUT_FULL
                std::cout << "erased:" << particleHandler.getObject(i) << std::endl;
#endif
                particleHandler.removeObject(i);
            }
            else
                i++;
        }
    }
    else
        count++;
}

int Chute::readNextArgument(int& i, int argc, char *argv[])
{
    if (!strcmp(argv[i], "-inflowHeight"))
    {
        setInflowHeight(atof(argv[i + 1]));
        setZMax(atof(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-inflowVelocity"))
    {
        setInflowVelocity(atof(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-chuteAngle"))
    {
        setChuteAngle(atof(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-chuteLength"))
    {
        setChuteLength(atof(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-chuteWidth"))
    {
        setChuteWidth(atof(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-fixedParticleRadius"))
    {
        setFixedParticleRadius(atof(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-max_failed"))
    {
        setMaxFailed(static_cast<unsigned int>(atoi(argv[i + 1])));
    }
    else if (!strcmp(argv[i], "-inflowParticleRadiusRange"))
    {
        setInflowParticleRadius(atof(argv[i + 1]), atof(argv[i + 2]));
        i++;
    }
    else if (!strcmp(argv[i], "-inflowParticleRadius"))
    {
        setInflowParticleRadius(atof(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-roughBottomType"))
    {
        std::string str(argv[i + 1]);
        setRoughBottomType(str);
    }
//    else if (!strcmp(argv[i], "-k_eps"))
//    {
//        Mdouble Mass = getLightestParticleMass();
//        //~ Mdouble Mass =  particleHandler.get_LightestParticle()->getMass();
//        speciesHandler.getObject(0)->setStiffnessAndRestitutionCoefficient(atof(argv[i + 1]), atof(argv[i + 2]), Mass);
//        std::cout << "reset contact properties of lightest Particle (mass=" << Mass << ") to k=" << speciesHandler.getObject(0)->getStiffness() << " and dissipation_=" << speciesHandler.getObject(0)->getDissipation() << std::endl;
//        i += 1;
//    }
//    else if (!strcmp(argv[i], "-tc_eps"))
//    {
//        Mdouble Mass = getLightestParticleMass();
//        speciesHandler.getObject(0)->setCollisionTimeAndRestitutionCoefficient(atof(argv[i + 1]), atof(argv[i + 2]), Mass);
//        std::cout << "reset contact properties of lightest Particle (mass=" << Mass << ") to k=" << speciesHandler.getObject(0)->getStiffness() << " and dissipation_=" << speciesHandler.getObject(0)->getDissipation() << std::endl;
//        i += 1;
//    }
//    else if (!strcmp(argv[i], "-tc_eps_beta"))
//    {
//        Mdouble Mass = getLightestParticleMass();
//        FrictionalSpecies* S = dynamic_cast<FrictionalSpecies*>(speciesHandler.getObject(0));
//        S->setCollisionTimeAndNormalAndTangentialRestitutionCoefficient(atof(argv[i + 1]), atof(argv[i + 2]), atof(argv[i + 3]), Mass);
//        std::cout << "reset contact properties of lightest Particle (mass=" << Mass << ") to k=" << S->getStiffness() << ", dissipation_=" << S->getDissipation() << ", kt=" << S->getSlidingStiffness() << " and dispt=" << S->getSlidingDissipation() << std::endl;
//        i += 2;
//    }
    else
        return Mercury3D::readNextArgument(i, argc, argv); //if argv[i] is not found, check the commands in Mercury3D
    return true; //returns true if argv[i] is found
}

void Chute::makeChutePeriodic()
{
    isChutePeriodic_ = true;
}

bool Chute::getIsPeriodic() const
{
    return isChutePeriodic_;
}

void Chute::setFixedParticleRadius(Mdouble fixedParticleRadius)
{
    if (fixedParticleRadius >= 0.0)
        fixedParticleRadius_ = fixedParticleRadius;
    else
        std::cerr << "WARNING : Fixed particle radius must be greater than or equal to zero" << std::endl;
}
Mdouble Chute::getFixedParticleRadius() const
{
    return fixedParticleRadius_;
}

void Chute::setRoughBottomType(RoughBottomType roughBottomType)
{
    roughBottomType_ = roughBottomType;
}

void Chute::setRoughBottomType(std::string roughBottomTypeString)
{
    if (!roughBottomTypeString.compare("MONOLAYER_ORDERED"))
    {
        roughBottomType_ = MONOLAYER_ORDERED;
    }
    else if (!roughBottomTypeString.compare("MONOLAYER_DISORDERED"))
    {
        roughBottomType_ = MONOLAYER_ORDERED;
    }
    else if (!roughBottomTypeString.compare("MULTILAYER"))
    {
        roughBottomType_ = MONOLAYER_ORDERED;
    }
    else
    {
        logger.log(Log::FATAL, "wrong argument in setRoughBottomType. It is %", roughBottomTypeString);
    }
}

RoughBottomType Chute::getRoughBottomType() const
{
    return roughBottomType_;
}

void Chute::setChuteAngle(Mdouble chuteAngle)
{
    Mdouble gravity = getGravity().getLength();
    if (gravity == 0)
    {
        std::cerr << "WARNING: zero gravity";
    }
    setChuteAngleAndMagnitudeOfGravity(chuteAngle, gravity);
}

void Chute::setChuteAngleAndMagnitudeOfGravity(Mdouble chuteAngle, Mdouble gravity)
{
    if (chuteAngle >= -90.0 && chuteAngle <= 90.0)
    {
        chuteAngle_ = chuteAngle * constants::pi / 180.0;
        setGravity(Vec3D(sin(chuteAngle_), 0.0, -cos(chuteAngle_)) * gravity);
    }
    else
        std::cerr << "WARNING : Chute angle must be within [-90,90]" << std::endl;
}

Mdouble Chute::getChuteAngle() const
{
    return chuteAngle_;
}

Mdouble Chute::getChuteAngleDegrees() const
{
    return chuteAngle_ * 180.0 / constants::pi;
}

void Chute::setMaxFailed(unsigned int maxFailed)
{
    maxFailed_ = maxFailed;
}

unsigned int Chute::getMaxFailed() const
{
    return maxFailed_;
}

void Chute::setInflowParticleRadius(Mdouble InflowParticleRadius)
{
    if (InflowParticleRadius >= 0.0)
    {
        minInflowParticleRadius_ = maxInflowParticleRadius_ = InflowParticleRadius;
    }
    else
    {
        std::cerr << "WARNING : Inflow particle must be greater than or equal to zero" << std::endl;
    }
}

void Chute::setInflowParticleRadius(Mdouble minInflowParticleRadius, Mdouble maxInflowParticleRadius)
{
    if (minInflowParticleRadius >= 0.0)
    {
        minInflowParticleRadius_ = minInflowParticleRadius;
    }
    else
    {
        std::cerr << "WARNING : Min. inflow particle radius must be nonnegative" << std::endl;
    }
    if (maxInflowParticleRadius >= minInflowParticleRadius)
    {
        maxInflowParticleRadius_ = maxInflowParticleRadius;
    }
    else
    {
        std::cerr << "WARNING : Max. inflow particle radius must be >= min. inflow particle radius" << std::endl;
    }
}

void Chute::setMinInflowParticleRadius(Mdouble minInflowParticleRadius)
{
    if (minInflowParticleRadius <= maxInflowParticleRadius_)
    {
        minInflowParticleRadius_ = minInflowParticleRadius;
    }
    else
    {
        std::cerr << "WARNING : Max. inflow particle radius must be >= min. inflow particle radius" << std::endl;
    }
}

void Chute::setMaxInflowParticleRadius(Mdouble maxInflowParticleRadius)
{
    if (maxInflowParticleRadius >= minInflowParticleRadius_)
    {
        maxInflowParticleRadius_ = maxInflowParticleRadius;
    }
    else
    {
        std::cerr << "WARNING : Max. inflow particle radius must be >= min. inflow particle radius" << std::endl;
    }
}

Mdouble Chute::getInflowParticleRadius() const
{
    return 0.5 * (minInflowParticleRadius_ + maxInflowParticleRadius_);
}

const Mdouble Chute::getMinInflowParticleRadius() const
{
    return minInflowParticleRadius_;
}

const Mdouble Chute::getMaxInflowParticleRadius() const
{
    return maxInflowParticleRadius_;
}

void Chute::setInflowHeight(Mdouble inflowHeight)
{
    if (inflowHeight >= minInflowParticleRadius_ + maxInflowParticleRadius_)
    {
        inflowHeight_ = inflowHeight;
        setZMax(1.2 * inflowHeight_);
    }
    else
    {
        std::cerr << "WARNING : Inflow height not changed to " << inflowHeight << ", value must be greater than or equal to diameter of inflow particle" << std::endl;
    }
}

Mdouble Chute::getInflowHeight() const
{
    return inflowHeight_;
}

void Chute::setInflowVelocity(Mdouble inflowVelocity)
{
    if (inflowVelocity >= 0.0)
    {
        inflowVelocity_ = inflowVelocity;
    }
    else
    {
        std::cerr << "WARNING : Inflow velocity not changed, value must be greater than or equal to zero" << std::endl;
    }
}

Mdouble Chute::getInflowVelocity() const
{
    return inflowVelocity_;
}

void Chute::setInflowVelocityVariance(Mdouble inflowVelocityVariance)
{
    if (inflowVelocityVariance >= 0.0 && inflowVelocityVariance <= 1.0)
    {
        inflowVelocityVariance_ = inflowVelocityVariance;
    }
    else
    {
        std::cerr << "WARNING : Inflow velocity variance not changed, value must be within [0,1]" << std::endl;
    }
}

Mdouble Chute::getInflowVelocityVariance() const
{
    return inflowVelocityVariance_;
}

void Chute::setChuteWidth(Mdouble chuteWidth)
{
    setYMax(chuteWidth);
}

Mdouble Chute::getChuteWidth() const
{
    return getYMax();
}

void Chute::setChuteLength(Mdouble chuteLength)
{
    setXMax(chuteLength);
}

Mdouble Chute::getChuteLength() const
{
    return getXMax();
}

void Chute::setInsertionBoundary(InsertionBoundary* insertionBoundary)
{
    insertionBoundary_ = insertionBoundary;
}
