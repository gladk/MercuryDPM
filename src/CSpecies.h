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

#ifndef CSPECIES_H
#define CSPECIES_H

enum ForceTypes {Linear, Hertzian, HM, HMD};
enum AdhesionForceTypes {None, LinearReversible, LinearIrreversible};

//This is a header with some extra standard maths function and constants that are required by the code
#include "ExtendedMath.h"

#include "Vector.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
/** Stores properties of the particles and the contact models such as the elastic modulus.
 * 
 * As particle species are distinguished by their species Particle::indSpecies. 
 * Based on this index, different particle and contact properties apply 
 * which are stored in MD::Species. The contact model for interspecies 
 * interactions are defined in CSpecies::MixedSpecies
 * \bug
 * Segfault, caused by:
 * In MD::add_Species(...) (MD.cc, line 1298), if the line Species.push_back(S); causes a reallocation of internal storage, then the existing elements of Species are copy-constructed into the new memory. However, the copy constructor of CSpecies (CSpecies.h, line 35), does not copy the MixedSpecies vector, so if we call add_Species more than once (meaning that for the second and subsequent calls, existing elements in the Species vector have non-empty MixedSpecies vectors) we are liable to lose this data.
 * Fix:
 * Copy the MixedSpecies member in the copy constructor (CSpecies.h, line 40-ish): MixedSpecies = S.MixedSpecies;
 * However, it looks to me as though if this line is added, the copy constructor could be replaced by the default bitwise copy constructor, so perhaps the copying of MixedSpecies has been left out for a reason...
 */
class CSpecies {
public:
	CSpecies() {
		k = 0; 
		kt = 0;
		krolling = 0;
		ktorsion = 0;
		disp = 0;
		dispt = 0;
		disprolling = 0;
		disptorsion = 0;
		mu = 0;
		mus = 0;
		mu = 0;
		murolling = 0;
		musrolling = 0;
		mutorsion = 0;
		mustorsion = 0;
		k2max = 0;
		kc = 0;
		depth = 0;
		rho = 0;
		dim_particle = 0;
		logRestitution = 0;
		useRestitution=false;
		ForceType = Linear;
		AdhesionForceType = None;
		k0 = 0;
		f0 = 0;
	}

	///Allows the spring constant to be changed
	void set_k0(Mdouble new_k0){if(new_k0>=0) k0=new_k0; else { std::cerr << "Error in set_k0" << std::endl; exit(-1); }}
	///Allows the spring constant to be accessed
	Mdouble get_k0() const {return k;}

	///Allows the spring constant to be changed
	void set_f0(Mdouble new_f0){if(new_f0>=0) f0=new_f0; else { std::cerr << "Error in set_f0" << std::endl; exit(-1); }}
	///Allows the spring constant to be accessed
	Mdouble get_f0() const {return f0;}
	
	Mdouble get_InteractionDistance() {
		if (AdhesionForceType==None) 
			return 0.0;
		else
			return f0/k0;
	}

	///Allows the spring constant to be changed
	void set_k(Mdouble new_k){if(new_k>=0) k=new_k; else { std::cerr << "Error in set_k" << std::endl; exit(-1); }}
	///Allows the spring constant to be accessed
	Mdouble get_k() const {return k;}

	///Allows the spring and dissipation constants to be changed simultaneously
	void set_k_and_disp(helperFunc::KAndDisp new_){set_k(new_.k); set_disp(new_.disp); }
	///Allows the normal and tangential spring and dissipation constants to be changed simultaneously
	void set_k_and_disp_and_kt_and_dispt(helperFunc::KAndDispAndKtAndDispt new_){set_k(new_.k); set_disp(new_.disp); set_kt(new_.kt); set_dispt(new_.dispt); }
	
	///Allows the spring constant to be changed
	void set_kt(Mdouble new_kt){if(new_kt>=0) {kt=new_kt;} else { std::cerr << "Error in set_kt" << std::endl; exit(-1); }}
	///Allows the spring constant to be accessed
	Mdouble get_kt() const {return kt;}
	///Allows the spring constant to be changed
	void set_krolling(Mdouble new_k){if(new_k>=0) {krolling=new_k;} else { std::cerr << "Error in set_krolling" << std::endl; exit(-1); }}
	///Allows the spring constant to be accessed
	Mdouble get_krolling() const {return krolling;}
	///Allows the spring constant to be changed
	void set_ktorsion(Mdouble new_k){if(new_k>=0) {ktorsion=new_k;} else { std::cerr << "Error in set_ktorsion" << std::endl; exit(-1); }}
	///Allows the spring constant to be accessed
	Mdouble get_ktorsion() const {return ktorsion;}
	
	///\todo recalculate masses when setting dim_particle or rho
	///Allows the density to be changed
	void set_rho(Mdouble new_rho){if(new_rho>=0) rho=new_rho; else { std::cerr << "Error in set_rho" << std::endl; exit(-1); }}
	///Allows the density to be accessed
	Mdouble get_rho() const {return rho;}
	
	///Allows the tangential viscosity to be changed
	void set_dispt(Mdouble new_dispt){if (new_dispt>=0) dispt = new_dispt; else { std::cerr << "Error in set_dispt" << std::endl; exit(-1); }}
	///Allows the tangential viscosity to be accessed
	Mdouble get_dispt() const {return dispt;}
	///Allows the tangential viscosity to be changed
	void set_disprolling(Mdouble new_disprolling){if (new_disprolling>=0) disprolling = new_disprolling; else { std::cerr << "Error in set_disprolling" << std::endl; exit(-1); }}
	///Allows the tangential viscosity to be accessed
	Mdouble get_disprolling() const {return disprolling;}
	///Allows the tangential viscosity to be changed
	void set_disptorsion(Mdouble new_disptorsion){if (new_disptorsion>=0) disptorsion = new_disptorsion; else { std::cerr << "Error in set_disptorsion" << std::endl; exit(-1); }}
	///Allows the tangential viscosity to be accessed
	Mdouble get_disptorsion() const {return disptorsion;}
	
	///Allows the normal dissipation to be changed
	void set_dissipation(Mdouble new_disp){if(new_disp>=0) {useRestitution=false; disp=new_disp;} else { std::cerr << "Error in set_dissipation(" << new_disp << ")" << std::endl; exit(-1); }}
	///Allows the normal dissipation to be accessed
	Mdouble get_dissipation() const {return disp;}
	///Allows the normal dissipation to be changed
	void set_disp(Mdouble new_disp){set_dissipation(new_disp);}
	///Allows the normal dissipation to be accessed
	Mdouble get_disp() const {return get_dissipation();}


	///Allows the normal dissipation to be changed
	void set_restitution(Mdouble new_){if(new_>=0) {useRestitution=true; logRestitution=log(new_);} else { std::cerr << "Error in set_dissipation(" << new_ << ")" << std::endl; exit(-1); }
		//std::cout << "restitution " << exp(logRestitution) << std::endl;
}
	///Allows the normal dissipation to be accessed
	Mdouble get_logRestitution() const {return logRestitution;}
	///Allows the normal dissipation to be accessed
	Mdouble get_useRestitution() const {return useRestitution;}

	void update_disp(Mdouble mass1, Mdouble mass2) {
		Mdouble reducedMass = mass1*mass2/(mass1+mass2);
		///\todo		check for rest. =0
		disp = - sqrt( 4.0*reducedMass*k/(sqr(constants::pi)+sqr(logRestitution)) ) * logRestitution;
		//std::cout << "disp " << disp << " reducedMass " << mass1 << " " << mass2 << std::endl;
	}

	///Allows the (dynamic) Coulomb friction coefficient to be changed; also sets mu_s by default
	//mu has to be set to allow tangential forces (sets dispt=disp as default)
	void set_mu(Mdouble new_mu){if (new_mu>=0) {mu = new_mu; mus = mu;} else { std::cerr << "Error in set_mu" << std::endl; exit(-1); }}
	///Allows the (dynamic) Coulomb friction coefficient to be accessed
	Mdouble get_mu() const {return mu;}
	///Allows the static Coulomb friction coefficient to be changed 
	void set_mus(Mdouble new_mu){if (new_mu>=0) {mus = new_mu;} else { std::cerr << "Error in set_mus" << std::endl; exit(-1); }}
	///Allows the static Coulomb friction coefficient to be accessed
	Mdouble get_mus() const {return mus;}

	///Allows the (dynamic) Coulomb friction coefficient to be changed; also sets murolling_s by default
	//murolling has to be set to allow tangential forces (sets dispt=disp as default)
	void set_murolling(Mdouble new_murolling){if (new_murolling>=0) {murolling = new_murolling; musrolling = murolling;} else { std::cerr << "Error in set_murolling" << std::endl; exit(-1); }}
	///Allows the (dynamic) Coulomb friction coefficient to be accessed
	Mdouble get_murolling() const {return murolling;}
	///Allows the static Coulomb friction coefficient to be changed 
	void set_musrolling(Mdouble new_mu){if (new_mu>=0) {musrolling = new_mu;} else { std::cerr << "Error in set_musrolling" << std::endl; exit(-1); }}
	///Allows the static Coulomb friction coefficient to be accessed
	Mdouble get_musrolling() const {return musrolling;}

	///Allows the (dynamic) Coulomb friction coefficient to be changed; also sets mu_s by default
	//mu has to be set to allow tangential forces (sets dispt=disp as default)
	void set_mutorsion(Mdouble new_mu){if (new_mu>=0) {mutorsion = new_mu; mustorsion = mutorsion;} else { std::cerr << "Error in set_mutorsion" << std::endl; exit(-1); }}
	///Allows the (dynamic) Coulomb friction coefficient to be accessed
	Mdouble get_mutorsion() const {return mutorsion;}
	///Allows the static Coulomb friction coefficient to be changed 
	void set_mustorsion(Mdouble new_mu){if (new_mu>=0) {mustorsion = new_mu;} else { std::cerr << "Error in set_mustorsion" << std::endl; exit(-1); }}
	///Allows the static Coulomb friction coefficient to be accessed
	Mdouble get_mustorsion() const {return mustorsion;}
	
	///Allows the dimension of the particle (f.e. for mass) to be changed
	void set_dim_particle(int new_dim){if (new_dim>=1 && new_dim<=3) dim_particle = new_dim; else { std::cerr << "Error in set_dim_particle" << std::endl; exit(-1); }}
	///Allows the dimension of the particle (f.e. for mass) to be accessed
	int get_dim_particle() const {return dim_particle;}
		
	///Writes all species data
	friend inline std::ostream& operator<<(std::ostream& os, const CSpecies &s) {
		os << "k " << s.k 
			<< " disp " << s.disp 
			<< " kt " << s.kt;
		if (s.krolling) os << " krolling " << s.krolling;
		if (s.ktorsion) os << " ktorsion " << s.ktorsion;
		os  << " dispt " << s.dispt;
		if (s.disprolling) os << " disprolling " << s.disprolling;
		if (s.disptorsion) os << " disptorsion " << s.disptorsion;
		os << " mu " << s.mu;
		//optional output
		if (s.mu!=s.mus) os << " mus " << s.mus;
		if (s.murolling) os << " murolling " << s.murolling;
		if (s.murolling!=s.musrolling) os << " musrolling " << s.musrolling;
		if (s.mutorsion) os << " mutorsion " << s.mutorsion;
		if (s.mutorsion!=s.mustorsion) os << " mustorsion " << s.mustorsion;
		if (s.depth) {
			os << " k2max " << s.k2max
            << " kc " << s.kc
            << " depth " << s.depth;
		}
		if (s.AdhesionForceType!=None) {
			os << " AdhesionForceType " << (unsigned int)s.AdhesionForceType
            << " k0 " << s.k0
            << " f0 " << s.f0;
		}
		os << " rho " << s.rho
			<< " dim_particle " << s.dim_particle;
		return os;
	}	
	
	///Reads all species data
	friend inline std::istream& operator>>(std::istream& is, CSpecies &s) {
 		is >> s.k >> s.disp >> s.kt >> s.krolling >> s.ktorsion >> s.dispt >> s.disprolling >> s.disptorsion >> s.mu >> s.mus >> s.murolling >> s.musrolling >> s.mutorsion >> s.mustorsion >> s.k2max >> s.kc >> s.depth >> s.rho >> s.dim_particle;
		//s.mus=s.mu;
		return is;
	}
	
	void read(std::istream& is)  { 
		std::string dummy;
		is	>> dummy >> k 
			>> dummy >> disp 
			>> dummy >> kt >> dummy;
		if (!strcmp(dummy.c_str(),"krolling")) is >> krolling >> dummy; else krolling = 0;
		if (!strcmp(dummy.c_str(),"ktorsion")) is >> ktorsion >> dummy; else ktorsion = 0;
		is	>> dispt >> dummy;
		if (!strcmp(dummy.c_str(),"disprolling")) is >> disprolling >> dummy; else disprolling = 0;
		if (!strcmp(dummy.c_str(),"disptorsion")) is >> disptorsion >> dummy; else disptorsion = 0;
		is	>> mu >> dummy;
		if (!strcmp(dummy.c_str(),"mus")) is >> mus >> dummy; else mus=mu;
		if (!strcmp(dummy.c_str(),"murolling"))  is >> murolling  >> dummy; else murolling  = 0;
		if (!strcmp(dummy.c_str(),"musrolling")) is >> musrolling >> dummy; else musrolling = murolling;
		if (!strcmp(dummy.c_str(),"mutorsion"))  is >> mutorsion  >> dummy; else mutorsion  = 0;
		if (!strcmp(dummy.c_str(),"mustorsion")) is >> mustorsion >> dummy; else mustorsion = mutorsion;

		// checks if plastic model parameters are included (for backward compability)
		if (!strcmp(dummy.c_str(),"k2max")) {
			is >> k2max
            >> dummy >> kc
            >> dummy >> depth
            >> dummy;
		}
		if (!strcmp(dummy.c_str(),"AdhesionForceType")) {
			unsigned int AFT;
            is >> AFT;
            AdhesionForceType = (AdhesionForceTypes)AFT;
            is >> dummy >> k0
            >> dummy >> f0
            >> dummy;
		}
		is 	>> rho
			>> dummy >> dim_particle;
		//read rest of line
		getline(is,dummy);
	}

	///Outputs species
	void print(std::ostream& os) {
		os 	<< "k:" << k 
			<< ", disp:" << disp 
			<< ", kt:" << kt;
		if (krolling) os << ", krolling: " << krolling;
		if (ktorsion) os << ", ktorsion: " << ktorsion;
		os  << ", dispt: " << dispt;
		if (disprolling) os << ", disprolling: " << disprolling;
		if (disptorsion) os << ", disptorsion: " << disptorsion;
		os << ", mu: " << mu;
		if (mu!=mus) os << ", mus: " << mus;
		if (murolling) os << ", murolling: " << murolling;
		if (murolling!=musrolling) os << ", musrolling: " << musrolling;
		if (mutorsion) os << ", mutorsion: " << mutorsion;
		if (mutorsion!=mustorsion) os << ", mustorsion: " << mustorsion;
		os	<< ", k2max:" << k2max 
			<< ", kc:" << kc 
			<< ", depth:" << depth;
		if (dim_particle) {
			os	<< ", rho:" << rho 
				<< ", dim_particle:" << dim_particle;
		} else {
			os	<< " (mixed)";
		}
	}
	
	///Calculates collision time for two copies of a particle of given disp, k, mass
	Mdouble get_collision_time(Mdouble mass){
		if(mass<=0) 	{std::cerr << "Error in get_collision_time(Mdouble mass) mass is not set or has an unexpected value, (get_collision_time("<<mass<<"))"<< std::endl; exit(-1);}
		if(k<=0) 		{std::cerr << "Error in get_collision_time(Mdouble mass) stiffness is not set or has an unexpected value, (get_collision_time("<<mass<<"), with stiffness="<<k<<")"<< std::endl; exit(-1);}
		if(disp<0) 		{std::cerr << "Error in get_collision_time(Mdouble mass) dissipation is not set or has an unexpected value, (get_collision_time("<<mass<<"), with dissipation="<<disp<<")"<< std::endl; exit(-1);}
		Mdouble tosqrt=k/(.5*mass) - sqr(disp/mass);
		//~ std::cout<<"tosqrt "<<tosqrt<<" 1e "<<k/(.5*mass)<<" 2e "<<sqr(disp/mass)<<std::endl;
		if (tosqrt<=0) 	{std::cerr << "Error in get_collision_time(Mdouble mass) values for mass, stiffness and dissipation would leads to an over damped system, (get_collision_time("<<mass<<"), with stiffness="<<k<<" and dissipation="<<disp<<")"<< std::endl; exit(-1);}
		return constants::pi / sqrt( tosqrt );
	}
	
	///Calculates restitution coefficient for two copies of given disp, k, mass
	Mdouble get_restitution_coefficient(Mdouble mass){return exp(-disp/mass*get_collision_time(mass));}
	
	///Calculates the maximum velocity allowed for a collision of two copies of P (for higher velocities particles could pass through each other)
	Mdouble get_maximum_velocity(Mdouble radius, Mdouble mass){return radius * sqrt(k/(.5*mass));}
	
	///Sets k, disp such that it matches a given tc and eps for a collision of two copies of P
	void set_k_and_restitution_coefficient(Mdouble k_, Mdouble eps, Mdouble mass){
		k = k_;
		///\todo check all instances of constants::sqrt_pi
		disp = - sqrt(2.0*mass*k/(constants::sqr_pi+sqr(log(eps)))) * log(eps);
	}
	
	///Sets k, disp such that it matches a given tc and eps for a collision of two copies of equal mass m
	void set_collision_time_and_restitution_coefficient(Mdouble tc, Mdouble eps, Mdouble mass){
		disp = - mass / tc * log(eps);
		k = .5 * mass * (sqr(constants::pi/tc) + sqr(disp/mass));
	}
	
	///Set k, disp such that is matches a given tc and eps for a collision of two different masses. 
	///Recall the resitution constant is a function of k, disp and the mass of each particle in the collision
	/// See also set_collision_time_and_restitution_coefficient(Mdouble tc, Mdouble eps, Mdouble mass)
	void set_collision_time_and_restitution_coefficient(Mdouble tc, Mdouble eps, Mdouble mass1, Mdouble mass2)
		{
			Mdouble reduced_mass = mass1*mass2/(mass1+mass2);
			set_collision_time_and_restitution_coefficient(tc,eps,2.0*reduced_mass);
		}	

	///Sets k, disp, kt, dispt such that it matches a given tc and eps for a collision of two particles of masses m0,m1
	void set_collision_time_and_normal_and_tangential_restitution_coefficient(Mdouble tc, Mdouble eps, Mdouble beta, Mdouble mass){
		set_collision_time_and_restitution_coefficient(tc,eps,mass);
		//from Deen...Kuipers2006, eq. 43 and 30
		set_kt(2.0/7.0*get_k()*(sqr(constants::pi)+sqr(log(beta)))/(sqr(constants::pi)+sqr(log(eps))));
		if (beta) set_dispt(-2*log(beta)*sqrt(1.0/7.0*mass*get_kt()/(sqr(constants::pi)+sqr(log(beta)))));
		else set_dispt(2.*sqrt(1.0/7.0*mass*get_kt()));
	}

	///Sets k, disp, kt, dispt such that it matches a given tc and eps for a collision of two particles of masses m0,m1
	void set_collision_time_and_normal_and_tangential_restitution_coefficient_nodispt(Mdouble tc, Mdouble eps, Mdouble beta, Mdouble mass){
		set_collision_time_and_restitution_coefficient(tc,eps,mass);
		//from BeckerSchwagerPoeschel2008, eq. 56
		set_kt(2.0/7.0*get_k()*sqr(acos(-beta)/constants::pi));
		set_dispt(0);
	}
	
	///Sets k, disp, kt, dispt such that it matches a given tc and eps for a collision of two particles of equal mass m
	void set_collision_time_and_normal_and_tangential_restitution_coefficient(Mdouble tc, Mdouble eps, Mdouble beta, Mdouble mass1, Mdouble mass2)
		{
			Mdouble reduced_mass = mass1*mass2/(mass1+mass2);
			set_collision_time_and_normal_and_tangential_restitution_coefficient(tc,eps,beta,2.0*reduced_mass);
		}	
	
	///Sets k, disp, kt, dispt such that it matches a given tc and eps for a collision of two particles of equal mass m
	void set_collision_time_and_normal_and_tangential_restitution_coefficient_nodispt(Mdouble tc, Mdouble eps, Mdouble beta, Mdouble mass1, Mdouble mass2)
		{
			Mdouble reduced_mass = mass1*mass2/(mass1+mass2);
			set_collision_time_and_normal_and_tangential_restitution_coefficient_nodispt(tc,eps,beta,2.0*reduced_mass);
		}	
	
	Mdouble get_average(Mdouble a, Mdouble b) {
		return (a+b) ? (2.*(a*b)/(a+b)) : 0;
	}
		
	///create values for mixed species
	void mix(CSpecies& S0, CSpecies& S1){
		k = get_average(S0.get_k(),S1.get_k());
		kt = get_average(S0.get_kt(),S1.get_kt());
		krolling = get_average(S0.get_krolling(),S1.get_krolling());
		ktorsion = get_average(S0.get_ktorsion(),S1.get_ktorsion());
		disp = get_average(S0.get_dissipation(),S1.get_dissipation());
		dispt = get_average(S0.get_dispt(),S1.get_dispt());
		disprolling = get_average(S0.get_disprolling(),S1.get_disprolling());
		disptorsion = get_average(S0.get_disptorsion(),S1.get_disptorsion());
		mu = get_average(S0.get_mu(),S1.get_mu());
		mus = get_average(S0.get_mus(),S1.get_mus());
		murolling = get_average(S0.get_murolling(),S1.get_murolling());
		musrolling = get_average(S0.get_musrolling(),S1.get_musrolling());
		mutorsion = get_average(S0.get_mutorsion(),S1.get_mutorsion());
		mustorsion = get_average(S0.get_mustorsion(),S1.get_mustorsion());
		k2max = get_average(S0.get_k2max(),S1.get_k2max());
		kc = get_average(S0.get_kc(),S1.get_kc());
		depth = get_average(S0.get_depth(),S1.get_depth());
		rho = 0; 
		dim_particle = 0; //this will be used to distinguish a mixed species
	}
	
	
	Mdouble k; //(normal) spring constant
	Mdouble kt;
	Mdouble krolling;
	Mdouble ktorsion; //tangential spring constant
	Mdouble disp; //(normal) viscosity
	Mdouble dispt; //tangential viscosity: should satisfy 4*dispt*dt<mass, dispt \approx disp
	Mdouble disprolling; //tangential viscosity: should satisfy 4*dispt*dt<mass, dispt \approx disp
	Mdouble disptorsion; //tangential viscosity: should satisfy 4*dispt*dt<mass, dispt \approx disp
	Mdouble mu;//< (dynamic) Coulomb friction coefficient
	Mdouble mus; //static Coulomb friction coefficient (by default set equal to mu)
	Mdouble murolling;//< (dynamic) Coulomb friction coefficient
	Mdouble musrolling; //static Coulomb friction coefficient 
	Mdouble mutorsion;//< (dynamic) Coulomb friction coefficient
	Mdouble mustorsion; //static Coulomb friction coefficient 
	Mdouble rho; //density
	int dim_particle; //determines if 2D or 3D volume is used for mass calculations
	std::vector<CSpecies> MixedSpecies;

public:
	
	//access function to MD::Hertzian
	ForceTypes get_ForceType() const {return ForceType;}
	void set_ForceType(ForceTypes new_){ForceType=new_;
		if (ForceType==Hertzian) std::cout << "Hertzian law activated" << std::endl;
		else if (ForceType==HMD) std::cout << "Hertz-Mindlin-Deresiewicz law activated" << std::endl;
		else if (ForceType==HM) std::cout << "Hertz-Mindlin law activated" << std::endl;
		else if (ForceType==Linear) std::cout << "Linear law activated" << std::endl;
	}

	AdhesionForceTypes get_AdhesionForceType() const 
	{
		return AdhesionForceType;
	}

	void set_AdhesionForceType(AdhesionForceTypes new_)
	{
		AdhesionForceType=new_;
		if (AdhesionForceType==None) 
			std::cout << "No adhesion activated" << std::endl;
		else if (AdhesionForceType==LinearReversible) 
			std::cout << "Linear reversible adhesion activated" << std::endl;
		else if (AdhesionForceType==LinearIrreversible) 
			std::cout << "Linear irreversible adhesion activated" << std::endl;
		else 
		{
			std::cerr << "Error: AdhesionForceType unknown" << std::endl;
			exit (-1);
		}
	}

	void set_AdhesionForceType(char new_[])
	{
		if (!strcmp(new_,"None"))
			set_AdhesionForceType(None);
		else if (!strcmp(new_,"LinearReversible"))
			set_AdhesionForceType(LinearReversible);
		else if (!strcmp(new_,"LinearIrreversible"))
			set_AdhesionForceType(LinearIrreversible);
		else 
		{
			std::cerr << "Error: AdhesionForceType unknown" << std::endl;
			exit (-1);
		}
	}


	///Acccess functions for the plastic model
	void set_plastic_k1_k2max_kc_depth(Mdouble k1_, Mdouble k2max_, Mdouble kc_, Mdouble depth_){
		if (k1_<=0 || k2max_<k1_ || kc_<0 || depth_<0 || depth_>1) {
			std::cerr << "Error: arguments of set_plastic_k1_k2max_kc_depth do not make sense" << std::endl;
			exit(-1);
		}
		set_k1(k1_);
		set_k2max(k2max_);
		set_kc(kc_);
		set_depth(depth_);
	}	
	
	Mdouble get_k1() const {return k;}
	Mdouble get_k2max() const {return k2max;}
	Mdouble get_kc() const {return kc;}
	Mdouble get_depth() const {return depth;}
	void set_k1(Mdouble new_) {k = new_;}
	void set_k2max(Mdouble new_) {k2max = new_;}
	void set_kc(Mdouble new_) {kc = new_;}
	void set_depth(Mdouble new_) {depth = new_;}
	///Calculates collision time for stiffest spring constant, divides by 50
	Mdouble get_plastic_dt(Mdouble mass){
		return 0.02 * constants::pi / sqrt( k2max/(.5*mass) - sqr(disp/mass) );
	}

	Mdouble k2max; //<for plastic deformations; the maximum elastic constant
	Mdouble kc;	  //<for plastic deformations; the adhesive spring constant
	Mdouble depth; //<for plastic deformations; the depth (relative to the normalized radius) at which kpmax is used
	Mdouble logRestitution; //restitution coefficient (only used if useRestitution=true
	bool useRestitution; //false if dissipation is set instead of the restitution coefficient 
	//the constant restitution coefficient was first implemented in: http://www.astro.umd.edu/~dcr/reprints/schwartz_gm14,363.pdf
	ForceTypes ForceType; //<make force law Hertzian
	AdhesionForceTypes AdhesionForceType; //<determines which adhesion force model is used
	Mdouble k0; //<stiffness of linear adhesion force
	Mdouble f0; //<adhesion force at zero overlap
};

#endif
