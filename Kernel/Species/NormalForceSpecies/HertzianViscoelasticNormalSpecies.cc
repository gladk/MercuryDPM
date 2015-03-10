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

#include "HertzianViscoelasticNormalSpecies.h"
#include "Interactions/BaseInteraction.h"
#include<cmath>
#include <Interactions/NormalForceInteractions/HertzianViscoelasticInteraction.h>

class BaseParticle;
class BaseInteractable;

HertzianViscoelasticNormalSpecies::HertzianViscoelasticNormalSpecies()
        : BaseSpecies()
{
    elasticModulus_ = 0;
    dissipation_ = 0;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"HertzianViscoelasticNormalSpecies::HertzianViscoelasticNormalSpecies() finished"<<std::endl;
#endif
}

HertzianViscoelasticNormalSpecies::HertzianViscoelasticNormalSpecies(const HertzianViscoelasticNormalSpecies &p)
        : BaseSpecies(p)
{
    elasticModulus_ = p.elasticModulus_;
    dissipation_ = p.dissipation_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"HertzianViscoelasticNormalSpecies::HertzianViscoelasticNormalSpecies(const HertzianViscoelasticNormalSpecies &p) finished"<<std::endl;
#endif
}

HertzianViscoelasticNormalSpecies::~HertzianViscoelasticNormalSpecies()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"HertzianViscoelasticNormalSpecies::~HertzianViscoelasticNormalSpecies() finished"<<std::endl;
#endif
}

void HertzianViscoelasticNormalSpecies::clear()
{
    std::cout << "HertzianViscoelasticNormalSpecies::clear(), this function shouldn't be called" << std::endl;
}

///HertzianViscoelasticNormalSpecies copy method. It calls to copy constructor of this HertzianViscoelasticNormalSpecies, useful for polymorphism
HertzianViscoelasticNormalSpecies* HertzianViscoelasticNormalSpecies::copy() const
{
    return new HertzianViscoelasticNormalSpecies(*this);
}

///HertzianViscoelasticNormalSpecies print function, which accepts an os std::stringstream as input. It prints human readable HertzianViscoelasticNormalSpecies information to the std::stringstream
void HertzianViscoelasticNormalSpecies::write(std::ostream& os) const
        {
    os << " stiffness " << elasticModulus_
            << " dissipation " << dissipation_;
}

void HertzianViscoelasticNormalSpecies::read(std::istream& is)
{
    std::string dummy;
    is >> dummy >> elasticModulus_
            >> dummy >> dissipation_;
}

std::string HertzianViscoelasticNormalSpecies::getBaseName() const
{
    return "HertzianViscoelastic";
}

///Allows the spring constant to be changed
void HertzianViscoelasticNormalSpecies::setElasticModulus(Mdouble elasticModulus)
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
Mdouble HertzianViscoelasticNormalSpecies::getElasticModulus() const
{
    return elasticModulus_;
}

///Allows the normal dissipation to be changed
void HertzianViscoelasticNormalSpecies::setDissipation(Mdouble dissipation)
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
Mdouble HertzianViscoelasticNormalSpecies::getDissipation() const
{
    return dissipation_;
}

///Calculates collision time for two copies of a particle of given disp, k, mass
Mdouble HertzianViscoelasticNormalSpecies::getCollisionTime(Mdouble mass)
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
Mdouble HertzianViscoelasticNormalSpecies::getRestitutionCoefficient(Mdouble mass)
{
    return std::exp(-dissipation_ / mass * getCollisionTime(mass));
}

///Calculates the maximum velocity allowed for a collision of two copies of P (for higher velocities particles could pass through each other)
Mdouble HertzianViscoelasticNormalSpecies::getMaximumVelocity(Mdouble radius, Mdouble mass)
{
    return radius * std::sqrt(elasticModulus_ / (.5 * mass));
}

bool HertzianViscoelasticNormalSpecies::getUseAngularDOFs() const
{
    return false;
}

///Sets k, disp such that it matches a given tc and eps for a collision of two copies of P
void HertzianViscoelasticNormalSpecies::setStiffnessAndRestitutionCoefficient(Mdouble k_, Mdouble eps, Mdouble mass)
{
    elasticModulus_ = k_;
    dissipation_ = -std::sqrt(2.0 * mass * elasticModulus_ / (constants::sqr_pi + mathsFunc::square(log(eps)))) * log(eps);
}

///Sets k, disp such that it matches a given tc and eps for a collision of two copies of equal mass m
void HertzianViscoelasticNormalSpecies::setCollisionTimeAndRestitutionCoefficient(Mdouble tc, Mdouble eps, Mdouble mass)
{
    dissipation_ = -mass / tc * std::log(eps);
    elasticModulus_ = .5 * mass * (mathsFunc::square(constants::pi / tc) + mathsFunc::square(dissipation_ / mass));
}

///Set k, disp such that is matches a given tc and eps for a collision of two different masses.
///Recall the resitution constant is a function of k, disp and the mass of each particle in the collision
/// See also setCollisionTimeAndRestitutionCoefficient(Mdouble tc, Mdouble eps, Mdouble mass)
void HertzianViscoelasticNormalSpecies::setCollisionTimeAndRestitutionCoefficient(Mdouble collisionTime, Mdouble restitutionCoefficient, Mdouble mass1, Mdouble mass2)
{
    Mdouble reduced_mass = mass1 * mass2 / (mass1 + mass2);
    setCollisionTimeAndRestitutionCoefficient(collisionTime, restitutionCoefficient, 2.0 * reduced_mass);
}

///create values for mixed species
void HertzianViscoelasticNormalSpecies::mix(HertzianViscoelasticNormalSpecies* const S, HertzianViscoelasticNormalSpecies* const T)
{
    BaseSpecies::mix(S, T);
    elasticModulus_ = average(S->getElasticModulus(), T->getElasticModulus());
    dissipation_ = average(S->getDissipation(), T->getDissipation());
}

BaseInteraction* HertzianViscoelasticNormalSpecies::getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
{
    return new HertzianViscoelasticInteraction(P, I, timeStamp);
}
