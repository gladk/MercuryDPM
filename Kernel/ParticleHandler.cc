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

#include <limits>
#include "ParticleHandler.h"
#include "DPMBase.h"
#include "SpeciesHandler.h"



///Constructor of the ParticleHandler class. It creates and empty ParticleHandler.
ParticleHandler::ParticleHandler()
{
    largestParticle_ = 0;
    smallestParticle_ = 0;
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "ParticleHandler::ParticleHandler() finished" << std::endl;
#endif
}

/// \param[in] PH The ParticleHandler that has te be copied.
ParticleHandler::ParticleHandler(const ParticleHandler& PH)
//    : BaseHandler<BaseParticle>()
{
    clear();
    setDPMBase(PH.getDPMBase());
    largestParticle_ = 0;
    smallestParticle_ = 0;
    copyContentsFromOtherHandler(PH);
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "ParticleHandler::ParticleHandler(const ParticleHandler &PH) finished" << std::endl;
#endif
}

/// \param[in] rhs The ParticleHandler on the right hand side of the assignment.
ParticleHandler ParticleHandler::operator =(const ParticleHandler& rhs)
{
    if (this != &rhs)
    {
        clear();
        largestParticle_ = 0;
        smallestParticle_ = 0;
        copyContentsFromOtherHandler(rhs);
    }
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "ParticleHandler::operator = (const ParticleHandler& rhs) finished" << std::endl;
#endif
    return *this;
}

ParticleHandler::~ParticleHandler()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout << "ParticleHandler::~ParticleHandler() finished" << std::endl;
#endif
}

/// \param[in] P A pointer to the BaseParticle (or derived class) that has to be added.
/// \todo Also insert the particle in the HGRID
void ParticleHandler::addObject(BaseParticle* P)
{
    //Puts the particle in the Particle list
    BaseHandler<BaseParticle>::addObject(P);
    if (getDPMBase())
    {
        //This places the particle in this grid
        getDPMBase()->hGridInsertParticle(P);
        //This computes where the particle currently is in the grid
        getDPMBase()->hGridUpdateParticle(P);
    }
    //set the particleHandler pointer
    P->setHandler(this);
    //compute mass of the particle
    P->computeMass();
    //Check if this particle has new extrema
    checkExtrema(P);
}

/// \details The BaseParticle at position id is removed by moving the last BaseParticle in the vector to the position of id. It also removes the BaseParticle from the HGrid.
/// \param[in] id The index of which BaseParticle has to be removed from the ParticleHandler
void ParticleHandler::removeObject(unsigned const int id)
{
#ifdef CONTACT_LIST_HGRID
    getDPMBase()->getPossibleContactList().remove_ParticlePosibleContacts(getObject(id));
#endif
    getDPMBase()->hGridRemoveParticle(getObject(id));
    BaseHandler<BaseParticle>::removeObject(id);
}

/// \details It also removes the particle from the HGrid.
void ParticleHandler::removeLastObject()
{
#ifdef CONTACT_LIST_HGRID
    getDPMBase()->getPossibleContactList().remove_ParticlePosibleContacts(getLastObject());
#endif
    getDPMBase()->hGridRemoveParticle(getLastObject());
    BaseHandler<BaseParticle>::removeLastObject();
}

/// \return A pointer to the to the smallest BaseParticle (by interactionRadius) in this ParticleHandler.
BaseParticle* ParticleHandler::getSmallestParticle() const
{
    if (!smallestParticle_)
    {
        std::cerr << "Warning: No particles to set get_SmallestParticle()" << std::endl;
    }
    return smallestParticle_;
}

/// \return A pointer to the to the fastest BaseParticle in this ParticleHandler.
BaseParticle* ParticleHandler::getFastestParticle() const
{
    if (!getNumberOfObjects())
    {
        std::cerr << "Warning: No particles to set get_FastestParticle()" << std::endl;
    }
    BaseParticle* p = 0;
    Mdouble maxSpeed = -std::numeric_limits<Mdouble>::max();
    for (std::vector<BaseParticle*>::const_iterator it = begin(); it != end(); it++)
    {
        if ((*it)->getVelocity().getLength() > maxSpeed)
        {
            maxSpeed = (*it)->getVelocity().getLength();
            p = (*it);
        }
    }
    return p;
}

BaseParticle* ParticleHandler::getLowestPositionComponentParticle(const int i) const
        {
    if (!getNumberOfObjects())
    {
        std::cerr << "Warning: No getLowestPositionComponentParticle(const int i)" << std::endl;
    }
    BaseParticle* p = 0;
    Mdouble min = std::numeric_limits<Mdouble>::max();
    for (std::vector<BaseParticle*>::const_iterator it = begin(); it != end(); it++)
    {
        if ((*it)->getPosition().getComponent(i) < min)
        {
            min = (*it)->getPosition().getComponent(i);
            p = (*it);
        }
    }
    return p;
}
BaseParticle* ParticleHandler::getHighestPositionComponentParticle(const int i) const
        {
    if (!getNumberOfObjects())
    {
        std::cerr << "Warning: No getHighestPositionComponentParticle(const int i)" << std::endl;
    }
    BaseParticle* p = 0;
    Mdouble max = -std::numeric_limits<Mdouble>::max();
    for (std::vector<BaseParticle*>::const_iterator it = begin(); it != end(); it++)
    {
        if ((*it)->getPosition().getComponent(i) > max)
        {
            max = (*it)->getPosition().getComponent(i);
            p = (*it);
        }
    }
    return p;
}
BaseParticle* ParticleHandler::getLowestVelocityComponentParticle(const int i) const
        {
    if (!getNumberOfObjects())
    {
        std::cerr << "Warning: No getLowestVelocityComponentParticle(const int i)" << std::endl;
    }
    BaseParticle* p = 0;
    Mdouble min = std::numeric_limits<Mdouble>::max();
    for (std::vector<BaseParticle*>::const_iterator it = begin(); it != end(); it++)
    {
        if ((*it)->getVelocity().getComponent(i) < min)
        {
            min = (*it)->getVelocity().getComponent(i);
            p = (*it);
        }
    }
    return p;
}
BaseParticle* ParticleHandler::getHighestVelocityComponentParticle(const int i) const
        {
    if (!getNumberOfObjects())
    {
        std::cerr << "Warning: No getHighestVelocityComponentParticle(const int i)" << std::endl;
    }
    BaseParticle* p = 0;
    Mdouble max = -std::numeric_limits<Mdouble>::max();
    for (std::vector<BaseParticle*>::const_iterator it = begin(); it != end(); it++)
    {
        if ((*it)->getVelocity().getComponent(i) > max)
        {
            max = (*it)->getVelocity().getComponent(i);
            p = (*it);
        }
    }
    return p;
}

/// \return A pointer to the to the lightest BaseParticle (by mass) in this ParticleHandler.
BaseParticle* ParticleHandler::getLightestParticle() const
{
    if (!getNumberOfObjects())
    {
        std::cerr << "Warning: No particles to set getLightestParticle()" << std::endl;
        throw;
    }
    BaseParticle* p = 0;
    Mdouble minMass = std::numeric_limits<Mdouble>::max();
    for (std::vector<BaseParticle*>::const_iterator it = begin(); it != end(); it++)
    {
        if ((*it)->getMass() < minMass)
        {
            minMass = (*it)->getMass();
            p = (*it);
        }
    }
    return p;
}

/// \return A pointer to the to the largest BaseParticle (by interactionRadius) in this ParticleHandler.
BaseParticle* ParticleHandler::getLargestParticle() const
{
    if (!largestParticle_)
    {
        std::cerr << "Warning: No particles to set get_LargestParticle()" << std::endl;
        //throw;
    }
    return largestParticle_;
}

void ParticleHandler::clear()
{
    smallestParticle_ = 0;
    largestParticle_ = 0;
    BaseHandler<BaseParticle>::clear();
}

/// \param[in] is The input stream from which the information is read.
void ParticleHandler::readObject(std::istream& is)
{
    std::string type;
    is >> type;
    if (type.compare("BaseParticle") == 0)
    {
        BaseParticle baseParticle;
        is >> baseParticle;
        copyAndAddObject(baseParticle);
    }
    ///todo{Remove for final version}
    else if (type.compare("BP") == 0)
    {
        BaseParticle baseParticle;
        baseParticle.oldRead(is);
        copyAndAddObject(baseParticle);
    }
    else if (isdigit(type[0]))
    {
        BaseParticle baseParticle;
        baseParticle.oldRead(is);
        copyAndAddObject(baseParticle);
    }
    else
    {
        std::cerr << "Particle type: " << type << " not understood in restart file" << std::endl;
        exit(-1);
    }
}

/// \param[in] P A pointer to the particle, which properties have to be checked against the ParticleHandlers extrema.
void ParticleHandler::checkExtrema(BaseParticle* P)
{
    if (!largestParticle_ || P->getInteractionRadius() > largestParticle_->getInteractionRadius())
    {
        largestParticle_ = P;
    }
    if (!smallestParticle_ || P->getInteractionRadius() < smallestParticle_->getInteractionRadius())
    {
        smallestParticle_ = P;
    }
}

/// \param[in] P A pointer to the particle, which is going to get deleted.
void ParticleHandler::checkExtremaOnDelete(BaseParticle* P)
{
    if (P == largestParticle_)
    {
        largestParticle_ = 0;
        Mdouble maxRadius = -std::numeric_limits<Mdouble>::max();
        for (std::vector<BaseParticle*>::const_iterator it = begin(); it != end(); it++)
        {
            if ((*it)->getInteractionRadius() > maxRadius && P != (*it))
            {
                maxRadius = (*it)->getInteractionRadius();
                largestParticle_ = (*it);
            }
        }
    }
    if (P == smallestParticle_)
    {
        smallestParticle_ = 0;
        Mdouble minRadius = std::numeric_limits<Mdouble>::max();
        for (std::vector<BaseParticle*>::const_iterator it = begin(); it != end(); it++)
        {
            if ((*it)->getInteractionRadius() < minRadius && P != (*it))
            {
                minRadius = (*it)->getInteractionRadius();
                smallestParticle_ = (*it);
            }
        }
    }
}

void ParticleHandler::computeAllMasses(unsigned int indSpecies)
{
    for (auto it= begin(); it!=end(); it++)
        if ((*it)->getIndSpecies()==indSpecies)
            (*it)->computeMass();
}

void ParticleHandler::computeAllMasses()
{
    for (auto it= begin(); it!=end(); it++)
        (*it)->computeMass();
}

std::string ParticleHandler::getName() const
{
    return "ParticleHandler";
}
