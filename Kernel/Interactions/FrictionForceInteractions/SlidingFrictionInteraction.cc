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


#include "SlidingFrictionInteraction.h"
#include "Species/FrictionForceSpecies/SlidingFrictionSpecies.h"
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
SlidingFrictionInteraction::SlidingFrictionInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
    : BaseInteraction(P, I, timeStamp)
{
    slidingSpring_.setZero();
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"SlidingFrictionInteraction::SlidingFrictionInteraction() finished"<<std::endl;
#endif
}
/*!
 * \param[in] p
 */
SlidingFrictionInteraction::SlidingFrictionInteraction(const SlidingFrictionInteraction& p)
    : BaseInteraction(p)
{
    slidingSpring_=p.slidingSpring_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"SlidingFrictionInteraction::SlidingFrictionInteraction(const SlidingFrictionInteraction& p) finished"<<std::endl;
#endif
}
/*!
 *
 */
SlidingFrictionInteraction::~SlidingFrictionInteraction()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout<<"SlidingFrictionInteraction::~SlidingFrictionInteraction() finished"<<std::endl;
#endif
}
/*!
 * \param[in,out] os
 */
void SlidingFrictionInteraction::write(std::ostream& os) const
{
    //BaseInteraction::write(os);
    os << " slidingSpring " << slidingSpring_;
}
/*!
 * \param[in,out] is
 */
void SlidingFrictionInteraction::read(std::istream& is)
{
    //BaseInteraction::read(is);
    std::string dummy;
    is >> dummy >> slidingSpring_;
}
/*!
 *
 */
void SlidingFrictionInteraction::computeFrictionForce()
{
    //If tangential forces are absent
    if (getAbsoluteNormalForce() == 0.0) return;

    const SlidingFrictionSpecies* species = getSpecies();//dynamic_cast
    
    if (species->getSlidingFrictionCoefficient() != 0.0)
    {
        //Compute the tangential component of relativeVelocity_
        Vec3D tangentialRelativeVelocity = getRelativeVelocity() - getNormal() * getNormalRelativeVelocity();

        if (species->getSlidingStiffness() != 0.0)
        {
            //used to Integrate the spring
            if (dynamic_cast<BaseParticle*>(getI())==0)  //if particle-wall
                slidingSpringVelocity_= tangentialRelativeVelocity;
            else //if particle-particle
                slidingSpringVelocity_= (tangentialRelativeVelocity - Vec3D::dot(slidingSpring_, getP()->getVelocity() - getI()->getVelocity()) * getNormal() / getDistance());

            //integrate(getHandler()->timeStep_);
            slidingSpring_ += slidingSpringVelocity_ * getHandler()->getDPMBase()->getTimeStep();

            //Calculate test force acting on P including viscous force
            tangentialForce_ = - species->getSlidingStiffness() * slidingSpring_ - species->getSlidingDissipation() * tangentialRelativeVelocity;

            //tangential forces are modelled by a spring-damper of elasticity kt and viscosity dispt (sticking),
            //but the force is limited by Coulomb friction (sliding):
            Mdouble tangentialForceSquared = tangentialForce_.getLengthSquared();
            if (tangentialForceSquared <= mathsFunc::square(species->getSlidingFrictionCoefficientStatic() * getAbsoluteNormalForce()))
            {
                //if sticking (|ft|<=mu*|fn|), apply the force
                addForce(tangentialForce_);
            }
            else
            {
                //if sliding, resize the tangential force such that |ft|=mu*|fn|
                tangentialForce_ *= species->getSlidingFrictionCoefficient() * getAbsoluteNormalForce() / std::sqrt(tangentialForceSquared);
                addForce(tangentialForce_);
                //resize the tangential spring accordingly such ft=-k*delta-nu*relVel
                slidingSpring_ = -(tangentialForce_ + species->getSlidingDissipation() * tangentialRelativeVelocity) / species->getSlidingStiffness();
            }
        }
        else //if no spring stiffness is set
        {
//            if (species->getSlidingDissipation()==0.0)
//            {
//                std::cerr << "SlidingFrictionInteraction::getForce(): warning:  both sliding stiffness and dissipation are zero" << std::endl;
//            }
            Mdouble tangentialRelativeVelocitySquared = tangentialRelativeVelocity.getLengthSquared();
            if (tangentialRelativeVelocitySquared * mathsFunc::square(species->getSlidingDissipation()) <= mathsFunc::square(species->getSlidingFrictionCoefficientStatic() * getAbsoluteNormalForce()))
                tangentialForce_=-species->getSlidingDissipation() * tangentialRelativeVelocity;
            else //if sliding, set force to Coulomb limit
                tangentialForce_=-(species->getSlidingFrictionCoefficient() * getAbsoluteNormalForce() / std::sqrt(tangentialRelativeVelocitySquared)) * tangentialRelativeVelocity;

            addForce(tangentialForce_);
        }
    }
//    else
//    {
//        std::cerr << "SlidingFrictionInteraction::getForce(): warning: sliding friction is zero" << std::endl;
//    }
}
/*!
 * \param[in] timeStep
 */
void SlidingFrictionInteraction::integrate(Mdouble timeStep)
{
    slidingSpring_ += slidingSpringVelocity_ * timeStep;
}
/*!
 * \return Mdouble
 */
Mdouble SlidingFrictionInteraction::getElasticEnergy() const
{
    return 0.5 * getSpecies()->getSlidingStiffness() * slidingSpring_.getLengthSquared();
}
/*!
 * \return Mdouble
 */
Mdouble SlidingFrictionInteraction::getTangentialOverlap() const
{
    ///\todo TWnow this should be positive
    return -slidingSpring_.getLength();
}
/*!
 * \return const Vec3D
 */
const Vec3D SlidingFrictionInteraction::getTangentialForce() const
{
    return tangentialForce_;
}
/*!
 * \return const SlidingFrictionSpecies*
 */
const SlidingFrictionSpecies* SlidingFrictionInteraction::getSpecies() const
{
    return dynamic_cast<const SlidingFrictionSpecies*>(getBaseSpecies());
}
/*!
 * \return std::string
 */
std::string SlidingFrictionInteraction::getBaseName() const
{
    return "SlidingFriction";
}
/*!
 *
 */
void SlidingFrictionInteraction::reverseHistory()
{
    slidingSpring_=-slidingSpring_;
    slidingSpringVelocity_=-slidingSpringVelocity_;
    tangentialForce_=-tangentialForce_;
}
