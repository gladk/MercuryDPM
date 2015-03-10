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

#include "MatrixSymmetric.h"
#include "ExtendedMath.h"

/*!
 * \details This operator casts a MatrixSymmetric3D object to a Matrix3D object 
 * when needed. 
 * \return The output is a symmetric matrix of type Matrix3D.
 */
MatrixSymmetric3D::operator Matrix3D() const
{
    return Matrix3D(XX, XY, XZ, XY, YY, YZ, XZ, YZ, ZZ);
}

/*!
 * \details Default constructor
 */
MatrixSymmetric3D::MatrixSymmetric3D()
{
}

/*!
 * \details Alternative constructor, lets you define all (6) elements
 * \param[in] xx initializer for an element of the matrix
 * \param[in] xy initializer for an element of the matrix
 * \param[in] xz initializer for an element of the matrix
 * \param[in] yy initializer for an element of the matrix
 * \param[in] yz initializer for an element of the matrix
 * \param[in] zz initializer for an element of the matrix
 */
MatrixSymmetric3D::MatrixSymmetric3D(const Mdouble xx, const Mdouble xy, const Mdouble xz, const Mdouble yy, const Mdouble yz, const Mdouble zz)
{
    XX = xx;
    XY = xy;
    XZ = xz;
    YY = yy;
    YZ = yz;
    ZZ = zz;
}

/*!
 * \details Sets all elements of the symmetric matrix to 0.
 */
void MatrixSymmetric3D::setZero()
{
    XX = XY = XZ = YY = YZ = ZZ = 0.0;
}

/*!
 * \details Returns the MEAN of the diagonal matrix elements, which is technically
 * the trace divided by the number of diagonal elements.
 * \return The mean of the diagonal elements.
 */
Mdouble MatrixSymmetric3D::trace() const
{
    return (XX + YY + ZZ) / 3;
}

/*!
 * \details Addition of symmetric 3D matrices
 * \param[in] A     symmetric matrix to be added
 * \return Result of the addition 
 */
MatrixSymmetric3D MatrixSymmetric3D::operator +(const MatrixSymmetric3D& A) const
        {
    return MatrixSymmetric3D(XX + A.XX, XY + A.XY, XZ + A.XZ, YY + A.YY, YZ + A.YZ, ZZ + A.ZZ);
}

/*!
 * \details Substraction of symmetric 3D matrices
 * \param[in] A     symmetric matrix to be substracted
 * \return Result of the substraction 
 */
MatrixSymmetric3D MatrixSymmetric3D::operator -(const MatrixSymmetric3D& A) const
        {
    return MatrixSymmetric3D(XX - A.XX, XY - A.XY, XZ - A.XZ, YY - A.YY, YZ - A.YZ, ZZ - A.ZZ);
}

/*!
 * \details Addition of a scalar
 * \param[in] a     scalar to be added
 * \return Result of the addition
 */
MatrixSymmetric3D MatrixSymmetric3D::operator +(const Mdouble a) const
        {
    return MatrixSymmetric3D(XX + a, XY + a, XZ + a, YY + a, YZ + a, ZZ + a);
}

/*!
 * \details Substraction of a scalar
 * \param[in] a     scalar to be substracted
 * \return Result of the substraction
 */
MatrixSymmetric3D MatrixSymmetric3D::operator -(const Mdouble a) const
        {
    return MatrixSymmetric3D(XX - a, XY - a, XZ - a, YY - a, YZ - a, ZZ - a);
}

/*!
 * \details Multiplication of a symmetric 3D matrix with a vector (global operator, 
 * friend of this class)
 * \param[in] A     the matrix
 * \param[in] b     the vector
 * \return The multiplication result
 */
Vec3D operator *(const MatrixSymmetric3D& A, const Vec3D& b)
{
    return Vec3D(A.XX * b.X + A.XY * b.Y + A.XZ * b.Z,
            A.XY * b.X + A.YY * b.Y + A.YZ * b.Z,
            A.XZ * b.X + A.YZ * b.Y + A.ZZ * b.Z);
}

/*!
 * \details Scalar multiplication
 * \param[in] a     the scalar
 * \return The multiplication result
 */
MatrixSymmetric3D MatrixSymmetric3D::operator *(const Mdouble a) const
        {
    return MatrixSymmetric3D(XX * a, XY * a, XZ * a,
            YY * a, YZ * a, ZZ * a);
}

/*!
 * \details Scalar division
 * \param[in] a     the scalar to be divided by
 * \return The division result
 */
MatrixSymmetric3D MatrixSymmetric3D::operator /(const Mdouble a) const
        {
    return MatrixSymmetric3D(XX / a, XY / a, XZ / a, YY / a, YZ / a, ZZ / a);
}

/*!
 * \details Adds all elements of a symmetric 3D matrix to an ostream
 * \param[in,out] os    output stream
 * \param[in] A         the matrix
 * \return (reference to) output stream with matrix elements added
 */
std::ostream& operator<<(std::ostream& os, const MatrixSymmetric3D& A)
{
    os << A.XX << ' ' << A.XY << ' ' << A.XZ << " " << A.YY << ' ' << A.YZ << " " << A.ZZ;
    return os;
}

/*!
 * \details Reads the elements of a symmetric 3D matrix from an istream
 * \param[in,out] is    input stream, 
 * \param[out] A        the matrix
 * \return (reference to) input stream from with matrix elements were read
 */
std::istream& operator>>(std::istream& is, MatrixSymmetric3D& A)
{
    is >> A.XX >> A.XY >> A.XZ >> A.YY >> A.YZ >> A.ZZ;
    return is;
}

/*!
 * \details Matrix addition
 * \param[in] A     the matrix to be added to this one
 * \return (reference to) this matrix, i.e. the result of the addition
 */
MatrixSymmetric3D& MatrixSymmetric3D::operator+=(const MatrixSymmetric3D& A)
{
    XX += A.XX;
    XY += A.XY;
    XZ += A.XZ;
    YY += A.YY;
    YZ += A.YZ;
    ZZ += A.ZZ;
    return *this;
}

/*!
 * \details Matrix substraction
 * \param[in] A     the matrix to be substracted
 * \return (reference to) this matrix, which is the result of the substraction
 */
MatrixSymmetric3D& MatrixSymmetric3D::operator-=(const MatrixSymmetric3D& A)
{
    XX -= A.XX;
    XY -= A.XY;
    XZ -= A.XZ;
    YY -= A.YY;
    YZ -= A.YZ;
    ZZ -= A.ZZ;
    return *this;
}

/*!
 * \details Scalar division
 * \param[in] a     the scalar to be divided by
 * \return (reference to) this matrix, which is the result of the division
 */
MatrixSymmetric3D& MatrixSymmetric3D::operator/=(const Mdouble a)
{
    XX /= a;
    XY /= a;
    XZ /= a;
    YY /= a;
    YZ /= a;
    ZZ /= a;
    return *this;
}

/*!
 * \details Pointwise squares the matrix, i.e. takes the square of each element.
 * NB: is a STATIC function!
 * \param[in] A     The matrix to be pointwise squared
 * \return Resulting matrix
 */
MatrixSymmetric3D MatrixSymmetric3D::square(const MatrixSymmetric3D& A)
{
    return MatrixSymmetric3D(mathsFunc::square(A.XX), mathsFunc::square(A.XY), mathsFunc::square(A.XZ), mathsFunc::square(A.YY), mathsFunc::square(A.YZ), mathsFunc::square(A.ZZ));
}

/*!
 * \details Pointwise square roots the matrix, i.e. takes the sq. root of each element.
 * NB: this is a STATIC function!
 * \param[in] A     The matrix to be pointwise square rooted
 * \return Resulting matrix
 */
MatrixSymmetric3D MatrixSymmetric3D::sqrt(const MatrixSymmetric3D& A)
{
    return MatrixSymmetric3D(std::sqrt(A.XX), std::sqrt(A.XY), std::sqrt(A.XZ), std::sqrt(A.YY), std::sqrt(A.YZ), std::sqrt(A.ZZ));
}

/*!
 * \details Calculates the dyadic product of a 3D vector with itself, which (obviously)
 * results in a symmetric 3D matrix.
 * NB: this is a STATIC function!
 * \param[in] a     the 3D vector
 * \return the resulting symmetric 3D matrix.
 */
MatrixSymmetric3D MatrixSymmetric3D::selfDyadic(const Vec3D& a)
{
    return MatrixSymmetric3D(a.X * a.X, a.X * a.Y, a.X * a.Z, a.Y * a.Y, a.Y * a.Z, a.Z * a.Z);
}

/*!
 * \details Calculates the dyadic product of two 3D vectors, and 'symmetrises'  the
 * resulting matrix by taking the average of each pair of opposing non-diagonal 
 * elements.
 * NB: this is a STATIC function!
 * \param[in] a     the first 3D vector
 * \param[in] b     the second 3D vector
 * \return the resulting symmetric 3D matrix
 */
MatrixSymmetric3D MatrixSymmetric3D::symmetrisedDyadic(const Vec3D& a, const Vec3D& b)
{
    return MatrixSymmetric3D(a.X * b.X, 0.5 * (a.X * b.Y + b.X * a.Y), 0.5 * (a.X * b.Z + b.X * a.Z), a.Y * b.Y, 0.5 * (a.Y * b.Z + b.Y * a.Z), a.Z * b.Z);
}
