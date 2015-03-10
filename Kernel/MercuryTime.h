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

/*!
 * \brief Allows for timing the algorithms; accurate up to 0.01 sec. 
 * \details Calculates the amount of computational time used, in seconds. Works on
 *        the same concept of stopwatch, where one presses start and stops when needed.
 *        The difference returns the total time used up.
 */
class Time
{
public:

    /*!
     * \brief This is like a start button of a stopwatch. Assigns the variable
     *        start with the current number of clock ticks.
     */
    void tic()
    {
        start = clock(); //clock tics
    }

    /*!
     * \brief This is like a stop button of a stopwatch. Assigns the variable finish
     *        to the current value of ticks returned by clock().
     * \return However, it also returns the total real time in seconds.
     */
    Mdouble toc()
    {
        finish = clock();
        return (Mdouble(finish) - Mdouble(start)) / CLOCKS_PER_SEC;
    }

private:
    /*!
     * \brief Stores the number of clock ticks, called by Time::tic().
     */
    clock_t start;

    /*!
     * \brief Stores the number of clock ticks, called by Time::toc().
     */
    clock_t finish;
};

/*!
 * \brief Estimates the total time, in seconds, left to reach the end of any simulation.
 * First, the class needs to be initialized by calling set. 
 * After the class is initialized, an estimate of the total remaining time of the
 * simulation can be found by calling getTime2Finish. 
 * The estimate is based on rate at which the simulation time progressed since initialization.
 
 * E.g., assume that the class has been initialized at simulation time 0, with final time 10.
 * Then, getTime2Finish is called after 1 hour at simulation time 2.
 * Since the code required 0.5 hours per simulation time unit and there are 8 
 * simulation time units left, it is likely to finish in 4 hours.
 */
class Time2Finish
{
public:

    /*!
     * \brief Initialises the variable start with the current value of clock 
     *        ticks, the current time and the final time of the simulation.
     * \param[in] t     current simulation time.
     * \param[in] tMax  total simulation time for which the simulation is set to run.
     */
    void set(Mdouble t, Mdouble tMax)
    {
        startTime_ = clock();
        time_ = t;
        timeMax_ = tMax;
    }

    /*!
     * \brief Estimates the total time, in seconds, left to reach the end of any simulation.
     * After the class is initialized, an estimate of the total remaining time of the
     * simulation can be found by calling getTime2Finish. 
     * The estimate is based on rate at which the simulation time progressed since initialization.
     *
     * E.g., assume that the class has been initialized at simulation time 0, with final time 10.
     * Then, getTime2Finish is called after 1 hour at simulation time 2.
     * Since the code required 0.5 hours per simulation time unit and there are 8 
     * simulation time units left, it is likely to finish in 4 hours.
     * \param[in] t     current simulation time.
     * \return Mdouble  time, in seconds, left to reach the end of any simulation
     */
    Mdouble getTime2Finish(Mdouble t)
    {
        clock_t finish = clock();
        Mdouble elapsedTime = (Mdouble(finish) - Mdouble(startTime_)) / CLOCKS_PER_SEC;

        if (fabs(time_ - t) < 1.e-9)
        {
            std::cout << "Choose an other value for t" << std::endl;
            return 0;
        }
        else
        {
            Mdouble time2Finish = elapsedTime * (timeMax_ - time_) / (t - time_);
            startTime_ = finish;
            time_ = t;
            return time2Finish;
        }
    }

    /*!
     * \brief Returns the estimated finish time based on the amount of time left to finish.
     * \param[in] t current simulation time
     * \return time of the day, in hours, at which the simulation is predicted to end
     */
    std::string getFinishTime(Mdouble t)
    {
        // gets the estimated time left to finish.
        Mdouble time2Finish = getTime2Finish(t);

        // adds to the estimated time to current time and also type-casting Mdouble to time_t.
        time_t finish = time(nullptr) + time2Finish;

        std::stringstream ss;

        //write estimated end time
        ss << ctime(&finish);

        //decrement put pointer by one to avoid line break
        ss.seekp((long) ss.tellp() - 1);

        //write time to finish
        ss << " (" << time2Finish / 3600 << "h)";
        return ss.str();
    }

private:
    /// Stores the current number of clock ticks at the start.
    clock_t startTime_;

    /// Stores the simulation time (DPM units)
    Mdouble time_;

    /// Stores the total simulation time (DPM units)
    Mdouble timeMax_;

};

#endif
