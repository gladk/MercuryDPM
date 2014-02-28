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

#include "../../src/HGRID_3D.h"
#include "../../src/MD.h"

class MD_demo : public MD{
public:
	
	void setup_particles_initial_conditions()
	{
		if(getParticleHandler().getNumberOfObjects()!=N)
        {
            getParticleHandler().clear();
            getBoundaryHandler().clear();
            getWallHandler().clear();
            
            double VP=constants::pi*4.0/3.0;
            L=pow(N*VP*DistInt(1,omega)/nu,1.0/3.0);

            set_xmin(0);
            set_ymin(0);
            set_zmin(0);
            set_xmax(L);
            set_ymax(L);
            set_zmax(L);

            PeriodicBoundary b0;
            b0.set(Vec3D(1,0,0), get_xmin(), get_xmax());
            getBoundaryHandler().copyAndAddObject(b0);
            b0.set(Vec3D(0,1,0), get_ymin(), get_ymax());
            getBoundaryHandler().copyAndAddObject(b0);        
            b0.set(Vec3D(0,0,1), get_zmin(), get_zmax());
            getBoundaryHandler().copyAndAddObject(b0);
            
            getParticleHandler().setStorageCapacity(2*N);
            BaseParticle p0;
            p0.set_Velocity(Vec3D(0.0,0.0,0.0));

            double V=0;
            
            //Use at least particles with maximum and minimum size
            p0.set_Radius(1.0);
            Vec3D position;
            position.Z = random.get_RN(0,get_zmax());
            position.Y = random.get_RN(0,get_ymax());
            position.X = random.get_RN(0,get_xmax());
            p0.set_Position(position);
            getParticleHandler().copyAndAddObject(p0);
            V+=VP*pow(p0.get_Radius(),3);
            
            p0.set_Radius(omega);
            position.Z = random.get_RN(0,get_zmax());
            position.Y = random.get_RN(0,get_ymax());
            position.X = random.get_RN(0,get_xmax());
            p0.set_Position(position);
            getParticleHandler().copyAndAddObject(p0);
            V+=VP*pow(p0.get_Radius(),3);

            //For other particles use a random distribution
            for(int i=2;i<N;i++)
            {
                p0.set_Radius(RandomRadius());
                position.Z = random.get_RN(0,get_zmax());
                position.Y = random.get_RN(0,get_ymax());
                position.X = random.get_RN(0,get_xmax());
                p0.set_Position(position);
                getParticleHandler().copyAndAddObject(p0);
                V+=VP*pow(p0.get_Radius(),3);
            }
        }
	}
    
    double RandomRadius()
    {
        double rand=random.get_RN(0,1);
        if(alpha==-1)
        {
            return pow(omega,rand);
        }
        else
        {
            return pow(rand*(pow(omega,1.0+alpha)-1.0)+1.0,1.0/(1.0+alpha));
        }
    }
    
    double DistInt(double s, double e)
    {
        if(omega==1)
        {
            return 1;
        }
        double teller;
        double noemer;
        if(alpha==-1)
        {
            noemer=log(omega);
        }
        else
        {
            noemer=(pow(omega,1.0+alpha)-1.0)/(1.0+alpha);
        }
        
        if(alpha==-4)
        {
            teller=log(e)-log(s);
        }
        else
        {
            teller=(pow(e,4.0+alpha)-pow(s,4.0+alpha))/(4.0+alpha);
        }
        return teller/noemer;
    }

    double L;
    double omega;
    double alpha;
    double nu;
    int N;
};

class HGrid_demo : public HGRID_3D
{
  public:
    HGrid_demo(MD& other) : MD(other){}
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
	MD_demo MD_problem;
    
    MD_problem.omega=40;
    MD_problem.alpha=-2;
    MD_problem.nu=0.7;
    MD_problem.N=10000;
    MD_problem.set_dim(3);
    MD_problem.set_dim_particle(3);
	MD_problem.set_name("HGrid_demo_MD");
	MD_problem.set_dt(0.1);
    MD_problem.set_tmax(0.09);
    MD_problem.set_savecount(1);
    MD_problem.setup_particles_initial_conditions();
        
    HGrid_demo HGrid_problem1(MD_problem);
    HGrid_problem1.setHGridMethod(TOPDOWN);
    HGrid_problem1.setHGridMaxLevels(3);
    HGrid_problem1.setHGridDistribution(EXPONENTIAL);
    HGrid_problem1.set_name("HGrid_demo_HGrid1");

    HGrid_demo HGrid_problem2(MD_problem);
    HGrid_problem2.setHGridMethod(BOTTOMUP);
    HGrid_problem2.setHGridMaxLevels(8);
    HGrid_problem2.setHGridDistribution(LINEAR);
    HGrid_problem2.set_name("HGrid_demo_HGrid2");

	MD_problem.solve();    
    HGrid_problem1.solve();
    HGrid_problem2.solve();
}
