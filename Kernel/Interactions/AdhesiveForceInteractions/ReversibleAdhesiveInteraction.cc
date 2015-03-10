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


#include "ReversibleAdhesiveInteraction.h"
#include "Species/AdhesiveForceSpecies/ReversibleAdhesiveSpecies.h"
#include "Particles/BaseParticle.h"
#include "InteractionHandler.h"
#include <iomanip>
#include <fstream>

ReversibleAdhesiveInteraction::ReversibleAdhesiveInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
    : BaseInteraction(P, I, timeStamp)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"ReversibleAdhesiveInteraction::ReversibleAdhesiveInteraction() finished"<<std::endl;
#endif
}

ReversibleAdhesiveInteraction::ReversibleAdhesiveInteraction(const ReversibleAdhesiveInteraction &p)
    : BaseInteraction(p)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"ReversibleAdhesiveInteraction::ReversibleAdhesiveInteraction(const ReversibleAdhesiveInteraction &p finished"<<std::endl;
#endif
}

ReversibleAdhesiveInteraction::~ReversibleAdhesiveInteraction()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout<<"ReversibleAdhesiveInteraction::~ReversibleAdhesiveInteraction() finished"<<std::endl;
#endif
}

void ReversibleAdhesiveInteraction::write(std::ostream& os  UNUSED) const
{}

void ReversibleAdhesiveInteraction::read(std::istream& is  UNUSED)
{}

void ReversibleAdhesiveInteraction::computeForce()
{
    //std::cout << "ReversibleAdhesiveInteraction::computeForce" << std::endl;
    const ReversibleAdhesiveSpecies* species = getSpecies();

    if (getOverlap()>=0)
        addForce(getNormal() * (-species->getAdhesionForceMax()));
    else
        addForce(getNormal() * (-species->getAdhesionStiffness() *getOverlap() - species->getAdhesionForceMax()));
}

Mdouble ReversibleAdhesiveInteraction::getElasticEnergy() const
{
    ///\todo TW
    return 0.0;
}

const ReversibleAdhesiveSpecies *ReversibleAdhesiveInteraction::getSpecies() const
{
    return dynamic_cast<const ReversibleAdhesiveSpecies *>(getBaseSpecies());
}

std::string ReversibleAdhesiveInteraction::getName() const
{
    return "ReversibleAdhesive";
}
