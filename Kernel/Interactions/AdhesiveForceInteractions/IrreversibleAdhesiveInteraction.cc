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


#include "IrreversibleAdhesiveInteraction.h"
#include "Species/AdhesiveForceSpecies/IrreversibleAdhesiveSpecies.h"
#include "Particles/BaseParticle.h"
#include "InteractionHandler.h"
#include <iomanip>
#include <fstream>

/*!
 * \param[in] P
 * \param[in] I
 * \param[in] timeStamp
 */
IrreversibleAdhesiveInteraction::IrreversibleAdhesiveInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
    : BaseInteraction(P, I, timeStamp),  ReversibleAdhesiveInteraction(P, I, timeStamp)
{
    wasInContact_=false;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"IrreversibleAdhesiveInteraction::IrreversibleAdhesiveInteraction() finished"<<std::endl;
#endif
}

/*!
 * \param[in] p
 */
IrreversibleAdhesiveInteraction::IrreversibleAdhesiveInteraction(const IrreversibleAdhesiveInteraction &p)
    : BaseInteraction(p),  ReversibleAdhesiveInteraction(p)
{
	///\todo tw check if the parameters are valid when inserting the species into the handler
    wasInContact_=p.wasInContact_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"IrreversibleAdhesiveInteraction::IrreversibleAdhesiveInteraction(const IrreversibleAdhesiveInteraction &p finished"<<std::endl;
#endif
}
/*!
 *
 */
IrreversibleAdhesiveInteraction::~IrreversibleAdhesiveInteraction()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout<<"IrreversibleAdhesiveInteraction::~IrreversibleAdhesiveInteraction() finished"<<std::endl;
#endif
}
/*!
 * \param[in,out] os
 */
void IrreversibleAdhesiveInteraction::write(std::ostream& os) const
{
    os << " wasInContact " << wasInContact_;
}
/*!
 * \param[in,out] is
 */
void IrreversibleAdhesiveInteraction::read(std::istream& is)
{
    std::string dummy;
    is >> dummy >> wasInContact_;
}
/*!
 * \details Uses the most basic adhesion contact model.
 */
void IrreversibleAdhesiveInteraction::computeAdhesionForce()
{
    const IrreversibleAdhesiveSpecies* species = getSpecies();
    if (getOverlap()>=0)
    {
        wasInContact_=true;
        addForce(getNormal() * (-species->getAdhesionForceMax()));
    }
    else if (wasInContact_)
    {
        addForce(getNormal() * (-species->getAdhesionStiffness() *getOverlap() - species->getAdhesionForceMax()));
    }
}
/*!
 * \return const pointer of IrreversibleAdhesiveSpecies*
 */
const IrreversibleAdhesiveSpecies* IrreversibleAdhesiveInteraction::getSpecies() const
{
    return dynamic_cast<const IrreversibleAdhesiveSpecies *>(getBaseSpecies());
}
/*!
 * \return std::string
 */
std::string IrreversibleAdhesiveInteraction::getBaseName() const
{
    return "IrreversibleAdhesive";
}

/*!
 * \details Elastic (=potential) energy is defined as the energy gained by separating two interactables.
 * As it costs energy to separate adhesive interactables, the elastic energy is negative.
 * \return the elastic energy stored in the adhesive spring. 
 */
Mdouble IrreversibleAdhesiveInteraction::getElasticEnergy() const
{
    if (!wasInContact_)
        return 0.0;
    else if (getOverlap() <= 0)
        return -0.5 * getSpecies()->getAdhesionStiffness() * mathsFunc::square(getOverlap() + getSpecies()->getInteractionDistance());
    else
        return -getSpecies()->getAdhesionForceMax() * getOverlap()
        - 0.5 * getSpecies()->getAdhesionStiffness() * mathsFunc::square(getSpecies()->getInteractionDistance());
}