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

#include <Math/Helpers.h>
#include "WallHandler.h"
#include "Walls/BaseWall.h"
#include "Walls/CylindricalWall.h"
#include "Walls/AxisymmetricIntersectionOfWalls.h"
#include "Walls/IntersectionOfWalls.h"
#include "Walls/InfiniteWall.h"
#include "Walls/InfiniteWallWithHole.h"
#include "Walls/Screw.h"
#include "Walls/Coil.h"

///Constructor of the WallHandler class. It creates and empty WallHandler.
WallHandler::WallHandler()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "WallHandler::WallHandler() finished" << std::endl;
#endif
}

/// \param[in] WH The WallHandler that has to be copied.
WallHandler::WallHandler(const WallHandler &WH)
: BaseHandler<BaseWall>()
{
    copyContentsFromOtherHandler(WH);
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "WallHandler::WallHandler(const WallHandler &PH) finished" << std::endl;
#endif
}

/// \param[in] rhs The WallHandler on the right hand side of the assignment.
WallHandler WallHandler::operator =(const WallHandler& rhs)
{
    if (this != &rhs)
    {
        clear();
        copyContentsFromOtherHandler(rhs);
    }
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "WallHandler::operator = (const WallHandler& rhs) finished" << std::endl;
#endif
    return *this;
}

WallHandler::~WallHandler()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout << "WallHandler::~WallHandler() finished" << std::endl;
#endif
}

/// \param[in] W A pointer to the BaseWall (or derived class) that has to be added.
void WallHandler::addObject(BaseWall* W)
{
    //Puts the particle in the Particle list
    BaseHandler<BaseWall>::addObject(W);
    //set the particleHandler pointer
    W->setHandler(this);
}

/// \param[in] is The input stream from which the information is read.
void WallHandler::readObject(std::istream& is)
{
    std::string type;
    is >> type;
    if (type.compare("CylindricalWall") == 0)
    {
        CylindricalWall cylindricalWall;
        is >> cylindricalWall;
        copyAndAddObject(cylindricalWall);
    }
    else if (type.compare("AxisymmetricIntersectionOfWalls") == 0)
    {
        AxisymmetricIntersectionOfWalls axisymmetricIntersectionOfWalls;
        is >> axisymmetricIntersectionOfWalls;
        copyAndAddObject(axisymmetricIntersectionOfWalls);
    }
    else if (type.compare("IntersectionOfWalls") == 0)
    {
        IntersectionOfWalls intersectionOfWalls;
        is >> intersectionOfWalls;
        copyAndAddObject(intersectionOfWalls);
    }
    else if (type.compare("InfiniteWall") == 0)
    {
        InfiniteWall infiniteWall;
        is >> infiniteWall;
        copyAndAddObject(infiniteWall);
    }
    else if (type.compare("InfiniteWallWithHole") == 0)
    {
        InfiniteWallWithHole infiniteWallWithHole;
        is >> infiniteWallWithHole;
        copyAndAddObject(infiniteWallWithHole);
    }
    else if (type.compare("Screw") == 0)
    {
        Screw screw;
        is >> screw;
        copyAndAddObject(screw);
    }
    else if (type.compare("Coil") == 0)
    {
        Coil coil;
        is >> coil;
        copyAndAddObject(coil);
    }
    //for backward compatibility (before svnversion ~2360)
    else if (type.compare("numFiniteWalls") == 0)
    {
        readOldObject(is);
    }
    else
    {
        std::cerr << "Wall type: " << type << " not understood in restart file" << std::endl;
        exit(-1);
    }
}

/// \param[in] is The input stream from which the information is read.
void WallHandler::readOldObject(std::istream& is)
{
    //read in next line
    std::stringstream line(std::stringstream::in | std::stringstream::out);
    helpers::getLineFromStringStream(is, line);
    //std::cout << line.str() << std::endl;

    std::string dummy;
    Mdouble numWalls, position;
    Vec3D normal;
    line >> numWalls;

    if (numWalls==0)
    {
        InfiniteWall infiniteWall;
        line >> dummy >> normal >> dummy >> position;
        infiniteWall.set(normal, position);
        copyAndAddObject(infiniteWall);
    }
    else
    {
        IntersectionOfWalls finiteWallInstance;
        IntersectionOfWalls intersectionOfWalls;
        for (unsigned int i=0; i<numWalls; i++)
        {
            line >> dummy >> normal >> dummy >> position;
            intersectionOfWalls.addObject(normal, position);
        }
        copyAndAddObject(intersectionOfWalls);
    }
}


std::string WallHandler::getName() const
{
    return "WallHandler";
}
