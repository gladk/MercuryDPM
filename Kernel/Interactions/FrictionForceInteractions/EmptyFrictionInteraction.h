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

#ifndef EMPTYTANGENTIALINTERACTION_H
#define EMPTYTANGENTIALINTERACTION_H

#include "Interactions/BaseInteraction.h"
#include "Math/Vector.h"
class BaseParticle;
class EmptyFrictionSpecies;
class BaseInteractable;
/*!
 * \brief In case one wants to have a frictionless interaction between the interactables
 *        (particles or walls), the following class can be used. See Interaction.h, where
 *        one can set the FrictionalForceInteraction to EmptyFrictionInteraction.
 * \details All the member functions are just dummy methods which basically do nothing but
 *          define a interface.
 */
class EmptyFrictionInteraction : public virtual BaseInteraction
{
public:
    /*!
     * \brief An alias name for EmptyFrictionSpecies.
     */
    typedef EmptyFrictionSpecies SpeciesType;
    /*!
     * \brief Constructor.
     */    
    EmptyFrictionInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);
    /*!
     * \brief Copy constructor.
     */    
    EmptyFrictionInteraction(const EmptyFrictionInteraction &p);
    /*!
     * \brief Destructor.
     */    
    virtual ~EmptyFrictionInteraction();
    /*!
     * \brief Computes nothing, it is an empty function.
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
     * \brief Returns zero as it is an frictionless interaction.
     */
    Mdouble getElasticEnergy() const;
    /*!
     * \brief Returns a const pointer of type EmptyFrictionSpecies*. 
     */
    const EmptyFrictionSpecies* getSpecies() const;
    /*!
     * \brief Returns interaction name/type.
     */
    std::string getBaseName() const;

// specific for tangential forces
    /*!
     * \brief Does nothing as there is no force.
     */    
    void integrate(Mdouble timeStep);
    /*!
     * \brief Returns zero overlap.
     */
    Mdouble getTangentialOverlap() const;
    /*!
     * \brief Returns zero tangential force. 
     */
    const Vec3D getTangentialForce() const;
};
#endif
