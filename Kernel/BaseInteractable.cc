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

#include "BaseInteractable.h"
#include "Particles/BaseParticle.h"
#include "Interactions/BaseInteraction.h"
#include "Species/BaseSpecies.h"

///\todo TW: why do some constructors (e.g. BaseInteractable, BaseParticle)not explicitly call the constructor from the inherited class?
BaseInteractable::BaseInteractable()
{
    position_.setZero();
    orientation_.setZero();
    velocity_.setZero();
    angularVelocity_.setZero();
    force_.setZero();
    torque_.setZero();
    indSpecies_ = 0;
    species_ = nullptr;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseInteractable::BaseInteractable() finished"<<std::endl;
#endif
}

BaseInteractable::BaseInteractable(const BaseInteractable &p)
        : BaseObject(p)
{
    //interactions_=p.interactions_;//Interactions are not copied by default
    position_ = p.position_;
    orientation_ = p.orientation_;
    velocity_ = p.velocity_;
    angularVelocity_ = p.angularVelocity_;
    force_ = p.force_;
    torque_ = p.torque_;
    species_ = p.species_;
    indSpecies_ = p.indSpecies_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseInteractable::BaseInteractable(const BaseInteractable &p finished"<<std::endl;
#endif
}

BaseInteractable::~BaseInteractable()
{
    //std::cout<<"Deleting BaseInteractable with index="<<getIndex()<<" and id="<<getId()<<" size="<<interactions_.size()<<std::endl;
    while (interactions_.size() > 0)
    {
        interactions_.front()->removeFromHandler();
    }
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseInteractable::~BaseInteractable() finished"<<std::endl;
#endif
}

unsigned int BaseInteractable::getIndSpecies() const
{
    return indSpecies_;
}

void BaseInteractable::setIndSpecies(unsigned int indSpecies)
{
    indSpecies_=indSpecies;
}

const BaseSpecies* BaseInteractable::getSpecies() const
{
    return species_;
}

void BaseInteractable::setSpecies(const BaseSpecies* species)
{
    species_ = species;
    indSpecies_ = species->getIndex();
}

const Vec3D& BaseInteractable::getForce() const
{
    return force_;
}

const Vec3D& BaseInteractable::getTorque() const
{
    return torque_;
}

void BaseInteractable::setForce(Vec3D _new)
{
    force_ = _new;
}

void BaseInteractable::setTorque(Vec3D _new)
{
    torque_ = _new;
}

void BaseInteractable::addForce(Vec3D _new)
{
    force_ += _new;
}

void BaseInteractable::addTorque(Vec3D _new)
{
    torque_ += _new;
}

const Vec3D& BaseInteractable::getPosition() const
{
    return position_;
}

const Vec3D& BaseInteractable::getOrientation() const
{
    return orientation_;
}

void BaseInteractable::setPosition(const Vec3D& position)
{
    position_ = position;
}
void BaseInteractable::setOrientation(const Vec3D& orientation)
{
    orientation_ = orientation;
}
void BaseInteractable::move(const Vec3D& move)
{
    position_ += move;
}
void BaseInteractable::rotate(const Vec3D& rotate)
{
    orientation_ += rotate;
}

void BaseInteractable::read(std::istream& is)
{
    BaseObject::read(is);
    std::string dummy;
    is >> dummy >> indSpecies_;
    is >> dummy >> position_;
    is >> dummy >> orientation_ >> dummy;
    is >> dummy >> velocity_;
    is >> dummy >> angularVelocity_ >> dummy;
    is >> dummy >> force_;
    is >> dummy >> torque_;
}

void BaseInteractable::write(std::ostream& os) const
{
    BaseObject::write(os);
    os << " indSpecies " << indSpecies_
        << " position " << position_
        << " orientation " << orientation_ << " " << 0.0
        << " velocity " << velocity_
        << " angularVelocity " << angularVelocity_ << " " << 0.0
            << " force " << force_
            << " torque " << torque_;
}

const std::list<BaseInteraction*>& BaseInteractable::getInteractions() const
{
    return interactions_;
}

void BaseInteractable::addInteraction(BaseInteraction* I)
{
    interactions_.push_back(I);
}

bool BaseInteractable::removeInteraction(BaseInteraction* I)
{
    for (std::list<BaseInteraction*>::iterator it=interactions_.begin(); it != interactions_.end(); ++it)
    {
        if (I==(*it))
        {
            //std::cout<<"Removing interaction from "<<getId()<<" between "<<I->getI()->getId()<<" and "<<I->getP()->getId()<<std::endl;
            interactions_.erase(it);
            return true;
        }
    }
    std::cerr << "Error in BaseInteractable::removeInteraction: Interaction could not be removed" << std::endl;
    return false;
}

///access function for velocity
const Vec3D& BaseInteractable::getVelocity() const
{
    return velocity_;
}

///access function for velocity
const Vec3D& BaseInteractable::getAngularVelocity() const
{
    return angularVelocity_;
}

void BaseInteractable::setVelocity(const Vec3D& velocity)
{
    velocity_=velocity;
}
void BaseInteractable::setAngularVelocity(const Vec3D& angularVelocity)
{
    angularVelocity_ = angularVelocity;
}
void BaseInteractable::addVelocity(const Vec3D& velocity)
{
    velocity_+=velocity;
}
void BaseInteractable::addAngularVelocity(const Vec3D& angularVelocity)
{
    angularVelocity_ += angularVelocity;
}
const Vec3D BaseInteractable::getVelocityAtContact(const Vec3D& contact) const
{
    return getVelocity() - Vec3D::cross(contact - getPosition(), getAngularVelocity());
}

void BaseInteractable::copyInteractionsForPeriodicParticles(const BaseInteractable &p)
{
    for (std::list<BaseInteraction*>::const_iterator it=p.interactions_.begin(); it != p.interactions_.end(); ++it)
    {
        (*it)->copySwitchPointer(&p,this);
    }
}
