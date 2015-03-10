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
 * \details Sets all vectors to zero and all point to nullptr.
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
 * \details Note, this does not copy interactions of a interactable. As these 
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
 * \details Removes all the interactions from the interactable.
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

/*!
 * \details This set the species associated with this interactable. 
 *          This function should not be used and BaseInteractable::setSpecies
 *          should be used instead.
 *          See also BaseInteractable::setSpecies
 */
MERCURY_DEPRECATED void BaseInteractable::setIndSpecies(unsigned int indSpecies)
{
    indSpecies_ = indSpecies;
}

/*!
 * \details This function return a ParticleSpecies* for the current interacable.
 *          Please note, this is a ParticleSpecies; not, a BaseSpecies as 
 *          interactables must have physically properties as well.
 * \return  constant ParticleSpecies* pointer to the species storing the physical
 *          properties of this interactable. 
 */
const ParticleSpecies* BaseInteractable::getSpecies() const
{
    return species_;
}

/*!
 *  \details This function sets the species associated with this interactable 
 *           object. Again this must be a ParticleSpecies. Note, it also 
 *           automatically sets the index on the indSpecies_ by working up the 
 *           correct index. However, index should be carefully used 
 *  \param[in] species  ParticleSpcies pointer which is species holding the 
 *                      physical properties.
 */
void BaseInteractable::setSpecies(const ParticleSpecies* species)
{
    species_ = species;
    indSpecies_ = species->getIndex();
}

/*!
 * \details Return the current force being to the BaseInteractable.
 *          Note, the code works by first computing the forces of each
 *          interaction and then it loops over all BaseInteracables applying
 *          forces to them from the interactions they are involved in.
 * \return  const Vec3D reference that is the total force applied to this 
 *          interactable.
 */
const Vec3D& BaseInteractable::getForce() const
{
    return force_;
}

/*!
 * \details Return the current torque being to the BaseInteractable.
 *          Note, the code works by first computing the forces of each
 *          interaction and then it loops over all BaseInteracables applying
 *          forces to them from the interactions they are involved in.
 * \return  const Vec3D reference that is the total force applied to this 
 *          interactable.
 */
const Vec3D& BaseInteractable::getTorque() const
{
    return torque_;
}

/*!
 * \details This sets the force being applied to this interactable.
 *          Note, first the code computes all forces in the interactions and 
 *          then loops over all interactable objects applying the forces from
 *          the interactions to the interactables involved in the interaction.
 * \param[in]   force   Vec3D which is the force to be applied.
 */
void BaseInteractable::setForce(Vec3D force)
{
    force_ = force;
}

/*!
 * \details This sets the torque being applied to this interactable.
 *          Note, first the code computes all force/torques in the interactions
 *          and then loops over all interactable objects applying the torques 
 *          from the interactions to the interactables involved in the 
 *          interaction.
 * \param[in]   torque   Vec3D which is the force to be applied.
 */
void BaseInteractable::setTorque(Vec3D torque)
{
    torque_ = torque;
}

/*!
 * \details Incremental version of BaseInteractable::setForce. 
 *          Also see BaseInteraction::setForce for were this is used.
 * \param[in]   addForce    Vec3D incremental force which is added to the total
 *              force of the interactable.
 */
void BaseInteractable::addForce(Vec3D addForce)
{
    force_ += addForce;
}

/*!
 * \details Incremental version of BaseInteractable::setTorque. 
 *          Also see BaseInteraction::setTorque for were this is used.
 * \param[in]   addTorque    Vec3D incremental force which is added to the total
 *              torque of the interactable.
 */
void BaseInteractable::addTorque(Vec3D addTorque)
{
    torque_ += addTorque;
}

/*!
 * \details Returns the reference to a Vec3D which contains the position of the
 *          interactionable.
 *          Please note the interpretation of this depends on which 
 *          interactable. For particles this is the centre of the particle;
 *          where for walls it is one point of the wall given \f$r.n=p\f$
 * \return  Returns a reference to a Vec3D returns the position of the 
 *          interactable.
 */
const Vec3D& BaseInteractable::getPosition() const
{
    return position_;
}

/*!
 * \details Returns the reference to a Vec3D which contains the orientation of 
 *          the
 *          interactionable.
 *          Please note the interpretation of this depends on which 
 *          interactable. Please see derived objects for details.
 *          \todo This is not full implemented and is related to quaterians.
 * \return  Returns a reference to a Vec3D returns the position of the 
 *          interactable.
 */
const Vec3D& BaseInteractable::getOrientation() const
{
    return orientation_;
}

/*!
 * \details Sets the positions of the interactable. 
 *          interpretation depends on which interactable is being considered
 *          See also BaseInteractable::getPosistion.
 * \param[in] position  Reference to Vec3D storing the position of the particle.
 */
void BaseInteractable::setPosition(const Vec3D& position)
{
    position_ = position;
}


/*!
 * \details Sets the orientation of the interactable. 
 *          interpretation depends on which interactable is being considered
 *          See also BaseInteractable::getOrientation.
 * \param[in] orientation  Reference to Vec3D storing the orientation
 *            of the particle.
 */
void BaseInteractable::setOrientation(const Vec3D& orientation)
{
    orientation_ = orientation;
}

/*!
 * \details Moves (displaces) the interacable a given distance. 
 *          Note, this just updates the position by the move.
 * \param[in] move  Reference to Vec3D which is the distance to move the 
 *            interactable.
 */
void BaseInteractable::move(const Vec3D& move)
{
    position_ += move;
}

/*!
 * \details Rotates the interacable a given solid angle. 
 *          Note, this just updates the orientation by the angle.
 * \param[in] rotate  Reference to Vec3D which is the solid angle through which 
 *            the interactable is rotated.
 */
void BaseInteractable::rotate(const Vec3D& rotate)
{
    orientation_ += rotate;
}

/*!
 * \details BaseInteacatable read functions. Reads in all the information about 
 *          an interacatable.
 *          Note, this can be from any istream but would normally be a file
 *          See also BaseInteractable::write
 * \param[in] is     std::istream to which the information is read from.
 */
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

/*!
 * \details BaseInteractable write function. Writes out all the information 
 *          required to recreate this interactable. 
 *          To write this interactable to the screen call write(std::cout).
 *          See also BaseInteractable::read
 * \param[in] os     std::ostream to which the information is written. Note, is
 *                   any ostream is can be file or screen.
 */
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

/*!
 * \details Returns a list of interactions which belong to this interactable.
 * \return  Returns an std::list of pointers to all the interactions which this
 *          interacable is involved in.
 */
const std::list<BaseInteraction*>& BaseInteractable::getInteractions() const
{
    return interactions_;
}

/*!
 * \details Added a new interactions to the current interactable. 
 * \param[in] I Pointer to the new interaction which is to be added to the list
 *            of interactions of this interactable.
 */
void BaseInteractable::addInteraction(BaseInteraction* I)
{
    interactions_.push_back(I);
}

/*!
 * \details Removes a given interaction form the list of interactions belonging 
 *          to the current interacable. 
 *          This functions returns true to the interaction was found and returns
 *          false if the given interaction did not exist and the interaction was
 *          not removed.
 * \param[in]   I BaseInteraction pointer which is the interaction to be removed.
 * \return bool True if the interaction was found and removed; false if the 
 *              interaction did not exist for that interactable.
 */
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

/*!
 * \details Returns the velocity of the BaseInteractbale. 
 *          Note, this is the same for all BaseInteractables; it is the 
 *          direction the object is moving in.
 * \return  Vec3D reference which contains the current velocity of the current
 *          BaseInteractable.
 */
const Vec3D& BaseInteractable::getVelocity() const
{
    return velocity_;
}

/*!
 * \details Returns the angular velocity of the BaseInteractbale. 
 *          Note, this is the same for all BaseInteractables; it is the 
 *          direction the object is moving in.
 * \return  Vec3D reference which contains the current angular velocity of the 
 *          current BaseInteractable.
 */
const Vec3D& BaseInteractable::getAngularVelocity() const
{
    return angularVelocity_;
}

/*!
 *  \details See also BaseInteractable::getVelocity
 *  \param[in]  velocity Vec3D which is the velocity of the interactable. 
 */
void BaseInteractable::setVelocity(const Vec3D& velocity)
{
    velocity_ = velocity;
}

/*!
 * \details See also BaseInteractable::getAngularVelocity
 * \param[in]   angularVelocity  Vec3D which is the angularVelocity of the 
 *              interactable.
 */
void BaseInteractable::setAngularVelocity(const Vec3D& angularVelocity)
{
    angularVelocity_ = angularVelocity;
}

/*!
 * \details See also BaseInteractable::setVelocity
 * \param[in] velocity Vec3D containing the velocity increment which to increase
 *            the velocity by.
 */
void BaseInteractable::addVelocity(const Vec3D& velocity)
{
    velocity_ += velocity;
}

/*!
 * \details See also BaseInteractable::setAngularVelocity
 * \param[in] angularVelocity increment which to increase the angularVelocity
 *            by.
 */
void BaseInteractable::addAngularVelocity(const Vec3D& angularVelocity)
{
    angularVelocity_ += angularVelocity;
}
const Vec3D BaseInteractable::getVelocityAtContact(const Vec3D& contact) const
        {
    return getVelocity() - Vec3D::cross(contact - getPosition(), getAngularVelocity());
}

/*!
 * \details This loops over all interactions of periodic (particle) and calls
 *          copySwitchPointer, which copies the interactions.
 * \param[in] pOriginal  Reference to the BaseInteractable which is to be copied 
 *            to create the ghost particles.
 */
void BaseInteractable::copyInteractionsForPeriodicParticles(const BaseInteractable &pOriginal)
{
    for (std::list<BaseInteraction*>::const_iterator it = pOriginal.interactions_.begin(); it != pOriginal.interactions_.end(); ++it)
    {
        //So here this is the ghost and it is the interaction of the ghost/
        (*it)->copySwitchPointer(&pOriginal, this);
    }
}

/*!
 * \details This functions is used to give an interactable a prescribed motion,
 *          which is defined by a std::function. 
 *          This is the new moving walls interface.
 *          A demo of the use would be:
 *          setPrescribedPosition([this] (double time)
                {
                    return Vec3D(getXMin(),0.0,shaker_amp * 
 *                      std::sin(t * 2.0 * shaker_freq * constants::pi));
                }
            );
 *          This example moves the wall sinusoidally with time.
 * \param[in] prescribedPosition std::function which is the lambda function and
 *            takes a double the time and returns a Vec 3D which is the
 *            position of the interactable for that time.
 *          See also BaseInteractable::setPrescribedVelocity for more
 *          information.
 */
void BaseInteractable::setPrescribedPosition(std::function<Vec3D (double)> prescribedPosition)
{
    prescribedPosition_ = prescribedPosition;
}

/*!
 * \details This calls the  prescribedPosition function if one has been defined.
 *          See also BaseInteractable::setPrescribedPosition
 * \param[in] time  double which is the current time of the simulation.
 */
void BaseInteractable::applyPrescribedPosition(double time)
{
    if(prescribedPosition_)
    {
        setPosition(prescribedPosition_(time));
    }
}

/*!
 * \details In a similar manner to BaseInteractable::setPrescribedPosition this
 *          sets the velocity of the BaseInteactable.
 *          See also BaseInteractable::setPrescribedPosition
 *          Note, it is valid to set both the velocity and the position. No
 *          checking if these are consist is done at all.
 *          If you only set one of these function the other is automatically
 *          calculated using a by numerically differentiating or integrating 
 *          the functions will is prescribed.
 * \param[in] prescribedVelocity std::function which is the lambda function and
 *            takes a double the time and returns a Vec 3D which is the 
 *            velocity of the intertable for that time.
 */
void BaseInteractable::setPrescribedVelocity(std::function<Vec3D (double)> prescribedVelocity)
{
    prescribedVelocity_ = prescribedVelocity;
}

/*!
 * \details This calls the prescribedVeclocity function if one has been defined.
 *          See also BaseInteractable::setPrescribedVelocity
 * \param[in] time  double which is the current time of the simulation.
 */
void BaseInteractable::applyPrescribedVelocity(double time)
{
    if(prescribedVelocity_)
    {
        setVelocity(prescribedVelocity_(time));
    }
}

/*!
 * \details This is similar to the BaseInteractable::setPrescribedPosition and  
 *          works the same way.
 *          See BaseInteractable::setPrescibedPosition and 
 *          BaseInteractable::setPrescribedVelocity for more details how it
 *          works.
 *          Note, the rate of change of the orientation can also be set using
 *          the function BaseInteractable::setPrescribedAngularVelocity.
 * \param[in] prescribedOrientation std::function which is the lambda function and 
 *            takes a double the time and returns a Vec 3D which is the orientation 
 *            of the interactable for that time.
 */
void BaseInteractable::setPrescribedOrientation(std::function<Vec3D (double)> prescribedOrientation)
{
    prescribedOrientation_ = prescribedOrientation;
}

/*!
 * \details This calls the prescribedOrientation function if one has been 
 *          defined.
 *          See also BaseInteractable::setPrescribedOrientation
 * \param[in] time  double which is the current time of the simulation.
 */
void BaseInteractable::applyPrescribedOrientation(double time)
{
    if(prescribedOrientation_)
    {
        setOrientation(prescribedOrientation_(time));
    }
}

/*
 * \details This functions sets a std::function which specifies the 
 *          AngularVeclocity of an interactable. This function takes a double 
 *          the time and returns a Vec3D which is the new angular velocity.
 *          See also BaseInteractable::setPrescribedOrientation
 *          Note this functions works the same way as 
 *          BaseInteractable::setPosition and BaseInteractable::setVeclocity.
 */
void BaseInteractable::setPrescribedAngularVelocity(std::function<Vec3D (double)> prescribedAngularVelocity)
{
    prescribedAngularVelocity_ = prescribedAngularVelocity;
}

/*!
 * \details This calls the prescribedAngularVelocity function if one has been 
 *          defined.
 *          See also BaseInteractable::setPrescribedAngularVelocity
 * \param[in] time  double which is the current time of the simulation.
 */
void BaseInteractable::applyPrescribedAngularVelocity(double time)
{
    if(prescribedAngularVelocity_)
    {
        setAngularVelocity(prescribedAngularVelocity_(time));
    }
}

/*!
 * \details This does not first part of verlet integration but for objects with
 *          an infinite mass i.e. there motion is prescribed and not calculated 
 *          from the applied forces.
 *          First it deals with the translation degrees of freedom and then 
 *          secondly if deals with the angular degrees of freedom.
 *          Note, in both cases if the user has prescribed both positions and
 *          velocity these are used. If only only position is prescribed the 
 *          velocity is computed from a finite difference. If only the velocity
 *          is prescribed the position is computed from integrating the 
 *          velocity.
 * 
 *          In the weird case they neither is set. The objects computed velocity
 *          is used to update its position.
 *          \param[in] time double which is the current simulation time
 *          \param[in] timeStep double which is the current delta time of the
 *                     simulation i.e. the size of each time step.
 */
void BaseInteractable::integrateBeforeForceComputation(double time, double timeStep)
{
    if (prescribedPosition_ )
    {
        if (prescribedVelocity_ )
        {
            //Both the velocity and position are defined; as we are using leap
            //frog method so the velocity is evaluated half a time later.
            applyPrescribedPosition(time + timeStep);
            applyPrescribedVelocity(time + 0.5 * timeStep);
        }
        else
        {
            //Only the position is defined.
            //Velocity is evaluated from a finite different of the Position
            //Note, we use 0.5 +- 0.1 timeStep for the velocity eval.
            applyPrescribedPosition(time + timeStep);
            setVelocity((prescribedPosition_(time + 0.6 * timeStep) - prescribedPosition_(time + 0.4 * timeStep)) / (0.2 * timeStep));
        }
    }
    else
    {
        if (prescribedVelocity_ )
        {
            //Only the velocity is set. The position is calculated from the
            //the integral of velocity.
            applyPrescribedVelocity(time + 0.5 * timeStep);
            move(getVelocity() * timeStep);
        }
        else
        {
            //Neither is set move based on the computed velocity of the object.
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
        if (prescribedAngularVelocity_)
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

/*!
 * \details This is the last part of time integration for interactable objects 
 * which have an infinite mass.
 *  \param[in] time double which is the current simulation time
 *  \param[in] timeStep double which is the current delta time of the
 *                     simulation i.e. the size of each time step.
 */
void BaseInteractable::integrateAfterForceComputation(double time, double timeStep)
{
    if (prescribedPosition_)
    {
        if (prescribedVelocity_)
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
        if (prescribedVelocity_)
        {
            applyPrescribedVelocity(time + 0.5 * timeStep);
        }
    }
    if (prescribedOrientation_)
    {
        if (prescribedAngularVelocity_)
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
        if (prescribedAngularVelocity_)
        {
            applyPrescribedAngularVelocity(time + 0.5 * timeStep);
        }
    }
}
