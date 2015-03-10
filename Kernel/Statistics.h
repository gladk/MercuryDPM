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

#ifndef STATISTICS_H
#define STATISTICS_H
#include "StatisticsVector.h"

/*!
 * \brief this is the function the user should call
 * \details
 */
void Statistics(unsigned int argc, char *argv[])
{
    
    if (argc > 1 && strcmp(argv[1], "-help"))
        std::cout << std::endl << "Get statistics for " << argv[1] << std::endl;
    
    //check for '-stattype' option
    StatType T = XYZ;
    for (unsigned int i = 2; i < argc; i++)
    {
        if (!strcmp(argv[i], "-stattype"))
        {
            if (!strcmp(argv[i + 1], "XYZ"))
                T = XYZ;
            else if (!strcmp(argv[i + 1], "RAZ"))
                T = RAZ;
            else if (!strcmp(argv[i + 1], "RA"))
                T = RA;
            else if (!strcmp(argv[i + 1], "RZ"))
                T = RZ;
            else if (!strcmp(argv[i + 1], "AZ"))
                T = AZ;
            else if (!strcmp(argv[i + 1], "R"))
                T = R;
            else if (!strcmp(argv[i + 1], "A"))
                T = A;
            else if (!strcmp(argv[i + 1], "XY"))
                T = XY;
            else if (!strcmp(argv[i + 1], "XZ"))
                T = XZ;
            else if (!strcmp(argv[i + 1], "YZ"))
                T = YZ;
            else if (!strcmp(argv[i + 1], "X"))
                T = X;
            else if (!strcmp(argv[i + 1], "Y"))
                T = Y;
            else if (!strcmp(argv[i + 1], "Z"))
                T = Z;
            else if (!strcmp(argv[i + 1], "O"))
                T = O;
            else
            {
                std::cerr << "stattype unknown" << std::endl;
                exit(-1);
            }
        }
    }
    if (T == XY)
    { // averaging in z-direction
        std::cout << "averaging in z-direction" << std::endl;
        StatisticsVector<XY> stats(argc, argv);
        stats.setDoPeriodicWalls(false);
        stats.statistics_from_fstat_and_data();
    }
    else if (T == XY)
    { // averaging in y-direction
        std::cout << "averaging in y-direction" << std::endl;
        StatisticsVector<XY> stats(argc, argv);
        stats.setDoPeriodicWalls(false);
        stats.statistics_from_fstat_and_data();
    }
    else if (T == XZ)
    { // averaging in y-direction
        std::cout << "averaging in y-direction" << std::endl;
        StatisticsVector<XZ> stats(argc, argv);
        stats.setDoPeriodicWalls(false);
        stats.statistics_from_fstat_and_data();
    }
    else if (T == YZ)
    { // averaging in x-direction
        std::cout << "averaging in x-direction" << std::endl;
        StatisticsVector<YZ> stats(argc, argv);
        stats.setDoPeriodicWalls(false);
        stats.statistics_from_fstat_and_data();
    }
    else if (T == X)
    { // averaging in yz-direction
        std::cout << "averaging in yz-direction" << std::endl;
        StatisticsVector<X> stats(argc, argv);
        stats.setDoPeriodicWalls(false);
        stats.statistics_from_fstat_and_data();
    }
    else if (T == Y)
    { // averaging in yz-direction
        std::cout << "averaging in xz-direction" << std::endl;
        StatisticsVector<Y> stats(argc, argv);
        stats.setDoPeriodicWalls(false);
        stats.statistics_from_fstat_and_data();
    }
    else if (T == Z)
    { // averaging in yz-direction
        std::cout << "averaging in xy-direction" << std::endl;
        StatisticsVector<Z> stats(argc, argv);
        stats.setDoPeriodicWalls(false);
        stats.statistics_from_fstat_and_data();
    }
    else if (T == O)
    { // averaging in all directions
        std::cout << "averaging in xyz-direction" << std::endl;
        StatisticsVector<O> stats(argc, argv);
        stats.setDoPeriodicWalls(false);
        stats.statistics_from_fstat_and_data();
    }
    else if (T == RAZ)
    { //no averaging
        std::cout << "cylindrical, no averaging" << std::endl;
        StatisticsVector<RAZ> stats(argc, argv);
        stats.statistics_from_fstat_and_data();
    }
    else if (T == RA)
    { //no averaging
        std::cout << "cylindrical, Z averaging" << std::endl;
        StatisticsVector<RA> stats(argc, argv);
        stats.statistics_from_fstat_and_data();
    }
    else if (T == RZ)
    { //no averaging
        std::cout << "cylindrical, A averaging" << std::endl;
        StatisticsVector<RZ> stats(argc, argv);
        stats.statistics_from_fstat_and_data();
    }
    else if (T == AZ)
    { //no averaging
        std::cout << "cylindrical, R averaging" << std::endl;
        StatisticsVector<AZ> stats(argc, argv);
        stats.statistics_from_fstat_and_data();
    }
    else if (T == A)
    { //no averaging
        std::cout << "cylindrical, RZ averaging" << std::endl;
        StatisticsVector<A> stats(argc, argv);
        stats.statistics_from_fstat_and_data();
    }
    else if (T == R)
    { //no averaging
        std::cout << "cylindrical, AZ averaging" << std::endl;
        StatisticsVector<R> stats(argc, argv);
        stats.statistics_from_fstat_and_data();
    }
    else if (T == XYZ)
    { //no averaging
        std::cout << "no averaging" << std::endl;
        StatisticsVector<XYZ> stats(argc, argv);
        stats.statistics_from_fstat_and_data();
    }
    
}

#endif

