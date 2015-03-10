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

#include "ParticleSpecies.h"
#include "Interactions/BaseInteraction.h"
#include "DPMBase.h"
#include<cmath>

class BaseParticle;
class BaseInteractable;

ParticleSpecies::ParticleSpecies()
        : BaseSpecies()
{
    density_ = 0.0;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"ParticleSpecies::ParticleSpecies() finished"<<std::endl;
#endif
}

ParticleSpecies::ParticleSpecies(const ParticleSpecies &p)
        : BaseSpecies(p)
{
    density_=p.density_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"ParticleSpecies::ParticleSpecies(const ParticleSpecies &p) finished"<<std::endl;
#endif
}

ParticleSpecies::~ParticleSpecies()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"ParticleSpecies::~ParticleSpecies() finished"<<std::endl;
#endif
}

void ParticleSpecies::clear()
{
    std::cout << "ParticleSpecies::clear(), this function shouldn't be called" << std::endl;
}

/////ParticleSpecies copy method. It calls to copy constructor of this ParticleSpecies, useful for polymorphism
//ParticleSpecies* ParticleSpecies::copy() const
//{
//    return new ParticleSpecies(*this);
//}

///ParticleSpecies print function, which accepts an os std::stringstream as input. It prints human readable ParticleSpecies information to the std::stringstream
void ParticleSpecies::write(std::ostream& os) const
{
    //note we inherit from BaseObject, not BaseParticle
    BaseObject::write(os);
    os << " density " << density_;
}

void ParticleSpecies::read(std::istream& is)
{
    BaseSpecies::read(is);
    std::string dummy;
    is >> dummy >> density_;
}

std::string ParticleSpecies::getBaseName() const
{
    return "Particle";
}

///\todo recalculate masses when setting dim_particle or rho
///Allows the density to be changed
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
        std::cerr << "Error in setDensity(" << density << ")" << std::endl;
        exit(-1);
    }
}

///Allows the density to be accessed
Mdouble ParticleSpecies::getDensity() const
{
    return density_;
}

BaseInteraction* ParticleSpecies::getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
{
    return new BaseInteraction(P, I, timeStamp);
}

Mdouble ParticleSpecies::getMassFromRadius(const Mdouble radius)
{
    int particleDimensions = getHandler()->getDPMBase()->getParticleDimensions();
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
        std::cerr << "In Species::MassFromRadius the dimension of the particle is set to " << particleDimensions << std::endl;
        exit(-1);
    }
}
