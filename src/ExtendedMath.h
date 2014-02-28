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


#ifndef EXTENDEDMATH_H
#define EXTENDEDMATH_H

#ifdef HIGH_PRECISION
	typedef long double Mdouble;
#else 
	typedef double Mdouble;
#endif

#define sqr(a) ((a)*(a))
#define cubic(a) ((a)*(a)*(a))
#define UNUSED  __attribute__ ((__unused__))
//#define UNUSED
#include <algorithm>
//#include <cmath>

/*
/// \brief This is a global isnan function because isnan is not standard i.e. is not defined on a mac
inline bool isnan(double x)
{
    return (x != x);
}
 */

namespace constants
{
	//Values from WolframAlpha
	const Mdouble pi		=3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117068;
	const Mdouble sqrt_pi	=1.772453850905516027298167483341145182797549456122387128213807789852911284591032181374950656738544665;
	const Mdouble sqr_pi	=9.869604401089358618834490999876151135313699407240790626413349376220044822419205243001773403718552232;
	const Mdouble sqrt_2 	=1.414213562373095048801688724209698078569671875376948073176679737990732478462107038850387534327641573;
	const Mdouble sqrt_3	=1.732050807568877293527446341505872366942805253810380628055806979451933016908800037081146186757248576;
}

///Namespace for some extra maths function that are often needed	
namespace mathsFunc
{
	
	/// This is the gamma function returns the true value for the half integer value
	Mdouble gamma(Mdouble gamma_in);
	
	///This is a chi_squared function return the value x and degrees of freedom k
	Mdouble chi_squared(Mdouble x, int k);
	
	/// This is the function which actually gives the probability back using a chi squared test
	Mdouble chi_squared_prob(Mdouble x,int k);
    
    /// This is a sign function, it returns -1 for negative numbers, 1 for positive numbers and 0 for 0
    template <typename T> int sign(T val)
    {
        return (T(0) < val) - (val < T(0));
    }	
}

///Namespace for evaluating the zeroth modified Bessel function of the first kind, I0(x), required in StatisticsPoint.hcc
namespace besselFunc
{
	
	Mdouble chebyshev(Mdouble x, const Mdouble coef[], int N);
	
	Mdouble I0_exp(Mdouble x);
	
	Mdouble I0(Mdouble x);
	
}


namespace helperFunc
{
	
	///return type specifically for fuctions returning k and disp at once
	class KAndDisp {
	  public:
		Mdouble k;
		Mdouble disp;	
	};

	/// Set disp and k such that is matches a given collision time tc and 
	/// restitution coefficient r for a collision of effective/reduced 
	/// mass m. 
	KAndDisp computeKAndDispFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(Mdouble tc, Mdouble r, Mdouble mass);

	///return type specifically for fuctions returning k, disp, kt, dispt at once
	class KAndDispAndKtAndDispt {
	  public:
		Mdouble k;
		Mdouble disp;	
		Mdouble kt;
		Mdouble dispt;	
	};

	/// Set disp, k, dispt and kt such that is matches a given collision 
	/// time tc and a normal andtangential restitution coefficient r, 
	/// beta for a collision of effective/reduced mass m. 
	/// From Deen...Kuipers2006, eq. 43 and 30
	KAndDispAndKtAndDispt computeDisptFromCollisionTimeAndRestitutionCoefficientAndTangentialRestitutionCoefficientAndEffectiveMass(Mdouble tc, Mdouble r, Mdouble beta, Mdouble mass);


	///Calculates collision time for two copies of a particle of given disp, k, effective mass
	Mdouble getCollisionTime(Mdouble k, Mdouble disp, Mdouble mass);

	///Calculates restitution coefficient for two copies of given disp, k, effective mass
	Mdouble getRestitutionCoefficient(Mdouble k, Mdouble disp, Mdouble mass);

	///Calculates the maximum relative velocity allowed for a normal collision of two particles of radius r and particle mass m
	///(for higher velocities particles could pass through each other)
	Mdouble getMaximumVelocity(Mdouble k, Mdouble disp, Mdouble radius, Mdouble mass);

}



#include <sstream>
#include <iostream>
void getLineFromStringStream(std::istream& in, std::stringstream& out);

#endif
