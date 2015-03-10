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

#include "../../src/HGRID_3D.h"
#include "../../src/FiniteWall.h"
#include "../../src/FiniteAxisSymmetricWall.h"
#include <iostream>

class HourGlass : public HGRID_3D{
public:
    
    void setup_particles_initial_conditions()
    {
        FiniteAxisSymmetricWall w1;
     	w1.setSymmetryAxis(Vec3D((get_xmin()+get_xmax())/2.0,(get_ymin()+get_ymax())/2.0,0),Vec3D(0,0,1));
        w1.add_finite_wall(Vec3D(1,0,0),Vec3D((get_xmax()-get_xmin())/2.0,0,0));
        getWallHandler().copyAndAddWall(w1);

        FiniteAxisSymmetricWall w2;
     	w2.setSymmetryAxis(Vec3D((get_xmin()+get_xmax())/2.0,(get_ymin()+get_ymax())/2.0,0),Vec3D(0,0,1));
        std::vector<Vec3D> Points(3);
		//left neck wall
        //define the corners of your prismatic wall in clockwise direction
        Points[0] = Vec3D((get_xmax()-get_xmin())/2.0                 ,0.0,(get_zmin()+get_zmax())/2.0+ContractionHeight);
        Points[1] = Vec3D((get_xmax()-get_xmin())/2.0-ContractionWidth,0.0,(get_zmin()+get_zmax())/2.0                  );
        Points[2] = Vec3D((get_xmax()-get_xmin())/2.0                 ,0.0,(get_zmin()+get_zmax())/2.0-ContractionHeight);
        w2.create_open_prism_wall(Points);
        getWallHandler().copyAndAddWall(w2);

        InfiniteWall w0;
        w0.set(Vec3D(0,0,-1), -0.5*(get_zmin()+get_zmax()));
        getWallHandler().copyAndAddWall(w0);

        TangentialSpringParticle p0;
        p0.set_Velocity(Vec3D(0.0,0.0,0.0));
        Mdouble Width = get_xmax()-get_xmin();
        for (double z=(get_zmin()+get_zmax())/2.0+ContractionHeight; getParticleHandler().getNumberOfObjects()<=N; z+=2.0*MaxParticleRadius) 
        for (double R=Width/2.0-MaxParticleRadius; R>0; R-=1.999*MaxParticleRadius) 
        for (double C=2.0*MaxParticleRadius; C<=2*constants::pi*R; C+=2.0*MaxParticleRadius) 
        {
			p0.set_Radius(random.get_RN(MinParticleRadius,MaxParticleRadius));
			//std::cout<<z<<" "<<R<<" "<<C<<" "<<std::endl;
            p0.set_Position(Vec3D((get_xmin()+get_xmax())/2.0+R*sin(C/R),(get_ymin()+get_ymax())/2.0+R*cos(C/R),z+p0.get_Radius()));
			p0.set_Velocity(1.0*Vec3D(random.get_RN(-1,1),0.0,random.get_RN(-1,0)));
            getParticleHandler().copyAndAddObject(p0);
        }
        
        //info();
    }

	void actions_after_time_step() 
	{
		if (get_t()<0.9 && get_t()+get_dt()>0.9)
		{
			std::cout<<"Shifting bottom wall downward"<<std::endl;
			((InfiniteWall*)getWallHandler().getLastWall())->set(Vec3D(0,0,-1), -get_zmin());
		}
	}
	
    Mdouble ContractionWidth;
    Mdouble ContractionHeight;
    Mdouble MinParticleRadius;
    Mdouble MaxParticleRadius;
    int N;
};

int main(int argc, char *argv[])
{
    std::cout<< "Hourglass Simulation" <<std::endl;
	// note: this code is based on stefan's implementation, see
	// /storage2/usr/people/sluding/COMPUTERS/hpc01/sluding/MDCC/MDCLR/DEMO/W7
	// however, it is scaled to SI units by the scaling factors
	// d=1e-3, m=1e-6, g=1

    //all parameters should be set in the main file
    //here, we use SI units (generally, other, scaled units are possible)
    
    //create an instance of the class and name it
    HourGlass HG;
    HG.set_name("HourGlass");
    
    //specify geometry
    //specify dimensions of the hourglass
    Mdouble Width  = 10e-2; // 10cm
    Mdouble Height = 60e-2; // 60cm
    //specify how big the wedge of the contraction should be
    Mdouble ContractionWidth  = 2.5e-2; //2.5cm
    Mdouble ContractionHeight = 5e-2; //5cm
    //set domain accordingly (domain boundaries are not walls!)
    HG.set_xmin(0.0);
    HG.set_xmax(Width);
    HG.set_ymin(0.0);
    HG.set_ymax(Width);
    HG.set_zmin(0.0);
    HG.set_zmax(Height);
    //these parameters are needed in setup_particles_initial_conditions()
    HG.ContractionWidth = ContractionWidth;
    HG.ContractionHeight = ContractionHeight;

    //specify particle properties
    HG.set_rho(2000.0);
    //these parameters are needed in setup_particles_initial_conditions()
    HG.MinParticleRadius =  6e-3; // 6mm
    HG.MaxParticleRadius = 10e-3; //10mm

    //specify body forces
    HG.set_gravity( Vec3D(0.0, 0.0, -9.8) );

    //specify contact properties
    //normal forces
    HG.set_k(1e5);
    HG.set_disp(0.63);
    //tangential (sliding) forces
    HG.set_mu(0.5);
    HG.set_kt(1.2e4);
    HG.set_dispt(0.16);
    //tangential (rolling) torques
    HG.set_murolling(0.2);
    HG.set_krolling(1.2e4);
    HG.set_disprolling(6.3e-2);
    //normal (torsion/spin) torques
    HG.set_mutorsion(0.1);
    HG.set_ktorsion(1.2e4);
    HG.set_disptorsion(6.3e-2);

    //test normal forces
    Mdouble MinParticleMass = HG.get_rho()*4.0/3.0*constants::pi*cubic(HG.MinParticleRadius);
   	//Calculates collision time for two copies of a particle of given disp, k, effective mass
	std::cout << "MinParticleMass =" << MinParticleMass << std::endl;
   	//Calculates collision time for two copies of a particle of given disp, k, effective mass
	Mdouble tc = helperFunc::getCollisionTime(HG.get_k(), HG.get_disp(), MinParticleMass/2.0);
	std::cout << "tc  =" << tc << std::endl;
	//Calculates restitution coefficient for two copies of given disp, k, effective mass
	Mdouble r = helperFunc::getRestitutionCoefficient(HG.get_k(), HG.get_disp(), MinParticleMass/2.0);
	std::cout << "r   =" << r << std::endl;
	//Calculates the maximum relative velocity allowed for a normal collision of two particles of radius r and particle mass m (for higher velocities particles could pass through each other)
	//std::cout << "vmax=" << helperFunc::getMaximumVelocity(HG.get_k(), HG.get_disp(), HG.MinParticleRadius, MinParticleMass) << std::endl;

	//set other simulation parameters
	HG.set_dt(tc/50.0);
	HG.set_tmax(10.0);
    HG.set_savecount(500);
    HG.set_xballs_additional_arguments("-v0 -solidf");
	HG.N=600; //number of particles

    HG.solve(argc, argv);
    return 0;
}

