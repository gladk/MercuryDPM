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

#ifndef INTERACTION_H
#define INTERACTION_H

#include "FrictionForceInteractions/EmptyFrictionInteraction.h"
#include "AdhesiveForceInteractions/EmptyAdhesiveInteraction.h"
#include "InteractionHandler.h"
#include "BaseInteractable.h"

class BaseInteractable;
template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies> class Species;

/*!
 * \class Interaction
 * \brief Contains information about the contact between two interactables, 
 * BaseInteraction::P_ and BaseInteraction::I_;.
 * \details An instance is created every time two BaseInteractables (particles 
 * or walls) get into contact with another, and gets deleted when the contact 
 * ends.
 * 
 * Next, the call process is described for this function for the case of two 
 * particles (it is very similar for a particle-wall contact.
 * 
 * Then, in each time step, every time that a contact is detected, 
 * DPMBase::computeInternalForces is called, which calls 
 * BaseParticle::getInteractionWith to create a new Interaction (setting 
 * the links to the interactables 
 * \link BaseInteraction::P_ P_\endlink and 
 * \link BaseInteraction::I_ I_\endlink 
 * and adds it to the InteractionHandler) or find an existing one. When a new 
 * Interaction is created, the Species determines what kind of Interaction it will be. 
 * DPMBase::computeInternalForces also sets the 
 * \link BaseInteraction::timeStamp_ timeStamp_\endlink, 
 * \link BaseInteraction::normal_ normal_\endlink, 
 * \link BaseInteraction::overlap_ overlap_\endlink, 
 * \link BaseInteraction::distance_ distance_\endlink, and 
 * \link BaseInteraction::contactPoint_ contactPoint_\endlink
 * of the Interaction. 
 * 
 * Then, computeForce is called, which sets the force_, which sets BaseInteraction::force_ and 
 * BaseInteraction::torque_ of the Interaction, and a few temporary values to communicate between 
 * the three different computeForce routines (see diamond inheritance below).
 * 
 * The force_ and torque_ is then used in DPMBase::integrateBeforeForceComputation 
 * and DPMBase::integrateAfterForceComputation to calcutate the new positions 
 * and velocities.
 * 
 * As there are many types of contact forces, the class is templated to allow 
 * for different force models. This is done in a diamond inheritance structure: 
 * First, three kinds of Interactions are created:
 * - NormalForceInteraction: Computes the normal contact force and sets the 
 *   torque to zero (we currently only have spherical particles). Also sets
 *   \link BaseInteraction::relativeVelocity_ relativeVelocity_\endlink, 
 *   \link BaseInteraction::normalRelativeVelocity_ normalRelativeVelocity_\endlink, 
 *   \link BaseInteraction::absoluteNormalForce_ absoluteNormalForce_\endlink.
 * - FrictionForceInteraction: Computes tangential contact force and torques.
 * - AdhesiveForceInteraction: Computes the short-range normal contact force.
 *
 * A full Interaction object is then derived by inheriting from all of the above:
 *  \dot
 *  digraph example {
 *      node [shape=record, fontname=Helvetica, fontsize=10];
 *      a [ label="class BaseInteraction" URL="\ref BaseInteraction"];
 *      f [ label="class ParticleInteraction" URL="\ref ParticleInteraction"];
 *      b [ label="class AdhesiveForceInteraction" URL="\ref AdhesiveForceInteraction"];
 *      c [ label="class FrictionForceInteraction" URL="\ref FrictionForceInteraction"];
 *      d [ label="class NormalForceInteraction" URL="\ref NormalForceInteraction"];
 *      e [ label="class Interaction" URL="\ref Interaction"];
 *      a -> b [ arrowhead="open" ];
 *      a -> c [ arrowhead="open" ];
 *      a -> d [ arrowhead="open" ];
 *      a -> f [ arrowhead="open" ];
 *      f -> e [ arrowhead="open" ];
 *      b -> e [ arrowhead="open" ];
 *      c -> e [ arrowhead="open" ];
 *      d -> e [ arrowhead="open" ];
 *  }
 *  \enddot
 * 
 */
//this class combines normal and tangential force laws
template<class NormalForceInteraction, class FrictionForceInteraction=EmptyFrictionInteraction, class AdhesiveForceInteraction=EmptyAdhesiveInteraction>
class Interaction : public NormalForceInteraction, public FrictionForceInteraction, public AdhesiveForceInteraction
{
public:

    ///\brief The default constructor.
    Interaction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);

    ///\brief The default copy constructor.
    Interaction(const Interaction &p);

    ///\brief The default destructor.
    virtual ~Interaction();

    ///\brief Creates a copy of this Interaction.
    Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>* copy() const final;

    ///\brief Computes the normal, tangential, and adhesive forces.
    void computeForce() final;

    ///\brief Read Interaction properties from a file.
    void read(std::istream& is) final;

    ///\brief Writes Interaction properties to a file.
    void write(std::ostream& os) const final;

    ///\brief Returns the name of the Interaction.
    std::string getName() const final;

    ///\brief Returns the elastic energy stored in the Interaction.
    Mdouble getElasticEnergy() const final;

    ///\brief Integrates the time-dependent parameters of the contact force.
    void integrate(Mdouble timeStep) final;

    ///\brief Reverses the parameters of the contact force.
    void reverseHistory() final;
};

template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::Interaction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
: BaseInteraction(P, I, timeStamp), NormalForceInteraction(P, I, timeStamp), FrictionForceInteraction(P, I, timeStamp), AdhesiveForceInteraction(P, I, timeStamp)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"Interaction::Interaction() finished"<<std::endl;
#endif
}

template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::Interaction(const Interaction &p)
: BaseInteraction(p), NormalForceInteraction(p), FrictionForceInteraction(p), AdhesiveForceInteraction(p)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"Interaction::Interaction(const Interaction &p finished"<<std::endl;
#endif
}

template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::~Interaction()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout<<"Interaction::~Interaction() finished"<<std::endl;
#endif
}

/*! 
 * \details Useful for polymorphism as it can be called from a BaseInteraction* pointer.
 */ 
template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>* Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::copy() const
{
    return new Interaction(*this);
}

/*!
 * \details Writes Interaction properties in human-readable form to a file, typically Files::restartFile_.
 * \param [out] os the ostream to which the data is written.
 */
template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
void Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::write(std::ostream& os) const
{
    NormalForceInteraction::write(os);
    FrictionForceInteraction::write(os);
    AdhesiveForceInteraction::write(os);
}

/*!
 * \details Reads Interaction properties in human-readable form from a file, typically Files::restartFile_.
 * \param [in] is the istream from which the data is read.
 */
template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
void Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::read(std::istream& is)
{
    NormalForceInteraction::read(is);
    FrictionForceInteraction::read(is);
    AdhesiveForceInteraction::read(is);
}

/*!
 * \details Called by BaseInteraction::copySwitchPointer to reverse the  
 * parameters of the contact force in the case that the interactables P_ and I_ 
 * are switched. This is needed to deal with periodic particles.
 */
template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
void Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::reverseHistory()
{
    NormalForceInteraction::reverseHistory();
    FrictionForceInteraction::reverseHistory();
    AdhesiveForceInteraction::reverseHistory();
}

/*!
 * \details Returns the name of the Interaction, which depends on the template parameters.
 * \return the string to which the name is written.
 */
template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
std::string Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::getName() const
{
    return NormalForceInteraction::getBaseName() + FrictionForceInteraction::getBaseName() + AdhesiveForceInteraction::getBaseName() + "Interaction";
}

/*!
 * \details Called by ??? to integrate time-dependent parameters of the 
 * contact force, such as the SlidingFrictionInteraction::slidingSpring_
 * \param[in] the time step.
 */
template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
void Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::integrate(Mdouble timeStep)
{
    FrictionForceInteraction::integrate(timeStep);
}

/*!
 * \details Computes the normal, tangential, and adhesive forces (in that order).
 * The order is important, as the normal force computation also calculates some 
 * additional parameters required by the other force laws:
 * \link BaseInteraction::relativeVelocity_ relativeVelocity_\endlink, 
 * \link BaseInteraction::normalRelativeVelocity_ normalRelativeVelocity_\endlink, 
 * \link BaseInteraction::absoluteNormalForce_ absoluteNormalForce_\endlink.
 */
template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
void Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::computeForce()
{
    NormalForceInteraction::computeNormalForce();
    FrictionForceInteraction::computeFrictionForce();
    AdhesiveForceInteraction::computeAdhesionForce();
}

/*!
 * \details Returns the elastic energy stored in the Interaction, adding up 
 * contributions from the normal, frictional and adhesive interaction
 * The elastic (=potential) energy is defined as the energy gained by separating P_ and I_.
 * \return the computed elastic energy.
 */
template<class NormalForceInteraction, class FrictionForceInteraction, class AdhesiveForceInteraction>
Mdouble Interaction<NormalForceInteraction, FrictionForceInteraction, AdhesiveForceInteraction>::getElasticEnergy() const
{
    return NormalForceInteraction::getElasticEnergy() + FrictionForceInteraction::getElasticEnergy() + AdhesiveForceInteraction::getElasticEnergy();
}
#endif
