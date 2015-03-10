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

#include "BaseWall.h"
#include "DPMBase.h"
#include "WallHandler.h"
#include "Species/ParticleSpecies.h"

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

void BaseWall::setSpecies(const ParticleSpecies* species)
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