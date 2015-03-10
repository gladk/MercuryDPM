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

#ifndef LinearPlasticViscoelasticNormalSpecies_H
#define LinearPlasticViscoelasticNormalSpecies_H
#include "Species/BaseSpecies.h"
#include "Math/ExtendedMath.h"
#include "Interactions/NormalForceInteractions/LinearPlasticViscoelasticInteraction.h"
class SpeciesHandler;
//class BaseParticle;
class BaseParticle;
class BaseInteractable;
class BaseInteraction;

//Note the getVelocity can for some Species be dependent on which point on the Species is meant.
class LinearPlasticViscoelasticNormalSpecies : public virtual BaseSpecies
{
public:
    typedef LinearPlasticViscoelasticInteraction InteractionType;
    LinearPlasticViscoelasticNormalSpecies();
    LinearPlasticViscoelasticNormalSpecies(const LinearPlasticViscoelasticNormalSpecies &p);
    virtual ~LinearPlasticViscoelasticNormalSpecies();
    LinearPlasticViscoelasticNormalSpecies* copy() const;
    void read(std::istream& is);
    void write(std::ostream& os) const;
    void clear();
    std::string getBaseName() const;
    BaseInteraction* getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);

// Species-specific functions

    ///create values for mixed species
    void mix(LinearPlasticViscoelasticNormalSpecies* const S, LinearPlasticViscoelasticNormalSpecies* const T);

//setters and getters

    ///Acccess functions for the plastic model
    void setPlasticParameters(Mdouble loadingStiffness, Mdouble maxUnloadingStiffness, Mdouble cohesionStiffness, Mdouble maxPenetration);

    ///gets k_1 in the plastic model
    Mdouble getLoadingStiffness() const;

    ///gets k_2^max in the plastic model
    Mdouble getUnloadingStiffnessMax() const;

    ///gets k_c in the plastic model
    Mdouble getCohesionStiffness() const;

    ///gets phi_f in the plastic model (consider renaming)
    Mdouble getPenetrationDepthMax() const;

    void setLoadingStiffness(Mdouble loadingStiffness);

    void setUnloadingStiffnessMax(Mdouble maxUnloadingStiffness);

    void setCohesionStiffness(Mdouble cohesionStiffness);

    void setPenetrationDepthMax(Mdouble maxPenetration);

    void setDissipation(Mdouble dissipation);

    Mdouble computePlasticTimeStep(Mdouble mass);

    ///Allows the normal dissipation to be accessed
    Mdouble getDissipation() const;

    ///Allows the dimension of the particle (f.e. for mass) to be accessed
    bool getUseAngularDOFs() const;

private:
    Mdouble loadingStiffness_; ///<(normal) spring constant
    Mdouble unloadingStiffnessMax_; ///<for plastic deformations; the maximum elastic constant
    Mdouble cohesionStiffness_; ///<for plastic deformations; the adhesive spring constant
    Mdouble penetrationDepthMax_; ///<for plastic deformations; the depth (relative to the normalized radius) at which kpmax is used
    Mdouble dissipation_; ///<(normal) viscosity

};
#endif
