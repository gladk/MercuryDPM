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


#ifndef PARTICLE_HANDLER_H
#define PARTICLE_HANDLER_H

#include "BaseHandler.h"
#include "Particles/BaseParticle.h"

class SpeciesHandler;
class BaseSpecies;

/*!
 * \class ParticleHandler
 * \brief Container to store all BaseParticle.
 * \details The ParticleHandler is a container to store all BaseParticle. It is implemented by a vector of pointers to BaseParticle.
 */
class ParticleHandler : public BaseHandler<BaseParticle>
{
public:
    /*!
     * \brief Default constructor, it creates an empty ParticleHandler.
     */
    ParticleHandler();

    /*!
     * \brief Copy constructor, it copies the ParticleHandler and all BaseParticle it contains.
     */
    ParticleHandler(const ParticleHandler& PH);

    /*!
     * \brief Assignment operator. 
     */
    ParticleHandler operator=(const ParticleHandler& rhs);
    
    /*!
     * \brief Destructor, it destructs the ParticleHandler and all BaseParticle it contains.
     */
    ~ParticleHandler();

    /*!
     * \brief Adds a BaseParticle to the ParticleHandler. 
     */
    virtual void addObject(BaseParticle* P);

    /*!
     * \brief Removes a BaseParticle from the ParticleHandler. 
     */
    virtual void removeObject(unsigned const int id);

    /*!
     * \brief Removes the last BaseParticle from the ParticleHandler.
     */
    void removeLastObject();

    /*!
     * \brief Computes the smallest particle (by interaction radius) and sets it in smallestParticle_
     */
    void computeSmallestParticle();
    
    /*!
     * \brief Computes the largest particle (by interaction radius) and sets it in largestParticle_
     */
    void computeLargestParticle();
    
    /*!
     * \brief Gets a pointer to the smallest BaseParticle (by interactionRadius) in this ParticleHandler. 
     */
    BaseParticle* getSmallestParticle() const;

    /*!
     * \brief Gets a pointer to the largest BaseParticle (by interactionRadius) in this ParticleHandler. 
     */
    BaseParticle* getLargestParticle() const;

    /*!
     * \brief Gets a pointer to the fastest BaseParticle in this ParticleHandler.
     */
    BaseParticle* getFastestParticle() const;

    /*!
     * \brief Gets a pointer to the particle with the lowest coordinates in direction i in this ParticleHandler.
     */
    BaseParticle* getLowestPositionComponentParticle(const int i) const;

    /*!
     * \brief Gets a pointer to the particle with the highest coordinates in direction i in this ParticleHandler. 
     */
    BaseParticle* getHighestPositionComponentParticle(const int i) const;

    /*!
     * \brief Gets a pointer to the particle with the lowest velocity in direction i in this ParticleHandler. 
     */
    BaseParticle* getLowestVelocityComponentParticle(const int i) const;

    /*!
     * \brief Gets a pointer to the particle with the highest velocity in direction i in this ParticleHandler. 
     */
    BaseParticle* getHighestVelocityComponentParticle(const int i) const;

    /*!
     * \brief Gets a pointer to the lightest BaseParticle (by mass) in this ParticleHandler. 
     */
    BaseParticle* getLightestParticle() const;

    /*!
     * \brief Empties the whole ParticleHandler by removing all BaseParticle.
     */
    void clear();

    /*!
     * \brief Reads BaseParticle into the ParticleHandler from restart data.
     */
    void readObject(std::istream& is);

    /*!
     * \brief Reads BaseParticle into the ParticleHandler from old-style restart data. 
     */
    void readOldObject(std::string type, std::istream& is);

    /*!
     * \brief Checks if the extrema of this ParticleHandler needs updating.
     * 
     */
    void checkExtrema(BaseParticle* P);

    /*!
     * \brief Checks if the extrema of this ParticleHandler needs updating when a particle is deleted.
     */
    void checkExtremaOnDelete(BaseParticle* P);

    /*!
     * \brief Computes the mass for all BaseParticle of the given species in this ParticleHandler. 
     */
    void computeAllMasses(unsigned int indSpecies);

    /*!
     * \brief Computes the mass for all BaseParticle in this ParticleHandler.
     */
    void computeAllMasses();

    /*!
     *  \brief Returns the name of the handler, namely the string "ParticleHandler".
     */    
    std::string getName() const;

private:
    /*!
     * \brief A pointer to the largest BaseParticle (by interactionRadius) in this ParticleHandler
     * \todo TW: note that checkExtrema gets called if a particle gets created 
     * and its Species and Radius gets set, even if it's not yet included in the 
     * particleHandler! This is necessary to check a not included particle for 
     * overlaps before inserting it into the handler. Not sure if this is a 
     * sensible structure; to be discussed. 
     */
    BaseParticle* largestParticle_;

    /*!
     * \brief A pointer to the smallest BaseParticle (by interactionRadius) in this ParticleHandler
     */
    BaseParticle* smallestParticle_;
};

#endif

