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

#ifndef POLY_H
#define POLY_H

#include <cmath>
#include <sstream>

/**This class is used to define polynomial axisymmetric coarse-graining functions.
 * 
 * This class stores a polynomial, \f$p(r)=\sum_{i=0}^N c_i r^{N-i}\f$, which is 
 * normalized such that the integral over the unit sphere of the axisymmetric 
 * function \f$p(|\vec{x}|)\f$ is unity.
 * 
 * Use #set_polynomial to define the polynomial.
 * Use #evaluate to evaluate the polynomial.
 * Use #evaluateGradient to evaluate the polynomial's gradient.
 *   
 * 
 * 
 * Calculations can be found in src/docs/Polynomials.nb
 * 
 * This is used to define polynomial axisymmetric coarse-graining functions (see StatisticsVector).
 * 
 * Note: not everything is implemented yet: only dim=3 is working, no 
 * gradients are computed.
 */
template <StatType T>
class NORMALIZED_POLYNOMIAL
{

//definition of member variables

private:

	///Contains the name of the polynomial which will be displayed as CGtype by the statistical code.
	std::string name;
	
	///The system dimension.
	unsigned int dim;
	
	///Stores the coefficients \f$c_i\f$.
	std::vector<Mdouble> coefficients; //starting with the highest order
	
	///Stores some coefficients used in #evaluate and #evaluateIntegral for StatTypes different from XYZ.
	std::vector<Mdouble> averaged_coefficients;

//definition of member functions

public:

	///Basic constructor; note that this does not determine the particular polynomial; one needs to call #set_polynomial to define the #coefficients.
	NORMALIZED_POLYNOMIAL () {	
		set_name("Polynomial");
		coefficients.resize(0);
		dim = 0;
	}

	///Use this function to set the polynomial #coefficients \f$c_i\f$. 
	///This function calls #finish_set_polynomial to normalize the #coefficients.
	void set_polynomial(std::vector<Mdouble> new_coefficients, unsigned int new_dim);

	///Some as #set_polynomial, but avoids the use of a vector
	void set_polynomial(Mdouble* new_coefficients, unsigned int num_coeff, unsigned int new_dim);

	///Use this function to change the #name of the polynomial
	void set_name(const char* new_name) {name = new_name;}

	///Returns #name of the polynomial
	std::string get_name() {return name;}

	/** Returns the value of the polynomial, \f$p(r)=\sum_{i=0}^N c_i r^{N-i}\f$. 
	 * 
	 * For averaged #StatType this function is templated.
	 * If averaging statistics are used, then an averaged function is stored as well; for averaging a  over certain dimensions is stored as well. 
	 * 
	 * For averaging over two dimensions,
	 * \f$(y_{max}-y_{min})\cdot (z_{max}-z_{min})\cdot \bar{p}(x)=\int_{\vec{x}\leq1} p(|\vec{x}|) dy\,dz = \sum_{i=0}^N \bar{c}_i r^{N-i}\f$.
	 * 
	 * For averaging over one dimensions,
	 * \f$(z_{max}-z_{min})\cdot \bar{p}(x,y)=\int_{\vec{x}\leq1} p(|\vec{x}|) dz = \sum_{i=0}^N \bar{c}_i r^{N-i}\f$.
	 **/
	Mdouble evaluate(Mdouble r);

	///Returns the gradient of the polynomial, \f$\partial_\alpha p(x,y,z)=\sum_{i=0}^N c_{i,\alpha} r^{N-i},\ \alpha=x,y,z\f$
	Mdouble evaluateGradient(Mdouble r);
	
	///Returns the value of the line integral along the normal P1P2 "from a to b" over the axisymmetric function 
	///\image html averaging.jpeg "circle denotes the cutoff radius of the cg function around P"
	///For averaged #StatType this function is templated.
	Mdouble evaluateIntegral(Mdouble a, Mdouble b, Mdouble t);

	///Returns the order of the polynomial
	int get_Order (void) {
	  return coefficients.size()-1;
	}

	///Returns a text description of the polynomial
	friend inline std::ostream& operator<<(std::ostream& os, const NORMALIZED_POLYNOMIAL &P) {
		unsigned int N = P.coefficients.size();
		for (unsigned int i=0; i<N; i++) {
			if (!P[i]) continue;
			if (P[i]>=0) os << "+";
			os << std::setprecision(2) << P[i];
			if (N-1-i>1) os << "r^" << N-1-i; 
			else if (N-1-i==1) os << "r"; 
		}
		return os;
	}

private:
	/** Normalizes the polynomial #coefficients \f$c_i\f$ such that the 
	 * integral over the unit sphere of the axisymmetric function 
	 * \f$p(r)\f$ is unity.
	 * In detail, \f$\int_0^1 f(r) p(r) dr = 1\f$, 
	 * with \f$f(r)=4\pi r^2\f$ for 3D, \f$f(r)=2\pi r\f$ for 2D, 
	 * \f$f(r)=2\f$ for 1D systems. 
	 * 
	 * Also sets #averaged_coefficients
	 * 
	 * Assumes that #dim and #coefficients are already set.
	 */
	void finish_set_polynomial();

	/** Returns the integral over the unit sphere of the axisymmetric function \f$p(r)\f$.
	 * In detail, \f$\int_{|\vec{x}|\leq1} p(|\vec{x}|) d\vec{x} = \int_0^1 f(r) p(r) dr = 1\f$, 
	 * with \f$f(r)=4\pi r^2\f$ for 3D, \f$f(r)=2\pi r\f$ for 2D, 
	 * \f$f(r)=2\f$ for 1D systems. 
	 * 
	 * For \f$p(r)=\sum_{i=0}^{N-1} c_i r^{N-1-i}\f$, we obtain
	 * \f$V = \sum_{i=0}^{N-1} 4\pi c_i/(2+N-i)\f$ for 3D,
	 * \f$V = \sum_{i=0}^{N-1} 2\pi c_i/(1+N-i)\f$ for 2D, 
	 * \f$V = \sum_{i=0}^{N-1} 2 c_i/(N-i)\f$ for 1D systems.
	 */
	Mdouble get_volume();
	
	///Returns the value of the polynomial averaged over 2 dimensions.
	///For #StatType=X, \f$r=|x|\f$. See also #set_average_1D.
	Mdouble evaluate_1D(Mdouble r);
	
	///Returns the value of the polynomial averaged over 1 dimension.
	///For #StatType=XY, \f$r=\sqrt{x^2+y^2}\f$.
	Mdouble evaluate_2D(Mdouble r);

	///Sets #averaged_coefficients. 
	///This function is templated, with the default used only for StatType=XYZ, so it does nothing.
	void set_average();

	///Sets #averaged_coefficients \f$\bar{c}_i\f$ for #StatType=X,Y,Z such that \f$\sum_{i=0}^N \bar{c}_i x^{N-i} = \int\int_{|\vec{x}|\leq 1} p(|\vec{x}|) dy dz\f$. 
	///See #evaluate_1D.
	void set_average_1D();

	///For #StatType=XY,XZ,XZ, #averaged_coefficients is not used since \f$\bar{p}(r)\f$ can be evaluated as a function of \f$c_i\f$. 
	///See #evaluate_2D.
	void set_average_2D();
	
	///Returns the value of the gradient averaged over 2 dimensions.
	Mdouble evaluateGradient_1D(Mdouble r);

	///Returns the value of the gradient averaged over 1 dimensions.
	Mdouble evaluateGradient_2D(Mdouble r);

	///Returns the value of the line integral along the normal P1P2 "from a to b" over the axisymmetric function averaged over 2 dimensions
	Mdouble evaluateIntegral_1D(Mdouble a, Mdouble b, Mdouble t);
	
	///Returns the value of the line integral along the normal P1P2 "from a to b" over the axisymmetric function averaged over 1 dimensions
	Mdouble evaluateIntegral_2D(Mdouble a, Mdouble b, Mdouble t);

	///Access to the #coefficients
	Mdouble operator[](int i) const {
	  return coefficients[i];
	}

};
#include "Polynomial.hcc"
#endif

