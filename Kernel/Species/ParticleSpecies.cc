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
