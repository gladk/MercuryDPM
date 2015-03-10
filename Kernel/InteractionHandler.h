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

#ifndef INTERACTIONHANDLER_H
#define INTERACTIONHANDLER_H
#include "BaseHandler.h"
#include "Interactions/BaseInteraction.h"

class SpeciesHandler;
/*!
 * \class InteractionHandler
 * \brief Container to store all BaseInteraction
 * \details The InteractionHandler is a container to store all BaseInteraction. It is implemented by a vector of pointers to BaseInteraction.
 */

class InteractionHandler : public BaseHandler<BaseInteraction>
{
public:
    /*!
     * \brief Default constructor, it simply creates an empty InteractionHandler.
     */
    InteractionHandler();

    /*!
     * \brief Copy constructor, it copies the InteractionHandler and all BaseInteraction it contains.
     */
    InteractionHandler(const InteractionHandler& IH);

    /*!
     * \brief Assignment operator.
     */
    InteractionHandler operator =(const InteractionHandler& rhs);
    
    /*!
     * \brief Destructor, it simply destructs the InteractionHandler and all BaseInteraction it contains.
     */
    ~InteractionHandler();

    /*!
     * \brief Adds a BaseInteraction to the InteractionHandler.
     */
    void addObject(BaseInteraction* I);

    /*!
     * \brief Reads BaseInteraction into the InteractionHandler from restart data.
     */
    void readObject(std::istream& is);

    /*!
     * \brief 
     */
    BaseInteraction* getExistingInteraction(BaseInteractable* P, BaseInteractable* I);
    
    /*!
     * \brief 
     */
    BaseInteraction* getInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);

    /*!
     * \brief 
     */
    virtual void removeObjectKeepingPeriodics(unsigned const int id);
    
    /*!
     * \brief 
     */
    void eraseOldInteractions(Mdouble lastTimeStep);

    /*!
     * \brief
     */
    void setSpecies(SpeciesHandler* speciesHandler);

    std::string getName() const;

    Mdouble timeStep_;

private:
    /*!
     * \brief 
     */
    SpeciesHandler* speciesHandler_;
};
#endif

