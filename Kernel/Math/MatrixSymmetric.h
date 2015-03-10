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
