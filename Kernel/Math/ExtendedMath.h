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

#define UNUSED  __attribute__ ((__unused__))

#include <iostream> //std::istream and std::stringstream
#include <fstream> //std::fstream
#include <cmath> 
#include <limits>

/*
 * \brief
 */
namespace constants
{
    //Values from WolframAlpha
    const Mdouble pi = 3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117068;
    const Mdouble sqrt_pi = 1.772453850905516027298167483341145182797549456122387128213807789852911284591032181374950656738544665;
    const Mdouble sqr_pi = 9.869604401089358618834490999876151135313699407240790626413349376220044822419205243001773403718552232;
    const Mdouble sqrt_2 = 1.414213562373095048801688724209698078569671875376948073176679737990732478462107038850387534327641573;
    const Mdouble sqrt_3 = 1.732050807568877293527446341505872366942805253810380628055806979451933016908800037081146186757248576;
}

/*!
 * \brief Namespace for some extra maths function that are often needed	
 */
namespace mathsFunc
{
    /*!
     * \brief This is the gamma function returns the true value for the half integer value
     */
    Mdouble gamma(Mdouble gamma_in);
    
    /*!
     * \brief This is a chi_squared function return the value x and degrees of freedom k
     */
    Mdouble chi_squared(const Mdouble x, const unsigned int k);
    
    /*!
     * \brief This is the function which actually gives the probability back using a chi squared test
     */
    Mdouble chi_squared_prob(const Mdouble x, const unsigned int k);
    
    /*!
     * \brief This function performs a golden section search to find the location of the minimum of a function
     * \param[in] function A function pointer to the function of which you want to calculate the location of its minimum.
     * \param[in] min The minimum location
     * \param[in] cur The current location
     * \param[in] max The maximum location
     * \param[in] endCondition The algorithm terminates when abs(max - min) < endCondition
     * \param[in] curVal The value of the function at the current location (on default this value is calculated internally)
     */
    double goldenSectionSearch(double(*function)(const double), double min, double cur, double max, double endCondition, double curVal = std::numeric_limits<Mdouble>::quiet_NaN());

    /*!
     * \brief This is a sign function, it returns -1 for negative numbers, 1 for positive numbers and 0 for 0
     */
    template<typename T> int sign(T val)
    {
        return (T(0) < val) - (val < T(0));
    }

    /*!
     * \brief squares a number
     */
    template<typename T> T square(T val)
    {
        return val*val;
    }

    /*!
     * \brief calculates the cube of a number
     */
    template<typename T> T cubic(T val)
    {
        return val*val*val;
    }

    /*!
     * \brief Compares the difference of two objects with an absolute error, useful in UnitTests.
     */
    template<typename T> bool compare(T v1, T v2, double absError)
    {
        return std::abs(v1-v2)>absError;
    }

    /*!
     * \brief factorial function
     */
    template <typename T> constexpr T factorial( const T t ) {

        return ( t == 0 ) ? 1 : t * factorial( t - 1 );

    }

}

    /*!
     * \brief Namespace for evaluating the zeroth modified Bessel function of the first kind, I0(x), required in StatisticsPoint.hcc
     */
namespace besselFunc
{
    
    Mdouble chebyshev(Mdouble x, const Mdouble coef[], int N);
    
    Mdouble I0_exp(Mdouble x);
    
    Mdouble I0(Mdouble x);

}

#endif
