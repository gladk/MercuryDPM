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
