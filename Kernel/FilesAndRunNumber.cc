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

#include "FilesAndRunNumber.h"
#include "Math/ExtendedMath.h"
#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

FilesAndRunNumber::FilesAndRunNumber()
{
    constructor();
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "FilesAndRunNumber::FilesAndRunNumber() finished"<<std::endl;
#endif
}

FilesAndRunNumber::FilesAndRunNumber(const FilesAndRunNumber& other)
        : Files(other)
{
    runNumber_ = other.runNumber_;
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "FilesAndRunNumber::FilesAndRunNumber(FilesAndRunNumber& other) finished"<<std::endl;
#endif
}

void FilesAndRunNumber::constructor()
{
    runNumber_ = 0;
}

void FilesAndRunNumber::autoNumber()
{
    setRunNumber(readRunNumberFromFile());
    incrementRunNumberInFile();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////This procedures gets the run number from the counter and increments the number stored in the file///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int FilesAndRunNumber::readRunNumberFromFile()
//This procedure reads the counter in from a file stores on the disk. Increaments the number stored on the disk and then returns the current counter.
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////This overrides the default counter value///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FilesAndRunNumber::setRunNumber(int runNumber)
{
    runNumber_ = runNumber;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////This returns the current value of the counter///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int FilesAndRunNumber::getRunNumber() const
{
    return runNumber_;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////This increaments the value of the counter stored on the disk and resaves it to the COUNTER file. This allows overriding of the run number
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///This turns the counter into a two index numbers for doing parameter studies///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// This launch a new program with the name passed///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int FilesAndRunNumber::launchNewRun(const char* name, bool quick UNUSED)
{
    std::stringstream com("");
    com << name << " &";
    return system(com.str().c_str());
}
