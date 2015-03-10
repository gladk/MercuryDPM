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

#include "LinearPlasticViscoelasticSpecies.h"
#include "Interactions/LinearPlasticViscoelasticInteraction.h"

class BaseParticle;
class BaseInteractable;

LinearPlasticViscoelasticSpecies::LinearPlasticViscoelasticSpecies()
    : BaseSpecies()
{
    loadingStiffness_ = 0.0;
    unloadingStiffnessMax_ = 0.0;
    cohesionStiffness_ = 0.0;
    penetrationDepthMax_ = 0.0;
    dissipation_ = 0.0;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LinearPlasticViscoelasticSpecies::LinearPlasticViscoelasticSpecies() finished"<<std::endl;
#endif
}

LinearPlasticViscoelasticSpecies::LinearPlasticViscoelasticSpecies(const LinearPlasticViscoelasticSpecies &p)
    : BaseSpecies(p)
{
    loadingStiffness_ = p.loadingStiffness_;
    unloadingStiffnessMax_ = p.unloadingStiffnessMax_;
    cohesionStiffness_ = p.cohesionStiffness_;
    penetrationDepthMax_ = p.penetrationDepthMax_;
    dissipation_ = p.dissipation_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LinearPlasticViscoelasticSpecies::LinearPlasticViscoelasticSpecies(const LinearPlasticViscoelasticSpecies &p) finished"<<std::endl;
#endif
}

LinearPlasticViscoelasticSpecies::~LinearPlasticViscoelasticSpecies()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LinearPlasticViscoelasticSpecies::~LinearPlasticViscoelasticSpecies() finished"<<std::endl;
#endif   
}

void LinearPlasticViscoelasticSpecies::clear()
{
    std::cout << "LinearPlasticViscoelasticSpecies::clear(), this function shouldn't be called" << std::endl;
}

///LinearPlasticViscoelasticSpecies copy method. It calls to copy constructor of this LinearPlasticViscoelasticSpecies, useful for polymorphism
LinearPlasticViscoelasticSpecies* LinearPlasticViscoelasticSpecies::copy() const
{
    return new LinearPlasticViscoelasticSpecies(*this);
}

///LinearPlasticViscoelasticSpecies print function, which accepts an os std::stringstream as input. It prints human readable LinearPlasticViscoelasticSpecies information to the std::stringstream
void LinearPlasticViscoelasticSpecies::write(std::ostream& os) const
{
    BaseSpecies::write(os);
    os  << " loadingStiffness " << loadingStiffness_;
    os  << " maxUnloadingStiffness " << unloadingStiffnessMax_;
    os  << " cohesionStiffness " << cohesionStiffness_;
    os  << " maxPenetration " << penetrationDepthMax_;
    os  << " dissipation " << dissipation_;
}

void LinearPlasticViscoelasticSpecies::read(std::istream& is)
{
    BaseSpecies::read(is);
    std::string dummy;
    is >> dummy >> loadingStiffness_;
    is >> dummy >> unloadingStiffnessMax_;
    is >> dummy >> cohesionStiffness_;
    is >> dummy >> penetrationDepthMax_;
    is >> dummy >> dissipation_;
}

std::string LinearPlasticViscoelasticSpecies::getBaseName() const
{
    return "LinearPlasticViscoelastic";
}

bool LinearPlasticViscoelasticSpecies::getUseAngularDOFs() const
{
    return false;
}

///create values for mixed species
void LinearPlasticViscoelasticSpecies::mix(LinearPlasticViscoelasticSpecies* const S, LinearPlasticViscoelasticSpecies* const T)
{
    BaseSpecies::mix(S, T);
    loadingStiffness_ = average(S->getLoadingStiffness(), T->getLoadingStiffness());
    unloadingStiffnessMax_ = average(S->getUnloadingStiffnessMax(), T->getUnloadingStiffnessMax());
    cohesionStiffness_ = average(S->getCohesionStiffness(), T->getCohesionStiffness());
    penetrationDepthMax_ = average(S->getPenetrationDepthMax(), T->getPenetrationDepthMax());
    dissipation_ = average(S->getDissipation(), T->getDissipation());
}

BaseInteraction* LinearPlasticViscoelasticSpecies::getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
{
    return new LinearPlasticViscoelasticInteraction(P, I, timeStamp);
}


///Acccess functions for the plastic model
void LinearPlasticViscoelasticSpecies::setPlasticParameters(Mdouble loadingStiffness, Mdouble maxUnloadingStiffness, Mdouble cohesionStiffness, Mdouble maxPenetration)
{
    if (loadingStiffness <= 0 || maxUnloadingStiffness < loadingStiffness || cohesionStiffness < 0 || maxPenetration < 0 || maxPenetration > 1)
    {
        std::cerr << "Error: arguments of setPlasticParameters do not make sense" << std::endl;
        exit(-1);
    }
    setLoadingStiffness(loadingStiffness);
    setUnloadingStiffnessMax(maxUnloadingStiffness);
    setCohesionStiffness(cohesionStiffness);
    setPenetrationDepthMax(maxPenetration);
}

Mdouble LinearPlasticViscoelasticSpecies::getLoadingStiffness() const
{
    return loadingStiffness_;
}
Mdouble LinearPlasticViscoelasticSpecies::getUnloadingStiffnessMax() const
{
    return unloadingStiffnessMax_;
}
Mdouble LinearPlasticViscoelasticSpecies::getCohesionStiffness() const
{
    return cohesionStiffness_;
}
Mdouble LinearPlasticViscoelasticSpecies::getPenetrationDepthMax() const
{
    return penetrationDepthMax_;
}
void LinearPlasticViscoelasticSpecies::setLoadingStiffness(Mdouble loadingStiffness)
{
    loadingStiffness_ = loadingStiffness;
}
void LinearPlasticViscoelasticSpecies::setUnloadingStiffnessMax(Mdouble maxUnloadingStiffness)
{
    unloadingStiffnessMax_ = maxUnloadingStiffness;
}
void LinearPlasticViscoelasticSpecies::setCohesionStiffness(Mdouble cohesionStiffness)
{
    cohesionStiffness_ = cohesionStiffness;
}
void LinearPlasticViscoelasticSpecies::setPenetrationDepthMax(Mdouble maxPenetration)
{
    penetrationDepthMax_ = maxPenetration;
}
///Calculates collision time for stiffest spring constant, divides by 50
Mdouble LinearPlasticViscoelasticSpecies::computePlasticTimeStep(Mdouble mass)
{
    return 0.02 * constants::pi / std::sqrt(unloadingStiffnessMax_ / (.5 * mass) - mathsFunc::square(dissipation_ /mass));
}

///Allows the normal dissipation to be changed
void LinearPlasticViscoelasticSpecies::setDissipation(Mdouble dissipation)
{
    if (dissipation >= 0)
    {
        dissipation_ = dissipation;
    }
    else
    {
        std::cerr << "Error in setDissipation(" << dissipation << ")" << std::endl;
        exit(-1);
    }
}

///Allows the normal dissipation to be accessed
Mdouble LinearPlasticViscoelasticSpecies::getDissipation() const
{
    return dissipation_;
}
