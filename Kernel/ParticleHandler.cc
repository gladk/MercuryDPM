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


#include <limits>
#include <Math/Helpers.h>
#include "ParticleHandler.h"
#include "DPMBase.h"
#include "SpeciesHandler.h"
#include "Species/ParticleSpecies.h"



///Constructor of the ParticleHandler class. It creates and empty ParticleHandler.
ParticleHandler::ParticleHandler()
{
    largestParticle_ = nullptr;
    smallestParticle_ = nullptr;
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "ParticleHandler::ParticleHandler() finished" << std::endl;
#endif
}

/// \param[in] PH The ParticleHandler that has to be copied.
ParticleHandler::ParticleHandler(const ParticleHandler& PH)
//    : BaseHandler<BaseParticle>()
{
    clear();
    setDPMBase(PH.getDPMBase());
    largestParticle_ = nullptr;
    smallestParticle_ = nullptr;
    copyContentsFromOtherHandler(PH);
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "ParticleHandler::ParticleHandler(const ParticleHandler &PH) finished" << std::endl;
#endif
}

/// \param[in] rhs The ParticleHandler on the right hand side of the assignment.
ParticleHandler ParticleHandler::operator =(const ParticleHandler& rhs)
{
    if (this != &rhs)
    {
        clear();
        largestParticle_ = nullptr;
        smallestParticle_ = nullptr;
        copyContentsFromOtherHandler(rhs);
    }
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "ParticleHandler::operator = (const ParticleHandler& rhs) finished" << std::endl;
#endif
    return *this;
}

ParticleHandler::~ParticleHandler()
{
    //First reset the pointers, such that they are not checked when removing particles
    largestParticle_ = nullptr;
    smallestParticle_ = nullptr;
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
    P->getSpecies()->computeMass(P)	;
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
        std::cerr << "Warning: No particles to set getSmallestParticle()" << std::endl;
    }
    return smallestParticle_;
}

/// \return A pointer to the to the fastest BaseParticle in this ParticleHandler.
BaseParticle* ParticleHandler::getFastestParticle() const
{
    if (!getNumberOfObjects())
    {
        std::cerr << "Warning: No particles to set getFastestParticle()" << std::endl;
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
        getLastObject()->setId(baseParticle.getId()); //to ensure old id
        ///\todo make sure setting the id doesn't break the id setter :)
    }
    ///todo{Remove for final version}
    else if (type.compare("BP") == 0)
    {
        BaseParticle baseParticle;
        baseParticle.oldRead(is);
        copyAndAddObject(baseParticle);
        getLastObject()->setId(baseParticle.getId()); //to ensure old id
    }
    else if (isdigit(type[0]))
    {
        readOldObject(type, is);
    }
    else
    {
        std::cerr << "Particle type: " << type << " not understood in restart file" << std::endl;
        exit(-1);
    }
}

/// \param[in] is The input stream from which the information is read.
void ParticleHandler::readOldObject(std::string type, std::istream& is)
{
    //read in next line
    std::stringstream line(std::stringstream::in | std::stringstream::out);
    helpers::getLineFromStringStream(is, line);
    //std::cout << line.str() << std::endl;

    BaseParticle particle;
    std::string dummy;
    Mdouble radius, inverseMass, inverseInertia, indSpecies;
    Vec3D position, velocity, orientation, angularVelocity;
    position.X = atof(type.c_str());

    line >> position.Y >> position.Z >> velocity >> radius >> orientation >> angularVelocity >> inverseMass >> inverseInertia >> indSpecies;
    particle.setPosition(position);
    particle.setVelocity(velocity);
    particle.setRadius(radius);
    particle.setOrientation(orientation);
    particle.setAngularVelocity(angularVelocity);
    if (inverseMass==0)
        particle.fixParticle();
    else
    {
        particle.setInertia(1./inverseInertia);
        particle.setMass(1./inverseMass);
    }
    particle.setIndSpecies(indSpecies);
    copyAndAddObject(particle);
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
    /** \todo: expose objects so we can iterate over them.
     * Expose either objects_ or rip out the entire basehandler as it
     * is NOT linear in memory and has no clear API.
     * @dducks
     **/
     for (BaseParticle* particle : *this)
        if (particle->getIndSpecies()==indSpecies)
            particle->getSpecies()->computeMass(particle);
}

void ParticleHandler::computeAllMasses()
{
    /** \todo: expose objects so we can iterate over them.
    * Expose either objects_ or rip out the entire basehandler as it
    * is NOT linear in memory and has no clear API.
    * @dducks
    **/
    for (BaseParticle* particle : *this)
        particle->getSpecies()->computeMass(particle);
}

std::string ParticleHandler::getName() const
{
    return "ParticleHandler";
}
