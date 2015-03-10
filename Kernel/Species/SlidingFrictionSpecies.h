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

#ifndef SLIDINGFRICTIONSPECIES_H
#define SLIDINGFRICTIONSPECIES_H
#include "BaseSpecies.h"
#include "Math/ExtendedMath.h"
#include "Interactions/SlidingFrictionInteraction.h"
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
