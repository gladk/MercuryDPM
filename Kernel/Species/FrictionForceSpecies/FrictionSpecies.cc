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

#include "FrictionSpecies.h"
#include<cmath>
#include "Species/BaseSpecies.h"

class BaseParticle;
class BaseInteractable;

FrictionSpecies::FrictionSpecies()
    : BaseSpecies()
{
    rollingStiffness_=0.0;
    rollingDissipation_=0.0;
    rollingFrictionCoefficient_=0.0;
    rollingFrictionCoefficientStatic_=0.0;
    torsionStiffness_=0.0;
    torsionDissipation_=0.0;
    torsionFrictionCoefficient_=0.0;
    torsionFrictionCoefficientStatic_=0.0;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"FrictionSpecies::FrictionSpecies() finished"<<std::endl;
#endif
}

FrictionSpecies::FrictionSpecies(const FrictionSpecies &s)
    : BaseSpecies(s)
{
    rollingStiffness_=s.rollingStiffness_;
    rollingDissipation_=s.rollingDissipation_;
    rollingFrictionCoefficient_=s.rollingFrictionCoefficient_;
    rollingFrictionCoefficientStatic_=s.rollingFrictionCoefficientStatic_;
    torsionStiffness_=s.torsionStiffness_;
    torsionDissipation_=s.torsionDissipation_;
    torsionFrictionCoefficient_=s.torsionFrictionCoefficient_;
    torsionFrictionCoefficientStatic_=s.torsionFrictionCoefficientStatic_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"FrictionSpecies::FrictionSpecies(const FrictionSpecies &p) finished"<<std::endl;
#endif
}

FrictionSpecies::~FrictionSpecies()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"FrictionSpecies::~FrictionSpecies() finished"<<std::endl;
#endif   
}

void FrictionSpecies::clear()
{
    std::cout << "FrictionSpecies::clear(), this function shouldn't be called" << std::endl;
}

///FrictionSpecies copy method. It calls to copy constructor of this FrictionSpecies, useful for polymorphism
FrictionSpecies* FrictionSpecies::copy() const
{
    return new FrictionSpecies(*this);
}

///FrictionSpecies print function, which accepts an os std::stringstream as input. It prints human readable FrictionSpecies information to the std::stringstream
void FrictionSpecies::write(std::ostream& os) const
{
    SlidingFrictionSpecies::write(os);
    os  << " rollingStiffness " << rollingStiffness_;
    os  << " rollingDissipation " << rollingDissipation_;
    os  << " rollingFrictionCoefficient " << rollingFrictionCoefficient_;
    os  << " rollingFrictionCoefficientStatic " << rollingFrictionCoefficientStatic_;
    os  << " torsionStiffness " << torsionStiffness_;
    os  << " torsionDissipation " << torsionDissipation_;
    os  << " torsionFrictionCoefficient " << torsionFrictionCoefficient_;
    os  << " torsionFrictionCoefficientStatic " << torsionFrictionCoefficientStatic_;
}

void FrictionSpecies::read(std::istream& is)
{
    SlidingFrictionSpecies::read(is);
    std::string dummy;
    is >> dummy >> rollingStiffness_;
    is >> dummy >> rollingDissipation_;
    is >> dummy >> rollingFrictionCoefficient_;
    is >> dummy >> rollingFrictionCoefficientStatic_;
    is >> dummy >> torsionStiffness_;
    is >> dummy >> torsionDissipation_;
    is >> dummy >> torsionFrictionCoefficient_;
    is >> dummy >> torsionFrictionCoefficientStatic_;
}

std::string FrictionSpecies::getBaseName() const
{
    return "Friction";
}

BaseInteraction* FrictionSpecies::getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
{
    return new FrictionInteraction(P, I, timeStamp);
}

bool FrictionSpecies::getUseAngularDOFs() const
{
    return true;
}

///create values for mixed species
void FrictionSpecies::mix(FrictionSpecies* const S, FrictionSpecies* const T)
{
    rollingStiffness_= average(S->getRollingStiffness(), T->getRollingStiffness());
    rollingDissipation_= average(S->getRollingDissipation(), T->getRollingDissipation());
    rollingFrictionCoefficient_= average(S->getRollingFrictionCoefficient(), T->getRollingFrictionCoefficient());
    rollingFrictionCoefficientStatic_= average(S->getRollingFrictionCoefficientStatic(), T->getRollingFrictionCoefficientStatic());
    torsionStiffness_= average(S->getTorsionStiffness(), T->getTorsionStiffness());
    torsionDissipation_= average(S->getTorsionDissipation(), T->getTorsionDissipation());
    torsionFrictionCoefficient_= average(S->getTorsionFrictionCoefficient(), T->getTorsionFrictionCoefficient());
    torsionFrictionCoefficientStatic_= average(S->getTorsionFrictionCoefficientStatic(), T->getTorsionFrictionCoefficientStatic());
}

///Allows the spring constant to be changed
void FrictionSpecies::setRollingStiffness(Mdouble new_kt)
{
    if (new_kt >= 0)
    {
        rollingStiffness_ = new_kt;
    }
    else
    {
        std::cerr << "Error in setRollingStiffness" << std::endl;
        exit(-1);
    }
}

///Allows the spring constant to be accessed
Mdouble FrictionSpecies::getRollingStiffness() const
{
    return rollingStiffness_;
}

///Allows the tangential viscosity to be changed
void FrictionSpecies::setRollingDissipation(Mdouble new_dispt)
{
    if (new_dispt >= 0)
        rollingDissipation_ = new_dispt;
    else
    {
        std::cerr << "Error in setRollingDissipation" << std::endl;
        exit(-1);
    }
}
///Allows the tangential viscosity to be accessed
Mdouble FrictionSpecies::getRollingDissipation() const
{
    return rollingDissipation_;
}

///Allows the (dynamic) Coulomb friction coefficient to be changed; also sets mu_s by default
//mu has to be set to allow tangential forces (sets dispt=disp as default)
void FrictionSpecies::setRollingFrictionCoefficient(Mdouble new_mu)
{
    if (new_mu >= 0)
    {
        rollingFrictionCoefficient_ = new_mu;
        rollingFrictionCoefficientStatic_ = rollingFrictionCoefficient_;
    }
    else
    {
        std::cerr << "Error in setSlidingFrictionCoefficient" << std::endl;
        exit(-1);
    }
}

///Allows the (dynamic) Coulomb friction coefficient to be accessed
Mdouble FrictionSpecies::getRollingFrictionCoefficient() const
{
    return rollingFrictionCoefficient_;
}

///Allows the static Coulomb friction coefficient to be changed; also sets mu_s by default
void FrictionSpecies::setRollingFrictionCoefficientStatic(Mdouble new_mu)
{
    if (new_mu >= 0)
    {
        rollingFrictionCoefficientStatic_ = new_mu;
    }
    else
    {
        std::cerr << "Error in setSlidingFrictionCoefficientStatic" << std::endl;
        exit(-1);
    }
}

///Allows the static Coulomb friction coefficient to be accessed
Mdouble FrictionSpecies::getRollingFrictionCoefficientStatic() const
{
    return rollingFrictionCoefficientStatic_;
}


///Allows the spring constant to be changed
void FrictionSpecies::setTorsionStiffness(Mdouble new_kt)
{
    if (new_kt >= 0)
    {
        torsionStiffness_ = new_kt;
    }
    else
    {
        std::cerr << "Error in setTorsionStiffness" << std::endl;
        exit(-1);
    }
}

///Allows the spring constant to be accessed
Mdouble FrictionSpecies::getTorsionStiffness() const
{
    return torsionStiffness_;
}

///Allows the tangential viscosity to be changed
void FrictionSpecies::setTorsionDissipation(Mdouble new_dispt)
{
    if (new_dispt >= 0)
        torsionDissipation_ = new_dispt;
    else
    {
        std::cerr << "Error in setTorsionDissipation" << std::endl;
        exit(-1);
    }
}
///Allows the tangential viscosity to be accessed
Mdouble FrictionSpecies::getTorsionDissipation() const
{
    return torsionDissipation_;
}

///Allows the (dynamic) Coulomb friction coefficient to be changed; also sets mu_s by default
//mu has to be set to allow tangential forces (sets dispt=disp as default)
void FrictionSpecies::setTorsionFrictionCoefficient(Mdouble new_mu)
{
    if (new_mu >= 0)
    {
        torsionFrictionCoefficient_ = new_mu;
        torsionFrictionCoefficientStatic_ = torsionFrictionCoefficient_;
    }
    else
    {
        std::cerr << "Error in setSlidingFrictionCoefficient" << std::endl;
        exit(-1);
    }
}

///Allows the (dynamic) Coulomb friction coefficient to be accessed
Mdouble FrictionSpecies::getTorsionFrictionCoefficient() const
{
    return torsionFrictionCoefficient_;
}

///Allows the static Coulomb friction coefficient to be changed; also sets mu_s by default
void FrictionSpecies::setTorsionFrictionCoefficientStatic(Mdouble new_mu)
{
    if (new_mu >= 0)
    {
        torsionFrictionCoefficientStatic_ = new_mu;
    }
    else
    {
        std::cerr << "Error in setSlidingFrictionCoefficientStatic" << std::endl;
        exit(-1);
    }
}

///Allows the static Coulomb friction coefficient to be accessed
Mdouble FrictionSpecies::getTorsionFrictionCoefficientStatic() const
{
    return torsionFrictionCoefficientStatic_;
}
