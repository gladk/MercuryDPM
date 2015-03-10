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
 * \class STD_save
 * \brief Here are the routines to use a counter file to store run numbers.
 * All my codes do this so I expect this routines will be used often.
 * So this saves my coping the same routine over and over again
 */
class STD_save : public Files
{
public:

    /*!
     * \brief Default constructor: sets the counter to 0 (i.e. no number will be included).
     */
    STD_save();

    /*!
     * \brief Copy constructor
     */
    STD_save(const STD_save& other);

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
