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

#include "WallHandler.h"
#include "BaseWall.h"
#include "CylindricalWall.h"
#include "FiniteAxisSymmetricWall.h"
#include "FiniteWall.h"
#include "InfiniteWall.h"
#include "InfiniteWallWithHole.h"
#include "Screw.h"
#include "Walls/Coil.h"


WallHandler::WallHandler()
{
    clear();
    #ifdef CONSTUCTOR_OUTPUT
        std::cout << "WallHandler::WallHandler() finished" << std::endl;
    #endif
}

/// \param[in] WH The WallHandler that has te be copied.
WallHandler::WallHandler(const WallHandler &WH)
{
    clear();
    for (std::vector<BaseWall*>::const_iterator it=WH.begin();it!=WH.end();it++)
    {
        addWall((*it)->copy());
    }
    #ifdef CONSTUCTOR_OUTPUT
        std::cout << "WallHandler::WallHandler(const WallHandler &WH) finished" << std::endl;
    #endif
}

/// \param[in] rhs The WallHandler on the right hand side of the assignment.
WallHandler WallHandler::operator = (const WallHandler& rhs)
{
    if (this != &rhs)
    {
        clear();
        for (std::vector<BaseWall*>::const_iterator it=rhs.begin();it!=rhs.end();it++)
        {
            addWall((*it)->copy());
        }
    }
    #ifdef OPERATOR_OUTPUT
        std::cout << "WallHandler WallHandler::operator = (const WallHandler& rhs) finished" << std::endl;
    #endif
    return *this;
}

WallHandler::~WallHandler()
{
    clear();
    #ifdef DESTRUCTOR_OUTPUT
        std::cout << "WallHandler::~WallHandler() finished" << std::endl;
    #endif
}

/// \param[in] W A reference to the BaseWall that has to be copied.
void WallHandler::copyAndAddWall(const BaseWall& W)
{
    addWall(W.copy());
}

/// \param[in] W A pointer to the BaseWall (or derived class) that has to be copied.
void WallHandler::copyAndAddWall(const BaseWall* W)
{
    addWall(W->copy());
}


/// \param[in] W A pointer to the BaseWall (or derived class) that has to be added.
void WallHandler::addWall(BaseWall* W)
{
    walls_.push_back(W);
}

/// \details The BaseWall at position id is removed by moving the last BaseWall in the vector to the position of id.
/// \param[in] id The index of which BaseWall has to be removed from the WallHandler
void WallHandler::removeWall(const unsigned int id)
{
    //Physically remove Wall
    delete getWall(id);

    //If the Wall was the last Wall nothing has to be done, otherwise some additional work is neccesary
    if (getWall(id) != getLastWall())
    {
        //Copy the pointer to the last Wall to position id
        walls_[id] = getLastWall();
    }
    //Remove the (now double) reference to that last Wall
    walls_.pop_back();
}

void WallHandler::removeLastWall()
{
    //Physically removes Wall
    delete getLastWall();
    //Remove the (now double) reference to that last Wall
    walls_.pop_back();
}

void WallHandler::clear()
{
    walls_.clear();
}

/// \param[in] is The input stream from which the information is read.
void WallHandler::readWall(std::istream& is)
{
    std::string type;
    is >> type;
    if(type.compare("CylindricalWall")==0)
    {
        CylindricalWall cylindricalWall;
        is>>cylindricalWall;
        copyAndAddWall(cylindricalWall);
    }
    else if(type.compare("FiniteAxisSymmetricWall")==0)
    {
        FiniteAxisSymmetricWall finiteAxisSymmetricWall;
        is>>finiteAxisSymmetricWall;
        copyAndAddWall(finiteAxisSymmetricWall);
    }
    else if(type.compare("FiniteWall")==0)
    {
        FiniteWall finiteWall;
        is>>finiteWall;
        copyAndAddWall(finiteWall);
    }
    else if(type.compare("InfiniteWall")==0)
    {
        InfiniteWall infiniteWall;
        is>>infiniteWall;
        copyAndAddWall(infiniteWall);
    }
    else if(type.compare("InfiniteWallWithHole")==0)
    {
        InfiniteWallWithHole infiniteWallWithHole;
        is>>infiniteWallWithHole;
        copyAndAddWall(infiniteWallWithHole);
    }
    else if(type.compare("Screw")==0)
    {
        Screw screw;
        is>>screw;
        copyAndAddWall(screw);
    }
    else if(type.compare("Coil")==0)
    {
        Coil coil;
        is>>coil;
        copyAndAddWall(coil);
    }    
	//for backward compatibility (before svnversion ~2360)
	else if(type.compare("numFiniteWalls")==0)
	{
		int numFiniteWalls;
		is >> numFiniteWalls;
		if (numFiniteWalls) {
			FiniteWall finiteWallInstance;
			is>>finiteWallInstance;
			copyAndAddWall(finiteWallInstance);
		} else {
			InfiniteWall infiniteWallInstance;
			is>>infiniteWallInstance;
			copyAndAddWall(infiniteWallInstance);
		}
	}      
    else
    {
        std::cerr<<"Wall type: "<<type<<" not understood in restart file"<<std::endl;
        exit(-1);
    }
}

/// \param[in] id The index of the requested BaseWall.
/// \return A pointer to the requested BaseWall.
BaseWall* WallHandler::getWall(const unsigned int id) const
{
    if (id<getNumberOfWalls())
    {
        return walls_[id];
    }
    else
    {
        std::cerr<<"In: BaseWall* WallHandler::getWall(const unsigned int id) const"<<std::endl;
        std::cerr<<"No BaseWall exist with index "<<id<<" maximum index is "<<getNumberOfWalls()<<std::endl;
        exit(-1);
    }
}

/// \return A pointer to the last BaseWall in the WallHandler.
BaseWall* WallHandler::getLastWall() const
{
    return walls_.back();
}

/// \return The number of BaseWall in this WallHandler.
unsigned int WallHandler::getNumberOfWalls() const
{
    return walls_.size();
}

/// \return The storage capacity of this WallHandler.
unsigned int WallHandler::getStorageCapacity() const
{
    return walls_.capacity();
}

/// \param[in] N The storage capacity the WallHandler will have.
void WallHandler::setStorageCapacity(const unsigned int N)
{
    walls_.reserve(N);
}

/// \return A const_iterator pointing to the first BaseWall.
const std::vector<BaseWall*>::const_iterator WallHandler::begin() const
{
    return walls_.begin();
}

/// \return An iterator pointing to the first BaseWall.
const std::vector<BaseWall*>::iterator WallHandler::begin()
{
    return walls_.begin();
}

/// \return bB A const_iterator pointing to the last BaseWall.
const std::vector<BaseWall*>::const_iterator WallHandler::end() const
{
    return walls_.end();
}

/// \return bB An interator pointing to the last BaseWall.
const std::vector<BaseWall*>::iterator WallHandler::end()
{
    return walls_.end();
}
