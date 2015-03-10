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

#ifndef SLIDINGFRICTIONINTERACTION_H
#define SLIDINGFRICTIONINTERACTION_H

#include "Interactions/BaseInteraction.h"
#include "Math/Vector.h"

class BaseParticle;
class SlidingFrictionSpecies;
class BaseInteractable;
/*!
 * \class SlidingFrictionInteraction
 * \brief Computes the forces corresponding to sliding friction.
 */
class SlidingFrictionInteraction : public virtual BaseInteraction
{
public:
    /*!
     * \brief An alias name for SlidingFrictionSpecies data type.
     */
    typedef SlidingFrictionSpecies SpeciesType;
    /*!
     * \brief Constructor.
     */
    SlidingFrictionInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);
    /*!
     * \brief Copy constructor.
     */
    SlidingFrictionInteraction(const SlidingFrictionInteraction &p);
    /*!
     * \brief Destructor.
     */
    virtual ~SlidingFrictionInteraction();
    /*!
     * \brief Computes the tangential force generated due to compression in the sliding spring.
     *        Does take into account if the interaction is between particle-particle or particle-wall.
     */
    void computeFrictionForce();
    /*!
     * \brief Interaction read function, which accepts an std::istream as input.
     */
    void read(std::istream& is);
    /*!
     * \brief Interaction write function, which accepts an std::ostream as input.
     */
    void write(std::ostream& os) const;
    /*!
     * \brief Increments the amount of compression in sliding spring.
     */    
    void integrate(Mdouble timeStep);
    /*!
     * \brief Returns the amount of elastic energy stored in sliding spring.
     */
    Mdouble getElasticEnergy() const;
    /*!
     * \brief Returns the amount of tangential overlap which is needed by BaseInteraction::writeToFstat().
     */  
    Mdouble getTangentialOverlap() const;
    /*!
     * \brief Returns the type/name of interaction (sliding friction interaction)
     */
    std::string getBaseName() const;

    //setters and getters
    /*!
     * \brief Returns the sliding friction force vector.
     */
    const Vec3D getTangentialForce() const;
    /*!
     * \brief Returns a const pointer of type SlidingFrictionSpecies*
     */
    const SlidingFrictionSpecies* getSpecies() const;
    /*!
     * \brief A useful feature if one wants to return to the initial state of the spring. However, 
     *        reverse history decrements the current state to the state corresponding to previous time step. 
     *        Decrements the value of slidingSpring_.
     */
    void reverseHistory();

private:
    /*!
     * \brief Stores the amount of sliding spring (\f$\delta\f$) compression from the expression \f$f_t=-k*\delta-\nu*relVel\f$.
     *        Set in the member function integrate(), used in computeFrictionForce().
     */    
    Vec3D slidingSpring_;
    /*!
     * \brief Stores the rate at which the sliding spring compressed or relaxed. Set in the member function
     *        computeFrictionForce() and used in integrate().
     */
    Vec3D slidingSpringVelocity_;
    /*!
     * \brief Computes the tangential force such that \f$|f_t|=\mu*|f_n|\f$. Set and computed in computeFrictionForce().
     */
    Vec3D tangentialForce_;
};
#endif
