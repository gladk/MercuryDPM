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

#ifndef EXTENDEDMATH_H
#define EXTENDEDMATH_H

#include <iostream> //std::istream and std::stringstream
#include <fstream> //std::fstream
#include <cmath> 
#include <limits>

#include "Vector.h"

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
    double goldenSectionSearch(double (*function)(const double), double min, double cur, double max, double endCondition, double curVal = std::numeric_limits<Mdouble>::quiet_NaN());

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
        return val * val;
    }

    /*!
     * \brief calculates the cube of a number
     */
    template<typename T> T cubic(T val)
    {
        return val * val * val;
    }

    /*!
     * \brief Compares the difference of two Mdouble with an absolute error, useful in UnitTests
     * \param[in] v1 The first Mdouble
     * \param[in] v2 The second Mdouble
     * \param[in] absError The allowed maximum absolute error
     * \return False if the two Mdouble are equal
     */
    bool isEqual(Mdouble v1,Mdouble v2, double absError);
    /*!
     * \brief Compares the difference of two Vec3D with an absolute error, useful in UnitTests
     * \param[in] v1 The first Vec3D
     * \param[in] v2 The second Vec3D
     * \param[in] absError The allowed maximum absolute error
     * \return true if the two Vec3D are equal
     */
    bool isEqual(Vec3D v1, Vec3D v2, double absError);

    /*!
     * \brief factorial function
     */
    template<typename T> constexpr T factorial(const T t)
    {
        return (t == 0) ? 1 : t * factorial(t - 1);
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
