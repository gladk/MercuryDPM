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

#include "Chute.h"
#include "ChuteBottom.h"

///This is the actually constructor it is called do both constructors above.
void Chute::constructor(){
	is_periodic=false;
	set_FixedParticleRadius(0.001);
	set_RandomizedBottom(0);
	set_ChuteAngle(0.0);

	set_max_failed(1);
	num_created = 0;
	set_InflowParticleRadius(0.001);
	set_InflowVelocity(0.1);
	set_InflowVelocityVariance(0.0);
	set_InflowHeight(0.02);
	initialize_inflow_particle();
}

///This function reads all chute data
void Chute::read(std::istream& is)  { 
	HGRID_base::read(is); 
	//read out the full line first, so if there is an error it does not affect the read of the next line
	std::string line_string;
	getline(is,line_string);
	std::cout<<"Chuteline="<<line_string<<std::endl;
	std::stringstream line (std::stringstream::in | std::stringstream::out);
	line << line_string;

	if (get_restart_version()==1) {
		line >> FixedParticleRadius >> RandomizedBottom >> ChuteAngle 
		   >> MinInflowParticleRadius >> MaxInflowParticleRadius >> max_failed >> num_created
		   >> InflowVelocity >> InflowVelocityVariance >> InflowHeight;
	} else {
		std::string dummy;
		line >> dummy >> FixedParticleRadius >> dummy >> MinInflowParticleRadius >> dummy >> MaxInflowParticleRadius
				 >> dummy >> RandomizedBottom >> dummy >> ChuteAngle >> dummy >> max_failed >> dummy >> num_created
		   >> dummy >> InflowVelocity >> dummy >> InflowVelocityVariance >> dummy >> InflowHeight;
	}
	//if the Chute Angle is given in degrees, move to radians;
	if (ChuteAngle>1.0) ChuteAngle *= constants::pi/180.; 
}

///This function writes all chute data
void Chute::write(std::ostream& os)  { 
	HGRID_base::write(os); 
	os<< "FixedParticleRadius " << FixedParticleRadius 
  << " MinInflowParticleRadius " << MinInflowParticleRadius 
  << " MaxInflowParticleRadius " << MaxInflowParticleRadius 
  << std::endl
  << "RandomizedBottom " << RandomizedBottom 
  << " ChuteAngle " << ChuteAngle/constants::pi*180.
  << " max_failed " << max_failed 
  << " num_created " << num_created 
  << std::endl
  << "InflowVelocity " << InflowVelocity 
  << " InflowVelocityVariance " << InflowVelocityVariance 
  << " InflowHeight " << InflowHeight 
  << std::endl;
}

///This function std::couts all chute data
void Chute::print(std::ostream& os, bool print_all) {
	HGRID_base::print(os, print_all); 
	os << " FixedParticleRadius:" << FixedParticleRadius << ", InflowParticleRadius: [" << MinInflowParticleRadius << "," << MaxInflowParticleRadius << "]," << std::endl
		 << " RandomizedBottom:" << RandomizedBottom << ", ChuteAngle:" << ChuteAngle/constants::pi*180. << ", max_failed:" << max_failed << ", num_created:" << num_created << "," << std::endl
		 << " InflowVelocity:" << InflowVelocity << ", InflowVelocityVariance:" << InflowVelocityVariance << ", InflowHeight:" << InflowHeight << std::endl;
}

///adds particle to hgrid
void Chute::add_particle(BaseParticle& P) {
	//Puts the particle in the Particle list
	getParticleHandler().copyAndAddObject(P);
	//This places the particle in this grid
	grid->InsertParticleToHgrid(getParticleHandler().getLastObject());
	//This computes where the particle currectly is in the grid
#ifndef ContactListHgrid
	HGRID_UpdateParticleInHgrid(getParticleHandler().getLastObject());
#endif
}

void Chute::actions_before_time_step(){
	add_particles();
	clean_chute();
}

///Sets initial values for particles that are created at the inflow
void Chute::initialize_inflow_particle()
{
	//Position, maybe also radius and velocity, is reset in create_inflow_particle()
	P0.set_Radius(MinInflowParticleRadius);
	P0.compute_particle_mass(Species);
	P0.set_Angle(Vec3D(0.0,0.0,0.0));
	P0.set_AngularVelocity(Vec3D(0.0,0.0,0.0));
	//P0.get_TangentialSprings().reset();
	P0.set_Position(Vec3D(1e20,1e20,1e20));
	P0.set_Velocity(Vec3D(0.0,0.0,0.0));
}

///Sets variable values for particles that are created at the inflow
void Chute::create_inflow_particle()
{
	P0.set_Radius(random.get_RN(MinInflowParticleRadius,MaxInflowParticleRadius));
	P0.compute_particle_mass(Species);
	
    Vec3D position,velocity;

	position.X= get_xmin() + P0.get_Radius();
	if ((get_ymax()-get_ymin())==2.0*MaxInflowParticleRadius)
        position.Y = get_ymin() + P0.get_Radius();
	else
        position.Y = random.get_RN(get_ymin() + P0.get_Radius(), get_ymax() - P0.get_Radius());
	position.Z = random.get_RN(get_zmin() + FixedParticleRadius + P0.get_Radius(), get_zmax() - P0.get_Radius());
    P0.set_Position(position); 
    
    //The velocity components are first stored in a Vec3D, because if you pass them directly into set_Velocity the compiler is allowed to change the order in which the numbers are generated
    velocity.X=InflowVelocity * random.get_RN(-InflowVelocityVariance,InflowVelocityVariance) + InflowVelocity;
    velocity.Y=InflowVelocity * random.get_RN(-InflowVelocityVariance,InflowVelocityVariance);
    velocity.Z=InflowVelocity * random.get_RN(-InflowVelocityVariance,InflowVelocityVariance);
    P0.set_Velocity(velocity);
}

void Chute::cout_time() {
	std::cout << "\rt=" << std::setprecision(3) << std::left << std::setw(6) << get_t() 
		<< ", tmax=" << std::setprecision(3) << std::left << std::setw(6) << get_tmax()
		<< ", N=" << std::setprecision(3) << std::left << std::setw(6) << getParticleHandler().getNumberOfObjects()
		<< "\r";
		std::cout.flush();
}

///Sets k, disp such that it matches a given tc and eps for a collision of two inflow particles
void Chute::set_collision_time_and_restitution_coefficient(Mdouble tc, Mdouble eps){
	P0.set_Radius(0.5*(MinInflowParticleRadius+MaxInflowParticleRadius));
	P0.compute_particle_mass(Species);
	set_dissipation(- P0.get_Mass() / tc * log(eps));
	set_k(.5 * P0.get_Mass() * (constants::pi*constants::pi/tc/tc + get_dissipation()*get_dissipation()/sqr(P0.get_Mass())));
	std::cout << "collision time and restitution coefficient is set for a particle of radius " << P0.get_Radius() << " and density " << get_rho() << std::endl; 
}
///Calculates collision time of two inflow particles
Mdouble Chute::get_collision_time(){
	P0.set_Radius(0.5*(MinInflowParticleRadius+MaxInflowParticleRadius));
	P0.compute_particle_mass(Species);
	return MD::get_collision_time(P0.get_Mass(),P0.get_IndSpecies());
}
///Calculates restitution coefficient of two inflow particles
Mdouble Chute::get_restitution_coefficient(){
	P0.set_Radius(0.5*(MinInflowParticleRadius+MaxInflowParticleRadius));
	P0.compute_particle_mass(Species);
	return MD::get_restitution_coefficient(P0.get_Mass(),P0.get_IndSpecies());
}

///Returns the smallest particle (by mass) in the system
BaseParticle* Chute::getSmallestParticle(){
	P0.set_Radius(MinInflowParticleRadius);
  P0.compute_particle_mass(Species);
	BaseParticle* pP = &P0;
	for (unsigned int i=0; i<getParticleHandler().getNumberOfObjects(); i++) { 
		if (getParticleHandler().getObject(i)->get_Mass()<pP->get_Mass()) pP = getParticleHandler().getObject(i); 
	}
	return pP;
}

///Returns the smallest particle (by mass) in the system
BaseParticle* Chute::getLargestParticle(){
	P0.set_Radius(MaxInflowParticleRadius);
	P0.compute_particle_mass(Species);
	BaseParticle* pP = &P0;
	for (unsigned int i=0; i<getParticleHandler().getNumberOfObjects(); i++) { 
		if (getParticleHandler().getObject(i)->get_Radius()>pP->get_Radius()) pP = getParticleHandler().getObject(i); 
	}
	return pP;
}

///Returns the radius of the smallest particle
Mdouble Chute::get_SmallestParticleInteractionRadius(){
	Mdouble min_rad = MinInflowParticleRadius;
	if (FixedParticleRadius) 
		min_rad = std::min(min_rad,FixedParticleRadius);
	for (std::vector<BaseParticle*>::iterator it = getParticleHandler().begin(); it!=getParticleHandler().end(); ++it)
		min_rad=std::min(min_rad,(*it)->get_InteractionRadius());
	return min_rad;
}

Mdouble Chute::get_LargestParticleInteractionRadius(){
	Mdouble max_rad=std::max(FixedParticleRadius,MaxInflowParticleRadius);
	for (std::vector<BaseParticle*>::iterator it = getParticleHandler().begin(); it!=getParticleHandler().end(); ++it)
		max_rad=std::max(max_rad,(*it)->get_InteractionRadius());
	return max_rad;

}

///Returns the radius of the smallest particle
Mdouble Chute::get_LightestParticleMass(){
	Mdouble Mass =  getParticleHandler().getLightestParticle()->get_Mass();
	P0.set_Radius(MinInflowParticleRadius);
	P0.compute_particle_mass(Species);
	Mdouble MassP0 =  P0.get_Mass();
	return std::min(Mass,MassP0);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
///This initially set up the particles///
////////////////////////////////////////////////////////////////////////////////////////////////////
void Chute::setup_particles_initial_conditions()
{
	//set side walls - solid if not a periodic
	if (is_periodic)
	{
		PeriodicBoundary b0;
		b0.set(Vec3D( 0.0, 1.0, 0.0), get_ymin(), get_ymax());
		getBoundaryHandler().copyAndAddObject(b0);
	}
	else
	{
		InfiniteWall w0;
		w0.set(Vec3D( 0.0,-1.0, 0.0), -get_ymin());
		getWallHandler().copyAndAddWall(w0);
		w0.set(Vec3D( 0.0, 1.0, 0.0),  get_ymax());
		getWallHandler().copyAndAddWall(w0);
	}

	//creates the bottom of the chute
	create_bottom();
}	

////////////////////////////////////////////////////////////////////////////////////////////////////
///Creates the bottom of the chute; either smooth, grid-like or random ///
////////////////////////////////////////////////////////////////////////////////////////////////////
void Chute::create_bottom() {
	if (fabs(get_FixedParticleRadius())<1e-12) // smooth bottom
	{
		//bottom wall 
		InfiniteWall w0;
		w0.set(Vec3D(0.0, 0.0, -1.0), -get_zmin());
		getWallHandler().copyAndAddWall(w0);
	}
	else //rough bottom
	{
		// Define standard fixed particle
		/// \todo Does the bottom we always has to be this particle?
		TangentialSpringParticle F0;
		F0.set_Radius(get_FixedParticleRadius());
		F0.set_Position(Vec3D(0.0,0.0,0.0));

		//define grid parameters
		Mdouble dx = 2.0 * F0.get_Radius(); 
		Mdouble dy = 2.0 * F0.get_Radius();
		int nx = std::max(1,(int)floor((get_xmax()-get_xmin())/dx)); 
		int ny = std::max(1,(int)floor((get_ymax()-get_ymin())/dy));
		dx = (get_xmax()-get_xmin())/nx; dy = (get_ymax()-get_ymin())/ny;

		if (!RandomizedBottom) { // grid-like fixed-particle bottom
			for (int i=0; i<nx; i++)
				for (int j=0; j<ny; j++)
				{
					F0.set_Position(Vec3D(F0.get_Radius() + dx * i,F0.get_Radius() + dy * j,0.0));
					getParticleHandler().copyAndAddObject(F0);
				}
			
		} else if (RandomizedBottom==1) { // random fixed-particle bottom
			std::cout << "create rough chute bottom, fixed z" << std::endl;
		
			Mdouble dx = 2.0 * F0.get_Radius(); 
			Mdouble dy = 2.0 * F0.get_Radius();
			int nx = std::max(1,(int)floor((get_xmax()-get_xmin())/dx)); 
			int ny = std::max(1,(int)floor((get_ymax()-get_ymin())/dy));
			dx = (get_xmax()-get_xmin())/nx; dy = (get_ymax()-get_ymin())/ny;
			
			//bottom wall 
			InfiniteWall w0;
			w0.set(Vec3D(0.0, 0.0, -1.0), -(get_zmin()-.5*F0.get_Radius()));
			getWallHandler().copyAndAddWall(w0);			
			
			//add first particle to initialize HGRID
            
            //The position components are first stored in a Vec3D, because if you pass them directly into set_Position the compiler is allowed to change the order in which the numbers are generated
            Vec3D position;
            position.X=random.get_RN(F0.get_Radius(), get_xmax() - F0.get_Radius());
            position.Y=random.get_RN(get_ymin()+F0.get_Radius(), get_ymax()-F0.get_Radius());
            F0.set_Position(position);
			getParticleHandler().copyAndAddObject(F0);

			HGRID_actions_before_time_loop();
			HGRID_actions_before_time_step();
			
			//now add more particles
			int failed = 0;
			while (failed<500) 
			{
                //The position components are first stored in a Vec3D, because if you pass them directly into set_Position the compiler is allowed to change the order in which the numbers are generated
                position.X=random.get_RN(F0.get_Radius(), get_xmax() - F0.get_Radius());
                position.Y=random.get_RN(get_ymin()+F0.get_Radius(), get_ymax()-F0.get_Radius());
                F0.set_Position(position);
				if (IsInsertable(F0))
					failed = 0;
				else
					failed++;
			}
		} else {
			//this pointer is the current MD class, so the bottom is create with the particles properties from the MD class
			ChuteBottom bottom(*this);
			bottom.set_InflowParticleRadius(get_FixedParticleRadius());
			bottom.make_rough_bottom(getParticleHandler());
			std::cout<<"Starting to destruct ChuteBottom"<<std::endl;
		}
		std::cout<<"Destructed ChuteBottom"<<std::endl;
		//finally, fix particles to the floor
		for (std::vector<BaseParticle*>::iterator it= getParticleHandler().begin(); it!=getParticleHandler().end(); ++it) 
			(*it)->fixParticle();
			
			

	}
}
	


////////////////////////////////////////////////////////////////////////////////////////////////////
///New particles are created at the inflow, subject to criteria the user can set
////////////////////////////////////////////////////////////////////////////////////////////////////
void Chute::add_particles() 
{
	int failed = 0;
	
	//try max_failed times to find new insertable particle
	while (failed<=max_failed){
		create_inflow_particle();
		if (IsInsertable(P0)) {
			failed = 0; 
			num_created++;
		} else failed++;
	};
}


////////////////////////////////////////////////////////////////////////////////////////////////////
///New particles are destroyed at the outflow, subject to criteria the user can set
////////////////////////////////////////////////////////////////////////////////////////////////////
void Chute::clean_chute() 
{
	//clean outflow every 100 timesteps
	static int count = 0, maxcount = 100;
	if (count>maxcount)
	{
		count = 0;
		// delete all outflowing particles
		for (unsigned int i=0;i<getParticleHandler().getNumberOfObjects();)
		{

			if (getParticleHandler().getObject(i)->get_Position().X>get_xmax()||getParticleHandler().getObject(i)->get_Position().X<get_xmin())//||getParticleHandler().getObject(i)->Position.Z+getParticleHandler().getObject(i)->Radius<zmin)

			{
				#ifdef DEBUG_OUTPUT_FULL
					std::cout << "erased:" << getParticleHandler().getObject(i) << std::endl;
				#endif
				removeParticle(i);
			}	
			else i++;
		}
	} else count++;
}

bool Chute::IsInsertable(BaseParticle &P) {
	
	add_particle(P);
	//std::cout<<"Adding particle "<<P<<std::endl;
	if(TestObjAgainstGrid(grid,getParticleHandler().getLastObject()))
	{
#ifdef ContactListHgrid
		HGRID_UpdateParticleInHgrid(getParticleHandler().getLastObject());
#endif
		//std::cout<<"Past"<<std::endl;
		return true;
	}
	else
	{
		//std::cout<<"Past niet"<<std::endl;
		///todo{Maybe also check if the last particle in a Particular level is removed}
#ifndef ContactListHgrid
        int bucket = grid->ComputeHashBucketIndex(getParticleHandler().getLastObject()->get_HGRID_x(),getParticleHandler().getLastObject()->get_HGRID_y(), getParticleHandler().getLastObject()->get_HGRID_z(), getParticleHandler().getLastObject()->get_HGRID_Level());
		grid->objectBucket[bucket] = getParticleHandler().getLastObject()->get_HGRID_NextObject();
#endif
		getParticleHandler().removeLastObject();
		return false;
	}
}


int Chute::readNextArgument(unsigned int& i, unsigned int argc, char *argv[]) {
	if (!strcmp(argv[i],"-inflowHeight")) {
		set_InflowHeight(atof(argv[i+1]));
		set_zmax(atof(argv[i+1]));
	} else if (!strcmp(argv[i],"-inflowVelocity")) {
		set_InflowVelocity(atof(argv[i+1]));
	} else if (!strcmp(argv[i],"-chuteAngle")) {
		set_ChuteAngle(atof(argv[i+1]));
	} else if (!strcmp(argv[i],"-chuteLength")) {
		set_ChuteLength(atof(argv[i+1]));
	} else if (!strcmp(argv[i],"-chuteWidth")) {
		set_ChuteWidth(atof(argv[i+1]));
	} else if (!strcmp(argv[i],"-fixedParticleRadius")) {
		set_FixedParticleRadius(atof(argv[i+1]));
	} else if (!strcmp(argv[i],"-max_failed")) {
		set_max_failed(atoi(argv[i+1]));
	} else if (!strcmp(argv[i],"-inflowParticleRadiusRange")) {
		set_InflowParticleRadius(atof(argv[i+1]),atof(argv[i+2]));
		i++;
	} else if (!strcmp(argv[i],"-inflowParticleRadius")) {
		set_InflowParticleRadius(atof(argv[i+1]));
	} else if (!strcmp(argv[i],"-randomizedBottom")) {
		set_RandomizedBottom(atof(argv[i+1]));
	} else if (!strcmp(argv[i],"-k_eps")) {
		Mdouble Mass = get_LightestParticleMass();
		//~ Mdouble Mass =  getParticleHandler().get_LightestParticle()->get_Mass();
		Species[0].set_k_and_restitution_coefficient(atof(argv[i+1]), atof(argv[i+2]), Mass);
		std::cout << "reset contact properties of lightest Particle (mass=" << Mass << ") to k=" << get_k() << " and disp=" << get_disp() << std::endl;
		i+=1;
	} else if (!strcmp(argv[i],"-tc_eps")) {
		Mdouble Mass = get_LightestParticleMass();
		Species[0].set_collision_time_and_restitution_coefficient(atof(argv[i+1]), atof(argv[i+2]), Mass);
		std::cout << "reset contact properties of lightest Particle (mass=" << Mass << ") to k=" << get_k() << " and disp=" << get_disp() << std::endl;
		i+=1;
	} else if (!strcmp(argv[i],"-tc_eps_beta")) {
		Mdouble Mass = get_LightestParticleMass();
		Species[0].set_collision_time_and_normal_and_tangential_restitution_coefficient(atof(argv[i+1]), atof(argv[i+2]), atof(argv[i+3]), Mass);
		std::cout << "reset contact properties of lightest Particle (mass=" << Mass << ") to k=" << get_k() << ", disp=" << get_disp() << ", kt=" << get_kt() << " and dispt=" << get_dispt() << std::endl;
		i+=2;
	} else return HGRID_3D::readNextArgument(i, argc, argv); //if argv[i] is not found, check the commands in HGRID_3D
	return true; //returns true if argv[i] is found
}
