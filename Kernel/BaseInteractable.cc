//Copyright (c) 2013-2015, The MercuryDPM Developers Team. All rights reserved.
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


#include "BaseInteractable.h"
#include "Particles/BaseParticle.h"
#include "Interactions/BaseInteraction.h"
#include "Species/ParticleSpecies.h"

/*!
 * \todo TW: why do some constructors (e.g. BaseInteractable, BaseParticle)not 
 *       explicitly call the constructor from the inherited class?
 * \detilas Sets all vectors to zero and all point to nullptr.
 */

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
    prescribedPosition_ = nullptr;
	prescribedVelocity_ = nullptr;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseInteractable::BaseInteractable() finished"<<std::endl;
#endif
}

/*!
 * \detials Note, this does not copy interactions of a interactable. As these 
 *          often require extra work. 
 *          All the other properties are copied normally. Please use this copy
 *          with care.
 */
BaseInteractable::BaseInteractable(const BaseInteractable &p)
        : BaseObject(p)
{
    interactions_.clear();
    position_ = p.position_;
    orientation_ = p.orientation_;
    velocity_ = p.velocity_;
    angularVelocity_ = p.angularVelocity_;
    force_ = p.force_;
    torque_ = p.torque_;
    species_ = p.species_;
    indSpecies_ = p.indSpecies_;
    prescribedPosition_ = p.prescribedPosition_;
    prescribedVelocity_ = p.prescribedVelocity_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseInteractable::BaseInteractable(const BaseInteractable &p finished"<<std::endl;
#endif
}

/*!
 * \detials Removes all the interactions from the interactable.
 */
BaseInteractable::~BaseInteractable()
{
    //std::cout<<"Deleting BaseInteractable with index="<<getIndex()<<" and id="<<getId()<<" size="<<interactions_.size()<<std::endl;
    while (interactions_.size() > 0)
    {
        interactions_.front()->removeFromHandler();
    }
#ifdef DEBUG_DESTRUCTOR
    std::cout<<"BaseInteractable::~BaseInteractable() finished"<<std::endl;
#endif
}

/*!
 * \details Returns the unsigned int to the index of the species associated with
 *          the interactable object.
 * \return  Unsigned int which is the unique index of the species 
 */
unsigned int BaseInteractable::getIndSpecies() const
{
    return indSpecies_;
}

void BaseInteractable::setIndSpecies(unsigned int indSpecies)
{
    indSpecies_ = indSpecies;
}

const ParticleSpecies* BaseInteractable::getSpecies() const
{
    return species_;
}

void BaseInteractable::setSpecies(const ParticleSpecies* species)
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

void BaseInteractable::setForce(Vec3D force)
{
    force_ = force;
}

void BaseInteractable::setTorque(Vec3D torque)
{
    torque_ = torque;
}

void BaseInteractable::addForce(Vec3D addForce)
{
    force_ += addForce;
}

void BaseInteractable::addTorque(Vec3D addTorque)
{
    torque_ += addTorque;
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
    for (std::list<BaseInteraction*>::iterator it = interactions_.begin(); it != interactions_.end(); ++it)
    {
        if (I == (*it))
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
    velocity_ = velocity;
}
void BaseInteractable::setAngularVelocity(const Vec3D& angularVelocity)
{
    angularVelocity_ = angularVelocity;
}
void BaseInteractable::addVelocity(const Vec3D& velocity)
{
    velocity_ += velocity;
}
void BaseInteractable::addAngularVelocity(const Vec3D& angularVelocity)
{
    angularVelocity_ += angularVelocity;
}
const Vec3D BaseInteractable::getVelocityAtContact(const Vec3D& contact) const
        {
    return getVelocity() - Vec3D::cross(contact - getPosition(), getAngularVelocity());
}

/*!
 * \details This loops over all interactions of periodic (particle) and calls copySwitchPointer, which copies the interactions.
 */
void BaseInteractable::copyInteractionsForPeriodicParticles(const BaseInteractable &pOriginal)
{
    for (std::list<BaseInteraction*>::const_iterator it = pOriginal.interactions_.begin(); it != pOriginal.interactions_.end(); ++it)
    {
        //So here this is the ghost and it is the interaction of the ghost/
        (*it)->copySwitchPointer(&pOriginal, this);
    }
}

void BaseInteractable::setPrescribedPosition(std::function<Vec3D (double)> prescribedPosition)
{
    prescribedPosition_ = prescribedPosition;
}

void BaseInteractable::applyPrescribedPosition(double time)
{
    if(prescribedPosition_!=nullptr)
    {
        setPosition(prescribedPosition_(time));
    }
}

void BaseInteractable::setPrescribedVelocity(std::function<Vec3D (double)> prescribedVelocity)
{
    prescribedVelocity_ = prescribedVelocity;
}

void BaseInteractable::applyPrescribedVelocity(double time)
{
    if(prescribedVelocity_!=nullptr)
    {
        setVelocity(prescribedVelocity_(time));
    }
}

void BaseInteractable::setPrescribedOrientation(std::function<Vec3D (double)> prescribedOrientation)
{
    prescribedOrientation_ = prescribedOrientation;
}

void BaseInteractable::applyPrescribedOrientation(double time)
{
    if(prescribedOrientation_!=nullptr)
    {
        setOrientation(prescribedOrientation_(time));
    }
}

void BaseInteractable::setPrescribedAngularVelocity(std::function<Vec3D (double)> prescribedAngularVelocity)
{
    prescribedAngularVelocity_ = prescribedAngularVelocity;
}

void BaseInteractable::applyPrescribedAngularVelocity(double time)
{
    if(prescribedAngularVelocity_!=nullptr)
    {
        setAngularVelocity(prescribedAngularVelocity_(time));
    }
}

void BaseInteractable::integrateBeforeForceComputation(double time, double timeStep)
{
    if (prescribedPosition_ != nullptr)
    {
        if (prescribedVelocity_ != nullptr)
        {
            applyPrescribedPosition(time + timeStep);
            applyPrescribedVelocity(time + 0.5 * timeStep);
        }
        else
        {
            applyPrescribedPosition(time + timeStep);
            setVelocity((prescribedPosition_(time + 0.6 * timeStep) - prescribedPosition_(time + 0.4 * timeStep)) / (0.2 * timeStep));
        }
    }
    else
    {
        if (prescribedVelocity_ != nullptr)
        {
            applyPrescribedVelocity(time + 0.5 * timeStep);
            move(getVelocity() * timeStep);
        }
        else
        {
            move(getVelocity() * timeStep);
        }
    }
    if (prescribedOrientation_ != nullptr)
    {
        if (prescribedAngularVelocity_ != nullptr)
        {
            applyPrescribedOrientation(time + timeStep);
            applyPrescribedAngularVelocity(time + 0.5 * timeStep);
        }
        else
        {
            applyPrescribedOrientation(time + timeStep);
            setAngularVelocity((prescribedOrientation_(time + 0.6 * timeStep) - prescribedOrientation_(time + 0.4 * timeStep)) / (0.2 * timeStep));
        }
    }
    else
    {
        if (prescribedAngularVelocity_ != nullptr)
        {
            applyPrescribedAngularVelocity(time + 0.5 * timeStep);
            rotate(getAngularVelocity() * timeStep);
        }
        else
        {
            rotate(getAngularVelocity() * timeStep);
        }
    }
}

void BaseInteractable::integrateAfterForceComputation(double time, double timeStep)
{
    if (prescribedPosition_ != nullptr)
    {
        if (prescribedVelocity_ != nullptr)
        {
            applyPrescribedVelocity(time + timeStep);
        }
        else
        {
            setVelocity((prescribedPosition_(time + 1.1 * timeStep) - prescribedPosition_(time + 0.9 * timeStep)) / (0.2 * timeStep));
        }
    }
    else
    {
        if (prescribedVelocity_ != nullptr)
        {
            applyPrescribedVelocity(time + 0.5 * timeStep);
        }
    }
    if (prescribedOrientation_ != nullptr)
    {
        if (prescribedAngularVelocity_ != nullptr)
        {
            applyPrescribedAngularVelocity(time + timeStep);
        }
        else
        {
            setAngularVelocity((prescribedOrientation_(time + 1.1 * timeStep) - prescribedOrientation_(time + 0.9 * timeStep)) / (0.2 * timeStep));
        }
    }
    else
    {
        if (prescribedAngularVelocity_ != nullptr)
        {
            applyPrescribedAngularVelocity(time + 0.5 * timeStep);
        }
    }
}
