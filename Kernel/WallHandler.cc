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
