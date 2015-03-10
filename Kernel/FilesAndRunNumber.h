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
 * \brief Here are the routines to use a counter file to store run numbers.
 * All my codes do this so I expect this routines will be used often.
 * So this saves my coping the same routine over and over again
 */
class FilesAndRunNumber : public Files
{
public:

    /*!
     * \brief Default constructor: sets the counter to 0 (i.e. no number will be included).
     */
    FilesAndRunNumber();

    /*!
     * \brief Copy constructor
     */
    FilesAndRunNumber(const FilesAndRunNumber& other);

    /*!
     * \brief
     */
    void constructor();

    /*!
     * \brief Increament the counter value stored in the file_counter by 1 and store the new value
     */
    void incrementRunNumberInFile();

    /*!
     * \brief Read from the counter file the counter
     */
    int readRunNumberFromFile();

    /*!
     * \brief
     */
    void autoNumber();

    /*!
     * \brief This turns a counter into two indices for doing parameter studies. The indices run from 1:size_x and 1:size_y, while the study number starts at 0.
     */
    std::vector<int> get2DParametersFromRunNumber(int size_x, int size_y);

    /*!
     * \brief This launch a code from within this code. Please pass the name of the code to run
     */
    int launchNewRun(const char* name, bool quick = false);

//setters and getters
    
    /*!
     * \brief This sets the counter, overriding the defaults
     */
    void setRunNumber(int runNumber);

    /*!
     * \brief This returns the current value of the counter
     */
    int getRunNumber() const;

private:

    /*!
     * \brief The stores the run number for saving
     */
    int runNumber_;
};

#endif
