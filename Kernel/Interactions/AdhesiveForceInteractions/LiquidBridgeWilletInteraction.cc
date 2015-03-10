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


#include "LiquidBridgeWilletInteraction.h"
#include "Species/AdhesiveForceSpecies/LiquidBridgeWilletSpecies.h"
#include "Particles/BaseParticle.h"
#include "InteractionHandler.h"
#include <iomanip>
#include <fstream>

LiquidBridgeWilletInteraction::LiquidBridgeWilletInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
    : BaseInteraction(P, I, timeStamp)
{
    wasInContact_=false;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LiquidBridgeWilletInteraction::LiquidBridgeWilletInteraction() finished"<<std::endl;
#endif
}

LiquidBridgeWilletInteraction::LiquidBridgeWilletInteraction(const LiquidBridgeWilletInteraction &p)
    : BaseInteraction(p)
{
    wasInContact_=p.wasInContact_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LiquidBridgeWilletInteraction::LiquidBridgeWilletInteraction(const LiquidBridgeWilletInteraction &p finished"<<std::endl;
#endif
}

LiquidBridgeWilletInteraction::~LiquidBridgeWilletInteraction()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout<<"LiquidBridgeWilletInteraction::~LiquidBridgeWilletInteraction() finished"<<std::endl;
#endif
}

void LiquidBridgeWilletInteraction::write(std::ostream& os  UNUSED) const
{
	os << " wasInContact " << wasInContact_;
}

void LiquidBridgeWilletInteraction::read(std::istream& is  UNUSED)
{
    // we read in wasInContact_ like this because an early version did not initialize it.
    std::string dummy;
    int dummy2;
    is >> dummy >> dummy2;
    if (dummy2==1)
        wasInContact_ = true;
    else
        wasInContact_ = false;
}

void LiquidBridgeWilletInteraction::computeForce()
{
    const LiquidBridgeWilletSpecies* species = getSpecies();
    if (getOverlap()>=0)
    {
        wasInContact_=true;
		Mdouble effectiveRadius = getEffectiveRadius();
		Mdouble fdotn = -2.0*constants::pi*effectiveRadius*species->getSurfaceTension()*std::cos(species->getContactAngle());
        addForce(getNormal() * fdotn);
    }
    else if (wasInContact_)
    {
		Mdouble effectiveRadius = getEffectiveRadius();
		Mdouble s_c = -getOverlap()*std::sqrt(effectiveRadius/species->getLiquidBridgeVolume());
		Mdouble fdotn = -2.0*constants::pi*effectiveRadius*species->getSurfaceTension()
			*std::cos(species->getContactAngle())/(1+(1.05+2.5*s_c)*s_c);
        addForce(getNormal() * fdotn);
    }
}

Mdouble LiquidBridgeWilletInteraction::getElasticEnergy() const
{
    ///\todo TW
    return 0.0;
}

const LiquidBridgeWilletSpecies *LiquidBridgeWilletInteraction::getSpecies() const
{
    return dynamic_cast<const LiquidBridgeWilletSpecies *>(getBaseSpecies());
}

std::string LiquidBridgeWilletInteraction::getName() const
{
    return "LiquidBridgeWillet";
}
