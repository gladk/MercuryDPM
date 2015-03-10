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

/*!
 * \param[in] the species that is copied
 */
LinearPlasticViscoelasticNormalSpecies::LinearPlasticViscoelasticNormalSpecies(const LinearPlasticViscoelasticNormalSpecies &p)
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
#ifdef DEBUG_DESTRUCTOR
    std::cout<<"LinearPlasticViscoelasticNormalSpecies::~LinearPlasticViscoelasticNormalSpecies() finished"<<std::endl;
#endif   
}

/*!
 * \param[out] output stream (typically the restart file)
 */
void LinearPlasticViscoelasticNormalSpecies::write(std::ostream& os) const
{
    os  << " loadingStiffness " << loadingStiffness_;
    os  << " maxUnloadingStiffness " << unloadingStiffnessMax_;
    os  << " cohesionStiffness " << cohesionStiffness_;
    os  << " maxPenetration " << penetrationDepthMax_;
    os  << " dissipation " << dissipation_;
}

/*!
 * \param[in] input stream (typically the restart file)
 */
void LinearPlasticViscoelasticNormalSpecies::read(std::istream& is)
{
    std::string dummy;
    is >> dummy >> loadingStiffness_;
    is >> dummy >> unloadingStiffnessMax_;
    is >> dummy >> cohesionStiffness_;
    is >> dummy >> penetrationDepthMax_;
    is >> dummy >> dissipation_;
}

/*!
 * \return a string containing the name of the species (minus the word "Species")
 */
std::string LinearPlasticViscoelasticNormalSpecies::getBaseName() const
{
    return "LinearPlasticViscoelastic";
}

/*!
 * \details For all parameters we assume that the harmonic mean of the parameters of the 
 * original two species is a sensible default.
 * \param[in] S,T the two species whose properties are mixed to create the new species
 */
void LinearPlasticViscoelasticNormalSpecies::mix(LinearPlasticViscoelasticNormalSpecies* const S, LinearPlasticViscoelasticNormalSpecies* const T)
{
    loadingStiffness_ = average(S->getLoadingStiffness(), T->getLoadingStiffness());
    unloadingStiffnessMax_ = average(S->getUnloadingStiffnessMax(), T->getUnloadingStiffnessMax());
    cohesionStiffness_ = average(S->getCohesionStiffness(), T->getCohesionStiffness());
    penetrationDepthMax_ = average(S->getPenetrationDepthMax(), T->getPenetrationDepthMax());
    dissipation_ = average(S->getDissipation(), T->getDissipation());
}

/*!
 * \param[in] loadingStiffness      the loading stiffness of the linear plastic-viscoelastic normal force.
 * \param[in] unloadingStiffnessMax the maximum unloading stiffness of the linear plastic-viscoelastic normal force.
 * \param[in] cohesionStiffness     the cohesive stiffness of the linear plastic-viscoelastic normal force.
 * \param[in] penetrationDepthMax   the maximum penetration depth of the linear plastic-viscoelastic normal force.
 */
void LinearPlasticViscoelasticNormalSpecies::setPlasticParameters (Mdouble loadingStiffness, Mdouble unloadingStiffnessMax, Mdouble cohesionStiffness, Mdouble penetrationDepthMax)
{
    if (loadingStiffness <= 0 || unloadingStiffnessMax < loadingStiffness || cohesionStiffness < 0 || penetrationDepthMax < 0 || penetrationDepthMax > 1)
    {
        std::cerr << "Error: arguments of setPlasticParameters do not make sense" << std::endl;
        exit(-1);
    }
    setLoadingStiffness(loadingStiffness);
    setUnloadingStiffnessMax(unloadingStiffnessMax);
    setCohesionStiffness(cohesionStiffness);
    setPenetrationDepthMax(penetrationDepthMax);
}

/*!
 * \return the loading stiffness of the linear plastic-viscoelastic normal force.
 */
Mdouble LinearPlasticViscoelasticNormalSpecies::getLoadingStiffness() const
{
    return loadingStiffness_;
}

/*!
 * \return the maximum unloading stiffness of the linear plastic-viscoelastic normal force.
 */
Mdouble LinearPlasticViscoelasticNormalSpecies::getUnloadingStiffnessMax() const
{
    return unloadingStiffnessMax_;
}

/*!
 * \return the cohesive stiffness of the linear plastic-viscoelastic normal force.
 */
Mdouble LinearPlasticViscoelasticNormalSpecies::getCohesionStiffness() const
{
    return cohesionStiffness_;
}

/*!
 * \return the maximum penetration depth of the linear plastic-viscoelastic normal force.
 */
Mdouble LinearPlasticViscoelasticNormalSpecies::getPenetrationDepthMax() const
{
    return penetrationDepthMax_;
}

/*!
 * \param[in] loadingStiffness      the loading stiffness of the linear plastic-viscoelastic normal force.
 */
void LinearPlasticViscoelasticNormalSpecies::setLoadingStiffness(Mdouble loadingStiffness)
{
    loadingStiffness_ = loadingStiffness;
}

/*!
 * \param[in] unloadingStiffnessMax the maximum unloading stiffness of the linear plastic-viscoelastic normal force.
 */
void LinearPlasticViscoelasticNormalSpecies::setUnloadingStiffnessMax(Mdouble unloadingStiffnessMax)
{
    unloadingStiffnessMax_ = unloadingStiffnessMax;
}

/*!
 * \param[in] cohesionStiffness     the cohesive stiffness of the linear plastic-viscoelastic normal force.
 */
void LinearPlasticViscoelasticNormalSpecies::setCohesionStiffness(Mdouble cohesionStiffness)
{
    cohesionStiffness_ = cohesionStiffness;
}

/*!
 * \param[in] penetrationDepthMax   the maximum penetration depth of the linear plastic-viscoelastic normal force.
 */
void LinearPlasticViscoelasticNormalSpecies::setPenetrationDepthMax(Mdouble penetrationDepthMax)
{
    penetrationDepthMax_ = penetrationDepthMax;
}

/*!
 * \details Calculates collision time for stiffest spring constant, divides by 50
 * \param[in] the optimal time step is computed to resolve a collision of two particles of this mass.
 */
Mdouble LinearPlasticViscoelasticNormalSpecies::computeTimeStep(Mdouble mass)
{
    return 0.02 * constants::pi / std::sqrt(unloadingStiffnessMax_ / (.5 * mass) - mathsFunc::square(dissipation_ /mass));
}

/*!
 * \details should be non-negative
 * \param[in] the linear dissipation coefficient of the linear plastic-viscoelastic normal force.
 */
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

/*!
 * \param[in] a helper struct containing both the loading stiffness and the disssipation coefficient.
 */
void LinearPlasticViscoelasticNormalSpecies::setLoadingStiffnessAndDissipation(helpers::KAndDisp new_)
{
    setLoadingStiffness(new_.k);
    setDissipation(new_.disp);
}

/*!
 * \return the linear dissipation coefficient
 */
Mdouble LinearPlasticViscoelasticNormalSpecies::getDissipation() const
{
    return dissipation_;
}

/*!
 * \details Sets k, disp such that it matches a given tc and eps for a collision of two copies of equal mass m
 * \param[in] tc collision time
 * \param[in] eps restitution coefficient
 * \param[in] mass effective particle mass, \f$\frac{2}{1/m1+1/m2}\f$
 */
void LinearPlasticViscoelasticNormalSpecies::setCollisionTimeAndRestitutionCoefficient(Mdouble tc, Mdouble eps, Mdouble mass)
{
    dissipation_ = -mass / tc * std::log(eps);
    loadingStiffness_ = .5 * mass * (mathsFunc::square(constants::pi / tc) + mathsFunc::square(dissipation_ / mass));
    unloadingStiffnessMax_ = loadingStiffness_;
}
