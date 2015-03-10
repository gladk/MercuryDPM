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

#include "Interactions/SlidingFrictionInteraction.h"
#include "Species/SlidingFrictionSpecies.h"
#include "Particles/BaseParticle.h"
#include "InteractionHandler.h"
#include <iomanip>
#include <fstream>

SlidingFrictionInteraction::SlidingFrictionInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
    : BaseInteraction(P, I, timeStamp)
{
    slidingSpring_.setZero();
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"SlidingFrictionInteraction::SlidingFrictionInteraction() finished"<<std::endl;
#endif
}

SlidingFrictionInteraction::SlidingFrictionInteraction(const SlidingFrictionInteraction &p)
    : BaseInteraction(p)
{
    slidingSpring_=p.slidingSpring_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"SlidingFrictionInteraction::SlidingFrictionInteraction(const SlidingFrictionInteraction &p finished"<<std::endl;
#endif
}

SlidingFrictionInteraction::~SlidingFrictionInteraction()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"SlidingFrictionInteraction::~SlidingFrictionInteraction() finished"<<std::endl;
#endif
}

void SlidingFrictionInteraction::write(std::ostream& os) const
{
    //BaseInteraction::write(os);
    os << " slidingSpring " << slidingSpring_;
}

void SlidingFrictionInteraction::read(std::istream& is)
{
    //BaseInteraction::read(is);
    std::string dummy;
    is >> dummy >> slidingSpring_;
}

void SlidingFrictionInteraction::computeForce()
{
    //If tangential forces are absent
    if (getAbsoluteNormalForce() == 0.0) return;

    const SlidingFrictionSpecies* species = getSpecies();
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
            slidingSpring_ += slidingSpringVelocity_ * getHandler()->timeStep_;

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

void SlidingFrictionInteraction::integrate(Mdouble timeStep)
{
    slidingSpring_ += slidingSpringVelocity_ * timeStep;
}

Mdouble SlidingFrictionInteraction::getElasticEnergy() const
{
    return 0.5 * getSpecies()->getSlidingStiffness() * slidingSpring_.getLengthSquared();
}

Mdouble SlidingFrictionInteraction::getTangentialOverlap() const
{
    ///\todo TWnow this should be positive
    return -slidingSpring_.getLength();
}

const Vec3D SlidingFrictionInteraction::getTangentialForce() const
{
    return tangentialForce_;
}

const SlidingFrictionSpecies* SlidingFrictionInteraction::getSpecies() const
{
    return dynamic_cast<const SlidingFrictionSpecies*>(getBaseSpecies());
}

std::string SlidingFrictionInteraction::getName() const
{
    return "SlidingFriction";
}

void SlidingFrictionInteraction::reverseHistory()
{
    slidingSpring_=-slidingSpring_;
    slidingSpringVelocity_=-slidingSpringVelocity_;
    tangentialForce_=-tangentialForce_;
}
