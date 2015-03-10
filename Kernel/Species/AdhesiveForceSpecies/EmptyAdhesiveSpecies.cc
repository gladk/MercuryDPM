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

#include "EmptyAdhesiveSpecies.h"

EmptyAdhesiveSpecies::EmptyAdhesiveSpecies() : BaseSpecies()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"EmptyAdhesiveSpecies::EmptyAdhesiveSpecies() finished"<<std::endl;
#endif
}

EmptyAdhesiveSpecies::EmptyAdhesiveSpecies(const EmptyAdhesiveSpecies &s)
        : BaseSpecies(s)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"EmptyAdhesiveSpecies::EmptyAdhesiveSpecies(const EmptyAdhesiveSpecies &p) finished"<<std::endl;
#endif
}

EmptyAdhesiveSpecies::~EmptyAdhesiveSpecies()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"EmptyAdhesiveSpecies::~EmptyAdhesiveSpecies() finished"<<std::endl;
#endif   
}

///EmptyAdhesiveSpecies print function, which accepts an os std::stringstream as input. It prints human readable EmptyAdhesiveSpecies information to the std::stringstream
void EmptyAdhesiveSpecies::write(std::ostream& os UNUSED) const
{
}

void EmptyAdhesiveSpecies::read(std::istream& is UNUSED)
{
}

//the name is set such that the full name does not extend
std::string EmptyAdhesiveSpecies::getBaseName() const
{
    return "";
}

///create values for mixed species
void EmptyAdhesiveSpecies::mix(EmptyAdhesiveSpecies* const S UNUSED, EmptyAdhesiveSpecies* const T UNUSED)
{
}

///Returns the particle distance below which adhesive forces can occur (needed for contact detection)
Mdouble EmptyAdhesiveSpecies::getInteractionDistance() const
{
    return 0.0;
}

