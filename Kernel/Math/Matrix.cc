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

#include "Matrix.h"
#include "ExtendedMath.h"

Matrix3D::Matrix3D()
{
}

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

void Matrix3D::setZero()
{
    XX = XY = XZ = YX = YY = YZ = ZX = ZY = ZZ = 0.0;
}

Mdouble Matrix3D::trace() const
{
    return (XX + YY + ZZ) / 3;
}

Matrix3D Matrix3D::operator +(const Matrix3D& A) const
        {
    return Matrix3D(XX + A.XX, XY + A.XY, XZ + A.XZ,
            YX + A.YX, YY + A.YY, YZ + A.YZ,
            ZX + A.ZX, ZY + A.ZY, ZZ + A.ZZ);
}

Matrix3D Matrix3D::operator -(const Matrix3D& A) const
        {
    return Matrix3D(XX - A.XX, XY - A.XY, XZ - A.XZ,
            YX - A.YX, YY - A.YY, YZ - A.YZ,
            ZX - A.ZX, ZY - A.ZY, ZZ - A.ZZ);
}

Matrix3D Matrix3D::operator +(const Mdouble a) const
        {
    return Matrix3D(XX + a, XY + a, XZ + a,
            YX + a, YY + a, YZ + a,
            ZX + a, ZY + a, ZZ + a);
}

Matrix3D Matrix3D::operator -(const Mdouble a) const
        {
    return Matrix3D(XX - a, XY - a, XZ - a,
            YX - a, YY - a, YZ - a,
            ZX - a, ZY - a, ZZ - a);
}

Vec3D operator *(const Matrix3D& A, const Vec3D& b)
{
    return Vec3D(A.XX * b.X + A.XY * b.Y + A.XZ * b.Z,
            A.YX * b.X + A.YY * b.Y + A.YZ * b.Z,
            A.ZX * b.X + A.ZY * b.Y + A.ZZ * b.Z);
}

Matrix3D Matrix3D::operator *(const Mdouble a) const
        {
    return Matrix3D(XX * a, XY * a, XZ * a,
            YX * a, YY * a, YZ * a,
            ZX * a, ZY * a, ZZ * a);
}

Vec3D Matrix3D::operator *(const Vec3D& a) const
        {
    return Vec3D(XX * a.X + XY * a.Y + XZ * a.Z,
            YX * a.X + YY * a.Y + YZ * a.Z,
            ZX * a.X + ZY * a.Y + ZZ * a.Z);
}

Matrix3D Matrix3D::operator /(const Mdouble a) const
        {
    return Matrix3D(XX / a, XY / a, XZ / a,
            YX / a, YY / a, YZ / a,
            ZX / a, ZY / a, ZZ / a);
}

std::ostream& operator <<(std::ostream& os, const Matrix3D& A)
{
    os << A.XX << ' ' << A.XY << ' ' << A.XZ << ' '
            << A.YX << ' ' << A.YY << ' ' << A.YZ << ' '
            << A.ZX << ' ' << A.ZY << ' ' << A.ZZ;
    return os;
}

std::istream& operator >>(std::istream& is, Matrix3D& A)
{
    is >> A.XX >> A.XY >> A.XZ >> A.YX >> A.YY >> A.YZ >> A.ZX >> A.ZY >> A.ZZ;
    return is;
}

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

Matrix3D Matrix3D::square(const Matrix3D& A)
{
    return Matrix3D(mathsFunc::square(A.XX), mathsFunc::square(A.XY), mathsFunc::square(A.XZ),
            mathsFunc::square(A.YX), mathsFunc::square(A.YY), mathsFunc::square(A.YZ),
            mathsFunc::square(A.ZX), mathsFunc::square(A.ZY), mathsFunc::square(A.ZZ));
}

Matrix3D Matrix3D::sqrt(const Matrix3D& A)
{
    return Matrix3D(std::sqrt(A.XX), std::sqrt(A.XY), std::sqrt(A.XZ),
            std::sqrt(A.YX), std::sqrt(A.YY), std::sqrt(A.YZ),
            std::sqrt(A.ZX), std::sqrt(A.ZY), std::sqrt(A.ZZ));
}

Matrix3D Matrix3D::dyadic(const Vec3D& a, const Vec3D& b)
{
    return Matrix3D(a.X * b.X, a.X * b.Y, a.X * b.Z,
            a.Y * b.X, a.Y * b.Y, a.Y * b.Z,
            a.Z * b.X, a.Z * b.Y, a.Z * b.Z);
}

Matrix3D Matrix3D::cross(const Vec3D& a, const Matrix3D& B)
{
    return Matrix3D(
            a.Y * B.ZX - a.Z * B.YX, a.Y * B.ZY - a.Z * B.YY, a.Y * B.ZZ - a.Z * B.YZ,
            a.Z * B.XX - a.X * B.ZX, a.Z * B.XY - a.X * B.ZY, a.Z * B.XZ - a.X * B.ZZ,
            a.X * B.YX - a.Y * B.XX, a.X * B.YY - a.Y * B.XY, a.X * B.YZ - a.Y * B.XZ);
}
