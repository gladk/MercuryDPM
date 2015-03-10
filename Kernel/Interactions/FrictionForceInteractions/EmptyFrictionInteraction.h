// This file is part of MercuryDPM.
// 
// MercuryDPM is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// MercuryDPM is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with MercuryDPM.  If not, see <http://www.gnu.org/licenses/>.
// 
// Copyright 2013 The Mercury Developers Team
// For the list of developers, see <http://www.MercuryDPM.org/Team>

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
