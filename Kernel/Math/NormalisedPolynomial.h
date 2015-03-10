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

#ifndef POLY_H
#define POLY_H

#include <cmath>
#include <sstream>
#include <iomanip> //

/*!
 * \class NORMALIZED_POLYNOMIAL
 * \brief This class is used to define polynomial axisymmetric coarse-graining functions.
 * 
 * \details This class stores a polynomial, \f$p(r)=\sum_{i=0}^N c_i r^{N-i}\f$, which is normalized such that the integral over the unit sphere of the axisymmetric function \f$p(|\vec{x}|)\f$ is unity.\n
 * 
 * Use #set_polynomial to define the polynomial.
 * Use #evaluate to evaluate the polynomial.
 * Use #evaluateGradient to evaluate the polynomial's gradient.\n
 * 
 * Calculations can be found in src/docs/Polynomials.nb \n
 * 
 * This is used to define polynomial axisymmetric coarse-graining functions (see StatisticsVector).\n
 * 
 * Note: not everything is implemented yet: only dim=3 is working, no 
 * gradients are computed.
 */
template<StatType T>
class NORMALIZED_POLYNOMIAL
{
    
//definition of member variables
    
private:
    
    /*!
     * \brief Contains the name of the polynomial which will be displayed as CGtype by the statistical code.
     */
    std::string name;

    /*!
     * \brief The system dimension.
     */
    unsigned int dim;

    /*!
     * \brief Stores the coefficients \f$c_i\f$.
     */
    std::vector<Mdouble> coefficients; //starting with the highest order
    
    /*!
     * \brief Stores some coefficients used in #evaluate and #evaluateIntegral for StatTypes different from XYZ.
     */
    std::vector<Mdouble> averaged_coefficients;

//definition of member functions
    
public:
    
    /*!
     * \brief Basic constructor; note that this does not determine the particular polynomial; one needs to call #set_polynomial to define the #coefficients.
     */
    NORMALIZED_POLYNOMIAL()
    {
        setName("Polynomial");
        coefficients.resize(0);
        dim = 0;
    }
    
    /*!
     * \brief Use this function to set the polynomial #coefficients \f$c_i\f$. This function calls #finish_set_polynomial to normalize the #coefficients.
     */
    void set_polynomial(std::vector<Mdouble> new_coefficients, unsigned int new_dim);

    /*!
     * \brief Some as #set_polynomial, but avoids the use of a vector
     */
    void set_polynomial(Mdouble* new_coefficients, unsigned int num_coeff, unsigned int new_dim);

    /*!
     * \brief Use this function to change the #name of the polynomial
     */
    void setName(const char* new_name)
    {
        name = new_name;
    }
    
    /*!
     * \brief Returns #name of the polynomial
     */
    std::string getName()
    {
        return name;
    }
    
    /*!
     * \brief Returns the value of the polynomial, \f$p(r)=\sum_{i=0}^N c_i r^{N-i}\f$. 
     * 
     * \details For averaged #StatType this function is templated.
     * If averaging statistics are used, then an averaged function is stored as well; for averaging a  over certain dimensions is stored as well. 
     * 
     * For averaging over two dimensions,
     * \f$(y_{max}-y_{min})\cdot (z_{max}-z_{min})\cdot \bar{p}(x)=\int_{\vec{x}\leq1} p(|\vec{x}|) dy\,dz = \sum_{i=0}^N \bar{c}_i r^{N-i}\f$.
     * 
     * For averaging over one dimensions,
     * \f$(z_{max}-z_{min})\cdot \bar{p}(x,y)=\int_{\vec{x}\leq1} p(|\vec{x}|) dz = \sum_{i=0}^N \bar{c}_i r^{N-i}\f$.
     */
    Mdouble evaluate(Mdouble r);

    /*!
     * \brief Returns the gradient of the polynomial, \f$\partial_\alpha p(x,y,z)=\sum_{i=0}^N c_{i,\alpha} r^{N-i},\ \alpha=x,y,z\f$
     */
    Mdouble evaluateGradient(Mdouble r);

    /*!
     * \brief Returns the value of the line integral along the normal P1P2 "from a to b" over the axisymmetric function 
     *
     * \image html averaging.jpeg "circle denotes the cutoff radius of the cg function around P"
     * For averaged #StatType this function is templated.
     */
    Mdouble evaluateIntegral(Mdouble a, Mdouble b, Mdouble t);

    /*!
     * \brief Returns the order of the polynomial
     */
    int getOrder(void)
    {
        return coefficients.size() - 1;
    }
    
    /*!
     * \brief Returns a text description of the polynomial
     */
    friend inline std::ostream& operator<<(std::ostream& os, const NORMALIZED_POLYNOMIAL &P)
    {
        unsigned int N = P.coefficients.size();
        for (unsigned int i = 0; i < N; i++)
        {
            if (P[i]==0.0)
                continue;
            if (P[i] >= 0)
                os << "+";
            os << std::setprecision(2) << P[i];
            if (N - 1 - i > 1)
                os << "r^" << N - 1 - i;
            else if (N - 1 - i == 1)
                os << "r";
        }
        return os;
    }
    
private:
    /*!
     * \brief Normalizes the polynomial #coefficients \f$c_i\f$ such that the integral over the unit sphere of the axisymmetric function \f$p(r)\f$ is unity.
     * \details \f$\int_0^1 f(r) p(r) dr = 1\f$, with \f$f(r)=4\pi r^2\f$ for 3D, \f$f(r)=2\pi r\f$ for 2D, \f$f(r)=2\f$ for 1D systems. 
     * 
     * Also sets #averaged_coefficients
     * 
     * Assumes that #dim and #coefficients are already set.
     */
    void finish_set_polynomial();

    /*!
     * \brief Returns the integral over the unit sphere of the axisymmetric function \f$p(r)\f$.
     * \details \f$\int_{|\vec{x}|\leq1} p(|\vec{x}|) d\vec{x} = \int_0^1 f(r) p(r) dr = 1\f$, with \f$f(r)=4\pi r^2\f$ for 3D, \f$f(r)=2\pi r\f$ for 2D, \f$f(r)=2\f$ for 1D systems. 
     * 
     * For \f$p(r)=\sum_{i=0}^{N-1} c_i r^{N-1-i}\f$, we obtain
     * \f$V = \sum_{i=0}^{N-1} 4\pi c_i/(2+N-i)\f$ for 3D,
     * \f$V = \sum_{i=0}^{N-1} 2\pi c_i/(1+N-i)\f$ for 2D, 
     * \f$V = \sum_{i=0}^{N-1} 2 c_i/(N-i)\f$ for 1D systems.
     */
    Mdouble get_volume();

    /*!
     * \brief Returns the value of the polynomial averaged over 2 dimensions. For #StatType=X, \f$r=|x|\f$. See also #set_average_1D.
     */
    Mdouble evaluate_1D(Mdouble r);

    /*!
     * \brief Returns the value of the polynomial averaged over 1 dimension. For #StatType=XY, \f$r=\sqrt{x^2+y^2}\f$.
     */
    Mdouble evaluate_2D(Mdouble r);

    /*!
     * \brief Sets #averaged_coefficients. 
     * \details This function is templated, with the default used only for StatType=XYZ, so it does nothing.
     */
    void set_average();

    /*!
     * \brief Sets #averaged_coefficients \f$\bar{c}_i\f$ for #StatType=X,Y,Z such that \f$\sum_{i=0}^N \bar{c}_i x^{N-i} = \int\int_{|\vec{x}|\leq 1} p(|\vec{x}|) dy dz\f$. 
     * See #evaluate_1D.
     */
    void set_average_1D();

    /*!
     * \brief For #StatType=XY,XZ,XZ, #averaged_coefficients is not used since \f$\bar{p}(r)\f$ can be evaluated as a function of \f$c_i\f$. 
     * See #evaluate_2D.
     */
    void set_average_2D();

    /*!
     * \brief Returns the value of the gradient averaged over 2 dimensions.
     */
    Mdouble evaluateGradient_1D(Mdouble r);

    /*!
     * \brief Returns the value of the gradient averaged over 1 dimensions.
     */
    Mdouble evaluateGradient_2D(Mdouble r);

    /*!
     * \brief Returns the value of the line integral along the normal P1P2 "from a to b" over the axisymmetric function averaged over 2 dimensions
     */
    Mdouble evaluateIntegral_1D(Mdouble a, Mdouble b, Mdouble t);

    /*!
     * \brief Returns the value of the line integral along the normal P1P2 "from a to b" over the axisymmetric function averaged over 1 dimensions
     */
    Mdouble evaluateIntegral_2D(Mdouble a, Mdouble b, Mdouble t);

    /*!
     * \brief Access to the #coefficients
     */
    Mdouble operator[](int i) const
    {
        return coefficients[i];
    }
    
};
#include "NormalisedPolynomial.hcc"
#endif

