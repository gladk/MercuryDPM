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


#include "MaserBoundary.h"
#include "DPMBase.h"
#include "BoundaryHandler.h"
#include "ParticleHandler.h"
#include "Particles/BaseParticle.h"
#include "Species/ParticleSpecies.h"
#include <Logger.h>

/*!
 * \details Copy method, creates a copy of the object on the heap and returns a 
 * pointer to it.
 * \return      pointer to the copy
 */
MaserBoundary* MaserBoundary::copy() const
{
    return new MaserBoundary(*this);
}

/*!
 * \details Set all the properties of the boundary at once.
 * \param[in] normal            Normal unit vector of the (parallel) boundary walls
 * \param[in] distanceLeft      The distance of the left wall to the origin
 * \param[in] distanceRight     The distance of the right wall to the origin
 */
void MaserBoundary::set(Vec3D normal, Mdouble distanceLeft, Mdouble distanceRight)
{
    // factor is used to set normal to unit length
    normal_ = normal;
    distanceLeft_ = distanceLeft;
    distanceRight_ = distanceRight;
    shift_ = normal_ * (distanceRight_ - distanceLeft_);
}

/*!
 * \details Reads the boundary properties from an istream
 * \param[in,out] is        the istream
 */
void MaserBoundary::read(std::istream& is)
{
    BaseBoundary::read(is);
}

/*!
 * \details Writes boundary's properties to an ostream
 * \param[in] os    the ostream
 */
void MaserBoundary::write(std::ostream& os) const
{
    BaseBoundary::write(os);
}

/*!
 * \details Returns the name of the object class
 * \return      the object's class' name, i.e. 'MaserBoundary'
 */
std::string MaserBoundary::getName() const
{
    return "MaserBoundary";
}

/*!
 * \details Returns the distance to the closest wall of the boundary to the particle,
 * , and sets closestToLeftBoundary_ = true if the left wall is the wall closest to the particle. 
 * \param[in] p     A reference to the particle of which the distance to the  
 *                  boundary wall is to be calculated
 */
Mdouble MaserBoundary::getDistance(BaseParticle& p)
{
    return getDistance(p.getPosition());
}

/*!
 * \details Returns the distance to the wall closest to the position, and sets 
   * closestToLeftBoundary_ = true if the left wall is the wall closest to the position. 
 * \param[in] position  (A reference to) the position of which the distance to the  
 *                      boundary wall is to be calculated
 */
Mdouble MaserBoundary::getDistance(const Vec3D& position)
{
    Mdouble distance = Vec3D::dot(position, normal_);

    if (distance - distanceLeft_ < distanceRight_ - distance)
    {
        closestToLeftBoundary_ = true;
        return distance - distanceLeft_;
    }
    else
    {
        closestToLeftBoundary_ = false;
        return distanceRight_ - distance;
    }
}

/*!
 * \details Shifts the particle (using the closestToLeftBoundary_ value)
 * \param[in] p         A pointer to the particle which will be shifted.
 * \todo Reconsider the way in which the shifting of particles is done. Now,
 * the procedure is as follows:
 *      > PARTICLE's distance to either of the two walls is gotten by MaserBoundary::getDistance
 *      > Which of the two walls the particle is closest to, is saved in the BOUNDARY's 
 *        boolean data member closestToLeftBoundary_
 *      > PARTICLE's position is then shifted based on the BOUNDARY's closestToLeftBoundary_
 *        data member and shift vector
 * What might go wrong, is that it is allowed now to do a shift operation on the particle without
 * being forced to first have the closestToLeftBoundary_ boolean set based on the same
 * particle. 
 * Suggestion: get rid of the closestToLeftBoundary_ data member all together (it is
 * a property of the particle after all, rather than one of the boundary), and implement 
 * the obtaining of which boundary to shift relatively to directly into the shift function.
 * (NB: The PeriodicBoundary class contains the same TODO).
 */
void MaserBoundary::shiftPosition(BaseParticle* p)
{
    if (closestToLeftBoundary_)
    {
        p->move(shift_);
        closestToLeftBoundary_ = false;
    }
    else
    {
        p->move(-shift_);
        closestToLeftBoundary_ = true;
    }
}

/*!
 * \details Checks the distance of given particle to the closest of both  
 * walls, and creates a periodic copy of the particle if needed (i.e. if the particle
 * is closer to the periodic wall than the radius of the largest particle in the
 * system).
 * \param[in] P         Particle to be checked and possibly periodically copied
 * \param[in,out] pH    System's ParticleHandler, (1) from which the interaction radius
 *                      of its largest particle is retrieved to determine the maximum 
 *                      distance from the wall at which a particle should still have
 *                      a periodic copy created, and (2) to which a possible periodic
 *                      copy of the particle will be added
 */
void MaserBoundary::createPeriodicParticles(BaseParticle* P, ParticleHandler& pH)
{
    // check if particle is close to either of the boundary walls
    if (getDistance(*P) < P->getInteractionRadius() + pH.getLargestParticle()->getInteractionRadius())
    {
        auto conversion = speciesConversionMaserToNormal_.find(P->getSpecies());
        if (conversion != speciesConversionMaserToNormal_.end()) //Test if it is a maser particle
        {
            // Copy the particle if it IS a maser particle
            BaseParticle* F0 = P->copy();
            F0->copyInteractionsForPeriodicParticles(*P);
            
            // shift to the periodic location
            shiftPosition(F0);

            //Set the 'from' particle. If Particle is multiply shifted, get correct original particle
            BaseParticle* From = P;
            while (From->getPeriodicFromParticle() != nullptr)
            {
                From = From->getPeriodicFromParticle();
            }
            F0->setPeriodicFromParticle(From);
            
            // add the periodic particle to the handler
            pH.addObject(F0);
            
            /*!
             * \todo The code has not implemented the actual outflow of 'normal' particles
             * from the right boundary wall. I.e., the maser now is 'simply' a 
             * normal Periodic Boundary, with a lot of potential ;). I think 
             * the 'original author' planned to implement the outflow below, which 
             * is still left to be done. (BvdH)
             * NB: a better place would be in the MaserBoundary::shiftPosition() 
             * or MaserBoundary::checkBoundaryAfterParticleMoved() method, which are
             * called when the particles actually cross either of the boundary
             * walls.
             */
            if (closestToLeftBoundary_)
            {   
            }
            else
            {
            }
        }
    }
}

/*!
 * \details Checks whether given particle has crossed the closest wall, and if so,
 * shifts its position so as to have it appear at the other wall
 * \param[in] p         The particle to be checked and possibly shifted
 * \param pH            The ParticleHandler, which is unused in this implementation
 * \return              ALWAYS returns FALSE for periodic boundaries. Only returns 
 *                      TRUE when particle gets actually deleted, e.g. in certain 
 *                      DeletionBoundary implementations.
 * 
 * \todo Apparently, somewhere in the ages past, at some time, there has existed a 
 * piece of code which prevents the particles crossing the periodic boundary on the 
 * left side from reappearing on the right. This was taken out somewhere between the 
 * dark ages an the current one (feb. 2015) and should be put back in. Only causes 
 * errors very rarely, and not in the compiler but rather in the actual physics 
 * going on (So its not detected by the MaserSelfTest). 
 * The current arrangement mainly causes problems with particles which oscillate
 * on the boundary, continuously crossing from left to right and back. A visual
 * consideration of this problem can be found in Documentation/Images/Maser_unperiodic_behaviour.jpg,
 * which, granted, is appalling in its own right, but has yet to suffice in awaitance
 * of a better (i.e. clearer) drawing. Therefore:
 * \todo Create a new diagram explaining the problems with the previous (NOW: CURRENT!) 
 * implementation, and the exact differences between the two. I.e., an improved version
 * of Documentation/Images/Maser_unperiodic_behaviour.jpg. (BvdH)
 */
bool MaserBoundary::checkBoundaryAfterParticleMoved(BaseParticle* P, ParticleHandler& pH UNUSED)
{
    // check if particle passed either of the boundary walls
    if (getDistance(*P) < 0)
    {
        // check if species is found in the list of maser particle species
        auto conversion = speciesConversionMaserToNormal_.find(P->getSpecies());
        if (conversion != speciesConversionMaserToNormal_.end()) //Test if it is a maser particle
        {
            // yes: shift it periodically!
            shiftPosition(P);
            
            /*
            // check if to be shifted leftward or rightward. Now done by MaserBoundary::shiftPosition().
            if (closestToLeftBoundary_)
            {
                P->move(shift_);
                closestToLeftBoundary_ = false;
            }
            else
            {
                P->move(-shift_);
                closestToLeftBoundary_ = true;
                //BaseParticle* pCopy = pH.copyAndAddObject(P);
                //pCopy->setIndSpecies(conversion->second);
            }
             */
        }
    }
    return false;
}

/*!
 * \details Turns given particle into a 'maser particle' by changing its species into 
 * a 'maser particle' copy species. If the particle species is not yet in the std::map 
 * speciesConversionNormalToMaser_, it and its maser copy species are added.
 * This function should be called at the beginning of the simulation, right after actually
 * filling the maser with particles, flagging each particle as one belonging to the 
 * maser. 
 * \param[in,out] P     The particle which is added to the maser. Its species is 'changed'
 *                      to the maser copy species.
 */
void MaserBoundary::addParticleToMaser(BaseParticle* P)
{
    // check if particle species already known by the maser
    auto conversion = speciesConversionNormalToMaser_.find(P->getSpecies());
    if (conversion == speciesConversionNormalToMaser_.end())
    {
        // nope: species has to be added to the list
        ParticleSpecies* newSpecies = getHandler()->getDPMBase()->speciesHandler.copyAndAddObject(P->getSpecies()->copy());
        speciesConversionNormalToMaser_.insert(std::pair<const ParticleSpecies*, const ParticleSpecies*>(P->getSpecies(), newSpecies));
        speciesConversionMaserToNormal_.insert(std::pair<const ParticleSpecies*, const ParticleSpecies*>(newSpecies, P->getSpecies()));
        logger(INFO,"[MaserBoundary::addParticleToMaser()] New species conversion created");
        
        // now the species IS added, so flag (convert) it!
        P->setSpecies(newSpecies);
    }
    else
    {
        // yep: species known and flagged (i.e. 'converted')
        logger(INFO,"[MaserBoundary::addParticleToMaser()] Species conversion already present");
        P->setSpecies(conversion->second);
    }
}

