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

#include "LeesEdwardsBoundary.h"
#include "ParticleHandler.h"
#include "Particles/BaseParticle.h"
#include "DPMBase.h"

/*!
 * \details Allows the user to set all the properties of the LeesEdwardsBoundary at once.
 * \param[in] velocity  The difference in velocities at which the bottom and to wall move
 * \param[in] left      The (signed) distance between the origin and the left wall
 * \param[in] right     The (signed) distance between the origin and the right wall
 * \param[in] down      The (signed) distance between the origin and the top wall
 * \param[in] up        The (signed) distance between the origin and the bottom wall
 */
void LeesEdwardsBoundary::set(std::function<double (double)> shift, std::function<double (double)> velocity, Mdouble left, Mdouble right, Mdouble down, Mdouble up)
{
    shift_ = shift;
    velocity_ = velocity;
    left_ = left;
    right_ = right;
    down_ = down;
    up_ = up;
}

/*!
 * \details  Reads all the properties of the LeesEdwardsBoundary from an std::istream
 * \param[in,out] is    The stream from which the parameters are read
 */
void LeesEdwardsBoundary::read(std::istream& is)
{
	std::string dummy;
    BaseBoundary::read(is);
    Mdouble shift;
    Mdouble velocity;
    is>>dummy>>left_>>dummy>>right_>>dummy>>down_>>dummy>>up_>>dummy>>shift>>dummy>>velocity;
    shift_=[shift] (double time UNUSED) { return shift;};
    velocity_=[velocity] (double time UNUSED) {return velocity;};
}

/*!
 * \details Writes all the properties of the LeesEdwardsBoundary to an std::ostream
 * \param[out] os       The stream to which the parameters are written
 */
void LeesEdwardsBoundary::write(std::ostream& os) const
{
    BaseBoundary::write(os);
    Mdouble time = getHandler()->getDPMBase()->getTime();
    os<<" left "<<left_<<" right "<<right_<<" down "<<down_<<" up "<<up_<<" shift "<<shift_(time)<<" vel "<<velocity_(time);
}

/*!
 * \return              The name of this object (i.e. "LeesEdwardsBoundary")
 */
std::string LeesEdwardsBoundary::getName() const
{
    return "LeesEdwardsBoundary";
}

/*!
 * \details Copy method; creates a copy on the heap and returns its pointer. 
 * \return              pointer to the copy on the heap
 */
LeesEdwardsBoundary* LeesEdwardsBoundary::copy() const
{
    return new LeesEdwardsBoundary(*this);
}

/*!
 * \details Returns the distance between BaseParticle p and the closest boundary wall in horizontal direction
 * \param[in] p             A reference to the BaseParticle
 * \param[out] positive     A boolean which is true when the left wall is closest
 * \return                  The distance to the closes wall in horizontal direction
 */
Mdouble LeesEdwardsBoundary::getHorizontalDistance(BaseParticle& p, bool& positive)
{
    Mdouble left = p.getPosition().X - left_;
    Mdouble right = right_ - p.getPosition().X;
    if (left < right)
    {
        positive = true;
        return left;
    } else
    {
        positive = false;
        return right;
    }
}

/*!
 * \details Returns the distance between BaseParticle p and the closest wall in vertical direction
 * \param[in] p             A reference to the BaseParticle
 * \param[out] positive     A boolean which is true when the bottom wall is closest
 * \return                  The distance to the closes wall in vertical direction
 */
Mdouble LeesEdwardsBoundary::getVerticalDistance(BaseParticle& p, bool& positive)
{
    Mdouble down = p.getPosition().Y - down_;
    Mdouble up = up_ - p.getPosition().Y;
    if (down < up)
    {
        positive = true;
        return down;
    } else
    {
        positive = false;
        return up;
    }
}

/*!
 * \details Shifts the BaseParticle p in horizontal direction to its 'periodic' position,
 * i.e. over the horizontal length of the boundary. 
 * \param[in] p         A reference to the BaseParticle that has to be shifted
 * \param[in] positive  A boolean which determines the direction of the shift
 *                      NB: TRUE if particle is closest to the left boundary wall
 */
void LeesEdwardsBoundary::shiftHorizontalPosition(BaseParticle* p, bool positive)
{
    if (positive)
    {
        p->move(Vec3D(right_ - left_, 0.0, 0.0));
    } else
    {
        p->move(Vec3D(left_ - right_, 0.0, 0.0));
    }
}

/*!
 * \details Shifts the BaseParticle p in vertical direction to its 'periodic' position,
 * i.e. over the vertical length of the boundary.
     * \param[in] p         A reference to the BaseParticle that has to be shifted
     * \param[in] positive  A boolean which determines the direction of the shift
 *                          NB: TRUE if particle is closest to the bottom boundary wall
 */
void LeesEdwardsBoundary::shiftVerticalPosition(BaseParticle* p, bool positive)
{
    Mdouble time = getHandler()->getDPMBase()->getTime();
    if (positive)
    {
        p->move(Vec3D(shift_(time), up_ - down_, 0.0));
        p->addVelocity(Vec3D(velocity_(time), 0.0, 0.0));
    } else
    {
        p->move(Vec3D(-shift_(time), down_ - up_, 0.0));
        p->addVelocity(Vec3D(-velocity_(time), 0.0, 0.0));
    }
}

/*!
 * \details Check if periodic copies of the particle in horizontal direction have 
 * to be made (i.e., if the distance of the particle to either of the horizontal 
 * boundary walls is smaller than its radius plus the radius of the largest particle 
 * in the system), and if so makes them
 * \param[in] p         A pointer to the BaseParticle that is checked
 * \param[out] pH       A reference to the ParticleHandler where periodic copies will be added
 */
void LeesEdwardsBoundary::createHorizontalPeriodicParticles(BaseParticle* p, ParticleHandler& pH)
{
    bool positive;      // TRUE if the particle is closest to the left boundary 
                        // wall (set by getVerticalDistance in the following if-statement)
    // check if particle is close enough to either of the walls
    if (getHorizontalDistance(*p, positive) < p->getInteractionRadius() + pH.getLargestParticle()->getInteractionRadius())
    {
        // create a periodic copy of the particle
        BaseParticle* F0 = p->copy();
        pH.addObject(F0);
        F0->copyInteractionsForPeriodicParticles(*p);

        // If Particle is doubly shifted, get correct original particle
        BaseParticle* From = p;
        while (From->getPeriodicFromParticle() != nullptr)
            From = From->getPeriodicFromParticle();
        F0->setPeriodicFromParticle(From);
        
        // shift the copy to the 'periodic' position
        shiftHorizontalPosition(F0, positive);
        
        // NB: No extra creation of possible vertical copies of the horizontal copy 
        // here (as compared to createVerticalPeriodicParticles), because these would
        // overlap with the extra creation of horizontal copies in createVerticalPeriodicParticles.
    }
}

/*!
 * \details Check if periodic copies of the particle in vertical direction have 
 * to be made (i.e., if the distance of the particle to either of the vertical 
 * boundary walls is smaller than its radius plus the radius of the largest particle 
 * in the system), and if so makes them
 * \param[in] p         A pointer to the BaseParticle that is checked
 * \param[out] pH       A reference to the ParticleHandler where periodic copies will be added
 */
void LeesEdwardsBoundary::createVerticalPeriodicParticles(BaseParticle* p, ParticleHandler& pH)
{
    bool positive;      // TRUE if the particle is closest to the bottom boundary 
                        // wall (set by getVerticalDistance in the following if-statement)
    // check if particle is close enough to either of the walls
    if (getVerticalDistance(*p, positive) < p->getInteractionRadius() + pH.getLargestParticle()->getInteractionRadius())
    {   
        // create a periodic copy of the particle
        BaseParticle* F0 = p->copy();
        pH.addObject(F0);
        F0->copyInteractionsForPeriodicParticles(*p);

        // If Particle is doubly shifted, get correct original particle
        BaseParticle* From = p;
        while (From->getPeriodicFromParticle() != nullptr)
            From = From->getPeriodicFromParticle();
        F0->setPeriodicFromParticle(From);
        
        // shift the copy to the 'periodic' position
        shiftVerticalPosition(F0, positive);
        while (getHorizontalDistance(*F0, positive) < 0)
        {
            shiftHorizontalPosition(F0, positive);
        }
        
        // Create horizontal periodic copies of the copy particle, if needed (i.e.,
        // if the original particle is in one of the boundary corners).
        createHorizontalPeriodicParticles(pH.getLastObject(), pH);
    }
}

/*!
 * \details Check if periodic copies of the particle have to be made for the contact detection and if so makes them
 * \param[in] p         A pointer to the BaseParticle that is checked
 * \param[out] pH       A reference to the ParticleHandler where periodic copies will be added
 */
void LeesEdwardsBoundary::createPeriodicParticles(BaseParticle* p, ParticleHandler& pH)
{
    createVerticalPeriodicParticles(p, pH);
    createHorizontalPeriodicParticles(p, pH);
}

/*!
 * \details Checks if the particle has to be shifted into the main domain (i.e., 
 * if the particle has crossed any of the boundary walls) and if so performs the shift
 * \param[in] p     A pointer to the BaseParticle that is checked
 */
bool LeesEdwardsBoundary::checkBoundaryAfterParticleMoved(BaseParticle* p, ParticleHandler& pH UNUSED)
{
    bool positive;
    while (getVerticalDistance(*p, positive) < 0)
    {
        shiftVerticalPosition(p, positive);
    }
    while (getHorizontalDistance(*p, positive) < 0)
    {
        shiftHorizontalPosition(p, positive);
    }
    return false;
}
