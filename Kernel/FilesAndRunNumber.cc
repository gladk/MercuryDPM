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

#include "FilesAndRunNumber.h"
#include "Math/ExtendedMath.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

/*!
 * 
 */
FilesAndRunNumber::FilesAndRunNumber()
{
    constructor();
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "FilesAndRunNumber::FilesAndRunNumber() finished" << std::endl;
#endif
}

/*!
 * \param[in] other
 */
FilesAndRunNumber::FilesAndRunNumber(const FilesAndRunNumber& other)
: Files(other)
{
    runNumber_ = other.runNumber_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "FilesAndRunNumber::FilesAndRunNumber(FilesAndRunNumber& other) finished" << std::endl;
#endif
}

/*!
 * 
 */
FilesAndRunNumber::~FilesAndRunNumber()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "FilesAndRunNumber::~FilesAndRunNumber() finished" << std::endl;
#endif
}

/*!
 * \details Initialises the runNumber_ = 0
 */
void FilesAndRunNumber::constructor()
{
    runNumber_ = 0;
}

/*!
 * 
 */
void FilesAndRunNumber::autoNumber()
{
    setRunNumber(readRunNumberFromFile());
    incrementRunNumberInFile();
}

/*!
 * \details The procedure below reads the counter in from a file stored on the disk. 
 * Increments the number stored on the disk and then returns the current counter.
 */
int FilesAndRunNumber::readRunNumberFromFile()
{
    int counter;

    FILE *counter_file;
    if ((counter_file = fopen("COUNTER_DONOTDEL", "r+")) == nullptr)
    {
        //counter file not found
        if ((counter_file = fopen("COUNTER_DONOTDEL", "w")) == nullptr)
        {
            //counter file could not be created
            fprintf(stderr, "\n\n\tERROR :: Counter File NOT found, please re-create\n\n");
            fclose(counter_file);
            exit(-1);
        }
        else
        {
            //counter file sucessfully created
            fprintf(counter_file, "1");
            fprintf(stderr, "Counter File created\n");
            fclose(counter_file);
            return 1;
        }
    }
    else
    {
        //counter file sucessfully opened
        if (fscanf(counter_file, "%d", &counter) != 1)
        {
            //Something with reading went wrong
            fprintf(stderr, "\n\n\tERROR :: Counter File found, but something went wrong with reading it\n\n");
            fclose(counter_file);
            exit(-1);
        }
        else
        {
            fclose(counter_file);
            return counter;
        }
    }

}

/*!
 * \param[in] runNumber
 */
void FilesAndRunNumber::setRunNumber(int runNumber)
{
    runNumber_ = runNumber;
}

/*!
 * \returns runNumber_
 */
int FilesAndRunNumber::getRunNumber() const
{
    return runNumber_;
}

/*!
 * \details In order to increment the counter stored in COUNTER_DONOTDEL, we initialise two fstream objects counter_file, counter_file2 and
 * an integer type temp_counter. First we open the file COUNTER_DONOTDEL, check if everything went fine with the opening. If yes, we extract the
 * runNumber (counter) into the temp_counter. Increment the temp_counter and then write it into COUNTER_DONOTDEL. This is how we increment the
 * counter in the file.
 */
void FilesAndRunNumber::incrementRunNumberInFile()
{
    std::fstream counter_file, counter_file2;
    int temp_counter;

    counter_file.open("COUNTER_DONOTDEL", std::ios::in);
    if (counter_file.fail())
    {
        fprintf(stderr, "\n\n\tERROR :: Counter File NOT found, please re-create\n\n");
        counter_file.close();
        exit(0);
    }

    counter_file >> temp_counter;
    counter_file.close();

    temp_counter++;

    counter_file2.open("COUNTER_DONOTDEL", std::ios::out);
    if (counter_file2.fail())
    {
        fprintf(stderr, "\n\n\tERROR :: Counter File NOT found, please re-create2\n\n");
        counter_file2.close();
        exit(0);
    }

    counter_file2 << temp_counter;

    counter_file2.close();
}

/*!
 * \details Lets say size_x = 2 and size_y = 5, counter stored in COUNTER_DONOTDEL =1. The study_size = 10.
 * Substituting these values into the below algorithm implies that study_num = 0 or 1, everytime the code is executed the counter gets incremented and hence determined
 * the values of study_num, i and j which is returned as a std::vector<int>
 * \param[in] size_x
 * \param[in] size_y
 * \returns std::vector<int>
 */
std::vector<int> FilesAndRunNumber::get2DParametersFromRunNumber(int size_x, int size_y)
{
    std::vector<int> temp(3);

    int counter = getRunNumber();

    int study_size = size_x * size_y;

    int study_num = (counter - 1) / study_size;

    counter = counter - study_size * study_num;
    int i = ((counter - 1) % size_x) + 1;
    int j = (counter - i) / size_x + 1;
    std::cout << "Counter: " << counter << " i: " << i << " j: " << j << std::endl;

    temp[0] = study_num;
    temp[1] = i;
    temp[2] = j;

    return (temp);
}

/*!
 * \param[in] name
 * \param[in] quick
 * \return int
 */
int FilesAndRunNumber::launchNewRun(const char* name, bool quick UNUSED)
{
    std::stringstream com("");
    com << name << " &";
    return system(com.str().c_str());
}

/*!
 * \param[in,out] is
 */
void FilesAndRunNumber::read(std::istream& is)
{
    std::string dummy;
    is >> std::ws;
    if (is.peek() == 'r')
        is >> dummy >> runNumber_;
    Files::read(is);
}

/*!
 * \param[in,out] os
 */
void FilesAndRunNumber::write(std::ostream& os) const
{
    //only write the run number if it is different from 0
    if (runNumber_ != 0)
        os << " runNumber " << runNumber_;
    Files::write(os);
}
