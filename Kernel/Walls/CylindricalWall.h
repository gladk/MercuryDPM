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

#ifndef CYLINDRICALWALL_H
#define CYLINDRICALWALL_H

#include "BaseWall.h"
/*!
 * \class CylindricalWall
 * \brief
 */
class CylindricalWall : public BaseWall
{
public:
   /*!
    * \brief
    */    
    CylindricalWall();
    
   /*!
    * \brief
    */    
    CylindricalWall(const CylindricalWall &p);
    
   /*!
    * \brief
    */    
    CylindricalWall(double radius);
    
   /*!
    * \brief Wall copy method. It calls the copy contrustor of this Wall, usefull for polymorfism
    */
    CylindricalWall* copy() const;
    
   /*!
    * \brief
    */    
    void clear();
    
   /*!
    * \brief Defines a standard wall, given an outward normal vector s. t. normal*x=position
    */
    void set(Mdouble radius);
    
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
    * \brief access function for radius
    */
    double get_Radius();

    /*!
     * \brief
     */
    BaseInteraction* getInteractionWith(BaseParticle *P, Mdouble timeStamp, InteractionHandler* interactionHandler);

private:
    Mdouble radius_;///
};

#endif
