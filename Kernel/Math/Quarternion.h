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
