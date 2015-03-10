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
		com_filename.str("");
		com_filename << name << ".com";
		
		//open in-streams
		data_file.open(data_filename.str().c_str(), std::fstream::in);
		
		if (data_file.fail())
		{
			std::cerr << "ERROR: Input file " << data_filename.str() << " not found" << std::endl;
			data_file.close();
			exit(-1);
		} else {
			std::cout << "Files opened: " << data_filename.str() << std::endl;
		}

		//open out-stream
		com_file.open(com_filename.str().c_str(), std::fstream::out);
		
		if (com_file.fail())
		{
			std::cerr << "ERROR: Output file " << com_filename.str() << " not found" << std::endl;
			com_file.close();
			exit(-1);
		} else {
			std::cout << "Files opened: " << com_filename.str() << std::endl;
		}

		splittingradius=0;
	}

	///Destructor
	~CFile() {
		data_file.close();
		std::cout << "Files closed: " << data_filename.str() << std::endl;
		com_file.close();
		std::cout << "Files closed: " << com_filename.str() << std::endl;
	}
		
	bool copy() {
		unsigned int N;
		double X, Y, Z;
		double CX, CY, CZ;
		double CX1, CY1, CZ1;
		double CX2, CY2, CZ2;
		double VX, VY, VZ;
		double T, R, M, M1, M2, mass;
		std::string line;
		std::stringstream output_filename;
		std::fstream output_file;				
		
		data_file >> N;
		while (data_file.good()) {
			M = M1 = M2 = 0;
			data_file >> T;
			//open, write, close output file
			getline(data_file,line);
			CX = CY = CZ = 0;
			CX1 = CY1 = CZ1 = 0;
			CX2 = CY2 = CZ2 = 0;
			for (unsigned int i=0; i<N; i++) {
				data_file >> X >> Y >> Z >> VX >> VY >> VZ >> R;
				mass=R*R*R;
				getline(data_file,line);
				CX += X*mass; CY += Y*mass; CZ += Z*mass; M+=mass;
				if (splittingradius) {
					if (R<splittingradius) {
						CX1 += X*mass; CY1 += Y*mass; CZ1 += Z*mass; M1+=mass;
					} else {
						CX2 += X*mass; CY2 += Y*mass; CZ2 += Z*mass; M2+=mass;
					}
				}
			}
			if (splittingradius) {
				com_file << T << " " << CX/M << " " << CY/M << " " << CZ/M;
				com_file << " " << CX1/M1 << " " << CY1/M1 << " " << CZ1/M1;
				com_file << " " << CX2/M2 << " " << CY2/M2 << " " << CZ2/M2 << std::endl;
			} else {
				com_file << T << " " << CX/M << " " << CY/M << " " << CZ/M << std::endl;
			}
			data_file >> N;
		}
		return true;
	}


private:
	///These store the save file names, 
	std::stringstream data_filename;
	std::stringstream com_filename;

	///Stream used for data files
	std::fstream data_file;
	std::fstream com_file;
	
public:
	//
	double splittingradius;
};

int main(int argc, char *argv[])
{
	if (argc<2) {
		std::cerr << "Please enter problem name as first argument" << std::endl;
		return -1;
	}
	std::string name(argv[1]);
	std::cout << "Name: " << name << std::endl;
	
	CFile files(name);
	
	//defines the splitting radius
	if (argc>2) files.splittingradius = atof(argv[2]);
	
	files.copy();
	std::cout << "finished writing files: " << name << std::endl;
	return 0;
}
