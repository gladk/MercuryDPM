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

// Tutorial 10: This tutorial shows how to load restart and data files

#include "Mercury3D.h"
#include "Math/Helpers.h"

int main()
{
    //writeToFile is used here to create a restart and a data file, which will be loaded below.
    helpers::writeToFile("Tutorial10.ini.restart",
        "restart_version 1.0 name Tutorial10\n"
        "dataFile name Tutorial10.data fileType ONE_FILE saveCount 10 counter 0 nextSavedTimeStep 0\n"
        "fStatFile name Tutorial10.fstat fileType NO_FILE saveCount 10 counter 0 nextSavedTimeStep 0\n"
        "eneFile name Tutorial10.ene fileType ONE_FILE saveCount 10 counter 0 nextSavedTimeStep 0\n"
        "restartFile name Tutorial10.restart fileType ONE_FILE saveCount 10 counter 0 nextSavedTimeStep 0\n"
        "statFile name Tutorial10.stat fileType ONE_FILE saveCount 10 counter 0 nextSavedTimeStep 0\n"
        "xMin 0 xMax 2 yMin 0 yMax 2 zMin 0 zMax 2\n"
        "timeStep 1e-03 time 0 ntimeSteps 0 timeMax 10\n"
        "systemDimensions 3 particleDimensions 3 gravity 0 0 -1\n"
        "Species 1\n"
        "LinearViscoelasticSpecies id 0 density 1.9098593 stiffness 2000 dissipation 10\n"
        "Walls 1\n"
        "InfiniteWall id 0 indSpecies 0 position 0 0 0 orientation 0 0 0 1 velocity 0 0 0 angularVelocity 0 0 0 0 force 0 0 0 torque 0 0 0 normal 0 0 -1 factor 1\n"
        "WallsPeriodic 0\n"
        "Particles 0\n"
        "Interactions 0\n"
    );

    helpers::writeToFile("Tutorial10.ini.data",
        "1 0 0 0 0 2 2 2\n"
        "1 1 1.5  0 0 0  0.5  0 0 0  0 0 0  0\n"
    );

    DPMBase Tutorial10;
    //use readRestartFile to load information from a restart file
    Tutorial10.readRestartFile("Tutorial10.ini.restart");

    //use readDataFile to load information from a data file
    Tutorial10.readDataFile("Tutorial10.ini.data");

    //now start the calculations
    Tutorial10.solve();

    return 0;
}
