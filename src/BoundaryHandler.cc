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

#include "BoundaryHandler.h"
#include "BaseBoundary.h"
#include "PeriodicBoundary.h"
#include "DeletionBoundary.h"
#include "CircularPeriodicBoundary.h"


/// \param[in] P A pointer to the BaseBoundary (or derived class) that has to be added.
void BoundaryHandler::addObject(BaseBoundary* P)
{
    //Puts the particle in the Particle list
    BaseHandler<BaseBoundary>::addObject(P);
    //set the particleHandler pointer
    getLastObject()->setHandler(this);
}

/// \param[in] is The input stream from which the information is read.
void BoundaryHandler::readObject(std::istream& is)
{
    std::string type;
    is >> type;
    if(type.compare("PeriodicBoundary")==0)
    {
        PeriodicBoundary periodicBoundary;
        is>>periodicBoundary;
        copyAndAddObject(periodicBoundary);
    }
    else if(type.compare("DeletionBoundary")==0)
    {
        DeletionBoundary deletionBoundary;
        is>>deletionBoundary;
        copyAndAddObject(deletionBoundary);
    }
    else if(type.compare("CircularPeriodicBoundary")==0)
    {
        CircularPeriodicBoundary circularPeriodicBoundary;
        is>>circularPeriodicBoundary;
        copyAndAddObject(circularPeriodicBoundary);
    }
    else
    {
        std::cerr<<"Boundary type: "<<type<<" not understood in restart file"<<std::endl;
        exit(-1);
    }
}
