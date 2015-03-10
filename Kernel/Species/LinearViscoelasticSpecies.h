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

#ifndef LinearViscoelasticSpecies_H
#define LinearViscoelasticSpecies_H
#include "BaseSpecies.h"
#include "Math/ExtendedMath.h"
#include "Interactions/LinearViscoelasticInteraction.h"
//class SpeciesHandler;
class BaseInteractable;
class BaseInteraction;

//Note the getVelocity can for some Species be dependent on which point on the Species is meant.
class LinearViscoelasticSpecies : public virtual BaseSpecies
{
public:
    typedef LinearViscoelasticInteraction InteractionType;

    LinearViscoelasticSpecies();
    LinearViscoelasticSpecies(const LinearViscoelasticSpecies &p);
    virtual ~LinearViscoelasticSpecies();
    LinearViscoelasticSpecies* copy() const;
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
    void mix(LinearViscoelasticSpecies* const SBase, LinearViscoelasticSpecies* const TBase);

//setters and getters

    ///Allows the spring constant to be changed
    void setStiffness(Mdouble new_k);

    ///Allows the spring constant to be accessed
    Mdouble getStiffness() const;

    void setDissipation(Mdouble dissipation);

    ///Allows the normal dissipation to be accessed
    Mdouble getDissipation() const;

    ///Allows the spring and dissipation constants to be changed simultaneously
    void setNormalStiffnessAndDissipation(helpers::KAndDisp new_);

    ///Allows the dimension of the particle (f.e. for mass) to be accessed
    bool getUseAngularDOFs() const;

private:
    Mdouble stiffness_; ///<(normal) spring constant
    Mdouble dissipation_; ///<(normal) viscosity
};
#endif
