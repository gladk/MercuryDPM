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


///Constructor of the ParticleHandler class. It creates and empty ParticleHandler.
BoundaryHandler::BoundaryHandler()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "BoundaryHandler::BoundaryHandler() finished" << std::endl;
#endif
}

/// \param[in] BH The BoundaryHandler that has to be copied.
BoundaryHandler::BoundaryHandler(const BoundaryHandler &BH)
        : BaseHandler<BaseBoundary>()
{
    copyContentsFromOtherHandler(BH);
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "BoundaryHandler::BoundaryHandler(const BoundaryHandler &BH) finished" << std::endl;
#endif
}

/// \param[in] rhs The BoundaryHandler on the right hand side of the assignment.
BoundaryHandler BoundaryHandler::operator =(const BoundaryHandler& rhs)
{
    if (this != &rhs)
    {
        clear();
        copyContentsFromOtherHandler(rhs);
    }
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "BoundaryHandler BoundaryHandler::operator =(const BoundaryHandler& rhs)" << std::endl;
#endif
    return *this;
}

BoundaryHandler::~BoundaryHandler()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout << "BoundaryHandler::~BoundaryHandler() finished" << std::endl;
#endif
}

/// \param[in] P A pointer to the BaseBoundary (or derived class) that has to be added.
void BoundaryHandler::addObject(BaseBoundary* P)
{
    //Puts the particle in the Particle list
    BaseHandler<BaseBoundary>::addObject(P);
    //set the particleHandler pointer
    P->setHandler(this);
}

/// \param[in] is The input stream from which the information is read.
void BoundaryHandler::readObject(std::istream& is)
{
    std::string type;
    is >> type;
    if (type.compare("AngledPeriodicBoundary") == 0)
    {
        AngledPeriodicBoundary AngledPeriodicBoundary;
        is >> AngledPeriodicBoundary;
        copyAndAddObject(AngledPeriodicBoundary);
    }
    else if (type.compare("ChuteInsertionBoundary") == 0)
    {
        ChuteInsertionBoundary chuteInsertionBoundary;
        is >> chuteInsertionBoundary;
        copyAndAddObject(chuteInsertionBoundary);
    }
    else if (type.compare("CubeInsertionBoundary") == 0)
    {
        CubeInsertionBoundary cubeInsertionBoundary;
        is >> cubeInsertionBoundary;
        copyAndAddObject(cubeInsertionBoundary);
    }
    else if (type.compare("CircularPeriodicBoundary") == 0)
    {
        CircularPeriodicBoundary circularPeriodicBoundary;
        is >> circularPeriodicBoundary;
        copyAndAddObject(circularPeriodicBoundary);
    }
    else if (type.compare("DeletionBoundary") == 0)
    {
        DeletionBoundary deletionBoundary;
        is >> deletionBoundary;
        copyAndAddObject(deletionBoundary);
    }
    else if (type.compare("HopperInsertionBoundary") == 0)
    {
        HopperInsertionBoundary hopperInsertionBoundary;
        is >> hopperInsertionBoundary;
        copyAndAddObject(hopperInsertionBoundary);
    }
    else if (type.compare("PeriodicBoundary") == 0)
    {
        PeriodicBoundary periodicBoundary;
        is >> periodicBoundary;
        copyAndAddObject(periodicBoundary);
    }
    //for backward compatibility (before svnversion ~2360)
    else if (type.compare("normal") == 0)
    {
        readOldObject(is);
    }
    else
    {
        std::cerr << "Boundary type: " << type << " not understood in restart file" << std::endl;
        exit(-1);
    }
}


/// \param[in] is The input stream from which the information is read.
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



std::string BoundaryHandler::getName() const
{
    return "BoundaryHandler";
}

