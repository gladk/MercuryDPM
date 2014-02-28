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
#include "../../src/HGRID_3D.h"


/// In this file 10^3 particles with the same velocity are placed in a tri-axial box. This makes them collide with the walls and eachother. Afterwards the same run is performed with hgrid on. It tests the working (and speedup) of the hgrid.

class my_problem : public virtual MD{
public:
	
	void setup_particles_initial_conditions()
	{
		int N1=static_cast<int>(pow(N,0.33))+1;	
		BaseParticle p0;		
		for (int i=0;i<N;i++)
		{
			int ix=static_cast<int>(i%N1);
			int iz=static_cast<int>(i/N1/N1);
			int iy=(i-ix-N1*N1*iz)/N1;
		
			double x=(get_xmax()-get_xmin())*(ix+1)/(N1+1);
			double y=(get_ymax()-get_ymin())*(iy+1)/(N1+1);
			double z=(get_zmax()-get_zmin())*(iz+1)/(N1+1);
			
		
			p0.set_Position(Vec3D(x,y,z));
			p0.set_Velocity(Vec3D(0.1,0.1,0.1));
			p0.set_Radius(0.0001);
			getParticleHandler().copyAndAddObject(p0);
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
		w0.set(Vec3D( 0, 0,-1), -get_zmin());
		getWallHandler().copyAndAddWall(w0);
		w0.set(Vec3D( 0, 0, 1),  get_zmax());
		getWallHandler().copyAndAddWall(w0);
	}
	
	int N;
};

class my_problem_HGRID : public HGRID_3D{
public:

	my_problem_HGRID(my_problem& other) : MD(other), HGRID_3D(other){}
	
	void setup_particles_initial_conditions()
	{
		int N=getParticleHandler().getNumberOfObjects();
		int N1=static_cast<int>(pow(N,0.33))+1;
		
		for (int i=0;i<N;i++)
		{
			int ix=static_cast<int>(i%N1);
			int iz=static_cast<int>(i/N1/N1);
			int iy=(i-ix-N1*N1*iz)/N1;
		
			double x=(get_xmax()-get_xmin())*(ix+1)/(N1+1);
			double y=(get_ymax()-get_ymin())*(iy+1)/(N1+1);
			double z=(get_zmax()-get_zmin())*(iz+1)/(N1+1);
			
			getParticleHandler().getObject(i)->set_Position(Vec3D(x,y,z));
			getParticleHandler().getObject(i)->set_Velocity(Vec3D(0.1,0.1,0.1));
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
		w0.set(Vec3D( 0, 0,-1), -get_zmin());
		getWallHandler().copyAndAddWall(w0);
		w0.set(Vec3D( 0, 0, 1),  get_zmax());
		getWallHandler().copyAndAddWall(w0);
	}

	int N;
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
	std::cout<<"In this file 10^3 particles with the same velocity are placed "
	"in a tri-axial box. This makes them collide with the walls and eachother. "
	"Afterwards the same run is performed with hgrid on. It tests the working "
	"(and speedup) of the hgrid."<<std::endl;
	
	///Start off my solving the default problem
 	my_problem problem;
 	problem.N=1000;
 	problem.set_name("free_cooling3D");
 	problem.set_dissipation(0.005);
	problem.set_gravity(Vec3D(0.0,0.0,0.0));
	problem.set_k(1e3);
	problem.set_dt(5e-5);
	problem.set_savecount(4);
 	problem.set_tmax(0.08);
    problem.set_dim(3);
	problem.set_zmax(0.01);
	problem.solve();
    
 	std::cout << "Now do the same problem with the HGrid turned on." << std::endl;
	my_problem_HGRID problem2(problem);
	problem2.set_name("free_cooling_hgrid3D");
    problem2.setHGridMaxLevels(1);
	problem2.setHGridCellOverSizeRatio(1.2);
	problem2.setHGridUpdateEachTimeStep(false);    
	problem2.solve();
}

