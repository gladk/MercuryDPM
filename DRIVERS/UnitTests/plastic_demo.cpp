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

#include "../../src/MD.h"
#include <iostream>
#include "../../src/Particles/DeltaMaxsParticle.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////
///This does the force computation
////////////////////////////////////////////////////////////////////////////////////////////////////
class Plastic : public MD{

	void compute_internal_forces(BaseParticle *PI, BaseParticle *PJ) {
		compute_plastic_internal_forces(PI, PJ);
	}

public:
	void setup_particles_initial_conditions()
	{
		set_dim(3);
		set_dim_particle(3);
		
		set_xmax( 3);
		set_ymax( 2);
		set_zmax( 2);
		set_gravity(Vec3D(0,0,0));
		set_rho(6/constants::pi);
		
		double depth = 0.05;
		double deltamax = (get_k2max()/(get_k2max()-get_k1()))*depth*((2*0.5*0.5)/(0.5+0.5));
		double relVelocity=sqrt(get_k1()*(chi*deltamax)*(chi*deltamax)*2);
		
        getParticleHandler().clear();
		
		DeltaMaxsParticle P0,P1;
		P0.set_Position(Vec3D( 1,1,1));
		P0.set_Velocity(Vec3D(relVelocity/2,0,0));
		P0.set_Radius(0.5);
		P1.set_Position(Vec3D( 2,1,1));
		P1.set_Velocity(Vec3D(-relVelocity/2,0,0));
		P1.set_Radius(0.5);
		
		getParticleHandler().copyAndAddObject(P0);
		getParticleHandler().copyAndAddObject(P1);		
		
		reset_TangentialSprings();
		reset_DeltaMax();
				
		double mass=1.0;
		set_dt(get_plastic_dt(mass));
		set_tmax(get_dt()*50);
		set_savecount(2);
	}
	
	void set_chi(double new_){chi=new_;}
	double get_chi() {return chi;}
	double chi;
};

int main(int argc UNUSED, char *argv[] UNUSED)
{	
	Plastic md;
	double k1=100;		
	md.set_plastic_k1_k2max_kc_depth(k1, 5.*k1, k1, 0.05);
	md.set_disp(0);
	
	double chi[] = {0.34, 0.69, 1.1, 1.37};
	for (int i=0; i<4; i++) {
		md.set_chi(chi[i]);
		std::stringstream ss("");
		ss << "collinear" << md.get_chi();
		md.set_name(ss.str().c_str());
		md.solve();
	}

	//to display use
	//gnuplot> plot 'plastic/collinear1.1.fstat' u 7:9 w lp
}
