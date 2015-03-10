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
    handler_ = nullptr;
    logger(DEBUG, "BaseWall::BaseWall() finished");
}

/*!
 * \param[in] w Wall that must be copied.
 */
BaseWall::BaseWall(const BaseWall& w)
        : BaseInteractable(w)
{
    handler_ = w.handler_;
    logger(DEBUG, "BaseWall::BaseWall(const BaseWall &p) finished");
}

BaseWall::~BaseWall()
{
    logger(DEBUG, "BaseWall::~BaseWall() finished");
}

void BaseWall::clear()
{
    logger(WARN, "BaseWall::clear(), this function shouldn't be called");
}

/*!
 * \param[in] is Input stream from which the BaseWall is read.
 */
void BaseWall::read(std::istream& is)
{
    BaseInteractable::read(is);
}

/*!
 * \param[in] os Output stream the BaseWall has to be written to.
 */
void BaseWall::write(std::ostream& os) const
{
    BaseInteractable::write(os);
}

/*!
 * \param[in] handler A pointer to the BaseHandler that handles this wall.
 */
void BaseWall::setHandler(WallHandler* handler)
{
    handler_ = handler;
    setSpecies(getHandler()->getDPMBase()->speciesHandler.getObject(getIndSpecies()));
}

/*!
 * \return A pointer to the WallHandler that manages this BaseWall.
 */
WallHandler* BaseWall::getHandler() const
{
    return handler_;
}

/*!
 * \param[in] indSpecies The index of the species of this BaseWall in the SpeciesHandler.
 */
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

/*!
 * \param[in] species The species this BaseWall is made of.
 * \details In addition to the functionality of BaseInteractable::setSpecies, this function sets the pointer to the
 * wallHandler, which is needed to retrieve species information.
 */
void BaseWall::setSpecies(const ParticleSpecies* species)
{
    BaseInteractable::setSpecies(species);

    //set pointer to the handler, which is needed to retrieve species information
    if (getHandler() == nullptr)
    {
        SpeciesHandler* sH = species->getHandler();
        if (sH != nullptr)
        {
            DPMBase* dB = sH->getDPMBase();
            if (dB != nullptr)
                setHandler(&dB->wallHandler);
        }
    }
}
