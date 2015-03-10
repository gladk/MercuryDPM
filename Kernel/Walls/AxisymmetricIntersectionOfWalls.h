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

#ifndef AXISYMMETRICINTERSECTIONOFWALLS_H
#define AXISYMMETRICINTERSECTIONOFWALLS_H

#include "IntersectionOfWalls.h"
#include "InteractionHandler.h"
#include "Math/Vector.h"
/*!
 * \class AxisymmetricIntersectionOfWalls
 * \brief
 */
class AxisymmetricIntersectionOfWalls : public IntersectionOfWalls
{
public:
   /*!
    * \brief
    */    
    AxisymmetricIntersectionOfWalls();

   /*!
    * \brief
    */    
    AxisymmetricIntersectionOfWalls(const AxisymmetricIntersectionOfWalls &p);

    ///Wall copy method. It calls the copy contrustor of this Wall, usefull for polymorfism
    AxisymmetricIntersectionOfWalls* copy() const;

   /*!
    * \brief
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
     * \brief
     */
    BaseInteraction* getInteractionWith(BaseParticle *P, Mdouble timeStamp, InteractionHandler* interactionHandler);
};


#endif
