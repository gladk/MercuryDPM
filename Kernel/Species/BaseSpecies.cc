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

#include "BaseSpecies.h"
#include "SpeciesHandler.h"
#include "DPMBase.h"
#include "Interactions/BaseInteraction.h"
#include<cmath>

class BaseParticle;
class BaseInteractable;

BaseSpecies::BaseSpecies()
{
    handler_ = 0;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseSpecies::BaseSpecies() finished"<<std::endl;
#endif
}

BaseSpecies::BaseSpecies(const BaseSpecies &p)
        : BaseObject(p)
{
    handler_ = p.handler_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseSpecies::BaseSpecies(const BaseSpecies &p) finished"<<std::endl;
#endif
}

BaseSpecies::~BaseSpecies()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseSpecies::~BaseSpecies() finished"<<std::endl;
#endif   
}

void BaseSpecies::clear()
{
    std::cout << "BaseSpecies::clear(), this function shouldn't be called" << std::endl;
}

void BaseSpecies::setHandler(SpeciesHandler* handler)
{
    handler_ = handler;
}

SpeciesHandler* BaseSpecies::getHandler() const
{
    return handler_;
}

///BaseSpecies print function, which accepts an os std::stringstream as input. It prints human readable BaseSpecies information to the std::stringstream
void BaseSpecies::write(std::ostream& os) const
{
    BaseObject::write(os);
}

void BaseSpecies::read(std::istream& is)
{
    BaseObject::read(is);
}

std::string BaseSpecies::getBaseName() const
{
    return "Base";
}

std::string BaseSpecies::getName() const
{
    return getBaseName() + "Species";
}

Mdouble BaseSpecies::getInteractionDistance() const
{
    return 0.0;
}

Mdouble BaseSpecies::average(Mdouble a, Mdouble b)
{
    return (a + b) != 0.0 ? (2. * (a * b) / (a + b)) : 0;
}


///create values for mixed species
void BaseSpecies::mix(BaseSpecies* const S UNUSED, BaseSpecies* const T UNUSED)
{
}

