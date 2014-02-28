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
