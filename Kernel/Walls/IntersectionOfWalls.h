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
#include "Logger.h"

/*!
 * \brief A IntersectionOfWalls is convex polygon defined as an intersection of InfiniteWall's.
 * \details A IntersectionOfWalls can be defined as the intersection of a set 
 * of InfiniteWalls, defined by the normal vector into the wall and a point on the wall.
 * For an example see Tutorial T8.
 * A particle touches a finite wall if position_i-normal_i*x<=radius for all InfiniteWall's i.
 * 
 * \image html Walls/finiteWall.jpg
 */
class IntersectionOfWalls : public BaseWall
{
public:
    /*!
     * \brief Default constructor.
     */
    IntersectionOfWalls();
    
    /*!
     * \brief Copy constructor.
     */
    IntersectionOfWalls(const IntersectionOfWalls& other);
    
    /*!
     * \brief Destructor.
     */
    virtual ~IntersectionOfWalls();
    
    /*!
     * Copy assignment operator.
     */
    IntersectionOfWalls& operator=(const IntersectionOfWalls& other);

    /*!
     * \brief Wall copy method. It calls the copy constructor of this Wall, useful for polymorphism
     */
    IntersectionOfWalls* copy() const override;

    /*!
     * \brief Removes all parts of the walls.
     * \deprecated Please don't use any clear() anymore, it will be gone soon.
     */
    MERCURY_DEPRECATED
    void clear();

    /*!
     * \brief Adds a wall to the set of infinite walls, given an outward normal vector s.t. normal*x=normal*point
     */
    void addObject(Vec3D normal, Vec3D point);

    /*!
     * \brief Adds a wall to the set of finite walls, given an outward normal vector s. t. normal*x=position
     * \deprecated Don't use this function, instead use the function addObject(Vec3D, Vec3D).
     */
    MERCURY_DEPRECATED
    void addObject(Vec3D normal, Mdouble position);

    /*!
     * \brief Creates an open prism which is a polygon between the points, except the first and last point,  and extends infinitely in the PrismAxis direction.
     */
    void createOpenPrism(std::vector<Vec3D> points, Vec3D prismAxis);

    /*!
     * \brief Creates an open prism which is a polygon between the points and extends infinitely in the PrismAxis direction.
     */
    void createPrism(std::vector<Vec3D> points, Vec3D prismAxis);

    /*!
     * \brief Creates an open prism which is a polygon between the points, except the first and last point, and extends infinitely in the direction perpendicular to the first and second wall.
     */
    void createOpenPrism(std::vector<Vec3D> points);

    /*!
     * \brief Creates an open prism which is a polygon between the points and extends infinitely in the direction perpendicular to the first and second wall.
     */
    void createPrism(std::vector<Vec3D> points);

    /*!
     * \brief Compute the distance from the wall for a given BaseParticle and return if there is a collision. If there is a collision, also return the normal vector.
     */
    bool getDistanceAndNormal(const BaseParticle& p, Mdouble& distance, Vec3D& normal_return) const override;

    /*!
     * \brief Compute the distance from the wall for a given BaseParticle and return if there is an interaction. If there is an interaction, also return the normal vector.
     */
    bool getDistanceAndNormal(const Vec3D& postition, Mdouble wallInteractionRadius, Mdouble& distance, Vec3D& normal_return) const;

    /*!
     * \brief Move the IntersectionOfWalls to a new position, which is a Vec3D from the old position.
     */
    void move(const Vec3D& move);
    
    /*!
     * \brief Reads an IntersectionOfWalls from an input stream, for example a restart file.
     */
    void read(std::istream& is);

    /*!
     * \brief Writes an IntersectionOfWalls to an output stream, for example a restart file.
     */
    void write(std::ostream& os) const;

    /*!
     * \brief Returns the name of the object, here the string "IntersectionOfWalls".
     */
    std::string getName() const override;

    /*!
     * \brief Get the interaction between this IntersectionOfWalls and given BaseParticle at a given time.
     */
    BaseInteraction* getInteractionWith(BaseParticle* p, Mdouble timeStamp, InteractionHandler* interactionHandler);

private:
    /*!
     * \brief The wall "segments"/directions that together make up the finite wall. 
     * \details An intersection of walls exists of a number of infinite walls that
     * are cut of at the intersection points. These InfiniteWall are saved in this
     * vector called iWObjects_.
     */
    std::vector<InfiniteWall> wallObjects_;
    
    /*!
     * \brief A vector that stores a point for each intersecting line between two different InfiniteWall.
     * \details A[n*(n-1)/2+m] is a point on the intersecting line between walls
     *  m and n, m<n.
     */    
    std::vector<Vec3D> A_;
    
    /*!
     * \brief A vector that stores the direction of the intersecting lines between two different InfiniteWall.
     * \details AB[n*(n-1)/2+m] is the direction of the intersecting line between
     *  walls m and n, m<n.
     */
    std::vector<Vec3D> AB_;
    
    /*!
     * \brief A vector that stores the intersection point of three different InfiniteWall.
     * \details C[(n-2)*(n-1)*n/6+(m-1)*m/2+l] is a point intersecting walls 
     * l, m and n, l<m<n
     */
    std::vector<Vec3D> C_;
};

#endif
