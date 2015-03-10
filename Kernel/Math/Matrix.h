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
