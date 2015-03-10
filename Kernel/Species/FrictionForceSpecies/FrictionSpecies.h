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

#ifndef FrictionSpecies_H
#define FrictionSpecies_H
#include "SlidingFrictionSpecies.h"
#include "Math/ExtendedMath.h"
#include "Interactions/FrictionForceInteractions/FrictionInteraction.h"
//class SpeciesHandler;
class BaseInteractable;
class BaseInteraction;

//Note the getVelocity can for some Species be dependent on which point on the Species is meant.
///\todo TW: should this class be renamed SlidingRollingTorsionFrictionSpecies?
class FrictionSpecies : public SlidingFrictionSpecies
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
