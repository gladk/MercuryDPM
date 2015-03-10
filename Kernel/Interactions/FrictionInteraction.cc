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

#include "Interactions/FrictionInteraction.h"
#include "Species/FrictionSpecies.h"
#include "Particles/BaseParticle.h"
#include "InteractionHandler.h"
#include <iomanip>
#include <fstream>

FrictionInteraction::FrictionInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
    : BaseInteraction(P, I, timeStamp), SlidingFrictionInteraction(P, I, timeStamp)
{
    rollingSpring_.setZero();
    torsionSpring_.setZero();
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"FrictionInteraction::FrictionInteraction() finished"<<std::endl;
#endif
}

FrictionInteraction::FrictionInteraction(const FrictionInteraction &p)
    : BaseInteraction(p), SlidingFrictionInteraction(p)
{
    rollingSpring_=p.rollingSpring_;
    torsionSpring_=p.torsionSpring_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"FrictionInteraction::FrictionInteraction(const FrictionInteraction &p finished"<<std::endl;
#endif
}

FrictionInteraction::~FrictionInteraction()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"FrictionInteraction::~FrictionInteraction() finished"<<std::endl;
#endif
}

void FrictionInteraction::write(std::ostream& os) const
{
    SlidingFrictionInteraction::write(os);
    os << " rollingSpring " << rollingSpring_;
    os << " torsionSpring " << torsionSpring_;
}

void FrictionInteraction::read(std::istream& is)
{
    SlidingFrictionInteraction::read(is);
    std::string dummy;
    is >> dummy >> rollingSpring_;
    is >> dummy >> torsionSpring_;
}

void FrictionInteraction::computeForce()
{
    SlidingFrictionInteraction::computeForce();

    const FrictionSpecies* species = getSpecies();
    //If tangential forces are present
    if (getAbsoluteNormalForce() == 0.0) return;

    Mdouble reducedDiameter;
    BaseParticle* PParticle = dynamic_cast<BaseParticle*>(getP());
    BaseParticle* IParticle = dynamic_cast<BaseParticle*>(getI());
    if (PParticle==nullptr)
    {
        std::cerr << "FrictionInteraction::computeForce(): first interactable P is not a particle" << std::endl;
        exit(-1);
    }

    if (species->getRollingFrictionCoefficient() != 0.0)
    {
        if (species->getRollingStiffness() != 0.0)
        {
            //Compute the reduced diameter
            if (IParticle==nullptr) //if particle-wall
            {
                reducedDiameter = 2.0 * PParticle->getRadius() - getOverlap();
            }
            else
            {
                Mdouble diameterP = 2.0 * PParticle->getRadius() - getOverlap();
                Mdouble diameterI = 2.0 * IParticle->getRadius() - getOverlap();
                reducedDiameter = diameterP * diameterI / (diameterP + diameterI);
            }

            //From Luding2008, objective rolling velocity (eq 15) w/o 2.0!
            Vec3D rollingRelativeVelocity = -reducedDiameter * Vec3D::cross(getNormal(), getP()->getAngularVelocity() - getI()->getAngularVelocity());

            //used to Integrate the spring
            rollingSpringVelocity_= rollingRelativeVelocity;
            //integrate(getHandler()->timeStep_);
            rollingSpring_ += rollingSpringVelocity_ * getHandler()->timeStep_;

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
            addTorque(reducedDiameter * Vec3D::cross(getNormal(), rollingForce));
        }
        else //if no spring stiffness is set
        {
            std::cerr << "FrictionInteraction::computeForce(): Rolling stiffness is zero" << std::endl;
            exit(-1);
        }
    } //end if rolling force

    if (species->getTorsionFrictionCoefficient() != 0.0)
    {
        if (species->getTorsionStiffness() != 0.0)
        {
            ///\todo TW: Why do we not use the corrected diameter here, as in the rolling case?
            //Compute the reduced diameter
            if (IParticle==0) //if particle-wall
            {
                reducedDiameter = 2.0 * PParticle->getRadius();
            }
            else
            {
                Mdouble diameterP = 2.0 * PParticle->getRadius();
                Mdouble diameterI = 2.0 * IParticle->getRadius();
                reducedDiameter = diameterP * diameterI / (diameterP + diameterI);
            }

            //From Luding2008, spin velocity (eq 16) w/o 2.0!
            Vec3D torsionRelativeVelocity =reducedDiameter * Vec3D::dot(getNormal(), getP()->getAngularVelocity() - getI()->getAngularVelocity()) * getNormal();

            //Integrate the spring
            torsionSpringVelocity_= torsionRelativeVelocity;
            //integrate(getHandler()->timeStep_);
            torsionSpring_ += Vec3D::dot(torsionSpring_ + torsionSpringVelocity_ * getHandler()->timeStep_, getNormal()) * getNormal();

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
            addTorque(reducedDiameter * torsionForce);
        }
        else //if no spring stiffness is set
        {
            std::cerr << "FrictionInteraction::computeForce(): Torsion stiffness is zero" << std::endl;
            exit(-1);
        }
    } //end if torsion force
}

void FrictionInteraction::integrate(Mdouble timeStep)
{
    SlidingFrictionInteraction::integrate(timeStep);
    rollingSpring_ += rollingSpringVelocity_ * timeStep;
    torsionSpring_ += Vec3D::dot(torsionSpring_ + torsionSpringVelocity_ * timeStep, getNormal()) * getNormal();
}

Mdouble FrictionInteraction::getElasticEnergy() const
{
    return SlidingFrictionInteraction::getElasticEnergy()
        +  0.5 * getSpecies()->getRollingStiffness() * rollingSpring_.getLengthSquared()
        +  0.5 * getSpecies()->getTorsionStiffness() * torsionSpring_.getLengthSquared();
}

const FrictionSpecies* FrictionInteraction::getSpecies() const
{
    return dynamic_cast<const FrictionSpecies*>(getBaseSpecies());
}

std::string FrictionInteraction::getName() const
{
    return "Friction";
}

void FrictionInteraction::reverseHistory()
{
    SlidingFrictionInteraction::reverseHistory();
    //rollingSpring_=-rollingSpring_;
    //rollingSpringVelocity_=-rollingSpringVelocity_;
    torsionSpring_=-torsionSpring_;
    torsionSpringVelocity_=-torsionSpringVelocity_;
}
