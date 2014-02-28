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
