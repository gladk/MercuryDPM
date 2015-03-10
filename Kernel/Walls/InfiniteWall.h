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
