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
     * \brief
     * \param[in]
     */
    void move(const Vec3D& move);
    
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
