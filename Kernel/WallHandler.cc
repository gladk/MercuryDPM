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

/*!
 * Constructor of the WallHandler class. It creates and empty WallHandler.
 */
WallHandler::WallHandler()
{
    clear();
    logger(DEBUG, "WallHandler::WallHandler() finished");
}

/*! 
 * \param[in] WH The WallHandler that has to be copied.
 * \details This is not a copy constructor! It only copies the pointer to the 
 *          DPMBase and the BaseWall in objects_, it sets the other data members
 *          to 0 or nullptr.
 */
WallHandler::WallHandler(const WallHandler &WH)
{
    clear();
    setDPMBase(WH.getDPMBase());
    copyContentsFromOtherHandler(WH);
    logger(DEBUG, "WallHandler::WallHandler(const WallHandler &PH) finished");
}

/*!
 * \param[in] rhs The WallHandler on the right hand side of the assignment.
 * \details This is not a copy assignment operator! It only copies the pointer to the 
 *          DPMBase and the BaseWall in objects_, it sets the other data members
 *          to 0 or nullptr.
 */
WallHandler WallHandler::operator =(const WallHandler& rhs)
{
    if(this != &rhs)
    {
        clear();
        setDPMBase(rhs.getDPMBase());
        copyContentsFromOtherHandler(rhs);
    }
#ifdef DEBUG_CONSTRUCTOR
    logger(DEBUG, "WallHandler::operator = (const WallHandler& rhs) finished");
#endif
    return *this;
}

WallHandler::~WallHandler()
{
#ifdef DEBUG_DESTRUCTOR
    logger(DEBUG, "WallHandler::~WallHandler() finished");
#endif
}

/*!
 * \param[in] W A pointer to the BaseWall (or derived class) that has to be added.
 * \details First the new BaseWall is added to the vector of BaseWall, then it is 
 * told that this is its handler.
 */ 
void WallHandler::addObject(BaseWall* W)
{
    if (W->getSpecies() == nullptr)
    {
        logger(WARN, "WARNING: The wall with ID % that is added in WallHandler::addObject "
                "does not have a species yet. Please make sure that you have "
                "set the species somewhere in the driver code.", W->getId());
    }
    //Puts the wall in the Wall list
    BaseHandler<BaseWall>::addObject(W);
    //set the particleHandler pointer
    W->setHandler(this);
}

/*!
 * \param[in] is The input stream from which the information is read.
 */
void WallHandler::readObject(std::istream& is)
{
    std::string type;
    is >> type;
    if (type == "CylindricalWall")
    {
        CylindricalWall cylindricalWall;
        is >> cylindricalWall;
        copyAndAddObject(cylindricalWall);
    }
    else if (type == "AxisymmetricIntersectionOfWalls")
    {
        AxisymmetricIntersectionOfWalls axisymmetricIntersectionOfWalls;
        is >> axisymmetricIntersectionOfWalls;
        copyAndAddObject(axisymmetricIntersectionOfWalls);
    }
    else if (type == "IntersectionOfWalls")
    {
        IntersectionOfWalls intersectionOfWalls;
        is >> intersectionOfWalls;
        copyAndAddObject(intersectionOfWalls);
    }
    else if (type == "InfiniteWall")
    {
        InfiniteWall infiniteWall;
        is >> infiniteWall;
        copyAndAddObject(infiniteWall);
    }
    else if (type == "InfiniteWallWithHole")
    {
        InfiniteWallWithHole infiniteWallWithHole;
        is >> infiniteWallWithHole;
        copyAndAddObject(infiniteWallWithHole);
    }
    else if (type == "Screw")
    {
        Screw screw;
        is >> screw;
        copyAndAddObject(screw);
    }
    else if (type == "Coil")
    {
        Coil coil;
        is >> coil;
        copyAndAddObject(coil);
    }
    //for backward compatibility (before svnversion ~2360)
    else if (type == "numFiniteWalls")
    {
        readOldObject(is);
    }
    else
    {
        logger(ERROR, "Wall type: % not understood in restart file", type);
    }
}

/*!
 * \param[in] is The input stream from which the information is read.
 * \details First determine whether or not the wall is an infinite wall. If it is
 * an infinite wall, read the normal and position and add the wall to the handler.
 * If it is a  finite wall, read the normal and position of each part and construct
 * an IntersectionOfWalls from it, which can then be added to the handler.
 */
void WallHandler::readOldObject(std::istream& is)
{
    //read in next line
    std::stringstream line(std::stringstream::in | std::stringstream::out);
    helpers::getLineFromStringStream(is, line);
    logger(VERBOSE, line.str());

    std::string dummy;
    unsigned int numWalls;
    Mdouble position;
    Vec3D normal;
    line >> numWalls;

    if (numWalls == 0)
    {
        InfiniteWall infiniteWall;
        line >> dummy >> normal >> dummy >> position;
        infiniteWall.set(normal, position*normal);
        copyAndAddObject(infiniteWall);
    }
    else
    {
        IntersectionOfWalls intersectionOfWalls;
        for (unsigned int i = 0; i < numWalls; ++i)
        {
            line >> dummy >> normal >> dummy >> position;
            intersectionOfWalls.addObject(normal, position*normal);
        }
        copyAndAddObject(intersectionOfWalls);
    }
}

/*!
 * \return The string "WallHandler".
 */
std::string WallHandler::getName() const
{
    return "WallHandler";
}
