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

#ifndef WALLHANDLER_H
#define WALLHANDLER_H

#include <iostream>
#include <vector>

class BaseWall;

/// \brief Container to store all BaseWall
/// \details The WallHandler is a container to store all BaseWall. It is implemented by a vector of pointers to BaseWall.
class WallHandler
{
public:
    /// \brief Default WallHandler constructor, it simply creates an empty WallHandler.
    WallHandler();

    /// \brief Copy constructor for the WallHandler class, it copies the WallHandler and all BaseWall it contains.
    WallHandler(const WallHandler& BH);

    /// \brief Assigns one WallHandler to another WallHandler.
    WallHandler operator = (const WallHandler& rhs);

    /// \brief Destructor, it simply destructs the WallHandler and all BaseWall it contains.
    ~WallHandler();

    /// \brief Creates a copy of a BaseWall and adds it to the WallHandler
    void copyAndAddWall(const BaseWall& B);

    /// \brief Creates a copy of a BaseWall and adds it to the WallHandler.
    void copyAndAddWall(const BaseWall* B);

    /// \brief Adds a new BaseWall to the WallHandler.
    void addWall(BaseWall* W);

    /// \brief Removes a BaseWall from the WallHandler.
    void removeWall(unsigned const int id);

    /// \brief Removes the last BaseWall from the WallHandler.
    void removeLastWall();

    /// \brief Empties the whole WallHandler by removing all BaseWall.
    void clear();

    /// \brief Reads BaseWall into the WallHandler from restart data.
    void readWall(std::istream& is);

    /// \brief Gets a pointer to the BaseWall at the specified index in the WallHandler.
    BaseWall* getWall(const unsigned int id) const;

    /// \brief Gets a pointer to the last BaseWall in this WallHandler.
    BaseWall* getLastWall() const;

    /// \brief Gets the number of BaseWalls in this WallHandler.
    unsigned int getNumberOfWalls() const;

    /// \brief Gets the storage capacity of this WallHandler.
    unsigned int getStorageCapacity() const;

    /// \brief Sets the storage capacity of this WallHandler.
    void setStorageCapacity(const unsigned int N);
    
    /// \brief Gets the begin of the const_iterator over all BaseWall in this WallHandler.
    const std::vector<BaseWall*>::const_iterator begin() const;

    /// \brief Gets the begin of the iterator over all BaseWall in this WallHandler.
    const std::vector<BaseWall*>::iterator begin();

    /// \brief Gets the end of the const_iterator over all BaseWall in this WallHandler.
    const std::vector<BaseWall*>::const_iterator end() const;

    /// \brief Gets the end of the iterator over all BaseWall in this WallHandler.
    const std::vector<BaseWall*>::iterator end();

private:
    ///The actual list of Wall pointers
    std::vector<BaseWall*> walls_;
};

#endif

