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

#ifndef LiquidBridgeWilletInteraction_H
#define LiquidBridgeWilletInteraction_H

#include "Interactions/BaseInteraction.h"
#include "Math/Vector.h"

class BaseParticle;
class LiquidBridgeWilletSpecies;
class BaseInteractable;
/*!
 * \class LiquidBridgeWilletInteraction
 * \brief Defines the liquid bridge willet interaction between two particles or walls.
 */
class LiquidBridgeWilletInteraction : public virtual BaseInteraction
{
public:
    /*!
     * \brief An alias name for LiquidBridgeWilletSpecies data type.
     */
    typedef LiquidBridgeWilletSpecies SpeciesType;
    /*!
     * \brief Constructor.
     */
    LiquidBridgeWilletInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);
    /*!
     * \brief Copy constructor.
     */
    LiquidBridgeWilletInteraction(const LiquidBridgeWilletInteraction &p);
    /*!
     * \brief Destructor.
     */
    virtual ~LiquidBridgeWilletInteraction();
    /*!
     * \brief Computes the adhesive forces for liquid bridge Willet type of interaction.
     */
    void computeAdhesionForce();
    /*!
     * \brief Interaction read function, which accepts an std::istream as input.
     */
    void read(std::istream& is);
    /*!
     * \brief Interaction print function, which accepts an std::ostream as input.
     */
    void write(std::ostream& os) const;
    /*!
     * \brief Returns the amount of Elastic energy involved in an interaction. Basically
     *        used in case you want to write the elastic energy into an output file. 
     */ 
    Mdouble getElasticEnergy() const;
    /*!
     * \brief A dynamic_cast of BaseSpecies type pointer to a pointer of type LiquidBridgeWilletSpecies.
     */
    const LiquidBridgeWilletSpecies* getSpecies() const;
    /*!
     * \brief Returns the name of the interaction, see Interaction.h.
     */  
    std::string getBaseName() const;
    
private:
    /*!
     * \brief A history parameter to store if the particles were in contact or not. Useful
     *        to compute adhesive forces.
     */    
    bool wasInContact_;
};
#endif
