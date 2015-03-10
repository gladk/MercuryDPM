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


#include "LinearPlasticViscoelasticNormalSpecies.h"
#include "Interactions/NormalForceInteractions/LinearPlasticViscoelasticInteraction.h"

class BaseParticle;
class BaseInteractable;

LinearPlasticViscoelasticNormalSpecies::LinearPlasticViscoelasticNormalSpecies()
    : BaseSpecies()
{
    loadingStiffness_ = 0.0;
    unloadingStiffnessMax_ = 0.0;
    cohesionStiffness_ = 0.0;
    penetrationDepthMax_ = 0.0;
    dissipation_ = 0.0;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LinearPlasticViscoelasticNormalSpecies::LinearPlasticViscoelasticNormalSpecies() finished"<<std::endl;
#endif
}

LinearPlasticViscoelasticNormalSpecies::LinearPlasticViscoelasticNormalSpecies(const LinearPlasticViscoelasticNormalSpecies &p)
    : BaseSpecies(p)
{
    loadingStiffness_ = p.loadingStiffness_;
    unloadingStiffnessMax_ = p.unloadingStiffnessMax_;
    cohesionStiffness_ = p.cohesionStiffness_;
    penetrationDepthMax_ = p.penetrationDepthMax_;
    dissipation_ = p.dissipation_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LinearPlasticViscoelasticNormalSpecies::LinearPlasticViscoelasticNormalSpecies(const LinearPlasticViscoelasticNormalSpecies &p) finished"<<std::endl;
#endif
}

LinearPlasticViscoelasticNormalSpecies::~LinearPlasticViscoelasticNormalSpecies()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"LinearPlasticViscoelasticNormalSpecies::~LinearPlasticViscoelasticNormalSpecies() finished"<<std::endl;
#endif   
}

void LinearPlasticViscoelasticNormalSpecies::clear()
{
    std::cout << "LinearPlasticViscoelasticNormalSpecies::clear(), this function shouldn't be called" << std::endl;
}

///LinearPlasticViscoelasticNormalSpecies copy method. It calls to copy constructor of this LinearPlasticViscoelasticNormalSpecies, useful for polymorphism
LinearPlasticViscoelasticNormalSpecies* LinearPlasticViscoelasticNormalSpecies::copy() const
{
    return new LinearPlasticViscoelasticNormalSpecies(*this);
}

///LinearPlasticViscoelasticNormalSpecies print function, which accepts an os std::stringstream as input. It prints human readable LinearPlasticViscoelasticNormalSpecies information to the std::stringstream
void LinearPlasticViscoelasticNormalSpecies::write(std::ostream& os) const
{
    os  << " loadingStiffness " << loadingStiffness_;
    os  << " maxUnloadingStiffness " << unloadingStiffnessMax_;
    os  << " cohesionStiffness " << cohesionStiffness_;
    os  << " maxPenetration " << penetrationDepthMax_;
    os  << " dissipation " << dissipation_;
}

void LinearPlasticViscoelasticNormalSpecies::read(std::istream& is)
{
    std::string dummy;
    is >> dummy >> loadingStiffness_;
    is >> dummy >> unloadingStiffnessMax_;
    is >> dummy >> cohesionStiffness_;
    is >> dummy >> penetrationDepthMax_;
    is >> dummy >> dissipation_;
}

std::string LinearPlasticViscoelasticNormalSpecies::getBaseName() const
{
    return "LinearPlasticViscoelastic";
}

bool LinearPlasticViscoelasticNormalSpecies::getUseAngularDOFs() const
{
    return false;
}

///create values for mixed species
void LinearPlasticViscoelasticNormalSpecies::mix(LinearPlasticViscoelasticNormalSpecies* const S, LinearPlasticViscoelasticNormalSpecies* const T)
{
    BaseSpecies::mix(S, T);
    loadingStiffness_ = average(S->getLoadingStiffness(), T->getLoadingStiffness());
    unloadingStiffnessMax_ = average(S->getUnloadingStiffnessMax(), T->getUnloadingStiffnessMax());
    cohesionStiffness_ = average(S->getCohesionStiffness(), T->getCohesionStiffness());
    penetrationDepthMax_ = average(S->getPenetrationDepthMax(), T->getPenetrationDepthMax());
    dissipation_ = average(S->getDissipation(), T->getDissipation());
}

BaseInteraction* LinearPlasticViscoelasticNormalSpecies::getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
{
    return new LinearPlasticViscoelasticInteraction(P, I, timeStamp);
}


///Acccess functions for the plastic model
void LinearPlasticViscoelasticNormalSpecies::setPlasticParameters(Mdouble loadingStiffness, Mdouble maxUnloadingStiffness, Mdouble cohesionStiffness, Mdouble maxPenetration)
{
    if (loadingStiffness <= 0 || maxUnloadingStiffness < loadingStiffness || cohesionStiffness < 0 || maxPenetration < 0 || maxPenetration > 1)
    {
        std::cerr << "Error: arguments of setPlasticParameters do not make sense" << std::endl;
        exit(-1);
    }
    setLoadingStiffness(loadingStiffness);
    setUnloadingStiffnessMax(maxUnloadingStiffness);
    setCohesionStiffness(cohesionStiffness);
    setPenetrationDepthMax(maxPenetration);
}

Mdouble LinearPlasticViscoelasticNormalSpecies::getLoadingStiffness() const
{
    return loadingStiffness_;
}
Mdouble LinearPlasticViscoelasticNormalSpecies::getUnloadingStiffnessMax() const
{
    return unloadingStiffnessMax_;
}
Mdouble LinearPlasticViscoelasticNormalSpecies::getCohesionStiffness() const
{
    return cohesionStiffness_;
}
Mdouble LinearPlasticViscoelasticNormalSpecies::getPenetrationDepthMax() const
{
    return penetrationDepthMax_;
}
void LinearPlasticViscoelasticNormalSpecies::setLoadingStiffness(Mdouble loadingStiffness)
{
    loadingStiffness_ = loadingStiffness;
}
void LinearPlasticViscoelasticNormalSpecies::setUnloadingStiffnessMax(Mdouble maxUnloadingStiffness)
{
    unloadingStiffnessMax_ = maxUnloadingStiffness;
}
void LinearPlasticViscoelasticNormalSpecies::setCohesionStiffness(Mdouble cohesionStiffness)
{
    cohesionStiffness_ = cohesionStiffness;
}
void LinearPlasticViscoelasticNormalSpecies::setPenetrationDepthMax(Mdouble maxPenetration)
{
    penetrationDepthMax_ = maxPenetration;
}
///Calculates collision time for stiffest spring constant, divides by 50
Mdouble LinearPlasticViscoelasticNormalSpecies::computePlasticTimeStep(Mdouble mass)
{
    return 0.02 * constants::pi / std::sqrt(unloadingStiffnessMax_ / (.5 * mass) - mathsFunc::square(dissipation_ /mass));
}

///Allows the normal dissipation to be changed
void LinearPlasticViscoelasticNormalSpecies::setDissipation(Mdouble dissipation)
{
    if (dissipation >= 0)
    {
        dissipation_ = dissipation;
    }
    else
    {
        std::cerr << "Error in setDissipation(" << dissipation << ")" << std::endl;
        exit(-1);
    }
}

///Allows the spring and dissipation constants to be changed simultaneously
void LinearPlasticViscoelasticNormalSpecies::setLoadingStiffnessAndDissipation(helpers::KAndDisp new_)
{
    setLoadingStiffness(new_.k);
    setDissipation(new_.disp);
}

///Allows the normal dissipation to be accessed
Mdouble LinearPlasticViscoelasticNormalSpecies::getDissipation() const
{
    return dissipation_;
}
