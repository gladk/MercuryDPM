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

#ifndef FrictionInteraction_H
#define FrictionInteraction_H

#include "SlidingFrictionInteraction.h"
#include "Math/Vector.h"

class BaseParticle;
class FrictionSpecies;
class BaseInteractable;
/*!
 * \class FrictionInteraction
 * \brief This class allows one to take all three types of frictional interactions 
 *        into account. The sliding, rolling and torsional frictional interaction.
 *        See 
 */
class FrictionInteraction : public SlidingFrictionInteraction
{
public:
    /*!
     * \brief An alias for FrictionSpecies.
     */
    typedef FrictionSpecies SpeciesType;
    /*!
     * \brief Constructor.
     */    
    FrictionInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);
    /*!
     * \brief Copy constructor.
     */
    FrictionInteraction(const FrictionInteraction &p);
    /*!
     * \brief Destructor.
     */
    virtual ~FrictionInteraction();
    /*!
     * \brief Computes the forces arising due to all three types of friction, i.e.,
     *        sliding, rolling and torsional.
     */
    void computeFrictionForce();
    /*!
     * \brief Interaction read function, which accepts an std::istream as input.
     */    
    void read(std::istream& is);
    /*!
     * \brief Interaction print function, which accepts an std::ostream as input.
     */    
    void write(std::ostream& os) const;
    /*!
     * \brief Computes the amount of compression in all the springs, i.e., increments the rollingSpring_,
     *        slidingSpring_ (see SlidingFrictionInteraction.cc) and torsionSpring_. 
     */    
    void integrate(Mdouble timeStep);
    /*!
     * \brief Returns the global amount of energy stored in all the springs (rolling, sliding and torsional).
     */
    Mdouble getElasticEnergy() const;
    /*!
     * \brief Returns interaction name/type.
     */
    std::string getBaseName() const;
    /*!
     * \brief Returns a const pointer of type FrictionSpecies*.
     */
    const FrictionSpecies* getSpecies() const;
    /*!
     * \brief A useful feature if one wants to return to the initial state of the springs. However, 
     *        reverse history decrements the current state to the state corresponding to previous time step. 
     *        Decrements the state or value of rollingSpring_, torsionSpring_ and slidingSpring_.
     */
    void reverseHistory();

private:
    /*!
     * \brief Stores the amount of rolling spring compression. Set in integrate(), used in computing
     *        frictional force due to rolling.
     */    
    Vec3D rollingSpring_;
    /*!
     * \brief Stores the rate at which the rolling spring compresses or relaxes. Set in computeFrictionForce(), used in
     *        computing the amount of compression in rolling spring. Used in integrate().
     */
    Vec3D rollingSpringVelocity_;
    /*!
     * \brief Stores the amount of torsional spring compression. Set in integrate(), used in computing
     *        frictional force due to torsion.
     */
    Vec3D torsionSpring_;
    /*!
     * \brief Stores the rate at which the torsional spring compresses or relaxes. Set in computeFrictionForce(), used in
     *        computing the amount of compression in torsion spring. Used in integrate().
     */   
    Vec3D torsionSpringVelocity_;
};
#endif
