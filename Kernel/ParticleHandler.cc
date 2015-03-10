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


/*!
 * \details Constructor of the ParticleHandler class. It creates and empty 
 *          ParticleHandler.
 */
ParticleHandler::ParticleHandler()
{
    largestParticle_ = nullptr;
    smallestParticle_ = nullptr;
    logger(DEBUG, "ParticleHandler::ParticleHandler() finished");
}

/*!
 * \param[in] PH The ParticleHandler that has to be copied. 
 * \details This is not a copy constructor! It copies the DPMBase and all 
 *          BaseParticle, and sets the other variables to 0. After that, it 
 *          computes the smallest and largest particle in this handler.
 */
ParticleHandler::ParticleHandler(const ParticleHandler& PH)
    : BaseHandler<BaseParticle>() 
{
    clear();
    setDPMBase(PH.getDPMBase());
    largestParticle_ = nullptr;
    smallestParticle_ = nullptr;
    copyContentsFromOtherHandler(PH);
    if (objects_.size() != 0)
    {
        computeLargestParticle();
        computeSmallestParticle();
    }
    logger(DEBUG, "ParticleHandler::ParticleHandler(const ParticleHandler &PH) finished");
}

/*!
 * \param[in] rhs The ParticleHandler on the right hand side of the assignment.
 * \details This is not a copy assignment operator! It copies the DPMBase and all 
 *          BaseParticle, and sets the other variables to 0. After that, it 
 *          computes the smallest and largest particle in this handler.
 */
ParticleHandler ParticleHandler::operator =(const ParticleHandler& rhs)
{
    if (this != &rhs)
    {
        clear();
        largestParticle_ = nullptr;
        smallestParticle_ = nullptr;
        copyContentsFromOtherHandler(rhs);
        if (objects_.size() != 0)
        {
            computeLargestParticle();
            computeSmallestParticle();
        }
    }
    logger(DEBUG, "ParticleHandler::operator = (const ParticleHandler& rhs) finished");
    return *this;
}

/*!
 * \details Set the pointers to largestParticle_ and smallestParticle_ to 
 *          nullptr, all BaseParticle are destroyed by the BaseHandler afterwards.
 */
ParticleHandler::~ParticleHandler()
{
    //First reset the pointers, such that they are not checked twice when removing particles
    largestParticle_ = nullptr;
    smallestParticle_ = nullptr;
    logger(DEBUG, "ParticleHandler::~ParticleHandler() finished");
}

/*!
 * \param[in] P A pointer to the BaseParticle that has to be added. 
 * \details To add a BaseParticle to the ParticleHandler, first check if it has
 *          a species, since it is as common bug to use a BaseParticle without
 *          species, which leads to a segmentation fault. To help the user with
 *          debugging, a warning is given if a particle without species is added.
 *          After that, the actions for adding the particle to the BaseHandler
 *          are taken, which include adding it to the vector of pointers to all 
 *          BaseParticle and assigning the correct id and index. Then the 
 *          particle is added to the HGrid, the particle is told that this is its
 *          handler, its mass is computed and finally it is checked if this is 
 *          the smallest or largest particle in this ParticleHandler.
 */
void ParticleHandler::addObject(BaseParticle* P)
{
    if (P->getSpecies() == nullptr)
    {
        logger(WARN, "WARNING: The particle with ID % that is added in "
                "ParticleHandler::addObject does not have a species yet."
                "Please make sure that you have "
                "set the species somewhere in the driver code.", P->getId());
    }
    //Puts the particle in the Particle list
    BaseHandler<BaseParticle>::addObject(P);
    if (getDPMBase() != nullptr)
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

/*!
 * \param[in] id    The index of which BaseParticle has to be removed from this 
 *                  ParticleHandler.
 * \details         The BaseParticle at position id is removed by moving the last 
 *                  BaseParticle in the vector to the position of id. It also 
 *                  removes the BaseParticle from the HGrid.
 */
void ParticleHandler::removeObject(unsigned const int id)
{
#ifdef CONTACT_LIST_HGRID
    getDPMBase()->getPossibleContactList().remove_ParticlePosibleContacts(getObject(id));
#endif
    getDPMBase()->hGridRemoveParticle(getObject(id));
    BaseHandler<BaseParticle>::removeObject(id);
}

/*!
 * \details Function that removes the last object from this ParticleHandler. It
 *          also removes the particle from the HGrid.
 */
void ParticleHandler::removeLastObject()
{
#ifdef CONTACT_LIST_HGRID
    getDPMBase()->getPossibleContactList().remove_ParticlePosibleContacts(getLastObject());
#endif
    getDPMBase()->hGridRemoveParticle(getLastObject());
    BaseHandler<BaseParticle>::removeLastObject();
}

void ParticleHandler::computeSmallestParticle()
{
    if (getNumberOfObjects() == 0)
    {
        logger(WARN, "No particles, so cannot compute the smallest particle.");
        return;
    }
    Mdouble min = std::numeric_limits<Mdouble>::max();
    smallestParticle_ = nullptr;
    for (BaseParticle* const particle : objects_)
    {
        if (particle->getInteractionRadius() < min)
        {
            min = particle->getInteractionRadius();
            smallestParticle_ = particle;
        }
    }
}

void ParticleHandler::computeLargestParticle() 
{
    if (getNumberOfObjects() == 0)
    {
        logger(WARN, "No particles, so cannot compute the largest particle.");
        return;
    }
    Mdouble max = -std::numeric_limits<Mdouble>::max();
    largestParticle_ = nullptr;
    for (BaseParticle* const particle : objects_)
    {
        if (particle->getInteractionRadius() > max)
        {
            max = particle->getInteractionRadius();
            largestParticle_ = particle;
        }
    }
}

/*!
 * \return  A pointer to the to the smallest BaseParticle (by interactionRadius) 
 *          in this ParticleHandler.
 */
BaseParticle* ParticleHandler::getSmallestParticle() const
{
    if (smallestParticle_ == nullptr)
    {
        logger(WARN, "No particles to return in getSmallestParticle()");
    }
    return smallestParticle_;
}

/*!
 * \return A pointer to the largest BaseParticle (by interactionRadius) in this 
 *         ParticleHandler.
 */
BaseParticle* ParticleHandler::getLargestParticle() const
{
    if (largestParticle_ == nullptr)
    {
        logger(WARN, "No particles to set get_LargestParticle()");
    }
    return largestParticle_;
}

/*!
 * \return A pointer to the fastest BaseParticle in this ParticleHandler.
 */
BaseParticle* ParticleHandler::getFastestParticle() const
{
    if (getNumberOfObjects() == 0)
    {
        logger(WARN, "No particles to set getFastestParticle()" );
        return nullptr;
    }
    BaseParticle* p = nullptr;
    Mdouble maxSpeed = -std::numeric_limits<Mdouble>::max();
    for (BaseParticle* const pLoop : objects_)
    {
        if ((pLoop->getVelocity().getLength()) > maxSpeed)
        {
            maxSpeed = pLoop->getVelocity().getLength();
            p = pLoop;
        }
    }
    return p;
}

/*!
 * \param[in] i Direction for which you want the particle with lowest coordinates.
 * \return      A pointer to the particle with the lowest coordinates in the
 *              given direction in this ParticleHandler.
 */
BaseParticle* ParticleHandler::getLowestPositionComponentParticle(const int i) const
{
    if (getNumberOfObjects() == 0)
    {
        logger(WARN, "No getLowestPositionComponentParticle(const int i) since there are no particles.");
        return nullptr;
    }
    BaseParticle* p = nullptr;
    Mdouble min = std::numeric_limits<Mdouble>::max();
    for (BaseParticle* const pLoop : objects_)
    {
        if (pLoop->getPosition().getComponent(i) < min)
        {
            min = pLoop->getPosition().getComponent(i);
            p = pLoop;
        }
    }
    return p;
}

/*!
 * \param[in] i Direction for which one wants the particle with highest coordinates.
 * \return      A pointer to the particle with the highest coordinates in 
 *              direction i in this ParticleHandler.
 */
BaseParticle* ParticleHandler::getHighestPositionComponentParticle(const int i) const
{
    if (getNumberOfObjects() == 0)
    {
        logger(WARN, "No getHighestPositionComponentParticle(const int i) since there are no particles.");
        return nullptr;
    }
    BaseParticle* p = nullptr;
    Mdouble max = -std::numeric_limits<Mdouble>::max();
    for (BaseParticle* const pLoop : objects_)
    {
        if (pLoop->getPosition().getComponent(i) > max)
        {
            max = pLoop->getPosition().getComponent(i);
            p = pLoop;
        }
    }
    
    return p;
}

/*!
 * \param[in] i Direction for which you want the particle with lowest velocity.
 * \return      A pointer to the particle with the lowest velocity in direction 
 *              i in this ParticleHandler.
 */
BaseParticle* ParticleHandler::getLowestVelocityComponentParticle(const int i) const
{
    if (getNumberOfObjects() == 0)
    {
        logger(WARN, "No getLowestVelocityComponentParticle(const int i) since there are no particles");
        return nullptr;
    }
    BaseParticle* p = nullptr;
    Mdouble min = std::numeric_limits<Mdouble>::max();
    for (BaseParticle* const pLoop : objects_)
    {
        if (pLoop->getVelocity().getComponent(i) < min)
        {
            min = pLoop->getVelocity().getComponent(i);
            p = pLoop;
        }
    }
    return p;
}

/*!
 * \param[in] i Direction for which you want the particle with highest velocity.
 * \return      A pointer to the particle with the highest velocity in direction
 *              i in this ParticleHandler.
 */
BaseParticle* ParticleHandler::getHighestVelocityComponentParticle(const int i) const
{
    if (!getNumberOfObjects())
    {
        logger(WARN, "No getHighestVelocityComponentParticle(const int i) since there are no particles");
        return nullptr;
    }
    BaseParticle* p = nullptr;
    Mdouble max = -std::numeric_limits<Mdouble>::max();
    for (BaseParticle* const pLoop : objects_)
    {
        if (pLoop->getVelocity().getComponent(i) > max)
        {
            max = pLoop->getVelocity().getComponent(i);
            p = pLoop;
        }
    }
    return p;
}

/*!
 * \return A pointer to the to the lightest BaseParticle (by mass) in this ParticleHandler.
 */
BaseParticle* ParticleHandler::getLightestParticle() const
{
    if (getNumberOfObjects() == 0)
    {
        logger(WARN, "No particles to set getLightestParticle()");
        return nullptr;
    }
    BaseParticle* p = nullptr;
    Mdouble minMass = std::numeric_limits<Mdouble>::max();
    for (BaseParticle* const pLoop : objects_)
    {
        if (pLoop->getMass() < minMass)
        {
            minMass = pLoop->getMass();
            p = pLoop;
        }
    }
    return p;
}

/*!
 * \details Note that the pointers to smallestParticle_ and largestParticle_ are
 *          set to nullptr since these particles don't exist anymore after 
 *          calling this function.
 */
void ParticleHandler::clear()
{
    smallestParticle_ = nullptr;
    largestParticle_ = nullptr;
    BaseHandler<BaseParticle>::clear();
}

/*!
 * \param[in] is The input stream from which the information is read.
 */
void ParticleHandler::readObject(std::istream& is)
{
    std::string type;
    is >> type;
    if (type == "BaseParticle")
    {
        BaseParticle baseParticle;
        is >> baseParticle;
        copyAndAddObject(baseParticle);
        getLastObject()->setId(baseParticle.getId()); //to ensure old id
        ///\todo make sure setting the id doesn't break the id setter :)
    }
    ///\todo{Remove for final version}
    else if (type == "BP")
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
        logger(ERROR, "Particle type % not understood in restart file. Particle has not been read.", type);
        return;
    }
}

/*!
 * \param[in] type The first value of the position.
 * \param[in] is The input stream from which the information is read.
 * \details The old objects did not have their type in the beginning of the line. 
 *          Instead, the first string of the file was the position in x-direction. 
 *          Since we already read the first string of the file, we need to give 
 *          it to this function and convert it to the position in x-direction. 
 *          The rest of the stream is then read in the usual way.
 */
void ParticleHandler::readOldObject(std::string type, std::istream& is)
{
    //read in next line
    std::stringstream line(std::stringstream::in | std::stringstream::out);
    helpers::getLineFromStringStream(is, line);
    logger(VERBOSE, line.str());
    //std::cout << line.str() << std::endl;

    BaseParticle particle;
    
    //Declare all properties of the particle
    unsigned int indSpecies;
    Mdouble radius, inverseMass, inverseInertia;
    Vec3D position, velocity, orientation, angularVelocity;
    
    //Read all values
    position.X = atof(type.c_str());

    line >> position.Y >> position.Z >> velocity >> radius >> orientation >> angularVelocity >> inverseMass >> inverseInertia >> indSpecies;
    
    //Put the values in the particle    
    particle.setIndSpecies(indSpecies);
    particle.setPosition(position);
    particle.setVelocity(velocity);
    particle.setRadius(radius);
    particle.setOrientation(orientation);
    particle.setAngularVelocity(angularVelocity);
    if (inverseMass == 0.0)
        particle.fixParticle();
    else
    {
        particle.setInertia(1./inverseInertia);
    }
    
    //Put the particle in the  handler
    copyAndAddObject(particle);
}

void ParticleHandler::write(std::ostream& os) const
{
    os << "Particles " << getNumberOfObjects() << std::endl;
    for (BaseParticle* it : *this)
        os << (*it) << std::endl;
}

/*!
 *  \param[in] P A pointer to the particle, which properties have to be checked 
 *               against the ParticleHandlers extrema.
 */
void ParticleHandler::checkExtrema(BaseParticle* P)
{
    if (P == largestParticle_) 
    {
        //if the properties of the largest particle changes
        computeLargestParticle();
    } 
    else if (!largestParticle_ || P->getInteractionRadius() > largestParticle_->getInteractionRadius())
    {
        largestParticle_ = P;
    }
    
    if (P == smallestParticle_) 
    {
        //if the properties of the smallest particle changes
        computeSmallestParticle();
    } 
    else if (!smallestParticle_ || P->getInteractionRadius() < smallestParticle_->getInteractionRadius())
    {
        smallestParticle_ = P;
    }
}

/*!
 * \param[in] P A pointer to the particle, which is going to get deleted.
 */
void ParticleHandler::checkExtremaOnDelete(BaseParticle* P)
{
    if (P == largestParticle_)
    {
        computeLargestParticle();
    }
    if (P == smallestParticle_)
    {
        computeSmallestParticle();
    }
}

/*!
 * \param[in] indSpecies Unsigned integer with the index of the species for which
 *                       the masses must be computed.
 */
void ParticleHandler::computeAllMasses(unsigned int indSpecies)
{
     for (BaseParticle* particle : objects_)
     {
        if (particle->getIndSpecies() == indSpecies)
        {
            particle->getSpecies()->computeMass(particle);
        }
     }
}

void ParticleHandler::computeAllMasses()
{
    for (BaseParticle* particle : objects_)
    {
        particle->getSpecies()->computeMass(particle);
    }
}

/*!
 * \return The string "ParticleHandler".
 */
std::string ParticleHandler::getName() const
{
    return "ParticleHandler";
}
