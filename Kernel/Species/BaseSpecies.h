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

#ifndef BASESPECIES_H
#define BASESPECIES_H
#include "BaseObject.h"
#include "Math/ExtendedMath.h"
#include "Math/Helpers.h"

class SpeciesHandler;
//class BaseParticle;
class BaseParticle;
class BaseInteractable;
class BaseInteraction;

//Note the getVelocity can for some Species be dependent on which point on the Species is meant.
class BaseSpecies : public BaseObject
{
public:
    BaseSpecies();
    BaseSpecies(const BaseSpecies &p);
    virtual ~BaseSpecies();
    virtual BaseSpecies* copy() const=0;
    virtual void read(std::istream& is);
    virtual void write(std::ostream& os) const;
    virtual void clear();
    void setHandler(SpeciesHandler* handler);
    SpeciesHandler* getHandler() const;
    virtual std::string getBaseName() const;
    virtual std::string getName() const;

// Species-specific functions

    Mdouble average(Mdouble a, Mdouble b);

    ///create values for mixed species
    void mix(BaseSpecies* const S, BaseSpecies* const T);

    ///Returns the particle distance below which adhesive forces can occur (needed for contact detection)
    virtual Mdouble getInteractionDistance() const;

//setters and getters

    ///Allows the dimension of the particle (f.e. for mass) to be accessed
    virtual bool getUseAngularDOFs() const = 0;

    virtual BaseInteraction* getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)=0;

private:
    SpeciesHandler* handler_;
};
#endif
