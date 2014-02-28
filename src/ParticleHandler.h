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

#ifndef PARTICLE_HANDLER_H
#define PARTICLE_HANDLER_H

#include "BaseHandler.h"
#include "CSpecies.h"

class BaseParticle;
//class CSpecies;

/// \brief Container to store all BaseParticle.
/// \details The ParticleHandler is a container to store all BaseParticle. It is implemented by a vector of pointers to BaseParticle.

class ParticleHandler : public BaseHandler<BaseParticle>
{
public:
    /// \brief Default constructor, it simply creates an empty ParticleHandler.
    ParticleHandler();

    /// \brief Copy constructor, it copies the ParticleHandler and all BaseParticle it contains.
    ParticleHandler(const ParticleHandler& PH);

    /// \brief Assignment operator.
    ParticleHandler operator = (const ParticleHandler& rhs);

    /// \brief Destructor, it simply destructs the ParticleHandler and all BaseParticle it contains.
    ~ParticleHandler();

    /// \brief Adds a BaseParticle to the ParticleHandler.
    virtual void addObject(BaseParticle* P);

    /// \brief Gets a pointer to the smallest BaseParticle (by interactionRadius) in this ParticleHandler.
    BaseParticle* getSmallestParticle() const;

    /// \brief Gets a pointer to the largest BaseParticle (by interactionRadius) in this ParticleHandler.
    BaseParticle* getLargestParticle() const;

    /// \brief Gets a pointer to the fastests BaseParticle in this ParticleHandler.
    BaseParticle* getFastestParticle() const;

    BaseParticle* getLowestPositionComponentParticle(const int i) const;
    BaseParticle* getHighestPositionComponentParticle(const int i) const;
    BaseParticle* getLowestVelocityComponentParticle(const int i) const;
    BaseParticle* getHighestVelocityComponentParticle(const int i) const;   
    
    /// \brief Gets a pointer to the lightest BaseParticle (by mass) in this ParticleHandler.
    BaseParticle* getLightestParticle() const;

    /// \brief Empties the whole ParticleHandler by removing all BaseParticle.
    void clear();

    /// \brief Reads BaseParticle into the ParticleHandler from restart data.
    void readObject(std::istream& is);

    /// \brief Checks if the extrema of this ParticleHandler needs updating.
    void checkExtrema(BaseParticle* P);
    
    /// \brief Checks if the extrema of this ParticleHandler needs updating when a particle is deleted.
    void checkExtremaOnDelete(BaseParticle* P);    

    CSpecies& getSpecies(const int i);  
    void setSpecies(std::vector<CSpecies>*) ;


private:
    ///A pointer to the largest BaseParticle (by interactionRadius) in this ParticleHandler
    BaseParticle* largestParticle_;

    ///A pointer to the smallest BaseParticle (by interactionRadius) in this ParticleHandler
    BaseParticle* smallestParticle_;

    ///
    std::vector<CSpecies>* species_;
};

#endif

