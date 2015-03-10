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

#ifndef EMPTYTANGENTIALSPECIES_H
#define EMPTYTANGENTIALSPECIES_H
#include "Species/BaseSpecies.h"
#include "Math/ExtendedMath.h"
class BaseInteractable;
class BaseInteraction;
class EmptyFrictionInteraction;

/*!
 * \brief EmptyFrictionSpecies is used to create a force law without frictional forces.
 */
class EmptyFrictionSpecies : public virtual BaseSpecies
{
public:
    ///\brief The correct Interaction type for this FrictionForceSpecies
    typedef EmptyFrictionInteraction InteractionType;

    ///\brief The default constructor.
    EmptyFrictionSpecies();

    ///\brief The default copy constructor.
    EmptyFrictionSpecies(const EmptyFrictionSpecies &s);

    ///\brief The default destructor.
    virtual ~EmptyFrictionSpecies();

    /// \brief Reads the species properties from an input stream.
    void read(std::istream& is);

    /// \brief Writes the species properties to an output stream.
    void write(std::ostream& os) const;

    /// \brief Used in Species::getName to obtain a unique name for each Species.
    std::string getBaseName() const;

    /*!
     * \brief Returns true if torques have to be calculated.
     */
    bool getUseAngularDOFs() const;

    ///\brief creates default values for mixed species
    void mix(EmptyFrictionSpecies* const S, EmptyFrictionSpecies* const T);
};
#endif
