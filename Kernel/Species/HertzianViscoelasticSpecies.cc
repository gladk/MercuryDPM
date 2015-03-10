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

#include "HertzianViscoelasticSpecies.h"
#include "Interactions/BaseInteraction.h"
#include<cmath>
#include <Interactions/HertzianViscoelasticInteraction.h>

class BaseParticle;
class BaseInteractable;

HertzianViscoelasticSpecies::HertzianViscoelasticSpecies()
        : BaseSpecies()
{
    elasticModulus_ = 0;
    dissipation_ = 0;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"HertzianViscoelasticSpecies::HertzianViscoelasticSpecies() finished"<<std::endl;
#endif
}

HertzianViscoelasticSpecies::HertzianViscoelasticSpecies(const HertzianViscoelasticSpecies &p)
        : BaseSpecies(p)
{
    elasticModulus_ = p.elasticModulus_;
    dissipation_ = p.dissipation_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"HertzianViscoelasticSpecies::HertzianViscoelasticSpecies(const HertzianViscoelasticSpecies &p) finished"<<std::endl;
#endif
}

HertzianViscoelasticSpecies::~HertzianViscoelasticSpecies()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"HertzianViscoelasticSpecies::~HertzianViscoelasticSpecies() finished"<<std::endl;
#endif
}

void HertzianViscoelasticSpecies::clear()
{
    std::cout << "HertzianViscoelasticSpecies::clear(), this function shouldn't be called" << std::endl;
}

///HertzianViscoelasticSpecies copy method. It calls to copy constructor of this HertzianViscoelasticSpecies, useful for polymorphism
HertzianViscoelasticSpecies* HertzianViscoelasticSpecies::copy() const
{
    return new HertzianViscoelasticSpecies(*this);
}

///HertzianViscoelasticSpecies print function, which accepts an os std::stringstream as input. It prints human readable HertzianViscoelasticSpecies information to the std::stringstream
void HertzianViscoelasticSpecies::write(std::ostream& os) const
        {
    BaseSpecies::write(os);
    os << " stiffness " << elasticModulus_
            << " dissipation " << dissipation_;
}

void HertzianViscoelasticSpecies::read(std::istream& is)
{
    BaseSpecies::read(is);
    std::string dummy;
    is >> dummy >> elasticModulus_
            >> dummy >> dissipation_;
}

std::string HertzianViscoelasticSpecies::getBaseName() const
{
    return "HertzianViscoelastic";
}

///Allows the spring constant to be changed
void HertzianViscoelasticSpecies::setElasticModulus(Mdouble elasticModulus)
{
    if (elasticModulus >= 0)
        elasticModulus_ = elasticModulus;
    else
    {
        std::cerr << "Error in setElasticModulus" << std::endl;
        exit(-1);
    }
}

///Allows the spring constant to be accessed
Mdouble HertzianViscoelasticSpecies::getElasticModulus() const
{
    return elasticModulus_;
}

///Allows the normal dissipation to be changed
void HertzianViscoelasticSpecies::setDissipation(Mdouble dissipation)
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
Mdouble HertzianViscoelasticSpecies::getDissipation() const
{
    return dissipation_;
}

///Calculates collision time for two copies of a particle of given disp, k, mass
Mdouble HertzianViscoelasticSpecies::getCollisionTime(Mdouble mass)
{
    if (mass <= 0)
    {
        std::cerr << "Error in getCollisionTime(Mdouble mass) mass is not set or has an unexpected value, (getCollisionTime(" << mass << "))" << std::endl;
        exit(-1);
    }
    if (elasticModulus_ <= 0)
    {
        std::cerr << "Error in getCollisionTime(Mdouble mass) stiffness is not set or has an unexpected value, (getCollisionTime(" << mass << "), with stiffness=" << elasticModulus_ << ")" << std::endl;
        exit(-1);
    }
    if (dissipation_ < 0)
    {
        std::cerr << "Error in getCollisionTime(Mdouble mass) dissipation is not set or has an unexpected value, (getCollisionTime(" << mass << "), with dissipation=" << dissipation_ << ")" << std::endl;
        exit(-1);
    }
    Mdouble tosqrt = elasticModulus_ / (.5 * mass) - mathsFunc::square(dissipation_ / mass);
    if (tosqrt <= 0)
    {
        std::cerr << "Error in getCollisionTime(Mdouble mass) values for mass, stiffness and dissipation would lead to an overdamped system, (getCollisionTime(" << mass << "), with stiffness=" << elasticModulus_ << " and dissipation=" << dissipation_ << ")" << std::endl;
        exit(-1);
    }
    return constants::pi / std::sqrt(tosqrt);
}

///Calculates restitution coefficient for two copies of given disp, k, mass
Mdouble HertzianViscoelasticSpecies::getRestitutionCoefficient(Mdouble mass)
{
    return std::exp(-dissipation_ / mass * getCollisionTime(mass));
}

///Calculates the maximum velocity allowed for a collision of two copies of P (for higher velocities particles could pass through each other)
Mdouble HertzianViscoelasticSpecies::getMaximumVelocity(Mdouble radius, Mdouble mass)
{
    return radius * std::sqrt(elasticModulus_ / (.5 * mass));
}

bool HertzianViscoelasticSpecies::getUseAngularDOFs() const
{
    return false;
}

///Sets k, disp such that it matches a given tc and eps for a collision of two copies of P
void HertzianViscoelasticSpecies::setStiffnessAndRestitutionCoefficient(Mdouble k_, Mdouble eps, Mdouble mass)
{
    elasticModulus_ = k_;
    dissipation_ = -std::sqrt(2.0 * mass * elasticModulus_ / (constants::sqr_pi + mathsFunc::square(log(eps)))) * log(eps);
}

///Sets k, disp such that it matches a given tc and eps for a collision of two copies of equal mass m
void HertzianViscoelasticSpecies::setCollisionTimeAndRestitutionCoefficient(Mdouble tc, Mdouble eps, Mdouble mass)
{
    dissipation_ = -mass / tc * std::log(eps);
    elasticModulus_ = .5 * mass * (mathsFunc::square(constants::pi / tc) + mathsFunc::square(dissipation_ / mass));
}

///Set k, disp such that is matches a given tc and eps for a collision of two different masses.
///Recall the resitution constant is a function of k, disp and the mass of each particle in the collision
/// See also setCollisionTimeAndRestitutionCoefficient(Mdouble tc, Mdouble eps, Mdouble mass)
void HertzianViscoelasticSpecies::setCollisionTimeAndRestitutionCoefficient(Mdouble collisionTime, Mdouble restitutionCoefficient, Mdouble mass1, Mdouble mass2)
{
    Mdouble reduced_mass = mass1 * mass2 / (mass1 + mass2);
    setCollisionTimeAndRestitutionCoefficient(collisionTime, restitutionCoefficient, 2.0 * reduced_mass);
}

///create values for mixed species
void HertzianViscoelasticSpecies::mix(HertzianViscoelasticSpecies* const S, HertzianViscoelasticSpecies* const T)
{
    BaseSpecies::mix(S, T);
    elasticModulus_ = average(S->getElasticModulus(), T->getElasticModulus());
    dissipation_ = average(S->getDissipation(), T->getDissipation());
}

BaseInteraction* HertzianViscoelasticSpecies::getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
{
    return new HertzianViscoelasticInteraction(P, I, timeStamp);
}
