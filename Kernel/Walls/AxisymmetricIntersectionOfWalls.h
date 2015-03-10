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
