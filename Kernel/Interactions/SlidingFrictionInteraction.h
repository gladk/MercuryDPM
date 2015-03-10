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

#ifndef SLIDINGFRICTIONINTERACTION_H
#define SLIDINGFRICTIONINTERACTION_H

#include "Interactions/BaseInteraction.h"
#include "Math/Vector.h"

class BaseParticle;
class SlidingFrictionSpecies;
class BaseInteractable;

class SlidingFrictionInteraction : public virtual BaseInteraction
{
public:
    typedef SlidingFrictionSpecies SpeciesType;
    SlidingFrictionInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);
    SlidingFrictionInteraction(const SlidingFrictionInteraction &p);
    virtual ~SlidingFrictionInteraction();

    void computeForce();

    ///Interaction read function, which accepts an std::stringstream as input.
    void read(std::istream& is);

    ///Interaction print function, which accepts an std::stringstream as input.
    void write(std::ostream& os) const;

    //currently, only tangential forces have an integratable part (the spring)
    void integrate(Mdouble timeStep);

    Mdouble getElasticEnergy() const;

    //needed by BaseInteraction::writeToFstat()
    Mdouble getTangentialOverlap() const;

    std::string getName() const;

//setters and getters

    const Vec3D getTangentialForce() const;

    const SlidingFrictionSpecies* getSpecies() const;

    void reverseHistory();

private:
    //set in integrate, used in compute force
    Vec3D slidingSpring_;

    //set in compute force, used in integrate
    Vec3D slidingSpringVelocity_;

    //internal variable (no set/get)
    Vec3D tangentialForce_;
};
#endif
