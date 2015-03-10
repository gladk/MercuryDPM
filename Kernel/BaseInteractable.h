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

#ifndef BASEINTERACTABLE_H
#define BASEINTERACTABLE_H

#include "BaseObject.h"
#include "Math/Vector.h"
#include <list>

class BaseParticle;
class BaseSpecies;
class BaseInteraction;
class InteractionHandler;
/*!
 * \class BaseInteractable
 * \brief
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
     * \brief 
     * \param[in]
     */
    virtual void read(std::istream& is) = 0;

    /*!
     * \brief
     * \param[in]
     */
    virtual void write(std::ostream& os) const = 0;

    /*!
     * \brief
     * \return
     */
    unsigned int getIndSpecies() const;

   /*!
     * \brief
     * \return
     */
    virtual void setIndSpecies(unsigned int indSpecies);

    /*!
     * \brief
     * \param[in]
     */
    const BaseSpecies* getSpecies() const;

    /*!
      * \brief
      * \param[in]
      */
    void setSpecies(const BaseSpecies* species);

    /*!
     * \brief
     * \return
     */
    const Vec3D& getForce() const;

    /*!
     * \brief
     * \return
     */
    const Vec3D& getTorque() const;

    /*!
     * \brief
     * \param[in]
     */
    void setForce(Vec3D _new);

    /*!
     * \brief
     * \param[in]
     */
    void setTorque(Vec3D _new);

    /*!
     * \brief
     * \param[in]
     */
    void addForce(Vec3D _new);

    /*!
     * \brief
     * \param[in]
     */
    void addTorque(Vec3D _new);

    /*!
     * \brief
     * \return
     */
    const Vec3D& getPosition() const;

    /*!
     * \brief
     * \return
     */
    const Vec3D& getOrientation() const;

    /*!
     * \brief
     * \param[in]
     */
    void setPosition(const Vec3D& position);

    /*!
     * \brief
     * \param[in]
     */
    void setOrientation(const Vec3D& orientation);

    /*!
     * \brief
     * \param[in]
     */
    void move(const Vec3D& move);

    /*!
     * \brief
     * \param[in]
     */
    void rotate(const Vec3D& rotate);

    /*!
     * \brief
     * \param[in]
     * \return
     */
    const std::list<BaseInteraction*>& getInteractions() const;

    /*!
     * \brief
     * \param[in]
     */
    void addInteraction(BaseInteraction* I);

    /*!
     * \brief
     * \param[in]
     * \return
     */
    bool removeInteraction(BaseInteraction* I);

    /*!
     * \brief
     * \param[in]
     */
    void copyInteractionsForPeriodicParticles(const BaseInteractable &p);

    void setVelocity(const Vec3D& velocity);

    void setAngularVelocity(const Vec3D& angularVelocity);

    void addVelocity(const Vec3D& velocity);

    void addAngularVelocity(const Vec3D& angularVelocity);

    virtual const Vec3D& getVelocity() const;

    virtual const Vec3D& getAngularVelocity() const;

    /*!
     * \brief
     * \param[in]
     * \param[out]
     * \todo TW make sure this function sets normal, distance, overlap, contact point
     */
    virtual BaseInteraction* getInteractionWith(BaseParticle *P, Mdouble timeStamp, InteractionHandler* interactionHandler)=0;
    virtual const Vec3D getVelocityAtContact(const Vec3D& contact) const;

private:
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
    const BaseSpecies* species_;

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

