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

class particle_particle_collision : public MD{
public:
	
	virtual void setup_particles_initial_conditions()
	{
		static int count = -1;
		count++;
		
		// only do this for the first time
		if (!count) { 
			
			set_dim(2);
			set_dim_particle(3);
			set_rho(2000.0);
			
			unsigned int Nparticle = 2;		
			normal = Vec3D(-1.0,0.0,0.0);
			tangent = Vec3D(0.0,1.0,0.0);
			
			TangentialSpringParticle P0;
			for (unsigned int j=0; j<Nparticle; j++){
				P0.set_Radius(random.get_RN(0.0005,0.001));
				P0.compute_particle_mass(Species);
				getParticleHandler().copyAndAddObject(P0);
			}

			//set random relative velocity
			vcn0 = random.get_RN(-0.1,-0.05);
			vct0 = random.get_RN( 0.05, 0.1);
		
			tc = random.get_RN(0.0,1.0e-5);
			en = random.get_RN(0.5,1.0);	
				
			set_xmax(0.005);
			set_ymax(0.005);
			set_zmax(0.005);
		
			m12 = (getParticleHandler().getObject(0)->get_Mass()*getParticleHandler().getObject(1)->get_Mass())/(getParticleHandler().getObject(0)->get_Mass()+getParticleHandler().getObject(1)->get_Mass());
			set_collision_time_and_restitution_coefficient(tc,en,2.0*m12);
	    set_tmax(2.0*tc);
			set_dt(tc/200);
			set_savecount(1);
		}
		
		// do this for all solves
		getParticleHandler().getObject(0)->set_Velocity(normal*vcn0+tangent*vct0);
		getParticleHandler().getObject(1)->set_Velocity(Vec3D(0.0,0.0,0.0));
		getParticleHandler().getObject(0)->set_AngularVelocity(Vec3D(0.0,0.0,0.0));
		getParticleHandler().getObject(1)->set_AngularVelocity(Vec3D(0.0,0.0,0.0));
		
		getParticleHandler().getObject(0)->set_Position(Vec3D(0.0025-getParticleHandler().getObject(0)->get_Radius(),0.0025,0.0025));
		getParticleHandler().getObject(1)->set_Position(Vec3D(0.0025+getParticleHandler().getObject(1)->get_Radius(),0.0025,0.0025));
		getParticleHandler().getObject(0)->set_Angle(Vec3D(0.0,0.0,0.0));
		getParticleHandler().getObject(1)->set_Angle(Vec3D(0.0,0.0,0.0));
		
        TangentialSpringParticle* TSParticle0= dynamic_cast<TangentialSpringParticle*>(getParticleHandler().getObject(0));
		TSParticle0->get_TangentialSprings().reset();
        TangentialSpringParticle* TSParticle1= dynamic_cast<TangentialSpringParticle*>(getParticleHandler().getObject(1));
		TSParticle1->get_TangentialSprings().reset();
	}
	
	Vec3D get_vc() {
		return getParticleHandler().getObject(0)->get_Velocity() - getParticleHandler().getObject(1)->get_Velocity() + Cross(normal,getParticleHandler().getObject(0)->get_AngularVelocity()*getParticleHandler().getObject(0)->get_Radius() + getParticleHandler().getObject(1)->get_AngularVelocity()*getParticleHandler().getObject(1)->get_Radius());
	}
	
	void get_vc_components(Mdouble& vcn, Mdouble& vct) {
		Vec3D vc = get_vc();
		vcn = vc.X / normal.X;
		vct = vc.Y / tangent.Y;
	}
	
	///Calculates collision time for two copies of a particle of species 0
	Mdouble get_collision_time(Mdouble mass){
		return Species[0].get_collision_time(mass);
	}
	
	
	Mdouble vcn0, vct0;
	Vec3D normal, tangent;
	Mdouble tc, en;
	Mdouble m12;
};

class wall_particle_collision : public particle_particle_collision{
public:
	
	void setup_particles_initial_conditions()
	{
		static int count = -1;
		count++;
		
		// only do this for the first time
		if (!count) { 
			set_dim(2);
			set_dim_particle(3);
			set_rho(2000.0);
			
			
			InfiniteWall w0;
			w0.set(Vec3D(1, 0, 0), 0.0025);
			getWallHandler().copyAndAddWall(w0);
			
			TangentialSpringParticle P0;
			getParticleHandler().copyAndAddObject(P0);			
			set_xmax(0.005);
			set_ymax(0.005);
			set_zmax(0.005);
			
			//set random masses

			getParticleHandler().getObject(0)->set_Radius(random.get_RN(0.0005,0.001));
			getParticleHandler().getObject(0)->compute_particle_mass(Species);
			
			//set random relative velocity
			normal = Vec3D(-1.0,0.0,0.0);
			tangent = Vec3D(0.0,1.0,0.0);
			vcn0 = random.get_RN(-0.1,-0.05);
			vct0 = random.get_RN( 0.05, 0.1);
			
			tc = random.get_RN(0.0,1.0e-5);
			en = random.get_RN(0.5,1.0);
			m12 = getParticleHandler().getObject(0)->get_Mass(); // wall counts as an infinite mass
			set_collision_time_and_restitution_coefficient(tc,en,2.0*m12);
			set_tmax(2.0*tc);
			set_dt(tc/200);
			set_savecount(1);
		}
		
		// do this for all solves
		getParticleHandler().getObject(0)->set_Velocity(normal*vcn0+tangent*vct0);
		getParticleHandler().getObject(0)->set_AngularVelocity(Vec3D(0.0,0.0,0.0));
		
		getParticleHandler().getObject(0)->set_Position(Vec3D(0.0025-getParticleHandler().getObject(0)->get_Radius(),0.0025,0.0025));
		getParticleHandler().getObject(0)->set_Angle(Vec3D(0.0,0.0,0.0));
		
        TangentialSpringParticle* TSParticle0= dynamic_cast<TangentialSpringParticle*>(getParticleHandler().getObject(0));
		TSParticle0->get_TangentialSprings().reset();
	}
	
	Vec3D get_vc() {
		return getParticleHandler().getObject(0)->get_Velocity() + Cross(normal,getParticleHandler().getObject(0)->get_AngularVelocity()*getParticleHandler().getObject(0)->get_Radius());
	}
	void get_vc_components(Mdouble& vcn, Mdouble& vct) {
		Vec3D vc = get_vc();
		vcn = vc.X / normal.X;
		vct = vc.Y / tangent.Y;
	}
	
	void print_ts() {
		TangentialSpringParticle* TSParticle0= dynamic_cast<TangentialSpringParticle*>(getParticleHandler().getObject(0));
        TSParticle0->get_TangentialSprings().print(std::cout,-2);
		std::cout << std::endl;
	}
	
	
	
};

void particle_particle_test()
{
	std::cout << std::endl << "testing particle-particle collisions ..." << std::endl << std::endl;
	
	
	
	particle_particle_collision problem;

	problem.random.set_RandomSeed(5);

	problem.setup_particles_initial_conditions();
	Mdouble vcn, vct, vct_analytic;
	
	std::cout << "5: without tangential forces" << std::endl;
	problem.set_name("force_test5");
	problem.solve();
	problem.get_vc_components(vcn, vct);
	std::cout << "vct: analytic=" << problem.vct0 << ", simulation=" << vct << std::endl;
	std::cout << "vcn: analytic:" << -problem.en*problem.vcn0 << ", simulation=" << vcn << std::endl;
	
	//problem.set_append_to_files(true);
	
	std::cout << "6: with Coulomb friction" << std::endl;
	Mdouble mu = problem.random.get_RN(0.0,1.0);
	problem.set_mu(mu);
	problem.set_dispt(1e20);
	problem.set_kt(0.0);
	problem.set_name("force_test6");
	problem.solve();
	problem.get_vc_components(vcn, vct);
	vct_analytic = std::max(0.0,problem.vct0 + mu*3.5*(1+problem.en)*problem.vcn0);
	std::cout << "vct: analytic=" << vct_analytic << ", simulation=" << vct << std::endl;
	std::cout << "vcn: analytic:" << -problem.en*problem.vcn0 << ", simulation=" << vcn << std::endl;
	
	std::cout << "7: with Coulomb friction, spring activated" << std::endl;
	problem.set_kt(1.0);
	//problem.set_dispt(1);
	problem.set_name("force_test7");
	problem.solve();
	std::cout << "vct: analytic=" << vct_analytic << ", simulation=" << vct << std::endl;
	std::cout << "vcn: analytic:" << -problem.en*problem.vcn0 << ", simulation=" << vcn << std::endl;
	
	std::cout << "8: with tangential viscous force" << std::endl;
	Mdouble et = problem.random.get_RN(-1.0,0.0);
	problem.set_mu(1e20);
	problem.set_dispt(-log(-et)/(2.0*problem.tc) /3.5 * 2.0 * problem.m12);
	problem.set_kt(0.0);
	problem.set_name("force_test8");
	problem.solve();
	problem.get_vc_components(vcn, vct);
	vct_analytic = problem.vct0 * exp(-2.0*3.5*problem.get_dispt()/(2.0*problem.m12) * problem.get_collision_time(2.0*problem.m12));
	std::cout << "vct: analytic=" << vct_analytic << ", simulation=" << vct << std::endl;
	std::cout << "vcn: analytic:" << -problem.en*problem.vcn0 << ", simulation=" << vcn << std::endl;
	
	std::cout << "9: with tangential elastic force" << std::endl;
	Mdouble et2 = problem.random.get_RN(0.0,1.0);
	problem.set_mu(1e20);
	problem.set_dispt(0.0);
	problem.set_kt(problem.get_k()/3.5*sqr(acos(-et2)/constants::pi));
	problem.set_name("force_test9");
	problem.solve();
	problem.get_vc_components(vcn, vct);
	vct_analytic = problem.vct0 * cos(sqrt(problem.get_kt()/problem.m12*3.5)* problem.get_collision_time(2.0*problem.m12));
	std::cout << "vct: analytic=" << vct_analytic << ", simulation=" << vct << std::endl;
	std::cout << "vcn: analytic:" << -problem.en*problem.vcn0 << ", simulation=" << vcn << std::endl;

}

void wall_particle_test()
{
	std::cout << std::endl << "testing wall-particle collisions ..." << std::endl << std::endl;
	
	srand(5);
	
	wall_particle_collision problem;
	problem.setup_particles_initial_conditions();
	Mdouble vcn, vct, vct_analytic;
	
	//problem.set_append_to_files(true);
	
	std::cout << "without tangential forces" << std::endl;
	problem.set_name("force_test0");
	problem.solve();
	
	problem.get_vc_components(vcn, vct);
	std::cout << "vct: analytic=" << problem.vct0 << ", simulation=" << vct << std::endl;
	std::cout << "vcn: analytic:" << -problem.en*problem.vcn0 << ", simulation=" << vcn << std::endl;
	
	std::cout << "1: with Coulomb friction" << std::endl;
	Mdouble mu = problem.random.get_RN(0.0,1.0);
	problem.set_mu(mu);
	problem.set_dispt(1e20);
	problem.set_kt(0.0);
	problem.set_name("force_test1");
	problem.solve();
	problem.get_vc_components(vcn, vct);
	vct_analytic = std::max(0.0,problem.vct0 + mu*3.5*(1+problem.en)*problem.vcn0);
	std::cout << "vct: analytic=" << vct_analytic << ", simulation=" << vct << std::endl;
	std::cout << "vcn: analytic:" << -problem.en*problem.vcn0 << ", simulation=" << vcn << std::endl;
	
	std::cout << "2: with Coulomb friction, spring activated" << std::endl;
	problem.set_kt(1.0);
	problem.set_name("force_test2");
	problem.solve();
	std::cout << "vct: analytic=" << vct_analytic << ", simulation=" << vct << std::endl;
	std::cout << "vcn: analytic:" << -problem.en*problem.vcn0 << ", simulation=" << vcn << std::endl;
	
	std::cout << "3: with tangential viscous force" << std::endl;
	Mdouble et = problem.random.get_RN(-1.0,0.0);
	problem.set_mu(1e20);
	problem.set_dispt(-log(-et)/(2.0*problem.tc) /3.5 * 2.0 * problem.m12);
	problem.set_kt(0.0);
	problem.set_name("force_test3");
	problem.solve();
	problem.get_vc_components(vcn, vct);
	vct_analytic = problem.vct0 * exp(-2.0*3.5*problem.get_dispt()/(2.0*problem.m12) * problem.get_collision_time(2.0*problem.m12));
	std::cout << "vct: analytic=" << vct_analytic << ", simulation=" << vct << std::endl;
	std::cout << "vcn: analytic:" << -problem.en*problem.vcn0 << ", simulation=" << vcn << std::endl;
	
	std::cout << "4: with tangential elastic force" << std::endl;
	Mdouble et2 = problem.random.get_RN(0.0,1.0);
	problem.set_mu(1e20);
	problem.set_dispt(0.0);
	problem.set_kt(problem.get_k()/3.5*sqr(acos(-et2)/constants::pi));
	problem.set_name("force_test4");
	problem.solve();
	problem.get_vc_components(vcn, vct);
	vct_analytic = problem.vct0 * cos(sqrt(problem.get_kt()/problem.m12*3.5)* problem.get_collision_time(2.0*problem.m12));
	std::cout << "vct: analytic=" << vct_analytic << ", simulation=" << vct << std::endl;
	std::cout << "vcn: analytic:" << -problem.en*problem.vcn0 << ", simulation=" << vcn << std::endl;
	/*problem.print_ts();*/
	
/*	std::cout << std::endl
		<< "initial values: vcn0 " << problem.vcn0
		<< ", vct0 " << problem.vct0
		<< ", normal " << problem.normal
		<< ", tangent " << problem.tangent
		<< ", tc " << problem.tc
		<< ", en " << problem.en
		<< ", m12 " << problem.m12
		<< ", mu " << mu
		<< ", et " << et
		<< ", et2 " << et2
		<< std::endl;*/
}

///todo{What does this function? It is commented out because it gives warnings and is never used (alteast I think (Dinant))}
/*void test_tangentialsprings() {
	CTangentialSprings ts;
	int P;
	int W;
	Mdouble time=0, dtime=.1;
	Mdouble tnew=time+dtime;
	
	ts.reset();
	ts.print(std::cout,time);
	
	
	Vec3D* spring = ts.select_particle(P,time,dtime);
	spring->X = 1.0;
	ts.print(std::cout,time);
	std::cout << "spring=" << *spring << std::endl << std::endl;
	
	Vec3D* spring2 = ts.select_wall(W,time,dtime);
	spring2->X = 2.0;
	ts.print(std::cout,time);
	std::cout << "spring=" << *spring2 << std::endl << std::endl;
	
	Vec3D* spring3 = ts.select_particle(P,tnew,dtime);
	ts.print(std::cout,time);
	std::cout << "spring=" << *spring3 << std::endl << std::endl;
	
	Vec3D* spring4 = ts.select_particle(P,tnew+dtime,dtime);
	ts.print(std::cout,time);
	std::cout << "spring=" << *spring4 << std::endl << std::endl;
}*/


int main(int argc UNUSED, char *argv[] UNUSED) {
	
	std::cout << std::endl << "note: analytic values are only asymptotically correct as delta->0" << std::endl;
	particle_particle_test();
	wall_particle_test();
	
	return 0;
}

