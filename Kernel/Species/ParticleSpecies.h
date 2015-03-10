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

#ifndef ParticleSpecies_H
#define ParticleSpecies_H
#include "BaseSpecies.h"
#include "Math/ExtendedMath.h"
#include "Interactions/BaseInteraction.h"
//class SpeciesHandler;
class BaseInteractable;
class BaseInteraction;

//Note the getVelocity can for some Species be dependent on which point on the Species is meant.
class ParticleSpecies : public virtual BaseSpecies
{
public:
    typedef BaseInteraction InteractionType;

    ///\brief The default constructor.
    ParticleSpecies();

    ///\brief The default copy constructor.
    ParticleSpecies(const ParticleSpecies &p);

    ///\brief The default destructor.
    virtual ~ParticleSpecies();
    
    /*!
     * \brief Creates a deep copy of the object from which it is called.
     * \details See BaseSpecies::copy for details
     */
    virtual ParticleSpecies* copy() const=0;

    /*!
     * \brief Creates a new MixedSpecies with the same force properties as the 
     * Species from which it is called. See Species::copyMixed for details
     */
    virtual BaseSpecies* copyMixed() const=0;

    /// \brief Reads the species properties from an input stream.
    void read(std::istream& is);

    /// \brief Writes the species properties to an output stream.
    void write(std::ostream& os) const;

    /// \brief Used in Species::getName to obtain a unique name for each Species.
    std::string getBaseName() const;

    ///Allows the density to be changed
    ///\todo recalculate masses when setting dim_particle or rho
    void setDensity(Mdouble density);

    ///\todo TW: should getMassFromRadius be removed? IFCD: it is used in at 
    /// least one driver (AxisymmetricHopper).
    Mdouble getMassFromRadius(const Mdouble radius);

    ///Allows the density to be accessed
    Mdouble getDensity() const;
    
    /*!
     * \brief Compute Particle mass function, which required a reference to the Species vector. It computes the Particles mass, Inertia and the inverses.
     */
    virtual void computeMass(BaseParticle* p) const;

private:
    /*!
     * \brief The particle mass density. 
     */
    Mdouble density_;
};
#endif
