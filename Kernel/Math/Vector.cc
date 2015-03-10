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
#include <Logger.h>

/*!
 * \details Default constructor
 */
Vec3D::Vec3D()
{
    setZero();
}

/*!
 * \details Alternative constructor, lets you define all three elements.
 * \param[in] x     the x-component
 * \param[in] y     the y-component
 * \param[in] z     the z-component
 */
Vec3D::Vec3D(const Mdouble x, const Mdouble y, const Mdouble z)
{
    X = x;
    Y = y;
    Z = z;
}

/*!
 * \details Sets each element to zero.
 */
void Vec3D::setZero()
{
    X = 0.0;
    Y = 0.0;
    Z = 0.0;
}

/*!
 * \details Checks if ALL elements are zero 
 * \return          TRUE if ALL elements are zero
 */
bool Vec3D::isZero() const
{
    return X == 0.0 && Y == 0.0 && Z == 0.0;
}

/*!
 * \details Adds vector to itself
 * \param[in] a     vector to be added
 * \return          resulting 3D vector
 */
Vec3D Vec3D::operator +(const Vec3D& a) const
        {
    return Vec3D(X + a.X, Y + a.Y, Z + a.Z);
}

/*!
 * \details Subtracts vector from itself
 * \param[in] a     vector to be subtracted
 * \return          resulting vector
 */
Vec3D Vec3D::operator -(const Vec3D& a) const
        {
    return Vec3D(X - a.X, Y - a.Y, Z - a.Z);
}

/*!
 * \details Adds scalar to each element
 * \param[in] a     scalar to be added
 * \return          resulting vector
 */
Vec3D Vec3D::operator +(const Mdouble a) const
        {
    return Vec3D(X + a, Y + a, Z + a);
}

/*!
 * \details Subtracts scalar from each element
 * \param[in] a     scalar to be subtracted
 * \return          resulting vector
 */
Vec3D Vec3D::operator -(const Mdouble a) const
        {
    return Vec3D(X - a, Y - a, Z - a);
}

/*!
 * \details Multiplies each element with a scalar
 * \param[in] a     the scalar to be multiplied with
 * \return          the resulting vector
 */
Vec3D Vec3D::operator *(const Mdouble a) const
        {
    return Vec3D(X * a, Y * a, Z * a);
}

/*!
 * \details Divides each element by a scalar
 * \param[in] a     the scalar to be divided by
 * \return          resulting vector
 */
Vec3D Vec3D::operator /(const Mdouble a) const
        {
    return Vec3D(X / a, Y / a, Z / a);
}

/*!
 * \details Adds a vector to itself
 * \param[in] a     vector to be added
 * \return          (reference to) itself, i.e. resulting vector
 */
Vec3D& Vec3D::operator+=(const Vec3D& a)
{
    X += a.X;
    Y += a.Y;
    Z += a.Z;
    return *this;
}

/*!
 * \details Subtracts a vector from itself
 * \param[in] a     vector to be subtracted
 * \return          (reference to) itself, i.e. resulting vector
 */
Vec3D& Vec3D::operator-=(const Vec3D& a)
{
    X -= a.X;
    Y -= a.Y;
    Z -= a.Z;
    return *this;
}

/*!
 * \details Multiplies each element by a scalar
 * \param[in] a     scalar to be multiplied by
 * \return          (reference to) itself, i.e. resulting vector
 */
Vec3D& Vec3D::operator*=(const Mdouble a)
{
    X *= a;
    Y *= a;
    Z *= a;
    return *this;
}

/*!
 * \details Divides each element by a scalar
 * \param[in] a     scalar to be divided by
 * \return          (reference to) itself, i.e. resulting vector
 */
Vec3D& Vec3D::operator/=(const Mdouble a)
{
    X /= a;
    Y /= a;
    Z /= a;
    return *this;
}

/*!
 * \details Calculates the dot product of two vectors.
 * NB: this is a STATIC function!
 * \param[in] a     the first vector
 * \param[in] b     the second vector 
 * \return          the resulting scalar
 */
Mdouble Vec3D::dot(const Vec3D& a, const Vec3D& b)
{
    return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
}

/*!
 * \details Calculates the pointwise maximum of two vectors.
 * NB: this is a STATIC function!
 * \param[in] a     the first vector
 * \param[in] b     the second vector 
 * \return          The resulting vector, in which each element is the maximum 
 *                  of the equivalent elements of the arguments
 */
Vec3D Vec3D::max(const Vec3D& a, const Vec3D& b)
{
    return Vec3D(std::max(a.X, b.X), std::max(a.Y, b.Y), std::max(a.Z, b.Z));
}

/*!
 * \details Calculates the pointwise minimum of two vectors.
 * NB: this is a STATIC function!
 * \param[in] a     the first vector
 * \param[in] b     the second vector 
 * \return          The resulting vector, in which each element is the minimum 
 *                  of the equivalent elements of the arguments
 */
Vec3D Vec3D::min(const Vec3D& a, const Vec3D& b)
{
    return Vec3D(std::min(a.X, b.X), std::min(a.Y, b.Y), std::min(a.Z, b.Z));
}

/*!
 * \details Calculates the pointwise square of the vector.
 * NB: this is a STATIC function!
 * \param[in] a     the vector to be squared.
 * \return          the resulting vector, of which each element is the square of
 *                  the equivalent element of the argument.
 */
Vec3D Vec3D::square(const Vec3D& a)
{
    return Vec3D(a.X * a.X, a.Y * a.Y, a.Z * a.Z);
}

/*!
 * \details Normalises the vector, i.e. divides all elements by the vectors length
 * (resulting in a vector in the same direction, but with unit length).
 */
void Vec3D::normalize()
{
    *this /= this->getLength();
}

/*!
 * \details Sets the length of the vector to a given scalar (while maintaining the
 * direction).
 * \param[in] length    the length to be set
 */
void Vec3D::setLength(Mdouble length)
{
    *this /= this->getLength() * length;
}

/*!
 * \details Calculates the pointwise square root of a given vector.
 * NB: this is a STATIC function!
 * \param[in] a     the vector to be pointwise square rooted
 * \return          the resulting vector, of which each element is the square root
 *                  of the equivalent element of the argument.
 */
Vec3D Vec3D::sqrt(const Vec3D& a)
{
    return Vec3D(std::sqrt(a.X), std::sqrt(a.Y), std::sqrt(a.Z));
}

/*!
 * \details Calculates the cross product of two vectors
 * NB: this is a STATIC function!
 * \param[in] a     the first vector
 * \param[in] b     the second vector 
 * \return          the cross product of the arguments
 */
Vec3D Vec3D::cross(const Vec3D& a, const Vec3D& b)
{
    return Vec3D(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X);
}

/*!
 * \details Calculates the distance (i.e. the length of the difference) between two vectors
 * NB: this is a STATIC function!
 * \param[in] a     the first vector
 * \param[in] b     the second vector 
 * \return          the distance between the two arguments.
 */
Mdouble Vec3D::getDistance(const Vec3D& a, const Vec3D& b)
{
    return std::sqrt(getDistanceSquared(a, b));
}

/*!
 * \details Calculates the square of the distance (i.e. the length of the difference)
 * between two vectors.
 * NB: this is a STATIC function!
 * \param[in] a     the first vector
 * \param[in] b     the second vector 
 * \return          the square of the distance between the two arguments.
 */
Mdouble Vec3D::getDistanceSquared(const Vec3D& a, const Vec3D& b)
{
    return ((a.X - b.X) * (a.X - b.X) + (a.Y - b.Y) * (a.Y - b.Y) + (a.Z - b.Z) * (a.Z - b.Z));
}

/*!
 * \details Calculates the square of the length of a given vector.
 * NB: this is a STATIC function!
 * \param[in] a     the vector.
 * \return          the square of the length of the argument.
 */
Mdouble Vec3D::getLengthSquared(const Vec3D& a)
{
    return (a.X * a.X + a.Y * a.Y + a.Z * a.Z);
}

/*!
 * \details Calculates the square of the length of itself
 * \return              the square of the length of this vector
 */
Mdouble Vec3D::getLengthSquared() const
{
    return (X * X + Y * Y + Z * Z);
}

/*!
 * \details returns the vector element belonging to the given index.
 * \param[in] index     the index of interest (should be 0, 1 or 2)
 * \return              the value of the vector element belonging to the given index
 */
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
            logger(ERROR, "[Vector::getComponent] Index = %, which is too high for a 3D vector (should be 0-2).", index);
            return 0;
    }
}

/*!
 * \details Sets the element of the vector belonging to the first argument 
 * (index) to the value given in the second argument (val).
 * \param[in] index     index of element of interest, 
 * \param[in] val       value to be set
 */
void Vec3D::setComponent(const int index, const double val)
{
    switch (index)
    {
        case 0:
            X = val;
            break;
        case 1:
            Y = val;
            break;
        case 2:
            Z = val;
            break;
        default:
            logger(ERROR, "[Vector::setComponent] Index = %, which is too high for a 3D vector (should be 0-2).", index);
    }
}

/*!
 * \details Transforms the (Cartesian) vector to cylindrical coordinates
 * \return              Transformed vector
 */
Vec3D Vec3D::getCylindricalCoordinates() const
{
    return Vec3D(std::sqrt(X * X + Y * Y), std::atan2(Y, X), Z);
}

/*!
 * \details Transforms the (cylindrical) vector to cartesian coordinates
 * \return              Transformed vector
 */
Vec3D Vec3D::getFromCylindricalCoordinates() const
{
    return Vec3D(X * std::cos(Y), X * std::sin(Y), Z);
}

/*!
 * \details Checks if the length of the vector is equal to the one given in the 
 * first argument (other), with a tolerance given in the second argument (tol).
 * \param[in] other     the 3D vector to check against
 * \param[in] tol       the tolerance
 * \return              returns TRUE if the difference between the lengths of this 
 *                      vector and that given in the first argument (other) is smaller than the 
 *                      given tolerance.
 */
bool Vec3D::isEqualTo(const Vec3D& other, const double tol) const
        {
    if ((Vec3D::getLengthSquared(*this - other)) <= tol * tol)
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

/*!
 * \details Calculates the length of this vector
 * \return          the (scalar) length of this vector
 */
Mdouble Vec3D::getLength() const
{
    return std::sqrt(getLengthSquared());
}

/*!
 * \details Calculates the length of a given vector
 * NB: this is a STATIC function!
 * \param[in] a     vector to be measured.
 * \return          length of the argument.
 */
Mdouble Vec3D::getLength(const Vec3D& a)
{
    return a.getLength();
}

/*!
 * \details Calculates the unit vector of a given vector (unless it is a vector 
 * with zero length; in that case it returns a 3D vector with each element equal
 * to zero).
 * NB: this is a STATIC function!
 * \param[in] a     the vector of interest
 * \return          unit vector in the direction of the argument (unless the 
 *                  argument has length zero; in that case a zero-vector).
 */
Vec3D Vec3D::getUnitVector(const Vec3D& a)
{
    Mdouble Length2 = a.getLengthSquared();
    if (Length2 != 0.0)
        return a / std::sqrt(Length2);
    else
        return Vec3D(0, 0, 0);
}

/*!
 * \details Adds all elements of the vector to an output stream.
 * NB: this is a global function and a friend of the Vec3D class!
 * \param[in] os    the output stream, 
 * \param[in] a     The vector of interest
 * \return          the output stream with vector elements added
 */
std::ostream& operator<<(std::ostream& os, const Vec3D& a)
{
    os << a.X << ' ' << a.Y << ' ' << a.Z;
    return os;
}

/*!
 * \details Reads all elements of a given vector from an input stream.
 * NB: this is a global function and a friend of the Vec3D class!
 * \param[in,out] is    the input stream
 * \param[in,out] a     the vector to be read in
 * \return              the input stream from which the vector elements were read
 */
std::istream& operator>>(std::istream& is, Vec3D& a)
{
    is >> a.X >> a.Y >> a.Z;
    return is;
}

/*!
 * \details Adds a scalar to the elements of given vector
 * NB this is a global function and a friend of the Vec3D class. Gets called when
 * addition operation of the form (Mdouble) + (Vec3D) is performed.
 * \param[in] a     the scalar to be added
 * \param[in] b     the vector the scalar gets added to.
 * \return          the resulting vector.
 */
Vec3D operator+(const Mdouble a, const Vec3D& b)
{
    return Vec3D(b.X + a, b.Y + a, b.Z + a);
}

/*!
 * \details Subtracts each element of a given vector from a scalar 
 * NB this is a global function and a friend of the Vec3D class. Gets called when
 * subtraction operation of the form (Mdouble) - (Vec3D) is performed.
 * \param[in] a     the scalar
 * \param[in] b     the vector to be subtracted the scalar gets subtracted from.
 * \return          the resulting vector.
 */
Vec3D operator-(const Mdouble a, const Vec3D& b)
{
    return Vec3D(a - b.X, a - b.Y, a - b.Z);
}

/*!
 * \details Returns the negative of a given vector.
 * NB: this is a global function and a friend of the Vec3D class. Gets called when
 * a negation operation of the form - (Vec3D) is performed. 
 * \param[in] a     the vector to be negated
 * \return          the negated vector
 */
Vec3D operator-(const Vec3D& a)
{
    return Vec3D(-a.X, -a.Y, -a.Z);
}

/*!
 * \details Multiplies each element of a given vector (b) by a given scalar (a).
 * NB: this is a global function and a friend of the Vec3D class. Gets called when
 * a scalar multiplication of the form (Mdouble) * (Vec3D) is performed.
 * \param[in] a     the scalar 
 * \param[in] b     the vector
 * \return          the resulting vector
 */
Vec3D operator*(const Mdouble a, const Vec3D& b)
{
    return Vec3D(b.X * a, b.Y * a, b.Z * a);
}
