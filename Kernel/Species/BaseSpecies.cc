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

#include "BaseSpecies.h"
#include "SpeciesHandler.h"
#include "DPMBase.h"
#include "Interactions/BaseInteraction.h"
#include<cmath>

class BaseParticle;
class BaseInteractable;

BaseSpecies::BaseSpecies()
{
    handler_ = 0;
    density_=0;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseSpecies::BaseSpecies() finished"<<std::endl;
#endif
}

BaseSpecies::BaseSpecies(const BaseSpecies &p)
        : BaseObject(p)
{
    handler_ = p.handler_;
    density_=p.density_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseSpecies::BaseSpecies(const BaseSpecies &p) finished"<<std::endl;
#endif
}

BaseSpecies::~BaseSpecies()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseSpecies::~BaseSpecies() finished"<<std::endl;
#endif   
}

void BaseSpecies::clear()
{
    std::cout << "BaseSpecies::clear(), this function shouldn't be called" << std::endl;
}

void BaseSpecies::setHandler(SpeciesHandler* handler)
{
    handler_ = handler;
}

SpeciesHandler* BaseSpecies::getHandler() const
{
    return handler_;
}

///BaseSpecies print function, which accepts an os std::stringstream as input. It prints human readable BaseSpecies information to the std::stringstream
void BaseSpecies::write(std::ostream& os) const
{
    BaseObject::write(os);
//    if (density_!=0)
        os << " density " << density_;
//    else
//        os << " mixedSpecies";
}

void BaseSpecies::read(std::istream& is)
{
    BaseObject::read(is);
    std::string dummy;
    is >> dummy;
//    if (dummy.compare("mixedSpecies"))
        is >> density_;
//    else
//        density_ = 0;
}

std::string BaseSpecies::getBaseName() const
{
    return "Base";
}

std::string BaseSpecies::getName() const
{
    return getBaseName() + "Species";
}

Mdouble BaseSpecies::getInteractionDistance() const
{
    return 0.0;
}

///\todo recalculate masses when setting dim_particle or rho
///Allows the density to be changed
void BaseSpecies::setDensity(Mdouble density)
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
Mdouble BaseSpecies::getDensity() const
{
    return density_;
}

Mdouble BaseSpecies::average(Mdouble a, Mdouble b)
{
    return (a + b) != 0.0 ? (2. * (a * b) / (a + b)) : 0;
}

Mdouble BaseSpecies::getMassFromRadius(const Mdouble radius)
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

///create values for mixed species
void BaseSpecies::mix(BaseSpecies* const S UNUSED, BaseSpecies* const T UNUSED)
{
    density_ = 0;
}

