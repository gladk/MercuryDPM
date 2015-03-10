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

class SeparateFilesSelfTest : public DPMBase{
public:

	void setupInitialConditions()
	{
        setXMax(1.0);
        setYMax(1.0);
        setZMax(2.0);
        setSystemDimensions(3);
        setParticleDimensions(3);
        setGravity(Vec3D(0.0,0.0,0.0));

        LinearViscoelasticSpecies* species = speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());
        species->setDensity(constants::pi/6);
        species->setStiffness(200000);

        particleHandler.clear();
        BaseParticle* p = particleHandler.copyAndAddObject(BaseParticle());
        p->setPosition(Vec3D(0.5,0.5,0.5));
        p->setVelocity(Vec3D(0.0,0.0,0.0));
        p->setRadius(0.5);
        BaseParticle* q = particleHandler.copyAndAddObject(BaseParticle());
        q->setPosition(Vec3D(0.5,0.5,1.499));
        q->setVelocity(Vec3D(0.0,0.0,0.0));
        q->setRadius(0.5);

        setTimeStep(1e-5);
		setTimeMax(6e-3);
        setSaveCount(helpers::getSaveCountFromNumberOfSavesAndTimeMaxAndTimestep(2, getTimeMax(), getTimeStep()));
	}

};

int main(int argc UNUSED, char *argv[] UNUSED)
{
	SeparateFilesSelfTest problem;

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
