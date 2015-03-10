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
 * \brief Inherits from class BaseObject (public)
 * \details
 */
class BaseInteractable : public BaseObject
{
public:
    /*!
     * \brief Default BaseInteractable constructor, it simply creates an empty BaseInteractable.
     * \details
     */
    BaseInteractable();

    /*!
     * \brief Copy constructor. It copies the BaseInteractable and all objects it contains.
     * \param[in] p the BaseInteractable that has to be copied
     */
    BaseInteractable(const BaseInteractable &p);

    /*!
     * \brief Destructor, it simply destructs the BaseInteractable and all the objects it contains.
     */
    virtual ~BaseInteractable();

    /*!
     * \brief Reads a BaseInteractable from an input stream.
     * \param[in] is The input stream from which the BaseInteractable is read.
     */
    virtual void read(std::istream& is) = 0;

    /*!
     * \brief Write a BaseInteractable to an output stream.
     * \param[in] os The output stream to which the BaseInteractable is written.
     */
    virtual void write(std::ostream& os) const = 0;

    /*!
     * \brief Returns the index of the Species of this BaseInteractable.
     * \return The index of the Species of this BaseInteractable.
     */
    unsigned int getIndSpecies() const;

    /*!
     * \brief Sets the index of the Species of this BaseInteractable.
     * \param[in] indSpecies The index of the species.
     */
    virtual void setIndSpecies(unsigned int indSpecies);

    /*!
     * \brief Returns a pointer to the species of this BaseInteractable.
     * \return A pointer to the species of this BaseInteractable.
     */
    const ParticleSpecies* getSpecies() const;

    /*!
     * \brief Sets the species of this BaseInteractable.
     * \param[in] species A pointer to the species.
     */
    void setSpecies(const ParticleSpecies* species);

    /*!
     * \brief Returns the force on this BaseInteractable.
     * \return A constant reference to the force on this BaseInteractable.
     */
    const Vec3D& getForce() const;

    /*!
     * \brief Returns the torque on this BaseInteractable.
     * \return A constant reference to the torque on this BaseInteractable.
     */
    const Vec3D& getTorque() const;

    /*!
     * \brief Sets the force on this BaseInteractable.
     * \param[in] force The force on this BaseInteractable.
     */
    void setForce(Vec3D force);

    /*!
     * \brief Sets the torque on this BaseInteractable.
     * \param[in] torque The torque on this BaseInteractable.
     */
    void setTorque(Vec3D torque);

    /*!
     * \brief Adds an amount to the force on this BaseInteractable.
     * \param[in] addForce The amount of force added to this BaseInteractable.
     */
    void addForce(Vec3D addForce);

    /*!
     * \brief Adds an amount to the torque on this BaseInteractable.
     * \param[in] addTorque The amount of torque added to this BaseInteractable.
     */
    void addTorque(Vec3D addTorque);

    /*!
     * \brief Returns the position of this BaseInteractable.
     * \return A constant reference to the position of this BaseInteractable.
     */
    const Vec3D& getPosition() const;

    /*!
     * \brief Returns the orientation of this BaseInteractable.
     * \return A constant reference to the orientation of this BaseInteractable.
     */
    const Vec3D& getOrientation() const;

    /*!
     * \brief Sets the position of this BaseInteractable.
     * \param[in] position The position of this BaseInteractable.
     */
    void setPosition(const Vec3D& position);

    /*!
     * \brief Sets the orientation of this BaseInteractable.
     * \param[in] orientation The orientation of this BaseInteractable.
     */
    void setOrientation(const Vec3D& orientation);

    /*!
     * \brief Moves this BaseInteractable by adding an amount to the position.
     * \param[in] move The distance over which this BaseInteractable is moved.
     */
    virtual void move(const Vec3D& move);

    /*!
     * \brief Rotates this BaseInteractable.
     * \param[in] rotate
     */
    void rotate(const Vec3D& rotate);

    /*!
     * \brief Returns a reference to the list of interactions in this BaseInteractable.
     * \return A reference to the list of interactions in this BaseInteractable.
     */
    const std::list<BaseInteraction*>& getInteractions() const;

    /*!
     * \brief Adds an interaction to this BaseInteractable.
     * \param[in] I A pointer to the BaseInteraction which will be added.
     */
    void addInteraction(BaseInteraction* I);

    /*!
     * \brief Removes an interaction from this BaseInteractable.
     * \param[in] I A pointer to the BaseInteraction which will be removed.
     * \return True if successful
     */
    bool removeInteraction(BaseInteraction* I);

    /*!
     * \brief Copies interactions to this BaseInteractable whenever a periodic copy made.
     * \param[in] p A reference to the BaseInteractable from which the BaseInteraction are copied.
     */
    void copyInteractionsForPeriodicParticles(const BaseInteractable &p);

    void setVelocity(const Vec3D& velocity);

    void setAngularVelocity(const Vec3D& angularVelocity);

    void addVelocity(const Vec3D& velocity);

    void addAngularVelocity(const Vec3D& angularVelocity);

    virtual const Vec3D& getVelocity() const;

    virtual const Vec3D& getAngularVelocity() const;

    void setPrescribedPosition(std::function<Vec3D (double)> prescribedPosition);

    void applyPrescribedPosition(double time);

    void setPrescribedVelocity(std::function<Vec3D (double)> prescribedVelocity);

    void applyPrescribedVelocity(double time);

    void setPrescribedOrientation(std::function<Vec3D (double)> prescribedOrientation);

    void applyPrescribedOrientation(double time);

    void setPrescribedAngularVelocity(std::function<Vec3D (double)> prescribedAngularVelocity);

    void applyPrescribedAngularVelocity(double time);

    /*!
     * \brief
     * \param[in]
     * \param[out]
     * \todo TW make sure this function sets normal, distance, overlap, contact point
     */
    virtual BaseInteraction* getInteractionWith(BaseParticle *P, Mdouble timeStamp, InteractionHandler* interactionHandler)=0;

    virtual const Vec3D getVelocityAtContact(const Vec3D& contact) const;

    void integrateBeforeForceComputation(double time, double timeStep);

    void integrateAfterForceComputation(double time, double timeStep);

private:
    /*!
     * \brief
     */
    std::function<Vec3D (double)> prescribedPosition_;
    std::function<Vec3D (double)> prescribedVelocity_;
    std::function<Vec3D (double)> prescribedOrientation_;
    std::function<Vec3D (double)> prescribedAngularVelocity_;

    /*!
     * \brief 
     */
    Vec3D position_;

    /*!
     * \brief 
     */
    Vec3D orientation_;

    /*!
     * \brief
     */
    Vec3D angularVelocity_;

    /*!
     * \brief 
     */
    Vec3D force_;

    /*!
     * \brief 
     */
    Vec3D torque_;

    /*!
     * \brief 
     */
    const ParticleSpecies* species_;

    /*!
     * \brief
     */
    unsigned int indSpecies_;

    /*!
     * \brief
     */
    Vec3D velocity_;

    /*!
     * \brief 
     */
    std::list<BaseInteraction*> interactions_;
};
#endif

