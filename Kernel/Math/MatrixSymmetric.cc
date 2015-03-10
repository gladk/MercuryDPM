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
