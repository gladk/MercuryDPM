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

#include <cmath>
#include "SlidingFrictionSpecies.h"
#include "Species/NormalForceSpecies/LinearViscoelasticNormalSpecies.h"
#include "Species/BaseSpecies.h"
#include "Species/NormalForceSpecies/LinearPlasticViscoelasticNormalSpecies.h"

class BaseParticle;
class BaseInteractable;

SlidingFrictionSpecies::SlidingFrictionSpecies()
        : BaseSpecies()
{
    slidingStiffness_ = 0;
    slidingDissipation_ = 0;
    slidingFrictionCoefficient_ = 0;
    slidingFrictionCoefficientStatic_ = 0;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"SlidingFrictionSpecies::SlidingFrictionSpecies() finished"<<std::endl;
#endif
}

SlidingFrictionSpecies::SlidingFrictionSpecies(const SlidingFrictionSpecies &s)
        : BaseSpecies(s)
{
    slidingStiffness_ = s.slidingStiffness_;
    slidingDissipation_ = s.slidingDissipation_;
    slidingFrictionCoefficient_ = s.slidingFrictionCoefficient_;
    slidingFrictionCoefficientStatic_ = s.slidingFrictionCoefficientStatic_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"SlidingFrictionSpecies::SlidingFrictionSpecies(const SlidingFrictionSpecies &p) finished"<<std::endl;
#endif
}

SlidingFrictionSpecies::~SlidingFrictionSpecies()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout<<"SlidingFrictionSpecies::~SlidingFrictionSpecies() finished"<<std::endl;
#endif   
}

void SlidingFrictionSpecies::clear()
{
    std::cout << "SlidingFrictionSpecies::clear(), this function shouldn't be called" << std::endl;
}

///SlidingFrictionSpecies copy method. It calls to copy constructor of this SlidingFrictionSpecies, useful for polymorphism
SlidingFrictionSpecies* SlidingFrictionSpecies::copy() const
{
    return new SlidingFrictionSpecies(*this);
}

///SlidingFrictionSpecies print function, which accepts an os std::stringstream as input. It prints human readable SlidingFrictionSpecies information to the std::stringstream
void SlidingFrictionSpecies::write(std::ostream& os) const
        {
    //BaseSpecies::write(os);
    os << " slidingStiffness " << slidingStiffness_;
    os << " slidingDissipation " << slidingDissipation_;
    os << " slidingFrictionCoefficient " << slidingFrictionCoefficient_;
    os << " slidingFrictionCoefficientStatic " << slidingFrictionCoefficientStatic_;
}

void SlidingFrictionSpecies::read(std::istream& is)
{
    //BaseSpecies::read(is);
    std::string dummy;
    is >> dummy >> slidingStiffness_;
    is >> dummy >> slidingDissipation_;
    is >> dummy >> slidingFrictionCoefficient_;
    is >> dummy >> slidingFrictionCoefficientStatic_;
}

std::string SlidingFrictionSpecies::getBaseName() const
{
    return "SlidingFriction";
}

///Allows the spring constant to be changed
void SlidingFrictionSpecies::setSlidingStiffness(Mdouble new_kt)
{
    if (new_kt >= 0)
    {
        slidingStiffness_ = new_kt;
    }
    else
    {
        std::cerr << "Error in setSlidingStiffness" << std::endl;
        exit(-1);
    }
}

///Allows the spring constant to be accessed
Mdouble SlidingFrictionSpecies::getSlidingStiffness() const
{
    return slidingStiffness_;
}

///Allows the tangential viscosity to be changed
void SlidingFrictionSpecies::setSlidingDissipation(Mdouble new_dispt)
{
    if (new_dispt >= 0)
        slidingDissipation_ = new_dispt;
    else
    {
        std::cerr << "Error in setSlidingDissipation" << std::endl;
        exit(-1);
    }
}
///Allows the tangential viscosity to be accessed
Mdouble SlidingFrictionSpecies::getSlidingDissipation() const
{
    return slidingDissipation_;
}

///Allows the (dynamic) Coulomb friction coefficient to be changed; also sets mu_s by default
//mu has to be set to allow tangential forces (sets dispt=disp as default)
void SlidingFrictionSpecies::setSlidingFrictionCoefficient(Mdouble new_mu)
{
    if (new_mu >= 0)
    {
        slidingFrictionCoefficient_ = new_mu;
        slidingFrictionCoefficientStatic_ = slidingFrictionCoefficient_;
    }
    else
    {
        std::cerr << "Error in setSlidingFrictionCoefficient" << std::endl;
        exit(-1);
    }
}

///Allows the (dynamic) Coulomb friction coefficient to be accessed
Mdouble SlidingFrictionSpecies::getSlidingFrictionCoefficient() const
{
    return slidingFrictionCoefficient_;
}

///Allows the static Coulomb friction coefficient to be changed; also sets mu_s by default
void SlidingFrictionSpecies::setSlidingFrictionCoefficientStatic(Mdouble new_mu)
{
    if (new_mu >= 0)
    {
        slidingFrictionCoefficientStatic_ = new_mu;
    }
    else
    {
        std::cerr << "Error in setSlidingFrictionCoefficientStatic" << std::endl;
        exit(-1);
    }
}

///Allows the static Coulomb friction coefficient to be accessed
Mdouble SlidingFrictionSpecies::getSlidingFrictionCoefficientStatic() const
{
    return slidingFrictionCoefficientStatic_;
}

BaseInteraction* SlidingFrictionSpecies::getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
{
    return new SlidingFrictionInteraction(P, I, timeStamp);
}

bool SlidingFrictionSpecies::getUseAngularDOFs() const
{
    return true;
}

///create values for mixed species
void SlidingFrictionSpecies::mix(SlidingFrictionSpecies* const SFrictional, SlidingFrictionSpecies* const TFrictional)
{
    slidingStiffness_ = average(SFrictional->getSlidingStiffness(), TFrictional->getSlidingStiffness());
    slidingDissipation_ = average(SFrictional->getSlidingDissipation(), TFrictional->getSlidingDissipation());
    slidingFrictionCoefficient_ = average(SFrictional->getSlidingFrictionCoefficient(), TFrictional->getSlidingFrictionCoefficient());
    slidingFrictionCoefficientStatic_ = average(SFrictional->getSlidingFrictionCoefficientStatic(), TFrictional->getSlidingFrictionCoefficientStatic());
}

///Sets k, disp, kt, dispt such that it matches a given tc and eps for a collision of two particles of masses m0,m1
void SlidingFrictionSpecies::setCollisionTimeAndNormalAndTangentialRestitutionCoefficient(Mdouble tc, Mdouble eps, Mdouble beta, Mdouble mass)
{
    Mdouble stiffness;
    //the dynamic cast is needed to check if the normal force species is LinearViscoelasticSpecies; otherwise this function cannot be used
    LinearViscoelasticNormalSpecies* species = dynamic_cast<LinearViscoelasticNormalSpecies*>(this);
    if (species!= nullptr)
    {
        species->setCollisionTimeAndRestitutionCoefficient(tc, eps, mass);
        stiffness = species->getStiffness();
    }
    else
    {
        LinearPlasticViscoelasticNormalSpecies* species2 = dynamic_cast<LinearPlasticViscoelasticNormalSpecies*>(this);
        if (species2 != nullptr)
        {
            species2->setDissipation(-mass / tc * std::log(eps));
            species2->setLoadingStiffness(.5 * mass * (mathsFunc::square(constants::pi/tc) + mathsFunc::square(species2->getDissipation()) /mass));
            stiffness = species2->getLoadingStiffness();
        }
        else
        {
            std::cerr << "SlidingFrictionSpecies::setCollisionTimeAndNormalAndTangentialRestitutionCoefficient only works for LinearViscoelasticSlidingFrictionSpecies or LinearPlasticViscoelasticSlidingFrictionSpecies" << std::endl;
            exit(-1);
        }
    }

    //from Deen...Kuipers2006, eq. 43 and 30
    setSlidingStiffness(2.0 / 7.0 * stiffness * (mathsFunc::square(constants::pi) + mathsFunc::square(log(beta))) / (mathsFunc::square(constants::pi) + mathsFunc::square(log(eps))));
    if (beta != 0.0)
        setSlidingDissipation(-2 * log(beta) * sqrt(1.0 / 7.0 * mass * getSlidingStiffness() / (mathsFunc::square(constants::pi) + mathsFunc::square(log(beta)))));
    else
        setSlidingDissipation(2. * sqrt(1.0 / 7.0 * mass * getSlidingStiffness()));
}

///Sets k, disp, kt, dispt such that it matches a given tc and eps for a collision of two particles of masses m0,m1
void SlidingFrictionSpecies::setCollisionTimeAndNormalAndTangentialRestitutionCoefficientNoDispt(Mdouble tc, Mdouble eps, Mdouble beta, Mdouble mass)
{
    //the dynamic cast is needed to check if the normal force species is LinearViscoelasticSpecies; otherwise this function cannot be used
    LinearViscoelasticNormalSpecies* species = dynamic_cast<LinearViscoelasticNormalSpecies*>(this);
    if (species == nullptr)
    {
        std::cerr << "SlidingFrictionSpecies::setCollisionTimeAndNormalAndTangentialRestitutionCoefficient only works for LinearViscoelasticSlidingFrictionSpecies" << std::endl;
        exit(-1);
    }
    species->setCollisionTimeAndRestitutionCoefficient(tc, eps, mass);
    //from BeckerSchwagerPoeschel2008, eq. 56
    setSlidingStiffness(2.0 / 7.0 * species->getStiffness() * mathsFunc::square(acos(-beta) / constants::pi));
    setSlidingDissipation(0);
}
