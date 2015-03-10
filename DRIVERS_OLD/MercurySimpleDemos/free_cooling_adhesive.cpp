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

///todo{This code is not working as is wanted}
#include<iostream>

#include "../../src/MD.h"
#include "../../src/HGRID_2D.h"


/// In this file 32^2 particles with the same velocity are placed in a
/// bi-axial box. This makes them collide with the walls and each other.
/// Afterwards the same run is performed with hgrid on.
/// It tests the working (and speedup) of the hgrid.

class my_problem_HGRID : public HGRID_2D{
public:
    
	void setup_particles_initial_conditions()
	{
		int N1=static_cast<int>(ceil(sqrt(N)));
		TangentialSpringParticle p0;
        p0.set_Radius(0.0001);
		for (int i=0;i<N;i++)
		{
            int ix=static_cast<int>(i%N1);
			int iy=static_cast<int>(i/N1);
            
			double x=(get_xmax()-get_xmin())*(ix+1)/(N1+1);
			double y=(get_ymax()-get_ymin())*(iy+1)/(N1+1);
			
            
			p0.set_Position(Vec3D(x,y,0.0));
			Vec3D vel;
            vel.X=random.get_RN(-1,1);
            vel.Y=random.get_RN(-1,1);
            p0.set_Velocity(1.0*vel);
			getParticleHandler().copyAndAddObject(p0);
            p0.set_Radius(0.0001*random.get_RN(0.7,1));
		}
		
		getWallHandler().clear();
		InfiniteWall w0;
		w0.set(Vec3D(-1, 0, 0), -get_xmin());
		getWallHandler().copyAndAddWall(w0);
		w0.set(Vec3D( 1, 0, 0),  get_xmax());
		getWallHandler().copyAndAddWall(w0);
		w0.set(Vec3D( 0,-1, 0), -get_ymin());
		getWallHandler().copyAndAddWall(w0);
		w0.set(Vec3D( 0, 1, 0),  get_ymax());
		getWallHandler().copyAndAddWall(w0);
	}
    
	int N;
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
	std::cout<<"In this file 32^2 particles with the same velocity are placed "
	"in a bi-axial box. This makes them collide with the walls and eachother. "
	"Afterwards the same run is performed with hgrid on. It tests the working "
	"(and speedup) of the hgrid."<<std::endl;
	
	///Start off my solving the default problem
 	my_problem_HGRID problem;
 	problem.N=25;
 	problem.set_name("free_cooling_adhesive");
    problem.set_xmax(0.0018);
    problem.set_ymax(0.0018);
 	problem.set_dissipation(0.01);
 	problem.set_gravity(Vec3D(0.0,0.0,0.0));
	problem.set_k(1e4);
	problem.set_dt(3e-6);
	problem.set_mu(0.1);
    problem.set_rho(2000);
	problem.set_savecount(50);
 	problem.set_tmax(0.05);
    problem.setHGridMaxLevels(1);
	//problem.setHGridCellOverSizeRatio(1.2);
	problem.setHGridUpdateEachTimeStep(true);

    problem.get_Species(0)->set_k0(0.05*problem.get_k());
    problem.get_Species(0)->set_f0(1.1e-4*0.05*problem.get_k());
    problem.get_Species(0)->set_AdhesionForceType(LinearReversible);
    problem.set_options_data(1);
    problem.set_options_fstat(0);
    problem.solve();
    std::cout << problem.getParticleHandler().getObject(0)->get_Radius() << std::endl;
    std::cout << problem.getParticleHandler().getObject(0)->get_InteractionRadius() << std::endl;
    std::cout << problem.grid->cellSizes_[0] << std::endl;
 }