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

MatrixSymmetric3D::operator Matrix3D() const
{
    return Matrix3D(XX, XY, XZ, XY, YY, YZ, XZ, YZ, ZZ);
}

MatrixSymmetric3D::MatrixSymmetric3D(void)
{
}

MatrixSymmetric3D::MatrixSymmetric3D(const Mdouble xx, const Mdouble xy, const Mdouble xz, const Mdouble yy, const Mdouble yz, const Mdouble zz)
{
    XX = xx;
    XY = xy;
    XZ = xz;
    YY = yy;
    YZ = yz;
    ZZ = zz;
}

void MatrixSymmetric3D::setZero()
{
    XX = XY = XZ = YY = YZ = ZZ = 0.0;
}

Mdouble MatrixSymmetric3D::trace() const
{
    return (XX + YY + ZZ) / 3;
}

MatrixSymmetric3D MatrixSymmetric3D::operator +(const MatrixSymmetric3D& A) const
        {
    return MatrixSymmetric3D(XX + A.XX, XY + A.XY, XZ + A.XZ, YY + A.YY, YZ + A.YZ, ZZ + A.ZZ);
}

MatrixSymmetric3D MatrixSymmetric3D::operator -(const MatrixSymmetric3D& A) const
        {
    return MatrixSymmetric3D(XX - A.XX, XY - A.XY, XZ - A.XZ, YY - A.YY, YZ - A.YZ, ZZ - A.ZZ);
}

MatrixSymmetric3D MatrixSymmetric3D::operator +(const Mdouble a) const
        {
    return MatrixSymmetric3D(XX + a, XY + a, XZ + a, YY + a, YZ + a, ZZ + a);
}

MatrixSymmetric3D MatrixSymmetric3D::operator -(const Mdouble a) const
        {
    return MatrixSymmetric3D(XX - a, XY - a, XZ - a, YY - a, YZ - a, ZZ - a);
}

Vec3D operator *(const MatrixSymmetric3D& A, const Vec3D& b)
{
    return Vec3D(A.XX * b.X + A.XY * b.Y + A.XZ * b.Z,
            A.XY * b.X + A.YY * b.Y + A.YZ * b.Z,
            A.XZ * b.X + A.YZ * b.Y + A.ZZ * b.Z);
}

MatrixSymmetric3D MatrixSymmetric3D::operator *(const Mdouble a) const
        {
    return MatrixSymmetric3D(XX * a, XY * a, XZ * a,
            YY * a, YZ * a, ZZ * a);
}

MatrixSymmetric3D MatrixSymmetric3D::operator /(const Mdouble a) const
        {
    return MatrixSymmetric3D(XX / a, XY / a, XZ / a, YY / a, YZ / a, ZZ / a);
}

std::ostream& operator<<(std::ostream& os, const MatrixSymmetric3D& A)
{
    os << A.XX << ' ' << A.XY << ' ' << A.XZ << " " << A.YY << ' ' << A.YZ << " " << A.ZZ;
    return os;
}

std::istream& operator>>(std::istream& is, MatrixSymmetric3D& A)
{
    is >> A.XX >> A.XY >> A.XZ >> A.YY >> A.YZ >> A.ZZ;
    return is;
}

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

MatrixSymmetric3D MatrixSymmetric3D::square(const MatrixSymmetric3D& A)
{
    return MatrixSymmetric3D(mathsFunc::square(A.XX), mathsFunc::square(A.XY), mathsFunc::square(A.XZ), mathsFunc::square(A.YY), mathsFunc::square(A.YZ), mathsFunc::square(A.ZZ));
}

MatrixSymmetric3D MatrixSymmetric3D::sqrt(const MatrixSymmetric3D& A)
{
    return MatrixSymmetric3D(std::sqrt(A.XX), std::sqrt(A.XY), std::sqrt(A.XZ), std::sqrt(A.YY), std::sqrt(A.YZ), std::sqrt(A.ZZ));
}

MatrixSymmetric3D MatrixSymmetric3D::selfDyadic(const Vec3D& a)
{
    return MatrixSymmetric3D(a.X * a.X, a.X * a.Y, a.X * a.Z, a.Y * a.Y, a.Y * a.Z, a.Z * a.Z);
}

MatrixSymmetric3D MatrixSymmetric3D::symmetrisedDyadic(const Vec3D& a, const Vec3D& b)
{
    return MatrixSymmetric3D(a.X * b.X, 0.5 * (a.X * b.Y + b.X * a.Y), 0.5 * (a.X * b.Z + b.X * a.Z), a.Y * b.Y, 0.5 * (a.Y * b.Z + b.Y * a.Z), a.Z * b.Z);
}
