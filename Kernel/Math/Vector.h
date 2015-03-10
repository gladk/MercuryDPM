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
     * \brief the vector components
     */
    Mdouble X, Y, Z;

    /*!
     * \brief constructor
     */
    Vec3D();
    
    /*!
     * \brief Alternative constructor, taking the three elements as arguments
     */
    Vec3D(const Mdouble x, const Mdouble y, const Mdouble z);
    
    /*!
     * \brief Sets all elements to zero
     */
    void setZero();
    
    /*!
     * \brief Checks if ALL elements are zero
     */
    bool isZero() const;
    
    /*!
     * \brief Adds another vector
     */
    Vec3D operator +(const Vec3D& a) const;
    
    /*!
     * \brief Subtracts another vector
     */
    Vec3D operator -(const Vec3D& a) const;
    
    /*!
     * \brief Adds a scalar
     */
    Vec3D operator +(const Mdouble a) const;
    
    /*!
     * \brief Subtracts a scalar
     */
    Vec3D operator -(const Mdouble a) const;
    
    /*!
     * \brief Multiplies by a scalar
     */
    Vec3D operator *(const Mdouble a) const;
    
    /*!
     * \brief Divides by a scalar
     */
    Vec3D operator /(const Mdouble a) const;
    
    /*!
     * \brief Adds another vector
     */
    Vec3D& operator+=(const Vec3D& a);
    
    /*!
     * \brief Subtracts another vector
     */
    Vec3D& operator-=(const Vec3D& a);
    
    /*!
     * \brief Multiplies by a scalar
     */
    Vec3D& operator*=(const Mdouble a);
    
    /*!
     * \brief Divides by a scalar
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
    bool isEqualTo(const Vec3D& other, const double tol) const;
    
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
     * \brief Adds elements to an output stream
     */
    friend std::ostream& operator<<(std::ostream& os, const Vec3D& a);
    
    /*!
     * \brief Adds elements to an input stream
     */
    friend std::istream& operator>>(std::istream& is, Vec3D& a);
    
    /*!
     * \brief Adds a scalar to a vector
     */
    friend Vec3D operator+(const Mdouble a, const Vec3D& b);
    
    /*!
     * \brief Subtracts the elements of a vector from a scalar
     */
    friend Vec3D operator-(const Mdouble a, const Vec3D& b);
    
    /*!
     * \brief Subtracts a vector 
     */
    friend Vec3D operator-(const Vec3D& a);
    
    /*!
     * \brief Multiplies all elements by a scalar
     */
    friend Vec3D operator*(const Mdouble a, const Vec3D& b);    
};

#endif
