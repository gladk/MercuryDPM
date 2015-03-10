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


#include <iostream>
#include <string>
#include <stdlib.h>
#include <Math/Helpers.h>

#include "BoundaryHandler.h"
#include "Boundaries/BaseBoundary.h"
#include "Boundaries/AngledPeriodicBoundary.h"
#include "Boundaries/ChuteInsertionBoundary.h"
#include "Boundaries/CubeInsertionBoundary.h"
#include "Boundaries/HopperInsertionBoundary.h"
#include "Boundaries/PeriodicBoundary.h"
#include "Boundaries/CircularPeriodicBoundary.h"
#include "Boundaries/DeletionBoundary.h"
#include "Boundaries/LeesEdwardsBoundary.h"


///Constructor of the ParticleHandler class. It creates and empty ParticleHandler.
BoundaryHandler::BoundaryHandler()
{
    logger(DEBUG, "BoundaryHandler::BoundaryHandler() finished");
}

/*!
 * \param[in] BH The BoundaryHandler that has to be copied.
 * \details This is not a copy constructor! It just copies all BaseBoundary from
 *          the other handler into this handler, and clears all other variables.
 */
BoundaryHandler::BoundaryHandler(const BoundaryHandler &BH)
        : BaseHandler<BaseBoundary>()
{
    copyContentsFromOtherHandler(BH);
    logger(DEBUG, "BoundaryHandler::BoundaryHandler(const BoundaryHandler &BH) finished");
}

/*!
 * \param[in] rhs The BoundaryHandler on the right hand side of the assignment.
 * \details This is not a copy assignment operator! It just copies all BaseBoundary
 *          from the other handler into this handler, and clears all other variables.
 */
BoundaryHandler BoundaryHandler::operator =(const BoundaryHandler& rhs)
{
    if (this != &rhs)
    {
        clear();
        copyContentsFromOtherHandler(rhs);
    }
    logger(DEBUG, "BoundaryHandler BoundaryHandler::operator =(const BoundaryHandler& rhs)");
    return *this;
}

///Default destructor. Note that the delete for all boundaries is done in the BaseHandler.
BoundaryHandler::~BoundaryHandler()
{
    logger(DEBUG, "BoundaryHandler::~BoundaryHandler() finished");
}

///\param[in] P A pointer to the BaseBoundary (or derived class) that has to be added.
///Add the object and tell the object that this is his handler.
void BoundaryHandler::addObject(BaseBoundary* P)
{
    //Puts the particle in the Particle list
    BaseHandler<BaseBoundary>::addObject(P);
    //set the particleHandler pointer
    P->setHandler(this);
}

///\param[in] is The input stream from which the information is read.
///First read the type of boundary, then compare the type to all existing types. 
///When the correct type is found, read it with the >> operator, copy it and add it to the handler.
void BoundaryHandler::readObject(std::istream& is)
{
    //Note that compare returns 0 if the strings are the same.
    std::string type;
    is >> type;
    if (type == "AngledPeriodicBoundary")
    {
        AngledPeriodicBoundary AngledPeriodicBoundary;
        is >> AngledPeriodicBoundary;
        copyAndAddObject(AngledPeriodicBoundary);
    }
    else if (type == "ChuteInsertionBoundary")
    {
        ChuteInsertionBoundary chuteInsertionBoundary;
        is >> chuteInsertionBoundary;
        copyAndAddObject(chuteInsertionBoundary);
    }
    else if (type == "CubeInsertionBoundary")
    {
        CubeInsertionBoundary cubeInsertionBoundary;
        is >> cubeInsertionBoundary;
        copyAndAddObject(cubeInsertionBoundary);
    }
    else if (type == "CircularPeriodicBoundary")
    {
        CircularPeriodicBoundary circularPeriodicBoundary;
        is >> circularPeriodicBoundary;
        copyAndAddObject(circularPeriodicBoundary);
    }
    else if (type == "DeletionBoundary")
    {
        DeletionBoundary deletionBoundary;
        is >> deletionBoundary;
        copyAndAddObject(deletionBoundary);
    }
    else if (type == "HopperInsertionBoundary")
    {
        HopperInsertionBoundary hopperInsertionBoundary;
        is >> hopperInsertionBoundary;
        copyAndAddObject(hopperInsertionBoundary);
    }
    else if (type == "PeriodicBoundary")
    {
        PeriodicBoundary periodicBoundary;
        is >> periodicBoundary;
        copyAndAddObject(periodicBoundary);
    }
    else if (type.compare("LeesEdwardsBoundary") == 0)
    {
        LeesEdwardsBoundary leesEdwardsBoundary;
        is >> leesEdwardsBoundary;
        copyAndAddObject(leesEdwardsBoundary);
    }    
    //for backward compatibility (before svnversion ~2360)
    else if (type == "normal")
    {
        readOldObject(is);
    }
    else
    {
        logger(ERROR, "Boundary type: % not understood in restart file.", type);       
    }
}

///\param[in] is The input stream from which the information is read.
///Get the normal, position left and position right for a periodic boundary from the 
///stream is, and construct a new periodic boundary from it.
///The boundaries that are written like that are outdated, this function is there for backward compatability.
void BoundaryHandler::readOldObject(std::istream& is)
{
    //read in next line
    std::stringstream line(std::stringstream::in | std::stringstream::out);
    helpers::getLineFromStringStream(is, line);

    std::string dummy;
    Vec3D normal;
    Mdouble positionLeft, positionRight;

    PeriodicBoundary periodicBoundary;
    line >> normal >> dummy >> positionLeft >> dummy >> positionRight;
    periodicBoundary.set(normal, positionLeft, positionRight);
    copyAndAddObject(periodicBoundary);
}

/// \return The string "BoundaryHandler"
std::string BoundaryHandler::getName() const
{
    return "BoundaryHandler";
}

