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

#ifndef EMPTYTANGENTIALINTERACTION_H
#define EMPTYTANGENTIALINTERACTION_H

#include "Interactions/BaseInteraction.h"
#include "Math/Vector.h"
class BaseParticle;
class EmptyFrictionSpecies;
class BaseInteractable;

class EmptyFrictionInteraction : public virtual BaseInteraction
{
public:
    typedef EmptyFrictionSpecies SpeciesType;
    EmptyFrictionInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);
    EmptyFrictionInteraction(const EmptyFrictionInteraction &p);
    virtual ~EmptyFrictionInteraction();

    void computeForce();

    ///Interaction read function, which accepts an std::stringstream as input.
    void read(std::istream& is);

    ///Interaction print function, which accepts an std::stringstream as input.
    void write(std::ostream& os) const;

    Mdouble getElasticEnergy() const;

    const EmptyFrictionSpecies* getSpecies() const;

    std::string getName() const;

// specific for tangential forces

    //currently, only tangential forces have an integratable part (the spring)
    void integrate(Mdouble timeStep);

    //needed by BaseInteraction::writeToFstat()
    Mdouble getTangentialOverlap() const;

    const Vec3D getTangentialForce() const;
};
#endif
