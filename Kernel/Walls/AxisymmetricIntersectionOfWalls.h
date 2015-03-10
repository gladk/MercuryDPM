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
 * \brief A AxisymmetricIntersectionOfWalls is a axisymmetric wall, defined by 
 * rotating a twodimensional IntersectionOfWalls around a symmetry axis.
 * \details A AxisymmetricIntersectionOfWalls can be defined by first defining a
 * twodimensional IntersectionOfWalls in the XZ plane. Then, the object is rotated 
 * around the x-axis, creating an three-dimensional axisymmetric wall. Finally, 
 * the object is translated  by the vector position_ and rotated such that the 
 * axis of rotation is the orientation_.
 */
class AxisymmetricIntersectionOfWalls : public IntersectionOfWalls
{
public:
    /*!
     * \brief Default constructor.
     */
    AxisymmetricIntersectionOfWalls();

    /*!
     * \brief Copy constructor.
     */
    AxisymmetricIntersectionOfWalls(const AxisymmetricIntersectionOfWalls& p);

    /*!
     * \brief Destructor.
     */
    ~AxisymmetricIntersectionOfWalls();

    /*!
     * \brief Copy assignment operator.
     */
    AxisymmetricIntersectionOfWalls& operator=(const AxisymmetricIntersectionOfWalls& other);

    /*!
     * \brief Wall copy method. It calls the copy constructor of this Wall, useful for polymorphism
     */
    AxisymmetricIntersectionOfWalls* copy() const final;

    /*!
     * \brief Computes the distance from the wall for a given BaseParticle and 
     * returns true if there is a collision. If there is a collision, also 
     * return the normal vector.
     */
    bool getDistanceAndNormal(const BaseParticle& P, Mdouble& distance, Vec3D& normal_return) const final;

    /*!
     * \brief reads wall
     */
    void read(std::istream& is) final;

    /*!
     * \brief outputs wall
     */
    void write(std::ostream& os) const final;

    /*!
     * \brief Returns the name of the object
     */
    std::string getName() const final;

    /*!
     * \brief Get the interaction between this AxisymmetricIntersectionOfWalls
     * and a given BaseParticle at a given time.
     */
    BaseInteraction* getInteractionWith(BaseParticle* P, Mdouble timeStamp, InteractionHandler* interactionHandler) final;
};


#endif
