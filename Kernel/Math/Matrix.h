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

#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include <sstream>
#include "Vector.h"

/*!
 * \brief Implementation of a 3D matrix.
 */
class Matrix3D
{
public:
    /*!
     * \brief 
     */    
    Mdouble XX, XY, XZ, YX, YY, YZ, ZX, ZY, ZZ;

    /*!
     * \brief default constructor
     */
    Matrix3D();

    /*!
     * \brief 
     */    
    Matrix3D(const Mdouble xx, const Mdouble xy, const Mdouble xz, const Mdouble yx, const Mdouble yy, const Mdouble yz, const Mdouble zx, const Mdouble zy, const Mdouble zz);
    
    /*!
     * \brief 
     */
    void setZero();
    
    /*!
     * \brief 
     */
    double trace() const;
    
    /*!
     * \brief 
     */
    Matrix3D operator +(const Matrix3D& A) const;
    
    /*!
     * \brief 
     */
    Matrix3D operator -(const Matrix3D& A) const;
    
    /*!
     * \brief 
     */
    Matrix3D operator +(const Mdouble a) const;
    
    /*!
     * \brief 
     */
    Matrix3D operator -(const Mdouble a) const;
    
    /*!
     * \brief 
     */
    friend Vec3D operator *(const Matrix3D& A, const Vec3D& b);
    
    /*!
     * \brief 
     */
    Matrix3D operator *(const Mdouble a) const;
    
    /*!
     * \brief 
     */
    Vec3D operator *(const Vec3D& a) const;
    
    /*!
     * \brief 
     */
    Matrix3D operator /(const Mdouble a) const;
    
    /*!
     * \brief 
     */
    friend std::ostream& operator<<(std::ostream& os, const Matrix3D &A);
    
    /*!
     * \brief 
     */
    friend std::istream& operator>>(std::istream& is, Matrix3D &A);
    
    /*!
     * \brief 
     */
    Matrix3D& operator+=(const Matrix3D &A);
    
    /*!
     * \brief 
     */
    Matrix3D& operator-=(const Matrix3D &A);
    
    /*!
     * \brief 
     */
    Matrix3D& operator/=(const Mdouble a);
    
    /*!
     * \brief Calculates the pointwise square 
     */
    static Matrix3D square(const Matrix3D &A);
    
    /*!
     * \brief Calculates the ointwise square root
     */
    static Matrix3D sqrt(const Matrix3D &A);
    
    /*!
     * \brief Calculates the dyadic product of a two Vec3D: \f$a \otimes b\f$
     */
    static Matrix3D dyadic(const Vec3D& a, const Vec3D& b);

    /*!
     * \brief Cross product
     */
    static Matrix3D cross(const Vec3D& a, const Matrix3D& b);
    
};



#endif
