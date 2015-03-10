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

#include "LiquidBridgeWilletSpecies.h"
#include <Logger.h>

LiquidBridgeWilletSpecies::LiquidBridgeWilletSpecies()
{
    liquidBridgeVolume_ = std::numeric_limits<double>::quiet_NaN();
    cbrtLiquidBridgeVolume_ = std::numeric_limits<double>::quiet_NaN();
    surfaceTension_ = std::numeric_limits<double>::quiet_NaN();
    contactAngle_ = std::numeric_limits<double>::quiet_NaN();
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LiquidBridgeWilletSpecies::LiquidBridgeWilletSpecies() finished"<<std::endl;
#endif
}

/*!
 * \param[in] the species that is copied
 */
LiquidBridgeWilletSpecies::LiquidBridgeWilletSpecies(const LiquidBridgeWilletSpecies &s)
{
    setLiquidBridgeVolume(s.liquidBridgeVolume_);
    surfaceTension_ = s.surfaceTension_;
    contactAngle_ = s.contactAngle_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LiquidBridgeWilletSpecies::LiquidBridgeWilletSpecies(const LiquidBridgeWilletSpecies &p) finished"<<std::endl;
#endif
}

LiquidBridgeWilletSpecies::~LiquidBridgeWilletSpecies()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout<<"LiquidBridgeWilletSpecies::~LiquidBridgeWilletSpecies() finished"<<std::endl;
#endif   
}

/*!
 * \param[out] output stream (typically the restart file)
 */
void LiquidBridgeWilletSpecies::write(std::ostream& os) const
        {
    os << " liquidBridgeVolume " << liquidBridgeVolume_;
    os << " surfaceTension " << surfaceTension_;
    os << " contactAngle " << contactAngle_;
}

/*!
 * \param[in] input stream (typically the restart file)
 */
void LiquidBridgeWilletSpecies::read(std::istream& is)
{
    std::string dummy;
    is >> dummy >> liquidBridgeVolume_;
    is >> dummy >> surfaceTension_;
    is >> dummy >> contactAngle_;
}

/*!
 * \return a string containing the name of the species (minus the word "Species")
 */
std::string LiquidBridgeWilletSpecies::getBaseName() const
{
    return "LiquidBridgeWillet";
}

/*!
 * \details For all parameters we assume that the harmonic mean of the parameters of the 
 * original two species is a sensible default.
 * \param[in] S,T the two species whose properties are mixed to create the new species
 */
void LiquidBridgeWilletSpecies::mix(LiquidBridgeWilletSpecies* const S, LiquidBridgeWilletSpecies* const T)
{
    setLiquidBridgeVolume(average(S->getLiquidBridgeVolume(), T->getLiquidBridgeVolume()));
    surfaceTension_ = average(S->getSurfaceTension(), T->getSurfaceTension());
    contactAngle_ = average(S->getContactAngle(), T->getContactAngle());
}

/*!
 * \return the maximum separation distance between particles below which the adhesive force is active.
 */
Mdouble LiquidBridgeWilletSpecies::getInteractionDistance() const
{
	return (1.0+0.5*contactAngle_)*cbrtLiquidBridgeVolume_;	
}

/*!
 * \param[in] liquidBridgeVolume the volume of the liquid bridge.
 */
void LiquidBridgeWilletSpecies::setLiquidBridgeVolume(Mdouble liquidBridgeVolume)
{
    if (liquidBridgeVolume >= 0) 
    {
        liquidBridgeVolume_ = liquidBridgeVolume;
        cbrtLiquidBridgeVolume_ = cbrt(liquidBridgeVolume);
    }
    else
    {
        std::cerr << "Error in setLiquidBridgeVolume" << std::endl;
        exit(-1);
    }
}

/*!
 * \return the volume of the liquid bridge.
 */
Mdouble LiquidBridgeWilletSpecies::getLiquidBridgeVolume() const
{
    return liquidBridgeVolume_;
}

/*!
 * \param[in] surfaceTension the surface tension of the liquid.
 */
void LiquidBridgeWilletSpecies::setSurfaceTension(Mdouble surfaceTension)
{
    if (surfaceTension >= 0)
        surfaceTension_ = surfaceTension;
    else
    {
        std::cerr << "Error in setSurfaceTension" << std::endl;
        exit(-1);
    }
}

/*!
 * \return the surface tension of the liquid.
 */
Mdouble LiquidBridgeWilletSpecies::getSurfaceTension() const
{
    return surfaceTension_;
}

/*!
 * \param[in] contactAngle the contact angle between particle and liquid bridge surface.
 */
void LiquidBridgeWilletSpecies::setContactAngle(Mdouble contactAngle)
{
    if (contactAngle >= 0)
        contactAngle_ = contactAngle;
    else
    {
        std::cerr << "Error in setContactAngle" << std::endl;
        exit(-1);
    }
}

/*!
 * \return the contact angle between particle and liquid bridge surface.
 */
Mdouble LiquidBridgeWilletSpecies::getContactAngle() const
{
    return contactAngle_;
}

