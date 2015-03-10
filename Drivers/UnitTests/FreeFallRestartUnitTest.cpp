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
#include <Logger.h>

class FreeFall : public DPMBase{
public:
	
	void setupInitialConditions()
	{
		InfiniteWall w0;
		w0.set(Vec3D(-1,0,0), Vec3D(getXMin(),0,0));
		wallHandler.copyAndAddObject(w0);
		w0.set(Vec3D( 1,0,0), Vec3D(getXMax(),0,0));
		wallHandler.copyAndAddObject(w0);
		w0.set(Vec3D(0,-1,0), Vec3D(0, getYMin(),0));
		wallHandler.copyAndAddObject(w0);
		w0.set(Vec3D(0, 1,0), Vec3D(0, getYMax(),0));
		wallHandler.copyAndAddObject(w0);
		
		BaseParticle p0;
		p0.setPosition(Vec3D(getXMax()/2,getYMax()/2,0.0));
		p0.setVelocity(Vec3D(0.0,0.0,0.0));
		p0.setRadius(0.0005);
		particleHandler.copyAndAddObject(p0);

	}
    
    void actionsOnRestart()
    {
        
        std::cout << "In this function you can add code that is run on restart" <<std::endl;
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
    FreeFallProblem.dataFile.getFstream().precision(10);
	FreeFallProblem.solve(argc,argv);
}

int main(int argc, char *argv[])
{
    
    //On first pass i.e. code being called as a selftest, it enters here and call the code again 3 times with different arguments.
	if (argc==1)
        {
            logger(INFO,"Case 1: not restarted");
		    if (system("./FreeFallRestartUnitTest -tmax 0.4 -name FreeFallRestartUnitTest_NoRestart"))
                       logger(FATAL, "code did not run");

            logger(INFO,"Case 2: restarted at t=0.2");
		    //restarted at t=0.2

		    if (system("./FreeFallRestartUnitTest -tmax 0.2 -name FreeFallRestartUnitTest_Restarted"))
                       logger(FATAL, "code did not run");
		
            if (system("./FreeFallRestartUnitTest -r FreeFallRestartUnitTest_Restarted -tmax 0.4"))
                       logger(FATAL,"code did not run");

		
            logger(INFO,"Case 3: restarted at t=0.2; using separate data files");


		    if (system("./FreeFallRestartUnitTest -tmax 0.2 -name FreeFallRestartUnitTest_SeparateFiles -fileTypeData 2"))
                       logger(FATAL,"code did not run");
		    if (system("./FreeFallRestartUnitTest -r FreeFallRestartUnitTest_SeparateFiles -tmax 0.4 "))
                       logger(FATAL,"code did not run");
	} else
	{
	    runFreeFall(argc, argv);
	    return 0;
	}
	
	//To compare the code using gnuplot follow the instructions below.
	//gnuplot> plot 'free_fall_restart/free_fall_restart.ene' u 1:3 w l, 'free_fall_restart/free_fall_no_restart.ene' u 1:3 w l
	//../sc/fpdiff.py ./free_fall_restart.fstat ./free_fall_no_restart.fstat
      
    //Final stage now we check what we get.
    logger(INFO,"Finished running, now comparing");
	
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
        
        if (!(*FreeFallProblemReloadIt)->getPosition().isEqualTo((*FreeFallProblemReloadRestartIt)->getPosition(),1e-6))
        {
            logger(FATAL,"Particles is not in the same place after restart. Before it was % and now it is %.",(*FreeFallProblemReloadIt)->getPosition(),(*FreeFallProblemReloadRestartIt)->getPosition());
        }
        if (!(*FreeFallProblemReloadIt)->getPosition().isEqualTo((*FreeFallProblemReloadSplitFilesIt)->getPosition(), 1e-10))
        {
             logger(FATAL,"Particles velocities are not the same place. Before it was % and now it is %.",(*FreeFallProblemReloadIt)->getVelocity(),(*FreeFallProblemReloadRestartIt)->getVelocity());
        }
        ++FreeFallProblemReloadRestartIt;
        ++FreeFallProblemReloadSplitFilesIt;
    }
    
    return 0;
}
