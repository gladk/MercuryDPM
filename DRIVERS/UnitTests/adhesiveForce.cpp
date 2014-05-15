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

#include "MD.h"
#include <iostream>
#include "Particles/DeltaMaxsParticle.h"

///interactionRadius.cpp is written to test short-distance non-contact 
///forces such as van-der-Waals or liquid bridge forces. 
///A simple reversible adhesion force is combined with a linear-elastic 
///contact force.
class ppInteraction : public MD{

public:
	void setup_particles_initial_conditions()
	{
        relVelocity = 1e-1;
        set_name("adhesiveForcePP");

		set_dim(3);
		set_dim_particle(3);
		set_gravity(Vec3D(0,0,0));
		
	   	set_rho(2000.0);
	    set_k(1e2);
	    Species[0].set_k0(1e2);
	    Species[0].set_f0(1e-5*Species[0].get_k0());
	    Species[0].set_AdhesionForceType(LinearIrreversible);
	    Mdouble R = 1e-3;
		set_kt(1.2e1);
	    set_mu(0.01);
	    //set_disp(2e-3);
	    //set_dispt(0.5e-3);
	    set_dt(5e-6/2.0);

	    set_xmax( 2*R);
		set_ymax(   R);
		set_zmax(   R);
		set_xmin(-2*R);
		set_ymin(-R);
		set_zmin(-R);
		
        getParticleHandler().clear();
		TangentialSpringParticle P;
		P.set_Radius(R);

		P.set_Position(Vec3D(-R-Species[0].get_InteractionDistance()/3,0,0));
		P.set_Velocity(Vec3D(relVelocity/2,0,0));
		getParticleHandler().copyAndAddObject(P);
		P.set_Position(Vec3D(R+Species[0].get_InteractionDistance()/3,0,0));
		P.set_Velocity(Vec3D(-relVelocity/2,0,0));
		getParticleHandler().copyAndAddObject(P);
				
		set_tmax(get_dt()*250*2);
		set_savecount(1);
	}
public:
	Mdouble relVelocity;
};

class ppInteractionPlastic : public ppInteraction{
    
	void compute_internal_forces(BaseParticle *PI, BaseParticle *PJ) {
		compute_plastic_internal_forces(PI, PJ);
	}

    void setup_particles_initial_conditions()
	{
        ppInteraction::setup_particles_initial_conditions();
        Mdouble R = 1e-3;
	    set_k2max(1.5*get_k());
	    set_kc(get_k());
	    set_depth(0.05);

        getParticleHandler().clear();
		DeltaMaxsParticle P;
		P.set_Radius(R);
        
		P.set_Position(Vec3D(-R-Species[0].get_InteractionDistance()/3,0,0));
		P.set_Velocity(Vec3D(relVelocity/2,0,0));
		getParticleHandler().copyAndAddObject(P);
		P.set_Position(Vec3D(R+Species[0].get_InteractionDistance()/3,0,0));
		P.set_Velocity(Vec3D(-relVelocity/2,0,0));
		getParticleHandler().copyAndAddObject(P);
        set_name("adhesiveForcePPPlastic");
        set_tmax(get_dt()*250*5);

    }
};


class pwInteraction : public MD{
    
public:
	void setup_particles_initial_conditions()
	{
        relVelocity = 1e-1;
        set_name("adhesiveForcePW");
        
		set_dim(3);
		set_dim_particle(3);
		set_gravity(Vec3D(0,0,0));
		
	   	set_rho(2000.0);
	    set_k(1e2);
	    Species[0].set_k0(1e2);
	    Species[0].set_f0(1e-5*Species[0].get_k0());
	    Species[0].set_AdhesionForceType(LinearIrreversible);
	    Mdouble R = 1e-3;
		set_kt(1.2e1);
	    set_mu(0.01);
	    //set_disp(2e-3);
	    //set_dispt(0.5e-3);
	    set_dt(5e-6/2.0);
        
	    set_xmax( 2*R);
		set_ymax(   R);
		set_zmax(   R);
		set_xmin(0);
		set_ymin(-R);
		set_zmin(-R);
		
        getParticleHandler().clear();
		TangentialSpringParticle P;
		P.set_Radius(R);
		P.set_Position(Vec3D(R+Species[0].get_InteractionDistance()*1/3,0,0));
		P.set_Velocity(Vec3D(-relVelocity/2,0,0));
		getParticleHandler().copyAndAddObject(P);
		
   		getWallHandler().clear();
		InfiniteWall w;
		w.set(Vec3D(-1, 0, 0), -get_xmin());
		getWallHandler().copyAndAddWall(w);
     
		set_tmax(get_dt()*250*4);
		set_savecount(1);
	}
public:
	Mdouble relVelocity;
};


class abhi : public MD{
    
public:
	void setup_particles_initial_conditions()
	{
        set_name("abhi");
        
		set_dim(3);
		set_dim_particle(3);
		set_gravity(Vec3D(0,0,0));
		
	   	set_rho(2.0);
	    set_k(1e4);
	    Species[0].set_k0(1e4);
	    Species[0].set_f0(1e-2*Species[0].get_k0());
	    Species[0].set_AdhesionForceType(LinearReversible);
	    
	    set_xmax(10);
		set_ymax(10);
		set_zmax(10);
		set_xmin(0);
		set_ymin(0);
		set_zmin(0);
		
        getParticleHandler().clear();
		TangentialSpringParticle P;
		P.set_Radius(1);
        
		P.set_Position(Vec3D(2.989,5,5));
		P.set_Velocity(Vec3D(1,0,0));
		getParticleHandler().copyAndAddObject(P);
		P.set_Position(Vec3D(5.011,5,5));
		P.set_Velocity(Vec3D(-1,0,0));
		getParticleHandler().copyAndAddObject(P);
        
	    set_dt(1e-4);
		set_tmax(0.12);
		set_savecount(1);
	}
public:
	Mdouble relVelocity;
};


int main(int argc UNUSED, char *argv[] UNUSED)
{	
	ppInteraction pp;
	pp.solve();
    
	ppInteractionPlastic ppp;
	ppp.solve();
    
	pwInteraction pw;
	pw.solve();

	abhi a;
	a.solve();
}
// now plot
// p 'interactionRadius.fstat' u 7:($9/100) every 4::5 w lp