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

#ifndef MATRIXSYMMETRIC_H
#define MATRIXSYMMETRIC_H

#include <cmath>
#include <sstream>
#include "Matrix.h"

/** Implementation of a 3D symmetric matrix.
 */
class MatrixSymmetric3D
{
public:
	
	Mdouble XX, XY, XZ,  YY, YZ,  ZZ;

    operator Matrix3D() { return Matrix3D(XX, XY, XZ, XY, YY, YZ, XZ, YZ, ZZ); };

	inline MatrixSymmetric3D(void){}

	inline MatrixSymmetric3D (const Mdouble xx, const Mdouble xy, const Mdouble xz, const Mdouble yy, const Mdouble yz, const Mdouble zz)
		{	
			XX = xx; XY = xy; XZ = xz;
			YY = yy; YZ = yz;
			ZZ = zz;
		}

	inline void set_zero ()
		{	
		XX = XY = XZ = YY = YZ = ZZ = 0.0;
		}

	inline Mdouble trace () const
	{	
		return (XX+YY+ZZ)/3;
	}

	inline MatrixSymmetric3D operator + (const MatrixSymmetric3D &A) const
	{ 
		return MatrixSymmetric3D(XX + A.XX, XY + A.XY, XZ + A.XZ, 
										YY + A.YY, YZ + A.YZ, ZZ + A.ZZ); 
	}

	inline MatrixSymmetric3D operator - (const MatrixSymmetric3D &A) const
		{ 
		return MatrixSymmetric3D(XX - A.XX, XY - A.XY, XZ - A.XZ, 
										YY - A.YY, YZ - A.YZ, ZZ - A.ZZ); 
		}

	inline MatrixSymmetric3D operator + (const Mdouble A) const
	{ 
		return MatrixSymmetric3D(XX + A, XY + A, XZ + A, 
		                         YY + A, YZ + A, ZZ + A); 
	}
	
	inline MatrixSymmetric3D operator - (const Mdouble A) const
	{ 
		return MatrixSymmetric3D(XX - A, XY - A, XZ - A, 
		                         YY - A, YZ - A, ZZ - A); 
	}
	
	friend inline Vec3D operator * (const MatrixSymmetric3D A, const Vec3D B)
	{ 
		return Vec3D(A.XX*B.X+A.XY*B.Y+A.XZ*B.Z, 
					 A.XY*B.X+A.YY*B.Y+A.YZ*B.Z, 
					 A.XZ*B.X+A.YZ*B.Y+A.ZZ*B.Z); 
	}	

	inline MatrixSymmetric3D operator * (const Mdouble A) const
	{ 
		return MatrixSymmetric3D(XX * A, XY * A, XZ * A, 
		                         YY * A, YZ * A, ZZ * A); 
	}
	
	inline MatrixSymmetric3D operator / (const Mdouble A) const
	{ 
		return MatrixSymmetric3D(XX / A, XY / A, XZ / A, 
		                         YY / A, YZ / A, ZZ / A); 
	}
	
	friend inline std::ostream& operator<<(std::ostream& os, const MatrixSymmetric3D &A)
	{
		os << A.XX << ' ' << A.XY << ' ' << A.XZ << " " << A.YY << ' ' << A.YZ << " " << A.ZZ;
		return os;
	}
		
	friend inline std::istream& operator>>(std::istream& is, MatrixSymmetric3D &A)
	{
		is >> A.XX >> A.XY >> A.XZ >> A.YY >> A.YZ >> A.ZZ;
		return is;
	}

	inline MatrixSymmetric3D& operator+=(const MatrixSymmetric3D &A)
	{
		XX += A.XX;
		XY += A.XY;
		XZ += A.XZ;
		YY += A.YY;
		YZ += A.YZ;
		ZZ += A.ZZ;
		return *this;
	}
		
	inline MatrixSymmetric3D& operator-=(const MatrixSymmetric3D &A)
	{
		XX -= A.XX;
		XY -= A.XY;
		XZ -= A.XZ;
		YY -= A.YY;
		YZ -= A.YZ;
		ZZ -= A.ZZ;
		return *this;
	}

	inline MatrixSymmetric3D& operator/=(const Mdouble a)
	{
		XX /= a;
		XY /= a;
		XZ /= a;
		YY /= a;
		YZ /= a;
		ZZ /= a;
		return *this;
	}
		
	// Pointwise square 
	friend inline MatrixSymmetric3D square(const MatrixSymmetric3D &A)
	{
		return MatrixSymmetric3D(sqr(A.XX), sqr(A.XY), sqr(A.XZ), sqr(A.YY), sqr(A.YZ), sqr(A.ZZ));
	}

	// Pointwise square root
	friend inline MatrixSymmetric3D sqrt(const MatrixSymmetric3D &A)
	{
		return MatrixSymmetric3D(sqrt(A.XX), sqrt(A.XY), sqrt(A.XZ), sqrt(A.YY), sqrt(A.YZ), sqrt(A.ZZ));
	}

	//calculates A*A'
	//friend inline MatrixSymmetric3D Dyadic(Vec3D A) {
	//	return MatrixSymmetric3D(A.X*A.X, A.X*A.Y, A.X*A.Z, A.Y*A.Y, A.Y*A.Z, A.Z*A.Z);
	//}
};

///calculates the symmetrized dyadic product (\f$ (A \otimes B + B \otimes A)/2 \f$)
MatrixSymmetric3D SymmetrizedDyadic(Vec3D A, Vec3D B) {
	return MatrixSymmetric3D(A.X*B.X, 0.5*(A.X*B.Y+B.X*A.Y), 0.5*(A.X*B.Z+B.X*A.Z), A.Y*B.Y, 0.5*(A.Y*B.Z+B.Y*A.Z), A.Z*B.Z);
}


#endif
