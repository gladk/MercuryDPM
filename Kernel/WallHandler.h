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

#include "BaseHandler.h"
#include "Walls/BaseWall.h"

class BaseWall;

/*!
 * \class WallHandler
 * \brief Container to store all BaseWall
 * \details The WallHandler is a container to store all BaseWall. It is implemented by a vector of pointers to BaseWall.
 */
class WallHandler : public BaseHandler<BaseWall>
{
public:
  /*!
   * \brief default constructor, it simply creates an empty WallHandler.
   */
    WallHandler();

  /*!
   * \brief Copy constructor, it copies the WallHandler and all BaseWall it contains.
   */
    WallHandler(const WallHandler& BH);

  /*!
   * \brief Assignment operator.
   */
    WallHandler operator =(const WallHandler& rhs);
    
  /*!
   * \brief Destructor, it simply destructs the WallHandler and all BaseWall it contains.
   */
    ~WallHandler();

  /*!
   * \brief Adds a new BaseWall to the WallHandler.
   */
    void addObject(BaseWall* W);

  /*!
   * \brief Reads BaseWall into the WallHandler from restart data.
   */
    void readObject(std::istream& is);

    std::string getName() const;

private:
    
};

#endif

