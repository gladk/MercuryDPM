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
#include "Species/BaseSpecies.h"

/// \brief Container to store all BaseSpecies
/// \details The SpeciesHandler is a container to store all BaseSpecies. It is implemented by a vector of pointers to BaseSpecies.
class SpeciesHandler : public BaseHandler<BaseSpecies>
{
public:
    /// \brief Default constructor, it simply creates an empty SpeciesHandler.
    SpeciesHandler();

    /// \brief Copy constructor, it copies the SpeciesHandler and all BaseSpecies it contains.
    SpeciesHandler(const SpeciesHandler&other);

    /// \brief Assignment operator.
    SpeciesHandler operator =(const SpeciesHandler& rhs);
    
    /// \brief Destructor, it simply destructs the SpeciesHandler and all BaseSpecies it contains.
    ~SpeciesHandler();

    /// \brief Adds a new BaseSpecies to the SpeciesHandler.
    virtual void addObject(BaseSpecies* const S);

    void removeObject(unsigned const int id);

    /// \brief Reads BaseSpecies into the SpeciesHandler from restart data.
    void readObject(std::istream& is);

    unsigned int getMixedId(const unsigned int id1, const unsigned int id2) const;

    template<typename U> U* getMixedObject(const U* S, const U* T);

    BaseSpecies* getMixedObject(const unsigned int id1, const unsigned int id2);

    const std::vector<BaseSpecies*>& getMixedObjects() const;

    virtual void read(std::istream& is);

    virtual void readVersion1(std::istream& is, unsigned int N);

    void readVersion2(std::istream& is);

    virtual void write(std::ostream& os) const;

    std::string getName() const;

private:
    ///The list of pointers to the mixed species
    std::vector<BaseSpecies*> mixedObjects_;
};

#endif

