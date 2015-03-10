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

#ifndef MATRIXSYMMETRIC_H
#define MATRIXSYMMETRIC_H

#include <cmath>
#include <sstream>
#include "Matrix.h"

/*!
 * \brief Implementation of a 3D symmetric matrix.
 */
class MatrixSymmetric3D
{
public:
    /*!
     * \brief 
     */    
    Mdouble XX, XY, XZ, YY, YZ, ZZ;

    /*!
     * \brief 
     */
    operator Matrix3D() const;

    /*!
     * \brief 
     */
    MatrixSymmetric3D(void);

    /*!
     * \brief 
     */
    MatrixSymmetric3D(const Mdouble xx, const Mdouble xy, const Mdouble xz, const Mdouble yy, const Mdouble yz, const Mdouble zz);

    /*!
     * \brief 
     */
    void setZero();

    /*!
     * \brief 
     */
    Mdouble trace() const;

    /*!
     * \brief 
     */
    MatrixSymmetric3D operator +(const MatrixSymmetric3D &A) const;

    /*!
     * \brief 
     */
    MatrixSymmetric3D operator -(const MatrixSymmetric3D &A) const;

    /*!
     * \brief 
     */
    MatrixSymmetric3D operator +(const Mdouble a) const;

    /*!
     * \brief 
     */
    MatrixSymmetric3D operator -(const Mdouble a) const;

    /*!
     * \brief 
     */
    friend Vec3D operator *(const MatrixSymmetric3D& A, const Vec3D& b);

    /*!
     * \brief 
     */
    MatrixSymmetric3D operator *(const Mdouble a) const;

    /*!
     * \brief 
     */
    MatrixSymmetric3D operator /(const Mdouble a) const;

    /*!
     * \brief 
     */
    friend std::ostream& operator<<(std::ostream& os, const MatrixSymmetric3D& A);

    /*!
     * \brief 
     */
    friend std::istream& operator>>(std::istream& is, MatrixSymmetric3D& A);

    /*!
     * \brief 
     */
    MatrixSymmetric3D& operator+=(const MatrixSymmetric3D& A);

    /*!
     * \brief 
     */
    MatrixSymmetric3D& operator-=(const MatrixSymmetric3D& A);

    /*!
     * \brief 
     */
    MatrixSymmetric3D& operator/=(const Mdouble a);

    /*!
     * \brief Calculates the pointwise square 
     */
    static MatrixSymmetric3D square(const MatrixSymmetric3D& A);

    /*!
     * \brief Calculates the pointwise square root
     */
    static MatrixSymmetric3D sqrt(const MatrixSymmetric3D& A);

    /*!
     * \brief Calculates the dyadic product of a Vec3D with itself: \f$a \otimes a\f$
     */
    static MatrixSymmetric3D selfDyadic(const Vec3D& a);

    /*!
     * \brief Calculates the symmetrized dyadic product of two Vec3D: \f$ \frac{1}{2}(a \otimes b + b \otimes a) \f$
     */
    static MatrixSymmetric3D symmetrisedDyadic(const Vec3D&  a, const Vec3D& b);
};

#endif
