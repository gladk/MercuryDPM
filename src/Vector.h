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

/** Implementation of a 3D vector (by Vitaliy).
 */
///Modifications
/// 21:9:2009 - Added the inclusion guards and some include objects
/// \todo Need to generise this to n-dimensional vectors of any type
#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>
#include <sstream>
#include <iostream>
#include <stdlib.h> 
#include "ExtendedMath.h"

class Vec3D
{
public:

	Mdouble X, Y, Z;

	inline Vec3D(void){
        set_zero();
    }

	inline Vec3D (const Mdouble x, const Mdouble y, const Mdouble z)
	{	
		X = x; Y = y; Z = z;
	}

	inline void set_zero ()
	{	
		X = 0.0; Y = 0.0; Z = 0.0;
	}

	inline bool is_zero ()
	{	
		return X == 0.0 && Y == 0.0 && Z == 0.0;
	}

	inline Vec3D operator + (const Vec3D &A) const
	{ 
		return Vec3D(X + A.X, Y + A.Y, Z + A.Z); 
	}

	inline Vec3D operator - (const Vec3D &A) const
	{
		return Vec3D(X - A.X, Y - A.Y, Z - A.Z);
	}

	inline Vec3D operator + (const Mdouble A) const
	{ 
		return Vec3D(X + A, Y + A, Z + A); 
	}

	inline Vec3D operator - (const Mdouble A) const
	{ 
		return Vec3D(X - A, Y - A, Z - A); 
	}

	inline Vec3D operator * (const Mdouble A) const
	{ 
		return Vec3D(X * A, Y * A, Z * A); 
	}

	inline Vec3D operator / (const Mdouble A) const
	{ 
		return Vec3D(X / A, Y / A, Z / A); 
	}

	inline Vec3D& operator+=(const Vec3D &A)
	{
		X += A.X;
		Y += A.Y;
		Z += A.Z;
		return *this;
	}

	inline Vec3D& operator-=(const Vec3D &A)
	{
		X -= A.X;
		Y -= A.Y;
		Z -= A.Z;
		return *this;
	}

	inline Vec3D& operator*=(const Mdouble a)
	{
		X *= a;
		Y *= a;
		Z *= a;
		return *this;
	}

	inline Vec3D& operator/=(const Mdouble a)
	{
		X /= a;
		Y /= a;
		Z /= a;
		return *this;
	}

	// Dot product
	friend inline Mdouble Dot(const Vec3D &A, const Vec3D &B)
	{
		return A.X * B.X + A.Y * B.Y + A.Z * B.Z;
	}

	// Piecewise max
	friend inline Vec3D max(const Vec3D &A, const Vec3D &B)
	{
		return Vec3D(std::max(A.X,B.X), std::max(A.Y,B.Y), std::max(A.Z,B.Z));
	}

	// Piecewise min
	friend inline Vec3D min(const Vec3D &A, const Vec3D &B)
	{
		return Vec3D(std::min(A.X,B.X), std::min(A.Y,B.Y), std::min(A.Z,B.Z));
	}

	// Pointwise square 
	friend inline Vec3D square(const Vec3D &A)
	{
		return Vec3D(A.X * A.X, A.Y * A.Y, A.Z * A.Z);
	}

	// make this vector unit length 
	inline void normalize() {
		*this  /= this->GetLength();
	}

	// make this vector unit length 
	inline void SetLength(Mdouble length) {
		*this  /= this->GetLength()*length;
	}

	// Pointwise square root
	friend inline Vec3D sqrt(const Vec3D &A)
	{
		return Vec3D(sqrt(A.X), sqrt(A.Y), sqrt(A.Z));
	}

	// Cross product
	friend inline Vec3D Cross(const Vec3D &A, const Vec3D &B)
	{
		return Vec3D(A.Y*B.Z-A.Z*B.Y, A.Z*B.X-A.X*B.Z, A.X*B.Y-A.Y*B.X);
	}

	friend inline Mdouble GetDistance(const Vec3D &A, const Vec3D &B)
	{
		return sqrt(GetDistance2(A, B));
	}

	friend inline Mdouble GetDistance2(const Vec3D &A, const Vec3D &B)
	{
		return ((A.X - B.X) * (A.X - B.X) + (A.Y - B.Y) * (A.Y - B.Y) + (A.Z - B.Z) * (A.Z - B.Z));
	}
	
	friend Mdouble GetLength2(const Vec3D &A)
	{
		return (A.X * A.X + A.Y * A.Y + A.Z * A.Z);
	}

	Mdouble GetLength2() const
	{
		return (X * X + Y * Y + Z * Z);
	}
    
    Mdouble getComponent(const int index) const
    {
        switch(index)
        {
            case 0:
                return X;
            case 1:
                return Y;
            case 2:
                return Z;
            default:
                std::cerr<<"Index="<<index<<" is to high for 3D vector"<<std::endl;
                exit(-1);
        }
    }
    
    void setComponent(int index,double val)
    {
        switch(index)
        {
            case 0:
                X=val;
                return;
            case 1:
                Y=val;
                return;
            case 2:
                Z=val;
                return;
            default:
                std::cerr<<"Index="<<index<<" is to high for 3D vector"<<std::endl;
                exit(-1);
        }
    }    

	Vec3D GetCylindricalCoordinates() const
	{
		return Vec3D(sqrt(X * X + Y * Y), atan2(Y, X), Z);
	}

	Vec3D GetFromCylindricalCoordinates()
	{
		return Vec3D(X * cos(Y), X * sin(Y), Z);
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

	inline Mdouble GetLength() const
	{
		return sqrt(GetLength2());
	}

	friend inline Mdouble GetLength(const Vec3D &A)
	{
		return A.GetLength();
	}
	
	friend inline Vec3D GetUnitVector(const Vec3D &A)
	{
		Mdouble Length2 = A.GetLength2();
		if (Length2) return A/sqrt(Length2);
		else return Vec3D(0,0,0);
	}
	
	friend inline std::ostream& operator<<(std::ostream& os, const Vec3D &A)
	{
		os << A.X << ' ' << A.Y << ' ' << A.Z;
		return os;
	}
	
	friend inline std::istream& operator>>(std::istream& is, Vec3D &A)
	{
		is >> A.X >> A.Y >> A.Z;
		return is;
	}
	
	friend inline Vec3D operator+ (const Mdouble& a, const Vec3D &A) {return Vec3D(A.X + a, A.Y + a, A.Z + a);} 
	friend inline Vec3D operator- (const Mdouble& a, const Vec3D &A) {return Vec3D(A.X - a, A.Y - a, A.Z - a);} 
	friend inline Vec3D operator- (const Vec3D &A) {return Vec3D(-A.X, -A.Y, -A.Z);} 
	friend inline Vec3D operator* (const Mdouble& a, const Vec3D &A) {return Vec3D(A.X * a, A.Y * a, A.Z * a);} 	
	
};

#endif
