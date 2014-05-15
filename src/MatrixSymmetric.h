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
