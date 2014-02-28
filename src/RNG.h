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

#ifndef RNG_H
#define RNG_H

//Used by the randomise function
#include <time.h>
#include <cmath>
#include "ExtendedMath.h"

/** This is a class that generates random numbers i.e. named the Random Number Generator (RNG). 
 * This is a stand-along class; but is encapsulated (used) by the MD class.
 * To make it architecture safe the both LCG and function is hard codes i.e. does not use the internal C++ one.
  */
class RNG
{
public:

	RNG(){random_seed_LCG=0; a=1103515245; c=12345;m=1024*1024*1024;type=1;p=607;q=273;random_seed_LFG.resize(p); seed_LFG();}

	/// \brief This is the seed for the random number generator 
	// (note the call to seed_LFG is only required really if using that type of generator, but the other one is always required
	void set_RandomSeed(Mdouble new_seed){random_seed_LCG=new_seed; seed_LFG();}

	///This is a random generating routine can be used for initial positions
	Mdouble get_RN(Mdouble min, Mdouble max);
	
	///This function tests the quality of random numbers, based on the chi-squared test. 
	Mdouble test();
	
	///This functions set the parameters for the LCG random number generator. It goes multiplier, addition, mod.
	void set_LCGParms(int new_a,int new_c,int new_m)
	{a=new_a;c=new_c;m=new_m;}
	
	///sets the random variables such that they differ for each run
	void randomise() {set_RandomSeed(time(NULL));}
	
	///This function sets the parametes for the LFG random number generator
	void set_LFGParms(int new_p, int new_q);
	
	///Make the random number generator based on LCG
	void set_RNtypeLCG(){type=0;}
	
	///Make the random number generator based on LFG
	void set_RNtypeLFG(){type=1;}
	
private:

	///This is a basic Linear Congruential Generator Random 
	Mdouble get_LCG(Mdouble min, Mdouble max);

	///This is a Laggend Fibonacci Generator
	Mdouble get_LFG(Mdouble min, Mdouble max);
	
	///This seed the LFG
	void seed_LFG();

	///This is the initiall seed of the RNG
	unsigned long int random_seed_LCG;
	
	///This is the seeds required for the LFG
	std::vector<Mdouble> random_seed_LFG;
	
	///This are the two parameters that control the LCG random generated
	long int a,c,m;
	
	///This are the parameters that control the LFG random generator
	long int p,q;
	
	///This is the type of random number generator
	int type;
	
};


#endif
