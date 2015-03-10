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
    void setPlasticParameters(Mdouble loadingStiffness, Mdouble unloadingStiffnessMax, Mdouble cohesionStiffness, Mdouble penetrationDepthMax);

    ///gets k_1 in the plastic model
    Mdouble getLoadingStiffness() const;

    ///gets k_2^max in the plastic model
    Mdouble getUnloadingStiffnessMax() const;

    ///gets k_c in the plastic model
    Mdouble getCohesionStiffness() const;

    ///gets phi_f in the plastic model (consider renaming)
    Mdouble getPenetrationDepthMax() const;

    void setLoadingStiffness(Mdouble loadingStiffness);

    void setUnloadingStiffnessMax(Mdouble unloadingStiffnessMax);

    void setCohesionStiffness(Mdouble cohesionStiffness);

    void setPenetrationDepthMax(Mdouble penetrationDepthMax);

    void setDissipation(Mdouble dissipation);

    ///Allows the spring and dissipation constants to be changed simultaneously
    void setLoadingStiffnessAndDissipation(helpers::KAndDisp new_);

    Mdouble computeTimeStep(Mdouble mass);

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
