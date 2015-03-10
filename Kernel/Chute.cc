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

#include <Logger.h>

/*!
 * \details Default constructor.
 */
Chute::Chute()
{
    constructor();
    logger(VERBOSE,"[Chute::Chute()] constructor finished");   
}

/*!
 * \details Copy constructor with a DPMBase object as an argument. This constructor
 * basically 'upgrades' a DPMBase object to one of the Chute class.
 * NB: The copy constructor of DPMBase has to be called because the link from DPMBase 
 * to MercuryBase is virtual.
 * \param[in] other     object of DPMBase class
 */
Chute::Chute(const DPMBase& other)
        : DPMBase(other), Mercury3D(other)
{
    constructor();
    logger(DEBUG,"[Chute::Chute(const DPMBase& other)] copy constructor finished");     
}

/*!
 * \details Copy constructor with a MercuryBase object as an argument. This constructor
 * basically 'upgrades' a MercuryBase object to one of the Chute class.
 * NB: The copy constructor of DPMBase has to be called because the link from DPMBase 
 * to MercuryBase is virtual.
 * \param[in] other     object of MercuryBase class
 */
Chute::Chute(const MercuryBase& other)
        : DPMBase(other), Mercury3D(other)
{
    constructor();
    logger(DEBUG,"[Chute::Chute(const MercuryBase& other)] copy constructor finished");
}

/*!
 * \details Copy constructor with a Mercury3D object as an argument. This constructor
 * basically 'upgrades' a Mercury3D object to one of the Chute class.
 * NB: The copy constructor of DPMBase has to be called because the link from DPMBase 
 * to MercuryBase is virtual.
 * \param[in] other     object of DPMBase class
 */
Chute::Chute(const Mercury3D& other)
        : DPMBase(other), Mercury3D(other)
{
        constructor();
        logger(DEBUG,"[Chute::Chute(const Mercury3D& other) copy constructor finished"); 
}

/*!
 * \details 'normal' copy constructor
 * \param[in] other     Chute object to be copied
 */
Chute::Chute(const Chute& other)
        : DPMBase(other), Mercury3D(other),
        chuteAngle_(other.chuteAngle_), fixedParticleRadius_(other.fixedParticleRadius_),
        minInflowParticleRadius_(other.minInflowParticleRadius_), maxInflowParticleRadius_(other.maxInflowParticleRadius_),
        inflowVelocity_(other.inflowVelocity_), inflowVelocityVariance_(other.inflowVelocityVariance_),
        inflowHeight_(other.inflowHeight_), roughBottomType_(other.roughBottomType_),
        maxFailed_(other.maxFailed_), insertionBoundary_(other.insertionBoundary_), 
        isChutePeriodic_(other.isChutePeriodic_)    
{
    logger(DEBUG,"[Chute::Chute(const Chute& other)] copy constructor finished");     
}

/*!
 * \details constructor METHOD, which sets all chute properties to something sensible.
 */
void Chute::constructor()
{
    insertionBoundary_ = nullptr;
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

/*!
 * \details Reads the object properties from an istream
 * \param[in,out] is    the istream
 */
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

/*!
 * \details Writes object's properties to an ostream
 * \param[in] os    the ostream
 */
///\todo I think this function is superseeded by write(std::ostream& os, bool writeAllParticles)
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

/*!
 * \details Writes object's properties to an ostream
 * \param[in] os                    the ostream
 * \param[in] writeAllParticles     If TRUE, the properties of ALL particles in the 
 *                                  particleHandler are written to the ostream. If 
 *                                  FALSE, only the properties of the first two 
 *                                  particles in the handler are written to the 
 *                                  ostream (see DPMBase::write(std::ostream& os, bool writeAllParticles)).                          
 */
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

/*!
 * \details Applies all necessary actions before the next time step. In this case, calls 
 * Chute::cleanChute().
 */
void Chute::actionsBeforeTimeStep()
{
    cleanChute();
}

/*!
 * \details Prints the current simulation time, the maximum simulation time, and 
 * the current number of particles in the chute.
 */
void Chute::printTime() const
{
    std::cout << "\rt=" << std::setprecision(3) << std::left << std::setw(6) << getTime()
            << ", tmax=" << std::setprecision(3) << std::left << std::setw(6) << getTimeMax()
            << ", N=" << std::setprecision(3) << std::left << std::setw(6) << particleHandler.getNumberOfObjects()
            << std::endl;
    std::cout.flush();
}

/*!
 * \details Adds side walls, a bottom and a particle insertion boundary
 */
void Chute::setupInitialConditions()
{
    if (speciesHandler.getNumberOfObjects()==0)
        logger(FATAL, "Chute % cannot complete because no species have been defined.", getName());
    
    // create the chute's side walls (which are solid if the chute is not periodic)
    setupSideWalls();
    
    // create a particle of which (altered) copies will fill the chute insertion boundary
    BaseParticle* p = new BaseParticle;
    // by default, insert particles of species 0
    p->setSpecies(speciesHandler.getObject(0));
    
    // set up the insertion boundary and add to handler
    ChuteInsertionBoundary b1;
    b1.set(p, maxFailed_, Vec3D(getXMin(), getYMin(), getZMin()), Vec3D(getXMax(), getYMax(), getZMax()), minInflowParticleRadius_, maxInflowParticleRadius_, fixedParticleRadius_, inflowVelocity_, inflowVelocityVariance_);
    insertionBoundary_ = boundaryHandler.copyAndAddObject(b1);

    //creates the bottom of the chute
    createBottom();
}

/*!
 * \details Create side walls (i.e., in the Y-direction), which can be either periodic 
 * (if isChutePeriodic_ is TRUE), or solid (if isChutePeriodic_ is FALSE).
 */
void Chute::setupSideWalls()
{
    // check if walls should be periodic or solid
    if (isChutePeriodic_)
    {
        // create a periodic boundary with walls at yMin_ and yMax_.
        PeriodicBoundary b0;
        b0.set(Vec3D(0.0, 1.0, 0.0), getYMin(), getYMax());
        boundaryHandler.copyAndAddObject(b0);
    }
    else
    {
        // create two infinite solid walls; one at yMin_...
        InfiniteWall w0;
        w0.set(Vec3D(0.0,-1.0, 0.0), Vec3D(0, getYMin(), 0));
        wallHandler.copyAndAddObject(w0);
        // ... and one at yMax_.
        w0.set(Vec3D(0.0, 1.0, 0.0), Vec3D(0, getYMax(), 0));
        wallHandler.copyAndAddObject(w0);
    }
}

/*!
 * \details Creates the bottom of the chute; either:
 *    * smooth:     fixedParticleRadius_ smaller than 1e-12,
 *   monolayered:
 *    * grid-like:  roughBottomType_ == MONOLAYER_ORDERED,
 *    * random:     roughBottomType_ == MONOLAYER_DISORDERED, or
 * 
 *    * multilayer: 'else' (roughBottomType_ == MULTILAYER), with by default contains
 *      all particles with a center within a depth band of 2.4 (= ChuteBottom::thickness_) * maxInflowParticleRadius_.
 */
void Chute::createBottom()
{
    
    if (fabs(getFixedParticleRadius()) < 1e-12) // smooth bottom
    {
        // flat wall as bottom
        logger(INFO,"create perfectly flat chute bottom");
        
        //bottom wall 
        InfiniteWall w0;
        w0.set(Vec3D(0.0, 0.0, -1.0), Vec3D(0, 0, getZMin()));
        wallHandler.copyAndAddObject(w0);
    }
    else //rough bottom
    {
        // Define standard fixed particle
        /*!
         * \todo Does the bottom we always has to be this particle?
         * Maybe add a BaseParticle* argument, and add a default value with 
         * particle radius < 1e-12.
         */
        BaseParticle F0;
        F0.setHandler(&particleHandler);
        F0.setRadius(getFixedParticleRadius());
        F0.setPosition(Vec3D(0.0, 0.0, 0.0));
        
        
        
        if (roughBottomType_ == MONOLAYER_ORDERED)
        {
            // grid-like fixed-particle bottom
            logger(INFO,"create monolayered, ordered rough chute bottom");
            
            // allowed space for each particle in each direction
            Mdouble dx = 2.0 * F0.getRadius();
            Mdouble dy = 2.0 * F0.getRadius();
            
            // number of particles that fit in each direction
            unsigned int nx = static_cast<unsigned int>(std::max(1, static_cast<int>(std::floor((getXMax() - getXMin()) / dx))));
            unsigned int ny = static_cast<unsigned int>(std::max(1, static_cast<int>(std::floor((getYMax() - getYMin()) / dy))));
            
            // adjust particle spacing (in case total space available in given direction
            // is not a multiple of 2*F0.getRadius() )
            dx = (getXMax() - getXMin()) / nx;
            dy = (getYMax() - getYMin()) / ny;
            
            for (unsigned int i = 0; i < nx; i++)
            {
                for (unsigned int j = 0; j < ny; j++)
                {
                    // placing of particles on rectangular grid points 
                    F0.setPosition(Vec3D(F0.getRadius() + dx * i, F0.getRadius() + dy * j, 0.0));
                    particleHandler.copyAndAddObject(F0);
                }
            }
        }
        else if (roughBottomType_ == MONOLAYER_DISORDERED)
        { 
            // random fixed-particle bottom
            logger(INFO,"create monolayered disordered rough chute bottom");

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

            //bottom wall (create after particle creation, as checkParticleForInteraction also checks against walls)
            InfiniteWall w0;
            w0.set(Vec3D(0.0, 0.0, -1.0), Vec3D(0, 0, getZMin() - .5 * F0.getRadius()));
            wallHandler.copyAndAddObject(w0);
        }
        else //if (roughBottomType_ == MULTILAYER)
        {
            // multilayered particle bottom
            logger(INFO,"create multilayered rough chute bottom");
            
            //'this' points to the current Chute object, the class of which is inherited
            // by the ChuteBottom class. I.e., the bottom is created with the particle 
            // properties from the current class.
            // ChuteBottom::makeRoughBottom() creates a randomly filled, multilayered
            // chute bottom. It 
            ChuteBottom bottom(*this);
            bottom.setInflowParticleRadius(getFixedParticleRadius());
            bottom.makeRoughBottom(*this);
        }
        //finally, fix particles to the floor
        for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it != particleHandler.end(); ++it)
            (*it)->fixParticle();
    }
}

/*!
 * \details Removes all particles which are outside the horizontal window,
 * i.e. all particles with an x-direction position greater than xMax_ or smaller
 * than xMin_. The removal operation is performed once every 100 time steps.
 *  
 */
void Chute::cleanChute()
{
    //clean outflow every 100 timesteps
    static int count = 0, maxcount = 100; // n00b remark: static variables are only initialised once, and their values
                                          // are stored even after the method returns. I.e., next time the method is 
                                          // called, the initialisation is ignored and the previously assigned value is used.
    if (count > maxcount)
    {
        // reset counter
        count = 0;
        
        // check all particles
        for (unsigned int i = 0; i < particleHandler.getNumberOfObjects();)
        {
            // check if particle is outside the problem window
            if (particleHandler.getObject(i)->getPosition().X > getXMax() || particleHandler.getObject(i)->getPosition().X < getXMin()) //||particleHandler.getObject(i)->Position.Z+particleHandler.getObject(i)->Radius<zMin_)    
            {
                // if so, delete the particle
                logger(DEBUG,"[Chute::cleanChute()] erased: %", particleHandler.getObject(i));
                particleHandler.removeObject(i);
            }
            else
                i++;
        }
    }
    else
        count++;
}

/*!
 * \details this reads chute parameters from a string.
 * \param[in] i         the index of the input parameter to be read
 * \param[in] argc      number of input parameters
 * \param[in] argv[]    pointer to the (first character of the) actual string
 */
bool Chute::readNextArgument(int& i, int argc, char* argv[])
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

/*!
 * \details Sets the side walls (i.e. the walls in the Y-direction) of the chute 
 * to be made periodic
 */
void Chute::makeChutePeriodic()
{
    isChutePeriodic_ = true;
}

/*!
 * \details Returns isChutePeriodic_, which is TRUE if the side walls (i.e. the 
 * walls in the Y-direction) of the chute are periodic
 * 
 */
bool Chute::getIsPeriodic() const
{
    return isChutePeriodic_;
}

/*!
 * \details Sets the radius of the fixed particles at the bottom (in case of a rough
 * bottom; see also Chute::createBottom() ).
 */
void Chute::setFixedParticleRadius(Mdouble fixedParticleRadius)
{
    if (fixedParticleRadius >= 0.0)
        fixedParticleRadius_ = fixedParticleRadius;
    else
        logger(WARN,"[Chute::setFixedParticleRadius()] Fixed particle radius must be greater than or equal to zero.");
}

/*!
 * \details Returns the radius of the fixed particles at the bottom (in case of a rough
 * bottom; see also Chute::createBottom() ). 
 */
Mdouble Chute::getFixedParticleRadius() const
{
    return fixedParticleRadius_;
}

/*!
 * \details Sets the roughBottomType_. Possible choices are defined by the enum
 * RoughBottomType, which is defined in Chute.h:
 *    * MONOLAYER_ORDERED:      Bottom will be a rectangularly ordered monolayer of particles
 *    * MONOLAYER_DISORDERED:   Bottom will be a disordered monolayer of particles
 *    * MULTILAYER:             Bottom will be a multilayer of particles
 * See also the documentation of Chute::createBottom().
 * \param[in] roughBottomType   The bottom type to be set
 */
void Chute::setRoughBottomType(RoughBottomType roughBottomType)
{
    roughBottomType_ = roughBottomType;
}

/*!
 * \details Same as Chute::setRoughBottomType(RoughBottomType roughBottomType), 
 * but takes a string identical to the RoughBottomType as an argument instead.
 */
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
        logger(FATAL, "[Chute::setRoughBottomType(std::string)] Invalid argument in setRoughBottomType. Given: %", roughBottomTypeString);
    }
}

/*!
 * \details Returns the roughBottomType_, which determines the type of rough bottom
 * of the chute.
 */
RoughBottomType Chute::getRoughBottomType() const
{
    return roughBottomType_;
}

/*!
 * \details Sets the angle of the chute as compared to the horizontal (i.e., the normal to gravity).
 * NB: since the X and Z directions are defined to be parallel and normal to the chute bottom,
 * respectively, this function effectively sets the GRAVITY angle.
 * \param[in] chuteAngle    the angle of the chute relative to the horizontal plane in DEGREES
 */
void Chute::setChuteAngle(Mdouble chuteAngle)
{   
    // retrieve the magnitude of gravity
    Mdouble gravity = getGravity().getLength();
    if (gravity == 0)
    {
        logger(WARN,"[Chute::setChuteAngle()] zero gravity");
    }
    
    // reset the gravity vector, with the given angle
    setChuteAngleAndMagnitudeOfGravity(chuteAngle, gravity);
}

/*!
 * \details Sets the angle of the chute as compared to the horizontal (i.e., the normal to gravity),
 * as well as the magnitude of gravity.
 * NB: since the X and Z directions are defined to be parallel and normal to the chute bottom,
 * respectively, this function effectively sets the GRAVITY angle.
 * \param[in] chuteAngle    the angle of the chute relative to the horizontal plane in DEGREES
 * \param[in] gravity       magnitude of the gravity vector
 */
void Chute::setChuteAngleAndMagnitudeOfGravity(Mdouble chuteAngle, Mdouble gravity)
{
    if (chuteAngle >= -90.0 && chuteAngle <= 90.0)
    {
        chuteAngle_ = chuteAngle * constants::pi / 180.0;
        setGravity(Vec3D(sin(chuteAngle_), 0.0, -cos(chuteAngle_)) * gravity);
    }
    else
        logger(WARN,"[Chute::setChuteAngleAndMagnitudeOfGravity()] Chute angle must be within [-90,90]");
}

/*!
 * \details Returns the chute angle (as compared to the horizontal plane) in RADIANS
 * \return  the chute angle in RADIANS
 */
Mdouble Chute::getChuteAngle() const
{
    return chuteAngle_;
}

/*!
 * \details Returns the chute angle (as compared to the horizontal plane) in DEGREES
 * \return  the chute angle in DEGREES (for writing to the terminal/an output stream)
 */
Mdouble Chute::getChuteAngleDegrees() const
{
    return chuteAngle_ * 180.0 / constants::pi;
}

/*!
 * \brief Sets the number of times that the boundary may fail to insert a particle,
 * before the boundary is considered filled.
 */
void Chute::setMaxFailed(unsigned int maxFailed)
{
    maxFailed_ = maxFailed;
}

/*!
 * \details Returns the number of times that the boundary may fail to insert a particle,
 * before the boundary is considered filled.
 */
unsigned int Chute::getMaxFailed() const
{
    return maxFailed_;
}

/*!
 * \details Sets the radius of particles flowing in at the start of the chute to 
 * be constant and the given one (i.e., the minimum and maximum allowed radius are set 
 * equal to each other and the argument).
 * \param[in] inflowParticleRadius  the radius of the particles entering the chute
 */
void Chute::setInflowParticleRadius(Mdouble inflowParticleRadius)
{
    if (inflowParticleRadius >= 0.0)
    {
        minInflowParticleRadius_ = maxInflowParticleRadius_ = inflowParticleRadius;
    }
    else
    {
        logger(WARN, "[Chute::setInflowParticleRadius(Mdouble)] Inflow particle must be greater than or equal to zero");
    }
}

/*!
 * \details Sets the minimum and maximum radius possible of particles entering the chute.
 * \param[in] minInflowParticleRadius       the minimum radius
 * \param[in] maxInflowParticleRadius       the maximum radius
 */
void Chute::setInflowParticleRadius(Mdouble minInflowParticleRadius, Mdouble maxInflowParticleRadius)
{
    if (minInflowParticleRadius >= 0.0)
    {
        minInflowParticleRadius_ = minInflowParticleRadius;
    }
    else
    {
        logger(WARN, "[Chute::setInflowParticleRadius(Mdouble,Mdouble)] Min. inflow particle radius must be nonnegative");
    }
    if (maxInflowParticleRadius >= minInflowParticleRadius)
    {
        maxInflowParticleRadius_ = maxInflowParticleRadius;
    }
    else
    {
        logger(WARN, "[Chute::setInflowParticleRadius(Mdouble,Mdouble)] Max. inflow particle radius must be >= min. inflow particle radius");
    }
}

/*!
 * \details Sets the minimum radius possible of particles entering the chute.
 * \param[in] minInflowParticleRadius       the minimum radius
 */
void Chute::setMinInflowParticleRadius(Mdouble minInflowParticleRadius)
{
    if (minInflowParticleRadius <= maxInflowParticleRadius_)
    {
        minInflowParticleRadius_ = minInflowParticleRadius;
    }
    else
    {
        logger(WARN,"[Chute::setMinInflowParticleRadius()] Min. inflow particle radius must be <= max. inflow particle radius");
    }
}

/*!
 * \details Sets the maximum radius possible of particles entering the chute.
 * \param[in] maxInflowParticleRadius       the maximum radius
 */
void Chute::setMaxInflowParticleRadius(Mdouble maxInflowParticleRadius)
{
    if (maxInflowParticleRadius >= minInflowParticleRadius_)
    {
        maxInflowParticleRadius_ = maxInflowParticleRadius;
    }
    else
    {
        logger(WARN,"[Chute::setMaxInflowParticleRadius()] Max. inflow particle radius must be >= min. inflow particle radius");
    }
}

/*!
 * \details Returns the mean of the min. and max. inflow particle radius.
 * \return the mean of the min. and max. inflow particle radius.
 */
Mdouble Chute::getInflowParticleRadius() const
{
    return 0.5 * (minInflowParticleRadius_ + maxInflowParticleRadius_);
}

/*!
 * \details returns the minimum inflow particle radius
 * \return  the minimum inflow particle radius
 */
Mdouble Chute::getMinInflowParticleRadius() const
{
    return minInflowParticleRadius_;
}

/*!
 * \details Returns the maximum inflow particle radius
 * \return the maximum inflow particle radius
 */
Mdouble Chute::getMaxInflowParticleRadius() const
{
    return maxInflowParticleRadius_;
}

/*!
 * \details Sets the maximum height in Z-direction at which particles are introduced
 * into the chute.
 * \param[in] inflowHeight  the maximum inflow height to be set
 */
void Chute::setInflowHeight(Mdouble inflowHeight)
{
    //if (inflowHeight >= minInflowParticleRadius_ + maxInflowParticleRadius_)
    {
        inflowHeight_ = inflowHeight;
        setZMax(1.2 * inflowHeight_);
    }
//    else
//    {
//        std::cerr << "WARNING : Inflow height not changed to " << inflowHeight << ", value must be greater than or equal to diameter of inflow particle" << std::endl;
//    }
}

/*!
 * \details Returns the maximum height at which particles are introduced into the chute
 * \return  the inflowHeight_
 */
Mdouble Chute::getInflowHeight() const
{
    return inflowHeight_;
}

/*!
 * \details Sets the mean velocity at which the particles enter the chute
 * \param[in] inflowVelocity    the mean velocity at which the particles enter the chute
 */
void Chute::setInflowVelocity(Mdouble inflowVelocity)
{
    if (inflowVelocity >= 0.0)
    {
        inflowVelocity_ = inflowVelocity;
    }
    else
    {
        logger(WARN,"[Chute::setInflowVelocity()] Inflow velocity not changed, value must be greater than or equal to zero");
    }
}

/*!
 * \details Returns the mean velocity at which the particles enter the chute
 * \return    the mean velocity at which the particles enter the chute
 */
Mdouble Chute::getInflowVelocity() const
{
    return inflowVelocity_;
}

/*!
 * \details Sets the variance of the velocities of the inflow particles around the mean
 * \param[in] inflowVelocityVariance    the variance of the velocities of the inflow particles around the 
 *                                      mean (expressed in ratio of the mean, i.e. the inflowVelocity_).
 */
void Chute::setInflowVelocityVariance(Mdouble inflowVelocityVariance)
{
    if (inflowVelocityVariance >= 0.0 && inflowVelocityVariance <= 1.0)
    {
        inflowVelocityVariance_ = inflowVelocityVariance;
    }
    else
    {
        logger(ERROR,"[Chute::setInflowVelocityVariance()] Inflow velocity variance not changed, value must be within [0,1]");
    }
}

/*!
 * \details Returns the variance of the velocities of the inflow particles around the mean
 * \return      the variance of the velocities of the inflow particles around the 
 *              mean (expressed in ratio of the mean, i.e. the inflowVelocity_).
 */
Mdouble Chute::getInflowVelocityVariance() const
{
    return inflowVelocityVariance_;
}

/*!
 * \details Sets the width of the chute. Actually sets yMax_, while it assumes 
 * yMin_ to have stayed 0.
 * \param[in] chuteWidth    width of the chute
 */
void Chute::setChuteWidth(Mdouble chuteWidth)
{
    setYMax(chuteWidth);
}

/*!
 * \details Returns the width of the chute. Actually returns yMax_, while it assumes 
 * yMin_ to have stayed 0.
 * \return      width of the chute
 */
Mdouble Chute::getChuteWidth() const
{
    return getYMax();
}

/*!
 * \details Sets the length of the chute. Actually sets xMax_, while xMin_ is assumed
 * to have stayed 0.
 * \param[in] chuteLength   length of the chute
 */
void Chute::setChuteLength(Mdouble chuteLength)
{
    setXMax(chuteLength);
}

/*!
 * \details Returns the length of the chute. Actually returns xMax_, while xMin_ is assumed
 * to have stayed 0.
 * \return      length of the chute
 */
Mdouble Chute::getChuteLength() const
{
    return getXMax();
}

/*!
 * \details Sets the insertion boundary of the chute.
 * \param[in] insertionBoundary     the insertion boundary of the chute
 */
void Chute::setInsertionBoundary(InsertionBoundary* insertionBoundary)
{
    insertionBoundary_ = insertionBoundary;
}
