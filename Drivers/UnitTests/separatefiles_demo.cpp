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
#include <Species/LinearViscoelasticSpecies.h>

class free_fall : public DPMBase{
public:

	void setupInitialConditions()
	{
		BaseParticle p0;
		
		//This code is solved three times, so you would get three copies of the particles without this check
		if (particleHandler.getNumberOfObjects() < 1)
			{
		
			p0.setPosition(Vec3D(getXMax()/2,getYMax()/2,0.0));
			p0.setVelocity(Vec3D(0.0,0.0,0.0));
			p0.setRadius(0.0005);
			particleHandler.copyAndAddObject(p0);
			
			}
			
		else
			{
				particleHandler.getObject(0)->setPosition(Vec3D(getXMax()/2,getYMax()/2,0.0));
				particleHandler.getObject(0)->setVelocity(Vec3D(0.0,0.0,0.0));
				
			}
			
		//endif


        setTimeStep(1e-6);
		setTimeMax(1e-3);
        setSaveCount(helpers::getSaveCountFromNumberOfSavesAndTimeMaxAndTimestep(2, getTimeMax(), getTimeStep()));
	}

};

int main(int argc UNUSED, char *argv[] UNUSED)
{
	free_fall problem;
    auto species = problem.speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());
    species->setDensity(2000);

	problem.setName("nofiles");
    problem.setFileType(FileType::NO_FILE);
	problem.solve();
	
	problem.setName("combinedfiles");
	problem.setFileType(FileType::ONE_FILE);
	problem.solve();
	
	problem.setName("separatefiles");
	problem.setFileType(FileType::MULTIPLE_FILES_PADDED);
	problem.solve();
	
}
