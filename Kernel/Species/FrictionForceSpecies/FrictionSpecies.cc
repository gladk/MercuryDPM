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

#include "FrictionSpecies.h"
#include<cmath>
#include "Species/BaseSpecies.h"

class BaseParticle;
class BaseInteractable;

FrictionSpecies::FrictionSpecies()
    : SlidingFrictionSpecies()
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

/*!
 * \param[in] the species that is copied
 */
FrictionSpecies::FrictionSpecies(const FrictionSpecies &s)
    : SlidingFrictionSpecies(s)
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
#ifdef DEBUG_DESTRUCTOR
    std::cout<<"FrictionSpecies::~FrictionSpecies() finished"<<std::endl;
#endif   
}

/*!
 * \param[out] output stream (typically the restart file)
 */
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

/*!
 * \param[in] input stream (typically the restart file)
 */
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

/*!
 * \return a string containing the name of the species (minus the word "Species")
 */
std::string FrictionSpecies::getBaseName() const
{
    return "Friction";
}

BaseInteraction* FrictionSpecies::getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
{
    return new FrictionInteraction(P, I, timeStamp);
}

/*!
 * \details Returns true for any FrictionForceSpecies except EmptyFrictionSpecies, 
 * because for spherical particles, torques are only caused by tangential forces. 
 * See SpeciesHandler::useAngularDOFs for more details
 * \return true 
 */
bool FrictionSpecies::getUseAngularDOFs() const
{
    return true;
}

/*!
 * \details For all parameters we assume that the harmonic mean of the parameters of the 
 * original two species is a sensible default.
 * \param[in] S,T the two species whose properties are mixed to create the new species
 */
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
