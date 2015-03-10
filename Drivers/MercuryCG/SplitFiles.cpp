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

///takes data and fstat files and splits them into *.data.???? and *.fstat.???? files

#include <string>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/stat.h> 
#include <stdio.h>
#include <stdlib.h>


class CFile {

public:

	///Constructor
	CFile(std::string name) {
		//set file names
		data_filename.str("");
		data_filename << name << ".data";
		fstat_filename.str("");
		fstat_filename << name << ".fstat";
		
		//open in-streams
		data_file.open(data_filename.str().c_str(), std::fstream::in);
		fstat_file.open(fstat_filename.str().c_str(), std::fstream::in);
		
		if (data_file.fail() || fstat_file.fail())
		{
			std::cerr << "ERROR: Input file " << data_filename.str() << " or " << fstat_filename.str() << " not found" << std::endl;
			data_file.close();
			fstat_file.close();
			exit(-1);
		} else {
			std::cout << "Files opened: " << data_filename.str() << " and " << fstat_filename.str() << std::endl;
		}
	}

	///Destructor
	~CFile() {
		data_file.close();
		fstat_file.close();
		std::cout << "Files closed: " << data_filename.str() << " and " << fstat_filename.str() << std::endl;
	}
		
	bool copy(unsigned int stepsize, unsigned int counter) {
		//return copy_last_time_step();
		return copy_data(stepsize,counter) && copy_fstat(stepsize,counter);
	}

	bool copy_data(unsigned int stepsize, unsigned int counter) {
		unsigned int N;
		std::string line;
		std::stringstream output_filename;
		std::fstream output_file;				
		
		data_file >> N;
		while (data_file.good()) {
			//set output_filename
			std::stringstream output_filename("");
			output_filename << data_filename.str() << ".";
			if (counter<1000) output_filename << "0";
			if (counter<100) output_filename << "0";
			if (counter<10) output_filename << "0";
			output_filename << counter;
			counter ++;
			//std::cout << "Outputfile: " << output_filename.str() << std::endl;
			
			//open, write, close output file
			output_file.open(output_filename.str().c_str(), std::fstream::out);
			getline(data_file,line);
			output_file << N << line << std::endl;
			std::cout << N << line << std::endl;
			for (unsigned int i=0; i<N; i++) {
				getline(data_file,line);
				output_file << line << std::endl;
			}
			output_file.close();
			data_file >> N;

			//step over some timesteps
			for(unsigned int j=1; j<stepsize; j++) {
				for (unsigned int i=0; i<N+1; i++) getline(data_file,line);
				data_file >> N;
			}

		}
		return true;
	}

	bool copy_fstat(unsigned int stepsize, unsigned int counter) {
		std::string line;
		std::stringstream output_filename;
		std::fstream output_file;				
		
		getline(fstat_file,line);
		while (fstat_file.good()) {
			//set output_filename
			output_filename.str("");
			output_filename << fstat_filename.str() << ".";
			if (counter<1000) output_filename << "0";
			if (counter<100) output_filename << "0";
			if (counter<10) output_filename << "0";
			output_filename << counter;
			counter ++;

			//open, write, close output file
			output_file.open(output_filename.str().c_str(), std::fstream::out);
			std::cout << line << std::endl;
			output_file << line << std::endl;
			getline(fstat_file,line); 
			output_file << line << std::endl;
			getline(fstat_file,line); 
			output_file << line << std::endl;
			getline(fstat_file,line);
			while (line.c_str()[0] != '#'&&fstat_file.good()) {
				getline(fstat_file,line);
				output_file << line << std::endl;
			}
			output_file.close();

			//step over some timesteps
			for(unsigned int j=1; j<stepsize; j++) {
				getline(fstat_file,line); 
				getline(fstat_file,line); 
				getline(fstat_file,line);
				while (line.c_str()[0] != '#'&&fstat_file.good()) {
					getline(fstat_file,line);
				}
			}
		}
		return true;
	}


private:
	///These store the save file names, 
	std::stringstream data_filename;
	std::stringstream fstat_filename;

	///Stream used for data files
	std::fstream data_file;
	std::fstream fstat_file;
};

int main(int argc, char *argv[])
{
	if (argc<2) {
		std::cerr << "split_files problem_name [stepsize [initial_counter]]" << std::endl;
		return -1;
	}
	std::string name(argv[1]);
	std::cout << "Name: " << name << std::endl;
	
	unsigned int stepsize = 1;
	if (argc>2) stepsize = atoi(argv[2]);
	
	//defines the initial counter
	unsigned int counter = 0;
	if (argc>3) counter = atoi(argv[3]);

	CFile files(name);
	files.copy(stepsize,counter);
	std::cout << "finished writing split files: " << name << std::endl;
	return 0;
}
