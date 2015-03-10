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

#include "RNG.h"
/**
 * This is a random number generator and returns a Mdouble within the range specified
 * \todo{Thomas: This code does sth. when min>max; I would prefer to throw an error.}
 * \todo{the random seed should be stored in restart}
 * */

RNG::RNG()
{
    randomSeedLinearCongruentialGenerator_ = 0;
    a_ = 1103515245;
    c_ = 12345;
    m_ = 1024 * 1024 * 1024;
    type_ = RNGType::LAGGED_FIBONACCI_GENERATOR;
    p_ = 607;
    q_ = 273;
    randomSeedLaggedFibonacciGenerator_.resize(p_);
    seedLaggedFibonacciGenerator();
}

void RNG::setRandomSeed(unsigned long int new_seed)
{
    randomSeedLinearCongruentialGenerator_ = new_seed;
    seedLaggedFibonacciGenerator();
}

void RNG::setLinearCongruentialGeneratorParmeters(unsigned const int a, unsigned const int c, unsigned const int m)
{
    a_ = a;
    c_ = c;
    m_ = m;
}

void RNG::randomise()
{
    setRandomSeed(static_cast<unsigned long int>(time(nullptr)));
}

void RNG::setRandomNumberGenerator(RNGType type)
{
    type=type_;
}

Mdouble RNG::getRandomNumber(Mdouble min, Mdouble max)
{   
    switch (type_)
    {
        case RNGType::LINEAR_CONGRUENTIAL_GENERATOR:
            return getRandomNumberFromLinearCongruentialGenerator(min, max);
        case RNGType::LAGGED_FIBONACCI_GENERATOR:
            return getRandomNumberFromLaggedFibonacciGenerator(min, max);
    }    
}
/**
 * This is a basic Linear Congruential Generator Random
 * Is described by three parameters, the multiplication a, the addition c and the mod m
 */
Mdouble RNG::getRandomNumberFromLinearCongruentialGenerator(Mdouble min, Mdouble max)
{
    //Update the random seed
    randomSeedLinearCongruentialGenerator_ = (a_ * randomSeedLinearCongruentialGenerator_ + c_) % m_;
    
    //Generate a random number in the required range
    
    Mdouble random_num;
    
    Mdouble range = max - min;
    random_num = min + range * randomSeedLinearCongruentialGenerator_ / (static_cast<Mdouble>(m_) + 1.0);
    
    return random_num;
}

/**************************************
 * This sets the seed for LFG using LCG
 * 
 * 
 **************************************/
void RNG::seedLaggedFibonacciGenerator()
{
    for (unsigned int i = 0; i < p_; i++)
    {
        randomSeedLaggedFibonacciGenerator_[i] = getRandomNumberFromLinearCongruentialGenerator(0, 1.0);
    }
}

/**
 * This is a basic Linear Fibonacci Generator Random
 * Is described by three parameters, the multiplication a, the addition c and the mod m
 */
Mdouble RNG::getRandomNumberFromLaggedFibonacciGenerator(Mdouble min, Mdouble max)
{
    Mdouble new_seed = fmod(randomSeedLaggedFibonacciGenerator_[0] + randomSeedLaggedFibonacciGenerator_[p_ - q_], static_cast<Mdouble>(1.0));
    //Update the random seed
    for (unsigned int i = 0; i < p_ - 1; i++)
    {
        randomSeedLaggedFibonacciGenerator_[i] = randomSeedLaggedFibonacciGenerator_[i + 1];
    }
    randomSeedLaggedFibonacciGenerator_[p_ - 1] = new_seed;
    
    //Generate a random number in the required range
    
    Mdouble random_num;
    
    Mdouble range = max - min;
    random_num = min + range * new_seed;
    
    return random_num;
}

/**
 * This function tests the quality of random numbers, based on the chi-squared test. 
 * It reports a probability that the random number being generated are coming from a uniform distributed. 
 * If this number is less than 0.95, it is strongly advised that you change the parameters being used
 * */
Mdouble RNG::test()
{
    //This are the fixed parameters that define the test
    static unsigned int num_of_tests = 100000;
    static Mdouble max_num = 100.0;
    static unsigned int num_of_bins = 10;
    
    //This is the generated random_number
    Mdouble rn;
    //This is the bin the random number will lie in
    unsigned int bin = 0;
    //This is a vector of bins
    std::vector<int> count;
    count.resize(num_of_bins);
    
    //Initialisation of the bins
    for (unsigned int i = 0; i < num_of_bins; i++)
    {
        count[bin] = 0;
    }
    
    //Loop over a number of tests
    for (unsigned int i = 0; i < num_of_tests; i++)
    {
        rn = getRandomNumber(0.0, max_num);
        bin = static_cast<unsigned int>(std::floor(rn * num_of_bins / max_num));
        
        //Add one to the bin count
        count[bin]++;
        
    }
    
    //Final post-process the result and report on the random number
    Mdouble chi_cum = 0.0;
    Mdouble expected = num_of_tests / num_of_bins;
    
    for (unsigned int i = 0; i < num_of_bins; i++)
    {
        chi_cum = chi_cum + (count[i] - expected) * (count[i] - expected) / expected;
        std::cout << i << " : " << count[i] << " : " << (count[i] - expected) * (count[i] - expected) / expected << std::endl;
    }
    //end for loop over computing the chi-squared value.
    std::cout << chi_cum << std::endl;
    
    return mathsFunc::chi_squared_prob(chi_cum, num_of_bins);
}

///This function sets the parameters for the LFG random number generator
void RNG::setLaggedFibonacciGeneratorParameters(const unsigned int p, const unsigned int q)
{
    //p must be greater than q so makes sure this is true. Not sure what happens if you set p=q, in the LFG alogrithm.
    if (p > q)
    {
        p_ = p;
        q_ = q;
    }
    else
    {
        p_ = p;
        q_ = q;
    }
    
    randomSeedLaggedFibonacciGenerator_.resize(p_);
    seedLaggedFibonacciGenerator();
    
}
