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

//#define DEBUG_OUTPUT
//#define DEBUG_OUTPUT_FULL

#include "MD.h"

#include <iostream>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <limits>

//This is only used to change the file permission of the xball script create, at some point this code may be moved from this file to a different file.
#include <sys/types.h>
#include <sys/stat.h>

//This is part of this class and just separates out the stuff to do with xballs.
///todo{to incorporate changes in icc a make clean is required. Why?}
#include "MD_xballs.icc"
#include "RNG.cc"

////////////////////////////////////////////////////////////////////////////////////////////////////
///This is where the constructor is defines setup a basic two dimensional problem which can be solved off the shelf///
////////////////////////////////////////////////////////////////////////////////////////////////////
void MD::constructor()
{
	Species.resize(1);
	set_restarted(false);

	//This sets the maximum number of particles
	particleHandler.setSpecies(&Species);
	particleHandler.setStorageCapacity(2);
	dim = 2;
	format = 0;
	
	//These are the particle parameters like dissipation etc..
	set_k(1e4);
	set_rho(2000);
	//\todo{Why shouldn't we set dim_particle (which defines the mass calculations) ALWAYS to three, even for 2D problems?}
	set_dim_particle(2); 
	
	// Set gravitational acceleration
	gravity = Vec3D(0.0, -9.8, 0.0);
	
	//This is the parameter of the numerical part
	dt=0e-5; // if dt is not user-specified, this is set in actions_before_time_loop()
	set_save_count_all(20);
	set_do_stat_always(false);
	tmax=1.0;
	
	//This sets the default xballs domain
	xmin=0.0;
	xmax=0.01;
	ymin=0.0;
	ymax=0.01;
	zmin=0.0;
	zmax=0.0;

	//This set the default position of walls
	/*Wall w0;
	w0.set(Vec3D(-1,0,0), -xmin);
	wallHandler.copyAndAddWall(w0);
	w0.set(Vec3D( 1,0,0),  xmax);
	wallHandler.copyAndAddWall(w0);
	w0.set(Vec3D(0,-1,0), -ymin);
	wallHandler.copyAndAddWall(w0);
	w0.set(Vec3D(0, 1,0),  ymax);
	wallHandler.copyAndAddWall(w0);*/
	
	problem_name << "Problem_1";
	data_FixedParticles = 0;
	
	//Default mode is energy with no scale of the vectors
	xballs_cmode=0;
	xballs_vscale=-1;
	xballs_scale=-1;
	xballs_additional_arguments = "";
	set_append(false);
	
	save_restart_data_counter = 0;

	PeriodicCreated=0;

	//The default random seed is 0
	random.set_RandomSeed(0);
	#ifdef DEBUG_OUTPUT
		std::cerr << "MD problem constructor finished " << std::endl;
	#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///This setup the particles initial conditions it is virtual as you expect the user to override this.
///By default the particles are randomly disibuted///
/////////////////////////////////////////////////////////////////////////////////////////////////////
void MD::setup_particles_initial_conditions()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///This function gathers statistical data
///////////////////////////////////////////////////////////////////////////////////////////////////
void MD::start_ene()
{
	//only write if we don't restart
	if (get_append()||!ene_file.is_open()) return;
	
	///todo{Why is there a +6 here?}
	static int width = ene_file.precision() + 6;
	ene_file 
	<< std::setw(width) << "t" << " "
    << std::setw(width) << "ene_gra" << " "
    << std::setw(width) << "ene_kin" << " "
    << std::setw(width) << "ene_rot" << " "
    << std::setw(width) << "ene_ela" << " "
    << std::setw(width) << "X_COM" << " "
	<< std::setw(width) << "Y_COM" << " "
	<< std::setw(width) << "Z_COM" << std::endl;
}

void MD::fstat_header()
{
	// line #1: time, volume fraction
	// line #2: wall box: wx0, wy0, wz0, wx1, wy1, wz1
	// line #3: radii-min-max & moments: rad_min, rad_max, r1, r2, r3, r4
	fstat_file << "#" 
		<< " " << get_t()
		<< " " << 0 
		<< std::endl;
	fstat_file << "#" 
		<< " " << get_xmin() 
		<< " " << get_ymin() 
		<< " " << get_zmin() 
		<< " " << get_xmax()
		<< " " << get_ymax() 
		<< " " << get_zmax()
		<< std::endl;
	fstat_file << "#" 
		<< " " << getSmallestParticle()->get_Radius()
		<< " " << getLargestParticle()->get_Radius()
		<< " " << 0 
		<< " " << 0 
		<< " " << 0 
		<< " " << 0 
		<< std::endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///This function outputs statistical data - The default is to compute the rotational kinetic engergy, linear kinetic energy, and the centre of mass.
///////////////////////////////////////////////////////////////////////////////////////////////////
void MD::output_ene()
{
	Mdouble ene_kin = 0, ene_rot = 0, ene_gra = 0, mass_sum= 0, x_masslength=0, y_masslength=0, z_masslength=0;

	for (std::vector<BaseParticle*>::iterator it = getParticleHandler().begin(); it!=getParticleHandler().end(); ++it) if (!(*it)->isFixed())
	{
		ene_kin += .5 * (*it)->get_Mass() * (*it)->get_Velocity().GetLength2();
		ene_rot += .5 * (*it)->get_Inertia() * (*it)->get_AngularVelocity().GetLength2();
		ene_gra -= (*it)->get_Mass() * Dot(get_gravity(),(*it)->get_Position());
		mass_sum+= (*it)->get_Mass();
		x_masslength +=(*it)->get_Mass()*(*it)->get_Position().X;
		y_masslength +=(*it)->get_Mass()*(*it)->get_Position().Y;
		z_masslength +=(*it)->get_Mass()*(*it)->get_Position().Z;
	} //end for loop over Particles

	///todo{Why is there a +6 here?}
	static int width = ene_file.precision() + 6;
	ene_file  << std::setw(width) << get_t() 
		<< " " << std::setw(width) << ene_gra
		<< " " << std::setw(width) << ene_kin
		<< " " << std::setw(width) << ene_rot
		<< " " << std::setw(width) << get_ene_ela()
		<< " " << std::setw(width) << (mass_sum?x_masslength/mass_sum:std::numeric_limits<double>::quiet_NaN())
		<< " " << std::setw(width) << (mass_sum?y_masslength/mass_sum:std::numeric_limits<double>::quiet_NaN()) 
		<< " " << std::setw(width) << (mass_sum?z_masslength/mass_sum:std::numeric_limits<double>::quiet_NaN())
		<< std::endl;
	
	set_ene_ela(0);
	//sliding = sticking = 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///This function outputs the location and velocity of the particle in a format the xballs progream can read  
///////////////////////////////////////////////////////////////////////////////////////////////////
void MD::output_xballs_data()
{
	//Set the correct formation based of dimension if the formation is not specified by the user
	if (format ==0) {
			switch (dim) {
				case 1:
				case 2:
					format=8;
					break;
				case 3:
					format=14;
					break;
			}
		}

	// This outputs the location of walls and how many particles there are to file this is required by the xballs plotting
	if (format!=14) // dim = 1 or 2
		data_file << particleHandler.getNumberOfObjects() << " " <<t << " " 
		<< xmin << " " << ymin << " " 
		<< xmax << " " << ymax << " " << std::endl;
	else //dim==3
		data_file << particleHandler.getNumberOfObjects() << " " <<t << " " 
		<< xmin << " " << ymin << " " << zmin << " "
		<< xmax << " " << ymax << " " << zmax << " " << std::endl;
				// This outputs the particle data
	for (unsigned int i = 0;i<particleHandler.getNumberOfObjects();i++) 
		output_xballs_data_particle(i);
	#ifdef DEBUG_OUTPUT
		std::cerr << "Have output the properties of the problem to disk " << std::endl;
	#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///This function loads data from the .data file 
/// i.e. you get position, mass and velocity information only
/// See also MD::load_restart_data
/// Can read in format 14 - 8 or format 7 data
///This code saves in format 8 for 2D and format 14 for 3D so if no extra parameters are specified it will assume this
///note: many parameters, like density cannot be set using the data file
///////////////////////////////////////////////////////////////////////////////////////////////////
///\todo change from deprecated const char* to std::string
bool MD::load_from_data_file (const char* filename, unsigned int format) 
{
	//This opens the file the data will be recalled from
	data_file.open( filename, std::fstream::in);
	if (!data_file.is_open()||data_file.bad()) {
		std::cout << "Loading data file " << filename << " failed" << std::endl;
		return false;
	} 

	//options_data is ignored
	int options_data = get_options_data();
	set_options_data(1);
	read_next_from_data_file(format);
	set_options_data(options_data);
	
	data_file.close();

	//std::cout << "Loaded data file " << filename << " (t=" << get_t() << ")" << std::endl;
	return true;
}

bool MD::load_par_ini_file (const char* filename) 
{
	//Opens the par.ini file
	std::fstream file;
	file.open( filename, std::fstream::in);
	if (!file.is_open()||file.bad()) {
		//std::cout << "Loading par.ini file " << filename << " failed" << std::endl;
		return false;
	} 

	Mdouble doubleValue;
	int integerValue;

	// inputfile par.ini 
	// line 1 =============================================================
	// Example: 1 1 0
	//   1: integer (0|1) switches from non-periodic to periodic
	//      integer (5|6) does 2D integration only (y-coordinates fixed)
	//                    and switches from non-periodic to periodic
	//      integer (11) uses a quarter system with circular b.c.
	file >> integerValue;
	//~ std::cout << "11" << integerValue << std::endl;
	if (integerValue==0) 
	{
	    InfiniteWall w0;
	    w0.set(Vec3D(-1,0,0), -get_xmin());
	    wallHandler.copyAndAddWall(w0);
	    w0.set(Vec3D( 1,0,0),  get_xmax());
	    wallHandler.copyAndAddWall(w0);
	    w0.set(Vec3D( 0,-1,0), -get_ymin());
	    wallHandler.copyAndAddWall(w0);
	    w0.set(Vec3D(0, 1,0),  get_ymax());
	    wallHandler.copyAndAddWall(w0);
	    w0.set(Vec3D(0,0,-1), -get_zmin());
	    wallHandler.copyAndAddWall(w0);
	    w0.set(Vec3D(0,0,1), get_zmax());
	    wallHandler.copyAndAddWall(w0);
	} 
	else if (integerValue==1) 
	{
	    PeriodicBoundary b0;
	    b0.set(Vec3D(1,0,0), get_xmin(), get_xmax());
	    boundaryHandler.copyAndAddObject(b0);
	    b0.set(Vec3D(0,1,0), get_ymin(), get_ymax());
	    boundaryHandler.copyAndAddObject(b0);
	    b0.set(Vec3D(0,0,1), get_zmin(), get_zmax());
	    boundaryHandler.copyAndAddObject(b0);
	}
	else if (integerValue==5) 
	{
	    InfiniteWall w0;
	    w0.set(Vec3D(-1,0,0), -get_xmin());
	    wallHandler.copyAndAddWall(w0);
	    w0.set(Vec3D( 1,0,0),  get_xmax());
	    wallHandler.copyAndAddWall(w0);
	    w0.set(Vec3D( 0,-1,0), -get_ymin());
	    wallHandler.copyAndAddWall(w0);
	    w0.set(Vec3D(0, 1,0),  get_ymax());
	    wallHandler.copyAndAddWall(w0);
	    
	    
	} 
	else if (integerValue==6) 
	{
	    PeriodicBoundary b0;
	    b0.set(Vec3D(1,0,0), get_xmin(), get_xmax());
	    boundaryHandler.copyAndAddObject(b0);
	    b0.set(Vec3D(0,1,0), get_ymin(), get_ymax());
	    boundaryHandler.copyAndAddObject(b0);
	    b0.set(Vec3D(0,0,1), get_zmin(), get_zmax());
	    boundaryHandler.copyAndAddObject(b0);
	} 
	else 
	{
		std::cerr << "Error in par.ini: line 1, value 1 is " << integerValue << std::endl;
		exit(-1);
	}
	
	//   2: integer (0|1) dont use | use the search pattern for linked cells
	file >> integerValue; //ignore
	
	//   3: real - gravity in z direction: positive points upwards
	file >> doubleValue;
	set_gravity(Vec3D(0.0,0.0,doubleValue));

	// line 2 =============================================================
	// Example: -10000 .5e-2
	//   1: time end of simulation - (negative resets start time to zero
	//                                and uses -time as end-time)	
	file >> doubleValue;
	if (doubleValue<0) set_t(0);
	set_tmax(fabs(doubleValue));
	
	//   2: time-step of simulation
	file >> doubleValue;
	set_dt(doubleValue);

	// line 3 =============================================================
	// Example: 1e-1 100
	file >> doubleValue;
	if (doubleValue>=0) {
		//   1: time-step for output on time-series protocoll file  -> "ene"
		int savecount = round(doubleValue/get_dt());
		set_savecount(savecount);

		//   2: time-step for output on film (coordinate) file      -> "c3d"
		//      (fstat-output is coupled to c3d-output time-step)
		file >> doubleValue;
		savecount = round(doubleValue/get_dt());
		set_save_count_data(savecount);
		set_save_count_fstat(savecount);
	} else {
		//  or: ---------------------------------------------------------------
		//   1: negative number is multiplied to the previous log-output-time
		//   2: requires initial log-output time
		//   3: negative number is multiplied to the previous film-output-time
		//   4: requires initial film-output time
		std::cerr << "Error in par.ini: line 3, value 1 is " << doubleValue << std::endl;
		exit(-1);
	}

	// line 4 =============================================================
	// Example: 2000 1e5 1e3 1e2
	//   1: particle density (mass=4/3*constants::pi*density*rad^3)
	file >> doubleValue;
	set_dim_particle(3);
	set_rho(doubleValue);

	//   2: linear spring constant
	file >> doubleValue;
	set_k(doubleValue);

	//   3: linear dashpot constant
	file >> doubleValue;
	set_disp(doubleValue);

	//   4: background damping dashpot constant
	file >> doubleValue;
	if (doubleValue) std::cerr << "Warning in par.ini: ignored background damping " << doubleValue << std::endl;
	
	// line 5 =============================================================
	// Example: 0 0
	//   1: growth rate:  d(radius) = xgrow * dt
	file >> doubleValue;
	if (doubleValue) std::cerr << "Warning in par.ini: ignored growth rate " << doubleValue << std::endl;

	//   2: target volume_fraction
	file >> doubleValue;
	if (doubleValue) std::cerr << "Warning in par.ini: ignored target volume_fraction " << doubleValue << std::endl;

	file.close();
	//std::cout << "Loaded par.ini file " << filename << std::endl;
	return true;
}

bool MD::find_next_data_file (Mdouble tmin, bool verbose)
{
	if (get_options_data()==2) {
		while(true) {
			increase_counter_data(std::fstream::in);
			//check if file exists and contains data
			int N;
			data_file >> N;		
			if (data_file.eof()||data_file.peek()==-1) {
				std::cout << "file " << get_data_filename() << " not found" << std::endl;
				return false;
			}
			//check if tmin condition is satisfied
			Mdouble t;
			data_file >> t;
			if (t>tmin) {
				set_file_counter(get_file_counter()-1);
				return true;
			}
			if (verbose) std::cout <<"Jumping file counter: "<<get_file_counter() << std::endl;
		}
	} else return true;
	//length = is.tellg();
	//is.seekg (0, ios::end);
}

///by default format do not pass an argument; only specify format if you have to read a special format (f.e. dim=2, but format=14 (3d format))
bool MD::read_next_from_data_file (unsigned int format) 
{
	if (get_options_data()==2) increase_counter_data(std::fstream::in);



	//Set the correct formation based of dimension if the formation is not specified by the user
	if (format ==0)
		{
			switch (dim)
			{
				case 1:
				case 2:
					format=8;
					break;
				case 3:
					format=14;
					break;
			}
		//end case
			
		}
	//end if
	
	static unsigned int N;
	static Mdouble dummy;
	
	//read first parameter and check if we reached the end of the file
	data_file >> N;
	BaseParticle* P0;
	///\todo{TW: what if mu=0, but get_Species(1).mu isn't? We have to check all mu, right?}
	///\todo{TW: also plastic particles have to be checked.}
	if(get_k2max()) {
		P0 = new DeltaMaxsParticle();
		//std::cout << "in read_next_from_data_file(): using DeltaMaxsParticle" << std::endl;
	} else if(get_mu()) {
		P0 = new TangentialSpringParticle();
		//std::cout << "in read_next_from_data_file(): using TangentialSpringParticle" << std::endl;
	} else {
		P0 = new BaseParticle();
		//std::cout << "in read_next_from_data_file(): using BaseParticle" << std::endl;
	}
	
	if(particleHandler.getNumberOfObjects()<N)
		while(particleHandler.getNumberOfObjects()<N)
			particleHandler.copyAndAddObject(P0);
	else
		while(particleHandler.getNumberOfObjects()>N)
			particleHandler.removeLastObject();
	delete P0;
	
	#ifdef DEBUG_OUTPUT
		std::cout << "Number of particles read from file "<<N << std::endl;
	#endif
		
	if (data_file.eof()||data_file.peek()==-1) return false;
	//read all other data available for the time step
	switch(format)
	{
		//This is the format that Stefan's and Vitaley's code saves in - note the axis rotation
		case 7:
		{
			data_file >> t >> xmin >> zmin >> ymin >> xmax >> zmax >> ymax;
			//std::cout << " time " << t <<  std::endl;
			Mdouble radius;
            Vec3D position,velocity;
			for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it!=particleHandler.end(); ++it) {
				data_file
					>> position.X
					>> position.Z
					>> position.Y
					>> velocity.X
					>> velocity.Z
					>> velocity.Y
					>> radius 
					>> dummy;
                    (*it)->set_Position(position);
                    (*it)->set_Velocity(velocity);
					(*it)->set_Angle(Vec3D(0.0,0.0,0.0));
					(*it)->set_AngularVelocity(Vec3D(0.0,0.0,0.0));
					(*it)->set_Radius(radius);
					(*it)->compute_particle_mass(Species);
				}
			//End the interator over all particles
			break;
		}
		//This is a 2D format
		case 8:
		{
			data_file >> t >> xmin >> ymin >> xmax >> ymax;
			zmin = 0.0;
			zmax = 0.0;
			Mdouble radius;
            Vec3D position,velocity,angle,angularVelocity;
			for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it!=particleHandler.end(); ++it) {
				data_file 
					>> position.X 
					>> position.Y 
					>> velocity.X 
					>> velocity.Y 
					>> radius 
					>> angle.Z
					>> angularVelocity.Z 
					>> dummy;
                (*it)->set_Position(position);
                (*it)->set_Velocity(velocity);
                (*it)->set_Angle(-angle);
                (*it)->set_AngularVelocity(-angularVelocity);
				(*it)->set_Radius(radius);
				(*it)->compute_particle_mass(Species);
			} //end for all particles
			break;
		}
		//This is a 3D format
		case 14:
		{	
			data_file >> t >> xmin >> ymin >> zmin >> xmax >> ymax >> zmax;
			Mdouble radius;
            Vec3D position,velocity,angle,angularVelocity;
			for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it!=particleHandler.end(); ++it) {
				data_file 
					>> position
					>> velocity
					>> radius 
					>> angle
					>> angularVelocity
					>> dummy;
                (*it)->set_Position(position);
                (*it)->set_Velocity(velocity);
                (*it)->set_Angle(angle);
                (*it)->set_AngularVelocity(angularVelocity);                    
				(*it)->set_Radius(radius);
				(*it)->compute_particle_mass(Species);
			} //end for all particles
			break;
		}
		//This is a 3D format
		case 15:
		{
			data_file >> t >> xmin >> ymin >> zmin >> xmax >> ymax >> zmax;
			Mdouble radius;
            Vec3D position,velocity,angle,angularVelocity;            
			for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it!=particleHandler.end(); ++it) {
				data_file 
					>> position
					>> velocity
					>> radius 
					>> angle
					>> angularVelocity
					>> dummy >> dummy;
                (*it)->set_Position(position);
                (*it)->set_Velocity(velocity);
                (*it)->set_Angle(angle);
                (*it)->set_AngularVelocity(angularVelocity);                    
				(*it)->set_Radius(radius);
				(*it)->compute_particle_mass(Species);
			} //end for all particles
			break;
		}	
	} //end switch statement
	
	//fix particles, if data_FixedParticles!=0
	if (data_FixedParticles) {
		//std::cout << "fix " << min(data_FixedParticles,getParticleHandler().getNumberOfObjects()) << " Particles" << std::endl;
		for (int i=0; i<std::min(data_FixedParticles,(int) particleHandler.getNumberOfObjects()); i++)
			particleHandler.getObject(i)->fixParticle();
	}
		
	//clean up tangential springs
	reset_TangentialSprings();
    
	return true;
}


int MD::read_dim_from_data_file () 
{
	//This opens the file the data will be recalled from; note that data_filename needs to be set
	std::fstream file;
	file.open( data_filename.str().c_str() , std::fstream::in);
	
	//read first line and close file
	std::string header_string;
	getline(data_file,header_string);
	std::stringstream header (std::stringstream::in | std::stringstream::out);
	header << header_string;
	file.close();
	
	//extract data from first line
	Mdouble N, t, xmin, ymin, zmin, xmax, ymax, zmax;
	header >> N >> t >> xmin >> ymin >> zmin >> xmax >> ymax >> zmax;

	//Set the correct formation based of dimension if the formation is not specified by the user
	if (header.eof()) return 2;
	else return 3;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///This function stores all MD data - See also MD::load_restart_data
///////////////////////////////////////////////////////////////////////////////////////////////////
void MD::save_restart_data () {	
	std::stringstream restart_filename;
	restart_filename.str("");
	restart_filename << problem_name.str().c_str()  << ".restart";
	
	///todo{This can probably done a lot nicer}
	if (options_restart==2) {
		restart_filename << ".";
		if (save_restart_data_counter<1000) restart_filename << "0";
		if (save_restart_data_counter<100) restart_filename << "0";
		if (save_restart_data_counter<10) restart_filename << "0";
		restart_filename << save_restart_data_counter;
		save_restart_data_counter++;
	}
	
	std::ofstream restart_data(restart_filename.str().c_str());
	restart_data.precision(8); //max. 16 for Mdouble precision
	if( restart_data.good() )
	{
		write(restart_data);
		restart_data.close();
	} else {
		std::cerr << "restart_data " << restart_filename.str() << " could not be saved" << std::endl; 
		exit(-1);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///This function loads all MD data - See also MD::save_restart_data, MD::load_from_data_file
/// This function return 1 if sucessful else it returns -1
///////////////////////////////////////////////////////////////////////////////////////////////////
int MD::load_restart_data () {
	std::stringstream restart_filename;
	restart_filename.str("");
	restart_filename << problem_name.str().c_str()  << ".restart";
	return load_restart_data(restart_filename.str());
}

int MD::load_restart_data (std::string filename) {
	std::ifstream restart_data(filename.c_str());
	if( restart_data.good() )
	{
		read(restart_data);
		restart_data.close();
		set_restarted(true);
		return(1);
	} else {
		std::cerr << "restart_data " << filename << " could not be loaded from " << filename << std::endl; 
		exit(-1);
	}
}

CTangentialSpring* MD::getTangentialSpring(BaseParticle* PI, BaseParticle* PJ, BaseParticle* PJreal)
{
    //std::cout<<"i="<<PI->get_Index()<<" j="<<PJ->get_Index()<<std::endl;
    TangentialSpringParticle* TSParticleI=dynamic_cast<TangentialSpringParticle*>(PI);
    TangentialSpringParticle* TSParticleJ=dynamic_cast<TangentialSpringParticle*>(PJ);
    
    //First check if particle I has a spring stores that is connected to particle J
    CTangentialSpring* TangentialSpring = TSParticleI->get_TangentialSprings().select_particle_spring(PJreal->get_Index(), get_t(), get_dt());
    if(TangentialSpring==NULL)
    {
        //Then check if particle J has a spring stored that is connected to particle I
        TangentialSpring = TSParticleJ->get_TangentialSprings().select_particle_spring(PI->get_Index(), get_t(), get_dt());
        if(TangentialSpring==NULL)
        {
            //A new spring has to be created, this is done in the real particle with the highest index
            if(PI->get_Index()>PJreal->get_Index())
            {
                TangentialSpring = TSParticleI->get_TangentialSprings().create_new(PJreal->get_Index(), t, dt);
                //std::cout<<"Created new spring in Particle "<<PI->get_Index()<<" between particle "<<PI->get_Index()<<" and "<<PJreal->get_Index()<<std::endl<<std::endl;
            }
            else
            {
                TangentialSpring = TSParticleJ->get_TangentialSprings().create_new(PI->get_Index(), t, dt);
                //std::cout<<"Created new spring in Particle "<<PJ->get_Index()<<" between particle "<<PI->get_Index()<<" and "<<PJreal->get_Index()<<std::endl<<std::endl;
            }
        }
        //else
        //std::cout<<"i="<<PI->get_Index()<<" j="<<PJ->get_Index()<<" Reconnected to tangential spring in j"<<std::endl;
    }
    //else
    //std::cout<<"i="<<PI->get_Index()<<" j="<<PJ->get_Index()<<" Reconnected to tangential spring in i"<<std::endl;
    return TangentialSpring;
}


CTangentialSpring* MD::getTangentialSpringWall(BaseParticle* pI, int w)
{
    TangentialSpringParticle* TSParticle=dynamic_cast<TangentialSpringParticle*>(pI);
    CTangentialSpring* TangentialSpring = TSParticle->get_TangentialSprings().select_wall_spring(w, get_t(), get_dt());

    if(TangentialSpring==NULL)
        TangentialSpring = TSParticle->get_TangentialSprings().create_new_wall(w, t, dt);

    return TangentialSpring;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
///This computer the internal forces (internal in the sence that they sum to zero) i.e. the fully modelled forces.
/////////////////////////////////////////////////////////////////////////////////////////////////////
void MD::compute_internal_forces(BaseParticle* P1, BaseParticle* P2)
{
	//this is because the rough bottom allows overlapping fixed particles
	if (P1->isFixed()&&P2->isFixed()) return;
	
	///Tangential spring information is always store in the real particle with highest index
	///When a Periodic contact is encountered it is always encoutered twice (for normal periodic walls), but the force is only applied to the real particle
	///The tangential spring information for periodic particles is stored in the normal particle (and thus twice for normal periodic interactions)
	///When a Particle is removed the tangential spring information has to be moved
    
    //Dificult cases for tangential springs in comination with periodic walls are:
    //
    // A new contact over a periodic wall:
    // Starting situation: There are no tangential springs stored at all
    // Creating periodic particles: There are no tangential springs so nothing happens
    // Contact detection: There are two contacts detected, one (CA) for P1 with P2per and one (CB) for P2 with P1per
    // Switch to the 4 cases:
    //   CA: PI=P1, PJ=P2per PJreal=P2
    //   CB: PI=P2, PJ=P1per PJreal=P1
    // Reconnecting springs: 
    //   CA: PI=P1 and PJ=P2per do not have a spring, so a new spring is created in either PI or PJ (could be in periodic particle)
    //   CB: PI=P2 and PJ=P1Per do not have a spring, so a new spring is created in either PI or PJ (could be in periodic particle)
    // Removing periodic particles: One of the springs will be stored in a periodic particle and thus removed, the other spring is kept and used (this is the real particle with the kighest index)
    
    // Reconnect to a contact over a periodic wall:
    // Starting situation: There is a tangential spring stored in the particle with the highest index, lets assume this is P1
    // Creating periodic particles: P1 has a tangential spring, so P1per has a reversed copy of this spring
    // Contact detection: There are two contacts detected, one (CA) for P1 with P2per and one (CB) for P2 with P1per
    // Switch to the 4 cases:
    //   CA: PI=P1, PJ=P2per PJreal=P2
    //   CB: PI=P2, PJ=P1per PJreal=P1
    // Reconnecting springs: 
    //   CA: PI=P1 and PJ=P2per have a spring in P1, so we reconnect to this spring in the normal way and integrate it.
    //   CB: PI=P2 and PJ=P1Per have a spring in P1per, however this spring has to be a reversed spring since it is in PJ.
    // Removing periodic particles: The spring in P1per is removed
	
	//Cases (start from P1 and P2 and go to PI and PJ
	//1 Normal-Normal		->PI=max(P1,P2), PJ=min(P1,P2)
	//2 Periodic-Normal		->(PI=P2 PJ=real(P1))
	//3 Normal-Periodic		->(PI=P1 PJ=real(P2))
	//4 Periodic-Periodic	->do nothing
	
	//Just some statements to handle the 4 cases
	BaseParticle *PI, *PJ, *PJreal;
	bool isPeriodic;
	BaseParticle *P1Per=P1->get_PeriodicFromParticle();
	BaseParticle *P2Per=P2->get_PeriodicFromParticle();
	if(P1Per==NULL)
	{
		if(P2Per==NULL)//N-N
			if(P1->get_Index()<P2->get_Index())
				{PI=P2;PJ=P1;PJreal=P1;isPeriodic=false;}
			else
				{PI=P1;PJ=P2;PJreal=P2;isPeriodic=false;}
		else //N-P
			{PI=P1;PJ=P2;PJreal=P2Per;isPeriodic=true;}
	} else {
		if(P2Per==NULL) //P-N
			{PI=P2;PJ=P1;PJreal=P1Per;isPeriodic=true;}
		else return;
	}
	
#ifdef DEBUG_OUTPUT
	std::cerr << "In computing internal forces between particle "<<PI->get_Position()<<" and "<<PJ->get_Position()<<std::endl;
#endif
	
	//Get the square of the distance between particle i and particle j
	Mdouble dist_squared=GetDistance2(PI->get_Position(), PJ->get_Position());
	Mdouble interactionRadii_sum=PI->get_InteractionRadius()+PJ->get_InteractionRadius();
	
#ifdef DEBUG_OUTPUT_FULL
	std::cerr << "Square of distance between " << dist_squared << " square sum of radii " << radii_sum*radii_sum <<std::endl;
#endif
	
	// True if the particles are in contact
	if (dist_squared<(interactionRadii_sum*interactionRadii_sum))
	{
        // For particles of the same species, set species vector to Species(PI);
		// for particles of different species, set species vector to MixedSpecies(PI,PJ)
		CSpecies* pSpecies = (PI->get_IndSpecies()==PJ->get_IndSpecies())?&Species[PI->get_IndSpecies()]:get_MixedSpecies(PI->get_IndSpecies(),PJ->get_IndSpecies());
		
		// Calculate distance between the particles
		Mdouble dist=sqrt(dist_squared);
		
		// Compute normal vector
		
		Vec3D normal=(PI->get_Position()-PJ->get_Position())/dist;
		
		// Compute the overlap between the particles
		Mdouble radii_sum=PI->get_Radius()+PJ->get_Radius();
		Mdouble deltan = std::max(0.0,radii_sum-dist);
	
		Vec3D force= Vec3D(0,0,0);;
		Vec3D forcet; forcet.set_zero();
		Vec3D forcerolling; forcerolling.set_zero();
		Vec3D forcetorsion; forcetorsion.set_zero();
		Mdouble fdotn = 0;
		CTangentialSpring* TangentialSpring = NULL;

		//evaluate shortrange non-contact forces
		if (pSpecies->AdhesionForceType!=None)
            fdotn += computeShortRangeForceWithParticle(PI, PJ, PJreal, pSpecies, dist);

		if (deltan>0) //if contact forces
		{

			// Compute the relative velocity vector v_ij
			Vec3D vrel;
			if (!pSpecies->mu) {
				vrel=(PI->get_Velocity()-PJ->get_Velocity());
			} else {
				vrel=(PI->get_Velocity()-PJ->get_Velocity()) + Cross(normal, PI->get_AngularVelocity() * (PI->get_Radius() - .5 * deltan) + PJ->get_AngularVelocity() * (PJ->get_Radius() - .5 * deltan) );
			}
			
			// Compute the projection of vrel onto the normal (can be negative)
			Mdouble vdotn=-Dot(vrel,normal);
			
			//update restitution coeff
			if (pSpecies->useRestitution) pSpecies->update_disp(PI->get_Mass(),PJ->get_Mass());
			Mdouble a=0, R=0;
			
			// Compute normal force on particle i due to contact
			if (pSpecies->get_ForceType()==HM||pSpecies->get_ForceType()==HMD) 
			{
				//R is twice the effective radius
				R = 2.0*PI->get_Radius()*PJ->get_Radius()/(PI->get_Radius()+PJ->get_Radius());
				a = sqrt(R*deltan);
				//pSpecies->k stores the elastic modulus
				Mdouble kn = 4./3. * pSpecies->k * a;
				fdotn += kn * deltan + pSpecies->disp*vdotn;
			} else {
				fdotn += pSpecies->k*deltan+pSpecies->disp*vdotn;
			}		
			force += normal * fdotn;

			//If tangential forces are present
			if (pSpecies->mu || pSpecies->murolling || pSpecies->mutorsion) { 
				//call tangential spring
				if (pSpecies->kt || pSpecies->krolling || pSpecies->ktorsion) 
                    TangentialSpring = getTangentialSpring(PI, PJ, PJreal);
				
				//Compute norm of normal force
				Mdouble norm_fn = fabs(fdotn);
				
				//calculate sliding friction
				if (pSpecies->mu) {
					//Compute the tangential component of vrel
					Vec3D vrelt=vrel+normal*vdotn;
					//Compute norm of vrelt
					Mdouble vdott=vrelt.GetLength();
					
					if (pSpecies->kt) {
						Vec3D* delta = &(TangentialSpring->delta);
						
						//Integrate the spring
						///Both options are up to first order the same (the first one is nicer becaus it always keeps the spring tangential, whereas the second one is in a nicer intergration form)
						//(*delta) += vrelt * dt - Dot(*delta,normal)*normal;
						Vec3D ddelta = (vrelt - Dot(*delta,PI->get_Velocity()-PJ->get_Velocity())*normal/dist) * get_dt();
						(*delta) += ddelta;
						
						//Calculate test force including viscous force
						if (pSpecies->get_ForceType()==HM) {
							//pSpecies->kt stores the elastic shear modulus
							Mdouble kt = 8. * pSpecies->kt * a;
							TangentialSpring->SlidingForce += - kt * ddelta;
							forcet = TangentialSpring->SlidingForce - pSpecies->dispt * vrelt;
						} else if (pSpecies->get_ForceType()==HMD) {
							//pSpecies->kt stores the elastic shear modulus
							forcet = TangentialSpring->SlidingForce - pSpecies->dispt * vrelt;
							Mdouble kt = 8. * pSpecies->kt * a * pow(1- GetLength(forcet)/pSpecies->mu/fdotn,0.33);
							TangentialSpring->SlidingForce += - kt * ddelta;
							forcet = TangentialSpring->SlidingForce - pSpecies->dispt * vrelt;
						} else {
							forcet = (-pSpecies->dispt) * vrelt - pSpecies->kt * (*delta);
						}

						Mdouble forcet2 = forcet.GetLength2();
						
						//tangential forces are modelled by a spring-damper of elastisity kt and viscosity dispt (sticking), 
						//but the force is limited by Coulomb friction (sliding):
						//f_t = -dispt*vrelt, if dispt*vrelt<=mu_s*fdotn, f_t=mu+s*fdotn*t, else
						double muact=(TangentialSpring->sliding)?(pSpecies->mu):(pSpecies->mus); // select mu from previous sliding mode
						if( forcet2 <= sqr(muact*norm_fn) ) { 
							//sticking++;
							TangentialSpring->sliding=false;
						} else { 
							//sliding++;
							TangentialSpring->sliding=true;
							///\todo{The spring should be cut back such that fdott=mu*fdotn. This is simple for dispt=0; we have to think about what happens in the sliding case with tang. dissipation; same for walls; Update Dec-2-2011: fixed}
							Mdouble norm_forcet = sqrt(forcet2);
							forcet *= pSpecies->mu * norm_fn / norm_forcet;
							TangentialSpring->SlidingForce = forcet + pSpecies->dispt * vrelt;
							(*delta) = TangentialSpring->SlidingForce/(-pSpecies->kt);
						}
						
						//Add tangential force to total force
						force += forcet;
						
					} else { //if no tangential spring
						//tangential forces are modelled by a damper of viscosity dispt (sticking), 
						//but the force is limited by Coulomb friction (sliding):
						//f_t = -dispt*vrelt, if dispt*vrelt<=mu_s*fdotn, f_t=mu+s*fdotn*t, else
						if (vdott*pSpecies->dispt <= pSpecies->mus*norm_fn) { //sticking++;
							forcet = -pSpecies->dispt * vrelt; 
						} else { //sliding++;
							//set force to Coulomb limit
							forcet = -(pSpecies->mu * norm_fn / vdott) * vrelt;
						}				
						//Add tangential force to total force
						force += forcet;
					}
				}
				
				//calculate rolling friction
				if (pSpecies->murolling) {
					//From Luding2008, objective rolling velocity (eq 15) w/o 2.0!
					Mdouble reducedRadiusI = PI->get_Radius() - .5 * deltan;
					Mdouble reducedRadiusJ = PJ->get_Radius() - .5 * deltan;
					Mdouble reducedRadiusIJ = 2.0*reducedRadiusI*reducedRadiusJ/(reducedRadiusI+reducedRadiusJ);
					Vec3D vrolling=-reducedRadiusIJ	* Cross(normal, PI->get_AngularVelocity() - PJ->get_AngularVelocity());
					if (pSpecies->krolling) {
						Vec3D* RollingSpring = &(TangentialSpring->RollingSpring);
						
						//Integrate the spring
						(*RollingSpring) += vrolling * get_dt();// - Dot(*RollingSpring,normal)*normal;
						
						//Calculate test force including viscous force
						forcerolling = (-pSpecies->disprolling) * vrolling - pSpecies->krolling * (*RollingSpring);
						Mdouble forcerolling2 = forcerolling.GetLength2();
						
						//tangential forces are modelled by a spring-damper of elastisity kt and viscosity dispt (sticking), 
						//but the force is limited by Coulomb friction (sliding):
						//f_t = -dispt*vrelt, if dispt*vrelt<=mu_s*fdotn, f_t=mu+s*fdotn*t, else
						double muact=(TangentialSpring->slidingRolling)?(pSpecies->murolling):(pSpecies->musrolling); // select mu from previous sliding mode
						if( forcerolling2 <= sqr(muact*norm_fn) ) { 
							//sticking++;
							TangentialSpring->slidingRolling=false;
						} else { 
							//sliding++;
							TangentialSpring->slidingRolling=true;
							forcerolling *= pSpecies->murolling * norm_fn / sqrt(forcerolling2);
							(*RollingSpring) = (forcerolling + pSpecies->disprolling * vrolling)/(-pSpecies->krolling);
						}
						
						//Add tangential force to torque
						Vec3D Torque = reducedRadiusIJ * Cross(normal, forcerolling);
						PI->add_Torque(Torque);
						PJreal->add_Torque(-Torque);
					}
				}

				
				//calculate torsive friction
				if (pSpecies->mutorsion) {
					//From Luding2008, spin velocity (eq 16) w/o 2.0!
					Mdouble RadiusIJ = 2.0*PI->get_Radius()*PJ->get_Radius()/(PI->get_Radius()+PJ->get_Radius());
					Vec3D vtorsion=RadiusIJ*Dot(normal,PI->get_AngularVelocity() - PJ->get_AngularVelocity())*normal;
					if (pSpecies->ktorsion) {
						//~ std::cout << "Error; not yet implemented" << std::endl;
						//~ exit(-1);
						Vec3D* TorsionSpring = &(TangentialSpring->TorsionSpring);
						
						//Integrate the spring
						(*TorsionSpring) = Dot((*TorsionSpring) + vtorsion * get_dt(),normal)*normal;
						
						//Calculate test force including viscous force
						forcetorsion = (-pSpecies->disptorsion) * vtorsion - pSpecies->ktorsion * (*TorsionSpring);
						Mdouble forcetorsion2 = forcetorsion.GetLength2();
						
						//tangential forces are modelled by a spring-damper of elastisity kt and viscosity dispt (sticking), 
						//but the force is limited by Coulomb friction (sliding):
						//f_t = -dispt*vrelt, if dispt*vrelt<=mu_s*fdotn, f_t=mu+s*fdotn*t, else
						double muact=(TangentialSpring->slidingTorsion)?(pSpecies->mutorsion):(pSpecies->mustorsion); // select mu from previous sliding mode
						if( forcetorsion2 <= sqr(muact*norm_fn) ) { 
							//sticking++;
							TangentialSpring->slidingTorsion=false;
						} else { 
							//sliding++;
							TangentialSpring->slidingTorsion=true;
							//~ std::cout << "sliding " << std::endl;
							forcetorsion *= pSpecies->mutorsion * norm_fn / sqrt(forcetorsion2);
							(*TorsionSpring) = (forcetorsion + pSpecies->disptorsion * vtorsion)/(-pSpecies->ktorsion);
						}
						
						//Add tangential force to torque
						Vec3D torque = RadiusIJ * forcetorsion;
						PI->add_Torque(torque);
						PJreal->add_Torque(-torque);
						
					}
				}			
			} //end if tangential forces
			
			///\todo TW: the following 13 lines concern only the sliding spring and could be moved into the if statement above
			//Make force Hertzian (note: deltan is normalized by the normal distanct of two particles in contact, as in Silbert)
			if (pSpecies->get_ForceType()==Hertzian) force *= sqrt(deltan/(PI->get_Radius()+PJ->get_Radius()));
		
		} else {//end if contact forces
			force += fdotn*normal;
		}

		//Add forces to total force
		PI->add_Force(force);
		if(!isPeriodic)
			PJreal->add_Force(-force);
		
		// Add torque due to tangential forces: t = Cross(l,f), l=dist*Wall.normal
		if (pSpecies->mu) {
			Vec3D cross = Cross(normal, force);
			PI    ->add_Torque(-cross * (PI->get_Radius() - .5 * deltan));
			if(!isPeriodic)
				PJreal->add_Torque(-cross * (PJ->get_Radius() - .5 * deltan));
		}
		
		// output for ene and stat files:
		if (get_save_data_ene()) {
			if(!isPeriodic)
				add_ene_ela(0.5 * (pSpecies->k * sqr(deltan) + (TangentialSpring?
															 (pSpecies->kt       * TangentialSpring->delta.GetLength2()
															 +pSpecies->krolling * TangentialSpring->RollingSpring.GetLength2()
															 +pSpecies->ktorsion * TangentialSpring->TorsionSpring.GetLength2()):0.0)));
			else
				add_ene_ela(0.25* (pSpecies->k * sqr(deltan) + (TangentialSpring?
															 (pSpecies->kt       * TangentialSpring->delta.GetLength2()
															 +pSpecies->krolling * TangentialSpring->RollingSpring.GetLength2()
															 +pSpecies->ktorsion * TangentialSpring->TorsionSpring.GetLength2()):0.0)));
		}
		if (get_save_data_fstat()||get_save_data_stat()||get_do_stat_always()) {			
			
			Mdouble fdott = forcet.GetLength();
			Mdouble deltat_norm = TangentialSpring?(-TangentialSpring->delta.GetLength()):0.0;
			
			///\todo Define the center this way or are radii involved? Or maybe just use middle of overlap region?
			///Thomas: Yes, we should correct that for polydispersed problems; however, then we also have to correct it in StatisticsVector::gather_statistics_collision.
			Vec3D centre = 0.5 * (PI->get_Position() + PJ->get_Position());
			
			///The second particle (i.e. the particle the force acts on) 
			///is always a flow particle 
			///\todo{Is it the first particle the force acts on?}
			
			if (!PI->isFixed())
			{
				if(get_save_data_stat()||get_do_stat_always()) gather_statistics_collision(PJreal->get_Index(),PI->get_Index(), centre, deltan, deltat_norm,fdotn,fdott,-normal,-(fdott?forcet/fdott:forcet));
				if(get_save_data_fstat()) fstat_file 
					<< get_t() << " "
					<< PJreal->get_Index() << " "
					<< PI->get_Index() << " "				
					<< centre << " "
					<< radii_sum-dist << " "
					<< deltat_norm << " "
					<< fdotn << " "
					<< fdott << " "
					<< -normal << " "
					<< -(fdott?forcet/fdott:forcet) << std::endl;
			}
			if (!PJreal->isFixed()&&!isPeriodic)
			{
				if(get_save_data_stat()||get_do_stat_always()) gather_statistics_collision(PI->get_Index(),PJreal->get_Index(), centre, deltan, deltat_norm,fdotn,fdott,normal,(fdott?forcet/fdott:forcet));
				if(get_save_data_fstat()) fstat_file 
					<< get_t() << " "
					<< PI->get_Index() << " "
					<< PJreal->get_Index() << " "
					<< centre << " "
					<< radii_sum-dist << " "
					<< deltat_norm << " "
					<< fdotn << " "
					<< fdott << " "
					<< normal << " "
					<< (fdott?forcet/fdott:forcet) << std::endl;
			}
		}
#ifdef FOLLOWPARTICLE
    if(PI->get_Index()==FPID||PJ->get_Index()==FPID)
        std::cout<<"Particle collission at time="<<get_t()<<" between "<<PI->get_Index()<<" and "<<PJ->get_Index()<<std::endl;
#endif
		
	} // end if particle i and j are overlapping
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
///This computer the internal forces for the plastic model
/////////////////////////////////////////////////////////////////////////////////////////////////////
void MD::compute_plastic_internal_forces(BaseParticle* P1, BaseParticle* P2)
{
	//this is because the rough bottom allows overlapping fixed particles
	if (P1->isFixed()&&P2->isFixed()) return;

	///Tangential spring information is always store in the real particle with highest index
	///When a Periodic contact is encountered it is always enstd::coutered twice, but only applied when the real particle has the highest index of both real indices
	///When a Particle is removed the tangential spring information has to be moved

	//Cases (start from P1 and P2 and go to PI and PJ
	//1 Normal-Normal		->PI=max(P1,P2), PJ=min(P1,P2)
	//2 Periodic-Normal		->if(P2>Real(P1)) (PI=P2 PJ=real(P1)) otherwise do nothing
	//3 Normal-Periodic		->if(P1>Real(P2)) (PI=P1 PJ=real(P2)) otherwise do nothing
	//4 Periodic-Periodic	->do nothing
	
	//Just some statements to handle the 4 cases
	BaseParticle *PI, *PJ,*PJreal;
	BaseParticle *P1Per=P1->get_PeriodicFromParticle();
	BaseParticle *P2Per=P2->get_PeriodicFromParticle();
	if(P1Per==NULL)
	{
		if(P2Per==NULL)
			//N-N
			if(P1->get_Index()<P2->get_Index())
				{PI=P2;PJ=P1;PJreal=PJ;}
			else
				{PI=P1;PJ=P2;PJreal=PJ;}
		else
			//N-P
			if(P1->get_Index()>P2Per->get_Index())
				{PI=P1;PJ=P2;PJreal=P2Per;}
			else return;
	} else {
		if(P2Per==NULL)
			//P-N
			if(P2->get_Index()>P1Per->get_Index())
				{PI=P2;PJ=P1;PJreal=P1Per;}
			else return;
		else return;
	}

	#ifdef DEBUG_OUTPUT
		std::cerr << "In computing interal forces between particle "<<PI->get_Position()<<" and "<<PJ->get_Position()<<std::endl;
	#endif

    
	//Get the square of the distance between particle i and particle j
	Mdouble dist_squared=GetDistance2(PI->get_Position(), PJ->get_Position());
	Mdouble interactionRadii_sum=PI->get_InteractionRadius()+PJ->get_InteractionRadius();
	
    #ifdef DEBUG_OUTPUT_FULL
        std::cerr << "Square of distance between " << dist_squared << " square sum of radii " << radii_sum*radii_sum <<std::endl;
    #endif
	
	// True if the particles are in contact
	if (dist_squared<(interactionRadii_sum*interactionRadii_sum))
	{
        // For particles of the same species, set species vector to Species(PI);
		// for particles of different species, set species vector to MixedSpecies(PI,PJ)
		CSpecies* pSpecies = (PI->get_IndSpecies()==PJ->get_IndSpecies())?&Species[PI->get_IndSpecies()]:get_MixedSpecies(PI->get_IndSpecies(),PJ->get_IndSpecies());
		
		// Calculate distance between the particles
		Mdouble dist=sqrt(dist_squared);
		
		// Compute normal vector
		Vec3D normal=(PI->get_Position()-PJ->get_Position())/dist;
		
		// Compute the overlap between the particles
		Mdouble radii_sum=PI->get_Radius()+PJ->get_Radius();
		Mdouble deltan = std::max(0.0,radii_sum-dist);
        
		Vec3D force= Vec3D(0,0,0);;
		Vec3D forcet; forcet.set_zero();
        Vec3D deltat; deltat.set_zero();
		Mdouble fdotn = 0;
        
		//evaluate shortrange non-contact forces
		if (pSpecies->AdhesionForceType!=None)
            fdotn += computeShortRangeForceWithParticle(PI, PJ, PJreal, pSpecies, dist);
        
		if (deltan>0) //if contact forces
		{
            
            // Compute the relative velocity vector v_ij
            Vec3D vrel;
            if (!pSpecies->mu) {
                vrel=(PI->get_Velocity()-PJ->get_Velocity());
            } else {
                vrel=(PI->get_Velocity()-PJ->get_Velocity()) + Cross(normal, PI->get_AngularVelocity() * (PI->get_Radius() - .5 * deltan) + PJ->get_AngularVelocity() * (PJ->get_Radius() - .5 * deltan) );
            }
            
            ///newcode begin
            // BEGIN add viscous normal force

            // Compute the projection of vrel onto the normal (can be negative)
            Mdouble vdotn=-Dot(vrel,normal);

            // Compute normal force on particle i due to contact
            if (pSpecies->useRestitution) pSpecies->update_disp(PI->get_Mass(),PJ->get_Mass());
            fdotn += pSpecies->disp*vdotn;

            // END add viscous normal force


            // BEGIN add elastic force
            
            //calculate deltastar, the overlap above which k2max becomes active (the 'fluid branch')
            Mdouble deltastar = (pSpecies->k2max/(pSpecies->k2max-pSpecies->k))*pSpecies->depth*((2*PI->get_Radius()*PJ->get_Radius())/(PI->get_Radius()+PJ->get_Radius()));
            //2*depth*r_eff is the overlap at which fn=0 on the k2max branch
            //deltastar is the overlap at which the k1 and the k2max branch meet

            //retrieve history parameter deltamax, the max. achieved overlap
            DeltaMaxsParticle* DMParticle=dynamic_cast<DeltaMaxsParticle*>(PI);
            Mdouble *deltamax = DMParticle->get_DeltaMaxs().select_particle(PJreal->get_Index(), t, dt);
            //update deltastar
            *deltamax = std::min(deltastar,std::max(*deltamax,deltan)); 
            
            //calculate k2(deltamax) (only use first case for Walton-Braun spring)
            Mdouble k2;		
            if (deltan>deltastar) {
                k2 = pSpecies->k2max;	
            } else {
                k2 = pSpecies->k+(pSpecies->k2max-pSpecies->k)*((*deltamax)/deltastar);
            } 		
                    
            //calculate delta0(deltamax), the overlap where the force is zero
            Mdouble delta0 = (k2-pSpecies->k)/k2*(*deltamax); 
            
            //add elastic force
            //std::cout << k2*(deltan-(delta0))-pSpecies->k*deltan << std::endl;
            if (deltan>deltastar) {
                fdotn += pSpecies->k2max*(deltan-(delta0));
            } else if (k2*(deltan-(delta0))>=pSpecies->k*deltan){
                fdotn += pSpecies->k*deltan;
            } else if (k2*(deltan-delta0)>=-pSpecies->kc*deltan){
                fdotn += k2*(deltan-delta0);
            } else {
                fdotn += -pSpecies->kc*deltan;
                *deltamax=(k2+pSpecies->kc)/(k2-pSpecies->k)*deltan;
            }
            
            force = normal * fdotn;
            // END add elastic force

            
            //If tangential forces are present
            if (pSpecies->mu) {
                
                //Compute the tangential component of vrel
                Vec3D vrelt=vrel+normal*vdotn;

                //Compute norm of vrelt
                Mdouble vdott=vrelt.GetLength();

                //Compute norm of normal force
                Mdouble norm_fn = fabs(fdotn);
                
                if (!pSpecies->kt) { //if no tangential spring
                    //tangential forces are modelled by a damper of viscosity dispt (sticking), 
                    //but the force is limited by Coulomb friction (sliding):
                    //f_t = -dispt*vrelt, if dispt*vrelt<=mu_s*fdotn, f_t=mu+s*fdotn*t, else
                    if (vdott*pSpecies->dispt <= pSpecies->mus*norm_fn) { //sticking++;
                        forcet = -pSpecies->dispt * vrelt; 
                    } else { //sliding++;
                        //set force to Coulomb limit
                        forcet = -(pSpecies->mu * norm_fn / vdott) * vrelt;
                    }
                    
                } else { //if with tangential spring	
                    //Retrieve the spring
                    CTangentialSpring* TangentialSpring = getTangentialSpring(PI, PJ, PJreal);
                    Vec3D* delta = &(TangentialSpring->delta);
                    
                    //Integrate the spring
                    ///Both options are up to first order the same (the first one is nicer becaus it always keeps the spring tangential, whereas the second one is in a nicer intergration form)
                    //(*delta) += vrelt * dt - Dot(*delta,normal)*normal;
                    (*delta) += (vrelt - Dot(*delta,PI->get_Velocity()-PJ->get_Velocity())*normal/dist) * dt;
                    deltat = (*delta);
                    
                    //Calculate test force including viscous force
                    forcet = (-pSpecies->dispt) * vrelt - pSpecies->kt * deltat;
                    Mdouble forcet2 = forcet.GetLength2();

                    //tangential forces are modelled by a spring-damper of elastisity kt and viscosity dispt (sticking), 
                    //but the force is limited by Coulomb friction (sliding):
                    //f_t = -dispt*vrelt, if dispt*vrelt<=mu_s*fdotn, f_t=mu+s*fdotn*t, else
                    if( forcet2 <= sqr(pSpecies->mus*norm_fn) ) { 
                        //sticking++;
                    } else { 
                        //sliding++;
                        Mdouble norm_forcet = sqrt(forcet2);
                        forcet *= pSpecies->mu * norm_fn / norm_forcet;
                        ///\todo{The spring should be cut back such that fdott=mu*fdotn. This is simple for dispt=0; we have to think about what happens in the sliding case with tang. dissipation; same for walls}
                        (*delta) = -(forcet + pSpecies->dispt * vrelt)/pSpecies->kt;
                        //This line should be removed before release it is the old tangential model (the new is shown to be better).
                        //(*delta) = forcet/(-pSpecies->kt);
                    }
                } //end if tangential spring
                
                //Add tangential force to total force
                force += forcet;

            } //end if tangential forces
		} else {//end if contact forces
			force = fdotn*normal;
		}
        
		//Make force Hertzian (note: deltan is normalized by the normal distanct of two particles in contact, as in Silbert)
		if (pSpecies->get_ForceType()==Hertzian) force *= sqrt(deltan/(PI->get_Radius()+PJ->get_Radius()));
		
		PI    ->add_Force( force);
		PJreal->add_Force(-force);
				
		// Add torque due to tangential forces: t = Cross(l,f), l=dist*Wall.normal
		if (pSpecies->mu) {
			Vec3D cross = -Cross(normal, force);
			PI    ->add_Torque(cross * (PI->get_Radius() - .5 * deltan));
			PJreal->add_Torque(cross * (PI->get_Radius() - .5 * deltan));
		}

		// output for ene and stat files:
		if (save_data_ene) {
	    	ene_ela += 0.5 * (pSpecies->k * sqr(deltan) + pSpecies->kt * deltat.GetLength2());
		}
		if (save_data_fstat||save_data_stat||do_stat_always) {			
			Mdouble fdott = forcet.GetLength();
			Mdouble deltat_norm = -deltat.GetLength();
			
			Vec3D centre = 0.5 * (PI->get_Position() + PJ->get_Position());
			///The second particle (i.e. the particle the force acts on) 
			///is always a flow particle 
			
			if (!PI->isFixed())
			{
				 if(save_data_stat||do_stat_always) gather_statistics_collision(PJreal->get_Index(),PI->get_Index(), centre, deltan, deltat_norm,fdotn,fdott,-normal,-(fdott?forcet/fdott:forcet));
				 if(save_data_fstat) fstat_file 
					<< t << " "
					<< PJreal->get_Index() << " "
					<< PI->get_Index() << " "				
					<< centre << " "
					<< radii_sum-dist << " "
					<< deltat_norm << " "
					<< fdotn << " "
					<< fdott << " "
					<< -normal << " "
					<< -(fdott?forcet/fdott:forcet) << std::endl;
			}
			if (!PJreal->isFixed())
			{
				 if(save_data_stat||do_stat_always) gather_statistics_collision(PI->get_Index(),PJreal->get_Index(), centre, deltan, deltat_norm,fdotn,fdott,normal,(fdott?forcet/fdott:forcet));
				 if(save_data_fstat){ fstat_file 
					<< t << " "
					<< PI->get_Index() << " "
					<< PJreal->get_Index() << " "
					<< centre << " "
					<< radii_sum-dist << " "
					<< deltat_norm << " "
					<< fdotn << " "
					<< fdott << " "
					<< normal << " "
					<< (fdott?forcet/fdott:forcet) << std::endl;
				}
			}
		}
		
	} // end if particle i and j are overlapping
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///This computes the external forces e.g. here it is gravity and walls
/////////////////////////////////////////////////////////////////////////////////////////////////////
void MD::compute_external_forces(BaseParticle* CI)
{
	if (!CI->isFixed()) {
		/// Now add on gravity
		CI->add_Force(get_gravity() * CI->get_Mass());
		///Finally walls
		compute_walls(CI);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///This is were the walls are implemented - normals are outward normals
/////////////////////////////////////////////////////////////////////////////////////////////////////
void MD::compute_walls(BaseParticle* pI) 
{
	//No need to compute interactions between periodic particle images and walls
	if(pI->get_PeriodicFromParticle()!=NULL)
		return;
	
	for (unsigned int i=0; i<wallHandler.getNumberOfWalls(); i++) 
	{
		
		// note: normal points away from particle i
		Vec3D normal;
		Mdouble dist;
		bool touch = wallHandler.getWall(i)->get_distance_and_normal(*pI, dist, normal);
		
		//If the wall is being touched (I think :Ant)
		if (touch) 
		{
			
			CSpecies* pSpecies = (pI->get_IndSpecies()==wallHandler.getWall(i)->indSpecies)?&Species[pI->get_IndSpecies()]:get_MixedSpecies(pI->get_IndSpecies(),wallHandler.getWall(i)->indSpecies);
			
			Mdouble deltan = std::max(0.0,pI->get_Radius()-dist);

            Vec3D force= Vec3D(0,0,0);;
            Vec3D forcet; forcet.set_zero();
            Vec3D forcerolling; forcerolling.set_zero();
            Vec3D forcetorsion; forcetorsion.set_zero();
            Mdouble fdotn = 0;
            CTangentialSpring* TangentialSpring = NULL;

            //evaluate shortrange non-contact forces
            if (pSpecies->AdhesionForceType!=None)
                fdotn += computeShortRangeForceWithWall(pI, i, pSpecies, dist);
            
            if (deltan>0) //if contact forces
            {

            
                // Compute the relative velocity vector v_ij
                Vec3D vrel;
                if (!pSpecies->mu) {
                    vrel = pI->get_Velocity() - wallHandler.getWall(i)->get_Velocity();
                } else {
                    vrel = pI->get_Velocity() - wallHandler.getWall(i)->get_Velocity() - Cross(normal, pI->get_AngularVelocity()) * dist;
                }
                
                // Compute the projection of vrel onto the normal (can be negative)
                Mdouble vdotn = -Dot(vrel, normal);
                
                //possibly update restitution coefficient
                if (pSpecies->useRestitution) pSpecies->update_disp(pI->get_Mass(),1e20);

                // Compute normal force on particle i due to contact
                Mdouble a=0, R=0;
                if (pSpecies->get_ForceType()==HM||pSpecies->get_ForceType()==HMD) {
                    //R is twice the effective radius
                    R = pI->get_Radius();
                    a = sqrt(deltan*R);
                    //pSpecies->k stores the elastic modulus
                    Mdouble kn = 4./3. * pSpecies->k * a; //it's not really kn
                    fdotn += kn*deltan - 2*pSpecies->disp*sqrt(pI->get_Mass()*2.*pSpecies->k * a)*vdotn;
                } else {
                    fdotn += pSpecies->k*deltan - pSpecies->disp*vdotn;
                }
                force = normal * (-fdotn);
                
                //If tangential forces are present
                if (pSpecies->mu || pSpecies->murolling || pSpecies->mutorsion) {
                    //call tangential spring
                    if (pSpecies->kt || pSpecies->krolling || pSpecies->ktorsion)
                        TangentialSpring = getTangentialSpringWall(pI, i);
                    
                    //Compute norm of normal force
                    Mdouble norm_fn = fabs(fdotn);
                    
                    //calculate sliding friction
                    if (pSpecies->mu) {
                        //Compute the tangential component of vrel
                        Vec3D vrelt=vrel+normal*vdotn;
                        //Compute norm of vrelt
                        Mdouble vdott=vrelt.GetLength();
                        
                        if (pSpecies->kt) {
                            Vec3D* delta = &(TangentialSpring->delta);
                            
                            //Integrate the spring
                            (*delta) += vrelt * get_dt(); //no correction because the normal direction is constant
                            //std::cout << (vrel) << std::endl;
                            //Calculate test force including viscous force
                            if (pSpecies->get_ForceType()==HM) {
                                //pSpecies->kt stores the elastic shear modulus
                                Mdouble kt = 8. * pSpecies->kt * a;
                                //std::cout << "kt" << kt << std::endl;
                                TangentialSpring->SlidingForce += - kt * (vrelt * get_dt());
                                a = sqrt(deltan*R);
                                forcet = TangentialSpring->SlidingForce - 2*pSpecies->dispt * sqrt(pI->get_Mass()*kt) * vrelt;
                            } else if (pSpecies->get_ForceType()==HMD) {  ///\todo HMD still has to be fully implemented; HM & HMD requires thinking about how to keep it tangential; you dont need to update delta AND the force
                                //pSpecies->kt stores the elastic shear modulus
                                forcet = TangentialSpring->SlidingForce - pSpecies->dispt * vrelt;
                                Mdouble kt;
                                if (get_t()<0.034/10) {
                                    // loading
                                    kt = 8. * pSpecies->kt * a * pow(1.001- GetLength(forcet)/pSpecies->mus/fdotn,1./3.);
                                } else if (Dot(vrelt,forcet)<=0) { 
                                    //unloading in opposite direction
                                    kt = 8. * pSpecies->kt * a * pow(.501 - .5 * GetLength(forcet)/pSpecies->mus/fdotn,1./3.);
                                } else {
                                    //unloading
                                    kt = 8. * pSpecies->kt * a * pow(.501 + .5* GetLength(forcet)/pSpecies->mus/fdotn,1./3.);
                                }
                                TangentialSpring->SlidingForce += - kt * (vrelt * get_dt());
                                forcet = TangentialSpring->SlidingForce - pSpecies->dispt * vrelt;
                            } else {
                                forcet = (-pSpecies->dispt) * vrelt - pSpecies->kt * (*delta);
                            }
                            
                            Mdouble forcet2 = forcet.GetLength2();
                            
                            //tangential forces are modelled by a spring-damper of elastisity kt and viscosity dispt (sticking), 
                            //but the force is limited by Coulomb friction (sliding):
                            //f_t = -dispt*vrelt, if dispt*vrelt<=mu_s*fdotn, f_t=mu+s*fdotn*t, else
                            double muact=(TangentialSpring->sliding)?(pSpecies->mu):(pSpecies->mus); // select mu from previous sliding mode
                            if( forcet2 <= sqr(muact*norm_fn) ) { 
                                //sticking++;
                                TangentialSpring->sliding=false;
                            } else { 
                                //sliding++;
                                TangentialSpring->sliding=true;
                                Mdouble norm_forcet = sqrt(forcet2);
                                forcet *= pSpecies->mu * norm_fn / norm_forcet;
                                TangentialSpring->SlidingForce = forcet + pSpecies->dispt * vrelt;
                                (*delta) = TangentialSpring->SlidingForce/(-pSpecies->kt);
                            }
                            
                            //Add tangential force to total force
                            force += forcet;
                            
                        } else { //if no tangential spring
                            //tangential forces are modelled by a damper of viscosity dispt (sticking), 
                            //but the force is limited by Coulomb friction (sliding):
                            //f_t = -dispt*vrelt, if dispt*vrelt<=mu_s*fdotn, f_t=mu+s*fdotn*t, else
                            if (vdott*pSpecies->dispt <= pSpecies->mus*norm_fn) { //sticking++;
                                forcet = -pSpecies->dispt * vrelt; 
                            } else { //sliding++;
                                //set force to Coulomb limit
                                forcet = -(pSpecies->mu * norm_fn / vdott) * vrelt;
                                //std::cout << "sliding " << std::endl;
                            }				
                            //Add tangential force to total force
                            force += forcet;
                        }
                    }
                    
                    //calculate rolling friction
                    if (pSpecies->murolling) {
                        //From Luding2008, objective rolling velocity (eq 15) w/o 2.0!
                        Mdouble reducedRadiusI = pI->get_Radius() - .5 * deltan;
                        ///\todo: reducedRadiusIJ should have a factor of 2.0, but then the rolling frequency differs from the normal frequency when krolling=2/5*k;
                        Mdouble reducedRadiusIJ = reducedRadiusI; 
                        Vec3D vrolling=-reducedRadiusIJ	* Cross(normal, pI->get_AngularVelocity());
                        if (pSpecies->krolling) {
                            Vec3D* RollingSpring = &(TangentialSpring->RollingSpring);
                            
                            //Integrate the spring
                            (*RollingSpring) += vrolling * get_dt();// - Dot(*RollingSpring,normal)*normal;
                            
                            //Calculate test force including viscous force
                            forcerolling = (-pSpecies->disprolling) * vrolling - pSpecies->krolling * (*RollingSpring);
                            Mdouble forcerolling2 = forcerolling.GetLength2();
                            
                            //tangential forces are modelled by a spring-damper of elastisity kt and viscosity dispt (sticking), 
                            //but the force is limited by Coulomb friction (sliding):
                            //f_t = -dispt*vrelt, if dispt*vrelt<=mu_s*fdotn, f_t=mu+s*fdotn*t, else
                            double muact=(TangentialSpring->slidingRolling)?(pSpecies->murolling):(pSpecies->musrolling); // select mu from previous sliding mode
                            if( forcerolling2 <= sqr(muact*norm_fn) ) { 
                                TangentialSpring->slidingRolling=false;
                            } else { 
                                TangentialSpring->slidingRolling=true;
                                forcerolling *= pSpecies->murolling * norm_fn / sqrt(forcerolling2);
                                (*RollingSpring) = (forcerolling + pSpecies->disprolling * vrolling)/(-pSpecies->krolling);
                            }
                            
                            //Add tangential force to torque
                            Vec3D Torque = reducedRadiusIJ * Cross(normal, forcerolling);
                            pI->add_Torque(Torque);
                            wallHandler.getWall(i)->add_Torque(Torque);

                        }
                    }
                    
                    //calculate torsive friction
                    if (pSpecies->mutorsion) {
                        //From Luding2008, spin velocity (eq 16) w/o 2.0!
                        ///\todo: RadiusIJ should have a factor of 2.0, but then the rolling frequency differs from the normal frequency when krolling=2/5*k;
                        Mdouble RadiusIJ = pI->get_Radius();
                        Vec3D vtorsion=RadiusIJ*Dot(normal,pI->get_AngularVelocity())*normal;
                        if (pSpecies->ktorsion) {
                            //~ std::cout << "Error; not yet implemented" << std::endl;
                            //~ exit(-1);
                            Vec3D* TorsionSpring = &(TangentialSpring->TorsionSpring);
                            
                            //Integrate the spring
                            (*TorsionSpring) = Dot((*TorsionSpring) + vtorsion * get_dt(),normal)*normal;
                            
                            //Calculate test force including viscous force
                            forcetorsion = (-pSpecies->disptorsion) * vtorsion - pSpecies->ktorsion * (*TorsionSpring);
                            Mdouble forcetorsion2 = forcetorsion.GetLength2();
                            
                            //tangential forces are modelled by a spring-damper of elastisity kt and viscosity dispt (sticking), 
                            //but the force is limited by Coulomb friction (sliding):
                            //f_t = -dispt*vrelt, if dispt*vrelt<=mu_s*fdotn, f_t=mu+s*fdotn*t, else
                            double muact=(TangentialSpring->slidingTorsion)?(pSpecies->mutorsion):(pSpecies->mustorsion); // select mu from previous 
                            if( forcetorsion2 <= sqr(muact*norm_fn) ) { 
                                //sticking++;
                                TangentialSpring->slidingTorsion=false;
                            } else { 
                                //sliding++;
                                TangentialSpring->slidingTorsion=true;
                                // std::cout << "torsion sliding " << std::endl;
                                forcetorsion *= pSpecies->mutorsion * norm_fn / sqrt(forcetorsion2);
                                (*TorsionSpring) = (forcetorsion + pSpecies->disptorsion * vtorsion)/(-pSpecies->ktorsion);
                            }
                            
                            //Add tangential force to torque
                            Vec3D Torque = RadiusIJ * forcetorsion;
                            
                            if (pSpecies->get_ForceType()==HM||pSpecies->get_ForceType()==HMD) {
                                //R is twice the effective radius
                                R = pI->get_Radius();
                                a = sqrt(deltan*R);
                                Torque = 20 * a * forcetorsion;
                            }
                            
                            pI->add_Torque(Torque);
                            wallHandler.getWall(i)->add_Torque(Torque);
                            //std::cout << " " << Torque.Z << std::endl;
                        }
                    }			
                    
                } //end if tangential forces
			} else {//end if contact forces
                force += (-fdotn)*normal;
            }
            
			//Make force Hertzian (note: deltan is normalized by the normal distanct of two particles in contact, as in Silbert)
			if (pSpecies->get_ForceType()==Hertzian) force *= sqrt(deltan/(2.0*pI->get_Radius())); 
			
			//Add forces to total force
			pI->add_Force(force);
			wallHandler.getWall(i)->add_Force(-force);
			
			if (pSpecies->mu) pI->set_Torque(pI->get_Torque()+Cross(normal, force) * dist);
			// Add torque due to tangential forces: t = Cross(l,f), l=dist*Wall.normal
			
			if (get_save_data_ene()) {
				add_ene_ela(0.5 * (pSpecies->k * sqr(deltan) + (TangentialSpring?
																(pSpecies->kt * TangentialSpring->delta.GetLength2()
																 +pSpecies->krolling * TangentialSpring->RollingSpring.GetLength2()
																 +pSpecies->ktorsion * TangentialSpring->TorsionSpring.GetLength2()):0.0)));
			}
			if (get_save_data_fstat()||get_save_data_stat()||get_do_stat_always()) 
			{
				
                deltan = pI->get_Radius()-dist;
				Mdouble fdott = forcet.GetLength();
				Mdouble deltat_norm = TangentialSpring?(-TangentialSpring->delta.GetLength()):0.0;
				
				if (!pI->isFixed())
				{
					if(get_save_data_stat()||get_do_stat_always()) gather_statistics_collision(pI->get_Index(),-(i+1), pI->get_Position() + normal*(pI->get_Radius()-deltan), deltan, deltat_norm,fdotn,fdott,normal,-(fdott?forcet/fdott:forcet));
					if(get_save_data_fstat()) fstat_file 
						<< get_t() << " "
						<< pI->get_Index() << " "
						<< -(static_cast<int>(i)+1) << " "
						<< pI->get_Position() + normal*(pI->get_Radius()-deltan) << " "
						<< deltan << " "
						<< deltat_norm << " "
						<< fdotn << " "
						<< fdott << " "
						<< normal << " "
						<< -(fdott?forcet/fdott:forcet) << std::endl;
				}
			} 
			
		} // end if particle i touches the wall
		
	}//end if for wall i
}

Mdouble MD::computeShortRangeForceWithParticle(BaseParticle* PI, BaseParticle* PJ, BaseParticle* PJreal, CSpecies* pSpecies, Mdouble dist) {
    Mdouble fdotn;
    if (pSpecies->AdhesionForceType==LinearReversible)
    {
        fdotn = std::min(0.0,pSpecies->k0*std::max(dist-PI->get_Radius()-PJ->get_Radius(),0.0)-pSpecies->f0);
    }
    else if (pSpecies->AdhesionForceType==LinearIrreversible)
    {
        TangentialSpringParticle* TSParticleI=dynamic_cast<TangentialSpringParticle*>(PI);
        TangentialSpringParticle* TSParticleJ=dynamic_cast<TangentialSpringParticle*>(PJ);
        //First check if particle I has a spring stores that is connected to particle J
        CTangentialSpring* TangentialSpring = TSParticleI->get_TangentialSprings().select_particle_spring(PJreal->get_Index(), get_t(), get_dt());
        if(TangentialSpring==NULL)
        {
            //Then check if particle J has a spring stored that is connected to particle I
            TangentialSpring = TSParticleJ->get_TangentialSprings().select_particle_spring(PI->get_Index(), get_t(), get_dt());
        }
        if(TangentialSpring==NULL)
        {
            fdotn = (PI->get_Radius()+PJ->get_Radius()>dist)?-pSpecies->f0:0.0;
        }
        else
        {
            fdotn = std::min(0.0,pSpecies->k0*std::max(dist-PI->get_Radius()-PJ->get_Radius(),0.0)-pSpecies->f0);
        }
    }
    else
    {
        fdotn=0;
    }
    return fdotn;
}

Mdouble MD::computeShortRangeForceWithWall(BaseParticle* pI, int wall, CSpecies* pSpecies, Mdouble dist) {
    Mdouble fdotn;
    if (pSpecies->AdhesionForceType==LinearReversible)
    {
        fdotn = std::min(0.0,pSpecies->k0*std::max(dist-pI->get_Radius(),0.0)-pSpecies->f0);
    }
    else if (pSpecies->AdhesionForceType==LinearIrreversible)
    {
        TangentialSpringParticle* TSParticle=dynamic_cast<TangentialSpringParticle*>(pI);
        CTangentialSpring* TangentialSpring = TSParticle->get_TangentialSprings().select_wall_spring(wall, get_t(), get_dt());
        if(TangentialSpring==NULL)
        {
            fdotn = (pI->get_Radius()>dist)?-pSpecies->f0:0.0;
        }
        else
        {
            fdotn = std::min(0.0,pSpecies->k0*std::max(dist-pI->get_Radius(),0.0)-pSpecies->f0);
        }
    }
    else
    {
        fdotn=0;
    }
    return fdotn;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///This is were the integration is done, at the moment it is Velocity Verlet integration
/////////////////////////////////////////////////////////////////////////////////////////////////////
void MD::do_integration_before_force_computation(BaseParticle* iP)
{
#ifdef USE_SIMPLE_VERLET_INTEGRATION
	
#else //use velocity verlet
	iP->accelerate(iP->get_Force()*iP->get_InvMass()*0.5*dt);
	iP->set_Displacement(iP->get_Velocity()*dt);
	iP->move(iP->get_Displacement());
	HGRID_update_move(iP,iP->get_Displacement().GetLength());
	if (rotation)
	{	
		iP->angularAccelerate(iP->get_Torque()*iP->get_InvInertia()*0.5*dt);
		iP->rotate(iP->get_AngularVelocity()*dt);
	}
#endif
#ifdef FOLLOWPARTICLE
    if(iP->get_Index()==FPID)
        std::cout<<"In integration before time="<<get_t()<<", particle "<<FPID<<": "<<*iP<<std::endl;
#endif
}

void MD::checkInteractionWithBoundaries()
{
	for(std::vector<BaseBoundary*>::iterator B = boundaryHandler.begin(); B!=boundaryHandler.end(); ++B)
	{
		for (std::vector<BaseParticle*>::iterator P = particleHandler.begin(); P!=particleHandler.end(); ++P)
		{
			if((*B)->checkBoundaryAfterParticleMoved(*P,particleHandler)) //Returns true if the particle is deleted
				--P;
		}
	}
}

void MD::print(std::ostream& os, bool print_all) {
		os << "problem_name:" << problem_name.str().c_str() << std::endl;
		os << " t:" << t << " dt:" << dt << ", tmax:" << tmax << ", save_count_data:" << save_count_data << ", save_count_ene:" << save_count_ene  << ", save_count_stat:" << save_count_stat << ", save_count_fstat:" << save_count_fstat << std::endl;
		os << " x:[" << xmin << "," << xmax << "], y:[" << ymin << "," << ymax << "], z:[" << zmin << "," << zmax << "]" << std::endl;
		os << " dim:" << dim << ", gravity:" << gravity << std::endl;
		if (Species.size()==1) {
			os << " "; Species[0].print(os); os << std::endl;
		} else {
			os << " Species: size:" << Species.size() << std::endl;
			for (unsigned int i=0; i<Species.size(); i++) { 
				os << "  "; Species[i].print(os); os << std::endl; 
				for (unsigned int j=0; j<Species[i].MixedSpecies.size(); j++) { os << "    "; Species[i].MixedSpecies[j].print(os); os << std::endl; }
			}
		}
		os << " options_fstat=" << options_fstat 
		   << " , options_data=" << options_data 
		   << " , options_ene=" << options_ene 
		   << " , options_restart=" << options_restart << std::endl; 
		os << " Particles: N:" << particleHandler.getNumberOfObjects() << ", Nmax:" << particleHandler.getStorageCapacity() << std::endl;
		if (print_all) for (unsigned int i=0; i<particleHandler.getNumberOfObjects(); i++) { os << "  "; particleHandler.getObject(i)->print(os); os << std::endl; }
		os << " Walls: NWall:" << wallHandler.getNumberOfWalls() << ", NBoundary:" << boundaryHandler.getNumberOfObjects() << std::endl;
		for (unsigned int i=0; i<wallHandler.getNumberOfWalls(); i++) { os << "  "; wallHandler.getWall(i)->print(os); os << std::endl; }
		for (unsigned int i=0; i<boundaryHandler.getNumberOfObjects(); i++) { os << "  "; boundaryHandler.getObject(i)->print(os); os << std::endl; }
	}


void MD::add_Species(CSpecies& S) {
	Species.push_back(S);
	unsigned int n = Species.size();
	Species.back().MixedSpecies.resize(n-1);
	for (unsigned int i=0; i<n-1; i++)
		Species.back().MixedSpecies[i].mix(Species[i],Species.back());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///This is were the integration is done, at the moment it is Velocity Verlet integration
/////////////////////////////////////////////////////////////////////////////////////////////////////
void MD::do_integration_after_force_computation(BaseParticle* pI)
{
#ifdef USE_SIMPLE_VERLET_INTEGRATION

	static Vec3D xtemp, atemp;
	/// At this point x is the next time step and s is at the previous i.e there are two time steps apart,
	xtemp=pI->get_Position();
	atemp=pI->get_Force()*pI->get_InvMass();
	pI->set_Position(xtemp*2.0-pI->PrevPosition+atemp*dt*dt);
	pI->set_Velocity((pI->get_Position()-pI->PrevPosition)/(2*dt)+atemp*dt);
	pI->PrevPosition=xtemp;
	if (rotation) {	
		pI->angularAccelerate(pI->get_Torque()*pI->get_InvInertia()*dt);
		pI->rotate(pI->get_AngularVelocity()*dt);
	}
	
	// This shifts particles that moved through periodic walls
	///\todo{TW: can we do the shift in the createPeriodicParticles function? Now, each particle is checked against the periodic walls twice.}
	for (unsigned int j=0; j<boundaryHandler.getNumberOfObjects(); j++) 
	{
		PeriodicBoundary* b0=dynamic_cast<PeriodicBoundary*>(boundaryHandler.getObject(j));
		//Note, checking the dynamic cast allows you to detect if the wall is of a certain type (or inherited from this type).
		if(b0) //If the dyamic_cast succeeds 
			if (b0->distance(*iP)<0)
			{
				b0->shift_position(iP->get_Position());
				if(!get_HGRID_UpdateEachTimeStep())
				{
					HGRID_RemoveParticleFromHgrid(iP);
					HGRID_UpdateParticleInHgrid(iP);
				}
			}
	}	
	
#else //use velocity verlet
	pI->accelerate(pI->get_Force()*pI->get_InvMass()*0.5*dt);
	if (rotation) pI->angularAccelerate(pI->get_Torque()*pI->get_InvInertia()*0.5*dt);;
#endif
#ifdef FOLLOWPARTICLE
    if(pI->get_Index()==FPID)
        std::cout<<"In integration after  time="<<get_t()<<", particle "<<FPID<<": "<<*pI<<std::endl;
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///statistics_from_restart_data
/////////////////////////////////////////////////////////////////////////////////////////////////////
void MD::statistics_from_restart_data(const char* name)
{
	///todo{Check this whole function}
	//This function loads all MD data
	load_restart_data();
		
	//This creates the file statistics will be saved to
	stat_filename.str("");
	stat_filename << name  << ".stat";
	stat_file.open( stat_filename.str().c_str() , std::fstream::out);
	
	
	// Sets up the initial conditions for the simulation	
	// setup_particles_initial_conditions();
	// Setup the previous position arrays and mass of each particle.
	compute_particle_masses();
	// Other routines required to jump-start the simulation
	actions_before_time_loop();
	initialize_statistics();
	HGRID_actions_before_time_loop();
	start_ene();
	
	while (load_from_data_file(data_filename.str().c_str())) {
		HGRID_actions_before_time_loop();
		actions_before_time_step();
		PeriodicCreated=Check_and_Duplicate_Periodic_Particles();
		HGRID_actions_before_time_step();
		save_data_data = true;
		save_data_ene  = true;
		save_data_stat = true;
		save_data_fstat= true;
		compute_all_forces();
		Remove_Duplicate_Periodic_Particles();
		actions_after_time_step();
		output_ene();
		std::cout << std::setprecision(6) << t << std::endl;
	}
	
	data_file.close();
	stat_file.close();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///This does the force computation
////////////////////////////////////////////////////////////////////////////////////////////////////
void MD::compute_all_forces()
{
	///Reset all forces to zero
	for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it!=particleHandler.end(); ++it)
	{
		(*it)->set_Force(Vec3D(0.0,0.0,0.0));
		(*it)->set_Torque(Vec3D(0.0,0.0,0.0));
	} 
	for (std::vector<BaseWall*>::iterator it = wallHandler.begin(); it!=wallHandler.end(); ++it)
	{
		(*it)->set_Force(Vec3D(0.0,0.0,0.0));
		(*it)->set_Torque(Vec3D(0.0,0.0,0.0));
	} //end reset forces loop


	#ifdef DEBUG_OUTPUT
		std::cerr << "Have all forces set to zero " << std::endl;
	#endif

	///Now loop over all particles contacts computing force contributions

	for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it!=particleHandler.end(); ++it)
	{
		
		///Now loop over all other particles looking for contacts
		compute_internal_forces(*it);
		//end inner loop over contacts.
			
		compute_external_forces(*it);

	}

#ifdef ContactListHgrid		
	PossibleContact* Curr=possibleContactList.getFirstPossibleContact();
	while(Curr)
	{
		compute_internal_forces(Curr->get_P1(),Curr->get_P2());
		Curr=Curr->get_Next();
	}
#endif
	//end outer loop over contacts.

}


void MD::compute_internal_forces(BaseParticle* i) {
	broad_phase(i);
}

///Writes all MD data
void MD::write(std::ostream& os) {
	os
    << "restart_version " << 3 
    << std::endl 
    << "name " << problem_name.str() 
    << std::endl
    << "xmin " << xmin << " xmax " << xmax 
    << " ymin " << ymin << " ymax " << ymax 
    << " zmin " << zmin << " zmax " << zmax
    << std::endl
    << "dt " << dt 
    << " t " << t 
    << " tmax " << tmax 
    << " save_count_data " << save_count_data
    << " save_count_ene " << save_count_ene
    << " save_count_stat " << save_count_fstat
    << " save_count_fstat " << save_count_fstat
    << std::endl
    << "dim " << dim
    << " gravity " << gravity
    << std::endl
    << "options_fstat " << options_fstat 
    << " options_data " << options_data 
    << " options_ene " << options_ene 
    << " options_restart " << options_restart
    << std::endl; 
	os<< "Species " << Species.size() << std::endl;
	for (std::vector<CSpecies>::iterator it = Species.begin(); it!=Species.end(); ++it) {
		os << (*it) << std::endl;
		for (std::vector<CSpecies>::iterator it2 = it->MixedSpecies.begin(); it2!=it->MixedSpecies.end(); ++it2)	os << (*it2) << " (mixed)" << std::endl;
	}
	os<< "Walls " << wallHandler.getNumberOfWalls() << std::endl;
	for (std::vector<BaseWall*>::iterator it = wallHandler.begin(); it!=wallHandler.end(); ++it) os << (**it) << std::endl;
	os<< "Boundaries " << boundaryHandler.getNumberOfObjects() << std::endl;
	for (std::vector<BaseBoundary*>::iterator it = boundaryHandler.begin(); it!=boundaryHandler.end(); ++it) os << (**it) << std::endl;
	os<< "Particles " << particleHandler.getNumberOfObjects() << std::endl;
	for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it!=particleHandler.end(); ++it) os << (**it) << std::endl;
	//should be optional: os << "FixedParticles " << data_FixedParticles << std::endl;
}

///Reads all MD data
void MD::read(std::istream& is) 
{
	std::string dummy;
	is >> dummy;
	if (strcmp(dummy.c_str(),"restart_version"))
	{
		dim = atoi(dummy.c_str());
		restart_version = 1;
		read_v1(is);
	}
	else
	{
		is	>> restart_version 
			>> dummy >> dummy;
		problem_name.str(dummy);
		if(restart_version==2)
		{
			read_v2(is);
		}
		else
		{
			//std::cout << "version 3" << std::endl;
			is	>> dummy >> xmin
				>> dummy >> xmax
				>> dummy >> ymin
				>> dummy >> ymax
				>> dummy >> zmin
				>> dummy >> zmax
				>> dummy >> dt
				>> dummy >> t
				>> dummy >> tmax
				>> dummy >> save_count_data
				>> dummy >> save_count_ene
				>> dummy >> save_count_stat
				>> dummy >> save_count_fstat
				>> dummy >> dim
				>> dummy >> gravity
				>> dummy >> options_fstat
				>> dummy >> options_data
				>> dummy >> options_ene  
				>> dummy;
				//this is optional to allow restart files with and without options_restart  
			if (!strcmp(dummy.c_str(),"options_restart")) is >> options_restart >> dummy;
			unsigned int N; 
			is >> N; 
			Species.resize(0); //to clear the Species vector
			Species.resize(N);
			//please don't change this code to iterators; the mixed species requires indexing
			//old code: for (vector<CSpecies>::iterator it = Species.begin(); it!=Species.end(); ++it) it->read(is);
			for (unsigned int i=0; i<N; i++) {
				Species[i].read(is);
				Species[i].MixedSpecies.resize(i);
				for (unsigned int j=0; j<i; j++) {
					Species[i].MixedSpecies[j].read(is);
				}
			}
            
            std::stringstream line (std::stringstream::in | std::stringstream::out);
            
			is >> dummy >> N; 
			wallHandler.clear();
			getLineFromStringStream(is,line);
            for (unsigned int i=0;i<N;i++)
			{
				getLineFromStringStream(is,line);
				wallHandler.readWall(line);
			}
            
			is >> dummy >> N;
			boundaryHandler.clear();
			getLineFromStringStream(is,line);
			for (unsigned int i=0;i<N;i++)
			{
				getLineFromStringStream(is,line);
				boundaryHandler.readObject(line);
			}
			
			is >> dummy >> N;
			particleHandler.clear();
			getLineFromStringStream(is,line);
			for (unsigned int i=0;i<N;i++)
			{ 
				getLineFromStringStream(is,line);
				particleHandler.readObject(line);
				particleHandler.getLastObject()->compute_particle_mass(Species);
			}
			
			//After the lines storing particle data, an optional variable 'FixedParticles' can be read in, which fixes the first 'FixedParticles' particles
			if (is.peek()==70) is >> dummy >> data_FixedParticles; 
		}
	}
}



void MD::read_v2(std::istream& is) 
{
	std::string dummy;
	//std::cout << "version 2" << std::endl;
	is	>> dummy >> xmin
		>> dummy >> xmax
		>> dummy >> ymin
		>> dummy >> ymax
		>> dummy >> zmin
		>> dummy >> zmax
		>> dummy >> dt
		>> dummy >> t
		>> dummy >> tmax
		>> dummy >> save_count_data
		>> dummy >> dim
		>> dummy >> gravity
		>> dummy >> options_fstat
		>> dummy >> options_data
		>> dummy >> options_ene
		>> dummy;
	save_count_ene=save_count_data;
	save_count_fstat=save_count_data;
	save_count_stat=save_count_data;

	//this is optional to allow restart files with and without options_restart  
	if (!strcmp(dummy.c_str(),"options_restart")) is >> options_restart >> dummy;

	unsigned int N; 

	is >> N; 
	Species.resize(N);
	//please don't change this code to iterators; the mixed species requires indexing
	//old code: for (vector<CSpecies>::iterator it = Species.begin(); it!=Species.end(); ++it) it->read(is);
	for (unsigned int i=0; i<N; i++) {
		Species[i].read(is);
		Species[i].MixedSpecies.resize(i);
		for (unsigned int j=0; j<i; j++) {
			Species[i].MixedSpecies[j].read(is);
		}
	}

	std::stringstream line (std::stringstream::in | std::stringstream::out);
	
	is >> dummy >> N; 
	wallHandler.clear();
	getLineFromStringStream(is,line);	
	for (unsigned int i=0;i<N;i++)
	{
		getLineFromStringStream(is,line);
		wallHandler.readWall(line);
	}

	is >> dummy >> N;
	boundaryHandler.clear();
	getLineFromStringStream(is,line);	
	for (unsigned int i=0;i<N;i++)
	{
		getLineFromStringStream(is,line);
		boundaryHandler.readObject(line);	
	}

	is >> dummy >> N;
	particleHandler.clear();
	getLineFromStringStream(is,line);	
	for (unsigned int i=0;i<N;i++)
	{ 
		getLineFromStringStream(is,line);
		particleHandler.readObject(line);
		particleHandler.getLastObject()->compute_particle_mass(Species);
	}
	
	//After the lines storing particle data, an optional variable 'FixedParticles' can be read in, which fixes the first 'FixedParticles' particles
	if (is.peek()==70) is >> dummy >> data_FixedParticles; 
}


///Writes all MD data
void MD::write_v1(std::ostream& os) 
{
	os << dim << " " << gravity << " " 
		<< xmin << " " << xmax << " " << ymin << " " << ymax << " " << zmin << " " << zmax << " " 
		<< dt << " " << t << " " << tmax << " " 
		<< save_count_data << " " << max_radius << " " 
		<< problem_name.str() << " " 
		<< Species.size() << " " 
		<< particleHandler.getNumberOfObjects() << " " 
		<< wallHandler.getNumberOfWalls() << " " 
		<< boundaryHandler.getNumberOfObjects() << std::endl;
	os << options_fstat << " " << options_data << " " << options_ene << std::endl; 
	for (std::vector<CSpecies>::iterator it = Species.begin(); it!=Species.end(); ++it) os << (*it) << std::endl;
	for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it!=particleHandler.end(); ++it) os << (**it) << std::endl;
	for (std::vector<BaseWall*>::iterator it = wallHandler.begin(); it!=wallHandler.end(); ++it) os << (**it) << std::endl;
	for (std::vector<BaseBoundary*>::iterator it = boundaryHandler.begin(); it!=boundaryHandler.end(); ++it) os << (**it) << std::endl;
}

///Reads all MD data
void MD::read_v1(std::istream& is) 
{
	std::cout << "reading restart data version 1" << std::endl;
	is	>> gravity 
		>> xmin >> xmax >> ymin >> ymax >> zmin >> zmax 
		>> dt >> t >> tmax 
		>> save_count_data >> max_radius;
	save_count_ene=save_count_data;
	save_count_fstat=save_count_data;
	save_count_stat=save_count_data;			
	std::string str;
	is >> str;
	problem_name.str(str);
	int N;
	unsigned int NParticles, NWalls, NBoundarys;
	is >> N; Species.resize(N);
	is >> NParticles; 			particleHandler.setStorageCapacity(NParticles);
	is >> NWalls;				wallHandler.setStorageCapacity(NWalls);
	is >> NBoundarys; 			boundaryHandler.setStorageCapacity(NBoundarys);
	is >> options_fstat >> options_data >> options_ene; 
	for (std::vector<CSpecies>::iterator it = Species.begin(); it!=Species.end(); ++it) is >> (*it);
	
	std::stringstream line (std::stringstream::in | std::stringstream::out);
	getLineFromStringStream(is,line);
	
	for (unsigned int i=0;i<NParticles;i++)
	{
		getLineFromStringStream(is,line);
		particleHandler.readObject(line);
		particleHandler.getLastObject()->compute_particle_mass(Species);
	}
	
	for (unsigned int i=0;i<NWalls;i++)
	{
		getLineFromStringStream(is,line);
		wallHandler.readWall(line);
	}
	
	for (unsigned int i=0;i<NBoundarys;i++)
	{
		getLineFromStringStream(is,line);
		boundaryHandler.readObject(line);
	}
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
///This is the main solve loop where all the action takes place///
/////////////////////////////////////////////////////////////////////////////////////////////////////
void MD::solve()
{
	#ifdef DEBUG_OUTPUT
		std::cerr << "Entered solve" << std::endl;
	#endif
    #ifdef ContactListHgrid
        std::cout << "Using ContactListHgrid"<<std::endl;
    #endif
		
	/// This is where the main code works
	
	///Print counter if it is used
	//if (get_counter()) std::cout << "Count: " << get_counter() << std::endl;
	//else std::cout << "Started to solve ..." << std::endl;
					
	///Set up the data file names
	if (get_counter()>0) problem_name << "." << get_counter();
	
	/// Initialise the time and
	/// sets up the initial conditions for the simulation
	///\todo Is it neccesarry to reset initial conditions here and in set_dt (i.e. should it be in constructor)?
	///Thomas: I agree, set_dt should be rewritten to work without calling setup_particles_initial_conditions
	if (!restarted) {
		t=0;
		setup_particles_initial_conditions();
		#ifdef DEBUG_OUTPUT 
			std::cerr << "Have created the particles initial conditions " << std::endl;
		#endif
	}


	///Set up the data file names
	set_data_filename();

	//undo append if data file does not exist
	//~ std::cout << "                  get_data_filename()" << get_data_filename() << ", append" << append << ", access" << access(get_data_filename().c_str(), F_OK ) << std::endl;
	//~ if (append && (access(get_data_filename().c_str(), F_OK ) == -1) ) {
		//~ append = false;
		//~ std::cerr << "Warning: cannot append because data file does not exist; set append to false." << std::endl;
	//~ }
	if (get_append()) {
		if (get_options_data()==2) increase_counter_data(std::fstream::out);
		if (get_options_ene()==2) increase_counter_ene(std::fstream::out);
		if (get_options_stat()==2) increase_counter_stat(std::fstream::out);
		if (get_options_fstat()==2) increase_counter_fstat(std::fstream::out);		
	}

	std::fstream::openmode mode;
	if (append) mode=std::fstream::out|std::fstream::app;
	else        mode=std::fstream::out;

	///This creates the file the data will be saved to
	if(!open_data_file(mode)) {std::cerr << "Problem opening data file aborting" << std::endl; exit(-1);}
	
	///Set up the ene file names
	set_ene_filename();
	
	///This creates the file ene will be saved to
	if(!open_ene_file(mode)){std::cerr << "Problem opening ene file aborting" << std::endl; exit(-1);}

	///Set up the fstat file names
	set_fstat_filename();
		
	///This creates the file fstatistics will be saved to
	if(!open_fstat_file(mode)){std::cerr << "Problem opening fstat file aborting" << std::endl; exit(-1);}

	///initializes PrevPosition if PositionVerlet is used
	#ifdef USE_SIMPLE_VERLET_INTEGRATION
		std::cout << "using simple verlet integration" << std::endl;
		for (int i = 0;i<Particles.size();i++) 
			getParticleHandler().getObject(i)->PrevPosition = getParticleHandler().getObject(i)->get_Position() - getParticleHandler().getObject(i)->get_Velocity() * dt + 0.5 * gravity * dt * dt;
	#endif 	
	
	/// Setup the mass of each particle.
	compute_particle_masses();
	
	/// Other initializations
	max_radius = getLargestParticle()->get_Radius();
	actions_before_time_loop();
	initialize_statistics();
	HGRID_actions_before_time_loop();
	
	ene_ela = 0;
	if (!append) start_ene();

	//needed for output_statistics
	save_data_data =false;
	save_data_ene  =false;
	save_data_stat =false;
	save_data_fstat=false;

	/// velocity verlet requires initial force calculation
	if(do_stat_always||save_data_stat) output_statistics();
	PeriodicCreated=Check_and_Duplicate_Periodic_Particles();
	HGRID_actions_before_time_step();
	///\todo{Thomas: the tangential spring should not be integrated here, but in the integration function; currently, the integration is supressed by setting dt=0, but sliding tangential springs are integrated}
	Mdouble dt_=dt; dt=0; compute_all_forces(); dt=dt_;
	Remove_Duplicate_Periodic_Particles();
	set_initial_pressures_for_pressure_controlled_walls();
	if(do_stat_always||save_data_stat) process_statistics(save_data_stat);
	#ifdef DEBUG_OUTPUT
		std::cerr << "Have computed the initial values for the forces " << std::endl;
	#endif 

	if (append) {
		save_data_data =false;
		save_data_ene  =false;
		save_data_stat =false;
		save_data_fstat=false;
	} else {
		save_data_data =true;
		save_data_ene  =true;
		save_data_stat =true;
		save_data_fstat=true;
	}

	int count_data=1;
	int count_ene=1;
	int count_stat=1;
	int count_fstat=1;
	//create .disp file if .data file is created
	/// \bug This CmakeDeftions is current not included and should be readded. Currectly cause a strange compliler error, which I (Ant) do not follow
	if (get_options_data()) create_xballs_script();
	//if ((get_options_data()) && (xballsSupportOn()=="ON")) create_xballs_script();
	
	ene_ela = 0;
	/// This is the main loop over advancing time
	while (t-dt<tmax&&continue_solve())
	{
        for (std::vector<BaseBoundary*>::iterator it = boundaryHandler.begin(); it!=boundaryHandler.end(); ++it)
		{
			(*it)->checkBoundaryActionsBeforeTimeStep(particleHandler,wallHandler,random);
		}       
		/// Here we output to the data file
		///\todo{When do we actually want to output force and location data?}
		if (save_data_data)
		{
			if (get_options_data()==2) increase_counter_data(std::fstream::out);
			if (get_options_restart()) save_restart_data();
		}
		if (save_data_ene)
		{
			if (get_options_ene()==2) increase_counter_ene(std::fstream::out);
		}
		if(save_data_stat)
		{
			if (get_options_stat()==2) increase_counter_stat(std::fstream::out);
		}
		if (save_data_fstat)
		{
			if (get_options_fstat()==2) increase_counter_fstat(std::fstream::out);		
		}
		
		/// Check if rotation is on
		rotation=false;
		for (unsigned int i=0;i<Species.size();i++) {
			if (Species[i].get_mu()||Species[i].get_murolling()||Species[i].get_mutorsion()) 
				rotation = true;
			for (unsigned int j=0;j<Species[i].MixedSpecies.size();j++) {
				if (Species[i].MixedSpecies[j].get_mu()||Species[i].MixedSpecies[j].get_murolling()||Species[i].MixedSpecies[j].get_mutorsion()) 
					rotation = true;
			}
		}

		/// Loop over all particles doing the time integration step
		HGRID_actions_before_integration();
		for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it!=particleHandler.end(); ++it)
		{
			do_integration_before_force_computation(*it);
		}
		checkInteractionWithBoundaries();
		HGRID_actions_after_integration();
	
		/// Output statistical information
		if (save_data_data) output_xballs_data();
		if (save_data_stat||do_stat_always) output_statistics();
		
		/// Compute forces
		
		///bug{In chute particles are added in actions_before_time_set(), however they are not written to the xballs data yet, but can have a collison and be written to the fstat data}
		actions_before_time_step();
		PeriodicCreated=Check_and_Duplicate_Periodic_Particles();

		HGRID_actions_before_time_step();
		
		if (save_data_fstat)	fstat_header();


		compute_all_forces();
		Remove_Duplicate_Periodic_Particles();
		actions_after_time_step();
							
		/// Loop over all particles doing the time integration step
		HGRID_actions_before_integration();
		for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it!=particleHandler.end(); ++it)
		{
			do_integration_after_force_computation(*it);
		}
		checkInteractionWithBoundaries();
		HGRID_actions_after_integration();
		
		if (save_data_ene) output_ene();		

		if(save_data_stat||do_stat_always) process_statistics(save_data_stat);				

		t+=dt;

		//To write the last time step (acutaly data is written at t+0.5*dt where t alreay is larger then tmax)
		if (count_data == save_count_data || t>tmax) //write data
		{
			save_data_data=true;
			count_data=1;
			cout_time();
		}
		else
		{
			save_data_data=false;
			count_data++;
		}
		if (count_ene == save_count_ene || t>tmax) //write data
		{
			save_data_ene=true;
			count_ene=1;
		}
		else
		{
			save_data_ene=false;
			count_ene++;
		}
		if (count_fstat == save_count_fstat || t>tmax) //write data
		{
			save_data_fstat=true;
			count_fstat=1;
		}
		else
		{
			save_data_fstat=false;
			count_fstat++;
		}
		if (count_stat == save_count_stat || t>tmax) //write data
		{
			save_data_stat=true;
			count_stat=1;
		}
		else
		{
			save_data_stat=false;
			count_stat++;
		}
		
	}
	//end loop over interaction count
	actions_after_solve();

	std::cout << std::endl;
	//To make sure get_t gets the correct time for outputting statistics
	finish_statistics();
	
	data_file.close();
			
	ene_file.close();
	
	fstat_file.close();
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
///Can interpret main function input arguments that are passed by the driver codes
/////////////////////////////////////////////////////////////////////////////////////////////////////
int MD::readArguments(unsigned int argc, char *argv[])
{
	bool isRead = true;
	for (unsigned int i = 1; i<argc; i+=2) {
		std::cout << "interpreting input argument " << argv[i]; 
		for (unsigned int j = i+1; j<argc; j++) {
			if (argv[j][0]=='-') break;
			std::cout << " " << argv[j];
		}
		std::cout << std::endl;
		isRead &= readNextArgument(i, argc, argv);
		if (!isRead) {
			std::cerr << "Warning: not all arguments read correctly!" << std::endl;
			exit(-10);
		}
	}	
	return isRead;
}

int MD::readNextArgument(unsigned int& i, unsigned int argc, char *argv[])
{
	///argv[i+1] interpreted as argument of type char*, Mdouble, integer or boolean unless noted
	if (!strcmp(argv[i],"-name")) {
		set_name(argv[i+1]);
	} else if (!strcmp(argv[i],"-xmin")) {
		set_xmin(atof(argv[i+1]));
	} else if (!strcmp(argv[i],"-ymin")) {
		set_ymin(atof(argv[i+1]));
	} else if (!strcmp(argv[i],"-zmin")) {
		set_zmin(atof(argv[i+1]));
	} else if (!strcmp(argv[i],"-xmax")) {
		set_xmax(atof(argv[i+1]));
	} else if (!strcmp(argv[i],"-ymax")) {
		set_ymax(atof(argv[i+1]));
	} else if (!strcmp(argv[i],"-zmax")) {
		set_zmax(atof(argv[i+1]));
	//} else if (!strcmp(argv[i],"-svn")) {
	//	std::cout << "svn version " << SVN_VERSION << std::endl;
	//	i--;
	} else if (!strcmp(argv[i],"-dt")) { 
		Mdouble old=get_dt();
		set_dt(atof(argv[i+1]));
		std::cout << "  reset dt from " << old << " to " << get_dt() << std::endl;
	} else if (!strcmp(argv[i],"-Hertzian")) { 
		Species[0].set_ForceType(Hertzian);
		i--;
	} else if (!strcmp(argv[i],"-tmax")) {
		Mdouble old = get_tmax();
		set_tmax(atof(argv[i+1]));
		std::cout << "  reset tmax from " << old << " to " << get_tmax() << std::endl;
	} else if (!strcmp(argv[i],"-save_count") || !strcmp(argv[i],"-savecount")) {
		set_savecount(atoi(argv[i+1]));
	} else if (!strcmp(argv[i],"-save_count_data")) {
		set_save_count_data(atoi(argv[i+1]));
	} else if (!strcmp(argv[i],"-save_count_fstat")) {
		set_save_count_fstat(atoi(argv[i+1]));
	} else if (!strcmp(argv[i],"-save_count_stat")) {
		set_save_count_stat(atoi(argv[i+1]));
	} else if (!strcmp(argv[i],"-save_count_ene")) {
		set_save_count_ene(atoi(argv[i+1]));
	} else if (!strcmp(argv[i],"-dim")) {
		set_dim(atoi(argv[i+1]));
	} else if (!strcmp(argv[i],"-gravity")) {
		///-gravity requires three arguments
		set_gravity(Vec3D(atof(argv[i+1]),atof(argv[i+2]),atof(argv[i+3]))); 
		i+=2; 
	} else if (!strcmp(argv[i],"-options_fstat")) {
		set_options_fstat(atoi(argv[i+1]));
	} else if (!strcmp(argv[i],"-options_restart")) {
		set_options_restart(atoi(argv[i+1]));
	} else if (!strcmp(argv[i],"-options_data")) {
		set_options_data(atoi(argv[i+1]));
	} else if (!strcmp(argv[i],"-options_stat")) {
		set_options_stat(atoi(argv[i+1]));
	} else if (!strcmp(argv[i],"-options_ene")) {
		set_options_ene(atoi(argv[i+1]));	
	} else if (!strcmp(argv[i],"-auto_number")) {
		auto_number();	
		i--;
	} else if (!strcmp(argv[i],"-number_of_saves")) {
		set_number_of_saves_all(atof(argv[i+1]));
	} else if (!strcmp(argv[i],"-restart")||!strcmp(argv[i],"-r")) {
		///-restart or -r loads a restart file. 
		///By default, it loads <get_name()>.restart.
		///If an argument "arg" is given it loads the file "arg", or "arg".restart (if the ending is not given).
		std::string filename;
		
		//use default filename if no argument is given
		if (i+1>=argc||argv[i+1][0]=='-') {
			i--;
			filename = get_name();
			std::cout << get_name() << std::endl;
		} else filename = argv[i+1];
				
		//add ".restart" if necessary
		const char* fileextension = (filename.c_str()+std::max(0,(int)filename.length()-8));
		if (strcmp(fileextension,".restart")) 
			filename=filename+".restart";
		
		std::cout << "restart from " << filename << std::endl;
		load_restart_data(filename);
	} else if (!strcmp(argv[i],"-data")) {
		std::string filename = argv[i+1];
		load_from_data_file(filename.c_str());
	} else if (!strcmp(argv[i],"-k")) {
		Mdouble old = get_k();
		set_k(atof(argv[i+1]));
		std::cout << "  reset k from " << old << " to " << get_k() << std::endl;
	} else if (!strcmp(argv[i],"-disp")) {
		Mdouble old = get_disp();
		set_disp(atof(argv[i+1]));
		std::cout << "  reset disp from " << old << " to " << get_disp() << std::endl;
	} else if (!strcmp(argv[i],"-kt")) {
		Mdouble old = get_kt();
		set_kt(atof(argv[i+1]));
		std::cout << "  reset kt from " << old << " to " << get_kt() << std::endl;
	} else if (!strcmp(argv[i],"-dispt")) {
		Mdouble old = get_dispt();
		set_dispt(atof(argv[i+1]));
		std::cout << "  reset dispt from " << old << " to " << get_dispt() << std::endl;
	} else if (!strcmp(argv[i],"-krolling")) {
		Mdouble old = get_krolling();
		set_krolling(atof(argv[i+1]));
		std::cout << "  reset krolling from " << old << " to " << get_krolling() << std::endl;
	} else if (!strcmp(argv[i],"-disprolling")) {
		Mdouble old = get_disprolling();
		set_disprolling(atof(argv[i+1]));
		std::cout << "  reset disprolling from " << old << " to " << get_disprolling() << std::endl;
	} else if (!strcmp(argv[i],"-mu")) {
		Mdouble old = get_mu();
		set_mu(atof(argv[i+1]));
		std::cout << "  reset mu from " << old << " to " << get_mu() << std::endl;
	} else if (!strcmp(argv[i],"-murolling")) {
		Mdouble old = get_mu();
		set_murolling(atof(argv[i+1]));
		std::cout << "  reset murolling from " << old << " to " << get_murolling() << std::endl;
	} else if (!strcmp(argv[i],"-randomise") || !strcmp(argv[i],"-randomize")) {
		random.randomise(); 
		i--;
	} else if (!strcmp(argv[i],"-k0")) {
		Mdouble old = Species[0].get_k0();
		Species[0].set_k0(atof(argv[i+1]));
		std::cout << "  reset k0 from " << old << " to " << Species[0].get_k0() << std::endl;
	} else if (!strcmp(argv[i],"-f0")) {
		Mdouble old = Species[0].get_f0();
		Species[0].set_f0(atof(argv[i+1]));
		std::cout << "  reset f0 from " << old << " to " << Species[0].get_f0() << std::endl;
	} else if (!strcmp(argv[i],"-AdhesionForceType")) {
		Mdouble old = Species[0].get_AdhesionForceType();
		Species[0].set_AdhesionForceType(argv[i+1]);
		std::cout << "  reset AdhesionForceType from " << old << " to " << Species[0].get_AdhesionForceType() << std::endl;
	} else if (!strcmp(argv[i],"-append")) {
		set_append(true); 
		i--;
	} else if (!strcmp(argv[i],"-fixedParticles")) {
		set_FixedParticles(atof(argv[i+1]));
	} else if (!strcmp(argv[i],"-rho")) {
		set_rho(atof(argv[i+1]));
	} else if (!strcmp(argv[i],"-dim_particle")) {
		set_dim_particle(atoi(argv[i+1]));
	} else if (!strcmp(argv[i],"-counter")) {
		set_counter(atoi(argv[i+1]));
	} else return false;
	return true; //returns true if argv is found
}

	
	///Remove periodic duplicate Particles (i.e. removes particles created by Check_and_Duplicate_Periodic_Particle(int i, int nWallPeriodic)). Note that between these two functions it is not allowed to create additional functions. It returns the number of Particles removed
	void MD::Remove_Duplicate_Periodic_Particles()
	{
		int R=0;
		for(int i=particleHandler.getNumberOfObjects()-1; i>=0&&particleHandler.getObject(i)->get_PeriodicFromParticle()!=NULL; i--)
		{
			removeParticle(i);
			R++;
		}
		if(R!=PeriodicCreated)
		{
			std::cerr<<"ERROR :: Periodic Particles Removed not equal to Periodic Particles Created, it created "<<PeriodicCreated<<" Particles, but removed "<<R<<std::endl<<std::endl;
			exit(-1);
		}
	}

	///Calls Check_and_Duplicate_Periodic_Particle for all Particles curently in Particles[] and returns the number of particles created
	int MD::Check_and_Duplicate_Periodic_Particles()
	{
//		std::cout<<"Largest R="<<particleHandler.getLargestParticle()->get_Radius()<<std::endl;
		int C=0;
		for (unsigned int k=0; k<boundaryHandler.getNumberOfObjects(); k++)
		{
//			std::cout<<"Checking wall number: "<<k<<std::endl;
			int N=particleHandler.getNumberOfObjects(); //Required because the number of particles increaeses
			for (int i=0; i<N; i++)
			{
//				std::cout<<"Particle number: "<<i<<std::endl;
				C+=boundaryHandler.getObject(k)->createPeriodicParticles(particleHandler.getObject(i), particleHandler);
			}
		}
		return C;			
	}
