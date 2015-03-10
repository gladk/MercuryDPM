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

#ifndef LINEARPLASTICVISCOELASTICINTERACTION_H
#define LINEARPLASTICVISCOELASTICINTERACTION_H

#include "Interactions/BaseInteraction.h"
class LinearPlasticViscoelasticNormalSpecies;
class BaseInteractable;

class LinearPlasticViscoelasticInteraction : public virtual BaseInteraction
{
public:
    LinearPlasticViscoelasticInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);
    LinearPlasticViscoelasticInteraction(const LinearPlasticViscoelasticInteraction &p);
    virtual ~LinearPlasticViscoelasticInteraction();
    BaseInteraction* copy() const;

    void computeLinearPlasticViscoelasticForce();
    void computeForce();

    ///Interaction read function, which accepts an std::stringstream as input.
    virtual void read(std::istream& is);

    ///Interaction print function, which accepts an std::stringstream as input.
    virtual void write(std::ostream& os) const;

    virtual std::string getName() const;

    Mdouble getElasticEnergy() const;

    const LinearPlasticViscoelasticNormalSpecies* getSpecies() const;

    Mdouble getMaxOverlap() const;

    void setMaxOverlap(const Mdouble maxOverlap);

private:

    //set in integrate, used in compute force
    Mdouble maxOverlap_;
};
#endif
