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

#ifndef CHUTEBOTTOM_H
#define CHUTEBOTTOM_H
#include "Chute.h"

/**  Used by Chute::create_bottom to create an unordered particle layer.
 * 
 * It creates a thick layer of particles in a box of the same x and y
 * dimensions as the Chute, and places a thin slice of that layer 
 * as fixed particles at the base of the chute.
 */ 
class ChuteBottom : public Chute {

public:
	///This is the default constructor. All it does is set sensible defaults.
	ChuteBottom() {constructor();}
	
	///Copy-constructor for creates an HGRID problem from an existing MD problem
	ChuteBottom(MD& other) :			MD(other), Chute(other) {constructor();}
	ChuteBottom(HGRID_base& other) :	MD(other), Chute(other) {constructor();}
	ChuteBottom(HGRID_3D& other) :		MD(other), Chute(other) {constructor();}
	ChuteBottom(Chute& other) :			MD(other), Chute(other) {constructor();}
	
	///This is the actual constructor it is called do both constructors above.
	void constructor(){	
		set_name("roughbottom");
		set_options_fstat(0); //set to 0 for no data creation
		set_options_data(1);
		set_options_restart(1);
		set_thickness(2.4);
		set_periodicbottom(true);
	}

	void make_rough_bottom(ParticleHandler& ChuteParticleHandler) 
	{
		// set all parameters that should be different from the original chute 
		set_ChuteAngle(0.0);
		set_InflowHeight(25.*get_InflowParticleRadius());
		//~ set_InflowHeight(45.*get_InflowParticleRadius()); note: Changing the Inflow height was an attempt to make the bottom density homogeneous, but it did not have the desired effect
		set_RandomizedBottom(1);
		set_FixedParticleRadius(get_InflowParticleRadius());

		set_collision_time_and_restitution_coefficient(get_collision_time()*10.0, 0.2);
		set_mu(0);
		
		set_dt();
		set_dt(get_dt()*10.0);
		set_tmax(get_dt()*2e3);
		//set_number_of_saves(2);
		set_savecount(100);

		//solve
		solve();

		//create_bottom		
		Mdouble height = 0;
		for (std::vector<BaseParticle*>::iterator it=getParticleHandler().begin(); it!=getParticleHandler().end(); it++) {
			height = std::max(height,(*it)->get_Position().Z);
		}
		
		std::cout << "Thickness" << thickness << std::endl;
		///todo{Dinant is not a fan of this alogirm (i.e. poping back stuff while in iterator}
		//now cut a slice of width 2*MaxInflowParticleRadius
		for (std::vector<BaseParticle*>::iterator it=getParticleHandler().begin(); it!=getParticleHandler().end();)
        {
			if ((*it)->get_Position().Z < height - (1.0+thickness)*MaxInflowParticleRadius || (*it)->get_Position().Z > height - MaxInflowParticleRadius) {
				//delete particles outside the given range
				//*it = getParticleHandler().back();
				//getParticleHandler().removeLastObject(); 
				getParticleHandler().removeObject((*it)->get_Index());
			} else {
				//fix the remaining particles on the floor

				//(*it)->get_Position().Z -= height - MaxInflowParticleRadius;
                (*it)->move(Vec3D(0.0,0.0,MaxInflowParticleRadius-height));
                
				(*it)->fixParticle();
				it++;
			}
		}
		
		//copy the rough bottom over
		ChuteParticleHandler.setStorageCapacity(getParticleHandler().getNumberOfObjects());
		std::cout << "Chute bottom finished, consisting of " << getParticleHandler().getNumberOfObjects() << " Particles" << std::endl; 
		
		ChuteParticleHandler = getParticleHandler();
	}

	void setup_particles_initial_conditions() {	
		

		getParticleHandler().setStorageCapacity((int)std::min(3.0*get_xmax()*get_ymax()*get_zmax()/cubic(2.0*get_InflowParticleRadius()),1e6));


		create_bottom();
		
		///todo{The create_bottom() function also creates some walls and boudarys, but at slightly different locations. In this version they are removed and reset, but this is (in my opinion (Dinant)) not the correct way}
		getWallHandler().clear();
		getBoundaryHandler().clear();
		if (periodicbottom) {
			InfiniteWall w0;
			w0.set(Vec3D(0.0, 0.0, -1.0), -get_zmin()+get_InflowParticleRadius());
			getWallHandler().copyAndAddWall(w0);
			PeriodicBoundary b0;
			b0.set(Vec3D( 1.0, 0.0, 0.0), get_xmin(), get_xmax());
			getBoundaryHandler().copyAndAddObject(b0);
			b0.set(Vec3D( 0.0, 1.0, 0.0), get_ymin(), get_ymax());
			getBoundaryHandler().copyAndAddObject(b0);
		} else {
			InfiniteWall w0;
			w0.set(Vec3D(0.0, 0.0, -1.0), -get_zmin()+get_InflowParticleRadius());
			getWallHandler().copyAndAddWall(w0);
			w0.set(Vec3D(-1.0, 0.0, 0.0), -get_xmin());
			getWallHandler().copyAndAddWall(w0);
			w0.set(Vec3D( 1.0, 0.0, 0.0),  get_xmax());
			getWallHandler().copyAndAddWall(w0);
			w0.set(Vec3D( 0.0,-1.0, 0.0), -get_ymin());
			getWallHandler().copyAndAddWall(w0);
			w0.set(Vec3D( 0.0, 1.0, 0.0),  get_ymax());
			getWallHandler().copyAndAddWall(w0);
		}
		
		//add particles
		HGRID_actions_before_time_loop();
		int failed = 0, max_failed = 500;
		//try max_failed times to find new insertable particle
		while (failed<=max_failed){
			P0.set_Radius(FixedParticleRadius);
			P0.compute_particle_mass(Species);

            //The position components are first stored in a Vec3D, because if you pass them directly into set_Position the compiler is allowed to change the order in which the numbers are generated
            Vec3D position;
            position.X=random.get_RN(  P0.get_Radius(), get_xmax()-P0.get_Radius());
            position.Y=random.get_RN(  P0.get_Radius(), get_ymax()-P0.get_Radius());
            position.Z=random.get_RN(2*P0.get_Radius(), get_zmax()-P0.get_Radius());
            P0.set_Position(position);
			P0.set_Velocity(Vec3D(0.0,0.0,0.0));

			if (IsInsertable(P0)) 
			{
				failed = 0; 
				///todo{Why is this an error?}
				/*if (Particles.capacity()==Particles.size()) 
				{
					
					std::cerr << "Error in creating bottom; exceeded capacity: " << Particles.size() << std::endl; exit(-1);
				}*/
				//Particles.push_back (P0); 
				//Undate the link list so this particle is back in the grid
				//grid->objectBucket[Particles[Particles.size()-1].bucket] = Particles.size()-1;
				//grid->objectBucket[Particles[Particles.size()-1].bucket] = &Particles[Particles.size()-1];
				num_created++;
			} 
			else failed++;
		}
		//set_Nmax(getParticleHandler().getNumberOfObjects());
		std::cout << "N=" << getParticleHandler().getNumberOfObjects() << std::endl;

		//fix hgrid (there is still an issue when particles are polydispersed)
		//assume 1-2 levels are optimal (which is the case for mono and bidispersed) and set the cell size to min and max 
		// !this is not optimal for polydispersed
		Mdouble minCell = 2.*std::min(get_FixedParticleRadius(),get_MinInflowParticleRadius());
		Mdouble maxCell = 2.*std::max(get_FixedParticleRadius(),get_MaxInflowParticleRadius());
		if ((minCell==maxCell)|(minCell==0.)) setHGridMaxLevels(1);
		else setHGridMaxLevels(2);
//		set_HGRID_cell_to_cell_ratio (1.0000000001*maxCell/minCell);
		//optimize number of buckets
		set_HGRID_num_buckets_to_power(getParticleHandler().getNumberOfObjects()*1.5);
		//end: fix hgrid
		
		//~ info();
	}
		
	void actions_before_time_step() {	};
	
	Mdouble get_thickness(){return thickness;}
	void set_thickness(Mdouble new_){
		if (new_>0.0) thickness=new_;
		else {std::cerr<<"Error: thickness " << new_ << " negative"<<std::endl; exit(-1);}
	}
	Mdouble get_periodicbottom(){return periodicbottom;}
	void set_periodicbottom(bool new_){periodicbottom=new_;}
	
private:
	Mdouble thickness; 
	bool periodicbottom; 
};
#endif
