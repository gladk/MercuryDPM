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

#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>
#include <sstream>
#include "Vector.h"

/** Implementation of a 3D matrix.
 */
class Matrix3D
{
public:
	
	Mdouble XX, XY, XZ,  YX, YY, YZ,  ZX, ZY, ZZ;

	inline Matrix3D(void){}

	inline Matrix3D (const Mdouble xx, const Mdouble xy, const Mdouble xz, const Mdouble yx, const Mdouble yy, const Mdouble yz, const Mdouble zx, const Mdouble zy, const Mdouble zz)
		{	
			XX = xx; XY = xy; XZ = xz;
			YX = yx; YY = yy; YZ = yz;
			ZX = zx; ZY = zy; ZZ = zz;
		}

	inline void set_zero ()
	{	
		XX = XY = XZ = YX = YY = YZ = ZX = ZY = ZZ = 0.0;
	}

	inline Mdouble trace () const
	{	
		return (XX+YY+ZZ)/3;
	}

	inline Matrix3D operator + (const Matrix3D &A) const
	{ 
		return Matrix3D(XX + A.XX, XY + A.XY, XZ + A.XZ, 
						YX + A.YX, YY + A.YY, YZ + A.YZ, 
						ZX + A.ZX, ZY + A.ZY, ZZ + A.ZZ); 
	}

	inline Matrix3D operator - (const Matrix3D &A) const
	{ 
		return Matrix3D(XX - A.XX, XY - A.XY, XZ - A.XZ, 
						YX - A.YX, YY - A.YY, YZ - A.YZ, 
						ZX - A.ZX, ZY - A.ZY, ZZ - A.ZZ); 
	}

	inline Matrix3D operator + (const Mdouble A) const
	{ 
		return Matrix3D(XX + A, XY + A, XZ + A, 
						YX + A, YY + A, YZ + A, 
						ZX + A, ZY + A, ZZ + A); 
	}
	
	inline Matrix3D operator - (const Mdouble A) const
	{ 
		return Matrix3D(XX - A, XY - A, XZ - A, 
						YX - A, YY - A, YZ - A, 
						ZX - A, ZY - A, ZZ - A); 
	}
	
	friend inline Vec3D operator * (const Matrix3D A, const Vec3D B)
	{ 
		return Vec3D(A.XX*B.X+A.XY*B.Y+A.XZ*B.Z, 
					 A.YX*B.X+A.YY*B.Y+A.YZ*B.Z, 
					 A.ZX*B.X+A.ZY*B.Y+A.ZZ*B.Z); 
	}		
	
	inline Matrix3D operator * (const Mdouble A) const
	{ 
		return Matrix3D(XX * A, XY * A, XZ * A, 
						YX * A, YY * A, YZ * A, 
						ZX * A, ZY * A, ZZ * A); 
	}
		
	inline Vec3D operator * (const Vec3D A) const
	{ 
		return Vec3D(XX * A.X + XY * A.Y + XZ * A.Z, 
		             YX * A.X + YY * A.Y + YZ * A.Z,
		             ZX * A.X + ZY * A.Y + ZZ * A.Z );
	}
	
	inline Matrix3D operator / (const Mdouble A) const
	{ 
		return Matrix3D(XX / A, XY / A, XZ / A, 
						YX / A, YY / A, YZ / A, 
						ZX / A, ZY / A, ZZ / A); 
	}
	
	friend inline std::ostream& operator<<(std::ostream& os, const Matrix3D &A)
	{
		os 	<< A.XX << ' ' << A.XY << ' ' << A.XZ << ' ' 
			<< A.YX << ' ' << A.YY << ' ' << A.YZ << ' '
			<< A.ZX << ' ' << A.ZY << ' ' << A.ZZ;
		return os;
	}
		
	friend inline std::istream& operator>>(std::istream& is, Matrix3D &A)
	{
		is >> A.XX >> A.XY >> A.XZ >> A.YX >> A.YY >> A.YZ >> A.ZX >> A.ZY >> A.ZZ;
		return is;
	}

	inline Matrix3D& operator+=(const Matrix3D &A)
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
		
	inline Matrix3D& operator-=(const Matrix3D &A)
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

	inline Matrix3D& operator/=(const Mdouble A)
	{
		XX /= A;
		XY /= A;
		XZ /= A;
		YX /= A;
		YY /= A;
		YZ /= A;
		ZX /= A;
		ZY /= A;
		ZZ /= A;
		return *this;
	}
		
	// Pointwise square 
	friend inline Matrix3D square(const Matrix3D &A)
	{
		return Matrix3D(sqr(A.XX), sqr(A.XY), sqr(A.XZ), 
						sqr(A.YX), sqr(A.YY), sqr(A.YZ), 
						sqr(A.ZX), sqr(A.ZY), sqr(A.ZZ));
	}

	// Pointwise square root
	friend inline Matrix3D sqrt(const Matrix3D &A)
	{
		return Matrix3D(sqrt(A.XX), sqrt(A.XY), sqrt(A.XZ), 
						sqrt(A.YX), sqrt(A.YY), sqrt(A.YZ), 
						sqrt(A.ZX), sqrt(A.ZY), sqrt(A.ZZ));
	}

};

///calculates the dyadic product (\f$ A \otimes B \f$)
Matrix3D Dyadic(Vec3D A, Vec3D B) {
	return Matrix3D(A.X*B.X, A.X*B.Y, A.X*B.Z, 
					A.Y*B.X, A.Y*B.Y, A.Y*B.Z, 
					A.Z*B.X, A.Z*B.Y, A.Z*B.Z);
}

// Cross product
Matrix3D Cross(const Vec3D &A, const Matrix3D &B)
{
    return Matrix3D(
    A.Y*B.ZX-A.Z*B.YX, A.Y*B.ZY-A.Z*B.YY, A.Y*B.ZZ-A.Z*B.YZ, 
    A.Z*B.XX-A.X*B.ZX, A.Z*B.XY-A.X*B.ZY, A.Z*B.XZ-A.X*B.ZZ, 
    A.X*B.YX-A.Y*B.XX, A.X*B.YY-A.Y*B.XY, A.X*B.YZ-A.Y*B.XZ);
}

#endif
