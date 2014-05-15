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

//#include "version.h"
#include<string>
#include<vector>
#include<sstream>
#include<fstream>
#include<iostream>
#include<iomanip>
#include<stdlib.h>

#ifdef HIGH_PRECISION
	typedef long double Mdouble;
#else 
	typedef double Mdouble;
#endif

/**Here are the rounties to use a counter file to store run numbers. 
 * All my codes do this so I expect this rounties will be used often. 
 * So this saves my coping the same routine over and over again
 */
class STD_save {

public:

/// Default constructor: sets the counter to 0 (i.e. no number will be included).
STD_save(){
	constructor();
	#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "STD_save() finished"<<std::endl;
	#endif	
}

/// Copy constructor
STD_save(STD_save& other) {
	constructor();
	counter = other.get_counter();
	problem_name.str(other.get_name());
	set_options_fstat(other.get_options_fstat());
	set_options_data(other.get_options_data());
	set_options_ene(other.get_options_ene());
	set_options_stat(other.get_options_stat());
	set_options_restart(other.get_options_restart());
	#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "STD_save(STD_save& other) finished"<<std::endl;
	#endif	
};

void constructor(){
	counter=0;
	set_options_restart(1);
	set_options_fstat(1);
	set_options_ene(1);
	set_options_data(1);
	set_options_stat(1);
	file_counter = 0;
	step_size = 1;
	data_file.precision(13);
	fstat_file.precision(13);
	ene_file.precision(13);
	stat_file.precision(5);
	stat_file.setf(std::ios::left);
}


/// Increament the counter value stored in the file_counter by 1 and store the new value
void inc_counter_in_file();

/// Read rom the counter file the counter
int read_run_num_from_file();

///Sets the counter based on the current number stored in the counter file.
void set_counter_from_file();

///Saves the information generated by info to disk in a file 
void save_info_to_disk();

///This set the counter, overriding the defaults
void set_counter(int new_counter);

///This returns the current value of the counter
int get_counter();

///Set up a virtual info this will be provided from the inhertiance
virtual void info(){}

///Function to check if a file exists, is used to check if a run has already need done
bool FileExists(std::string strFilename);

void auto_number(){	set_counter_from_file(); inc_counter_in_file();}

///This turns a counter into two indexs for doing parmater studies. The indexs run from 1:size_x and 1:size_y where as the study number starts at 0.
std::vector<int> get_numbers(int size_x, int size_y);

///This launch a code from within this code. Please pass the name of the code to run
int launch_new(const char* name, bool quick=false);

///Sets the name of the problem, used for the same data files
void set_name(const char* name){problem_name.str(""); problem_name << name;}
///Allows the problem_name to be accessed
std::string get_name(){return problem_name.str();}

///Allows the problem_name to be accessed
std::fstream& get_data_file(){return data_file;}	
///Allows the problem_name to be accessed
std::fstream& get_stat_file(){return stat_file;}	
///Allows the problem_name to be accessed
std::fstream& get_fstat_file(){return fstat_file;}	
///Allows the problem_name to be accessed
std::fstream& get_ene_file(){return ene_file;}

void set_fstat_filename(std::string filename){fstat_filename.str(""); fstat_filename << filename;}
void set_data_filename(std::string filename){data_filename.str(""); data_filename << filename;}
void set_stat_filename(std::string filename){stat_filename.str(""); stat_filename << filename;}
void set_ene_filename(std::string filename){ene_filename.str(""); ene_filename << filename;}

void set_fstat_filename(){fstat_filename.str(""); fstat_filename << problem_name.str().c_str()  << ".fstat";}
void set_data_filename(){data_filename.str(""); data_filename << problem_name.str().c_str()  << ".data";}
void set_stat_filename(){stat_filename.str(""); stat_filename << problem_name.str().c_str()  << ".stat";}
void set_ene_filename(){ene_filename.str(""); ene_filename << problem_name.str().c_str()  << ".ene";}

std::string get_fstat_filename(){return fstat_filename.str();}
std::string get_data_filename(){return data_filename.str();}
std::string get_stat_filename(){return stat_filename.str();}
std::string get_ene_filename(){return ene_filename.str();}


void set_step_size(unsigned int new_){step_size= new_;}	
unsigned int get_step_size(){return step_size;}	

///set and get for file options
void set_options_fstat(unsigned int new_){options_fstat = new_;}
unsigned int get_options_fstat(void){return options_fstat;}

void set_options_data(unsigned int new_){options_data = new_;}
unsigned int get_options_data(void){return options_data;}

void set_options_stat(unsigned int new_){options_stat = new_;}
unsigned int get_options_stat(void){return options_stat;}

void set_options_restart(unsigned int new_){options_restart = new_;}
unsigned int get_options_restart(void){return options_restart;}

void set_options_ene(unsigned int new_){options_ene = new_;}
unsigned int get_options_ene(void){return options_ene;}


bool open_file (std::fstream& file, std::string filename, unsigned int options, std::fstream::openmode mode) 
{
	if (options==1) {
		//std::cout << "open " << filename << std::endl;
		file.open( filename.c_str(), mode);
		if (file.fail()) return false;
	}
	return true;
}

bool open_fstat_file (std::fstream::openmode mode = std::fstream::out) {
	return open_file (fstat_file, fstat_filename.str(), get_options_fstat(), mode);
}
bool open_data_file (std::fstream::openmode mode = std::fstream::out) {
	return open_file (data_file, data_filename.str(), get_options_data(), mode);
}
bool open_stat_file (std::fstream::openmode mode = std::fstream::out) {
	return open_file (stat_file, stat_filename.str(), get_options_stat(), mode);
}
bool open_ene_file (std::fstream::openmode mode = std::fstream::out) {
	return open_file (ene_file, ene_filename.str(), get_options_ene(), mode);
}

/// opens file needed if data is written in multiple files
bool open_counted_file(std::fstream& file, std::string filenameNoCount, std::fstream::openmode mode){
	//create filename
	std::stringstream filename;
	filename << filenameNoCount << ".";
	if (file_counter<1000) filename << "0";
	if (file_counter<100) filename << "0";
	if (file_counter<10) filename << "0";
	filename << file_counter;
	//close old file
	if (file.is_open()) file.close();
	//open new file
	file.open(filename.str().c_str(), mode);
	if (!file.is_open()) {std::cerr << "Error in opening " << filename.str() << std::endl; return false;}
	//else std::cout << "open(" << mode << ") " << filename.str() << std::endl;
	//increase counter
	file_counter += step_size;
	return true;
}

bool increase_counter_fstat(std::fstream::openmode mode){
	file_counter -= step_size;
	return open_counted_file(fstat_file, fstat_filename.str(), mode);
}
bool increase_counter_data(std::fstream::openmode mode){
	return open_counted_file(data_file, data_filename.str(), mode);
}
bool increase_counter_stat(std::fstream::openmode mode){
	file_counter -= step_size;
	return open_counted_file(stat_file, stat_filename.str(), mode);
}
bool increase_counter_ene(std::fstream::openmode mode){
	file_counter -= step_size;
	return open_counted_file(ene_file, ene_filename.str(), mode);
}

void set_file_counter(int new_) {file_counter = new_;}
int get_file_counter() {return file_counter;}

private:
/// The stores the run number for saving
int counter;

protected:
///Stores the problem_name.
std::stringstream problem_name;

///These store the save file names, 
///by default they are derived from problem_name.
std::stringstream data_filename;
std::stringstream stat_filename;
std::stringstream fstat_filename;
std::stringstream ene_filename;

///Stream used for data files
std::fstream data_file;
std::fstream stat_file;
std::fstream fstat_file;
std::fstream ene_file;


///Indicators if files are created or not
/// 0: file will not be created
/// 1: file will be written in one file
/// 2: file will be written in multiple files
unsigned int options_fstat; 
unsigned int options_data; 
unsigned int options_stat; 
unsigned int options_ene; 
unsigned int options_restart; 

/// Counter needed if file will be written in multiple files
unsigned int file_counter;
unsigned int step_size;
};


#endif
