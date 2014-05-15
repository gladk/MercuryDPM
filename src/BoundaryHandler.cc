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
