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

#ifndef INTERSECTIONOFWALLS_H
#define INTERSECTIONOFWALLS_H

#include <vector>
#include "BaseWall.h"
#include "InfiniteWall.h"
#include "Math/Vector.h"

/*!
 * \class IntersectionOfWalls
 * \brief A finite wall is convex polygon defined by a set of normals normal_i and positions position_i. A particle touches a finite wall if position_i-normal_i*x<=radius for all i.
 * \image html Walls/finiteWall.jpg
 */

class IntersectionOfWalls : public BaseWall
{
public:
    /*!
     * \brief
     */
    IntersectionOfWalls();

    /*!
     * \brief Wall copy method. It calls the copy contrustor of this Wall, usefull for polymorfism
     */
    virtual IntersectionOfWalls* copy() const;

    /*!
     * \brief
     */
    void clear();

    /*!
     * \brief Adds a wall to the set of finite walls, given an outward normal vector s.t. normal*x=normal*point
     */
    void addObject(Vec3D normal, Vec3D point);

    /*!
     * \brief Adds a wall to the set of finite walls, given an outward normal vector s. t. normal*x=position
     */
    void addObject(Vec3D normal_, Mdouble position_);

    /*!
     * \brief
     */
    void createOpenPrism(std::vector<Vec3D> Points, Vec3D PrismAxis);

    /*!
     * \brief
     */
    void createPrism(std::vector<Vec3D> Points, Vec3D PrismAxis);

    /*!
     * \brief
     */
    void createOpenPrism(std::vector<Vec3D> Points);

    /*!
     * \brief
     */
    void createPrism(std::vector<Vec3D> Points);

    /*!
     * \brief Since this function should be called before calculating any Particle-Wall interactions, it can also be used to set the normal vector in case of curved walls.
     */
    bool getDistanceAndNormal(const BaseParticle& P, Mdouble& distance, Vec3D& normal_return) const;

    /*!
     * \brief
     */
    bool getDistanceAndNormal(const Vec3D& postition, Mdouble wallInteractionRadius, Mdouble &distance, Vec3D &normal_return) const;

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
     * \brief
     */
    BaseInteraction* getInteractionWith(BaseParticle *P, Mdouble timeStamp, InteractionHandler* interactionHandler);

private:
    std::vector<InfiniteWall> objects_; ///
    std::vector<Vec3D> A_; ///A[n*(n-1)/2+m] is a point on the intersecting line between walls m and n, m<n
    std::vector<Vec3D> AB_; ///AB[n*(n-1)/2+m] is the direction of the intersecting line between walls m and n, m<n
    std::vector<Vec3D> C_; ///C[(n-2)*(n-1)*n/6+(m-1)*m/2+l] is a point intersecting walls l, m and n, l<m<n
};

#endif
