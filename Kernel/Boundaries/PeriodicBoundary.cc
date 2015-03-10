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

#include "PeriodicBoundary.h"
#include "ParticleHandler.h"
#include "Particles/BaseParticle.h"

/*!
 * \details constructor
 */
PeriodicBoundary::PeriodicBoundary()
        : BaseBoundary()
{
    closestToLeftBoundary_=true;
    distanceLeft_ = std::numeric_limits<double>::quiet_NaN();
    distanceRight_= std::numeric_limits<double>::quiet_NaN();
    scaleFactor_= std::numeric_limits<double>::quiet_NaN();

#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"PeriodicBoundary::PeriodicBoundary() finished"<<std::endl;
#endif
}

/*!
 * \details destructor
 */
PeriodicBoundary::~PeriodicBoundary()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout<<"PeriodicBoundary::~PeriodicBoundary() finished"<<std::endl;
#endif   
}

/*!
 * \details Copy method; creates a copy on the heap and returns its pointer. 
 */
PeriodicBoundary* PeriodicBoundary::copy() const
{
    return new PeriodicBoundary(*this);
}

/*!
 * \details Defines the boundary, given a normal vector such that all particles 
 * are within {x: position_left<=normal*x<position_right}. The shift vector is set 
 * assuming that the domain is rectangular (shift parallel to normal).
 * \param[in] normal        The normal vector pointing from the left wall into the domain
 * \param[in] distanceLeft  The (signed) distance between the left wall and the origin
 * \param[in] distanceRight The (signed) distance between the right wall and the origin
 */
void PeriodicBoundary::set(Vec3D normal, Mdouble distanceLeft, Mdouble distanceRight)
{
    // factor is used to set normal to unit length
    scaleFactor_ = 1. / std::sqrt(Vec3D::dot(normal, normal));
    normal_ = normal * scaleFactor_;
    distanceLeft_ = distanceLeft * scaleFactor_;
    distanceRight_ = distanceRight * scaleFactor_;
    shift_ = normal_ * (distanceRight_ - distanceLeft_);
}

/*!
 * \details like PeriodicBoundary::set(normal, distanceLeft, distanceRight), but 
 * including the possibility of setting the shift direction vector.
 * \param[in]   normal The normal vector pointing from the left wall into the domain
 * \param[in]   distanceLeft The (signed) distance between the left wall and the origin
 * \param[in]   distanceRight The (signed) distance between the right wall and the origin
 * \param[in]   shiftDirection The vector over which particles will be shifted when 
 *              moving through the PeriodicBoundary
 */
void PeriodicBoundary::set(Vec3D normal, Mdouble distanceLeft, Mdouble distanceRight, Vec3D shiftDirection)
{
    // factor is used to set normal to unit length
    scaleFactor_ = 1. / std::sqrt(Vec3D::dot(normal, normal));
    normal_ = normal * scaleFactor_;
    distanceLeft_ = distanceLeft * scaleFactor_;
    distanceRight_ = distanceRight * scaleFactor_;
    // factor is used to set shift vector to correct length
    scaleFactor_ = (distanceRight_ - distanceLeft_) * Vec3D::dot(shiftDirection, normal_);
    shift_ = shiftDirection * scaleFactor_;
}
/*!
 * \details Allows the left periodic boundary to be moved to a new position and 
 * automatically changes its shift value
 * \param[in] distanceLeft  The distance (from the origin) to which the left 
 *                          boundary is moved
 */
void PeriodicBoundary::moveLeft(Mdouble distanceLeft)
{
    distanceLeft_ = distanceLeft * scaleFactor_;
    shift_ = normal_ * (distanceRight_ - distanceLeft_);
}

/*!
 * \details Allows the right periodic wall to be moved to a new position and 
 * automatically changes its shift value
 * \param[in] distanceRight     The distance (from the origin) to which the right 
 *                              boundary is moved
 */
void PeriodicBoundary::moveRight(Mdouble distanceRight)
{
    distanceRight_ = distanceRight * scaleFactor_;
    shift_ = normal_ * (distanceRight_ - distanceLeft_);
}

/*!
 * \details Returns the distance to the closest wall of the boundary to the particle,
 * , and sets closestToLeftBoundary_ = true if the left wall is the wall closest to the particle. 
 * Since this function should be called before calculating any Particle-Wall 
 * interactions, it can also be used to set the shift vector in case of curved walls.
 * \param[in] p     A reference to the particle which distance to the periodic 
 *                  boundary is calculated
 */
Mdouble PeriodicBoundary::getDistance(BaseParticle& p)
{
    return getDistance(p.getPosition());
}

/*!
 * \details Returns the distance to the wall closest to the position, and sets 
   * closestToLeftBoundary_ = true if the left wall is the wall closest to the position. 
 * \param[in] position  A reference to the position which distance to the periodic 
 *                      boundary is to be calculated
 */
Mdouble PeriodicBoundary::getDistance(const Vec3D& position)
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
 *      > PARTICLE's distance to either of the two walls is gotten by PeriodicBoundary::getDistance
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
 */
void PeriodicBoundary::shiftPosition(BaseParticle* p)
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

/*
 Vec3D PeriodicBoundary::getShiftedPosition(Vec3D &position)
 {
 if (left_wall)
 {
 return position + shift_;
 }
 else
 {
 return position - shift_;
 }
 }
 */

/*!
 * \details Shifts two given positions by the shift_ vector. 
 * \param[in] position1     The first position to be shifted
 * \param[in] position2     The second position to be shifted
 * \todo (AT) see toDo of PeriodicBoundary::shiftPosition().
 */
void PeriodicBoundary::shiftPositions(Vec3D& postition1, Vec3D& position2)
{
    if (closestToLeftBoundary_)
    {
        postition1 += shift_;
        position2 += shift_;
        closestToLeftBoundary_ = false;
    }
    else
    {
        postition1 -= shift_;
        position2 -= shift_;
        closestToLeftBoundary_ = true;
    }
}
\
/*
 * \details Returns TRUE if last particle/position checked is closest to the 'left' 
 * wall, and FALSE if it is closest to the 'right' wall. 
 * \return      value of closestToLeftBoundary_ data member
 */
bool PeriodicBoundary::isClosestToLeftBoundary() const
{
    return closestToLeftBoundary_;
}

/*
 void PeriodicBoundary::getCloseTogether(Vec3D &P, Vec3D &Q)
 {
 Mdouble PQdotn = Vec3D::Dot(P - Q, normal_);
 Mdouble shift_norm2 = shift.GetLength2();
 //Check if P is so far from Q that a shift would move it closer
 if (mathsFunc::square(PQdotn) > .25 * shift_norm2)
 {
 //Now determine the direction of the shift
 if (PQdotn > 0.0)
 P -= shift;
 else
 P += shift;
 }
 }
 */

/*!
 * \details Reads the boundary properties from an istream
 * \param[in] is        the istream
 */
void PeriodicBoundary::read(std::istream& is)
{
    BaseBoundary::read(is);
    std::string dummy;
    is >> dummy >> normal_
            >> dummy >> scaleFactor_
            >> dummy >> distanceLeft_
            >> dummy >> distanceRight_
            >> dummy >> shift_;
}

/*!
 * \details Deprecated version of read().
 * \deprecated Should be gone by Mercury 2.0. Instead, use CubeInsertionBoundary::read().
 */
void PeriodicBoundary::oldRead(std::istream& is)
{
    std::string dummy;
    is >> dummy >> normal_
            >> dummy >> scaleFactor_
            >> dummy >> distanceLeft_
            >> dummy >> distanceRight_
            >> dummy >> shift_;
}

/*!
 * \details Writes boundary's properties to an ostream
 * \param[in] os    the ostream
 */
void PeriodicBoundary::write(std::ostream& os) const
        {
    BaseBoundary::write(os);
    os << " normal " << normal_
            << " scaleFactor " << scaleFactor_
            << " distanceLeft " << distanceLeft_
            << " distanceRight " << distanceRight_
            << " shift " << shift_;
}

/*!
 * \details Returns the name of the object class
 * \return      the object's class' name, i.e. 'CubeInsertionBoundary'
 */
std::string PeriodicBoundary::getName() const
{
    return "PeriodicBoundary";
}

/*
 Vec3D& PeriodicBoundary::getNormal()
 {
 return normal_;
 }
 */

/*!
 * \details Checks the distance of given particle to the closest of both periodic 
 * walls, and creates a periodic copy of the particle if needed (i.e. if the particle
 * is closer to the periodic wall than the radius of the largest particle in the
 * system).
 * \param[in] p         Particle to be checked and possibly periodically copied
 * \param[in,out] pH    System's ParticleHandler, (1) from which the interaction radius
 *                      of its largest particle is retrieved to determine the maximum 
 *                      distance from the wall at which a particle should still have
 *                      a periodic copy created, and (2) to which a possible periodic
 *                      copy of the particle will be added
 */
void PeriodicBoundary::createPeriodicParticles(BaseParticle* p, ParticleHandler& pH)
{
    if (getDistance(*p) < p->getInteractionRadius() + pH.getLargestParticle()->getInteractionRadius())
    {
        //Step 1: Copy the particle to new ghost particle.
        BaseParticle*  pGhost = p->copy();
        
        //Step 2: Copy the interactions of the ghost particle.
        pGhost->copyInteractionsForPeriodicParticles(*p);

        //Step 3: Shift the ghost to the 'reflected' location.
        shiftPosition(pGhost);
                
        //Step 4: If Particle is double shifted, get correct original particle
        BaseParticle* from = p;
        while (from->getPeriodicFromParticle() != nullptr)
            from = from->getPeriodicFromParticle();
        pGhost->setPeriodicFromParticle(from);
        
        pH.addObject(pGhost);
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
 */
bool PeriodicBoundary::checkBoundaryAfterParticleMoved(BaseParticle* p, ParticleHandler& pH UNUSED)
{
    if (getDistance(*p) < 0)
    {
        shiftPosition(p);
    }
    return false;
}
