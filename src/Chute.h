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

#ifndef CHUTE_H
#define CHUTE_H
#include "HGRID_3D.h"

///Chute adds three new effects to the HGrid: the gravity direction can 
///be set using the ChuteAngle variable, a (smooth or rough) bottom wall 
///is created by default, and some basic inflow and outflow routines are 
///added
class Chute : public HGRID_3D { 
public:
	
	///This is the default constructor. All it does is set sensible defaults.
	Chute()
	{
		constructor();
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "Chute() finished" << std::endl;
		#endif		
	}
	
	///Copy-constructor for creates an HGRID problem from an existing MD problem
	//The copy-constructor of MD has to be called because the link from MD to HGRID_base is virtual
	Chute(MD& other) : MD(other), HGRID_3D(other)
		{
		constructor();
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "Chute(MD& other) finished" << std::endl;
		#endif		
	}
	Chute(HGRID_base& other) : MD(other), HGRID_3D(other)
		{
		constructor();
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "Chute(HGRID_base& other)  finished" << std::endl;
		#endif		
	}
	Chute(HGRID_3D& other) : MD(other), HGRID_3D(other)
		{
		constructor();
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "Chute(HGRID_3D& other) finished" << std::endl;
		#endif		
	}
	
	///This is the actual constructor; it is called do both constructors above.
	void constructor();
	
	///This makes the chute periodic, in y
	void make_chute_periodic(){is_periodic=true;}
	
	///Get wether the chute is periodic
	bool get_IsPeriodic() {return is_periodic;}
	
	/// initialize particle position, velocity, radius
	void setup_particles_initial_conditions();
	
	///This function reads all chute data
	void read(std::istream& is);

	///This function writes all chute data
	virtual void write(std::ostream& os);
	
	///This function std::couts all chute data
	void print(std::ostream& os, bool print_all=false);

	///Allows radius of fixed particles to be changed
	void set_FixedParticleRadius(Mdouble new_){if (new_ >= 0.0) FixedParticleRadius=new_; else std::cerr << "WARNING : Fixed particle radius must be greater than or equal to zero" << std::endl;}
	///Allows radius of fixed particles to be accessed
	Mdouble get_FixedParticleRadius(){return FixedParticleRadius;}
	
	///Changes RandomizedBottom
	void set_RandomizedBottom(int new_){RandomizedBottom = new_;}
	///Accesses RandomizedBottom
	int get_RandomizedBottom(){return RandomizedBottom;}
	
	///Sets gravity vector according to chute angle (in degrees)
	void set_ChuteAngle(Mdouble new_){Mdouble gravity = get_gravity().GetLength(); if (gravity==0) {std::cerr<<"WARNING: zero gravity";} set_ChuteAngle(new_, gravity);}
	///Sets gravity vector according to chute angle (in degrees)
	void set_ChuteAngle(Mdouble new_, Mdouble gravity){if (new_>=-90.0&&new_<=90.0) {ChuteAngle = new_*constants::pi/180.0; set_gravity(Vec3D(sin(ChuteAngle), 0.0, -cos(ChuteAngle))*gravity);} else std::cerr << "WARNING : Chute angle must be within [-90,90]" << std::endl;}
	//void set_ChuteAngle(Mdouble new_, Mdouble gravity){if (new_>=0.0&&new_<=90.0) {ChuteAngle = new_; set_gravity(Vec3D(sin(ChuteAngle*pi/180.0), 0.0, -cos(ChuteAngle*pi/180.0))*gravity);} else std::cerr << "WARNING : Chute angle must be within [0,90]" << std::endl;}
	///Gets chute angle (in radians)
	///\todo Thomas: This should return the angle in degrees
	Mdouble get_ChuteAngle(){return ChuteAngle;}
	Mdouble get_ChuteAngleDegrees(){return ChuteAngle*180./constants::pi;}

	///Allows radius of fixed particles to be changed
	void set_max_failed(unsigned int new_){max_failed = new_;}
	///Allows radius of fixed particles to be accessed
	unsigned int get_max_failed(){return max_failed;}
	
	///Allows radius of inflow particles to be changed
	void set_InflowParticleRadius(Mdouble new_){
		if (new_>=0.0) {MinInflowParticleRadius=MaxInflowParticleRadius=new_;} else std::cerr << "WARNING : Inflow particle must be greater than or equal to zero" << std::endl;
	}
	///Allows radius of inflow particles to be set to a range of values
	void set_InflowParticleRadius(Mdouble new_min, Mdouble new_max){
		if (new_min>=0.0)     {MinInflowParticleRadius=new_min;} else std::cerr << "WARNING : Min. inflow particle radius must be nonnegative" << std::endl;
		if (new_max>=new_min) {MaxInflowParticleRadius=new_max;} else std::cerr << "WARNING : Max. inflow particle radius must be >= min. inflow particle radius" << std::endl;
	}
	void set_MinInflowParticleRadius(Mdouble new_min){
		if (new_min<=MaxInflowParticleRadius) {MinInflowParticleRadius=new_min;} else std::cerr << "WARNING : Max. inflow particle radius must be >= min. inflow particle radius" << std::endl;
	}
	void set_MaxInflowParticleRadius(Mdouble new_max){
		if (new_max>=MinInflowParticleRadius) {MaxInflowParticleRadius=new_max;} else std::cerr << "WARNING : Max. inflow particle radius must be >= min. inflow particle radius" << std::endl;
	}
	///Allows radius of inflow particles to be accessed
	Mdouble get_InflowParticleRadius(){return 0.5*(MinInflowParticleRadius+MaxInflowParticleRadius);}
	///Allows radius of inflow particles to be accessed
	Mdouble get_MinInflowParticleRadius(){return MinInflowParticleRadius;}
	///Allows radius of inflow particles to be accessed
	Mdouble get_MaxInflowParticleRadius(){return MaxInflowParticleRadius;}

	///Changes inflow height
	void set_InflowHeight(Mdouble new_){
		if (new_ >= MinInflowParticleRadius+MaxInflowParticleRadius)  { InflowHeight=new_; set_zmax(1.2*InflowHeight); } else std::cerr << "WARNING : Inflow height not changed to " << new_ << ", value must be greater than or equal to diameter of inflow particle" << std::endl;
	}
	///Accesses inflow height
	Mdouble get_InflowHeight(){return InflowHeight;}
	
	///Changes inflow velocity
	void set_InflowVelocity(Mdouble new_){
		if (new_ >= 0.0) InflowVelocity=new_; else std::cerr << "WARNING : Inflow velocity not changed, value must be greater than or equal to zero" << std::endl;
	}
	///Accesses inflow velocity
	Mdouble get_InflowVelocity(){return InflowVelocity;}

	///Changes inflow velocity variance
	void set_InflowVelocityVariance(Mdouble new_){
		if (new_>=0.0&&new_<=1.0) InflowVelocityVariance=new_; else std::cerr << "WARNING : Inflow velocity variance not changed, value must be within [0,1]" << std::endl;
	}
	///Accesses inflow velocity variance
	Mdouble get_InflowVelocityVariance(){return InflowVelocityVariance;}

	void set_InitialHeight(Mdouble new_){
		std::cerr << "WARNING : set_InitialHeight(Mdouble) is a deprecated function, use set_InflowHeight(Mdouble) instead." << std::endl;
		set_InflowHeight(new_);
	}
	Mdouble get_InitialHeight(){
		std::cerr << "WARNING : get_InitialHeight(Mdouble) is a deprecated function, use get_InflowHeight(Mdouble) instead." << std::endl;
		return get_InflowHeight();
	}
	void set_InitialVelocity(Mdouble new_){
		std::cerr << "WARNING : set_InitialVelocity(Mdouble) is a deprecated function, use set_InflowVelocity(Mdouble) instead." << std::endl;
		set_InflowVelocity(new_);
	}
	Mdouble get_InitialVelocity(){
		std::cerr << "WARNING : get_InitialVelocity(Mdouble) is a deprecated function, use get_InflowVelocity(Mdouble) instead." << std::endl;
		return get_InflowVelocity();
	}
	void set_InitialVelocityVariance(Mdouble new_){
		std::cerr << "WARNING : set_InitialVelocityVariance(Mdouble) is a deprecated function, use set_InflowVelocityVariance(Mdouble) instead." << std::endl;
		set_InflowVelocityVariance(new_);
	}
	Mdouble get_InitialVelocityVariance(){
		std::cerr << "WARNING : get_InitialVelocityVariance(Mdouble) is a deprecated function, use get_InflowVelocityVariance(Mdouble) instead." << std::endl;
		return get_InflowVelocityVariance();
	}
	
	///Access function that set the width of the chute
	void set_ChuteWidth(Mdouble new_){set_ymax(new_);}
	Mdouble get_ChuteWidth(){return get_ymax();}
	virtual void set_ChuteLength(Mdouble new_){set_xmax(new_);}
	Mdouble get_ChuteLength(){return get_xmax();}

	int readNextArgument(unsigned int& i, unsigned int argc, char *argv[]);

	///Sets k, disp such that it matches a given tc and eps for a collision of two inflow particles
	void set_collision_time_and_restitution_coefficient(Mdouble tc, Mdouble eps);
	
	///Calculates collision time of two inflow particles
	Mdouble get_collision_time();
	
	///Calculates restitution coefficient of two inflow particles
	Mdouble get_restitution_coefficient();

	///Sets dt to 1/50-th of the collision time for two particles of mass P
	void set_dt(){std::cout<<"Chute set_dt"<<std::endl;set_dt(.02 * get_collision_time());}
	///Sets dt
	void set_dt(Mdouble dt){MD::set_dt(dt);}
	
	///Returns the smallest particle (by mass) in the system
 	BaseParticle* getSmallestParticle();
  	
 	BaseParticle* getLargestParticle();
  	
  	BaseParticle* get_P0(){return &P0;}
	
	///Returns the radius of the smallest particle
	Mdouble get_SmallestParticleInteractionRadius();

	Mdouble get_LightestParticleMass();

protected:
	
	///here, CheckObjects is called; returns true is the particle should be added
	virtual bool IsInsertable(BaseParticle &P);
	
	///adds particle to hgrid
	void add_particle(BaseParticle &P);
	
	void actions_before_time_step();

	///Here we define the inflow
	virtual void add_particles();
	
	///Here we define the outflow
	void clean_chute();
	
	///Sets initial values for particles that are created at the inflow
	void initialize_inflow_particle();
	
	///Sets variable values for particles that are created at the inflow
	virtual void create_inflow_particle();
	
	///Create the bottom of chute out of particles
	virtual void create_bottom();
	
	void cout_time();


	Mdouble get_LargestParticleInteractionRadius(); 

protected:
	
	Mdouble FixedParticleRadius; //<radius of the fixed particles at the bottom
	int RandomizedBottom; //<distinguishes between grid-like (0), one-layer randomized (1), and thick random dense (2) bottom
	Mdouble ChuteAngle; //<chute angle in degrees

	Mdouble MinInflowParticleRadius; //<minimal radius of inflowing particles
	Mdouble MaxInflowParticleRadius; //<maximal radius of inflowing particles
	Mdouble InflowVelocity; //<Average inflow velocity in x-direction
	Mdouble InflowVelocityVariance; //Inflow velocity variance in x-direction
	Mdouble InflowHeight; //<Height of inflow
	int max_failed;//indicates how many attempts are undertake to insert a new particle before each timestep
	int num_created;//internal variable; increases by one for each particle inserted
	/// \todo Do we always want to insert this particle?
	TangentialSpringParticle P0; //<standard particle used for the inflow

private:

	bool is_periodic;
		
};




#endif
