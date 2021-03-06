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

#include<cmath>
#include <Logger.h>
#include "ParticleSpecies.h"
#include "Interactions/BaseInteraction.h"
#include "DPMBase.h"


class BaseParticle;
class BaseInteractable;

ParticleSpecies::ParticleSpecies()
{
    density_ = 0.0;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"ParticleSpecies::ParticleSpecies() finished"<<std::endl;
#endif
}

/*!
 * \param[in] p the species that is copied
 */
ParticleSpecies::ParticleSpecies(const ParticleSpecies &p)
{
    density_=p.density_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"ParticleSpecies::ParticleSpecies(const ParticleSpecies &p) finished"<<std::endl;
#endif
}

ParticleSpecies::~ParticleSpecies()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout<<"ParticleSpecies::~ParticleSpecies() finished"<<std::endl;
#endif
}

/*!
 * \param[out] os output stream (typically the restart file)
 */
void ParticleSpecies::write(std::ostream& os) const
{
    //note we inherit from BaseObject, not BaseParticle
    BaseObject::write(os);
    os << " density " << density_;
}

/*!
 * \param[in] is input stream (typically the restart file)
 */
void ParticleSpecies::read(std::istream& is)
{
    BaseSpecies::read(is);
    std::string dummy;
    is >> dummy >> density_;
}

/*!
 * \return a string containing the name of the species (minus the word "Species")
 */
std::string ParticleSpecies::getBaseName() const
{
    return "Particle";
}

/*!
 * \param[in] density the particle density
 */
void ParticleSpecies::setDensity(Mdouble density)
{
    if (density >= 0)
    {
        density_ = density;
        if (getHandler())
            getHandler()->getDPMBase()->particleHandler.computeAllMasses(getIndex());
    }
    else
    {
        logger(ERROR, "[ParticleSpecies::setDensity()] Error in the density to be set: % ", density);
        exit(-1);
    }
}

/*!
 * \return the particle density
 */
Mdouble ParticleSpecies::getDensity() const
{
    return density_;
}

Mdouble ParticleSpecies::getMassFromRadius(const Mdouble radius)
{
    if (getHandler() == nullptr)
    {
        logger(ERROR, "[Species::MassFromRadius()] No handler has been set, therefore, I can't figure out the dimensions.");
        return 0;
    }
    
    unsigned int particleDimensions = getHandler()->getDPMBase()->getParticleDimensions();
    if (particleDimensions == 3)
    {
        return 4.0 / 3.0 * constants::pi * radius * radius * radius * getDensity();
    }
    else if (particleDimensions == 2)
    {
        return constants::pi * radius * radius * getDensity();
    }
    else if (particleDimensions == 1)
    {
        return 2.0 * radius * getDensity();
    }
    else
    {
       logger(ERROR, "[Species::MassFromRadius()] the dimension of the particle is wrongly set to %",particleDimensions);
       return 0.0;
    }
}

///Compute BaseParticle mass function, which required a reference to the Species vector. It computes the Particles mass, Inertia and the inverses.
/// this function is called, if BaseParticleHandler::addObject, SpeciesHandler::addObject, ParticleSpecies::setDensity, BaseParticle::setRadius or DPMBase::setParticleDimensions is called
void ParticleSpecies::computeMass(BaseParticle* p) const
{
    if (!p->isFixed())
    {
        switch (p->getParticleDimensions())
        {
            case 3:
            {
                p->mass_ = (4.0 / 3.0 * constants::pi * p->getRadius() * p->getRadius() * p->getRadius() * getDensity());
                p->invMass_ = 1.0 / (p->mass_);
                p->inertia_ = (.4 * p->getMass() * mathsFunc::square(p->getRadius()));
                p->invInertia_ = 1.0 / (p->inertia_);
                break;
            }
            case 2:
            {
                p->mass_ = (constants::pi * p->getRadius() * p->getRadius() * getDensity());
                p->invMass_ = 1.0 / (p->mass_);
                p->inertia_ = (.5 * p->getMass() * mathsFunc::square(p->getRadius()));
                p->invInertia_ = 1.0 / (p->inertia_);
                break;
            }
            case 1:
            {
                p->mass_ = (2.0 * p->getRadius() * getDensity());
                p->invMass_ = 1.0/p->mass_;
                p->inertia_ = 0.0;
                p->invInertia_ = std::numeric_limits<Mdouble>::quiet_NaN();
                break;
            }
            default:
            {
                logger(ERROR, "ParticleSpecies::computeMass()] the dimension of the particle is not set");
            }
        }
    }
}

