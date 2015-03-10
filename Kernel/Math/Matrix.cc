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

#include "Matrix.h"
#include "ExtendedMath.h"

/*!
 * \details default constructor, which is empty (i.e., only creates the object)
 */
Matrix3D::Matrix3D()
{
}

/*!
 *  \details Alternative constructor. Let's you specify ALL 9 elements of the 3x3
 *  matrix. 
 *  \param[in] [all] xx/xy/xz /yx/yy/yz /zx/zy/zz are all nine elements (left-to-right,
 *   top-to-bottom) of the 3D matrix.
 */
Matrix3D::Matrix3D(const Mdouble xx, const Mdouble xy, const Mdouble xz, const Mdouble yx, const Mdouble yy, const Mdouble yz, const Mdouble zx, const Mdouble zy, const Mdouble zz)
{
    XX = xx;
    XY = xy;
    XZ = xz;
    YX = yx;
    YY = yy;
    YZ = yz;
    ZX = zx;
    ZY = zy;
    ZZ = zz;
}

/*!
 *  \details Sets all elements to zero. 
 */
void Matrix3D::setZero()
{
    XX = XY = XZ = YX = YY = YZ = ZX = ZY = ZZ = 0.0;
}

/*!
 * \details Returns the sum of the diagonal elements of the maxtrix, divided by 
 * the total number of diagonal elements.
 * \return resulting scalar
 */
Mdouble Matrix3D::trace() const
{
    return (XX + YY + ZZ) / 3;
}

/*!
 * \details Addition of this matrix with given one
 * \param[in] A     Matrix to be added
 * \return resulting matrix
 */
Matrix3D Matrix3D::operator +(const Matrix3D& A) const
        {
    return Matrix3D(XX + A.XX, XY + A.XY, XZ + A.XZ,
            YX + A.YX, YY + A.YY, YZ + A.YZ,
            ZX + A.ZX, ZY + A.ZY, ZZ + A.ZZ);
}

/*!
 * \details Substraction of given matrix from this one
 * \param[in] A     Matrix to be substracted
 * \return resulting matrix
 * 
 */
Matrix3D Matrix3D::operator -(const Matrix3D& A) const
        {
    return Matrix3D(XX - A.XX, XY - A.XY, XZ - A.XZ,
            YX - A.YX, YY - A.YY, YZ - A.YZ,
            ZX - A.ZX, ZY - A.ZY, ZZ - A.ZZ);
}

/*!
 * \details Addition of scalar
 * \param[in] a     Scalar to be added
 * \return resulting matrix
 */
Matrix3D Matrix3D::operator +(const Mdouble a) const
        {
    return Matrix3D(XX + a, XY + a, XZ + a,
            YX + a, YY + a, YZ + a,
            ZX + a, ZY + a, ZZ + a);
}

/*!
 * \details Substraction of scalar
 * \param[in] a     Scalar to be substracted
 * \return resulting matrix
 */
Matrix3D Matrix3D::operator -(const Mdouble a) const
        {
    return Matrix3D(XX - a, XY - a, XZ - a,
            YX - a, YY - a, YZ - a,
            ZX - a, ZY - a, ZZ - a);
}

/*!
 * \details Multiplication with scalar
 * \param[in] a     Scalar to be multiplied with
 * \return resulting matrix
 */
Matrix3D Matrix3D::operator *(const Mdouble a) const
        {
    return Matrix3D(XX * a, XY * a, XZ * a,
            YX * a, YY * a, YZ * a,
            ZX * a, ZY * a, ZZ * a);
}

/*!
 * \details Multiplication with vector
 * \param[in] a Vector to be multiplied with
 * \return Resulting vector
 */
Vec3D Matrix3D::operator *(const Vec3D& a) const
        {
    return Vec3D(XX * a.X + XY * a.Y + XZ * a.Z,
            YX * a.X + YY * a.Y + YZ * a.Z,
            ZX * a.X + ZY * a.Y + ZZ * a.Z);
}

/*!
 * \details Division by a scalar
 * \param[in] a     scalar to be divided by
 * \return resulting matrix
 */
Matrix3D Matrix3D::operator /(const Mdouble a) const
        {
    return Matrix3D(XX / a, XY / a, XZ / a,
            YX / a, YY / a, YZ / a,
            ZX / a, ZY / a, ZZ / a);
}

/*!
 * \details Adds all elements of a matrix to an ostream
 * \param[out] os   output stream
 * \param[in] A     3D matrix
 * \return output stream with matrix elements added
 */
std::ostream& operator <<(std::ostream& os, const Matrix3D& A)
{
    os << A.XX << ' ' << A.XY << ' ' << A.XZ << ' '
            << A.YX << ' ' << A.YY << ' ' << A.YZ << ' '
            << A.ZX << ' ' << A.ZY << ' ' << A.ZZ;
    return os;
}

/*!
 * \details Reads the elements of a matrix from an istream
 * \param[in,out] is    input stream
 * \param[out] A        3D matrix
 * \return is input stream after the read operation.
 */
std::istream& operator >>(std::istream& is, Matrix3D& A)
{
    is >> A.XX >> A.XY >> A.XZ >> A.YX >> A.YY >> A.YZ >> A.ZX >> A.ZY >> A.ZZ;
    return is;
}

/*!
 * \details Adds all elements of a given matrix to its own
 * \param[in] A     3D matrix to be added
 * \return (reference to) resulting (this) matrix
 */
Matrix3D& Matrix3D::operator +=(const Matrix3D& A)
{
    XX += A.XX;
    XY += A.XY;
    XZ += A.XZ;
    YX += A.YX;
    YY += A.YY;
    YZ += A.YZ;
    ZX += A.ZX;
    ZY += A.ZY;
    ZZ += A.ZZ;
    return *this;
}

/*!
 * \details Substract all elements of a given matrix from its own
 * \param[in] A     3D matrix to be substracted
 * \return (reference to) resulting (this) matrix
 */
Matrix3D& Matrix3D::operator -=(const Matrix3D& A)
{
    XX -= A.XX;
    XY -= A.XY;
    XZ -= A.XZ;
    YX -= A.YX;
    YY -= A.YY;
    YZ -= A.YZ;
    ZX -= A.ZX;
    ZY -= A.ZY;
    ZZ -= A.ZZ;
    return *this;
}

/*!
 * \details Division by a scalar
 * \param[in] a     scalar to be divided by
 * \return (reference to) resulting (this) matrix
 */
Matrix3D& Matrix3D::operator /=(const Mdouble a)
{
    XX /= a;
    XY /= a;
    XZ /= a;
    YX /= a;
    YY /= a;
    YZ /= a;
    ZX /= a;
    ZY /= a;
    ZZ /= a;
    return *this;
}

/*!
 * \details Squares all the elements in given matrix
 * \param[in] A     Matrix to be pointwise squared
 * \return Resulting matrix
 */
Matrix3D Matrix3D::square(const Matrix3D& A)
{
    return Matrix3D(mathsFunc::square(A.XX), mathsFunc::square(A.XY), mathsFunc::square(A.XZ),
            mathsFunc::square(A.YX), mathsFunc::square(A.YY), mathsFunc::square(A.YZ),
            mathsFunc::square(A.ZX), mathsFunc::square(A.ZY), mathsFunc::square(A.ZZ));
}

/*!
 * \details Takes the square root of all the elements in given matrix
 * \param[in] A     Matrix to be pointwise square rooted
 * \return Resulting matrix
 */
Matrix3D Matrix3D::sqrt(const Matrix3D& A)
{
    return Matrix3D(std::sqrt(A.XX), std::sqrt(A.XY), std::sqrt(A.XZ),
            std::sqrt(A.YX), std::sqrt(A.YY), std::sqrt(A.YZ),
            std::sqrt(A.ZX), std::sqrt(A.ZY), std::sqrt(A.ZZ));
}

/*!
 * \details Dyadic product of two vectors
 * \param[in] a     first vector
 * \param[in] b     second vector
 * \return Resulting matrix
 */
Matrix3D Matrix3D::dyadic(const Vec3D& a, const Vec3D& b)
{
    return Matrix3D(a.X * b.X, a.X * b.Y, a.X * b.Z,
            a.Y * b.X, a.Y * b.Y, a.Y * b.Z,
            a.Z * b.X, a.Z * b.Y, a.Z * b.Z);
}

/*!
 * \details Returns a matrix, who's columns are the inner product of a given vector
 * with the corresponding columns of a given matrix
 * \param[in] a     vector
 * \param[in] B     matrix
 * \return Resulting matrix
 */
Matrix3D Matrix3D::cross(const Vec3D& a, const Matrix3D& B)
{
    return Matrix3D(
            a.Y * B.ZX - a.Z * B.YX, a.Y * B.ZY - a.Z * B.YY, a.Y * B.ZZ - a.Z * B.YZ,
            a.Z * B.XX - a.X * B.ZX, a.Z * B.XY - a.X * B.ZY, a.Z * B.XZ - a.X * B.ZZ,
            a.X * B.YX - a.Y * B.XX, a.X * B.YY - a.Y * B.XY, a.X * B.YZ - a.Y * B.XZ);
}
