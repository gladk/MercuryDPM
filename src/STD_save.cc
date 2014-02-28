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

#include "STD_save.h"
#include<stdio.h>
#include<stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h> 
#include <vector>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////This procedures gets the run number from the counter and increments the number stored in the file///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int STD_save::read_run_num_from_file()
//This procedure reads the counter in from a file stores on the disk. Increaments the number stored on the disk and then returns the current counter.
{
int counter;


FILE *counter_file;
if ((counter_file=fopen("COUNTER_DONOTDEL", "r+"))==NULL)
{
  //counter file not found
  if ((counter_file=fopen("COUNTER_DONOTDEL", "w"))==NULL) 
	{
		//counter file could not be created
		fprintf(stderr, "\n\n\tERROR :: Counter File NOT found, please re-create\n\n");
		fclose(counter_file);
		exit(-1);
	} else {
		//counter file sucessfully created
		fprintf(counter_file, "1");
		fprintf(stderr, "Counter File created\n");
		fclose(counter_file);
		return 1;
	}
} else {
	//counter file sucessfully opened
	if (fscanf(counter_file,"%d",&counter)!=1)
	{
		//Something with reading went wrong
		fprintf(stderr, "\n\n\tERROR :: Counter File found, but something went wrong with reading it\n\n");
		fclose(counter_file);
		exit(-1);		
	} else {
		fclose(counter_file);
		return counter;
	}
}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////This overrides the default counter value///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void STD_save::set_counter(int new_counter)
{
counter=new_counter;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////This returns the current value of the counter///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int STD_save::get_counter()
{
return(counter);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////This set teh counter using the current value in the file
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void STD_save::set_counter_from_file()
{
set_counter(read_run_num_from_file());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////This increaments the value of the counter stored on the disk and resaves it to the COUNTER file. This allows overriding of the run number
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void STD_save::inc_counter_in_file()
{
std::fstream counter_file, counter_file2;
int temp_counter;

counter_file.open("COUNTER_DONOTDEL",std::ios::in);
if (counter_file.fail())
  	{
	fprintf(stderr, "\n\n\tERROR :: Counter File NOT found, please re-create\n\n");
	counter_file.close();
	exit(0);
	}
	
counter_file >> temp_counter;
counter_file.close();
	
	
temp_counter++;	
	

counter_file2.open("COUNTER_DONOTDEL",std::ios::out);
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
///This save the information file to disk my redirection the std::cout stream and calling the normal info call///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void STD_save::save_info_to_disk()
{
std::fstream file;

//set up a std::string stream
std::stringstream filename;

//Write  info.counter to this stream
filename << "info." << counter;

//open a file with this name. This has to be a c std::string, hence the rahter convoluted syntax.
file.open((filename.str()).c_str(),std::ios::out);
//Make a copy of where the current output buffer is pointing 
std::streambuf *temp = std::cout.rdbuf();
std::cout.rdbuf(file.rdbuf());
info();
file.close();
//Return the output buffer back to were it was before.
std::cout.rdbuf(temp);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///This is a FileExist routine, which is used to test if a run have already need preformed, allows me to plug holes in parm studies.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool STD_save::FileExists(std::string strFilename)
{
  struct stat stFileInfo;
  bool blnReturn;
  int intStat;

  // Attempt to get the file attributes
  
  intStat = stat(strFilename.c_str(),&stFileInfo);
  if(intStat == 0) {
    // We were able to get the file attributes
    // so the file obviously exists.
    blnReturn = true;
  } else {
    // We were not able to get the file attributes.
    // This may mean that we don't have permission to
    // access the folder which contains this file. If you
    // need to do that level of checking, lookup the
    // return values of stat which will give you
    // more details on why stat failed.
    blnReturn = false;
  }
  
  return(blnReturn);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///This turns the counter into a two index numbers for doing parmeter studies///
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
std::vector<int> STD_save::get_numbers(int size_x,int size_y)
{
std::vector<int> temp(3);

int counter=get_counter();

int study_size = size_x*size_y;

int study_num=(counter-1)/study_size;

counter=counter-study_size*study_num;
int i=((counter-1)%size_x)+1;
int j=(counter-i)/size_x+1;
std::cout<<"Counter: "<<counter<<" i: "<<i<<" j: "<<j<<std::endl;

temp[0]=study_num;
temp[1]=i;
temp[2]=j;

return(temp);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// This launch a new program with the name passed///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int STD_save::launch_new(const char* name,bool quick)
{
	std::stringstream com("");
	
	if (quick)
		{
		com << "../sc/run " << name << " &";
		}
	else
		{
		com << "../sc/run " << name << " &";
		}	
			
	///todo{Do something with the resulting integer (to remove compilation warning)}
	return system(com.str().c_str());
}
