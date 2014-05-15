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
