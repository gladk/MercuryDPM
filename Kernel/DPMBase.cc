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
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <limits>
#include <string>
#include <sstream>
#include <cstdio>
///todo strcmp relies on this, should be changed to more modern version
#include <cstring>
//This is only used to change the file permission of the xball script create, at some point this code may be moved from this file to a different file.
#include <sys/types.h>
#include <sys/stat.h>
#include <Species/LinearViscoelasticSlidingFrictionSpecies.h>
#include "Interactions/Interaction.h"

#include "Species/Species.h"
#include "Species/LinearViscoelasticSpecies.h"
#include "Species/FrictionForceSpecies/SlidingFrictionSpecies.h"

//This is part of this class and just separates out the stuff to do with xballs.
#include "CMakeDefinitions.h"
#include "DPMBaseXBalls.icc"
#include "Logger.h"
#include "Particles/BaseParticle.h"
#include "Walls/BaseWall.h"
#include "Walls/InfiniteWall.h"
#include "Boundaries/PeriodicBoundary.h"

/*!
 * \param[in] module
 * \param[in] message
 */
void logWriteAndDie(std::string module, std::string message)
{
    std::cerr << "A fatal   error has occured"
            << "\n Module  :" << module
            << "\n Message :" << message << std::endl;
    
    std::exit(-1);
}
/*!
 * \param[in] os
 * \param[in] md
 */
std::ostream& operator<<(std::ostream& os, DPMBase &md)
{
    md.write(os);
    return os;
}
/*!
 * \param[in] other
 */
DPMBase::DPMBase(const FilesAndRunNumber& other)
        : FilesAndRunNumber(other)
{
    constructor();
}

/*!
 * \param[in] other
 */
DPMBase::DPMBase(const DPMBase& other)
{
    systemDimensions_ = other.systemDimensions_;
    particleDimensions_ = other.particleDimensions_;
    gravity_ = other.gravity_;
    xMin_ = other.xMin_;
    xMax_ = other.xMax_;
    yMin_ = other.yMin_;
    yMax_ = other.yMax_;
    zMin_ = other.zMin_;
    zMax_ = other.zMax_;
    time_ = other.time_;
    timeStep_ = other.timeStep_;
    ntimeSteps_ = other.ntimeSteps_;
    timeMax_ = other.timeMax_;
    restartVersion_ = other.restartVersion_; //to read new and old restart data
    restarted_ = other.restarted_; //to see if it was restarted or not
    append_ = other.append_;
    rotation_ = other.rotation_;
    xBallsColourMode_ = other.xBallsColourMode_; // sets the xballs argument cmode (see xballs.txt)
    xBallsVectorScale_ = other.xBallsVectorScale_; // sets the xballs argument vscale (see xballs.txt)
    xBallsScale_ = other.xBallsScale_; // sets the xballs argument scale (see xballs.txt)
    xBallsAdditionalArguments_ = other.xBallsAdditionalArguments_; // std::string where additional xballs argument can be specified (see xballs.txt)
#ifdef CONTACT_LIST_HGRID
            possibleContactList=other.possibleContactList;
#endif
    random = other.random;

    boundaryHandler.setDPMBase(this);
    particleHandler.setDPMBase(this);
    interactionHandler.setDPMBase(this);
    speciesHandler.setDPMBase(this);
    wallHandler.setDPMBase(this);

    speciesHandler = other.speciesHandler;
    particleHandler = other.particleHandler;
    wallHandler = other.wallHandler;
    boundaryHandler = other.boundaryHandler;
    interactionHandler = other.interactionHandler;
}
/*!
 * 
 */
DPMBase::DPMBase()
{
    constructor();
}
/*!
 * 
 */
DPMBase::~DPMBase()
{

}
/*!
 * \param[in] argc
 * \param[in] argv
 */
void DPMBase::solve(int argc, char* argv[])
{
    readArguments(argc, argv);
    solve();
}
/*!
 * \return time_
 */
Mdouble DPMBase::getTime() const
{
    return time_;
}
/*!
 * \return ntimeSteps_
 */
unsigned int DPMBase::getNtimeSteps() const
{
    return ntimeSteps_;
}
/*!
 * \param[in] time
 */
void DPMBase::setTime(Mdouble time)
{
    time_ = time;
}
/*!
 * \param[in] newTMmax
 */
void DPMBase::setTimeMax(Mdouble newTMax)
{
    if (newTMax >= 0)
    {
        timeMax_ = newTMax;
    }
    else
    {
        std::cerr << "Error in setTimeMax, newTMax=" << newTMax << std::endl;
        exit(-1);
    }
}
/*!
 * \return timeMax_ 
 */
Mdouble DPMBase::getTimeMax() const
{
    return timeMax_;
}
/*!
 *
 */
#ifdef CONTACT_LIST_HGRID
PossibleContactList& DPMBase::getPossibleContactList()
{   
    return possibleContactList;
}
#endif
/*!
 * \param[in] newRotFlag
 */
void DPMBase::setRotation(bool newRotFlag)
{
    rotation_ = newRotFlag;
}
/*!
 * \returns bool (True or False)
 */
bool DPMBase::getRotation() const
{
    return rotation_;
}
/*!
 * \returns xMin_
 */
Mdouble DPMBase::getXMin() const
{
    return xMin_;
}
/*!
 * \returns xMax_
 */
Mdouble DPMBase::getXMax() const
{
    return xMax_;
}
/*!
 * \returns yMin_
 */
Mdouble DPMBase::getYMin() const
{
    return yMin_;
}
/*!
 * \returns yMax_
 */
Mdouble DPMBase::getYMax() const
{
    return yMax_;
}
/*!
 * \returns zMin_
 */
Mdouble DPMBase::getZMin() const
{
    return zMin_;
}
/*!
 * \returns zMax_
 */
Mdouble DPMBase::getZMax() const
{
    return zMax_;
}
/*!
 * \param[in] newXMin
 */
void DPMBase::setXMin(Mdouble newXMin)
{
    if (newXMin <= getXMax())
    {
        xMin_ = newXMin;
    }
    else
    {
        std::cerr << "Warning in setXMin(" << newXMin << "): xMax=" << getXMax() << std::endl;
    }
}
/*!
 * \param[in] newYMin
 */
void DPMBase::setYMin(Mdouble newYMin)
{
    if (newYMin <= getYMax())
    {
        yMin_ = newYMin;
    }
    else
    {
        std::cerr << "Error in setYMin(" << newYMin << "): yMax=" << getYMax() << std::endl;
        exit(-1);
    }
}
/*!
 * \param[in] newZMin
 */
void DPMBase::setZMin(Mdouble newZMin)
{
    if (newZMin <= getZMax())
    {
        zMin_ = newZMin;
    }
    else
    {
        std::cerr << "Warning in setZMin(" << newZMin << "): zMax=" << getZMax() << std::endl;
    }
}
/*!
 * \param[in] newXMax
 */
void DPMBase::setXMax(Mdouble newXMax)
{
    if (newXMax >= getXMin())
    {
        xMax_ = newXMax;
    }
    else
    {
        std::cerr << "Error in setXMax(" << newXMax << "): xMin=" << getXMin() << std::endl;
        exit(-1);
    }
}
/*!
 * \param[in] newYMax
 */
void DPMBase::setYMax(Mdouble newYMax)
{
    if (newYMax >= getYMin())
    {
        yMax_ = newYMax;
    }
    else
    {
        std::cerr << "Warning in setYMax(" << newYMax << "): yMin=" << getYMin() << std::endl;
    }
}
/*!
 * \param[in] newZMax
 */
void DPMBase::setZMax(Mdouble newZMax)
{
    if (newZMax >= getZMin())
    {
        zMax_ = newZMax;
    }
    else
    {
        std::cerr << "Error in setZMax(" << newZMax << "): zMin=" << getZMin() << std::endl;
        exit(-1);
    }
}
/*!
 * \param[in] timeStep
 */
void DPMBase::setTimeStep(Mdouble timeStep)
{
    if (timeStep >= 0.0)
    {
        timeStep_ = timeStep;
    }
    else
    {
        std::cerr << "Error in setTimeStep" << std::endl;
        exit(-1);
    }
}
/*!
 * \return timeStep_
 */
Mdouble DPMBase::getTimeStep() const
{
    return timeStep_;
}
/*!
 * \param[in] newCMode
 */
void DPMBase::setXBallsColourMode(int newCMode)
{
    xBallsColourMode_ = newCMode;
}
/*!
 * \return int xBallsColourMode_ 
 */
int DPMBase::getXBallsColourMode() const
{
    return xBallsColourMode_;
}
/*!
 * \param[in] newVScale
 */
void DPMBase::setXBallsVectorScale(double newVScale)
{
    xBallsVectorScale_ = newVScale;
}
/*!
 * \return double xBallsVectorScale_
 */
double DPMBase::getXBallsVectorScale() const
{
    return xBallsVectorScale_;
}
/*!
 * \param[in] newXBArgs
 */
void DPMBase::setXBallsAdditionalArguments(std::string newXBArgs)
{
    xBallsAdditionalArguments_ = newXBArgs.c_str();
}
/*!
 * \return xBallsAdditionalArguments_
 */
std::string DPMBase::getXBallsAdditionalArguments() const
{
    return xBallsAdditionalArguments_;
}
/*!
 * \param[in] newScale
 */
void DPMBase::setXBallsScale(Mdouble newScale)
{
    xBallsScale_ = newScale;
}
/*!
 * \return double xBallsScale_
 */
double DPMBase::getXBallsScale() const
{
    return xBallsScale_;
}
/*!
 * \param[in] newGravity
 */
void DPMBase::setGravity(Vec3D newGravity)
{
    gravity_ = newGravity;
}
/*!
 * \return Vec3D gravity_
 */
Vec3D DPMBase::getGravity() const
{
    return gravity_;
}
/*!
 * \param[in] newDim
 */
void DPMBase::setDimension(unsigned int newDim)
{
    setSystemDimensions(newDim);
    setParticleDimensions(newDim);
}
/*!
 * \param[in] newDim
 */
void DPMBase::setSystemDimensions(unsigned int newDim)
{
    if (newDim >= 1 && newDim <= 3)
        systemDimensions_ = newDim;
    else
    {
        std::cerr << "Error in setSystemDimensions" << std::endl;
        exit(-1);
    }
}
/*!
 * \return systemDimensions_
 */
unsigned int DPMBase::getSystemDimensions() const
{
    return systemDimensions_;
}

/*!
 * \param[in] particleDimensions
 */
void DPMBase::setParticleDimensions(unsigned int particleDimensions)
{
    if (particleDimensions >= 1 && particleDimensions <= 3)
    {
        particleDimensions_ = particleDimensions;
        particleHandler.computeAllMasses();
    }
    else
    {
        std::cerr << "Error in setParticleDimensions" << std::endl;
        exit(-1);
    }
}

/*!
 * \return particleDimensions_
 */

unsigned int DPMBase::getParticleDimensions() const
{
    return particleDimensions_;
}

/*!
 * \return restartVersion_
 */

std::string DPMBase::getRestartVersion() const
{
    return restartVersion_;
}

/*!
 * \param[in] newRV
 */

void DPMBase::setRestartVersion(std::string newRV)
{
    restartVersion_ = newRV;
}

/*!
 * \return restarted_
 */

bool DPMBase::getRestarted() const
{
    return restarted_;
}

/*!
 * \param[in] newRestartedFlag
 */
void DPMBase::setRestarted(bool newRestartedFlag)
{
    restarted_ = newRestartedFlag;
    //setAppend(new_);
}

/*!
 * \return bool (True or False)
 */
bool DPMBase::getAppend() const
{
    return append_;
}

/*!
 * \param[in] newAppendFlag
 */
void DPMBase::setAppend(bool newAppendFlag)
{
    append_ = newAppendFlag;
}

/*!
 * \return Mdouble elasticEnergy
 */
Mdouble DPMBase::getElasticEnergy() const
{
    Mdouble elasticEnergy = 0.0;
    for (std::vector<BaseInteraction*>::const_iterator it = interactionHandler.begin(); it != interactionHandler.end(); ++it)
        elasticEnergy += (*it)->getElasticEnergy();
    return elasticEnergy;
}

/*!
 * \return Mdouble kineticEnergy
 */
Mdouble DPMBase::getKineticEnergy() const
{
    Mdouble kineticEnergy = 0;
    for (std::vector<BaseParticle*>::const_iterator it = particleHandler.begin(); it != particleHandler.end(); ++it)
    {
        if (!(*it)->isFixed())
        {
            kineticEnergy += .5 * (*it)->getMass() * (*it)->getVelocity().getLengthSquared();
        }
    }
    return kineticEnergy;

}

/*!
 * \return double 
 */
double DPMBase::getInfo(const BaseParticle& P) const
{
    return P.getSpecies()->getId(); // was getIndex()
}

/*!
 * \param[in] pI
 * \param[in] pJ
 * \return bool (True or False)
 */
bool DPMBase::areInContact(const BaseParticle* pI, const BaseParticle* pJ) const
{
    return pI != pJ && Vec3D::getDistanceSquared(pI->getPosition(), pJ->getPosition()) < mathsFunc::square(pI->getInteractionRadius() + pJ->getInteractionRadius());
}

/*!
 * \brief no implementation but can be overriden in its derived classes.
 */
void DPMBase::actionsBeforeTimeLoop()
{
}

/*!
 * \brief no implementation but can be overidden in its derived classes.
 */
void DPMBase::hGridActionsBeforeTimeLoop()
{
}

/*!
 * \brief no implementation but can be overidden in its derived classes.
 */
void DPMBase::actionsOnRestart()
{
}

/*!
 * \brief no implementation but can be overidden in its derived classes.
 */
void DPMBase::hGridActionsBeforeTimeStep()
{
}

/*!
 * \brief no implementation but can be overidden in its derived classes.
 */
void DPMBase::hGridInsertParticle(BaseParticle *obj UNUSED)
{
}

/*!
 * \brief no implementation but can be overidden in its derived classes.
 */
void DPMBase::hGridUpdateParticle(BaseParticle *obj UNUSED)
{
}

/*!
 * \brief no implementation but can be overidden in its derived classes.
 */
void DPMBase::hGridRemoveParticle(BaseParticle *obj UNUSED)
{
}

/*!
 * \return bool (True or False)
 */
bool DPMBase::getHGridUpdateEachTimeStep() const
{
    return true;
}

/*!
 * \brief no implementation but can be overidden in its derived classes.
 */
void DPMBase::actionsBeforeTimeStep()
{
}

/*!
 * \brief no implementation but can be overidden in its derived classes.
 */
void DPMBase::actionsAfterSolve()
{
}

/*!
 * \brief no implementation but can be overidden in its derived classes.
 */
void DPMBase::actionsAfterTimeStep()
{
}

/*!
 * \brief no implementation but can be overidden in its derived classes.
 */
void DPMBase::initialiseStatistics()
{
}

/*!
 * \brief no implementation but can be overidden in its derived classes.
 */
void DPMBase::outputStatistics()
{
}

void DPMBase::gatherContactStatistics()
{
    for (std::vector<BaseInteraction*>::const_iterator it = interactionHandler.begin(); it != interactionHandler.end(); ++it)
        (*it)->gatherContactStatistics();
}

/*!
 * \brief no implementation but can be overidden in its derived classes.
 */
void DPMBase::gatherContactStatistics(unsigned int index1 UNUSED, int index2 UNUSED, Vec3D Contact UNUSED, Mdouble delta UNUSED, Mdouble ctheta UNUSED, Mdouble fdotn UNUSED, Mdouble fdott UNUSED, Vec3D P1_P2_normal_ UNUSED, Vec3D P1_P2_tangential UNUSED)
{
}

/*!
 * \brief no implementation but can be overidden in its derived classes.
 */
void DPMBase::processStatistics(bool usethese UNUSED)
{
}

/*!
 * \brief no implementation but can be overidden in its derived classes.
 */
void DPMBase::finishStatistics()
{
}

/*!
 * \brief no implementation but can be overidden in its derived classes.
 */
void DPMBase::hGridUpdateMove(BaseParticle*, Mdouble)
{
}

/*!
 * \brief no implementation but can be overidden in its derived classes.
 */
void DPMBase::hGridActionsBeforeIntegration()
{
}

/*!
 * \brief no implementation but can be overidden in its derived classes.
 */
void DPMBase::hGridActionsAfterIntegration()
{
}

/*!
 * \param[in] i
 */
void DPMBase::broadPhase(BaseParticle* i)
{
    for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); (*it) != i; ++it)
    {
        computeInternalForces(i, *it);
    }
}

/*!
 * \param[in] n
 */
void DPMBase::setFixedParticles(unsigned int n)
{
    for (unsigned int i = 0; i < std::min(particleHandler.getNumberOfObjects(), n); i++)
        particleHandler.getObject(i)->fixParticle();
}

/*!
 * 
 */
void DPMBase::printTime() const
{
    std::cout << "t=" << std::setprecision(3) << std::left << std::setw(6) << getTime()
        << ", tmax=" << std::setprecision(3) << std::left << std::setw(6) << getTimeMax()
        << std::endl;
    std::cout.flush();
}

/*!
 * \return bool (True or False)
 */
bool DPMBase::continueSolve() const
{
    return true;
}

/*!
 *
 */
void DPMBase::constructor()
{
    //These are the particle parameters like dissipation etc..
    setRestarted(false);

    //This sets the maximum number of particles
    boundaryHandler.setDPMBase(this);
    speciesHandler.setDPMBase(this);
    particleHandler.setDPMBase(this);
    interactionHandler.setDPMBase(this);
    wallHandler.setDPMBase(this);
    interactionHandler.setDPMBase(this);

    particleHandler.setStorageCapacity(2);
    setSystemDimensions(2);
    setParticleDimensions(2);

    // Set gravitational acceleration
    gravity_ = Vec3D(0.0, -9.8, 0.0);
    
    //This is the parameter of the numerical part
    time_ = 0.0;
    timeStep_ = 0.0; // if dt is not user-specified, this is set in actionsBeforeTimeLoop()
    ntimeSteps_ = 0;
    setSaveCount(20);
    timeMax_ = 1.0;
    
    //This sets the default xballs domain
    xMin_ = 0.0;
    xMax_ = 0.01;
    yMin_ = 0.0;
    yMax_ = 0.01;
    zMin_ = 0.0;
    zMax_ = 0.0;
    
    loggerOutput->onFatal = logWriteAndDie;
    
    setName("");
    
    //Default mode is energy with no scale of the vectors
    xBallsColourMode_ = 0;
    xBallsVectorScale_ = -1;
    xBallsScale_ = -1;
    xBallsAdditionalArguments_ = "";
    setAppend(false);

    //The default random seed is 0
    random.setRandomSeed(0);
#ifdef DEBUG_OUTPUT
    std::cerr << "MD problem constructor finished " << std::endl;
#endif
}

/*!
 * \brief A virtual function with no implementation but can be overriden
 */
void DPMBase::setupInitialConditions()
{
}

/*!
* \param[in] os 
*/
void DPMBase::writeEneHeader(std::ostream& os) const
{
    //only write if we don't restart
    if (getAppend())
        return;
    
    ///todo{Why is there a +6 here? TW: to get the numbers and title aligned}
    long width = os.precision() + 6;
    os << std::setw(width) << "t" << " " << std::setw(width)
            << "ene_gra" << " " << std::setw(width)
            << "ene_kin" << " " << std::setw(width)
            << "ene_rot" << " " << std::setw(width)
            << "ene_ela" << " " << std::setw(width)
            << "X_COM" << " " << std::setw(width)
            << "Y_COM" << " " << std::setw(width)
            << "Z_COM" << std::endl;
}

/*!
 * \param[in] os
 */
void DPMBase::writeFstatHeader(std::ostream& os) const
        {
    // line #1: time, volume fraction
    // line #2: wall box: wx0, wy0, wz0, wx1, wy1, wz1
    // line #3: radii-min-max & moments: rad_min, rad_max, r1, r2, r3, r4
    os << "#"
            << " " << getTime()
            << " " << 0
            << std::endl;
    os << "#"
            << " " << getXMin()
            << " " << getYMin()
            << " " << getZMin()
            << " " << getXMax()
            << " " << getYMax()
            << " " << getZMax()
            << std::endl;
    os << "#"
            << " ";
    if (particleHandler.getSmallestParticle())
    {
        os << particleHandler.getSmallestParticle()->getRadius();
    }
    else
    {
        os << std::numeric_limits<double>::quiet_NaN();
    }
    os << " ";
    if (particleHandler.getLargestParticle())
    {
        os << particleHandler.getLargestParticle()->getRadius();
    }
    else
    {
        os << std::numeric_limits<double>::quiet_NaN();
    }
    os << " " << 0
            << " " << 0
            << " " << 0
            << " " << 0
            << std::endl;
    //B: write data
    for (std::vector<BaseInteraction*>::const_iterator it = interactionHandler.begin(); it != interactionHandler.end(); ++it)
    {
        (*it)->writeToFStat(os);
    }
}

/*!
 * \param[in] os
 */
void DPMBase::writeEneTimestep(std::ostream& os) const
{
    Mdouble ene_kin = 0, ene_elastic = 0, ene_rot = 0, ene_gra = 0, mass_sum = 0, x_masslength = 0, y_masslength = 0, z_masslength = 0;

    for (std::vector<BaseParticle*>::const_iterator it = particleHandler.begin(); it != particleHandler.end(); ++it)
        if (!(*it)->isFixed())
        {
            ene_kin += .5 * (*it)->getMass() * (*it)->getVelocity().getLengthSquared();
            ene_rot += .5 * (*it)->getInertia() * (*it)->getAngularVelocity().getLengthSquared();
            ene_gra -= (*it)->getMass() * Vec3D::dot(getGravity(), (*it)->getPosition());
            mass_sum += (*it)->getMass();
            x_masslength += (*it)->getMass() * (*it)->getPosition().X;
            y_masslength += (*it)->getMass() * (*it)->getPosition().Y;
            z_masslength += (*it)->getMass() * (*it)->getPosition().Z;
        } //end for loop over Particles

    ene_elastic = getElasticEnergy();

    ///todo{Why is there a +6 here?  TW: to ensure the numbers fit into a constant width column}
    long width = os.precision() + 6;
    os << std::setw(width) << getTime()
            << " " << std::setw(width) << ene_gra
            << " " << std::setw(width) << ene_kin
            << " " << std::setw(width) << ene_rot
            << " " << std::setw(width) << ene_elastic
            << " " << std::setw(width) << (mass_sum != 0.0 ? x_masslength / mass_sum : std::numeric_limits<double>::quiet_NaN())
            << " " << std::setw(width) << (mass_sum != 0.0 ? y_masslength / mass_sum : std::numeric_limits<double>::quiet_NaN())
            << " " << std::setw(width) << (mass_sum != 0.0 ? z_masslength / mass_sum : std::numeric_limits<double>::quiet_NaN()) << std::endl;

    //sliding = sticking = 0;
}

/*!
 * \param[in] os
 */
void DPMBase::outputXBallsData(std::ostream& os) const
        {
    //Set the correct formation based of dimension if the formation is not specified by the user
    unsigned int format;
    switch (getSystemDimensions())
    {
        case 2:
            format = 8;
            break;
        case 3:
            format = 14;
            break;
        default:
            std::cerr << "Unknown systemdimension" << std::endl;
            exit(-1);
    }
    
    // This outputs the location of walls and how many particles there are to file this is required by the xballs plotting
    if (format != 14) // dim = 1 or 2
    {
        os << particleHandler.getNumberOfObjects() << " " << getTime() << " " << getXMin() << " " << getYMin() << " " << getXMax() << " " << getYMax() << " " << std::endl;
    }
    else
    {
        //dim==3
        os << particleHandler.getNumberOfObjects() << " " << getTime() << " " << getXMin() << " " << getYMin() << " " << getZMin() << " " << getXMax() << " " << getYMax() << " " << getZMax() << " " << std::endl;
    }
    // This outputs the particle data
    for (unsigned int i = 0; i < particleHandler.getNumberOfObjects(); i++)
    {
        outputXBallsDataParticle(i, format, os);
    }
#ifdef DEBUG_OUTPUT
    std::cerr << "Have output the properties of the problem to disk " << std::endl;
#endif
}

/*!
 * \param[in] fileName
 * \param[in] format (format for specifying if its for 2D or 3D data)
 * \return bool (True or False)
 */
bool DPMBase::readDataFile(const std::string fileName, unsigned int format)
{
    std::string oldFileName = dataFile.getName();
    //This opens the file the data will be recalled from
    dataFile.setName(fileName);
    dataFile.open(std::fstream::in);
    if (!dataFile.getFstream().is_open() || dataFile.getFstream().bad())
    {
        std::cout << "Loading data file " << fileName << " failed" << std::endl;
        return false;
    }

    //dataFile.getFileType() is ignored
    FileType fileTypeData = dataFile.getFileType();
    dataFile.setFileType(FileType::ONE_FILE);
    readNextDataFile(format);
    dataFile.setFileType(fileTypeData);

    dataFile.close();
    dataFile.setName(oldFileName);

    //std::cout << "Loaded data file " << filename << " (t=" << getTime() << ")" << std::endl;
    return true;
}

/*!
 * \param[in] fileName
 * \return bool (True or False)
 */
bool DPMBase::readParAndIniFiles(const std::string fileName)
{
    //Opens the par.ini file
    std::fstream file;
    file.open(fileName, std::fstream::in);
    if (!file.is_open() || file.bad())
    {
        //std::cout << "Loading par.ini file " << filename << " failed" << std::endl;
        return false;
    }
    
    Mdouble doubleValue;
    int integerValue;
    
    // inputfile par.ini 
    // line 1 =============================================================
    // Example: 1 1 0
    //   1: integer (0|1) switches from non-periodic to periodic
    //      integer (5|6) does 2D integration only (y-coordinates fixed)
    //                    and switches from non-periodic to periodic
    //      integer (11) uses a quarter system with circular b.c.
    file >> integerValue;
    //~ std::cout << "11" << integerValue << std::endl;
    if (integerValue == 0)
    {
        InfiniteWall w0;
        w0.set(Vec3D(-1, 0, 0), Vec3D(getXMin(), 0, 0));
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D( 1, 0, 0), Vec3D(getXMax(), 0, 0));
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D(0, -1, 0), Vec3D(0, getYMin(), 0));
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D(0,  1, 0), Vec3D(0, getYMax(), 0));
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D(0, 0, -1), Vec3D(0, 0, getZMin()));
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D(0, 0,  1), Vec3D(0, 0, getZMax()));
        wallHandler.copyAndAddObject(w0);
    }
    else if (integerValue == 1)
    {
        PeriodicBoundary b0;
        b0.set(Vec3D(1, 0, 0), getXMin(), getXMax());
        boundaryHandler.copyAndAddObject(b0);
        b0.set(Vec3D(0, 1, 0), getYMin(), getYMax());
        boundaryHandler.copyAndAddObject(b0);
        b0.set(Vec3D(0, 0, 1), getZMin(), getZMax());
        boundaryHandler.copyAndAddObject(b0);
    }
    else if (integerValue == 5)
    {
        InfiniteWall w0;
        w0.set(Vec3D(-1, 0, 0), Vec3D(-getXMin(), 0, 0));
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D(1, 0, 0), Vec3D(getXMax(), 0, 0));
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D(0, -1, 0), Vec3D(0, -getYMin(), 0));
        wallHandler.copyAndAddObject(w0);
        w0.set(Vec3D(0, 1, 0), Vec3D(0, getYMax(), 0));
        wallHandler.copyAndAddObject(w0);
        
    }
    else if (integerValue == 6)
    {
        PeriodicBoundary b0;
        b0.set(Vec3D(1, 0, 0), getXMin(), getXMax());
        boundaryHandler.copyAndAddObject(b0);
        b0.set(Vec3D(0, 1, 0), getYMin(), getYMax());
        boundaryHandler.copyAndAddObject(b0);
        b0.set(Vec3D(0, 0, 1), getZMin(), getZMax());
        boundaryHandler.copyAndAddObject(b0);
    }
    else
    {
        std::cerr << "Error in par.ini: line 1, value 1 is " << integerValue << std::endl;
        exit(-1);
    }
    
    //   2: integer (0|1) dont use | use the search pattern for linked cells
    file >> integerValue; //ignore
            
    //   3: real - gravity in z direction: positive points upwards
    file >> doubleValue;
    setGravity(Vec3D(0.0, 0.0, doubleValue));
    
    // line 2 =============================================================
    // Example: -10000 .5e-2
    //   1: time end of simulation - (negative resets start time to zero
    //                                and uses -time as end-time)	
    file >> doubleValue;
    if (doubleValue < 0)
        setTime(0);
    setTimeMax(fabs(doubleValue));
    
    //   2: time-step of simulation
    file >> doubleValue;
    setTimeStep(doubleValue);
    
    // line 3 =============================================================
    // Example: 1e-1 100
    file >> doubleValue;
    if (doubleValue >= 0)
    {
        //   1: time-step for output on time-series protocoll file  -> "ene"
        unsigned int savecount = static_cast<unsigned int>(round(doubleValue / getTimeStep()));
        setSaveCount(savecount);
        
        //   2: time-step for output on film (coordinate) file      -> "c3d"
        //      (fstat-output is coupled to c3d-output time-step)
        file >> doubleValue;
        savecount = static_cast<unsigned int>(round(doubleValue / getTimeStep()));
        dataFile.setSaveCount(savecount);
        fStatFile.setSaveCount(savecount);
    }
    else
    {
        //  or: ---------------------------------------------------------------
        //   1: negative number is multiplied to the previous log-output-time
        //   2: requires initial log-output time
        //   3: negative number is multiplied to the previous film-output-time
        //   4: requires initial film-output time
        std::cerr << "Error in par.ini: line 3, value 1 is " << doubleValue << std::endl;
        exit(-1);
    }
    
    // line 4 =============================================================
    // Example: 2000 1e5 1e3 1e2
    //   1: particle density (mass=4/3*constants::pi*density*rad^3)
    file >> doubleValue;

    //clear species handler
    speciesHandler.clear();
    auto S = new LinearViscoelasticSlidingFrictionSpecies;
    speciesHandler.addObject(S);

    setParticleDimensions(3);
    S->setDensity(doubleValue);

    //   2: linear spring constant
    file >> doubleValue;
    S->setStiffness(doubleValue);

    //   3: linear dashpot constant
    file >> doubleValue;
    S->setDissipation(doubleValue);

    //   4: background damping dashpot constant
    file >> doubleValue;
    if (doubleValue != 0.0)
        std::cerr << "Warning in par.ini: ignored background damping " << doubleValue << std::endl;
    
    // line 5 =============================================================
    // Example: 0 0
    //   1: growth rate:  d(radius) = xgrow * dt
    file >> doubleValue;
    if (doubleValue != 0.0)
        std::cerr << "Warning in par.ini: ignored growth rate " << doubleValue << std::endl;
    
    //   2: target volume_fraction
    file >> doubleValue;
    if (doubleValue != 0.0)
        std::cerr << "Warning in par.ini: ignored target volume_fraction " << doubleValue << std::endl;
    
    file.close();
    //std::cout << "Loaded par.ini file " << filename << std::endl;
    return true;
}

/*!
 * \param[in] tMin
 * \param[in] verbose
 * \return bool (True or False)
 */
bool DPMBase::findNextExistingDataFile(Mdouble tMin, bool verbose)
{
    if (dataFile.getFileType() == FileType::MULTIPLE_FILES || dataFile.getFileType() == FileType::MULTIPLE_FILES_PADDED)
    {
        while (true)// This true corresponds to the if s
        {
            dataFile.openNextFile();
            //check if file exists and contains data
            int N;
            dataFile.getFstream() >> N;
            if (dataFile.getFstream().eof() || dataFile.getFstream().peek() == -1)
            {
                std::cout << "file " << dataFile.getName() << " not found" << std::endl;
                return false;
            }
            //check if tmin condition is satisfied
            Mdouble t;
            dataFile.getFstream() >> t;
            if (t > tMin)
            {
                //set_file_counter(get_file_counter()-1);
                return true;
            }
            if (verbose)
                std::cout << "Jumping file counter: " << dataFile.getCounter() << std::endl;
        }
    }
    return true;
}

/*!
 * \param[in] format
 */
bool DPMBase::readNextDataFile(unsigned int format)
{
    dataFile.openNextFile(std::fstream::in);
    //fStatFile.openNextFile();
    //Set the correct formation based of dimension if the formation is not specified by the user
    if (format == 0)
    {
        switch (getSystemDimensions())
        {
            case 1:
                case 2:
                format = 8;
                break;
            case 3:
                format = 14;
                break;
        }
        //end case
        
    }
    //end if
    
    unsigned int N=0;
    Mdouble dummy;
    dataFile.getFstream() >> N;

    //read first parameter and check if we reached the end of the file
    if (N==0)
        return false;
    
    BaseParticle* P0 = new BaseParticle();
    if (particleHandler.getNumberOfObjects() < N)
        while (particleHandler.getNumberOfObjects() < N)
            particleHandler.copyAndAddObject(P0);
    else
        while (particleHandler.getNumberOfObjects() > N)
            particleHandler.removeLastObject();
    delete P0;

#ifdef DEBUG_OUTPUT
    std::cout << "Number of particles read from file "<<N << std::endl;
#endif
    
    //read all other data available for the time step
    switch (format)
    {
        //This is the format_ that Stefan's and Vitaley's code saves in - note the axis rotation_
        case 7:
            {
            dataFile.getFstream() >> dummy;
            setTime(dummy);
            dataFile.getFstream() >> dummy;
            setXMin(dummy);
            dataFile.getFstream() >> dummy;
            setYMin(dummy);
            dataFile.getFstream() >> dummy;
            setZMin(dummy);
            dataFile.getFstream() >> dummy;
            setXMax(dummy);
            dataFile.getFstream() >> dummy;
            setYMax(dummy);
            dataFile.getFstream() >> dummy;
            setZMax(dummy);
            //std::cout << " time " << t <<  std::endl;
            Mdouble radius;
            Vec3D position, velocity;
            for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it != particleHandler.end(); ++it)
            {
                dataFile.getFstream() >> position.X >> position.Z >> position.Y >> velocity.X >> velocity.Z >> velocity.Y >> radius >> dummy;
                (*it)->setPosition(position);
                (*it)->setVelocity(velocity);
                (*it)->setOrientation(Vec3D(0.0, 0.0, 0.0));
                (*it)->setAngularVelocity(Vec3D(0.0, 0.0, 0.0));
                (*it)->setRadius(radius);
            }
            //End the interator over all particles
            break;
        }
            //This is a 2D format_
        case 8:
            {
            dataFile.getFstream() >> time_ >> xMin_ >> yMin_ >> xMax_ >> yMax_;
            zMin_ = 0.0;
            zMax_ = 0.0;
            Mdouble radius;
            Vec3D position, velocity, angle, angularVelocity;
            for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it != particleHandler.end(); ++it)
            {
                dataFile.getFstream() >> position.X >> position.Y >> velocity.X >> velocity.Y >> radius >> angle.Z >> angularVelocity.Z >> dummy;
                (*it)->setPosition(position);
                (*it)->setVelocity(velocity);
                (*it)->setOrientation(-angle);
                (*it)->setAngularVelocity(-angularVelocity);
                (*it)->setRadius(radius);
            } //end for all particles
            break;
        }
            //This is a 3D format_
        case 14:
            {
            dataFile.getFstream() >> time_ >> xMin_ >> yMin_ >> zMin_ >> xMax_ >> yMax_ >> zMax_;
            Mdouble radius;
            Vec3D position, velocity, angle, angularVelocity;
            for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it != particleHandler.end(); ++it)
            {
                dataFile.getFstream() >> position >> velocity >> radius >> angle >> angularVelocity >> dummy;
                (*it)->setPosition(position);
                (*it)->setVelocity(velocity);
                (*it)->setOrientation(angle);
                (*it)->setAngularVelocity(angularVelocity);
                (*it)->setRadius(radius);
            } //end for all particles
            break;
        }
            //This is a 3D format_
        case 15:
            {
            dataFile.getFstream() >> time_ >> xMin_ >> yMin_ >> zMin_ >> xMax_ >> yMax_ >> zMax_;
            Mdouble radius;
            Vec3D position, velocity, angle, angularVelocity;
            for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it != particleHandler.end(); ++it)
            {
                dataFile.getFstream() >> position >> velocity >> radius >> angle >> angularVelocity >> dummy >> dummy;
                (*it)->setPosition(position);
                (*it)->setVelocity(velocity);
                (*it)->setOrientation(angle);
                (*it)->setAngularVelocity(angularVelocity);
                (*it)->setRadius(radius);
                
            } //end for all particles
            break;
        }
    } //end switch statement
    particleHandler.computeAllMasses();
    
    return true;
}

/*!
 * /// See also MD::readRestartFile
 */
void DPMBase::writeRestartFile()
{
    write(restartFile.getFstream());
}

/*!
 * \details Calls the read() and sets the restarted_ flag to true 
 *          (if the file is found)
 * \return int
 */
int DPMBase::readRestartFile()
{
    if (restartFile.open(std::fstream::in))
    {
        read(restartFile.getFstream());
        restartFile.close();
        setRestarted(true);
        return (1);
    }
    else
    {
        std::cerr << restartFile.getName() << " could not be loaded" << std::endl;
        return (0);
    }
}

/*!
 * \param[in] fileName
 * \return int
 */
int DPMBase::readRestartFile(std::string fileName)
{
    restartFile.setName(fileName);
    return readRestartFile();
}

/*!
 * \param[in] P1
 * \param[in] P2
 */
void DPMBase::computeInternalForces(BaseParticle* P1, BaseParticle* P2)
{
    //this is because the rough bottom allows overlapping fixed particles
    if (P1->isFixed() && P2->isFixed())
    {
        return;
    }

    if (P1->getPeriodicFromParticle() && P2->getPeriodicFromParticle())
    {
        return;
    }

    BaseParticle *PI, *PJ;
    if (P1->getId() > P2->getId())
    {
        PI = P2;
        PJ = P1;
    }
    else
    {
        PI = P1;
        PJ = P2;
    }

    //if statement above ensures that the PI has the lower id than PJ
    BaseInteraction* C = PJ->getInteractionWith(PI, getTime(), &interactionHandler);
    if (C != 0)
    {
        C->computeForce();

        PI->addForce(C->getForce());
        PJ->addForce(-C->getForce());

        if (getRotation())
        {
            PI->addTorque(C->getTorque() - Vec3D::cross(PI->getPosition() - C->getContactPoint(), C->getForce()));
            PJ->addTorque(-C->getTorque() + Vec3D::cross(PJ->getPosition() - C->getContactPoint(), C->getForce()));
        }
    }
}

/*!
 * \todo take out computeWalls() from compute External Forces method.
 * \param[in] CI
 */
void DPMBase::computeExternalForces(BaseParticle* CI)
{
    if (!CI->isFixed())
    {
        // Gravitational force
        CI->addForce(getGravity() * CI->getMass());
        // Still calls this in compute External Forces.
        computeForcesDueToWalls(CI);
    }
}

/*!
 * \param[in] pI
 */
void DPMBase::computeForcesDueToWalls(BaseParticle* pI)
{
    //No need to compute interactions between periodic particle images and walls
    if (pI->getPeriodicFromParticle() != nullptr)
        return;

    for (std::vector<BaseWall*>::iterator it = wallHandler.begin(); it != wallHandler.end(); ++it)
    {
        BaseInteraction* C = (*it)->getInteractionWith(pI, getTime(), &interactionHandler);

        if (C != nullptr)
        {
            C->computeForce();

            pI->addForce(C->getForce());
            (*it)->addForce(-C->getForce());

            if (getRotation()) // getRotation() returns a boolean.
            {
                pI->addTorque(C->getTorque() - Vec3D::cross(pI->getPosition() - C->getContactPoint(), C->getForce()));
                ///\todo TW: I think this torque has the wrong sign
                (*it)->addTorque(-C->getTorque() + Vec3D::cross((*it)->getPosition() - C->getContactPoint(), C->getForce()));
            }
        }
    }
}

/*!
 * 
 */
void DPMBase::integrateBeforeForceComputation()
{
    for_each(particleHandler.begin(), particleHandler.end(), [this] (BaseParticle* p)
    {
        p->integrateBeforeForceComputation(getTime(),getTimeStep());
    });
    for_each(wallHandler.begin(), wallHandler.end(), [this] (BaseWall* w)
    {
        w->integrateBeforeForceComputation(getTime(),getTimeStep());
    });
}
/*!
 * \\\ calls checkBoundaryAfterParticleMoved()
 */
void DPMBase::checkInteractionWithBoundaries()
{
    for (std::vector<BaseBoundary*>::iterator B = boundaryHandler.begin(); B != boundaryHandler.end(); ++B)
    {
        for (std::vector<BaseParticle*>::iterator P = particleHandler.begin(); P != particleHandler.end(); ++P)
        {
            if ((*B)->checkBoundaryAfterParticleMoved(*P, particleHandler)) //Returns true if the particle is deleted
                --P;
        }
    }
}

/*!
 *
 */
void DPMBase::integrateAfterForceComputation()
{
    for_each(particleHandler.begin(), particleHandler.end(), [this] (BaseParticle* p)
    {
        p->integrateAfterForceComputation(getTime(),getTimeStep());
    });
    for_each(wallHandler.begin(), wallHandler.end(), [this] (BaseWall* w)
    {
        w->integrateAfterForceComputation(getTime(),getTimeStep());
    });
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
/////statisticsFromRestartData
///////////////////////////////////////////////////////////////////////////////////////////////////////
//void DPMBase::statisticsFromRestartData(const char *name)
//{
//    ///todo{Check this whole function}
//    //This function loads all MD data
//    readRestartFile();
//
//    //This creates the file statistics will be saved to
//    std::stringstream ss("");
//    ss << name << ".stat";
//    statFile.setName(ss.str());
//    statFile.setOpenMode(std::fstream::out);
//    statFile.open();
//
//    // Sets up the initial conditions for the simulation
//    // setupInitialConditions();
//    // Setup the previous position arrays and mass of each particle.
//    computeParticleMasses();
//    // Other routines required to jump-start the simulation
//    actionsBeforeTimeLoop();
//    initialiseStatistics();
//    hGridActionsBeforeTimeLoop();
//    writeEneHeader(eneFile.getFstream());
//
//    while (readDataFile(dataFile.getName().c_str()))
//    {
//        hGridActionsBeforeTimeLoop();
//        actionsBeforeTimeStep();
//        checkAndDuplicatePeriodicParticles();
//        hGridActionsBeforeTimeStep();
////        dataFile.setSaveCurrentTimestep(true);
////        eneFile.setSaveCurrentTimestep(true);
////        statFile.setSaveCurrentTimestep(true);
////        fStatFile.setSaveCurrentTimestep(true);
//        computeAllForces();
//        removeDuplicatePeriodicParticles();
//        actionsAfterTimeStep();
//        writeEneTimestep(eneFile.getFstream());
//        std::cout << std::setprecision(6) << getTime() << std::endl;
//    }
//
//    dataFile.close();
//    statFile.close();
//}

/*!
 * 
 */
void DPMBase::computeAllForces()
{
    ///Reset all forces to zero
    for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it != particleHandler.end(); ++it)
    {
        (*it)->setForce(Vec3D(0.0, 0.0, 0.0));
        (*it)->setTorque(Vec3D(0.0, 0.0, 0.0));
    }
    for (std::vector<BaseWall*>::iterator it = wallHandler.begin(); it != wallHandler.end(); ++it)
    {
        (*it)->setForce(Vec3D(0.0, 0.0, 0.0));
        (*it)->setTorque(Vec3D(0.0, 0.0, 0.0));
    } //end reset forces loop
    
#ifdef DEBUG_OUTPUT
    std::cerr << "Have all forces set to zero " << std::endl;
#endif
    
    ///Now loop over all particles contacts computing force contributions
    
    for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it != particleHandler.end(); ++it)
    {
        
        ///Now loop over all other particles looking for contacts
        computeInternalForces(*it);
        //end inner loop over contacts.
        
        computeExternalForces(*it);
        
    }
    
#ifdef CONTACT_LIST_HGRID
    //possibleContactList.write(std::cout);
    PossibleContact* Curr=possibleContactList.getFirstPossibleContact();
    while(Curr)
    {   
        computeInternalForces(Curr->getP1(),Curr->getP2());
        Curr=Curr->getNext();
    }
#endif
    //end outer loop over contacts.
    
}

/*!
 * \param[in] i
 */
void DPMBase::computeInternalForces(BaseParticle* i)
{
    broadPhase(i);
}

/*!
 * \param[in] os
 * \param[in] writeAllParticles
 */
void DPMBase::write(std::ostream& os, bool writeAllParticles) const
{
    os << "restart_version " << "1.0";
    FilesAndRunNumber::write(os);
    os << "xMin " << getXMin()
        << " xMax " << getXMax()
        << " yMin " << getYMin()
        << " yMax " << getYMax()
        << " zMin " << getZMin()
        << " zMax " << getZMax() << std::endl
        << "timeStep " << getTimeStep()
        << " time " << getTime()
        << " ntimeSteps " << ntimeSteps_
        << " timeMax " << getTimeMax() << std::endl
        << "systemDimensions " << getSystemDimensions()
        << " particleDimensions " << getParticleDimensions()
        << " gravity " << getGravity() << std::endl;
    speciesHandler.write(os);
    os << "Walls " << wallHandler.getNumberOfObjects() << std::endl;
    for (std::vector<BaseWall*>::const_iterator it = wallHandler.begin(); it != wallHandler.end(); ++it)
        os << (**it) << std::endl;
    os << "Boundaries " << boundaryHandler.getNumberOfObjects() << std::endl;
    for (std::vector<BaseBoundary*>::const_iterator it = boundaryHandler.begin(); it != boundaryHandler.end(); ++it)
        os << (**it) << std::endl;
    if (writeAllParticles || particleHandler.getNumberOfObjects() < 4)
    {
        particleHandler.write(os);
    }
    else
    {
        os << "Particles " << particleHandler.getNumberOfObjects() << std::endl;
        for (unsigned int i = 0; i < 2; i++)
            os << *particleHandler.getObject(i) << std::endl;
        os << "..." << std::endl;
    }
    if (writeAllParticles || interactionHandler.getNumberOfObjects() < 4)
    {
        interactionHandler.write(os);
    }
    else
    {
        os << "Interactions " << interactionHandler.getNumberOfObjects() << std::endl;
        for (unsigned int i = 0; i < 2; i++)
            os << *interactionHandler.getObject(i) << std::endl;
        os << "..." << std::endl;
    }
    ///\todo TW: random number seed is not stored
    ///\todo TW: xBalls arguments are not stored
}

/*!
 * \param[in] is
 */
void DPMBase::read(std::istream& is)
{
    std::string dummy;
    is >> dummy;
    if (strcmp(dummy.c_str(), "restart_version"))
    {
        //only very old files did not have a restart_version
        logger.log(Log::FATAL, "Error in DPMBase::read(is): this is not a valid restart file");
    }
    else
    {
        is >> restartVersion_;
        if (!restartVersion_.compare("1.0"))
        {
            FilesAndRunNumber::read(is);
            is >> dummy >> xMin_
                >> dummy >> xMax_
                >> dummy >> yMin_
                >> dummy >> yMax_
                >> dummy >> zMin_
                >> dummy >> zMax_
                >> dummy >> timeStep_
                >> dummy >> time_
                >> dummy >> ntimeSteps_
                >> dummy >> timeMax_
                >> dummy >> systemDimensions_
                >> dummy >> particleDimensions_
                >> dummy >> gravity_;
            speciesHandler.read(is);

            unsigned int N;
            std::stringstream line(std::stringstream::in | std::stringstream::out);
            is >> dummy >> N;
            wallHandler.clear();
            helpers::getLineFromStringStream(is, line);
            for (unsigned int i = 0; i < N; i++)
            {
                helpers::getLineFromStringStream(is, line);
                wallHandler.readObject(line);
            }

            is >> dummy >> N;
            boundaryHandler.clear();
            helpers::getLineFromStringStream(is, line);
            for (unsigned int i = 0; i < N; i++)
            {
                helpers::getLineFromStringStream(is, line);
                boundaryHandler.readObject(line);
            }

            is >> dummy >> N;
            particleHandler.clear();
            helpers::getLineFromStringStream(is, line);
            for (unsigned int i = 0; i < N; i++)
            {
                helpers::getLineFromStringStream(is, line);
                particleHandler.readObject(line);
                ///todo{Do we want to calculate the mass?}
                //particleHandler.getLastObject()->computeMass();
            }
            interactionHandler.read(is);
        }
        else if (!restartVersion_.compare("3"))
        {
            logger.log(Log::INFO, "DPMBase::read(is): restarting from an old restart file (restart_version %).", restartVersion_);
            readOld(is);
        }
        else
        {
            //only very old files did not have a restart_version
            logger.log(Log::FATAL, "Error in DPMBase::read(is): restart_version % cannot be read; use an older version of Mercury to upgrade the file", restartVersion_);
        }
    }
}
/*!
 * \param[in] is
 */
void DPMBase::readOld(std::istream &is)
{
    std::string dummy;
    is >> dummy >> dummy;
    setName(dummy);

    unsigned int saveCountData, saveCountEne, saveCountStat, saveCountFStat;
    unsigned int fileTypeFstat, fileTypeData, fileTypeEne, fileTypeRestart;
    is >> dummy >> xMin_
        >> dummy >> xMax_
        >> dummy >> yMin_
        >> dummy >> yMax_
        >> dummy >> zMin_
        >> dummy >> zMax_
        >> dummy >> timeStep_
        >> dummy >> time_
        >> dummy >> timeMax_
        >> dummy >> saveCountData
        >> dummy >> saveCountEne
        >> dummy >> saveCountStat
        >> dummy >> saveCountFStat
        >> dummy >> systemDimensions_
        >> dummy >> gravity_
        >> dummy >> fileTypeFstat
        >> dummy >> fileTypeData
        >> dummy >> fileTypeEne;
    dataFile.setSaveCount(saveCountData);
    eneFile.setSaveCount(saveCountEne);
    statFile.setSaveCount(saveCountStat);
    fStatFile.setSaveCount(saveCountFStat);
    
    fStatFile.setFileType(static_cast<FileType>(fileTypeFstat));
    dataFile.setFileType(static_cast<FileType>(fileTypeData));
    eneFile.setFileType(static_cast<FileType>(fileTypeEne));
    
    //this is optional to allow restart files with and without restartFile.getFileType()
    is >> dummy;
    if (!strcmp(dummy.c_str(), "options_restart"))
    {
        is >> fileTypeRestart;
        restartFile.setFileType(static_cast<FileType>(fileTypeRestart));
    }

    speciesHandler.read(is);
    wallHandler.read(is);
    boundaryHandler.read(is);
    particleHandler.read(is);
}

/*!
 *
 */
void DPMBase::checkSettings()
{
    if (speciesHandler.getNumberOfObjects() == 0)
    {
        std::cerr << "Error in solve(): at least one species has to be defined" << std::endl;
        exit(-1);
    }
    if (getParticleDimensions() == 0)
    {
        std::cerr << "Error in solve(): particleDimensions not specified" << std::endl;
        exit(-1);
    }
    if (getSystemDimensions() == 0)
    {
        std::cerr << "Error in solve(): systemDimensions not specified" << std::endl;
        exit(-1);
    }
    if (getTimeStep() == 0.0)
    {
        std::cerr << "Error in solve(): timeStep not specified" << std::endl;
        exit(-1);
    }
}

void DPMBase::writeOutputFiles()
{
    if (fStatFile.saveCurrentTimestep(ntimeSteps_))
        writeFstatHeader(fStatFile.getFstream());

    if (eneFile.saveCurrentTimestep(ntimeSteps_))
    {
        if (eneFile.getCounter()==1 || eneFile.getFileType()==FileType::MULTIPLE_FILES || eneFile.getFileType()==FileType::MULTIPLE_FILES_PADDED)
            writeEneHeader(eneFile.getFstream());
        writeEneTimestep(eneFile.getFstream());
    }

    if (dataFile.saveCurrentTimestep(ntimeSteps_))
    {
        printTime();
        if ((getRestarted() ||dataFile.getCounter()==1) && dataFile.getFileType()!= FileType::NO_FILE)
            writeXBallsScript();
        outputXBallsData(dataFile.getFstream());
    }

//    if (statFile.saveCurrentTimestep(ntimeSteps_))
//    {
//        outputStatistics();
//        gatherContactStatistics();
//        processStatistics(true);
//    }

    //write restart file last, otherwise the output cunters are wrong
    if (restartFile.saveCurrentTimestep(ntimeSteps_))
    {
        writeRestartFile();
        restartFile.close(); //overwrite old restart file if FileType::ONE_FILE
    }
}

/*!
 * \details  - Initialises the time, sets up the initial conditions for the simulation by 
 *             calling the setupInitialConditions() and resets the counter using
 *             setNExtSavedTimeStep().
 *          -  HGrid operations which is the contact detection algorithm.
 *          -  Checks if the basic essentials are set for carrying out the
 *             simulations using checkSettings()
 *          -  And many more vital operations. See below ;)
 * \todo Is it neccesarry to reset initial conditions here and in setTimeStepByParticle
 *       (i.e. should it be in constructor) Thomas: I agree, setTimeStepByParticle should be 
 *       rewritten to work without calling setupInitialConditions
 */
void DPMBase::solve()
{
#ifdef DEBUG_OUTPUT
    std::cerr << "Entered solve" << std::endl;
#endif
#ifdef CONTACT_LIST_HGRID
    std::cout << "Using CONTACT_LIST_HGRID"<<std::endl;
#endif
    
    /// Initialise the time and
    /// sets up the initial conditions for the simulation
    ///\todo Is it neccesarry to reset initial conditions here and in setTimeStepByParticle (i.e. should it be in constructor)?
    ///Thomas: I agree, setTimeStepByParticle should be rewritten to work without calling setupInitialConditions
    if (getRestarted()==false)
    {
        ntimeSteps_ = 0;
        resetFileCounter();
        setTime(0.0);
        setupInitialConditions();
        setNextSavedTimeStep(0); //reset the counter
#ifdef DEBUG_OUTPUT
        std::cerr << "Have created the particles initial conditions " << std::endl;
#endif
    }
    else
    {
        actionsOnRestart();
    }

    checkSettings();

    if (getRunNumber()>0) 
    {
        std::stringstream name;
        name << getName() << "." << getRunNumber();
        setName(name.str());
    }
    
    //append_ determines if files have to be appended or not
    if (getAppend())
    {
        setOpenMode(std::fstream::out | std::fstream::app);
        restartFile.setOpenMode(std::fstream::out); //Restart files should always be overwritten.
    }
    else
        setOpenMode(std::fstream::out);

    initialiseStatistics();

    // Setup the mass of each particle.
    particleHandler.computeAllMasses();

    // Other initialisations
    //max_radius = getLargestParticle()->getRadius();
    actionsBeforeTimeLoop();
    hGridActionsBeforeTimeLoop();

    // do a first force computation
    checkAndDuplicatePeriodicParticles();
    hGridActionsBeforeTimeStep();
    computeAllForces();
    removeDuplicatePeriodicParticles();


#ifdef DEBUG_OUTPUT
    std::cerr << "Have computed the initial values for the forces " << std::endl;
#endif

    // This is the main loop over advancing time
    while (getTime() < getTimeMax() && continueSolve())
    {
        writeOutputFiles(); //everything is written at the beginning of the timestep!
        
        // Loop over all particles doing the time integration step
        hGridActionsBeforeIntegration();
        integrateBeforeForceComputation();
        checkInteractionWithBoundaries(); // INSERTION boundaries handled
        hGridActionsAfterIntegration();

        // Compute forces
        
        ///\bug{In chute particles are added in actions_before_time_set(), however they are not written to the xballs data yet, but can have a collison and be written to the fstat data}
        // INSERTION/DELETION boundary flag change
        for (std::vector<BaseBoundary*>::iterator it = boundaryHandler.begin(); it != boundaryHandler.end(); ++it)
        {
            (*it)->checkBoundaryBeforeTimeStep(this);
        }

        actionsBeforeTimeStep();

        checkAndDuplicatePeriodicParticles();

        hGridActionsBeforeTimeStep();

        computeAllForces();

        removeDuplicatePeriodicParticles();

        actionsAfterTimeStep();

        // Loop over all particles doing the time integration step
        hGridActionsBeforeIntegration();
        integrateAfterForceComputation();

        checkInteractionWithBoundaries(); // DELETION boundaries handled
        hGridActionsAfterIntegration();

        //erase interactions that have not been used during the last timestep
        interactionHandler.eraseOldInteractions(getTime() - getTimeStep() * 0.5);

        time_ += timeStep_;
        ntimeSteps_ ++;
    }
    //force writing of the last time step
    setNextSavedTimeStep(ntimeSteps_);
    writeOutputFiles();

    //end loop over interaction count
    actionsAfterSolve();
    
    std::cout << std::endl;
    //To make sure getTime gets the correct time for outputting statistics
    finishStatistics();
    
    closeFiles();
}

/*!
 * \param[in] argc
 * \param[in] *argv[]  
 */
bool DPMBase::readArguments(int argc, char *argv[])
{
    bool isRead = true;
    for (int i = 1; i < argc; i += 2)
    {
        std::cout << "interpreting input argument " << argv[i];
        for (int j = i + 1; j < argc; j++)
        {
            if (argv[j][0] == '-')
                break;
            std::cout << " " << argv[j];
        }
        std::cout << std::endl;
        isRead &= readNextArgument(i, argc, argv);
        if (!isRead)
        {
            std::cerr << "Warning: not all arguments read correctly!" << std::endl;
        }
    }
    return isRead;
}

/*!
 * \param[in] i
 * \param[in] argc
 * \param[in] *argv[]
 * \return bool (True or False)
 */
bool DPMBase::readNextArgument(int& i, int argc, char *argv[])
{
    ///argv[i+1] interpreted as argument of type char*, Mdouble, integer or boolean unless noted
    if (!strcmp(argv[i], "-name"))
    {
        setName(argv[i + 1]);
    }
    else if (!strcmp(argv[i], "-xmin"))
    {
        setXMin(atof(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-ymin"))
    {
        setYMin(atof(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-zmin"))
    {
        setZMin(atof(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-xmax"))
    {
        setXMax(atof(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-ymax"))
    {
        setYMax(atof(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-zmax"))
    {
        setZMax(atof(argv[i + 1]));
        //} else if (!strcmp(argv[i],"-svn")) {
        //	std::cout << "svn version " << SVN_VERSION << std::endl;
        //	i--;
    }
    else if (!strcmp(argv[i], "-dt"))
    {
        Mdouble old = getTimeStep();
        setTimeStep(atof(argv[i + 1]));
        std::cout << "  reset dt from " << old << " to " << getTimeStep() << std::endl;
    }
//    else if (!strcmp(argv[i], "-Hertz"))
//    {
//        speciesHandler.getObject(0)->setForceType(ForceType::HERTZ);
//        i--;
//    }
    else if (!strcmp(argv[i], "-tmax"))
    {
        Mdouble old = getTimeMax();
        setTimeMax(atof(argv[i + 1]));
        std::cout << "  reset timeMax from " << old << " to " << getTimeMax() << std::endl;
    }
    else if (!strcmp(argv[i], "-saveCount"))
    {
        setSaveCount(static_cast<unsigned int>(atoi(argv[i + 1])));
    }
    else if (!strcmp(argv[i], "-saveCountData"))
    {
        dataFile.setSaveCount(static_cast<unsigned int>(atoi(argv[i + 1])));
    }
    else if (!strcmp(argv[i], "-saveCountFStat"))
    {
        fStatFile.setSaveCount(static_cast<unsigned int>(atoi(argv[i + 1])));
    }
    else if (!strcmp(argv[i], "-saveCountStat"))
    {
        statFile.setSaveCount(static_cast<unsigned int>(atoi(argv[i + 1])));
    }
    else if (!strcmp(argv[i], "-saveCountEne"))
    {
        eneFile.setSaveCount(static_cast<unsigned int>(atoi(argv[i + 1])));
    }
    else if (!strcmp(argv[i], "-saveCountRestart"))
    {
        restartFile.setSaveCount(static_cast<unsigned int>(atoi(argv[i + 1])));
    }
    else if (!strcmp(argv[i], "-dim"))
    {
        setSystemDimensions(static_cast<unsigned int>(atoi(argv[i + 1])));
    }
    else if (!strcmp(argv[i], "-gravity"))
    {
        ///-gravity_ requires three arguments
        setGravity(Vec3D(atof(argv[i + 1]), atof(argv[i + 2]), atof(argv[i + 3])));
        i += 2;
    }
    else if (!strcmp(argv[i], "-fileType"))
    { //uses int input
        setFileType(static_cast<FileType>(atoi(argv[i + 1])));
    }
    else if (!strcmp(argv[i], "-fileTypeFStat"))
    { //uses int input
        fStatFile.setFileType(static_cast<FileType>(atoi(argv[i + 1])));
    }
    else if (!strcmp(argv[i], "-fileTypeRestart"))
    {
        restartFile.setFileType(static_cast<FileType>(atoi(argv[i + 1])));
    }
    else if (!strcmp(argv[i], "-fileTypeData"))
    {
        dataFile.setFileType(static_cast<FileType>(atoi(argv[i + 1])));
    }
    else if (!strcmp(argv[i], "-fileTypeStat"))
    {
        statFile.setFileType(static_cast<FileType>(atoi(argv[i + 1])));
    }
    else if (!strcmp(argv[i], "-fileTypeEne"))
    {
        eneFile.setFileType(static_cast<FileType>(atoi(argv[i + 1])));
    }
    else if (!strcmp(argv[i], "-auto_number"))
    {
        autoNumber();
        i--;
    }
//    else if (!strcmp(argv[i], "-number_of_saves"))
//    {
//        set_number_of_saves_all(atof(argv[i + 1]));
//    }
    else if (!strcmp(argv[i], "-restart") || !strcmp(argv[i], "-r"))
    {
        ///-restart or -r loads a restart file. 
        ///By default, it loads <name>.restart.
        ///If an argument "arg" is given it loads the file "arg", or "arg".restart (if the ending is not given).
        std::string filename;
        
        //use default filename if no argument is given
        if (i + 1 >= argc || argv[i + 1][0] == '-')
        {
            i--;
            filename = getName();
            std::cout << getName() << std::endl;
        }
        else
            filename = argv[i + 1];
        
        //add ".restart" if necessary
        if (filename.find(".restart") == std::string::npos)
        {            
            filename = filename + ".restart";
        }
        
        std::cout << "restart from " << filename << std::endl;
        readRestartFile(filename);
    }    
    else if (!strcmp(argv[i], "-clean") || !strcmp(argv[i], "-c"))
    {
        std::cout<< "Remove old " << getName() << ".* files" << std::endl;
        ///-clean of -c removes all files <name>.*. 
//        std::string filename;
        std::ostringstream filename;
        std::vector<std::string> ext {".restart",".stat",".fstat",".data",".ene",".xballs"};
        for (unsigned int j=0; j<ext.size(); j++)
        {
            // remove files with given extension for FileType::ONE_FILE
            filename.clear();
            filename << getName() <<ext[j];
            if( !remove( filename.str().c_str() ) )
            {
                std::cout<< "  File " << filename.str() << " successfully deleted" << std::endl;
            }
            // remove files with given extension for FileType::MULTIPLE_FILES
            unsigned int k=0;
            filename.clear(); filename << getName() << ext[j] << '.' << k;
            while( !remove( filename.str().c_str() ) )
            {
                std::cout<< "  File " << filename.str() << " successfully deleted" << std::endl;
                filename.clear();
                filename << getName() << ext[j] << '.' << ++k;
            }
            // remove files with given extension for FileType::MULTIPLE_FILES_PADDED
            k=0;           
            filename.clear(); filename << getName() << ext[j] << '.' << to_string_padded(k);
            while( !remove( filename.str().c_str() ) )
            {
                std::cout<< "  File " << filename.str() << " successfully deleted" << std::endl;
                filename.clear();
                filename << getName() << ext[j] << '.' << to_string_padded(++k);
            }
        }
        i--;
    }
    else if (!strcmp(argv[i], "-data"))
    {
        std::string filename = argv[i + 1];
        readDataFile(filename.c_str());
    }
//    else if (!strcmp(argv[i], "-k"))
//    {
//        Mdouble old = getSpecies(0)->getStiffness();
//        getSpecies(0)->setStiffness(atof(argv[i + 1]));
//        std::cout << "  reset k from " << old << " to " << getSpecies(0)->getStiffness() << std::endl;
//    }
//    else if (!strcmp(argv[i], "-dissipation") || !strcmp(argv[i], "-disp"))
//    {
//        Mdouble old = getSpecies(0)->getDissipation();
//        getSpecies(0)->setDissipation(atof(argv[i + 1]));
//        std::cout << "  reset getDissipation() from " << old << " to " << getSpecies(0)->getDissipation() << std::endl;
//    }
//    else if (!strcmp(argv[i], "-kt"))
//    {
//        Mdouble old = getSpecies(0)->getSlidingStiffness();
//        getSpecies(0)->setSlidingStiffness(atof(argv[i + 1]));
//        std::cout << "  reset kt from " << old << " to " << getSpecies(0)->getSlidingStiffness() << std::endl;
//    }
//    else if (!strcmp(argv[i], "-dispt"))
//    {
//        Mdouble old = getSpecies(0)->getSlidingDissipation();
//        getSpecies(0)->setSlidingDissipation(atof(argv[i + 1]));
//        std::cout << "  reset dispt from " << old << " to " << getSpecies(0)->getSlidingDissipation() << std::endl;
//    }
//    else if (!strcmp(argv[i], "-krolling"))
//    {
//        Mdouble old = getSpecies(0)->getRollingStiffness();
//        getSpecies(0)->setRollingStiffness(atof(argv[i + 1]));
//        std::cout << "  reset krolling from " << old << " to " << getSpecies(0)->getRollingStiffness() << std::endl;
//    }
//    else if (!strcmp(argv[i], "-disprolling"))
//    {
//        Mdouble old = getSpecies(0)->getRollingDissipation();
//        getSpecies(0)->setRollingDissipation(atof(argv[i + 1]));
//        std::cout << "  reset disprolling from " << old << " to " << getSpecies(0)->getRollingDissipation() << std::endl;
//    }
//    else if (!strcmp(argv[i], "-mu"))
//    {
//        Mdouble old = getSpecies(0)->getSlidingFrictionCoefficient();
//        getSpecies(0)->setSlidingFrictionCoefficient(atof(argv[i + 1]));
//        std::cout << "  reset mu from " << old << " to " << getSpecies(0)->getSlidingFrictionCoefficient() << std::endl;
//    }
//    else if (!strcmp(argv[i], "-murolling"))
//    {
//        Mdouble old = getSpecies(0)->getRollingFrictionCoefficient();
//        getSpecies(0)->setRollingFrictionCoefficient(atof(argv[i + 1]));
//        std::cout << "  reset murolling from " << old << " to " << getSpecies(0)->getRollingFrictionCoefficient() << std::endl;
//    }
    else if (!strcmp(argv[i], "-randomise") || !strcmp(argv[i], "-randomize"))
    {
        random.randomise();
        i--;
    }
//    else if (!strcmp(argv[i], "-k0"))
//    {
//        Mdouble old = speciesHandler.getObject(0)->getAdhesionStiffness();
//        speciesHandler.getObject(0)->setAdhesionStiffness(atof(argv[i + 1]));
//        std::cout << "  reset k0 from " << old << " to " << speciesHandler.getObject(0)->getAdhesionStiffness() << std::endl;
//    }
//    else if (!strcmp(argv[i], "-f0"))
//    {
//        Mdouble old = speciesHandler.getObject(0)->getAdhesionForceMax();
//        speciesHandler.getObject(0)->setAdhesionForceMax(atof(argv[i + 1]));
//        std::cout << "  reset f0 from " << old << " to " << speciesHandler.getObject(0)->getAdhesionForceMax() << std::endl;
//    }
//    else if (!strcmp(argv[i], "-AdhesionForceType"))
//    {
//        AdhesionForceType old = speciesHandler.getObject(0)->getAdhesionForceType();
//        speciesHandler.getObject(0)->setAdhesionForceType(argv[i + 1]);
//        std::cout << "  reset AdhesionForceType from "
//                << static_cast<signed char>(old) << " to "
//                << static_cast<signed char>(speciesHandler.getObject(0)->getAdhesionForceType()) << std::endl;
//    }
    else if (!strcmp(argv[i], "-append"))
    {
        setAppend(true);
        i--;
    }
    else if (!strcmp(argv[i], "-fixedParticles"))
    {
        setFixedParticles(static_cast<unsigned int>(atoi(argv[i + 1])));
    }
//    else if (!strcmp(argv[i], "-rho"))
//    {
//        Mdouble old = speciesHandler.getObject(0)->getDensity();
//        speciesHandler.getObject(0)->setDensity(atof(argv[i + 1]));
//        std::cout << "  reset rho from " << old << " to " << speciesHandler.getObject(0)->getDensity() << std::endl;
//    }
//    else if (!strcmp(argv[i], "-dim_particle"))
//    {
//        setParticleDimensions(atoi(argv[i + 1]));
//    }
    else if (!strcmp(argv[i], "-counter"))
    {
        setRunNumber(atoi(argv[i + 1]));
    }
    else
        return false;
    return true; //returns true if argv is found
}

/*!
 * \details A very useful feature. For e.g. when one wants to have an initial condition 
 *          with particles free of interactions with other particles or walls, one could use this
 *          method and check for particles if they are interacting. If yes, then it would not 
 *          consider this particle for insertion and continue onto next particle.
 *          However can prove expensive if the number of particles is large.
 * \param[in] p
 * \return bool (True or False) 
 */
bool DPMBase::checkParticleForInteraction(const BaseParticle& p)
{
    Mdouble distance;
    Vec3D normal;
    
    //Check if it has no collision with walls
    for (std::vector<BaseWall*>::const_iterator it = wallHandler.begin(); it != wallHandler.end(); it++)
    {
        if ((*it)->getDistanceAndNormal(p, distance, normal))
        {
            //std::cout<<"failure: Collision with wall: "<<**it<<std::endl;
            return false;
        }
        else
        {
            //std::cout<<"No collision with wall: "<<**it<<std::endl;
        }
    }
    
    //Check if it has no collision with other particles
    for (std::vector<BaseParticle*>::const_iterator it = particleHandler.begin(); it != particleHandler.end(); it++)
    {
        if (Vec3D::getDistanceSquared(p.getPosition(), (*it)->getPosition()) < mathsFunc::square(p.getInteractionRadius() + (*it)->getInteractionRadius()))
        {
            //std::cout<<"failure: Collision with particle "<<**it<<std::endl;
            return false;
        }
        else
        {
            //std::cout<<"No collision with particle "<<**it<<std::endl;
        }
    }
    return true;
    ///\todo tw check against periodic copies (see ShearCell3DInitialConditions.cpp)
}

/*!
 * \details Removes particles created by CheckAndDuplicatePeriodicParticle(int i, int nWallPeriodic)).
 *          Note that between these two functions it is not allowed to create additional functions
 * \image html Walls/periodicBoundary.pdf
 */
void DPMBase::removeDuplicatePeriodicParticles()
{
    for (unsigned int i = particleHandler.getNumberOfObjects(); i >= 1 && particleHandler.getObject(i - 1)->getPeriodicFromParticle() != nullptr; i--)
    {
        while (particleHandler.getObject(i - 1)->getInteractions().size() > 0)
        {
            interactionHandler.removeObjectKeepingPeriodics(particleHandler.getObject(i - 1)->getInteractions().front()->getIndex());
        }
        particleHandler.removeObject(i - 1);
    }
}

/*!
 * \image html Walls/periodicBoundary.pdf
 */  
void DPMBase::checkAndDuplicatePeriodicParticles()
{
    for (BaseBoundary* it : boundaryHandler) // For all pointers of type BaseBoundary in Boundary handler
    {
        unsigned int N = particleHandler.getNumberOfObjects(); //Required because the number of particles increases
        for (unsigned int i = 0; i < N; i++)
        {
            it->createPeriodicParticles(particleHandler.getObject(i), particleHandler);
        }
    }
}
/*!
 * \details Skims through all the object pointers of type BaseInteraction in the interaction handler. Outputs the type of
 *          interaction between two particles P and I.
 */
void DPMBase::outputInteractionDetails() const
{
    std::cout << "Interactions currently in the handler:" << std::endl;
    for (std::vector<BaseInteraction*>::const_iterator it = interactionHandler.begin(); it != interactionHandler.end(); ++it)
    {
        (*it)->write(std::cout);
        std::cout << std::endl;
        std::cout << "Interaction " << (*it)->getName() << " " << (*it)->getId() << " between " << (*it)->getP()->getId() << " and " << (*it)->getI()->getId() << std::endl;
    }
    /*std::cout << "Interactions currently in the particles:" << std::endl;
     for (std::vector<BaseParticle*>::const_iterator it = particleHandler.begin(); it != particleHandler.end(); ++it)
     {
     std::cout << "Base particle " << (*it)->getId() << " has interactions:" << std::endl;
     for (std::list<BaseInteraction*>::const_iterator it2 = (*it)->getInteractions().begin(); it2 != (*it)->getInteractions().end(); ++it2)
     {
     std::cout << (*it2)->getId() << " between " << (*it2)->getP()->getId() << " and " << (*it2)->getI()->getId() << std::endl;
     }
     }*/
}
/*!
 * \return bool (True or False)
 */
bool DPMBase::isTimeEqualTo(Mdouble time) const
{
    return getTime()<=time && getTime()+getTimeStep()>time;
}
