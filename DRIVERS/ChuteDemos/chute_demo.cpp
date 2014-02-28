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
#include "../../src/Chute.h"
using namespace std;

/** A quasi-2D inclined plane with inflow conditions on the left boundary, 
 * and deletion of particles when they exit the domain.
 **/
int main(int argc, char *argv[])
{
    //Print description
    cout << endl << "Description: A quasi-2D inclined plane with inflow conditions on the left boundary, and deletion of particles when they exit the domain." << endl;
 
    // Problem parameters
    Chute problem;
    problem.set_name("chute_demo");
    problem.set_tmax(0.5);
    
    // Particle properties
    problem.set_FixedParticleRadius(0.001);
    problem.set_InflowParticleRadius(0.001);
    problem.set_collision_time_and_restitution_coefficient(2.5e-3, 0.8);
    problem.set_dispt(0.0);
    problem.set_dt(); 
    
    // Chute properties
    problem.set_ChuteAngle(30.0);
    problem.set_xmax(0.1);
    problem.set_ymax(2.0*problem.get_InflowParticleRadius());
    
    // Inflow properties
    problem.set_InflowHeight(0.1);
    problem.set_InflowVelocity(0.1);
    problem.set_InflowVelocityVariance(0.02);
    
    // Output properties
    problem.set_number_of_saves(1); //minimize output to the last timestep
    problem.set_number_of_saves_data(100); //allow enough data output so the evolution can be viewed in xballs
    problem.set_number_of_saves_ene(100);

    //solve
    problem.solve(argc, argv);
}
