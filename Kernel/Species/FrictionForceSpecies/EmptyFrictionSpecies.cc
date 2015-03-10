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

#include "EmptyFrictionSpecies.h"

EmptyFrictionSpecies::EmptyFrictionSpecies()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"EmptySpecies::EmptySpecies() finished"<<std::endl;
#endif
}

/*!
 * \param[in] the species that is copied
 */
EmptyFrictionSpecies::EmptyFrictionSpecies(const EmptyFrictionSpecies &s UNUSED)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"EmptySpecies::EmptySpecies(const EmptySpecies &p) finished"<<std::endl;
#endif
}

EmptyFrictionSpecies::~EmptyFrictionSpecies()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout<<"EmptySpecies::~EmptySpecies() finished"<<std::endl;
#endif   
}

/*!
 * \param[out] output stream (typically the restart file)
 */
void EmptyFrictionSpecies::write(std::ostream& os UNUSED) const
{
}

/*!
 * \param[in] input stream (typically the restart file)
 */
void EmptyFrictionSpecies::read(std::istream& is UNUSED)
{
}

/*!
 * \return a string containing the name of the species (minus the word "Species")
 */
std::string EmptyFrictionSpecies::getBaseName() const
{
    return "";
}

/*!
 * \param[in] S,T the two species whose properties are mixed to create the new species
 */
void EmptyFrictionSpecies::mix(EmptyFrictionSpecies* const S UNUSED, EmptyFrictionSpecies* const T UNUSED)
{
}

/*!
 * \details Returns true for any FrictionForceSpecies except EmptyFrictionSpecies, 
 * because for spherical particles, torques are only caused by tangential forces. 
 * See SpeciesHandler::useAngularDOFs for more details
 * \return false
 */
bool EmptyFrictionSpecies::getUseAngularDOFs() const
{
    return false;
}
