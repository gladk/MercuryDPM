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
#include "Walls/Coil.h" 
#include "Walls/CylindricalWall.h" 
#include "Walls/AxisymmetricIntersectionOfWalls.h" 
#include "Walls/IntersectionOfWalls.h" 
#include "Walls/InfiniteWall.h" 
#include "Walls/InfiniteWallWithHole.h" 
#include "Walls/Screw.h"

#include "Boundaries/AngledPeriodicBoundary.h"
#include "Boundaries/ChuteInsertionBoundary.h"
#include "Boundaries/CubeInsertionBoundary.h"
#include "Boundaries/HopperInsertionBoundary.h"
#include "Boundaries/PeriodicBoundary.h"
#include "Boundaries/CircularPeriodicBoundary.h"
#include "Boundaries/DeletionBoundary.h"

#include "Particles/BaseParticle.h"


class FullRestartTest : public DPMBase
{
public:
    
    void setupInitialConditions()
    {
        Coil coil(Vec3D(0.1, 0.2, 0.3), 0.4, 0.5, 0.6, 0.7, 0.8);
        
        CylindricalWall cylindricalWall;
        cylindricalWall.set(0.1);
        
        AxisymmetricIntersectionOfWalls axisymmetricIntersectionOfWalls;
        axisymmetricIntersectionOfWalls.setPosition(Vec3D(0.1,0.2,0.3));
        axisymmetricIntersectionOfWalls.setOrientation(Vec3D(0.4,0.5,0.6));
        axisymmetricIntersectionOfWalls.addObject(Vec3D(0.7,0.8,0.9),0.11);

        IntersectionOfWalls intersectionOfWalls;
        intersectionOfWalls.addObject(Vec3D(0.1, 0.2, 0.3), Vec3D(0.4, 0.5, 0.6));
        
        InfiniteWall infiniteWall;
        infiniteWall.set(Vec3D(0.1, 0.2, 0.3), 0.4);
        
        InfiniteWallWithHole infiniteWallWithHole;
        infiniteWallWithHole.set(Vec3D(0.1, 0.2, 0.3), 0.4, 0.5);
        
        Screw screw(Vec3D(0.1, 0.2, 0.3), 0.4, 0.5, 0.6, 0.7, 0.8);
        
        wallHandler.copyAndAddObject(coil);
        wallHandler.copyAndAddObject(cylindricalWall);
        wallHandler.copyAndAddObject(axisymmetricIntersectionOfWalls);
        wallHandler.copyAndAddObject(intersectionOfWalls);
        wallHandler.copyAndAddObject(infiniteWall);
        wallHandler.copyAndAddObject(infiniteWallWithHole);
        wallHandler.copyAndAddObject(screw);
        
        AngledPeriodicBoundary angledPeriodicBoundary;
        angledPeriodicBoundary.set(Vec3D(0.1, 0.2, 0.3), Vec3D(0.4, 0.5, 0.6), Vec3D(0.7, 0.8, 0.9));

        ChuteInsertionBoundary chuteInsertionBoundary;
        chuteInsertionBoundary.set(new BaseParticle, 13, Vec3D(0.1, 0.2, 0.3), Vec3D(0.4, 0.5, 0.6), 0.33, 0.44, 0.55, 1.1, 1.2);

        CubeInsertionBoundary cubeInsertionBoundary;
        cubeInsertionBoundary.set(new BaseParticle, 13, Vec3D(0.1, 0.2, 0.3), Vec3D(0.4, 0.5, 0.6), Vec3D(0.11, 0.22, 0.33), Vec3D(0.44, 0.55, 0.66), 0.77, 0.88);

        HopperInsertionBoundary hopperInsertionBoundary;
        hopperInsertionBoundary.set(new BaseParticle, 13, 0.2, 0.3, 0.66, 0.77, 3.1, 0.69, false, 3, 0.11, 0.21, 0.09, 2.31, 0.001, 30);

        PeriodicBoundary periodicBoundary;
        periodicBoundary.set(Vec3D(0.1, 0.2, 0.3), 1.1, 2.2, Vec3D(0.4, 0.5, 0.6));

        CircularPeriodicBoundary circularPeriodicBoundary(0.3);

        DeletionBoundary deletionBoundary;
        deletionBoundary.set(Vec3D(0.1, 0.2, 0.3), 3.14);

        boundaryHandler.copyAndAddObject(angledPeriodicBoundary);
        boundaryHandler.copyAndAddObject(chuteInsertionBoundary);
        boundaryHandler.copyAndAddObject(cubeInsertionBoundary);
        boundaryHandler.copyAndAddObject(hopperInsertionBoundary);
        boundaryHandler.copyAndAddObject(periodicBoundary);
        boundaryHandler.copyAndAddObject(circularPeriodicBoundary);
        boundaryHandler.copyAndAddObject(deletionBoundary);
        
        BaseParticle baseParticle;
        particleHandler.copyAndAddObject(baseParticle);
        
    }
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
    FullRestartTest normal;
    normal.setName("FullRestartTest_Normal");
    normal.setupInitialConditions();
    normal.writeRestartFile();
    
    FullRestartTest restart;
    restart.readRestartFile("FullRestartTest_Normal.restart");
    restart.setName("FullRestartTest_Restart");
    restart.writeRestartFile();
}
