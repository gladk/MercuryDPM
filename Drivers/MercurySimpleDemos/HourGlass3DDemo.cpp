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

#include "Mercury3D.h"
#include "Particles/BaseParticle.h"
#include "Walls/IntersectionOfWalls.h"
#include "Walls/AxisymmetricIntersectionOfWalls.h"
#include <iostream>
#include "Species/LinearViscoelasticFrictionSpecies.h"

class HourGlass : public Mercury3D{
public:
    
    void setupInitialConditions()
    {
        AxisymmetricIntersectionOfWalls w1;
        w1.setPosition(Vec3D((getXMin()+getXMax())/2.0,(getYMin()+getYMax())/2.0,0));
        w1.setOrientation(Vec3D(0, 0, 1));
        w1.addObject(Vec3D(1,0,0),Vec3D((getXMax()-getXMin())/2.0,0,0));
        wallHandler.copyAndAddObject(w1);

        AxisymmetricIntersectionOfWalls w2;
        w2.setPosition(Vec3D((getXMin()+getXMax())/2.0,(getYMin()+getYMax())/2.0,0));
        w2.setOrientation(Vec3D(0, 0, 1));
        std::vector<Vec3D> Points(3);
		//left neck wall
        //define the corners of your prismatic wall in clockwise direction
        Points[0] = Vec3D((getXMax()-getXMin())/2.0                 ,0.0,(getZMin()+getZMax())/2.0+ContractionHeight);
        Points[1] = Vec3D((getXMax()-getXMin())/2.0-ContractionWidth,0.0,(getZMin()+getZMax())/2.0                  );
        Points[2] = Vec3D((getXMax()-getXMin())/2.0                 ,0.0,(getZMin()+getZMax())/2.0-ContractionHeight);
        w2.createOpenPrism(Points);
        wallHandler.copyAndAddObject(w2);

        InfiniteWall w0;
        w0.set(Vec3D(0,0,-1), Vec3D(0,0,0.5*(getZMin()+getZMax())));
        wallHandler.copyAndAddObject(w0);

        BaseParticle p0;
        p0.setVelocity(Vec3D(0.0,0.0,0.0));
        Mdouble Width = getXMax()-getXMin();
        for (double z=(getZMin()+getZMax())/2.0+ContractionHeight; particleHandler.getNumberOfObjects()<=N; z+=2.0*MaxParticleRadius) 
        for (double R=Width/2.0-MaxParticleRadius; R>0; R-=1.999*MaxParticleRadius) 
        for (double C=2.0*MaxParticleRadius; C<=2*constants::pi*R; C+=2.0*MaxParticleRadius) 
        {
			p0.setRadius(random.getRandomNumber(MinParticleRadius,MaxParticleRadius));
			//std::cout<<z<<" "<<R<<" "<<C<<" "<<std::endl;
            p0.setPosition(Vec3D((getXMin()+getXMax())/2.0+R*sin(C/R),(getYMin()+getYMax())/2.0+R*cos(C/R),z+p0.getRadius()));
			p0.setVelocity(1.0*Vec3D(random.getRandomNumber(-1,1),0.0,random.getRandomNumber(-1,0)));
            particleHandler.copyAndAddObject(p0);
        }
        
        //write(std::cout,false);
    }

	void actionsAfterTimeStep() 
	{
		if (getTime()<0.9 && getTime()+ getTimeStep()>0.9)
		{
			std::cout<<"Shifting bottom wall downward"<<std::endl;
			dynamic_cast<InfiniteWall*>(wallHandler.getLastObject())->set(Vec3D( 0, 0,-1), Vec3D(0, 0, getZMin()));
		}
	}
	
    Mdouble ContractionWidth;
    Mdouble ContractionHeight;
    Mdouble MinParticleRadius;
    Mdouble MaxParticleRadius;
    unsigned int N;
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
    HG.setName("HourGlass");
    auto species = HG.speciesHandler.copyAndAddObject(LinearViscoelasticFrictionSpecies());
    species->setDensity(2000);
    
    //specify geometry
    //specify dimensions of the hourglass
    Mdouble Width  = 10e-2; // 10cm
    Mdouble Height = 60e-2; // 60cm
    //specify how big the wedge of the contraction should be
    Mdouble ContractionWidth  = 2.5e-2; //2.5cm
    Mdouble ContractionHeight = 5e-2; //5cm
    //set domain accordingly (domain boundaries are not walls!)
    HG.setXMin(0.0);
    HG.setXMax(Width);
    HG.setYMin(0.0);
    HG.setYMax(Width);
    HG.setZMin(0.0);
    HG.setZMax(Height);
    //these parameters are needed in setupInitialConditions()
    HG.ContractionWidth = ContractionWidth;
    HG.ContractionHeight = ContractionHeight;

    //specify particle properties
    species->setDensity(2000.0);
    //these parameters are needed in setupInitialConditions()
    HG.MinParticleRadius =  6e-3; // 6mm
    HG.MaxParticleRadius = 10e-3; //10mm

    //specify body forces
    HG.setGravity( Vec3D(0.0, 0.0, -9.8) );

    //specify contact properties
    //normal forces
    species->setStiffness(1e5);
    species->setDissipation(0.63);
    //tangential (sliding) forces
    species->setSlidingFrictionCoefficient(0.5);
    species->setSlidingStiffness(1.2e4);
    species->setSlidingDissipation(0.16);
    //tangential (rolling) torques
    species->setRollingFrictionCoefficient(0.2);
    species->setRollingStiffness(1.2e4);
    species->setRollingDissipation(6.3e-2);
    //normal (torsion/spin) torques
    species->setTorsionFrictionCoefficient(0.1);
    species->setTorsionStiffness(1.2e4);
    species->setSlidingDissipation(6.3e-2);

    //test normal forces
    Mdouble MinParticleMass = species->getDensity()*4.0/3.0*constants::pi*mathsFunc::cubic(HG.MinParticleRadius);
   	//Calculates collision time for two copies of a particle of given dissipation_, k, effective mass
	std::cout << "MinParticleMass =" << MinParticleMass << std::endl;
   	//Calculates collision time for two copies of a particle of given dissipation_, k, effective mass
	Mdouble tc = species->getCollisionTime(MinParticleMass);
    std::cout << "tc  =" << tc << std::endl;
	//Calculates restitution coefficient for two copies of given dissipation_, k, effective mass
	Mdouble r = species->getRestitutionCoefficient(MinParticleMass);
    std::cout << "r   =" << r << std::endl;
	//Calculates the maximum relative velocity allowed for a normal collision of two particles of radius r and particle mass m (for higher velocities particles could pass through each other)
	//std::cout << "vmax=" << helpers::getMaximumVelocity(species->getStiffness(), HGgetSpecies(0)->getDissipation(), HG.MinParticleRadius, MinParticleMass) << std::endl;

	//set other simulation parameters
    HG.setTimeStep(tc / 50.0);
	HG.setTimeMax(10.0);
    HG.setSaveCount(500);
    HG.setXBallsAdditionalArguments("-v0 -solidf");
	HG.N=600; //number of particles

    HG.solve(argc, argv);
    return 0;
}

