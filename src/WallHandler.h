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

