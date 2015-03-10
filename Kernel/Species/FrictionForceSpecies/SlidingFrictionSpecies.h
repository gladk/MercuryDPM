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

#ifndef SLIDINGFRICTIONSPECIES_H
#define SLIDINGFRICTIONSPECIES_H
#include "Species/BaseSpecies.h"
#include "Math/ExtendedMath.h"
#include "Interactions/FrictionForceInteractions/SlidingFrictionInteraction.h"
class BaseInteractable;
class BaseInteraction;

//Note the getVelocity can for some Species be dependent on which point on the Species is meant.
class SlidingFrictionSpecies : public virtual BaseSpecies
{
public:
    typedef SlidingFrictionInteraction InteractionType;
    SlidingFrictionSpecies();
    SlidingFrictionSpecies(const SlidingFrictionSpecies &s);
    virtual ~SlidingFrictionSpecies();
    virtual SlidingFrictionSpecies* copy() const;
    virtual void read(std::istream& is);
    virtual void write(std::ostream& os) const;
    virtual void clear();
    virtual std::string getBaseName() const;

//setters and getters
    ///Allows the spring constant to be changed
    void setSlidingStiffness(Mdouble new_kt);

    ///Allows the spring constant to be accessed
    Mdouble getSlidingStiffness() const;


    ///Allows the tangential viscosity to be changed
    void setSlidingDissipation(Mdouble new_dispt);

    ///Allows the tangential viscosity to be accessed
    Mdouble getSlidingDissipation() const;

    ///Allows the (dynamic) Coulomb friction coefficient to be changed; also sets mu_s by default
    //mu has to be set to allow tangential forces (sets dispt=disp as default)
    void setSlidingFrictionCoefficient(Mdouble new_mu);

    ///Allows the (dynamic) Coulomb friction coefficient to be accessed
    Mdouble getSlidingFrictionCoefficient() const;

    void setSlidingFrictionCoefficientStatic(Mdouble new_mu);

    Mdouble getSlidingFrictionCoefficientStatic() const;

    virtual BaseInteraction* getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);

    bool getUseAngularDOFs() const;

    void mix(SlidingFrictionSpecies* const S, SlidingFrictionSpecies* const T);

    ///Sets k, disp, kt, dispt such that it matches a given tc and eps for a collision of two particles of masses m0,m1
    void setCollisionTimeAndNormalAndTangentialRestitutionCoefficient(Mdouble tc, Mdouble eps, Mdouble beta, Mdouble mass);

    void setCollisionTimeAndNormalAndTangentialRestitutionCoefficientNoDispt(Mdouble tc, Mdouble eps, Mdouble beta, Mdouble mass);

private:
    Mdouble slidingStiffness_;
    Mdouble slidingDissipation_; ///<tangential viscosity: should satisfy \f$4*dispt*dt<mass, dispt \approx disp\f$
    Mdouble slidingFrictionCoefficient_; ///< (dynamic) Coulomb friction coefficient
    Mdouble slidingFrictionCoefficientStatic_; ///<static Coulomb friction coefficient (by default set equal to mu)
};
#endif
