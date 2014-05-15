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

//#define FOLLOWPARTICLE
//#define FPID 1

#ifndef MD_H
#define MD_H

//This is a header with some extra standard maths function and constants that are required by the code
#include "ExtendedMath.h"

//The vector class contains a 3D vector class.
#include "Vector.h"
#include <string.h>
//This class defines the particle handler
#include "ParticleHandler.h"
#include "Particles/TangentialSpringParticle.h"
#include "Particles/DeltaMaxsParticle.h"
//This class defines the wall handler
#include "WallHandler.h"
#include "BaseWall.h"
#include "InfiniteWall.h"
//This class defines the boundary handler
#include "BoundaryHandler.h"
#include "PeriodicBoundary.h"
//This class defines the species
#include "CSpecies.h"

#ifdef ContactListHgrid
	//This class defines the posibleContact lists
	#include "PossibleContactList.h"
#endif

//This is the class that defines the std_save routines
#include "STD_save.h"
//#include "time.h"
//#include "CMakeDefinitions.hpp"

//This is only used to change the file permission of the xball script create, at some point this code may be moved from this file to a different file.
#include <sys/types.h>
#include <sys/stat.h>

#include "RNG.h"

/// A class that defines and solves a MD problem
/// \bug When restarting the first time step is not saved, therefore there is a missing time step after a restart
class MD : public STD_save {
public:
	///A public constructor, which sets defaults so the problem can be solved off the shelf
	void constructor();
	
	MD()
	{
		constructor();
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "MD() finished"<<std::endl;
		#endif
	}
	
	MD(STD_save& other) : STD_save(other) {
		constructor();
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "MD(STD_save& other) finished " << std::endl;
		#endif		
	}
	
	///\todo write a destructor
	virtual ~MD() {};
	
	void info(){print(std::cout);}
	
	///The work horse of the code
	void solve();

	///Read arguments before solving
	void solve(unsigned int argc, char *argv[]) {
		readArguments(argc, argv);
		solve();
	};

	///Tries to solve the problem using MDCLR
	void solveWithMDCLR();
	
	///Access function for the time
	Mdouble get_t(){return t;}
	///Access function for the time
	void set_t(Mdouble new_t){t=new_t;}
	
	//~ ///Allows the number of Species to be changed
	//~ void set_NSpecies(int N){
		//~ if (N>=0) {
			//~ Species.resize(N); 
			//~ for (int i=0; i<N; i++) {
				//~ Species[i].MixedSpecies.resize(i); 
				//~ std::cout << "S" << Species[i].MixedSpecies.size() << std::endl;
			//~ }
		//~ } else {std::cerr << "Error in set_NSpecies" << std::endl; exit(-1);}
	//~ }
	///Allows the number of Species to be accessed
	int get_NSpecies(){return Species.size();}
	
	///Allows the species to be copied
	std::vector<CSpecies>& get_Species(void){return Species;}
	///Allows the species to be accessed
	CSpecies* get_Species(int i) {return &Species[i];}
	
	///Allows the mixed species to be accessed
	CSpecies* get_MixedSpecies(int i, int j) {
		if (i>j) return &Species[i].MixedSpecies[j];
		else return &Species[j].MixedSpecies[i];
	};
	///Allows the mixed species to be set
	void set_MixedSpecies(int i, int j, CSpecies& S) {
		if (i>j) Species[i].MixedSpecies[j] = S;
		else Species[j].MixedSpecies[i] = S;
	};
		
	///Allows the upper time limit to be changed
	void set_tmax(Mdouble new_tmax){if (new_tmax>=0){tmax=new_tmax;} else { std::cerr << "Error in set_tmax, new_tmax="<<new_tmax << std::endl; exit(-1); }}
	///Allows the upper time limit to be accessed
	Mdouble get_tmax(){return tmax;}
	

	ParticleHandler& getParticleHandler() { return particleHandler;}
	WallHandler& getWallHandler() { return wallHandler;}
	BoundaryHandler& getBoundaryHandler() { return boundaryHandler;}
#ifdef ContactListHgrid
    PossibleContactList& getPossibleContactList() { return possibleContactList;}
#endif
	
	///Allows the number of time steps between saves to be changed, see also \ref set_number_of_saves
	void set_savecount       (int new_){if (new_>0) {set_save_count_all(new_);}  else {std::cerr << "Error in set_savecount (set_savecount("<<new_<<"))"<< std::endl; exit(-1);}}
	void set_save_count_all  (int new_){if (new_>0) {save_count_data =new_;save_count_ene  =new_;save_count_stat =new_;save_count_fstat=new_;} else {std::cerr << "Error in set_save_count_all (set_save_count_all("<<new_<<"))"<< std::endl; exit(-1);}}
	void set_save_count_data (int new_){if (new_>0) {save_count_data =new_;} else {std::cerr << "Error in set_save_count_data, (set_save_count_data ("<<new_<<"))"<< std::endl; exit(-1);}}
	void set_save_count_ene  (int new_){if (new_>0) {save_count_ene  =new_;} else {std::cerr << "Error in set_save_count_ene, (set_save_count_ene  ("<<new_<<"))"<< std::endl; exit(-1);}}
	void set_save_count_stat (int new_){if (new_>0) {save_count_stat =new_;} else {std::cerr << "Error in set_save_count_stat, (set_save_count_stat ("<<new_<<"))"<< std::endl; exit(-1);}}
	void set_save_count_fstat(int new_){if (new_>0) {save_count_fstat=new_;} else {std::cerr << "Error in set_save_count_fstat, (set_save_count_fstat("<<new_<<"))"<< std::endl; exit(-1);}}
	///Allows the number of time steps between saves to be accessed
	int get_savecount  (){return get_save_count_data ();}
	int get_save_count (){return get_save_count_data ();}
	int get_save_count_data (){return save_count_data;}
	int get_save_count_ene  (){return save_count_ene;}
	int get_save_count_stat (){return save_count_stat;}
	int get_save_count_fstat(){return save_count_fstat;}
	
	/// Sets how often the data is saved using the number of saves wanted, tmax, and dt. See also \ref set_savecount
	void set_do_stat_always (bool new_) {do_stat_always=new_;}
	void set_number_of_saves      (Mdouble N){set_number_of_saves_all(N);}
	void set_number_of_saves_all  (Mdouble N){if (dt) {set_save_count_all((N>1)?((int)ceil(tmax/dt/(N-1.0))):1000000);} else {std::cerr << "Error in set_number_of_saves_all ; dt must be set"; exit(-1);} }
	void set_number_of_saves_data (Mdouble N){if (dt) {save_count_data  = (N>1)?((int)ceil(tmax/dt/(N-1.0))):1000000; } else {std::cerr << "Error in set_number_of_saves_data ; dt must be set"; exit(-1);} }
	void set_number_of_saves_ene  (Mdouble N){if (dt) {save_count_ene   = (N>1)?((int)ceil(tmax/dt/(N-1.0))):1000000; } else {std::cerr << "Error in set_number_of_saves_ene  ; dt must be set"; exit(-1);} }
	void set_number_of_saves_stat (Mdouble N){if (dt) {save_count_stat  = (N>1)?((int)ceil(tmax/dt/(N-1.0))):1000000; } else {std::cerr << "Error in set_number_of_saves_stat ; dt must be set"; exit(-1);} }
	void set_number_of_saves_fstat(Mdouble N){if (dt) {save_count_fstat = (N>1)?((int)ceil(tmax/dt/(N-1.0))):1000000; } else {std::cerr << "Error in set_number_of_saves_fstat; dt must be set"; exit(-1);} }
	
	///\todo{these functions should also update the mixed species}

	///Allows the plastic constants to be changed
	void set_plastic_k1_k2max_kc_depth(Mdouble k1_, Mdouble k2max_, Mdouble kc_, Mdouble depth_, unsigned int indSpecies = 0)
	{if (indSpecies<Species.size()) {Species[indSpecies].set_plastic_k1_k2max_kc_depth(k1_, k2max_, kc_, depth_);} else {std::cerr << "Error in set_k: species does not exist"; exit(-1);} }		
	void set_k1(Mdouble new_, unsigned int indSpecies = 0) 
	{if (indSpecies<Species.size()) {Species[indSpecies].set_k1(new_);} else {std::cerr << "Error in set_k: species does not exist"; exit(-1);} }
	void set_k2max(Mdouble new_, unsigned int indSpecies = 0) 
	{if (indSpecies<Species.size()) {Species[indSpecies].set_k2max(new_);} else {std::cerr << "Error in set_k: species does not exist"; exit(-1);} }
	void set_kc(Mdouble new_, unsigned int indSpecies = 0) 
	{if (indSpecies<Species.size()) {Species[indSpecies].set_kc(new_);} else {std::cerr << "Error in set_k: species does not exist"; exit(-1);} }
	void set_depth(Mdouble new_, unsigned int indSpecies = 0) 
	{if (indSpecies<Species.size()) {Species[indSpecies].set_depth(new_);} else {std::cerr << "Error in set_k: species does not exist"; exit(-1);} }
	///Allows the plastic constants to be accessed
	Mdouble get_k1(unsigned int indSpecies = 0) 
	{return Species[indSpecies].get_k1();}
	Mdouble get_k2max(unsigned int indSpecies = 0) 
	{return Species[indSpecies].get_k2max();}
	Mdouble get_kc(unsigned int indSpecies = 0) 
	{return Species[indSpecies].get_kc();}
	Mdouble get_depth(unsigned int indSpecies = 0) 
	{return Species[indSpecies].get_depth();}
	Mdouble get_plastic_dt(Mdouble mass, unsigned int indSpecies = 0)
	{return Species[indSpecies].get_plastic_dt(mass);}
	
	///Allows the spring constant to be changed
	void set_k(Mdouble new_, unsigned int indSpecies = 0){if (indSpecies<Species.size()) {Species[indSpecies].set_k(new_);} else {std::cerr << "Error in set_k: species does not exist"; exit(-1);} }
	///Allows the spring constant to be accessed
	Mdouble get_k(int indSpecies = 0){return Species[indSpecies].get_k();}
	///Allows the spring constant to be changed
	void set_kt(Mdouble new_, unsigned int indSpecies = 0){if (indSpecies<Species.size()) {Species[indSpecies].set_kt(new_);} else {std::cerr << "Error in set_kt: species does not exist"; exit(-1);}}
	///Allows the spring constant to be accessed
	Mdouble get_kt(int indSpecies = 0){return Species[indSpecies].get_kt();}
	///Allows the spring constant to be changed
	void set_krolling(Mdouble new_, unsigned int indSpecies = 0){if (indSpecies<Species.size()) {Species[indSpecies].set_krolling(new_);} else {std::cerr << "Error in set_krolling: species does not exist"; exit(-1);}}
	///Allows the spring constant to be accessed
	Mdouble get_krolling(int indSpecies = 0){return Species[indSpecies].get_krolling();}
	///Allows the spring constant to be changed
	void set_ktorsion(Mdouble new_, unsigned int indSpecies = 0){if (indSpecies<Species.size()) {Species[indSpecies].set_ktorsion(new_);} else {std::cerr << "Error in set_ktorsion: species does not exist"; exit(-1);}}
	///Allows the spring constant to be accessed
	Mdouble get_ktorsion(int indSpecies = 0){return Species[indSpecies].get_ktorsion();}
	///Allows the density to be changed
	void set_rho(Mdouble new_, unsigned int indSpecies = 0){if (indSpecies<Species.size()) {Species[indSpecies].set_rho(new_);} else {std::cerr << "Error in set_rho: species does not exist"; exit(-1);}}
	///Allows the density to be accessed
	Mdouble get_rho(int indSpecies = 0){return Species[indSpecies].get_rho();}
	///Allows the tangential viscosity to be changed
	void set_dispt(Mdouble new_, unsigned int indSpecies = 0){if (indSpecies<Species.size()) {Species[indSpecies].set_dispt(new_);} else {std::cerr << "Error in set_dispt: species does not exist"; exit(-1);}}
	///Allows the tangential viscosity to be accessed
	Mdouble get_dispt(unsigned int indSpecies = 0){return Species[indSpecies].get_dispt();}
	///Allows the tangential viscosity to be changed
	void set_disprolling(Mdouble new_, unsigned int indSpecies = 0){if (indSpecies<Species.size()) {Species[indSpecies].set_disprolling(new_);} else {std::cerr << "Error in set_disprolling: species does not exist"; exit(-1);}}
	///Allows the tangential viscosity to be accessed
	Mdouble get_disprolling(unsigned int indSpecies = 0){return Species[indSpecies].get_disprolling();}
	///Allows the tangential viscosity to be changed
	void set_disptorsion(Mdouble new_, unsigned int indSpecies = 0){if (indSpecies<Species.size()) {Species[indSpecies].set_disptorsion(new_);} else {std::cerr << "Error in set_disptorsion: species does not exist"; exit(-1);}}
	///Allows the tangential viscosity to be accessed
	Mdouble get_disptorsion(unsigned int indSpecies = 0){return Species[indSpecies].get_disptorsion();}

	///Allows the normal dissipation to be changed
	void set_disp(Mdouble new_, unsigned int indSpecies = 0){if (indSpecies<Species.size()) {Species[indSpecies].set_dissipation(new_);} else {std::cerr << "Error in set_dissipation: species does not exist"; exit(-1);}}
	///Allows the normal dissipation to be accessed
	Mdouble get_disp(unsigned int indSpecies = 0){return Species[indSpecies].get_dissipation();}
	///Allows the normal dissipation to be changed
	void set_dissipation(Mdouble new_, unsigned int indSpecies = 0){if (indSpecies<Species.size()) {Species[indSpecies].set_dissipation(new_);} else {std::cerr << "Error in set_dissipation: species does not exist"; exit(-1);}}
	///Allows the normal dissipation to be accessed
	Mdouble get_dissipation(unsigned int indSpecies = 0){return Species[indSpecies].get_dissipation();}
	///Allows the Coulomb friction coefficient to be changed 
	void set_mu(Mdouble new_, unsigned int indSpecies = 0){if (indSpecies<Species.size()) {Species[indSpecies].set_mu(new_);} else {std::cerr << "Error in set_mu: species does not exist"; exit(-1);}}
	///Allows the Coulomb friction coefficient to be accessed
	Mdouble get_mu(unsigned int indSpecies = 0){return Species[indSpecies].get_mu();}
	///Allows the Coulomb friction coefficient to be changed 
	void set_murolling(Mdouble new_, unsigned int indSpecies = 0){if (indSpecies<Species.size()) {Species[indSpecies].set_murolling(new_);} else {std::cerr << "Error in set_murolling: species does not exist"; exit(-1);}}
	///Allows the Coulomb friction coefficient to be accessed
	Mdouble get_murolling(unsigned int indSpecies = 0){return Species[indSpecies].get_murolling();}
	///Allows the Coulomb friction coefficient to be changed 
	void set_mutorsion(Mdouble new_, unsigned int indSpecies = 0){if (indSpecies<Species.size()) {Species[indSpecies].set_mutorsion(new_);} else {std::cerr << "Error in set_mutorsion: species does not exist"; exit(-1);}}
	///Allows the Coulomb friction coefficient to be accessed
	Mdouble get_mutorsion(unsigned int indSpecies = 0){return Species[indSpecies].get_mutorsion();}

	void set_rotation(bool new_){rotation=new_;}
	bool get_rotation(){return rotation;}

	///Allows the dimension of the particle (f.e. for mass) to be changed
	void set_dim_particle(int new_, unsigned int indSpecies = 0){if (indSpecies<Species.size()) {Species[indSpecies].set_dim_particle(new_);} else {std::cerr << "Error in set_dim_particle: species does not exist"; exit(-1);}}
	///Allows the dimension of the particle (f.e. for mass) to be accessed
	int get_dim_particle(unsigned int indSpecies = 0){return Species[indSpecies].get_dim_particle();}
	///Returns the data counter
	bool get_save_data_data(){return save_data_data;}
	bool get_save_data_ene(){return save_data_ene;}
	bool get_save_data_fstat(){return save_data_fstat;}
	bool get_save_data_stat(){return save_data_stat;}
	bool get_do_stat_always(){return do_stat_always;}

	///Sets k, disp such that it matches a given tc and eps for a collision of two copies of P
	void set_k_and_restitution_coefficient(Mdouble k_, Mdouble eps, Mdouble mass, unsigned int indSpecies = 0){
		if (indSpecies<Species.size()) {Species[indSpecies].set_k_and_restitution_coefficient(k_, eps, mass);} else {std::cerr << "Error in set_k_and_restitution_coefficient: species does not exist"; exit(-1);}
	}
	///Sets k, disp such that it matches a given tc and eps for a collision of two copies of P
	void set_collision_time_and_restitution_coefficient(Mdouble tc, Mdouble eps, Mdouble mass, unsigned int indSpecies = 0){
		if (indSpecies<Species.size()) {Species[indSpecies].set_collision_time_and_restitution_coefficient(tc, eps, mass);} else {std::cerr << "Error in set_collision_time_and_restitution_coefficient: species does not exist"; exit(-1);}
	}
	
	///Set k, disp such that is matches a given tc and eps for a collision of two different masses. 
	///Recall the resitution constant is a function of k, disp and the mass of each particle in the collision
	/// See also set_collision_time_and_restitution_coefficient(Mdouble tc, Mdouble eps, Mdouble mass)
	void set_collision_time_and_restitution_coefficient(Mdouble tc, Mdouble eps, Mdouble mass1, Mdouble mass2,unsigned int indSpecies =0)
		{
				if (indSpecies<Species.size()) {Species[indSpecies].set_collision_time_and_restitution_coefficient(tc, eps, mass1, mass2);} else {std::cerr << "Error in set_collision_time_and_restitution_coefficient: species does not exist"; exit(-1);}
		}

	/// See CSpecies::set_collision_time_and_normal_and_tangential_restitution_coefficient
	void set_collision_time_and_normal_and_tangential_restitution_coefficient(Mdouble tc, Mdouble eps, Mdouble beta, Mdouble mass1, Mdouble mass2,unsigned int indSpecies =0) {
		if (indSpecies<Species.size()) {Species[indSpecies].set_collision_time_and_normal_and_tangential_restitution_coefficient(tc, eps, beta, mass1, mass2);} else {std::cerr << "Error in set_collision_time_and_restitution_coefficient: species does not exist"; exit(-1);}
	}

	/// See CSpecies::set_collision_time_and_normal_and_tangential_restitution_coefficient
	void set_collision_time_and_normal_and_tangential_restitution_coefficient_nodispt(Mdouble tc, Mdouble eps, Mdouble beta, Mdouble mass1, Mdouble mass2, unsigned int indSpecies =0) {
		if (indSpecies<Species.size()) {Species[indSpecies].set_collision_time_and_normal_and_tangential_restitution_coefficient_nodispt(tc, eps, beta, mass1, mass2);} else {std::cerr << "Error in set_collision_time_and_restitution_coefficient: species does not exist"; exit(-1);}

	}

	///Calculates collision time for two copies of a particle of given disp, k, mass
	Mdouble get_collision_time(Mdouble mass, unsigned int indSpecies = 0){return Species[indSpecies].get_collision_time(mass);}
	///Calculates restitution coefficient for two copies of given disp, k, mass
	Mdouble get_restitution_coefficient(Mdouble mass, unsigned int indSpecies = 0){return Species[indSpecies].get_restitution_coefficient(mass);}
	
	///Get xmin
	Mdouble get_xmin(){return xmin;}
	///Get xmax
	Mdouble get_xmax(){return xmax;}
	///Gets ymin
	Mdouble get_ymin(){return ymin;}	
	///Gets ymax
	Mdouble get_ymax(){return ymax;}
	///Gets zmin
	Mdouble get_zmin(){return zmin;}	
	///Gets zmax
	Mdouble get_zmax(){return zmax;}
	
	///Sets xmin and walls, assuming the standard definition of walls as in the default constructor
	void set_xmin(Mdouble new_xmin){if (new_xmin<=xmax){xmin=new_xmin;} else { std::cerr << "Warning in set_xmin(" << new_xmin << "): xmax=" << xmax << std::endl; }}

	void set_ymin(Mdouble new_ymin){if (new_ymin<=ymax){ymin=new_ymin;} else { std::cerr << "Error in set_ymin(" << new_ymin << "): ymax=" << ymax << std::endl; exit(-1); }}

	///Sets ymin and walls, assuming the standard definition of walls as in the default constructor

	void set_zmin(Mdouble new_zmin){if (new_zmin<=zmax){zmin=new_zmin;} else { std::cerr << "Warning in set_zmin(" << new_zmin << "): zmax=" << zmax << std::endl; }}

	///Sets xmax and walls, assuming the standard definition of walls as in the default constructor

	void set_xmax(Mdouble new_xmax){if (new_xmax>=xmin){xmax=new_xmax;} else { std::cerr << "Error in set_xmax(" << new_xmax << "): xmin=" << xmin << std::endl; exit(-1); }}
	///Sets ymax and walls, assuming the standard definition of walls as in the default constructor

	void set_ymax(Mdouble new_ymax){if (new_ymax>ymin){ymax=new_ymax;} else { std::cerr << "Warning in set_ymax(" << new_ymax << "): ymin=" << ymin << std::endl; }}
	///Sets ymax and walls, assuming the standard definition of walls as in the default constructor

	void set_zmax(Mdouble new_zmax){if (new_zmax>ymin){zmax=new_zmax;} else { std::cerr << "Error in set_zmax(" << new_zmax << "): zmin=" << zmin << std::endl; exit(-1); }}
	
	///Allows the time step dt to be changed
	void set_dt(Mdouble new_dt){if (dt>=0.0){dt=new_dt;} else { std::cerr << "Error in set_dt" << std::endl; exit(-1); }}
	///Allows the time step dt to be accessed
	Mdouble get_dt(){return dt;}
	
	///Sets the name of the problem, used for the same data files
	void set_name(const char* name){problem_name.str(""); problem_name << name;}
	
	///Set the xball output mode
	void set_xballs_colour_mode(int new_cmode){xballs_cmode=new_cmode;}
	void set_xballs_cmode(int new_cmode){xballs_cmode=new_cmode;}
	int get_xballs_cmode(){return xballs_cmode;}
	
	///Set the scale of vectors in xballs
	void set_xballs_vector_scale(double new_vscale){xballs_vscale=new_vscale;}
	double get_xballs_vscale(){return xballs_vscale;}
	
	///Set the additional arguments for xballs
	void set_xballs_additional_arguments(std::string new_){xballs_additional_arguments=new_.c_str();}
	std::string get_xballs_additional_arguments(){return xballs_additional_arguments;}
	
	///Set the scale of the xballs problem. The default is fit to screen
	void set_xballs_scale(Mdouble new_scale){xballs_scale=new_scale;}
	double get_xballs_scale(){return xballs_scale;}
	
	///Allows the gravitational acceleration to be changed
	void set_gravity(Vec3D new_gravity){gravity = new_gravity;}
	///Allows the gravitational acceleration to be accessed
	Vec3D get_gravity(){return gravity;}
	
	///Allows the dimension of the simulation to be changed
	void set_dim(int new_dim){ if (new_dim>=1 && new_dim<=3) dim = new_dim; else { std::cerr << "Error in set_dim" << std::endl; exit(-1); }}
	///Allows the dimension of the simulation to be accessed
	int get_dim(){return dim;}
	
	///Gets restart_version
	int get_restart_version(){return restart_version;}
	///Sets restart_version
	void set_restart_version(int new_){restart_version=new_;}
	
	///Gets restarted
	bool get_restarted(){return restarted;}
	///Sets restarted

	///Get maximum radius
	Mdouble get_max_radius(){return max_radius;}
	
	void set_restarted(bool new_){
		restarted=new_;
		set_append(new_);
	}

	///Gets restarted
	bool get_append(){return append;}
	///Sets restarted
	void set_append(bool new_){append=new_;}


	///Gets ene_ela
	Mdouble get_ene_ela(){return ene_ela;}
	///Sets ene_ela
	void set_ene_ela(Mdouble new_){ene_ela=new_;}
	///Sets ene_ela
	void add_ene_ela(Mdouble new_){ene_ela+=new_;}

	///This function removes partice IP from the vector of particles by moving the last particle in the vector to the position if IP Also it checks if the moved Particle has any tangentialsspring-information, which needs to be moved to a different particle, because tangential spring information always needs to be stored in the real particle with highest particle index
	void Remove_Particle(int IP);
	
	////////////////////////////////////////////////////////////////////////////////////////
	// Some statistical output that might be of interest when setting particles' parameters
	
	Mdouble get_Mass_from_Radius(Mdouble radius, int indSpecies=0) {
		BaseParticle P;
		P.set_Radius(radius);
		P.set_IndSpecies(indSpecies);
		P.compute_particle_mass(Species);
		return P.get_Mass();
	}
	
	///Calculates the maximum velocity allowed for a collision of two copies of P (for higher velocities particles could pass through each other)
	Mdouble get_maximum_velocity(BaseParticle &P){return Species[P.get_IndSpecies()].get_maximum_velocity(P.get_Radius(),P.get_Mass());}
	
	virtual BaseParticle* getSmallestParticle() {return particleHandler.getSmallestParticle();}
	virtual BaseParticle* getLargestParticle() {return particleHandler.getLargestParticle();}
	virtual void removeParticle(int iP)
	{
#ifdef ContactListHgrid
		possibleContactList.remove_ParticlePosibleContacts(particleHandler.getObject(iP));
#endif
		HGRID_RemoveParticleFromHgrid(particleHandler.getObject(iP));
		particleHandler.removeObject(iP);
	}
	
	///\todo this function is dangerous since it uses particle data, which the user might not intend
	///Calculates the maximum velocity allowed for a collision of two copies of the smallest particle (for higher velocities particles could pass through each other)
	Mdouble get_maximum_velocity(){
		BaseParticle *P = getSmallestParticle();
		return P->get_Radius() * sqrt(Species[P->get_IndSpecies()].k/(.5*P->get_Mass()));
	}
	
	///Sets dt to 1/50-th of the collision time for two particles of mass P
	void set_dt_by_mass(Mdouble mass){dt = .02 * get_collision_time(mass);}
	
	///Sets dt to 1/50-th of the collision time for two copies of P
	void set_dt(BaseParticle &P){
		Mdouble mass = P.get_Mass(); 
		dt = .02 * get_collision_time(mass);
		if (Species[P.get_IndSpecies()].dispt) { dt = std::min(dt,0.125*mass/Species[P.get_IndSpecies()].dispt); std::cerr << "Warning: dispt is large, dt had to be lowered"; }
	}
	
	///Sets dt to 1/50-th of the smallest possible collision time
	void set_dt(){
		///\todo should setup_... be used here or not (by Thomas)
		///\todo{Is it necesarry to reset initial conditions here and in solve? (i.e. should it be in constructor)?}
		setup_particles_initial_conditions();
		for (unsigned int i=0;i<particleHandler.getNumberOfObjects();i++) particleHandler.getObject(i)->compute_particle_mass(Species);
		set_dt(getSmallestParticle()[0]);
	}
	
	///This function allows the initial conditions of the particles to be set, by default locations is random. Remember particle properties must also be defined here.
	///\todo I (Anthony) wants to change this to be an external function. This has a lot of advantages espcially when using copy-constructors. This is a major change and will break other codes, so therefore has to be done carefully.
	virtual void setup_particles_initial_conditions();
	
	///This creates a scipt which can be used to load the xballs problem to display the data just generated
	virtual void create_xballs_script();

	///Allows the user to set what is written into the info column in the data file. By default is store the Species ID number
	virtual double getInfo(BaseParticle& P) {return P.get_Species();}
	
	///Stores all MD data
	virtual void save_restart_data ();
	
	///Loads all MD data
	int load_restart_data ();
	int load_restart_data (std::string filename);
	
	///Loads all MD data and plots statistics for all timesteps in the .data file
	void statistics_from_restart_data(const char* name);
	
	///Writes all MD data
	virtual void write(std::ostream& os);
	///Reads all MD data
	virtual void read(std::istream& is);
	///Writes all MD data
	virtual void write_v1(std::ostream& os);
	///Reads all MD data
	virtual void read_v1(std::istream& is);
	virtual void read_v2(std::istream& is);
	
	///This allows particle data to be reloaded from data files
	bool load_from_data_file (const char* filename, unsigned int format=0);
	///allows the user to read par.ini files (useful to read MDCLR files)
	bool load_par_ini_file (const char* filename);
	bool read_next_from_data_file (unsigned int format=0); 
	int read_dim_from_data_file ();
	bool find_next_data_file (Mdouble tmin, bool verbose=true);

	
	///Outputs MD

	virtual void print(std::ostream& os, bool print_all=false);
	
	friend inline std::ostream& operator<<(std::ostream& os, MD &md) {
		md.print(os);
		return os;
	}
	
	void add_Species(CSpecies& S);
	
	void add_Species(void) { add_Species(Species[0]); }
		
	void set_format(int new_) {format = new_;}
	int get_format() {return format;}
	
	int readArguments(unsigned int argc, char *argv[]);

	virtual int readNextArgument(unsigned int& i, unsigned int argc, char *argv[]);
	
	//This is a random generator, often used by the initial conditions etc...
	RNG random;

//functions that should only be used in the class definitions 
protected:
	
	///This does the force computation
	virtual void compute_all_forces();

	///Computes the forces between particles (internal in the sence that the sum over all these forces is zero i.e. fully modelled forces)
	virtual void compute_internal_forces(BaseParticle* i);

    CTangentialSpring* getTangentialSpring(BaseParticle* PI, BaseParticle* PJ, BaseParticle* PJreal);
    CTangentialSpring* getTangentialSpringWall(BaseParticle* pI, int w);

	///Computes the forces between particles (internal in the sence that the sum over all these forces is zero i.e. fully modelled forces)
	virtual void compute_internal_forces(BaseParticle* P1, BaseParticle* P2);
	
	///Computes plastic forces between particles
	void compute_plastic_internal_forces(BaseParticle* P1, BaseParticle* P2);
	
	///This is were the computation of external forces takes place (e.g. gravity)
	virtual void compute_external_forces(BaseParticle* PI);
	
	///This is were the walls are
	virtual void compute_walls(BaseParticle* PI);
	
    Mdouble computeShortRangeForceWithWall(BaseParticle* pI, int wall, CSpecies* pSpecies, Mdouble dist);
    Mdouble computeShortRangeForceWithParticle(BaseParticle* PI, BaseParticle* PJ, BaseParticle* PJreal, CSpecies* pSpecies, Mdouble dist);
    
	///This is actions before the start of the main time loop
	virtual void actions_before_time_loop(){
		///todo thomas: can this be erased?
		//automatically sets dt if dt is not specified by the user
		if (!dt) set_dt(); 
	};
	
	///This is actions before the start of the main time loop
	virtual void HGRID_actions_before_time_loop(){};
	
	///This is action before the time step is started
	virtual void HGRID_actions_before_time_step(){};
	
	///This is action before the time step is started
	virtual void HGRID_InsertParticleToHgrid(BaseParticle *obj UNUSED){};
	virtual void HGRID_UpdateParticleInHgrid(BaseParticle *obj UNUSED){};
	virtual void HGRID_RemoveParticleFromHgrid(BaseParticle *obj UNUSED){};
	virtual bool get_HGRID_UpdateEachTimeStep(){return true;};
	
	///This is action before the time step is started
	virtual void actions_before_time_step(){};
	
	///This is actions at the end of the code, but before the files are closed
	virtual void actions_after_solve(){};

	///This is action after the time step is finished
	virtual void actions_after_time_step(){};	
	
	///Output xball data for Particle i
	virtual void output_xballs_data();
	virtual void output_xballs_data_particle(int i);
	
	///Functions for ene file
	virtual void start_ene();
	virtual void fstat_header();
	virtual void output_ene();
	
	///Functions for statistics
	virtual void initialize_statistics(){};
	virtual void output_statistics(){};
	virtual void gather_statistics_collision(int index1 UNUSED,int index2 UNUSED, Vec3D Contact UNUSED, Mdouble delta UNUSED, Mdouble ctheta UNUSED, Mdouble fdotn UNUSED, Mdouble fdott UNUSED, Vec3D P1_P2_normal_ UNUSED, Vec3D P1_P2_tangential UNUSED){};
	virtual void process_statistics(bool usethese UNUSED){};
	virtual void finish_statistics(){};
	virtual void set_initial_pressures_for_pressure_controlled_walls(){};	
	
	///This is were the integration is done
	virtual void do_integration_before_force_computation(BaseParticle* pI);

	virtual void checkInteractionWithBoundaries();	
	
	virtual void HGRID_update_move(BaseParticle*, Mdouble){};
	virtual void HGRID_actions_before_integration(){};
	virtual void HGRID_actions_after_integration(){};
	
	///This is were the integration is done
	virtual void do_integration_after_force_computation(BaseParticle* pI);
	
	///Initialisation of Broad Phase Information (Default no Broad Phase so empty)
	virtual void InitBroadPhase(){};
	
	///Broad phase of contact detection goes here. Default check all contacts.
	virtual void broad_phase(BaseParticle* i)
	{
		for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); (*it)!=i; ++it)
		{
			compute_internal_forces(i,*it);
		}
	}
	
	void set_FixedParticles(unsigned int n){for (unsigned int i=0; i<std::min((unsigned int)particleHandler.getNumberOfObjects(),n); i++) particleHandler.getObject(i)->fixParticle();}
	
	void initialize_tangential_springs();
		
	///Computes the mass of each particle
	void compute_particle_masses() {for (unsigned int i=0;i<particleHandler.getNumberOfObjects();i++) particleHandler.getObject(i)->compute_particle_mass(Species);}
	
	///std::couts time
	virtual void cout_time() {
		std::cout << "\rt=" << std::setprecision(3) << std::left << std::setw(6) << t 
			<< ", tmax=" << std::setprecision(3) << std::left << std::setw(6) << tmax << "        \r";
		std::cout.flush();
        #ifdef FOLLOWPARTICLE
            std::cout<<std::endl;
        #endif
	}
	
	virtual bool continue_solve() {return true;}
	
	///sets the history parameter DeltaMax of all particles to zero
	void reset_DeltaMax()
    {
        for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it!=particleHandler.end(); ++it)
        {
            DeltaMaxsParticle* DMParticle=dynamic_cast<DeltaMaxsParticle*>(*it);
            if(DMParticle)
                DMParticle->get_DeltaMaxs().resize(0);
        }
    }

	///sets the history parameter TangentialSprings of all particles to zero
	void reset_TangentialSprings()
    {
        for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it!=particleHandler.end(); ++it)
        {
            TangentialSpringParticle* TSParticle=dynamic_cast<TangentialSpringParticle*>(*it);
            if(TSParticle)
                TSParticle->get_TangentialSprings().resize(0);
        }
    }

	///These are the particle parameters like dissipation etc.
	std::vector<CSpecies> Species;

private:

	///The dimension of the simulation
	int dim;

	/// Gravitational acceleration
	Vec3D gravity; 
	
	Mdouble max_radius;
	
	///These store the size of the domain, assume walls at the ends
	Mdouble xmin,xmax,ymin,ymax,zmin,zmax;
	
	///These are numerical constants like the time step size
	Mdouble dt;
	Mdouble tmax;
	
	///These are informations for saving
	int save_count_data; //<determines how many time steps are skipped before the next data file is written
	int save_count_ene;  //<determines how many time steps are skipped before the next ene file is written
	int save_count_stat; //<determines how many time steps are skipped before the next fstat file is written
	int save_count_fstat;//<determines how many time steps are skipped before the next stat file is written
	bool save_data_data;//<determines if the current timestep is written into the data file
	bool save_data_ene;//<determines if the current timestep is written into the ene file
	bool save_data_fstat;//<determines if the current timestep is written into the fstat file
	bool save_data_stat;//<determines if the current timestep is written into the stat file
	bool do_stat_always;
	

	/// This stores the current time
	Mdouble t;

	/// used in force calculations
	Mdouble ene_ela; //<stores the potential energy in the elastic springs (global, because it has to be calculated in the force loop

	
	//This is the private data that is only used by the xballs output

	int xballs_cmode; //< sets the xballs argument cmode (see xballs.txt)
	Mdouble xballs_vscale; //< sets the xballs argument vscale (see xballs.txt)
	Mdouble xballs_scale; //< sets the xballs argument scale (see xballs.txt)
	std::string xballs_additional_arguments; //< std::string where additional xballs argument can be specified (see xballs.txt)
	int format; //< sets the data file argument format (i.e. how many columns the data file has)
	int restart_version; //<to read new and old restart data
	bool restarted; //<to read new and old restart data
	int data_FixedParticles; //<determines how many particles are fixed when reading in data files

	
private:

	ParticleHandler particleHandler;
	WallHandler wallHandler;
	BoundaryHandler boundaryHandler;
#ifdef ContactListHgrid
    PossibleContactList possibleContactList;
#endif
	
	///Remove periodic duplicate Particles (i.e. removes particles created by Check_and_Duplicate_Periodic_Particle(int i, int nWallPeriodic)). Note that between these two functions it is not allowed to create additional functions
	void Remove_Duplicate_Periodic_Particles();
	///Calls Check_and_Duplicate_Periodic_Particle for all Particles curently in Particles[] and returns the number of particles created
	int Check_and_Duplicate_Periodic_Particles();


	bool append;
	bool rotation;
	int PeriodicCreated;
	int save_restart_data_counter;
};

#endif
