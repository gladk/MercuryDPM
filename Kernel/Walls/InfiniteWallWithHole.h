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
