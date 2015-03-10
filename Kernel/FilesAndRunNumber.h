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

#ifndef STD_SAVE_H
#define STD_SAVE_H

#include "Files.h"
#include <vector>

#ifdef HIGH_PRECISION
typedef long double Mdouble;
#else 
typedef double Mdouble;
#endif

/*!
 * \class FilesAndRunNumber
 * \brief It is publicly inherited from class Files. It defines an awesome feature that is ideal when doing a parameter study.
 * Below are the routines that manipulate a counter file, called COUNTER_DONOTDEL, to store run numbers.
 * \details For a paramater study, a particular DPM simulation is run several times. Each time the code is executed,
 * the run number or counter, in the COUNTER_DONOTDEL, gets incremented. Based on the counter your file name is named as 
 * problemName.1.data, problemName.2.data...
 * If the File::fileType_ is chosen as Multiple files, then the your data files will have the name as problemName.runNumber.0,
 *  problemName.runNumber.1 ...
 */

 // All my codes do this so I expect this routines will be used often.
 //So this saves my coping the same routine over and over again

class FilesAndRunNumber : public Files
{
public:

    /*!
     * \brief Constructor
     */
    FilesAndRunNumber();

    /*!
     * \brief Copy constructor
     */
    FilesAndRunNumber(const FilesAndRunNumber& other);

    /*!
     * \brief Constructor
     */
    virtual ~FilesAndRunNumber();

    /*!
     * \brief a function called by the FilesAndRunNumber() (constructor)
     */
    void constructor();

    /*!
     * \brief Increment the run Number (counter value) stored in the file_counter (COUNTER_DONOTDEL) by 1 and store the new value
     * in the counter file
     */
    void incrementRunNumberInFile();

    /*!
     * \brief Read the run number or the counter from the counter file (COUNTER_DONOTDEL)
     */
    int readRunNumberFromFile();

    /*!
     * \brief The autoNumber() function is the trigger. It calls three functions. setRunNumber(), readRunNumberFromFile() and 
     * incrementRunNumberInFile().
     */
    void autoNumber();

    /*!
     * \brief This turns a counter into two indices which is an amazing feature for doing two dimensional parameter studies. 
     * The indices run from 1:size_x and 1:size_y, while the study number starts at 0 ( initially the counter=1 in COUNTER_DONOTDEL)
     */
    std::vector<int> get2DParametersFromRunNumber(int size_x, int size_y);

    /*!
     * \brief This launches a code from within this code. Please pass the name of the code to run.
     */
    int launchNewRun(const char* name, bool quick = false);

    //setters and getters
    
    /*!
     * \brief This sets the counter/Run number, overriding the defaults
     */
    void setRunNumber(int runNumber);

    /*!
     * \brief This returns the current value of the counter (runNumber_)
     */
    int getRunNumber() const;

    /*!
     * \brief Accepts an input stream <a href="http://en.cppreference.com/w/cpp/io/basic_istreams.html">std::istream</a>.
     */
    void read(std::istream& is);

    /*!
     * \brief Accepts an output stream read function, which accepts an input stream <a href="http://en.cppreference.com/w/cpp/io/basic_ostreams.html">std::ostream</a>.
     */
    void write(std::ostream& os) const;

private:

    /*!
     * \brief This stores the run number for saving
     */
    int runNumber_;
};

#endif
