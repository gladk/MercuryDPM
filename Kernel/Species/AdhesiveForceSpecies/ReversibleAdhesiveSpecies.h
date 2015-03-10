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

#ifndef LINEARREVERSIBLEADHESIVESPECIES_H
#define LINEARREVERSIBLEADHESIVESPECIES_H
#include "Species/BaseSpecies.h"
#include "Math/ExtendedMath.h"
#include "Interactions/AdhesiveForceInteractions/ReversibleAdhesiveInteraction.h"

/*!
 * \brief ReversibleAdhesiveSpecies contains the parameters used to describe a linear reversible short-range force.
 * \details See ReversibleAdhesiveInteraction::computeForce for a description of the force law.
 */
class ReversibleAdhesiveSpecies : public virtual BaseSpecies
{
public:
    ///\brief The correct Interaction type for this AdhesiveForceSpecies
    typedef ReversibleAdhesiveInteraction InteractionType;

    ///\brief The default constructor.
    ReversibleAdhesiveSpecies();

    ///\brief The default constructor.
    ReversibleAdhesiveSpecies(const ReversibleAdhesiveSpecies &s);

    ///\brief The default constructor.
    virtual ~ReversibleAdhesiveSpecies();

    /// \brief Reads the species properties from an input stream.
    void read(std::istream& is);

    /// \brief Writes the species properties to an output stream.
    void write(std::ostream& os) const;

    /// \brief Used in Species::getName to obtain a unique name for each Species.
    std::string getBaseName() const;

    ///\brief creates default values for mixed species
    void mix(ReversibleAdhesiveSpecies* const S, ReversibleAdhesiveSpecies* const T);

//adhesion-specific functions

    ///\brief returns the largest separation distance at which adhesive short-range forces can occur.
    Mdouble getInteractionDistance() const;

//setters and getters
    ///\brief Allows the spring constant to be changed
    void setAdhesionStiffness(Mdouble new_k0);

    ///\brief Allows the spring constant to be accessed
    Mdouble getAdhesionStiffness() const;

    ///\brief Allows the spring constant to be changed
    void setAdhesionForceMax(Mdouble new_f0);

    ///\brief Allows the spring constant to be accessed
    Mdouble getAdhesionForceMax() const;

private:
    ///\brief stiffness of linear adhesion force
    Mdouble adhesionStiffness_; 

    ///\brief adhesion force at zero overlap
    Mdouble adhesionForceMax_;     
};
#endif
