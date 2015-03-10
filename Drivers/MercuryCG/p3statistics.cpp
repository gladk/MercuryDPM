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

#include <iostream>  // ostream
#include <iomanip>   // setw
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include "StatisticsVector.h"

///This code (p3dstatistics.cpp) can be used to create statistics from p3d files. 
///It is a modified version of fstatistics.cpp
int main(int argc, char *argv[]) {	
	
	if (argc>1&&strcmp(argv[1],"-help")) std::cout << std::endl << "Get statistics for " << argv[1] << std::endl;

	//check for '-stattype' option
	StatType T = XYZ;
	for (int i = 2; i<argc; i++) {
		if (!strcmp(argv[i],"-stattype")) {
			if (!strcmp(argv[i+1],"XYZ")) T = XYZ;
			else if (!strcmp(argv[i+1],"RAZ")) T = RAZ;
			else if (!strcmp(argv[i+1],"RA")) T = RA;
			else if (!strcmp(argv[i+1],"RZ")) T = RZ;
			else if (!strcmp(argv[i+1],"AZ")) T = AZ;
			else if (!strcmp(argv[i+1],"R")) T = R;
			else if (!strcmp(argv[i+1],"A")) T = A;
			else if (!strcmp(argv[i+1],"YZ")) T = YZ;
			else if (!strcmp(argv[i+1],"XZ")) T = XZ;
			else if (!strcmp(argv[i+1],"XY")) T = XY;
			else if (!strcmp(argv[i+1],"X")) T = X;
			else if (!strcmp(argv[i+1],"Y")) T = Y;
			else if (!strcmp(argv[i+1],"Z")) T = Z;
			else if (!strcmp(argv[i+1],"O")) T = O;
			else {std::cerr << "stattype unknown" << std::endl; exit(-1);}
		}
	}
 	if (T==XY) { // averaging in z-direction
 		std::cout << "averaging in z-direction" << std::endl;
 		StatisticsVector<XY> stats(argc, argv);
 		stats.setDoPeriodicWalls(false);
 		stats.statistics_from_p3();
	} else if (T==XZ) { // averaging in y-direction
		std::cout << "averaging in y-direction" << std::endl;
		StatisticsVector<XZ> stats(argc, argv);
		stats.setDoPeriodicWalls(false);
		stats.statistics_from_p3();
 	} else if (T==YZ) { // averaging in x-direction
 		std::cout << "averaging in x-direction" << std::endl;
 		StatisticsVector<YZ> stats(argc, argv);
 		stats.setDoPeriodicWalls(false);
 		stats.statistics_from_p3();
 	} else if (T==X) { // averaging in yz-direction
 		std::cout << "averaging in yz-direction" << std::endl;
 		StatisticsVector<X> stats(argc, argv);
 		stats.setDoPeriodicWalls(false);
 		stats.statistics_from_p3();
	} else if (T==Y) { // averaging in yz-direction
 		std::cout << "averaging in xz-direction" << std::endl;
 		StatisticsVector<Y> stats(argc, argv);
 		stats.setDoPeriodicWalls(false);
 		stats.statistics_from_p3();
	} else if (T==Z) { // averaging in yz-direction
		std::cout << "averaging in xy-direction" << std::endl;
		StatisticsVector<Z> stats(argc, argv);
		stats.setDoPeriodicWalls(false);
		stats.statistics_from_p3();
 	} else if (T==O) { // averaging in all directions
 		std::cout << "averaging in xyz-direction" << std::endl;
 		StatisticsVector<O> stats(argc, argv);
 		stats.setDoPeriodicWalls(false);
 		stats.statistics_from_p3();
// 	} else if (T==RAZ) { //no averaging
// 		std::cout << "cylindrical, no averaging" << std::endl;
// 		StatisticsVector<RAZ> stats(argc, argv);
// 		stats.statistics_from_p3();
// 	} else if (T==RA) { //no averaging
// 		std::cout << "cylindrical, Z averaging" << std::endl;
// 		StatisticsVector<RA> stats(argc, argv);
// 		stats.statistics_from_p3();
// 	} else if (T==RZ) { //no averaging
// 		std::cout << "cylindrical, A averaging" << std::endl;
// 		StatisticsVector<RZ> stats(argc, argv);
// 		stats.statistics_from_p3();
// 	} else if (T==AZ) { //no averaging
// 		std::cout << "cylindrical, R averaging" << std::endl;
// 		StatisticsVector<AZ> stats(argc, argv);
// 		stats.statistics_from_p3();
// 	} else if (T==A) { //no averaging
// 		std::cout << "cylindrical, RZ averaging" << std::endl;
// 		StatisticsVector<A> stats(argc, argv);
// 		stats.statistics_from_p3();
// 	} else if (T==R) { //no averaging
// 		std::cout << "cylindrical, AZ averaging" << std::endl;
// 		StatisticsVector<R> stats(argc, argv);
// 		stats.statistics_from_p3();
	} else if (T==XYZ) { //no averaging
		std::cout << "no averaging" << std::endl;
		StatisticsVector<XYZ> stats(argc, argv);
		std::cout << "stats" << std::endl;
		stats.statistics_from_p3();
	} else {std::cerr << "stattype not found" << std::endl; exit(-1); }
	return 0;
}
	
