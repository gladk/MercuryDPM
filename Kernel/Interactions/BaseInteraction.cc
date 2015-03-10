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

#include "BaseInteraction.h"
#include "InteractionHandler.h"
#include "Particles/BaseParticle.h"
#include "Species/BaseSpecies.h"
#include "DPMBase.h"
#include<iomanip>
#include<fstream>

BaseInteraction::BaseInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
    : BaseObject()
{
    P_ = P;
    P->addInteraction(this);
    I_ = I;
    I->addInteraction(this);
    normal_.setZero();
    overlap_ = 0;
    timeStamp_ = timeStamp;
    species_ = 0;
    force_.setZero();
    torque_.setZero();
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseInteraction::BaseInteraction() finished"<<std::endl;
#endif
}

BaseInteraction::BaseInteraction(const BaseInteraction &p)
    : BaseObject(p)
{
    P_ = p.P_;
    I_ = p.I_;
    normal_ = p.normal_;
    overlap_ = p.overlap_;
    force_ = p.force_;
    torque_ = p.torque_;
    species_ = p.species_;
    timeStamp_ = p.timeStamp_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseInteraction::BaseInteraction(const BaseInteraction &p finished"<<std::endl;
#endif
}

BaseInteraction::~BaseInteraction()
{
    //std::cout<<"Removing interaction with index="<<getIndex()<<" id="<<getId()<<std::endl;
    P_->removeInteraction(this);
    I_->removeInteraction(this);
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseInteraction::~BaseInteraction() finished"<<std::endl;
#endif
}

///BaseParticle print function, which accepts an os std::stringstream as input. It prints human readable BaseParticle information to the std::stringstream
void BaseInteraction::write(std::ostream& os) const
        {
    os << "Interaction" << " particleIds " << P_->getId() << " " << I_->getId() <<" timeStamp "<<timeStamp_<< " force " << force_ << " torque " << torque_;
}

void BaseInteraction::read(std::istream& is)
{
    unsigned int id1, id2;
    std::string dummy;
    is >> dummy >> id1 >> dummy >> id2 >> dummy >> force_ >> dummy >> torque_;
    P_->setId(id1);
    I_->setId(id2);
}

std::string BaseInteraction::getName() const
{
    return "BaseInteraction";
}

void BaseInteraction::setNormal(Vec3D normal)
{
    normal_ = normal;
}

void BaseInteraction::setDistance(Mdouble distance)
{
    distance_ = distance;
}

void BaseInteraction::setOverlap(Mdouble overlap)
{
    overlap_ = overlap;
}

void BaseInteraction::setContactPoint(Vec3D contactPoint)
{
    contactPoint_ = contactPoint;
}

void BaseInteraction::setTimeStamp(Mdouble timeStamp)
{
    timeStamp_ = timeStamp;
}

void BaseInteraction::setHandler(InteractionHandler* handler)
{
    handler_ = handler;
}

InteractionHandler* BaseInteraction::getHandler() const
{
    return handler_;
}

void BaseInteraction::removeFromHandler()
{
    handler_->removeObject(getIndex());
}

void BaseInteraction::copySwitchPointer(const BaseInteractable* from, BaseInteractable* to) const
{
    BaseInteraction* C = copy();
    if (C->getP() == from)
    {
        C->reverseHistory();
        C->P_ =C->getI();
    }
    C->I_ = to;

    C->getP()->addInteraction(C);
    C->getI()->addInteraction(C);
    handler_->addObject(C);
}

const Vec3D& BaseInteraction::getForce() const
{
    return force_;
}
const Vec3D& BaseInteraction::getTorque() const
{
    return torque_;
}

const Vec3D& BaseInteraction::getNormal() const
{
    return normal_;
}

const Vec3D& BaseInteraction::getContactPoint() const
{
    return contactPoint_;
}

Mdouble BaseInteraction::getOverlap() const
{
    return overlap_;
}

BaseInteractable* BaseInteraction::getP()
{
    return P_;
}

BaseInteractable* BaseInteraction::getI()
{
    return I_;
}

const BaseInteractable* BaseInteraction::getP() const
{
    return P_;
}

const BaseInteractable* BaseInteraction::getI() const
{
    return I_;
}

Mdouble BaseInteraction::getTimeStamp() const
{
    return timeStamp_;
}

void BaseInteraction::integrate(Mdouble timeStep UNUSED)
{

}

void BaseInteraction::setSpecies(BaseSpecies *species)
{
    species_ = species;
}

void BaseInteraction::setP(BaseInteractable* P)
{
    P_->removeInteraction(this);
    P_=P;
    P_->addInteraction(this);
}
void BaseInteraction::setI(BaseInteractable* I)
{
    I_->removeInteraction(this);
    I_=I;
    I_->addInteraction(this);
}

void BaseInteraction::writeToFStat(std::ostream& os) const
{
    BaseParticle* IParticle = dynamic_cast<BaseParticle*>(I_);
    BaseParticle* PParticle = dynamic_cast<BaseParticle*>(P_);

    Vec3D tangentialForce = getTangentialForce();
    Mdouble tangentialOverlap = getTangentialOverlap();

    Mdouble scalarNormalForce = Vec3D::dot(force_, getNormal());
    Mdouble scalarTangentialForce = tangentialForce.getLength();
    Vec3D tangential;
    if (scalarTangentialForce!=0.0)
        tangential = tangentialForce/scalarTangentialForce;
    else
        tangential = Vec3D(0.0,0.0,0.0);

    ///\todo TW centre is used just for backward compatibility; replace centre by contact law; we also have to correct it in StatisticsVector::gatherContactStatistics.
    ///There also seems to be an issue with the normal being defined differently for walls
    Vec3D centre;
    if (IParticle!=0)
        centre = 0.5 * (getP()->getPosition() + getI()->getPosition());
    else
        centre = getP()->getPosition() - normal_ * (PParticle->getRadius() - overlap_);

    if (PParticle!=0 && !PParticle->isFixed())
    {
        os << timeStamp_
            << " " << P_->getIndex()
            << " " << static_cast<int>((IParticle==0?(-I_->getIndex()-1):I_->getIndex()))
            << " " << centre
            << " " << getOverlap()
            << " " << tangentialOverlap
            << " " << scalarNormalForce
            << " " << scalarTangentialForce
            << " " << (IParticle==0?-normal_:normal_)
            << " " << (IParticle==0?-tangential:tangential) << std::endl;
        ///\todo the flip in normal/tangential direction for walls should not be done; this is an old bug
    }
    if (IParticle!=0 && !IParticle->isFixed() && IParticle->getPeriodicFromParticle()==0)
    {
        os << timeStamp_
            << " " << I_->getIndex()
            << " " << P_->getIndex()
            << " " << centre
            << " " << getOverlap()
            << " " << tangentialOverlap
            << " " << scalarNormalForce
            << " " << scalarTangentialForce
            << " " << -normal_
            << " " << -tangential << std::endl;
    }

}

Mdouble BaseInteraction::getDistance() const
{
    return distance_;
}

Mdouble BaseInteraction::getTangentialOverlap() const
{
    return 0;
}

const Vec3D BaseInteraction::getTangentialForce() const
{
    return Vec3D(0.0,0.0,0.0);
}

const Vec3D& BaseInteraction::getRelativeVelocity() const
{
    return relativeVelocity_;
}

Mdouble BaseInteraction::getNormalRelativeVelocity() const
{
    return normalRelativeVelocity_;
}

Mdouble BaseInteraction::getAbsoluteNormalForce() const
{
    return absoluteNormalForce_;
}

void BaseInteraction::addForce(Vec3D force)
{
    force_+=force;
}

void BaseInteraction::addTorque(Vec3D torque)
{
    torque_+=torque;
}

void BaseInteraction::setForce(Vec3D force)
{
    force_=force;
}

void BaseInteraction::setTorque(Vec3D torque)
{
    torque_=torque;
}

void BaseInteraction::setRelativeVelocity(Vec3D relativeVelocity)
{
    relativeVelocity_=relativeVelocity;
}

void BaseInteraction::setNormalRelativeVelocity(Mdouble normalRelativeVelocity)
{
    normalRelativeVelocity_=normalRelativeVelocity;
}
void BaseInteraction::setAbsoluteNormalForce(Mdouble absoluteNormalForce)
{
    absoluteNormalForce_ = absoluteNormalForce;
}

const BaseSpecies* BaseInteraction::getBaseSpecies() const
{
    return species_;
}

BaseInteraction* BaseInteraction::copy() const
{
    return new BaseInteraction(*this);
}

void BaseInteraction::computeForce()
{}

Mdouble BaseInteraction::getElasticEnergy() const
{
    return 0.0;
}

void BaseInteraction::reverseHistory()
{
}

Mdouble BaseInteraction::getEffectiveRadius()
{
    BaseParticle* PParticle = dynamic_cast<BaseParticle*>(getP());
    BaseParticle* IParticle = dynamic_cast<BaseParticle*>(getI());
    if (PParticle==nullptr)
    {
        std::cerr << "BaseInteraction::getEffectiveCorrectedRadius(): first interactable P is not a particle" << std::endl;
        exit(-1);
    }
    //Compute the reduced diameter
    if (IParticle==nullptr) //if particle-wall
    {
        return PParticle->getRadius();
    }
    else
    {
        Mdouble radiusP = PParticle->getRadius();
        Mdouble radiusI = IParticle->getRadius();
        return radiusP * radiusI / (radiusP + radiusI);
    }
}

Mdouble BaseInteraction::getEffectiveCorrectedRadius()
{
    BaseParticle* PParticle = dynamic_cast<BaseParticle*>(getP());
    BaseParticle* IParticle = dynamic_cast<BaseParticle*>(getI());
    if (PParticle==nullptr)
    {
        std::cerr << "BaseInteraction::getEffectiveCorrectedRadius(): first interactable P is not a particle" << std::endl;
        exit(-1);
    }
    //Compute the reduced diameter
    if (IParticle==nullptr) //if particle-wall
    {
        return PParticle->getRadius() - 0.5*getOverlap();
    }
    else
    {
        Mdouble radiusP = PParticle->getRadius() - 0.5*getOverlap();
        Mdouble radiusI = IParticle->getRadius() - 0.5*getOverlap();
        return radiusP * radiusI / (radiusP + radiusI);
    }
}

void BaseInteraction::gatherContactStatistics()
{
    BaseParticle* IParticle = dynamic_cast<BaseParticle*>(I_);
    BaseParticle* PParticle = dynamic_cast<BaseParticle*>(P_);

    Vec3D tangentialForce = getTangentialForce();
    Mdouble tangentialOverlap = getTangentialOverlap();

    Mdouble scalarNormalForce = Vec3D::dot(force_, getNormal());
    Mdouble scalarTangentialForce = tangentialForce.getLength();
    Vec3D tangential;
    if (scalarTangentialForce!=0.0)
        tangential = tangentialForce/scalarTangentialForce;
    else
        tangential = Vec3D(0.0,0.0,0.0);

    ///\todo TW centre is used just for backward compatibility; replace centre by contact law; we also have to correct it in StatisticsVector::gatherContactStatistics.
    ///There also seems to be an issue with the normal being defined differently for walls
    Vec3D centre;
    if (IParticle!=0)
        centre = 0.5 * (getP()->getPosition() + getI()->getPosition());
    else
        centre = getP()->getPosition() - normal_ * (PParticle->getRadius() - overlap_);

    if (PParticle!=0 && !PParticle->isFixed())
    {
        getHandler()->getDPMBase()->gatherContactStatistics(
            P_->getIndex(),
            static_cast<int>((IParticle==0?(-I_->getIndex()-1):I_->getIndex())),
            centre,
            getOverlap(),
            tangentialOverlap,
            scalarNormalForce,
            scalarTangentialForce,
            (IParticle==0?-normal_:normal_),
            (IParticle==0?-tangential:tangential));
    }
    if (IParticle!=0 && !IParticle->isFixed() && IParticle->getPeriodicFromParticle()==0)
    {
        getHandler()->getDPMBase()->gatherContactStatistics(
            I_->getIndex(),
            P_->getIndex(),
            centre,
            getOverlap(),
            tangentialOverlap,
            scalarNormalForce,
            scalarTangentialForce,
            -normal_,
            -tangential);

    }
}