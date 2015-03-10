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

//based on /storage2/usr/people/sluding/MDCC/C3DshearXL30/MU0_LONG2
#ifndef MERCURY3DRESTART_H
#define MERCURY3DRESTART_H
#include "Mercury3D.h"
#include <sys/time.h>
#include <string.h>

/** This class adds to Mercury3D the ability to restart after a certain wall 
 * time. This was made necessary to run codes on clusters with wall-time limits.
 * For an example of use, see CSCRun.cpp.
 */
class Mercury3DRestart : public Mercury3D
{
public:

    Mercury3DRestart() //: Mercury3D()
    {
        initialWallTime_ = getWallTime();
        maxWallTime_ = 10;
        clusterCommand_ = "";
    }

    void setClusterCommand(std::string clusterCommand)
    {
        clusterCommand_ = clusterCommand;
    }

    void setMaxWallTime(Mdouble maxWallTime)
    {
        maxWallTime_ = maxWallTime;
    }

    Mdouble getMaxWallTime() const
    {
        return maxWallTime_;
    }

    Mdouble getInitialWallTime() const
    {
        return initialWallTime_;
    }

    /** the -r option is used to restart the code; this should probably be moved 
     * to DPMBase
     */
    int readNextArgument(int& i, int argc, char *argv[])
    {
        if (!strcmp(argv[i], "-restart") || !strcmp(argv[i], "-r"))
        {
            setName(argv[i + 1]);
            std::cout << "Reading file " << restartFile.getName() << std::endl;
            readRestartFile();
            std::cout << "tmax= " << getTimeMax() << std::endl;
            restartFile.getFstream().precision(18);
            setAppend(true);
            printTime();
        }
        else
        {
            return Mercury3D::readNextArgument(i, argc, argv);
        }
        return true;
    }

    /** Returns wall time (uses sys command, thus might not work on Windows)
     */
    double getWallTime() const
    {
        struct timeval time;
        if (gettimeofday(&time, NULL))
        {
            std::cerr << "Error in getWallTime: Wall time could not be read" << std::endl;
            return 0;
        }
        return (double) time.tv_sec + (double) time.tv_usec * .000001;
    }

private:

    /* writeOutputFiles is modified to force a restart when maxWallTime_ is 
     * reached. This is done by resetting the final simulation time to the 
     * current time.
     */
    void writeOutputFiles()
    {
        Mercury3D::writeOutputFiles();

        if (getWallTime() - initialWallTime_ > maxWallTime_)
        {
    		actionsAfterSolve();
    		finishStatistics();
    		closeFiles();
            std::cout << "Exiting for restarting after "
                << getWallTime() - initialWallTime_ << "s" << std::endl;
            
            //set the restart command
            std::stringstream com("");
            //check if filename contaion a dot
            //this is so the code works for autonumbered files
            std::size_t found = getName().find('.');
            if (found==std::string::npos) {
                com << clusterCommand_ << " ./" << getName() << " -r " << getName();
            } else {
                com << clusterCommand_ << " ./" << getName().substr (0,found) << " -r " << getName();
            }
            //std::cout << com << std::endl;        
            std::cout << "system output:" << system(com.str().c_str()) << std::endl;        
	        exit(0);
    	}
    }

    /** Command required to execute code on a cluster;
     * for einder, the command is ~/bin/sclusterscriptexecute
     */
    std::string clusterCommand_;
    
    /** Time in seconds after which the code is automatically restarted
     * for einder, this should be set to ~10*3600
     */
    double maxWallTime_;
    
    /** Internal variable (i.e. not to be set by the user); 
     * stores the wall time before solve is executed
     */
    double initialWallTime_;
    
};
#endif
