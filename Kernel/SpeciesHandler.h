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

#ifndef SPECIESHANDLER_H
#define SPECIESHANDLER_H

#include "BaseHandler.h"
class BaseSpecies;
class ParticleSpecies;

/// \brief Container to store all ParticleSpecies
/// \details The SpeciesHandler is a container to store all ParticleSpecies. It is implemented by a vector of pointers to ParticleSpecies.
class SpeciesHandler : public BaseHandler<ParticleSpecies>
{
public:
    /// \brief Default constructor, it simply creates an empty SpeciesHandler.
    SpeciesHandler();

    /// \brief Copy constructor, it copies the SpeciesHandler and all ParticleSpecies it contains.
    SpeciesHandler(const SpeciesHandler&other);

    /// \brief Assignment operator.
    SpeciesHandler operator =(const SpeciesHandler& rhs);
    
    /// \brief Destructor, it simply destructs the SpeciesHandler and all ParticleSpecies it contains.
    ~SpeciesHandler();

    /// \brief Adds a new ParticleSpecies to the SpeciesHandler.
    virtual void addObject(ParticleSpecies* const S);

    void removeObject(unsigned const int id);

    /// \brief Reads ParticleSpecies into the SpeciesHandler from restart data.
    void readObject(std::istream& is);

    ParticleSpecies* readOldObject(std::istream& is);

    unsigned int getMixedId(const unsigned int id1, const unsigned int id2) const;

    template<typename U> typename U::MixedSpeciesType* getMixedObject(const U* S, const U* T);

//    MixedLinearViscoelasticSpecies* getMixedObject(const LinearViscoelasticSpecies*,const LinearViscoelasticSpecies*);
//    MixedLinearPlasticViscoelasticSpecies* getMixedObject(const LinearPlasticViscoelasticSpecies*,const LinearPlasticViscoelasticSpecies*);
//    MixedLinearViscoelasticSlidingFrictionSpecies* getMixedObject(const LinearViscoelasticSlidingFrictionSpecies*,const LinearViscoelasticSlidingFrictionSpecies*);
//    MixedLinearPlasticViscoelasticSlidingFrictionSpecies* getMixedObject(const LinearPlasticViscoelasticSlidingFrictionSpecies*,const LinearPlasticViscoelasticSlidingFrictionSpecies*);
//    MixedLinearViscoelasticFrictionSpecies* getMixedObject(const LinearViscoelasticFrictionSpecies*,const LinearViscoelasticFrictionSpecies*);
//    MixedLinearPlasticViscoelasticFrictionSpecies* getMixedObject(const LinearPlasticViscoelasticFrictionSpecies*,const LinearPlasticViscoelasticFrictionSpecies*);

    BaseSpecies* getMixedObject(const unsigned int id1, const unsigned int id2);

    const std::vector<BaseSpecies*>& getMixedObjects() const;

    virtual void write(std::ostream& os) const;

    std::string getName() const;

    /// Check if angular DOF have to be used
    bool useAngularDOFs();

private:
    ///The list of pointers to the mixed species
    std::vector<BaseSpecies*> mixedObjects_;
};

#endif

