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
#ifndef QUARTERNION_H
#define QUARTERNION_H

#include "Vector.h"
#include "Matrix.h"
#include "ExtendedMath.h"

/*!
 * \class Quaternion
 * \brief
 */
class Quarternion
{
public:
    /*!
     * \brief Constructs a basic Quarternion
     */
    Quarternion();
    
    /*!
     * \brief Copy constructor
     */
    Quarternion(const Quarternion& other);
    
    /*!
     * \brief Constructs a a Quarternion using its four components.
     */
    Quarternion(double x, double y, double z, double w);
    
    /*!
     * \brief Constructs a Quarternion using a rotation around an axis.
     */
    Quarternion(Vec3D axis, double angle);
    
    /*!
     * \brief Returns the orientation as represented by a rotation around an axis.
     */
    void getAxisAndAngle(Vec3D& axis, double& angle) const;
    
    /*!
     * \brief 
     */
    Matrix3D getRotationMatrix() const;
    
    /*!
     * \brief Resets a the Quarternion
     */
    void reset();

    /*!
     * \brief Normalises the Quarternion
     */
    void normalise();
    
    /*!
     * \brief Multiplication operator
     */
    Quarternion operator *(const Quarternion &other) const;
    
    /*!
     * \brief Integration of the Quarternion using rotational velocity and timestep
     */
    void integrate(const Vec3D& omega, double timeStep);
    
    /*!
     * \brief Constructs a Quaternion using the lab x-y-z sequence, where the object is
     * \details first rotated along lab X-axis then rotated along lab Y-axis then rotated along lab Z-axis
     */
    Quarternion(const Vec3D& a);
    
    /*!
     * \brief Constructs a representation of the quarternion using 3 rotations in the lab x-y-z sequence, where the object is first rotated along lab X-axis then rotated along lab Y-axis then rotated along lab Z-axis
     */
    Vec3D get3DAngles() const;
    
    /*!
     * \brief 
     */
    friend std::ostream& operator<<(std::ostream& os, const Quarternion &q);
    
    /*!
     * \brief 
     */
    friend std::istream& operator>>(std::istream& is, Quarternion &q);
    
private:
    /*!
     * \brief 
     */
    Mdouble w_, x_, y_, z_;
};

#endif
