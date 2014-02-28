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

#include <sstream>
#include <iostream>
#include <iomanip>
#include <cmath>
#include "../../src/ChuteWithHopper.h"
using namespace std;

/** A quasi-2D inclined plane with hopper inflow conditions, 
 * and deletion of particles when they exit the domain.
 **/
int main(int argc, char *argv[])
{
    //Print description
    cout << endl << "Description: A quasi-2D inclined plane with hopper inflow conditions and deletion of particles when they exit the domain." << endl;
    
    //Problem parameters
    ChuteWithHopper problem;
    problem.set_name("hopper_demo");
    problem.set_tmax(0.2);
    
    //Particle properties
    problem.set_rho(2400.0);
    problem.set_InflowParticleRadius(0.32e-3,0.40e-3);
    problem.set_collision_time_and_restitution_coefficient(4.0e-4, 0.95);
    problem.set_FixedParticleRadius(0.36e-3);
    problem.set_RandomizedBottom(false);
    problem.set_mu(0.5);
    problem.set_dt();
    problem.MD::set_dt(problem.get_dt()*5.0); //run five times as fast (will decrease accuracy)
    
    //Chute properties
    problem.set_ChuteAngle(10.0);
    problem.set_ChuteLength(40.0e-3);
    problem.set_ChuteWidth(3.2e-3/2.0);

    //Hopper properties
    problem.set_max_failed(1);
    double ExitHeight = 10.0e-3, ExitLength = ExitHeight, HopperAngle = 60.0, HopperLength = 4.0 * ExitLength;
    Mdouble HopperLowestPoint = ExitHeight - ExitLength * tan(problem.get_ChuteAngle());
    Mdouble HopperHeight=HopperLowestPoint + 1.1 * 0.5*(HopperLength+ExitLength) / tan(HopperAngle*constants::pi/180.0);
    Mdouble HopperCornerHeight = HopperHeight - 0.5*(HopperLength-ExitLength) / tan(HopperAngle*constants::pi/180.0);
    if (HopperCornerHeight<=0.0) 
    { 
        HopperHeight += -HopperCornerHeight + problem.get_P0()->get_Radius(); 
        HopperCornerHeight = problem.get_P0()->get_Radius(); 
    }    
    problem.set_Hopper(ExitLength,ExitHeight,HopperAngle,HopperLength,HopperHeight);

    // Output properties
    problem.set_number_of_saves(1); //minimize output to the last timestep
    problem.set_number_of_saves_data(100); //allow enough data output so the evolution can be viewed in xballs
    problem.set_number_of_saves_ene(100);
    cout << "Maximum allowed speed of particles: " << problem.get_maximum_velocity() << endl; //speed allowed before particles move through each other!
    cout << "Maximum possible speed of particles: " << problem.get_MaximumVelocityInducedByGravity() << endl; 
    cout << "dt=" << problem.get_dt() << endl;

    problem.setHGridDistribution(OLDHGRID);
    //solve
    problem.solve(argc, argv);
}
