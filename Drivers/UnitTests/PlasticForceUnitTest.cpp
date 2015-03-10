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

#include "DPMBase.h"
#include "Particles/BaseParticle.h"
#include <iostream>
#include <vector>
#include <Species/LinearPlasticViscoelasticSpecies.h>
#include "Logger.h"
extern Logger<LOG_MAIN_LEVEL> logger;


///This code tests our plastic force model, as published in Luding 2008.
class PlasticForceUnitTest : public DPMBase{
public:

    PlasticForceUnitTest()
    {
        species = speciesHandler.copyAndAddObject(LinearPlasticViscoelasticSpecies());
    }

	void setupInitialConditions()
	{
		setXMax(3.0);
		setYMax(2.0);
		setZMax(2.0);
		setGravity(Vec3D(0.0,0.0,0.0));
        species->setDensity(6.0/constants::pi);
		
		double depth = 0.05;
		double deltamax = (species->getUnloadingStiffnessMax()/(species->getUnloadingStiffnessMax()-species->getLoadingStiffness()))*depth*((2*0.5*0.5)/(0.5+0.5));
		double relVelocity=sqrt(species->getLoadingStiffness()*(chi*deltamax)*(chi*deltamax)*2);
        
        particleHandler.clear();
		
		BaseParticle P0,P1;
		P0.setPosition(Vec3D( 1.0,1.0,1.0));
		P0.setVelocity(Vec3D(relVelocity/2.0,0.0,0.0));
		P0.setRadius(0.5);
		P1.setPosition(Vec3D( 2.0,1.0,1.0));
		P1.setVelocity(Vec3D(-relVelocity/2.0,0.0,0.0));
		P1.setRadius(0.5);
		
		particleHandler.copyAndAddObject(P0);
		particleHandler.copyAndAddObject(P1);
				
		double mass=1.0;
        setTimeStep(species->computePlasticTimeStep(mass)*2.0);
		setTimeMax(getTimeStep()*100.0);
        setFileType(FileType::ONE_FILE);
        setSaveCount(1);
	}

    void set_chi(double new_){chi=new_;}
    double get_chi() {return chi;}

	double chi;
    LinearPlasticViscoelasticSpecies* species;
};

int main(int argc UNUSED, char *argv[] UNUSED)
{	
	PlasticForceUnitTest PlasticForceUnitTestProblem;
	double k1=100.0;
	PlasticForceUnitTestProblem.species->setPlasticParameters(k1, 5.0*k1, k1, 0.05);
    PlasticForceUnitTestProblem.species->setDissipation(0);
    PlasticForceUnitTestProblem.setParticleDimensions(3);
    PlasticForceUnitTestProblem.setSystemDimensions(3);

    logger.log(Log::INFO,"Testing particle particles collision for elastic plastic forces. \n"
        "This will be done for serveral values of scaled relative velocity chi");
    
    //PlasticForceUnitTestProblem.getRestartFile().getFstream().precision(20);
    
	//Set up constant data that will be used
    const std::vector<double> chi = {0.34, 0.69, 1.1, 1.37};
    const std::vector<Vec3D> leftFinalVecloity = {
        Vec3D(-0.032721738352012,0.0,0.0),
        Vec3D(-0.0138683231953154,0.0,0.0),
        Vec3D(-0.0204655358555405,0.0,0.0),
        Vec3D(-0.163049415300304,0.0,0.0)};
    const std::vector<Vec3D> leftFinalPosition = {
        Vec3D(0.995546292935715,1.0,1.0),
        Vec3D(1.00695193269955,1.0,1.0),
        Vec3D(1.00840467123501,1.0,1.0),
        Vec3D(0.969386085767181,1.0,1.0)};

    //Loop over all test cases
	for (int i=0; i<4; i++)
    {
        logger.log(Log::INFO, "Running for chi=%",chi[i]);
		PlasticForceUnitTestProblem.set_chi(chi[i]);
		std::stringstream ss("");
		ss << "PlasticForceUnitTest" << PlasticForceUnitTestProblem.get_chi();
		PlasticForceUnitTestProblem.setName(ss.str().c_str());
		PlasticForceUnitTestProblem.solve();
        PlasticForceUnitTestProblem.writeRestartFile();

        //Now check the particles are in the right place for each of the 4 cases
        auto pIt = PlasticForceUnitTestProblem.particleHandler.begin();
        if (!(*pIt)->getPosition().compareTo(leftFinalPosition[i], 1e-10))
                logger.log(Log::FATAL,"Left particle is in the wrong position. It is at % and should be %",(*pIt)->getPosition(),leftFinalPosition[i]);
        if (!(*pIt)->getVelocity().compareTo(leftFinalVecloity[i]  , 1e-10))
                logger.log(Log::FATAL,"Left particle has the wrong velocity. It is at % and should be %",(*pIt)->getVelocity(),leftFinalVecloity[i]);
    }

	//to display use
	//gnuplot> plot 'plastic/collinear1.1.fstat' u 7:9 w lp
}
