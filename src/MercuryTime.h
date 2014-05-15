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

#ifndef TIME_H
#define TIME_H

#include <ctime>
#include <time.h>
#include <string.h>
#include <sstream>

/** Allows timing of algorithms; accurate up to 0.01 sec. 
 * Calculates time used for computing in clocks, not total time.
 */
class Time {
	
public:
	void tic() {start = clock();}
	Mdouble toc() {finish = clock();	return (Mdouble(finish)-Mdouble(start))/CLOCKS_PER_SEC; 	}
	
private:
	clock_t start,finish;
	
};

///estimates the time in seconds when tmax should be reached
class Time2Finish {
	
public:
	void set(Mdouble t, Mdouble tmax) {start = clock(); t_=t; tmax_=tmax;}
	
	Mdouble getTime2Finish(Mdouble t) {
		clock_t finish = clock();
		Mdouble elapsedTime = (Mdouble(finish)-Mdouble(start))/CLOCKS_PER_SEC;
		Mdouble time2Finish = elapsedTime*(tmax_-t_)/(t-t_);
		start = finish;
		t_ = t;
		return time2Finish; 	
	}
	
	std::string getFinishTime(Mdouble t) {	
		Mdouble time2Finish = getTime2Finish(t);
		time_t finish = time(NULL) + time2Finish;
		std::stringstream ss;
		//write estimated end time
		ss << ctime(&finish);
		//decrement put pointer by one to avoid line break
		ss.seekp((long)ss.tellp()-1);
		//write time to finish
		ss << " (" << time2Finish/3600 << "h)";
		return ss.str();
	}
	
private:
	clock_t start;
	Mdouble t_, tmax_;
	
};


#endif
