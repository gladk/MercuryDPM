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

/** Implementation of a 3D vector (by Vitaliy).
 */
///Modifications
/// 21:9:2009 - Added the inclusion guards and some include objects
/// \todo Need to generalize this to n-dimensional vectors of any type
#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <sstream>
#include <iostream>
#include <stdlib.h> 

#include "GeneralDefine.h"

/*!
 * \class Vec3D
 * \brief
 */
class Vec3D
{
public:
    
    /*!
     * \brief 
     */
    Mdouble X, Y, Z;

    /*!
     * \brief constructor
     */
    Vec3D(void);
    
    /*!
     * \brief 
     */
    Vec3D(const Mdouble x, const Mdouble y, const Mdouble z);
    
    /*!
     * \brief 
     */
    void setZero();
    
    /*!
     * \brief 
     */
    bool isZero() const;
    
    /*!
     * \brief 
     */
    Vec3D operator +(const Vec3D& a) const;
    
    /*!
     * \brief 
     */
    Vec3D operator -(const Vec3D& a) const;
    
    /*!
     * \brief 
     */
    Vec3D operator +(const Mdouble a) const;
    
    /*!
     * \brief 
     */
    Vec3D operator -(const Mdouble a) const;
    
    /*!
     * \brief 
     */
    Vec3D operator *(const Mdouble a) const;
    
    /*!
     * \brief 
     */
    Vec3D operator /(const Mdouble a) const;
    
    /*!
     * \brief 
     */
    Vec3D& operator+=(const Vec3D& a);
    
    /*!
     * \brief 
     */
    Vec3D& operator-=(const Vec3D& a);
    
    /*!
     * \brief 
     */
    Vec3D& operator*=(const Mdouble a);
    
    /*!
     * \brief 
     */
    Vec3D& operator/=(const Mdouble a);
    
    /*!
     * \brief Calculates the dot product of two Vec3D: \f$ a \cdot b\f$
     */
    static Mdouble dot(const Vec3D& a, const Vec3D& b);
    
    /*!
     * \brief Calculates the pointwise maximum of two Vec3D
     */
    static Vec3D max(const Vec3D& a, const Vec3D& b);
    
    /*!
     * \brief Calculates the pointwise minimum of two Vec3D
     */
    static Vec3D min(const Vec3D& a, const Vec3D& b);
    
    /*!
     * \brief Calculates the pointwise square of a Vec3D
     */
    static Vec3D square(const Vec3D& a);
    
    /*!
     * \brief Makes this Vec3D unit length
     */
    void normalize();
    
    /*!
     * \brief Make this Vec3D a certain length 
     */
    void setLength(Mdouble length);
    
    /*!
     * \brief Calculates the pointwise square root of a Vec3D
     */
    static Vec3D sqrt(const Vec3D& a);
    
    /*!
     * \brief Calculates the cross product of two Vec3D:  \f$ a \times b\f$
     */
    static Vec3D cross(const Vec3D& a, const Vec3D& b);
    
    /*!
     * \brief Calculates the distance between two Vec3D: \f$ \sqrt{\left(a-b\right) \cdot \left(a-b\right)} \f$
     */
    static Mdouble getDistance(const Vec3D& a, const Vec3D& b);
    
    /*!
     * \brief Calculates the squared distance between two Vec3D: \f$ \left(a-b\right) \cdot \left(a-b\right) \f$
     */
    static Mdouble getDistanceSquared(const Vec3D& a, const Vec3D& b);
    
    /*!
     * \brief Calculates the length of a Vec3D: \f$ \sqrt{a\cdot a} \f$
     */
    static Mdouble getLength(const Vec3D& a);

    /*!
     * \brief Calculates the squared length of a Vec3D: \f$ a\cdot a \f$
     */
    static Mdouble getLengthSquared(const Vec3D& a);
    
    /*!
     * \brief Calculates the length of this Vec3D: \f$ \sqrt{a\cdot a} \f$
     */
    Mdouble getLength() const;
    
    /*!
     * \brief Calculates the squared length of this Vec3D: \f$ a\cdot a \f$
     */
    Mdouble getLengthSquared() const;
    
    /*!
     * \brief Returns the requested component of this Vec3D
     */
    Mdouble getComponent(const int index) const;
    
    /*!
     * \brief Sets the requested component of this Vec3D to the requested value
     */
    void setComponent(const int index, const double val);
    
    /*!
     * \brief Returns the representation of this Vec3D in cylindrical coordinates
     */
    Vec3D getCylindricalCoordinates() const;
    
    /*!
     * \brief Returns the representation of this Vec3D in cartesian coordinates
     */
    Vec3D getFromCylindricalCoordinates() const;

    /*!
     * \brief Checks if the length this Vec3D is equal the length of other with a certain tolerance
     */
    bool compareTo(const Vec3D& other, const double tol) const;
    
// 	void ConvertToCylindricalCoordinates()
// 	{
// 		double R = sqrt(X*X+Y*Y); Y = atan2(Y,X); X = R; return;
// 	}
// 
// 	void ConvertFromCylindricalCoordinates()
// 	{
// 		double Xnew = X * cos(Y); Y = X * sin(Y); X = Xnew; return;
// 	}
    
    /*!
     * \brief Returns a unit Vec3D based on a.
     */
    static Vec3D getUnitVector(const Vec3D& a);
    
    /*!
     * \brief 
     */
    friend std::ostream& operator<<(std::ostream& os, const Vec3D& a);
    
    /*!
     * \brief 
     */
    friend std::istream& operator>>(std::istream& is, Vec3D& a);
    
    /*!
     * \brief 
     */
    friend Vec3D operator+(const Mdouble a, const Vec3D& b);
    
    /*!
     * \brief 
     */
    friend Vec3D operator-(const Mdouble a, const Vec3D& b);
    
    /*!
     * \brief 
     */
    friend Vec3D operator-(const Vec3D& a);
    
    /*!
     * \brief 
     */
    friend Vec3D operator*(const Mdouble a, const Vec3D& b);    
};

#endif
