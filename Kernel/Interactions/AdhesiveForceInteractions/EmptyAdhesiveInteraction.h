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

#ifndef EMPTYADHESIVEINTERACTION_H
#define EMPTYADHESIVEINTERACTION_H

#include "Interactions/BaseInteraction.h"
#include "Math/Vector.h"
class BaseParticle;
class EmptyAdhesiveSpecies;
class BaseInteractable;
/*!
 * \class EmptyAdhesiveInteraction
 * \brief In case one doesn't want to have an adhesive (short range non contact) interaction between the interactables
 *        (particles or walls), the following class can be used. See Interaction.h, where one can set the Adhesive 
 *        interaction to EmptyAdhesiveInteraction.
 */
class EmptyAdhesiveInteraction : public virtual BaseInteraction
{
public:
    /*!
     * \brief An alias name for EmptyAdhesiveSpecies data type.
     */
    typedef EmptyAdhesiveSpecies SpeciesType;
    /*!
     * \brief Constructor.
     */
    EmptyAdhesiveInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);
    /*!
     * \brief copy constructor.
     */
    EmptyAdhesiveInteraction(const EmptyAdhesiveInteraction &p);
    /*!
     * \brief Destructor.
     */
    virtual ~EmptyAdhesiveInteraction();
    /*!
     * \brief Computes nothing as it is an Empty Adhesive interaction.
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
     * \brief A dynamic_cast of BaseSpecies pointer type to a pointer to an object of 
     *        type EmptyAdhesiveSpecies.
     */
    const EmptyAdhesiveSpecies* getSpecies() const;
    /*!
     * \brief Returns the name of the interaction, see Interaction.h.
     */
    std::string getBaseName() const;
};
#endif
