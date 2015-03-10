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

#ifndef INFINITEWALLWITHHOLE_H
#define INFINITEWALLWITHHOLE_H

/*!
 * \class InfiniteWallWithHole
 * \brief
 */
#include "BaseWall.h"
#include "Math/Vector.h"

class InfiniteWallWithHole : public BaseWall
{
public:

    /*!
     * \brief default constructor
     */
    InfiniteWallWithHole();
    
    /*!
     * \brief
     */
    InfiniteWallWithHole(Vec3D normal, Mdouble position, Mdouble holeRadius);
    
    /*!
     * \brief
     */
    InfiniteWallWithHole(const InfiniteWallWithHole &p);
    
    /*!
     * \brief Wall copy method. It calls the copy contrustor of this Wall, usefull for polymorfism
     */
    InfiniteWallWithHole* copy() const;

    /*!
     * \brief
     */
    void clear();

    /*!
     * \brief Defines a standard wall, given an outward normal vector s. t. normal*x=position
     */
    void set(Vec3D normal, Mdouble position, Mdouble holeRadius);
    
    /*!
     * \brief Allows the wall to be moved to a new position
     */
    void move(Mdouble position);
    
    /*!
     * \brief Allows the wall to be moved to a new position (also orthogonal to the normal), and setting the velocity
     */
    void move(Vec3D velocity, Mdouble dt);
    
    /*!
     * \brief Allows the wall to be moved with time
     */
    void move_time(Mdouble dt);
    
    /*!
     * \brief Returns the distance of the wall to the particle. 
     */
    Mdouble getWallDistance(const Vec3D &position) const;
    
    /*!
     * \brief
     */
    Mdouble getHoleDistance(const Vec3D &position) const;
    
    /*!
     * \brief Since this function should be called before calculating any Particle-Wall interactions, it can also be used to set the normal vector in case of curved walls.
     */
    bool getDistanceAndNormal(const BaseParticle& P, Mdouble& distance, Vec3D& normal_return) const;

    /*!
     * \brief
     */
    BaseInteraction* getInteractionWith(BaseParticle *P, Mdouble timeStamp, InteractionHandler* interactionHandler);

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
    Vec3D getNormal();
    
    /*!
     * \brief access function for position
     */
    Mdouble getPosition();

private:
    Vec3D normal_; ///<outward unit normal vector
    Mdouble factor_; ///<This is the normal to rescale to unit vectors.
    Mdouble position_; ///<position n*x=p
    Mdouble holeRadius_;

};

#endif
