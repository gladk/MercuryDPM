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

#ifndef BOUNDARYHANDLER_H
#define BOUNDARYHANDLER_H

#include "BaseHandler.h"

class BaseBoundary;

/// \brief Container to store all BaseBoundary
/// \details The BoundaryHandler is a container to store all BaseBoundary. It is implemented by a vector of pointers to BaseBoundary.
class BoundaryHandler : public BaseHandler<BaseBoundary>
{
public:
    /// \brief Adds a BaseBoundary to the BoundaryHandler.
    void addObject(BaseBoundary* P);
    
    /// \brief Reads BaseBoundary into the BoundaryHandler from restart data.
    void readObject(std::istream& is);
};

#endif

