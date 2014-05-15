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

#include "ExtendedMath.h"
#include <cmath>
#include <assert.h>
//This has the defintion of quiet nan
#include <limits>

/**
 * This is the gamma function, gives 'exact' answers for the half integer values
 * This is done using the recussion relation and the known values for 1 and 0.5
 * Note, return NaN for non-half integer values
*/
Mdouble mathsFunc::gamma(Mdouble gamma_in)
{
const Mdouble ep=1e-5;

if (gamma_in > 1.0+ep)
	{
	return ((gamma_in-1)*gamma(gamma_in-1));
	}
else
	{ 

	if ((gamma_in-ep<1.0) && (gamma_in+ep>1.0))
		return 1.0;
	else if ((gamma_in-ep<0.5) && (gamma_in+ep>0.5))
		return constants::sqrt_pi;
	else
		return std::numeric_limits<Mdouble>::quiet_NaN();
	}
}//end func gamma

/**
 * This is a chi_squared function return the value x and degrees of freedom k
*/
Mdouble mathsFunc::chi_squared(Mdouble x, int k)
{

Mdouble prefactor = pow(2,k/2.0)*gamma(k/2.0);
Mdouble mainfactor = pow(x,k/2.0-1)*exp(x/-2.0);

return mainfactor/prefactor;


	
}

/**This calulates the probabity based on a chi squared test
 * First we calculated the  cummelative chi_squared function. 
 * This is the function which actually gives the probability back
 * It is calculated by calling the normal chi_squated function and using the trapezoidal rule.
 * The final results is 1-the cummulative chi_squared function
 */ 
Mdouble mathsFunc::chi_squared_prob(Mdouble x_max,int k)
{

//The current value was picked by tried were it stopped effect the 4 d.p.
const int num_steps_per_unit=100;
Mdouble sum=0;
Mdouble x=0;
long int num_steps=static_cast<int>(num_steps_per_unit*x_max);
//Use trapezional rule, but ignoring the ends
for (int i=0;i<num_steps;i++)
	{
	x=x_max/num_steps*(i+0.5);
	sum=sum+chi_squared(x,k);
	}
return 1.0-sum*x_max/num_steps;
	
}
	


Mdouble besselFunc::chebyshev(Mdouble x, const Mdouble coef[], int N)
{
	const Mdouble *p = coef;
	Mdouble b0 = *p++;
	Mdouble b1=0, b2;
	int i = N - 1;
	
	assert(i>0);
	do
	{
		b2 = b1;
		b1 = b0;
		b0 = x * b1  -  b2  + *p++;
	}
	while( --i );
	
	return( 0.5*(b0-b2) );
}

Mdouble besselFunc::I0_exp(Mdouble x)
{
	// Cooefficients for [0..8]
	const Mdouble A[] =
	{
		-4.415341646479339379501E-18,
		3.330794518822238097831E-17,
		-2.431279846547954693591E-16,
		1.715391285555133030611E-15,
		-1.168533287799345168081E-14,
		7.676185498604935616881E-14,
		-4.856446783111929460901E-13,
		2.955052663129639834611E-12,
		-1.726826291441555707231E-11,
		9.675809035373236912241E-11,
		-5.189795601635262906661E-10,
		2.659823724682386650351E-9,
		-1.300025009986248042121E-8,
		6.046995022541918949321E-8,
		-2.670793853940611733911E-7,
		1.117387539120103718151E-6,
		-4.416738358458750563591E-6,
		1.644844807072889708931E-5,
		-5.754195010082103703981E-5,
		1.885028850958416557291E-4,
		-5.763755745385823658851E-4,
		1.639475616941335798421E-3,
		-4.324309995050575944301E-3,
		1.054646039459499831831E-2,
		-2.373741480589946881561E-2,
		4.930528423967070848781E-2,
		-9.490109704804764442101E-2,
		1.716209015222087753491E-1,
		-3.046826723431983986831E-1,
		6.767952744094760849951E-1
	};
	
	// Cooefficients for [8..infinity]
	const Mdouble B[] =
	{
		-7.233180487874753954561E-18,
		-4.830504485944182071261E-18,
		4.465621420296759999011E-17,
		3.461222867697461093101E-17,
		-2.827623980516583484941E-16,
		-3.425485619677219134621E-16,
		1.772560133056526383601E-15,
		3.811680669352622420751E-15,
		-9.554846698828307648701E-15,
		-4.150569347287222086631E-14,
		1.540086217521409826911E-14,
		3.852778382742142701141E-13,
		7.180124451383666233671E-13,
		-1.794178531506806117781E-12,
		-1.321581184044771311881E-11,
		-3.149916527963241364541E-11,
		1.188914710784643834241E-11,
		4.940602388224969589101E-10,
		3.396232025708386345151E-9,
		2.266668990498178064591E-8,
		2.048918589469063741831E-7,
		2.891370520834756482971E-6,
		6.889758346916823984261E-5,
		3.369116478255694089901E-3,
		8.044904110141088316081E-1
	};
	
	
	if (x < 0)
		x = -x;
	
	if (x <= 8.0)
	{
		Mdouble y = (x/2.0) - 2.0;
		return( chebyshev( y, A, 30 ) );
	}
	
	return( chebyshev( 32.0/x - 2.0, B, 25 ) / sqrt(x) );
}

Mdouble besselFunc::I0(Mdouble x)
{
	if (x< 0) 
		x = -x;
	return exp(x) * I0_exp(x);
}

void getLineFromStringStream(std::istream& in, std::stringstream& out)
{
	std::string line_string;
	getline(in,line_string);
	out.str(line_string);
	out.clear();
}

helperFunc::KAndDisp helperFunc::computeKAndDispFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(Mdouble tc, Mdouble r, Mdouble mass)
{
	helperFunc::KAndDisp ans;
	ans.disp = - 2.0 * mass * std::log(r) / tc;
	ans.k = mass * (sqr(constants::pi/tc) + sqr(ans.disp/(2.0*mass)));
	return ans;
}	

///from Deen...Kuipers2006, eq. 43 and 30
helperFunc::KAndDispAndKtAndDispt helperFunc::computeDisptFromCollisionTimeAndRestitutionCoefficientAndTangentialRestitutionCoefficientAndEffectiveMass(Mdouble tc, Mdouble r, Mdouble beta, Mdouble mass)
{
	helperFunc::KAndDispAndKtAndDispt ans;
	ans.disp = - 2.0 * mass * log(r) / tc;
	ans.k  = mass * (sqr(constants::pi/tc) + sqr(ans.disp/(2.0*mass)));
	ans.kt = 2.0/7.0*ans.k*(sqr(constants::pi)+sqr(log(beta)))/(sqr(constants::pi)+sqr(log(r)));
	if (beta!=0.0) ans.dispt = -2*log(beta)*sqrt(1.0/7.0*mass*ans.kt/(sqr(constants::pi)+sqr(log(beta))));
	else ans.dispt = 2.*sqrt(1.0/7.0*mass*ans.kt);
	return ans;
}	

Mdouble helperFunc::getCollisionTime(Mdouble k, Mdouble disp, Mdouble mass)
{
	
	//check for errors in input
	if(mass<=0) 	
	{
		std::cerr << "Error in get_collision_time(Mdouble mass) mass is not set or has an unexpected value, (get_collision_time("<<mass<<"))"<< std::endl; 
		exit(-1);
	}
	if(k<=0) 		
	{
		std::cerr << "Error in get_collision_time(Mdouble mass) stiffness is not set or has an unexpected value, (get_collision_time("<<mass<<"), with stiffness="<<k<<")"<< std::endl; 
		exit(-1);
	}
	if(disp<0) 		
	{
		std::cerr << "Error in get_collision_time(Mdouble mass) dissipation is not set or has an unexpected value, (get_collision_time("<<mass<<"), with dissipation="<<disp<<")"<< std::endl; 
		exit(-1);
	}
	
	//calculate square of oscillation frequency
	Mdouble w2=k/mass - sqr(disp/(2.0*mass));
	
	if (w2<=0) 	
	{
		std::cerr << "Error in get_collision_time(Mdouble mass) values for mass, stiffness and dissipation lead to an over-damped system, (get_collision_time("<<mass<<"), with stiffness="<<k<<" and dissipation="<<disp<<")"<< std::endl; 
		exit(-1);
	}
	
	return constants::pi / sqrt( w2 );
}

Mdouble helperFunc::getRestitutionCoefficient(Mdouble k, Mdouble disp, Mdouble mass)
{
	return exp(-disp/(2.0*mass)*helperFunc::getCollisionTime(k, disp, mass));
}

Mdouble helperFunc::getMaximumVelocity(Mdouble k, Mdouble disp, Mdouble radius, Mdouble mass)
{
	// note: underestimate based on energy argument,
	// Ekin = 2*1/2*m*(v/2)^2 = 1/2*k*(2*r)^2, gives
	// return radius * sqrt(8.0*k/mass);

	// with dissipation, see S. Luding, Collisions & Contacts between two particles, eq 34
	Mdouble w = sqrt(k/mass - sqr(disp/(2.0*mass)));
	Mdouble w0 = sqrt(k/mass);
	Mdouble DispCorrection = exp(-disp/mass/w)*asin(w/w0);
	//std::cout << "DispCorrection" << DispCorrection << std::endl;
	return radius * sqrt(8.0*k/mass) / DispCorrection;
}
