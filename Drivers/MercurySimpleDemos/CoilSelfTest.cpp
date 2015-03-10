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

#include "Mercury3D.h"
#include "Particles/BaseParticle.h"
#include "Walls/InfiniteWall.h"
#include "Walls/InfiniteWallWithHole.h"
#include "Walls/Coil.h"
#include "Species/LinearViscoelasticSpecies.h"
 
class CoilSelfTest : public Mercury3D{
	private:
	
	void setupInitialConditions()
	{
		setName("CoilSelfTest");
        setSystemDimensions(3);
        setGravity(Vec3D(0.0, -9.8, 0.0));
		setTimeStep(0.02*0.05);
		setTimeMax(2.0);
		setSaveCount(helpers::getSaveCountFromNumberOfSavesAndTimeMaxAndTimestep(1000, getTimeMax(), getTimeStep()));
		particleRadius=0.2;

		setXMax(1.0);
		setYMax(5.0);
		setZMax(2.0);
		setXMin(-1.0);
		setYMin(-1.0);

	    species = speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());
		species->setDensity(1000);
		species->setCollisionTimeAndRestitutionCoefficient(0.05,0.8, pow(particleRadius,3)*constants::pi*4.0/3.0*1000);

		frontWall = wallHandler.copyAndAddObject(InfiniteWall());
		frontWall->set(Vec3D(-1, 0, 0), -getXMin());
		backWall = wallHandler.copyAndAddObject(InfiniteWall());
		backWall->set(Vec3D( 1, 0, 0),  getXMax());
		bottomWall = wallHandler.copyAndAddObject(InfiniteWall());
		bottomWall->set(Vec3D( 0,-1, 0), -getYMin());
		topWall = wallHandler.copyAndAddObject(InfiniteWall());
		topWall->set(Vec3D( 0, 1, 0),  getYMax());
		leftWall = wallHandler.copyAndAddObject(InfiniteWall());
		leftWall->set(Vec3D( 0, 0,-1), -getZMin());
		
		rightWall = wallHandler.copyAndAddObject(InfiniteWallWithHole());
		rightWall->set(Vec3D(0,0,1),getZMax(),1.0);
		
		coil = wallHandler.copyAndAddObject(Coil());
        ///Here coil properties are set
		///Coil(Start position,Length,Radius,Number of turns,Rotation speed,Thickness)
		coil->set(Vec3D(0,0,0),1.0,1.0-particleRadius,2.0,-1.0,0.5*particleRadius);
		
		particleHandler.clear();
		BaseParticle p0;
		p0.setSpecies(species);
		p0.setVelocity(Vec3D(0.0,0.0,0.0));
		p0.setRadius(particleRadius);

        /*
        //Single test case
        double distance;
		Vec3D normal;
        p0.setPosition(Vec3D(1.0,0.0,0.0));
        if(coil->get_distance_and_normal(p0, distance, normal))
			std::cout<<"Collision, distance screw="<<distance<<std::endl;
		else
			std::cout<<"No collision, distance screw="<<distance<<std::endl;
        */

        //Simple run settings
        //Nx*Ny*Nz particles are created evenly spaced between [xMin_,xMax_]*[yMin_,yMax_]*[zMin_,zMax_] and checked for contact with the coil
		int Nx=static_cast<int>(std::floor((getXMax()-getXMin())/(2.1*particleRadius)));
		int Ny=static_cast<int>(std::floor((getYMax()-getYMin())/(2.1*particleRadius)));
		int Nz=static_cast<int>(std::floor((getZMax()-getZMin())/(2.1*particleRadius)));
		Mdouble distance;
		Vec3D normal;		
		for(int i=0;i<Nx;i++)
			for(int j=0;j<Ny;j++)
				for(int k=0;k<Nz;k++)
				{
					p0.setPosition(Vec3D(getXMin()+(getXMax()-getXMin())*(0.5+i)/Nx,getYMin()+(getYMax()-getYMin())*(0.5+j)/Ny,getZMin()+(getZMax()-getZMin())*(0.5+k)/Nz));
                    if(!coil->getDistanceAndNormal(p0, distance, normal))
                    {
						particleHandler.copyAndAddObject(p0);
                    }
                    else
                    {
                        //std::cout<<p0.getPosition()<<std::endl;
                    }
				}
	}
	
	void actionsBeforeTimeStep()
    {
	  if (getTime()>1)
			coil->move_time(getTimeStep());
			
	}
	
public:
    double particleRadius;
    LinearViscoelasticSpecies* species;
    Coil* coil;
    InfiniteWall* frontWall;
    InfiniteWall* backWall;
    InfiniteWall* topWall;
    InfiniteWall* bottomWall;
    InfiniteWall* leftWall;
    InfiniteWallWithHole* rightWall;
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
	CoilSelfTest problem;
	problem.solve();
}
