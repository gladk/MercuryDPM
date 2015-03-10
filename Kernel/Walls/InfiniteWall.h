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

///This is a class defining walls. It defines the 
///interaction of regular walls and periodic walls
///with particles as defined in Particle
///Modifications:

#ifndef INFINITEWALL_H
#define INFINITEWALL_H

#include "BaseWall.h"
#include "Math/Vector.h"

/*!
 * \class InfiniteWall
 * \brief A standard wall is a plane defined  as {x: normal*x=position}, with normal being the outward unit normal vector of the wall. 
 * \details Please note that this wall is infinite and straight.
 *  A particle touches an infinite wall if position-normal*x<=radius.
 * \todo remove access to set Nwalls and wall set etc.. and add function add_wall. This would have stoped a lot of bugs in my own (Anthony's) driver codes.
 */

class InfiniteWall : public BaseWall
{
public:

    /*!
     * \brief Default constructor, the normal is infinitely long.
     */
    InfiniteWall();

    /*!
     * \brief Copy constructor, copy the given wall.
     */
    InfiniteWall(const InfiniteWall& w);

    /*!
     * \brief Default destructor.
     */
    virtual ~InfiniteWall();

    /*!
     * \brief Wall copy method. It calls the copy constructor of this Wall, useful for polymorphism
     */
    InfiniteWall* copy() const override;

    /*!
     * \brief Empty function, but is used for polymorphism.
     */
    void clear();

    /*!
     * \brief Defines a standard wall, given an outward normal vector s.t. normal*x=normal*point for all x of the wall.
     */
    void set(Vec3D normal, Vec3D point);

    /*!
     * \brief Changes the normal of the InfiniteWall.
     */
    void setNormal(const Vec3D normal);

    /*!
     * \brief Defines a standard wall by computing normal*position = point and using the overloaded function set(Vec3D, vec3D).
     * \deprecated We will go to the new interface, namely set(Vec3D, Vec3D).
     */
    MERCURY_DEPRECATED
    void set(Vec3D normal, Mdouble position);

    using BaseWall::move;
    
    /*!
     * \brief Move the wall to a new position by giving the new position in the direction of the unit normal vector.
     * \deprecated We will go the new interface, namely move(Vec3D) that is 
     * implemented in BaseInteractable.
     */
    MERCURY_DEPRECATED
    void move(Mdouble position);

    /*!
     * \brief Returns the distance of the wall to the particle.
     */
    Mdouble getDistance(const Vec3D& otherPosition) const;

    /*!
     * \brief Compute the distance from the wall for a given BaseParticle and return if there is a collision. If there is a collision, also return the normal vector.
     */
    bool getDistanceAndNormal(const BaseParticle& p, Mdouble& distance, Vec3D& normal_return) const override;

    /*!
     * \brief Reads InfiniteWall from a restart file.
     */
    void read(std::istream& is);

    /*!
     * \brief Reads InfiniteWall from an old-style restart file.
     */
    void oldRead(std::istream& is);

    /*!
     * \brief Writes the InfiniteWall to an output stream, usually a restart file.
     */
    void write(std::ostream& os) const;

    /*!
     * \brief Returns the name of the object, in this case the string "InfiniteWall".
     */
    virtual std::string getName() const;

    /*!
     * \brief Access function for normal.
     */
    Vec3D getNormal() const;

    /*!
     * \brief Look up the interaction between this wall and a BaseParticle at a certain timeStamp.
     */
    BaseInteraction* getInteractionWith(BaseParticle* p, Mdouble timeStamp, InteractionHandler* interactionHandler);

private:
    /*!
     * Outward normal vector. This does not have to be a unit vector.
     */
    Vec3D normal_;

    /*!
     * This is the factor to rescale to unit vectors.
     * \todo{TW: it is unnecessary to store the factor; remove?}
     * IFCD: It seems to not be unnecessary if someone defined a normal with non-unit
     * length and wants to move the wall.
     */
    Mdouble factor_;
};

#endif
