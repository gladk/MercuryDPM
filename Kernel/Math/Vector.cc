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
#include "Vector.h"

Vec3D::Vec3D(void)
{
    setZero();
}

Vec3D::Vec3D(const Mdouble x, const Mdouble y, const Mdouble z)
{
    X = x;
    Y = y;
    Z = z;
}

void Vec3D::setZero()
{
    X = 0.0;
    Y = 0.0;
    Z = 0.0;
}

bool Vec3D::isZero() const
{
    return X == 0.0 && Y == 0.0 && Z == 0.0;
}

Vec3D Vec3D::operator +(const Vec3D& a) const
        {
    return Vec3D(X + a.X, Y + a.Y, Z + a.Z);
}

Vec3D Vec3D::operator -(const Vec3D& a) const
        {
    return Vec3D(X - a.X, Y - a.Y, Z - a.Z);
}

Vec3D Vec3D::operator +(const Mdouble a) const
        {
    return Vec3D(X + a, Y + a, Z + a);
}

Vec3D Vec3D::operator -(const Mdouble a) const
        {
    return Vec3D(X - a, Y - a, Z - a);
}

Vec3D Vec3D::operator *(const Mdouble a) const
        {
    return Vec3D(X * a, Y * a, Z * a);
}

Vec3D Vec3D::operator /(const Mdouble a) const
        {
    return Vec3D(X / a, Y / a, Z / a);
}

Vec3D& Vec3D::operator+=(const Vec3D& a)
{
    X += a.X;
    Y += a.Y;
    Z += a.Z;
    return *this;
}

Vec3D& Vec3D::operator-=(const Vec3D& a)
{
    X -= a.X;
    Y -= a.Y;
    Z -= a.Z;
    return *this;
}

Vec3D& Vec3D::operator*=(const Mdouble a)
{
    X *= a;
    Y *= a;
    Z *= a;
    return *this;
}

Vec3D& Vec3D::operator/=(const Mdouble a)
{
    X /= a;
    Y /= a;
    Z /= a;
    return *this;
}

Mdouble Vec3D::dot(const Vec3D& a, const Vec3D& b)
{
    return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}

Vec3D Vec3D::max(const Vec3D& a, const Vec3D& b)
{
    return Vec3D(std::max(a.X, b.X), std::max(a.Y, b.Y), std::max(a.Z, b.Z));
}

Vec3D Vec3D::min(const Vec3D& a, const Vec3D& b)
{
    return Vec3D(std::min(a.X, b.X), std::min(a.Y, b.Y), std::min(a.Z, b.Z));
}

Vec3D Vec3D::square(const Vec3D& a)
{
    return Vec3D(a.X * a.X, a.Y * a.Y, a.Z * a.Z);
}

void Vec3D::normalize()
{
    *this /= this->getLength();
}

void Vec3D::setLength(Mdouble length)
{
    *this /= this->getLength() * length;
}

Vec3D Vec3D::sqrt(const Vec3D& a)
{
    return Vec3D(std::sqrt(a.X), std::sqrt(a.Y), std::sqrt(a.Z));
}

Vec3D Vec3D::cross(const Vec3D& a, const Vec3D& b)
{
    return Vec3D(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}

Mdouble Vec3D::getDistance(const Vec3D& a, const Vec3D& b)
{
    return std::sqrt(getDistanceSquared(a, b));
}

Mdouble Vec3D::getDistanceSquared(const Vec3D& a, const Vec3D& b)
{
    return ((a.X - b.X) * (a.X - b.X) + (a.Y - b.Y) * (a.Y - b.Y) + (a.Z - b.Z) * (a.Z - b.Z));
}

Mdouble Vec3D::getLengthSquared(const Vec3D& a)
{
    return (a.X * a.X + a.Y * a.Y + a.Z * a.Z);
}

Mdouble Vec3D::getLengthSquared() const
{
    return (X * X + Y * Y + Z * Z);
}

Mdouble Vec3D::getComponent(const int index) const
        {
    switch (index)
    {
        case 0:
            return X;
        case 1:
            return Y;
        case 2:
            return Z;
        default:
            std::cerr << "Index=" << index << " is to high for 3D vector" << std::endl;
            exit(-1);
    }
}

void Vec3D::setComponent(const int index, const double val)
{
    switch (index)
    {
        case 0:
            X = val;
            return;
        case 1:
            Y = val;
            return;
        case 2:
            Z = val;
            return;
        default:
            std::cerr << "Index=" << index << " is to high for 3D vector" << std::endl;
            exit(-1);
    }
}

Vec3D Vec3D::getCylindricalCoordinates() const
{
    return Vec3D(std::sqrt(X * X + Y * Y), std::atan2(Y, X), Z);
}

Vec3D Vec3D::getFromCylindricalCoordinates() const
{
    return Vec3D(X * std::cos(Y), X * std::sin(Y), Z);
}

bool Vec3D::compareTo(const Vec3D& other, const double tol) const
        {
    if ((Vec3D::getLengthSquared(*this - other)) < tol * tol)
    {
        return true;
    }
    else
    {
        return false;
    }
}

// 	void ConvertToCylindricalCoordinates()
// 	{
// 		double R = sqrt(X*X+Y*Y); Y = atan2(Y,X); X = R; return;
// 	}
// 
// 	void ConvertFromCylindricalCoordinates()
// 	{
// 		double Xnew = X * cos(Y); Y = X * sin(Y); X = Xnew; return;
// 	}

Mdouble Vec3D::getLength() const
{
    return std::sqrt(getLengthSquared());
}

Mdouble Vec3D::getLength(const Vec3D& a)
{
    return a.getLength();
}

Vec3D Vec3D::getUnitVector(const Vec3D& a)
{
    Mdouble Length2 = a.getLengthSquared();
    if (Length2 != 0.0)
        return a / std::sqrt(Length2);
    else
        return Vec3D(0, 0, 0);
}

std::ostream& operator<<(std::ostream& os, const Vec3D& a)
{
    os << a.X << ' ' << a.Y << ' ' << a.Z;
    return os;
}

std::istream& operator>>(std::istream& is, Vec3D& a)
{
    is >> a.X >> a.Y >> a.Z;
    return is;
}

Vec3D operator+(const Mdouble a, const Vec3D& b)
{
    return Vec3D(b.X + a, b.Y + a, b.Z + a);
}

Vec3D operator-(const Mdouble a, const Vec3D& b)
{
    return Vec3D(b.X - a, b.Y - a, b.Z - a);
}

Vec3D operator-(const Vec3D& a)
{
    return Vec3D(-a.X, -a.Y, -a.Z);
}

Vec3D operator*(const Mdouble a, const Vec3D& b)
{
    return Vec3D(b.X * a, b.Y * a, b.Z * a);
}
