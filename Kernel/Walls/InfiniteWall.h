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

///This is a class defining walls. It defines the 
///interaction of regular walls and periodic walls
///with particles as defined in Particle
///Modifications:

#ifndef INFINITEWALL_H
#define INFINITEWALL_H

/*!
 * \class InfiniteWall
 * \brief A standard wall is a plane defined  as {x: normal*x=position}, with normal being the outward unit normal vector of the wall. A particle touches a standard wall if position-normal*x<=radius.
 * \todo remove access to set Nwalls and wall set etc.. and add function add_wall. This would have stoped a lot of bugs in my own (Anthony's) driver codes.
 */

#include "BaseWall.h"
#include "Math/Vector.h"

class InfiniteWall : public BaseWall
{
public:

    /*!
     * \brief
     */
    InfiniteWall();

    /*!
     * \brief
     */
    InfiniteWall(const InfiniteWall &p);

    /*!
     * \brief
     */
    virtual ~InfiniteWall();

    /*!
     * \brief Wall copy method. It calls the copy constructor of this Wall, usefull for polymorfism
     */
    InfiniteWall* copy() const;

    /*!
     * \brief
     */
    void clear();

    /*!
     * \brief Adds a wall to the set of infinite walls, given an outward normal vector s.t. normal*x=normal*point
     */
    void set(Vec3D normal, Vec3D point);

    /*!
     * \brief Adds a wall to the set of infinite walls, given an outward normal vector s.t. normal*x=normal*point
     */
    void setNormal(const Vec3D normal);

    /*!
     * \brief Defines a standard wall, given an outward normal vector s. t. normal*x=position
     */
    void set(Vec3D normal, Mdouble position);

    /*!
     * \brief Allows the wall to be moved to a new position
     */
    void move(Mdouble position);

    /*!
     * \brief Returns the distance of the wall to the particle.
     */
    Mdouble getDistance(const Vec3D &Position) const;

    /*!
     * \brief Since this function should be called before calculating any Particle-Wall interactions, it can also be used to set the normal vector in case of curved walls.
     */
    bool getDistanceAndNormal(const BaseParticle& P, Mdouble& distance, Vec3D& normal_return) const;

    /*!
     * \brief reads wall
     */
    void read(std::istream& is);

    /*!
     * \brief
     */
    void oldRead(std::istream& is);

    /*!
     * \brief outputs wall
     */
    void write(std::ostream& os) const;

    /*!
     * \brief Returns the name of the object
     */
    virtual std::string getName() const;

    /*!
     * \brief access function for normal
     */
    Vec3D getNormal() const;

    /*!
     * \brief
     */
    BaseInteraction* getInteractionWith(BaseParticle *P, Mdouble timeStamp, InteractionHandler* interactionHandler);

private:
    Vec3D normal_; ///<outward unit normal vector
    Mdouble factor_; ///This is the normal to rescale to unit vectors.
};

#endif
