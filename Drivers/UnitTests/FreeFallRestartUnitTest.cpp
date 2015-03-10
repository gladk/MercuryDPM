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


///This code tests:
///1) Restarting
///2) Saving arcoss multiple files
///3) and accepting command line argument.
///4) Also tests restart reloading.

#include "DPMBase.h"
#include "Particles/BaseParticle.h"
#include "Walls/InfiniteWall.h"
#include <iostream>
#include <Species/LinearViscoelasticSpecies.h>
#include "Logger.h"
extern Logger<LOG_MAIN_LEVEL> logger;

class FreeFall : public DPMBase{
public:
	
	void setupInitialConditions()
	{
		InfiniteWall w0;
		w0.set(Vec3D(-1,0,0), -getXMin());
		wallHandler.copyAndAddObject(w0);
		w0.set(Vec3D( 1,0,0),  getXMax());
		wallHandler.copyAndAddObject(w0);
		w0.set(Vec3D(0,-1,0), -getYMin());
		wallHandler.copyAndAddObject(w0);
		w0.set(Vec3D(0, 1,0),  getYMax());
		wallHandler.copyAndAddObject(w0);
		
		BaseParticle p0;
		p0.setPosition(Vec3D(getXMax()/2,getYMax()/2,0.0));
		p0.setVelocity(Vec3D(0.0,0.0,0.0));
		p0.setRadius(0.0005);
		particleHandler.copyAndAddObject(p0);

	}

};


void runFreeFall(int argc, char *argv[])
{
	///Start off my solving the default problem
	FreeFall FreeFallProblem;
    auto species = FreeFallProblem.speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());
    species->setDensity(2000);
    FreeFallProblem.setName("FreeFallRestartUnitTest");
	FreeFallProblem.setSaveCount(10000);
    FreeFallProblem.setTimeStep(1e-6);
	FreeFallProblem.setYMax(0.1);
	FreeFallProblem.setXMax(0.01);
    FreeFallProblem.getDataFile().getFstream().precision(10);
	FreeFallProblem.solve(argc,argv);
}

int main(int argc, char *argv[])
{
    
    //On first pass i.e. code being called as a selftest, it enters here and call the code again 3 times with different arguments.
	if (argc==1)
        {
            logger.log(Log::INFO,"Case 1: not restarted");
		    if (system("./FreeFallRestartUnitTest -tmax 0.4 -name FreeFallRestartUnitTest_NoRestart"))
                       logger.log(Log::FATAL, "code did not run");

            logger.log(Log::INFO,"Case 2: restarted at t=0.2");
		    //restarted at t=0.2

		    if (system("./FreeFallRestartUnitTest -tmax 0.2 -name FreeFallRestartUnitTest_Restarted"))
                       logger.log(Log::FATAL, "code did not run");
		
            if (system("./FreeFallRestartUnitTest -r FreeFallRestartUnitTest_Restarted -tmax 0.4"))
                       logger.log(Log::FATAL,"code did not run");

		
            logger.log(Log::FATAL,"Case 3: restarted at t=0.2; using separate data files");


		    if (system("./FreeFallRestartUnitTest -tmax 0.2 -name FreeFallRestartUnitTest_SeparateFiles -options_data 2"))
                       logger.log(Log::FATAL,"code did not run");
		    if (system("./FreeFallRestartUnitTest -r FreeFallRestartUnitTest_SeparateFiles -tmax 0.4 "))
                       logger.log(Log::FATAL,"code did not run");
	} else
	{
	    runFreeFall(argc, argv);
	    return 0;
	}
	
	//To compare the code using gnuplot follow the instructions below.
	//gnuplot> plot 'free_fall_restart/free_fall_restart.ene' u 1:3 w l, 'free_fall_restart/free_fall_no_restart.ene' u 1:3 w l
	//../sc/fpdiff.py ./free_fall_restart.fstat ./free_fall_no_restart.fstat
      
    //Final stage now we check what we get.
    logger.log(Log::INFO,"Finished running, now comparing");
	
	FreeFall FreeFallProblemReload;
    FreeFall FreeFallProblemReloadRestart;
    FreeFall FreeFallProblemReloadSplitFiles;
    
    FreeFallProblemReload.readRestartFile("FreeFallRestartUnitTest_NoRestart.restart");
    FreeFallProblemReloadRestart.readRestartFile("FreeFallRestartUnitTest_Restarted.restart");
    FreeFallProblemReloadSplitFiles.readRestartFile("FreeFallRestartUnitTest_SeparateFiles.restart");

    auto FreeFallProblemReloadRestartIt = FreeFallProblemReloadRestart.particleHandler.begin();
    auto FreeFallProblemReloadSplitFilesIt = FreeFallProblemReloadSplitFiles.particleHandler.begin();
   
    for (auto FreeFallProblemReloadIt = FreeFallProblemReload.particleHandler.begin(); FreeFallProblemReloadIt != FreeFallProblemReload.particleHandler.end(); ++FreeFallProblemReloadIt)
    {
      //  if (!(*FreeFallProblemReloadIt))->getTimeMax()
        
        if (!(*FreeFallProblemReloadIt)->getPosition().compareTo((*FreeFallProblemReloadRestartIt)->getPosition(),1e-6))
        {
            logger.log(Log::FATAL,"Particles is not in the same place after restart. Before it was % and now it is %.",(*FreeFallProblemReloadIt)->getPosition(),(*FreeFallProblemReloadRestartIt)->getPosition());
        }
        if (!(*FreeFallProblemReloadIt)->getPosition().compareTo((*FreeFallProblemReloadSplitFilesIt)->getPosition(), 1e-10))
        {
             logger.log(Log::FATAL,"Particles velocities are not the same place. Before it was % and now it is %.",(*FreeFallProblemReloadIt)->getVelocity(),(*FreeFallProblemReloadRestartIt)->getVelocity());
        }
        ++FreeFallProblemReloadRestartIt;
        ++FreeFallProblemReloadSplitFilesIt;
    }
}
