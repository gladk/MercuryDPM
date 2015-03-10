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


#include "FrictionInteraction.h"
#include "Species/FrictionForceSpecies/FrictionSpecies.h"
#include "Particles/BaseParticle.h"
#include "InteractionHandler.h"
#include <iomanip>
#include <fstream>
#include <DPMBase.h>

/*!
 * \param[in] P
 * \param[in] I
 * \param[in] timeStamp
 */
FrictionInteraction::FrictionInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
    : BaseInteraction(P, I, timeStamp), SlidingFrictionInteraction(P, I, timeStamp)
{
    rollingSpring_.setZero();
    torsionSpring_.setZero();
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"FrictionInteraction::FrictionInteraction() finished"<<std::endl;
#endif
}
/*!
 * \param[in] p
 */
FrictionInteraction::FrictionInteraction(const FrictionInteraction& p)
    : BaseInteraction(p), SlidingFrictionInteraction(p)
{
    rollingSpring_=p.rollingSpring_;
    torsionSpring_=p.torsionSpring_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"FrictionInteraction::FrictionInteraction(const FrictionInteraction& p) finished"<<std::endl;
#endif
}
/*!
 *
 */
FrictionInteraction::~FrictionInteraction()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout<<"FrictionInteraction::~FrictionInteraction() finished"<<std::endl;
#endif
}
/*!
 * \param[in/out] os
 */
void FrictionInteraction::write(std::ostream& os) const
{
    SlidingFrictionInteraction::write(os);
    os << " rollingSpring " << rollingSpring_;
    os << " torsionSpring " << torsionSpring_;
}
/*!
 * \param[in/out] is
 */
void FrictionInteraction::read(std::istream& is)
{
    SlidingFrictionInteraction::read(is);
    std::string dummy;
    is >> dummy >> rollingSpring_;
    is >> dummy >> torsionSpring_;
}
/*!
 * \details Calls the slidingFrictionInteraction::computeFrictionForce() as well, see slidingFrictionInteraction.cc.
 */
void FrictionInteraction::computeFrictionForce()
{
    SlidingFrictionInteraction::computeFrictionForce();

    const FrictionSpecies* species = getSpecies();
    //If tangential forces are present
    if (getAbsoluteNormalForce() == 0.0) return;


    if (species->getRollingFrictionCoefficient() != 0.0)
    {
        if (species->getRollingStiffness() != 0.0)
        {
            Mdouble effectiveDiameter = 2.0*getEffectiveCorrectedRadius();

            //From Luding2008, objective rolling velocity (eq 15) w/o 2.0!
            Vec3D rollingRelativeVelocity = -effectiveDiameter * Vec3D::cross(getNormal(), getP()->getAngularVelocity() - getI()->getAngularVelocity());

            //used to Integrate the spring
            rollingSpringVelocity_= rollingRelativeVelocity;
            //integrate(getHandler()->timeStep_);
            rollingSpring_ += rollingSpringVelocity_ * getHandler()->getDPMBase()->getTimeStep();

            //Calculate test force acting on P including viscous force
            Vec3D rollingForce = - species->getRollingStiffness() * rollingSpring_ - species->getRollingDissipation() * rollingRelativeVelocity;

            //tangential forces are modelled by a spring-damper of elasticity kt and viscosity dispt (sticking),
            //but the force is limited by Coulomb friction (rolling):
            Mdouble rollingForceSquared = rollingForce.getLengthSquared();
            if (rollingForceSquared <= mathsFunc::square(species->getRollingFrictionCoefficientStatic() * getAbsoluteNormalForce()))
            {
                //if sticking (|ft|<=mu*|fn|), apply the force
            }
            else
            {
                //if rolling, resize the tangential force such that |ft|=mu*|fn|
                rollingForce *= species->getRollingFrictionCoefficient() * getAbsoluteNormalForce() / std::sqrt(rollingForceSquared);
                //resize the tangential spring accordingly such ft=-k*delta-nu*relVel
                rollingSpring_ = -(rollingForce + species->getRollingDissipation() * rollingRelativeVelocity) / species->getRollingStiffness();
            }
            //Add (virtual) rolling force to torque
            addTorque(effectiveDiameter * Vec3D::cross(getNormal(), rollingForce));
        }
        else //if no spring stiffness is set
        {
            std::cerr << "FrictionInteraction::computeFrictionForce(): Rolling stiffness is zero" << std::endl;
            exit(-1);
        }
    } //end if rolling force

    if (species->getTorsionFrictionCoefficient() != 0.0)
    {
        if (species->getTorsionStiffness() != 0.0)
        {
            ///\todo TW: Why do we not use the corrected diameter here, as in the rolling case? And check if Stefan uses radius or diameter
            Mdouble effectiveDiameter = 2.0*getEffectiveRadius();

            //From Luding2008, spin velocity (eq 16) w/o 2.0!
            Vec3D torsionRelativeVelocity = effectiveDiameter * Vec3D::dot(getNormal(), getP()->getAngularVelocity() - getI()->getAngularVelocity()) * getNormal();

            //Integrate the spring
            torsionSpringVelocity_= torsionRelativeVelocity;
            //integrate(getHandler()->timeStep_);
            torsionSpring_ += Vec3D::dot(torsionSpring_ + torsionSpringVelocity_ * getHandler()->getDPMBase()->getTimeStep(), getNormal()) * getNormal();

            //Calculate test force acting on P including viscous force
            Vec3D torsionForce = - species->getTorsionStiffness() * torsionSpring_ - species->getTorsionDissipation() * torsionRelativeVelocity;

            //tangential forces are modelled by a spring-damper of elasticity kt and viscosity dispt (sticking),
            //but the force is limited by Coulomb friction (torsion):
            Mdouble torsionForceSquared = torsionForce.getLengthSquared();
            if (torsionForceSquared <= mathsFunc::square(species->getTorsionFrictionCoefficientStatic() * getAbsoluteNormalForce()))
            {
                //if sticking (|ft|<=mu*|fn|), apply the force
            }
            else
            {
                //if torsion, resize the tangential force such that |ft|=mu*|fn|
                torsionForce *= species->getTorsionFrictionCoefficient() * getAbsoluteNormalForce() / std::sqrt(torsionForceSquared);
                //resize the tangential spring accordingly such ft=-k*delta-nu*relVel
                torsionSpring_ = -(torsionForce + species->getTorsionDissipation() * torsionRelativeVelocity) / species->getTorsionStiffness();
            }
            //Add (virtual) rolling force to torque
            addTorque(effectiveDiameter * torsionForce);
        }
        else //if no spring stiffness is set
        {
            std::cerr << "FrictionInteraction::computeFrictionForce(): Torsion stiffness is zero" << std::endl;
            exit(-1);
        }
    } //end if torsion force
}
/*!
 * \param[in] timeStep
 */
void FrictionInteraction::integrate(Mdouble timeStep)
{
    SlidingFrictionInteraction::integrate(timeStep);
    rollingSpring_ += rollingSpringVelocity_ * timeStep;
    torsionSpring_ += Vec3D::dot(torsionSpring_ + torsionSpringVelocity_ * timeStep, getNormal()) * getNormal();
}
/*!
 * \return Mdouble
 */
Mdouble FrictionInteraction::getElasticEnergy() const
{
    return SlidingFrictionInteraction::getElasticEnergy()
        +  0.5 * getSpecies()->getRollingStiffness() * rollingSpring_.getLengthSquared()
        +  0.5 * getSpecies()->getTorsionStiffness() * torsionSpring_.getLengthSquared();
}
/*!
 * \return const FrictionSpecies*
 */
const FrictionSpecies* FrictionInteraction::getSpecies() const
{
    return dynamic_cast<const FrictionSpecies*>(getBaseSpecies());
}
/*!
 * \return std::string
 */
std::string FrictionInteraction::getBaseName() const
{
    return "Friction";
}
/*!
 *
 */
void FrictionInteraction::reverseHistory()
{
    SlidingFrictionInteraction::reverseHistory();
    //rollingSpring_=-rollingSpring_;
    //rollingSpringVelocity_=-rollingSpringVelocity_;
    torsionSpring_=-torsionSpring_;
    torsionSpringVelocity_=-torsionSpringVelocity_;
}
