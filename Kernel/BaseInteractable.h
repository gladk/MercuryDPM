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

#ifndef BASEINTERACTABLE_H
#define BASEINTERACTABLE_H

#include <list>
#include <functional>

#include "BaseObject.h"
#include "Math/Vector.h"

class BaseParticle;
class ParticleSpecies;
class BaseInteraction;
class InteractionHandler;
/*!
 * \class BaseInteractable
 * \brief   Defines the basic properties that a interactable object can have.
 * \details Inherits from class BaseObject (public)
 *          Also it includes a lot of code to deal with interactable objects
 *          that have a prescibed motion.
 *          Most of the code in here is MercuryDPM internal. The only place an
 *          user will interface with this code is for setting the lambda
 *          functions that prescribe the motion of infinite mass particles.
 *          \todo Check prescribed objects have infinite mass.
 */
class BaseInteractable : public BaseObject
{
public:
    /*!
     * \brief Default BaseInteractable constructor, it simply creates an empty 
     *        BaseInteractable.
     */
    BaseInteractable();

    /*!
     * \brief Copy constructor. It copies the BaseInteractable and all objects
     *        it contains
     */
    BaseInteractable(const BaseInteractable &p);

    /*!
     * \brief Destructor, it simply destructs the BaseInteractable and all the 
     *        objects it contains.
     */
    virtual ~BaseInteractable();

    /*!
     * \brief Reads a BaseInteractable from an input stream.
     */
    virtual void read(std::istream& is) = 0;

    /*!
     * \brief Write a BaseInteractable to an output stream.
     * \param[in] os The output stream to which the BaseInteractable is written.
     */
    virtual void write(std::ostream& os) const = 0;

    /*!
     * \brief Returns the index of the Species of this BaseInteractable.
     */
    unsigned int getIndSpecies() const;

    /*!
     * \brief Sets the index of the Species of this BaseInteractable.
     */
    virtual void setIndSpecies(unsigned int indSpecies);

    /*!
     * \brief Returns a pointer to the species of this BaseInteractable.
     */
    const ParticleSpecies* getSpecies() const;

    /*!
     * \brief Sets the species of this BaseInteractable.
     */
    void setSpecies(const ParticleSpecies* species);

    /*!
     * \brief Returns the force on this BaseInteractable.
     */
    const Vec3D& getForce() const;

    /*!
     * \brief Returns the torque on this BaseInteractable.
     */
    const Vec3D& getTorque() const;

    /*!
     * \brief Sets the force on this BaseInteractable.
     */
    void setForce(Vec3D force);

    /*!
     * \brief Sets the torque on this BaseInteractable.
     */
    void setTorque(Vec3D torque);

    /*!
     * \brief Adds an amount to the force on this BaseInteractable.
     */
    void addForce(Vec3D addForce);

    /*!
     * \brief Adds an amount to the torque on this BaseInteractable.
     */
    void addTorque(Vec3D addTorque);

    /*!
     * \brief Returns the position of this BaseInteractable.
     */
    const Vec3D& getPosition() const;

    /*!
     * \brief Returns the orientation of this BaseInteractable.
     */
    const Vec3D& getOrientation() const;

    /*!
     * \brief Sets the position of this BaseInteractable.
     */
    void setPosition(const Vec3D& position);

    /*!
     * \brief Sets the orientation of this BaseInteractable.
     */
    void setOrientation(const Vec3D& orientation);

    /*!
     * \brief Moves this BaseInteractable by adding an amount to the position.
     */
    virtual void move(const Vec3D& move);

    /*!
     * \brief Rotates this BaseInteractable.
     */
    void rotate(const Vec3D& rotate);

    /*!
     * \brief Returns a reference to the list of interactions in this BaseInteractable.
     */
    const std::list<BaseInteraction*>& getInteractions() const;

    /*!
     * \brief Adds an interaction to this BaseInteractable.
     */
    void addInteraction(BaseInteraction* I);

    /*!
     * \brief Removes an interaction from this BaseInteractable.
     */
    bool removeInteraction(BaseInteraction* I);

    /*!
     * \brief Copies interactions to this BaseInteractable whenever a periodic 
     *        copy made.
     */
    void copyInteractionsForPeriodicParticles(const BaseInteractable &p);

    /*!
     * \brief set the velocity of the BaseInteractable.
     */
    void setVelocity(const Vec3D& velocity);
    
    /*!
     * \brief set the angular velocity of the BaseInteractble.
     */
    void setAngularVelocity(const Vec3D& angularVelocity);

    /*!
     * \brief adds an increment to the velocity.
     */
    void addVelocity(const Vec3D& velocity);

    /*!
     * \brief add an increment to the angular velocity.
     */
    void addAngularVelocity(const Vec3D& angularVelocity);
    
    /*!
     * \brief Returns the velocity of this interactable.
     */
    virtual const Vec3D& getVelocity() const;

    /*!
     * \brief Returns the angular velocity of this interactable.
     */
    virtual const Vec3D& getAngularVelocity() const;

    /*!
     * \brief Allows the position of an infinite mass interactable to be 
     *        prescribed.
     */
    void setPrescribedPosition(std::function<Vec3D (double)> prescribedPosition);

    /*!
     * \brief Computes the position from the user defined prescribed position 
     *        function.
     */
    void applyPrescribedPosition(double time);

    /*!
     * \brief Allows the velocity of an infinite mass interactable to be 
     *        prescribed.
     */
    void setPrescribedVelocity(std::function<Vec3D (double)> prescribedVelocity);

    /*!
     * \brief Computes the velocity from the user defined prescribed velocity
     *        function.
     */
    void applyPrescribedVelocity(double time);

    /*!
     * \brief Allows the orientation of the infinite mass interactbale to be
     *        prescribed. 
     */
    void setPrescribedOrientation(std::function<Vec3D (double)> prescribedOrientation);

    /*!
     * \brief Computes the orientation from the user defined prescribed 
     *        orientation function.
     */
    void applyPrescribedOrientation(double time);

    /*!
     * \brief Allows the angular velocity of the infinite mass interactable to 
     *         be prescribed.
     */
    void setPrescribedAngularVelocity(std::function<Vec3D (double)> prescribedAngularVelocity);

    /*!
     * \brief Computes the angular velocity from the user defined prescribed 
     *        angular velocity.
     */
    void applyPrescribedAngularVelocity(double time);

    /*!
     * \brief Returns the interaction between this object and a given 
     *        BaseParticle
     * \todo TW make sure this function sets normal, distance, overlap, contact point
     * \todo AT why is this a BaseParticle and not a BaseInteratable.
     */
    virtual BaseInteraction* getInteractionWith(BaseParticle *P, Mdouble timeStamp, InteractionHandler* interactionHandler)=0;

    /*!
     * \brief Returns the velocity at the contact point, use by many force laws.
     */
    virtual const Vec3D getVelocityAtContact(const Vec3D& contact) const;

    /*!
     * \brief This is part of integrate routine for objects with infinite mass
     */
    void integrateBeforeForceComputation(double time, double timeStep);

    /*!
     * \brief This is part of the integration routine for objects with infinite
     *        mass.
     */
    void integrateAfterForceComputation(double time, double timeStep);

private:
    /*!
     * User defined function which if set describes the position of the 
     * interactable
     */
    std::function<Vec3D (double)> prescribedPosition_;
    
    /*!
     * User defined function which if set describes the velocity of the
     * interactable.
     */
    std::function<Vec3D (double)> prescribedVelocity_;
    
    /*!
     * User defined function which if set describes the orientation of the 
     * interactable
     */
    std::function<Vec3D (double)> prescribedOrientation_;
    
    /*!
     * User defined functions which if set describes the angular velocity of the
     * interactable.
     */
    std::function<Vec3D (double)> prescribedAngularVelocity_;

    /*!
     * Stores the position of the interactable. Exactly what is stored depends
     * on the type of interactable.
     */
    Vec3D position_;

    /*!
     * Stores the orientation of the interactable.  Exactly what is stored 
     * depends on the type of interatable
     */
    Vec3D orientation_;

    /*!
     * Store the angular velocity of the interactable.
     */
    Vec3D angularVelocity_;

    /*!
     * Stores the force applied to the interactable. 
     */
    Vec3D force_;

    /*!
     * Stores the torque applied to the interactable. 
     */
    Vec3D torque_;

    /*!
     * Point to the ParticlesSpecies which stores density and other material
     * properties of the interactable. 
     */
    const ParticleSpecies* species_;

    /*!
     * Stores the index on the species associated with this interactable.
     */
    unsigned int indSpecies_;

    /*!
     * Stores the velocity of this interactable.
     */
    Vec3D velocity_;

    /*!
     * List of interactions this interactable is involved with. 
     */
    std::list<BaseInteraction*> interactions_;
};
#endif

