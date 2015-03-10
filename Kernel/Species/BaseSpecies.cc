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
: BaseObject()
{
    handler_ = 0;
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "BaseSpecies::BaseSpecies() finished" << std::endl;
#endif
}

/*!
 * \param[in] the species that is copied
 */
BaseSpecies::BaseSpecies(const BaseSpecies &p)
: BaseObject(p)
{
    handler_ = p.handler_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "BaseSpecies::BaseSpecies(const BaseSpecies &p) finished" << std::endl;
#endif
}

BaseSpecies::~BaseSpecies()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout << "BaseSpecies::~BaseSpecies() finished" << std::endl;
#endif   
}

/*!
 * \param[in] the pointer to the handler to which this species belongs.
 */
void BaseSpecies::setHandler(SpeciesHandler* handler)
{
    handler_ = handler;
}

/*!
 * \return the pointer to the handler to which this species belongs.
 */
SpeciesHandler* BaseSpecies::getHandler() const
{
    return handler_;
}

/*!
 * Defines the average of two variables by the harmonic mean.
 * This function is used to define default mixed species.
 * \param[in] a,b The two variables you want to average
 * \return The harmonic mean of a and b, \f$\frac{2}{1/a+1/b}\f$
 */
Mdouble BaseSpecies::average(Mdouble a, Mdouble b)
{
    //the second algorithm seems to have a better accuracy, at least for the case average(2e5,2e5)
    //return (a + b) != 0.0 ? (2. * (a * b) / (a + b)) : 0;
    return (a + b) != 0.0 ? (2./(1.0/a+1.0/b)) : 0.0;
}



