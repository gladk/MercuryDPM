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

#include "BaseWall.h"
#include "DPMBase.h"
#include "WallHandler.h"
#include "Species/BaseSpecies.h"

BaseWall::BaseWall()
{
    handler_ = 0;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseWall::BaseWall() finished"<<std::endl;
#endif
}

BaseWall::BaseWall(const BaseWall &p)
        : BaseInteractable(p)
{
    handler_ = p.handler_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseWall::BaseWall(const BaseWall &p) finished"<<std::endl;
#endif
}

BaseWall::~BaseWall()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseWall::~BaseWall() finished"<<std::endl;
#endif   
}

void BaseWall::read(std::istream& is)
{
    BaseInteractable::read(is);
}
void BaseWall::write(std::ostream& os) const
{
    BaseInteractable::write(os);
}

void BaseWall::clear()
{
    std::cout << "BaseWall::clear(), this function shouldn't be called" << std::endl;
}

void BaseWall::setHandler(WallHandler* handler)
{
    handler_ = handler;
    setSpecies(getHandler()->getDPMBase()->speciesHandler.getObject(getIndSpecies()));
}

WallHandler* BaseWall::getHandler() const
{
    return handler_;
}

void BaseWall::setIndSpecies(unsigned int indSpecies)
{
    if(handler_!=nullptr)
    {
        setSpecies(getHandler()->getDPMBase()->speciesHandler.getObject(getIndSpecies()));
    }
    else
    {
        BaseInteractable::setIndSpecies(indSpecies);
    }
}

void BaseWall::setSpecies(const BaseSpecies* species)
{
    BaseInteractable::setSpecies(species);

    //set pointer to the handler, which is needed to retrieve species information
    if (getHandler()== nullptr)
    {
        SpeciesHandler* sH = species->getHandler();
        if (sH!= nullptr)
        {
            DPMBase* dB = sH->getDPMBase();
            if (dB!= nullptr)
                setHandler(&dB->wallHandler);
        }
    }
}