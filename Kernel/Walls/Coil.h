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

#ifndef COIL_H
#define COIL_H

#include "BaseWall.h"
#include "Math/Vector.h"

/*!
 * \class Coil
 * \brief This class defines a coil in the z-direction from a (constant) starting point, a (constant) length L, a (constant) radius r, a (constant) number or revelations N and a (constant) rotation speed (rev/s)
 * \details q is a new coordinate going from 0 to 1 and t is the time, x=xs+r*cos(2*pi(offset+N*q)), y=ys+r*sin(2*pi*(offset+N*q)) and z=zs+q*L
 */

class Coil : public BaseWall
{
public:
   /*!
    * \brief
    */    
    Coil();
    
   /*!
    * \brief
    */    
    Coil(Vec3D Start, double L, double r, double N, double omega, double thickness);
    
    /*!
     * \brief
     */
    void set(Vec3D Start, double L, double r, double N, double omega, double thickness);

   /*!
    * \brief
    */    
    virtual Coil* copy() const;
    
   /*!
    * \brief
    */    
    bool getDistanceAndNormal(const BaseParticle& P, Mdouble& distance, Vec3D& normal_return) const;

   /*!
    * \brief
    */    
    BaseInteraction* getInteractionWith(BaseParticle *P, Mdouble timeStamp, InteractionHandler* interactionHandler);

   /*!
    * \brief Allows the wall to be moved to a new position (also orthogonal to the normal), and setting the velocity
    */
    void move_time(Mdouble dt);
    
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

private:
    Vec3D start_;///

   /*!
    * \brief
    */    
    double l_, r_, n_, omega_, offset_, thickness_;
};

#endif
