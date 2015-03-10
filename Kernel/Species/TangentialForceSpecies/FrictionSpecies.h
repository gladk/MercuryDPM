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

#ifndef FrictionSpecies_H
#define FrictionSpecies_H
#include "SlidingFrictionSpecies.h"
#include "Math/ExtendedMath.h"
#include "Interactions/TangentialForceInteractions/FrictionInteraction.h"
//class SpeciesHandler;
class BaseInteractable;
class BaseInteraction;

//Note the getVelocity can for some Species be dependent on which point on the Species is meant.
class FrictionSpecies : public virtual SlidingFrictionSpecies
{
public:
    typedef FrictionInteraction InteractionType;
    FrictionSpecies();
    FrictionSpecies(const FrictionSpecies &s);
    virtual ~FrictionSpecies();
    virtual FrictionSpecies* copy() const;
    virtual void read(std::istream& is);
    virtual void write(std::ostream& os) const;
    virtual void clear();
    virtual std::string getBaseName() const;

    virtual BaseInteraction* getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);
    bool getUseAngularDOFs() const;
    void mix(FrictionSpecies* const S, FrictionSpecies* const T);

//setters and getters

    ///Allows the spring constant to be changed
    void setRollingStiffness(Mdouble new_kt);

    ///Allows the spring constant to be accessed
    Mdouble getRollingStiffness() const;

    ///Allows the tangential viscosity to be changed
    void setRollingDissipation(Mdouble new_dispt);

    ///Allows the tangential viscosity to be accessed
    Mdouble getRollingDissipation() const;

    ///Allows the (dynamic) Coulomb friction coefficient to be changed; also sets mu_s by default
    //mu has to be set to allow tangential forces (sets dispt=disp as default)
    void setRollingFrictionCoefficient(Mdouble new_mu);

    ///Allows the (dynamic) Coulomb friction coefficient to be accessed
    Mdouble getRollingFrictionCoefficient() const;

    void setRollingFrictionCoefficientStatic(Mdouble new_mu);

    Mdouble getRollingFrictionCoefficientStatic() const;

    void setTorsionStiffness(Mdouble new_kt);

    ///Allows the spring constant to be accessed
    Mdouble getTorsionStiffness() const;

    ///Allows the tangential viscosity to be changed
    void setTorsionDissipation(Mdouble new_dispt);

    ///Allows the tangential viscosity to be accessed
    Mdouble getTorsionDissipation() const;

    ///Allows the (dynamic) Coulomb friction coefficient to be changed; also sets mu_s by default
    //mu has to be set to allow tangential forces (sets dispt=disp as default)
    void setTorsionFrictionCoefficient(Mdouble new_mu);

    ///Allows the (dynamic) Coulomb friction coefficient to be accessed
    Mdouble getTorsionFrictionCoefficient() const;

    void setTorsionFrictionCoefficientStatic(Mdouble new_mu);

    Mdouble getTorsionFrictionCoefficientStatic() const;

private:
    Mdouble rollingStiffness_;
    Mdouble rollingDissipation_; ///<tangential viscosity: should satisfy \f$4*dispt*dt<mass, dispt \approx disp\f$
    Mdouble rollingFrictionCoefficient_; ///< (dynamic) Coulomb friction coefficient
    Mdouble rollingFrictionCoefficientStatic_; ///<static Coulomb friction coefficient (by default set equal to mu)
    Mdouble torsionStiffness_;
    Mdouble torsionDissipation_; ///<tangential viscosity: should satisfy \f$4*dispt*dt<mass, dispt \approx disp\f$
    Mdouble torsionFrictionCoefficient_; ///< (dynamic) Coulomb friction coefficient
    Mdouble torsionFrictionCoefficientStatic_; ///<static Coulomb friction coefficient (by default set equal to mu)
};
#endif
