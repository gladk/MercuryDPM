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

#ifndef FrictionInteraction_H
#define FrictionInteraction_H

#include "SlidingFrictionInteraction.h"
#include "Math/Vector.h"

class BaseParticle;
class FrictionSpecies;
class BaseInteractable;

class FrictionInteraction : public SlidingFrictionInteraction
{
public:
    typedef FrictionSpecies SpeciesType;
    FrictionInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);
    FrictionInteraction(const FrictionInteraction &p);
    virtual ~FrictionInteraction();

    void computeForce();

    ///Interaction read function, which accepts an std::stringstream as input.
    void read(std::istream& is);

    ///Interaction print function, which accepts an std::stringstream as input.
    void write(std::ostream& os) const;

    //currently, only tangential forces have an integratable part (the spring)
    void integrate(Mdouble timeStep);

    Mdouble getElasticEnergy() const;

    std::string getName() const;

    const FrictionSpecies* getSpecies() const;

    void reverseHistory();

//setters and getters

private:
    //set in integrate, used in compute force
    Vec3D rollingSpring_;

    //set in compute force, used in integrate
    Vec3D rollingSpringVelocity_;

    //set in integrate, used in compute force
    Vec3D torsionSpring_;

    //set in compute force, used in integrate
    Vec3D torsionSpringVelocity_;
};
#endif
