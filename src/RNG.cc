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

#include "ExtendedMath.h"
/**
 * This is a random number generator and returns a Mdouble within the range specified
 * \todo{Thomas: This code does sth. when min>max; I would prefer to throw an error.}
 * \todo{the random seed should be stored in restart}
 * */
Mdouble RNG::get_RN(Mdouble min, Mdouble max)
{

switch (type)
	{
	case 0:
		return get_LCG(min,max);
	break;
	case 1:
		return get_LFG(min,max);
	break;
	}
//end switch
	
	

return get_LFG(min,max);
	
}
/**
 * This is a basic Linear Congruential Generator Random
 * Is described by three parameters, the multiplication a, the addition c and the mod m
 */
Mdouble RNG::get_LCG(Mdouble min, Mdouble max)
{
    //Update the random seed
    random_seed_LCG = (a*random_seed_LCG + c) % m;


    //Generate a random number in the required range

    Mdouble random_num; 
        
    Mdouble range=max-min; 
    random_num = min+range*random_seed_LCG/(static_cast<Mdouble>(m) + 1.0); 
                        
    return random_num;
}


/**************************************
 * This sets the seed for LFG using LCG
 * 
 * 
 **************************************/
void RNG::seed_LFG()
{
	for (int i=0;i<p;i++)
		{
			random_seed_LFG[i]=get_LCG(0,1.0);
		}
}

/**
 * This is a basic Linear Fibonacci Generator Random
 * Is described by three parameters, the multiplication a, the addition c and the mod m
 */
Mdouble RNG::get_LFG(Mdouble min, Mdouble max)
{
    Mdouble new_seed=fmod(random_seed_LFG[0]+random_seed_LFG[p-q],(Mdouble) 1.0);
    //Update the random seed
    for (int i=0;i<p-1;i++)
        {
        random_seed_LFG[i]=random_seed_LFG[i+1];
        }
    random_seed_LFG[p-1]=new_seed;



    //Generate a random number in the required range

    Mdouble random_num; 
        
    Mdouble range=max-min; 
    random_num = min+range*new_seed; 
                        
    return random_num;
}


/**
 * This function tests the quality of random numbers, based on the chi-squared test. 
 * It reports a probabity that the random number being generated are coming from a uniform distributed. 
 * If this number is less than 0.95, it is strongly advised that you change the parameters being used
 * */
Mdouble RNG::test()
{
	//This are the fixed parameters that define the test
	static int num_of_tests=100000;
	static Mdouble max_num=100.0;
	static int num_of_bins=10;
	
	//This is the generated random_number
	Mdouble rn;
	//This is the bin the random number will lie in
	int bin=0;
	//This is a vector of bins
	std::vector<int> count;
	count.resize(num_of_bins);
	
	//Initialisation of the bins
	for (int i=0;i<num_of_bins;i++)
		{
			count[bin]=0;
		}

	
	
	//Loop over a number of tests
	for (int i=0;i<num_of_tests;i++)
		{
			rn=get_RN(0.0,max_num);
			bin=floor(rn*num_of_bins/max_num);
			
			
			//Add one to the bin count
			count[bin]++;
				
		}
	
	
	
	
	
	//Final post-process the result and report on the random number
	Mdouble chi_cum=0.0;
	Mdouble expected=num_of_tests/num_of_bins;
	
	for (int i=0;i<num_of_bins;i++)
		{
			chi_cum=chi_cum+(count[i]-expected)*(count[i]-expected)/expected;
			std::cout << i << " : " << count[i] << " : " <<(count[i]-expected)*(count[i]-expected)/expected << std::endl;
		}
	//end for loop over comuputing the chi-squared value.
	std::cout << chi_cum << std::endl;
		
    return mathsFunc::chi_squared_prob(chi_cum,num_of_bins);
}

///This function sets the parametes for the LFG random number generator
void RNG::set_LFGParms(int new_p, int new_q)
{
	//p must be greater than q so makes sure this is true. Not sure what happens if you set p=q, in the LFG alogrithm.
	if (new_p > new_q)
		{
			p=new_p;
			q=new_q;
		}
	else
		{
			p=new_q;
			q=new_p;
		}
		
	random_seed_LFG.resize(p); 
	seed_LFG();
	
}
