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

#ifndef BASEINTERACTION_H
#define BASEINTERACTION_H

#include "BaseObject.h"
#include "Math/Vector.h"

class InteractionHandler;
class BaseParticle;
class BaseSpecies;
class BaseInteractable;

/*!
 * \class BaseInteraction
 * \brief
 */
class BaseInteraction : public BaseObject
{
public:
    /*!
     * \brief 
     */
    BaseInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);

    /*!
     * \brief 
     */
    BaseInteraction(const BaseInteraction &p);

    /*!
     * \brief destructor
     */
    virtual ~BaseInteraction();

    /*!
     * \brief
     */
    virtual void computeForce();

    /*!
     * \brief Interaction read function, which accepts an std::stringstream as input.
     */
    virtual void read(std::istream& is);

    /*!
     * \brief Interaction print function, which accepts an std::stringstream as input.
     */
    virtual void write(std::ostream& os) const;

    void writeToFStat(std::ostream& os) const;

    /*!
     * \brief
     */
    virtual std::string getName() const;

    virtual Mdouble getElasticEnergy() const;

    void setDistance(Mdouble distance);

    void setNormal(Vec3D normal);

    /*!
     * \brief
     */
    void setOverlap(Mdouble overlap);

    /*!
     * \brief
     */
    void setContactPoint(Vec3D contactPoint);

    /*!
     * \brief
     */
    void setTimeStamp(Mdouble timeStamp);

    /*!
     * \brief
     */
    void setSpecies(BaseSpecies *species);

    /*!
     * \brief
     */
    void setP(BaseInteractable* P);

    /*!
     * \brief
     */
    void setI(BaseInteractable* I);

    /*!
     * \brief
     */
    void setHandler(InteractionHandler* handler);

    InteractionHandler* getHandler() const;

    /*!
     * \brief
     */
    const Vec3D& getForce() const;

    /*!
     * \brief
     */
    const Vec3D& getTorque() const;

    /*!
     * \brief
     */
    const Vec3D& getNormal() const;

    /*!
     * \brief
     */
    const Vec3D& getContactPoint() const;

    /*!
     * \brief
     */
    Mdouble getOverlap() const;

    BaseInteractable* getP();
    BaseInteractable* getI();
    const BaseInteractable* getP() const;
    const BaseInteractable* getI() const;
    Mdouble getTimeStamp() const;
    virtual void integrate(Mdouble timeStep);
    virtual Mdouble getTangentialOverlap() const;
    Mdouble getDistance() const;

    /*!
     * \brief
     */
    void removeFromHandler();

    /*!
      * \brief
      */
    void copySwitchPointer(const BaseInteractable* from, BaseInteractable* to) const;

protected:

    //Variables that only the derived Interactions have to know about:
    const Vec3D& getRelativeVelocity() const;
    Mdouble getNormalRelativeVelocity() const;
    Mdouble getAbsoluteNormalForce() const;
    virtual const Vec3D getTangentialForce() const;

    void addForce(Vec3D force);
    void addTorque(Vec3D torque);

    //these functions are only used for normal forces and should be made private by the normal forces
    void setForce(Vec3D force);
    void setTorque(Vec3D torque);
    void setRelativeVelocity(Vec3D relativeVelocity);
    void setNormalRelativeVelocity(Mdouble normalRelativeVelocit);
    void setAbsoluteNormalForce(Mdouble absoluteNormalForce);

    const BaseSpecies* getBaseSpecies() const;

    virtual BaseInteraction* copy() const;

    void clear();

    virtual void reverseHistory();

private:

    //Variables that are set before any force calculation

    /*!
     * \brief
     */
    InteractionHandler* handler_;

    /*!
     * \brief for particle-particle collision it is assumed BaseInteractable* P has a lower id I.
     */
    BaseInteractable* P_;

    /*!
     * \brief for wall-particle collisions it is assumed that BaseInteractable* I is the wall
     */
    BaseInteractable* I_;
    /*!
     * \brief
     */
    Vec3D contactPoint_;

    //Variables calculated by the normal force routines used in other force routines (but not changed by them)
    Vec3D relativeVelocity_;
    Mdouble normalRelativeVelocity_;
    Mdouble absoluteNormalForce_;
    Mdouble distance_;

    //Variables that the derived interactions have to change
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
    Mdouble timeStamp_;
    /*!
     * \brief
     */
    Vec3D normal_;
    /*!
     * \brief
     */
    Mdouble overlap_;
    ///\todo species should be taken out since DPMBase is now linked as well
    /*!
     * \brief
     */
    BaseSpecies* species_;
};
#endif
