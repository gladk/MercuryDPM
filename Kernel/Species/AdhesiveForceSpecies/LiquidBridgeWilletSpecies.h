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

#ifndef LiquidBridgeWilletSpecies_H
#define LiquidBridgeWilletSpecies_H
#include "Species/BaseSpecies.h"
#include "Math/ExtendedMath.h"
#include "Interactions/AdhesiveForceInteractions/LiquidBridgeWilletInteraction.h"
class BaseInteractable;
class BaseInteraction;

//Note the getVelocity can for some Species be dependent on which point on the Species is meant.
class LiquidBridgeWilletSpecies : public virtual BaseSpecies
{
public:
    typedef LiquidBridgeWilletInteraction InteractionType;
    LiquidBridgeWilletSpecies();
    LiquidBridgeWilletSpecies(const LiquidBridgeWilletSpecies &s);
    virtual ~LiquidBridgeWilletSpecies();
    void read(std::istream& is);
    void write(std::ostream& os) const;
    std::string getBaseName() const;
    void mix(LiquidBridgeWilletSpecies* const S, LiquidBridgeWilletSpecies* const T);

//adhesion-specific functions

    ///Returns the particle distance below which adhesive forces can occur (needed for contact detection)
    Mdouble getInteractionDistance() const;

//setters and getters
    void setLiquidBridgeVolume(Mdouble liquidBridgeVolume);

    Mdouble getLiquidBridgeVolume() const;

    void setSurfaceTension(Mdouble surfaceTension);

    Mdouble getSurfaceTension() const;

    void setContactAngle(Mdouble contactAngle);

    Mdouble getContactAngle() const;

private:
	/// Volume of the liquid bridge
    Mdouble liquidBridgeVolume_; 
    Mdouble cbrtLiquidBridgeVolume_; 
    /// contact angle between particle and liquid bridge surface
    Mdouble contactAngle_; 
    /// surface tension of the liquid bridge
    Mdouble surfaceTension_; 
    
};
#endif
