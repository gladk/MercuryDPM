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

#include "LinearViscoelasticNormalSpecies.h"
#include "Interactions/BaseInteraction.h"
#include<cmath>
#include <Interactions/NormalForceInteractions/LinearViscoelasticInteraction.h>

class BaseParticle;
class BaseInteractable;

LinearViscoelasticNormalSpecies::LinearViscoelasticNormalSpecies()
        : BaseSpecies()
{
    stiffness_ = 0;
    dissipation_ = 0;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LinearViscoelasticNormalSpecies::LinearViscoelasticNormalSpecies() finished"<<std::endl;
#endif
}

LinearViscoelasticNormalSpecies::LinearViscoelasticNormalSpecies(const LinearViscoelasticNormalSpecies &p)
        : BaseSpecies(p)
{
    stiffness_ = p.stiffness_;
    dissipation_ = p.dissipation_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LinearViscoelasticNormalSpecies::LinearViscoelasticNormalSpecies(const LinearViscoelasticNormalSpecies &p) finished"<<std::endl;
#endif
}

LinearViscoelasticNormalSpecies::~LinearViscoelasticNormalSpecies()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LinearViscoelasticNormalSpecies::~LinearViscoelasticNormalSpecies() finished"<<std::endl;
#endif
}

void LinearViscoelasticNormalSpecies::clear()
{
    std::cout << "LinearViscoelasticNormalSpecies::clear(), this function shouldn't be called" << std::endl;
}

///LinearViscoelasticNormalSpecies copy method. It calls to copy constructor of this LinearViscoelasticNormalSpecies, useful for polymorphism
LinearViscoelasticNormalSpecies* LinearViscoelasticNormalSpecies::copy() const
{
    return new LinearViscoelasticNormalSpecies(*this);
}

///LinearViscoelasticNormalSpecies print function, which accepts an os std::stringstream as input. It prints human readable LinearViscoelasticNormalSpecies information to the std::stringstream
void LinearViscoelasticNormalSpecies::write(std::ostream& os) const
{
    os << " stiffness " << stiffness_
            << " dissipation " << dissipation_;
}

void LinearViscoelasticNormalSpecies::read(std::istream& is)
{
    std::string dummy;
    is >> dummy >> stiffness_
            >> dummy >> dissipation_;
}

std::string LinearViscoelasticNormalSpecies::getBaseName() const
{
    return "LinearViscoelastic";
}

///Allows the spring constant to be changed
void LinearViscoelasticNormalSpecies::setStiffness(Mdouble new_k)
{
    if (new_k >= 0)
        stiffness_ = new_k;
    else
    {
        std::cerr << "Error in set_k" << std::endl;
        exit(-1);
    }
}

///Allows the spring constant to be accessed
Mdouble LinearViscoelasticNormalSpecies::getStiffness() const
{
    return stiffness_;
}

///Allows the spring and dissipation constants to be changed simultaneously
void LinearViscoelasticNormalSpecies::setNormalStiffnessAndDissipation(helpers::KAndDisp new_)
{
    setStiffness(new_.k);
    setDissipation(new_.disp);
}

///Allows the normal dissipation to be changed
void LinearViscoelasticNormalSpecies::setDissipation(Mdouble dissipation)
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
Mdouble LinearViscoelasticNormalSpecies::getDissipation() const
{
    return dissipation_;
}

///Calculates collision time for two copies of a particle of given disp, k, mass
Mdouble LinearViscoelasticNormalSpecies::getCollisionTime(Mdouble mass)
{
    if (mass <= 0)
    {
        std::cerr << "Error in getCollisionTime(" << mass << ") mass is not set or has an unexpected value, (getCollisionTime(" << mass << "))" << std::endl;
        exit(-1);
    }
    if (stiffness_ <= 0)
    {
        std::cerr << "Error in getCollisionTime(" << mass << ") stiffness=" << stiffness_ << " is not set or has an unexpected value, (getCollisionTime(" << mass << "), with stiffness=" << stiffness_ << ")" << std::endl;
        exit(-1);
    }
    if (dissipation_ < 0)
    {
        std::cerr << "Error in getCollisionTime(" << mass << ") dissipation=" << dissipation_ << " is not set or has an unexpected value, (getCollisionTime(" << mass << "), with dissipation=" << dissipation_ << ")" << std::endl;
        exit(-1);
    }
    Mdouble tosqrt = stiffness_ / (.5 * mass) - mathsFunc::square(dissipation_ / mass);
    if (tosqrt <= 0)
    {
        std::cerr << "Error in getCollisionTime(" << mass << ") values for mass, stiffness and dissipation would lead to an overdamped system, (getCollisionTime(" << mass << "), with stiffness=" << stiffness_ << " and dissipation=" << dissipation_ << ")" << std::endl;
        exit(-1);
    }
    return constants::pi / std::sqrt(tosqrt);
}

///Calculates restitution coefficient for two copies of given disp, k, mass
Mdouble LinearViscoelasticNormalSpecies::getRestitutionCoefficient(Mdouble mass)
{
    return std::exp(-dissipation_ / mass * getCollisionTime(mass));
}

///Calculates the maximum velocity allowed for a collision of two copies of P (for higher velocities particles could pass through each other)
Mdouble LinearViscoelasticNormalSpecies::getMaximumVelocity(Mdouble radius, Mdouble mass)
{
    return radius * std::sqrt(stiffness_ / (.5 * mass));
}

bool LinearViscoelasticNormalSpecies::getUseAngularDOFs() const
{
    return false;
}

///Sets k, disp such that it matches a given tc and eps for a collision of two copies of P
void LinearViscoelasticNormalSpecies::setStiffnessAndRestitutionCoefficient(Mdouble k_, Mdouble eps, Mdouble mass)
{
    stiffness_ = k_;
    dissipation_ = -std::sqrt(2.0 * mass * stiffness_ / (constants::sqr_pi + mathsFunc::square(log(eps)))) * log(eps);
}

///Sets k, disp such that it matches a given tc and eps for a collision of two copies of equal mass m
void LinearViscoelasticNormalSpecies::setCollisionTimeAndRestitutionCoefficient(Mdouble tc, Mdouble eps, Mdouble mass)
{
    dissipation_ = -mass / tc * std::log(eps);
    stiffness_ = .5 * mass * (mathsFunc::square(constants::pi / tc) + mathsFunc::square(dissipation_ / mass));
}

///Set k, disp such that is matches a given tc and eps for a collision of two different masses.
///Recall the resitution constant is a function of k, disp and the mass of each particle in the collision
/// See also setCollisionTimeAndRestitutionCoefficient(Mdouble tc, Mdouble eps, Mdouble mass)
void LinearViscoelasticNormalSpecies::setCollisionTimeAndRestitutionCoefficient(Mdouble collisionTime, Mdouble restitutionCoefficient, Mdouble mass1, Mdouble mass2)
{
    Mdouble reduced_mass = mass1 * mass2 / (mass1 + mass2);
    setCollisionTimeAndRestitutionCoefficient(collisionTime, restitutionCoefficient, 2.0 * reduced_mass);
}

///create values for mixed species
void LinearViscoelasticNormalSpecies::mix(LinearViscoelasticNormalSpecies* const S, LinearViscoelasticNormalSpecies* const T)
{
    BaseSpecies::mix(S, T);
    stiffness_ = average(S->getStiffness(), T->getStiffness());
    dissipation_ = average(S->getDissipation(), T->getDissipation());
}

BaseInteraction* LinearViscoelasticNormalSpecies::getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
{
    return new LinearViscoelasticInteraction(P, I, timeStamp);
}