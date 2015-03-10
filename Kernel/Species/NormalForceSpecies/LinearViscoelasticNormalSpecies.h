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

#ifndef LinearViscoelasticNormalSpecies_H
#define LinearViscoelasticNormalSpecies_H
#include "Species/BaseSpecies.h"
#include "Math/ExtendedMath.h"
#include "Interactions/NormalForceInteractions/LinearViscoelasticInteraction.h"
//class SpeciesHandler;
class BaseInteractable;
class BaseInteraction;

//Note the getVelocity can for some Species be dependent on which point on the Species is meant.
class LinearViscoelasticNormalSpecies : public virtual BaseSpecies
{
public:
    typedef LinearViscoelasticInteraction InteractionType;

    LinearViscoelasticNormalSpecies();
    LinearViscoelasticNormalSpecies(const LinearViscoelasticNormalSpecies &p);
    virtual ~LinearViscoelasticNormalSpecies();
    LinearViscoelasticNormalSpecies* copy() const;
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void clear();
    std::string getBaseName() const;
    BaseInteraction* getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);

// Species-specific functions

    ///Calculates the maximum velocity allowed for a collision of two copies of P (for higher velocities particles could pass through each other)
    Mdouble getMaximumVelocity(Mdouble radius, Mdouble mass);

    ///Sets k, disp such that it matches a given tc and eps for a collision of two copies of P
    void setStiffnessAndRestitutionCoefficient(Mdouble k_, Mdouble eps, Mdouble mass);

    ///Sets k, disp such that it matches a given tc and eps for a collision of two copies of equal mass m
    void setCollisionTimeAndRestitutionCoefficient(Mdouble tc, Mdouble eps, Mdouble mass);

    ///Set k, disp such that is matches a given tc and eps for a collision of two different masses.
    ///Recall the resitution constant is a function of k, disp and the mass of each particle in the collision
    /// See also setCollisionTimeAndRestitutionCoefficient(Mdouble tc, Mdouble eps, Mdouble mass)
    void setCollisionTimeAndRestitutionCoefficient(Mdouble collisionTime, Mdouble restitutionCoefficient, Mdouble mass1, Mdouble mass2);

    /*!
     * \brief Calculates collision time for two copies of a particle of given disp, k, mass
     */
    Mdouble getCollisionTime(Mdouble mass);

    /*!
     * \brief Calculates restitution coefficient for two copies of given disp, k, mass
     */
    Mdouble getRestitutionCoefficient(Mdouble mass);

    ///create values for mixed species
    void mix(LinearViscoelasticNormalSpecies* const SBase, LinearViscoelasticNormalSpecies* const TBase);

//setters and getters

    ///Allows the spring constant to be changed
    void setStiffness(Mdouble new_k);

    ///Allows the spring constant to be accessed
    Mdouble getStiffness() const;

    void setDissipation(Mdouble dissipation);

    ///Allows the normal dissipation to be accessed
    Mdouble getDissipation() const;

    ///Allows the spring and dissipation constants to be changed simultaneously
    void setStiffnessAndDissipation(helpers::KAndDisp new_);

    ///Allows the dimension of the particle (f.e. for mass) to be accessed
    bool getUseAngularDOFs() const;

private:
    Mdouble stiffness_; ///<(normal) spring constant
    Mdouble dissipation_; ///<(normal) viscosity
};
#endif
