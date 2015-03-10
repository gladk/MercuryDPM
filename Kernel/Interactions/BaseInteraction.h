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

    void gatherContactStatistics();

    const Vec3D& getRelativeVelocity() const;
    Mdouble getNormalRelativeVelocity() const;
    Mdouble getAbsoluteNormalForce() const;
    virtual const Vec3D getTangentialForce() const;

    Mdouble getEffectiveRadius() const;
    Mdouble getEffectiveCorrectedRadius();

protected:

    //functions that only the derived Interactions have to know about:
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
