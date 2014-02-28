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
