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

#include <iostream>

#include "Mercury2D.h"
#include "Boundaries/LeesEdwardsBoundary.h"
#include "Particles/BaseParticle.h"
#include "Species/LinearViscoelasticSpecies.h"

class LeesEdwardsSelfTest : public Mercury2D
{
public:
    
    void setupInitialConditions()
    {
        Mdouble tc=1e-3;
        Mdouble r=0.8;
        Mdouble rho=2000;
        Mdouble minRadius= 0.11415;
        Mdouble velocity=1.0;
        Mdouble minMass=constants::pi * minRadius * minRadius * rho;
        
        setName("LeesEdwardsSelfTest");
        setSystemDimensions(2);
        setGravity(Vec3D(0.0,0.0,0.0));
        setTimeMax(1);
        
        setTimeStep(0.02*tc);
        dataFile.setSaveCount(5000);
        statFile.setFileType(FileType::NO_FILE);
        eneFile.setFileType(FileType::NO_FILE);
        fStatFile.setFileType(FileType::NO_FILE);
        setHGridMaxLevels(1);
        
        setXMin(0.0);
        setYMin(0.0);
        setXMax(1.0);
        setYMax(1.0);

        species = speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());
        species->setDensity(rho);
        species->setCollisionTimeAndRestitutionCoefficient(tc, r, minMass);
        //species->setStiffness(helpers::computeKFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass (tc, r, 0.5*minMass));
        //species->setDissipation(helpers::computeDispFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass (tc, r, 0.5*minMass));
        
        leesEdwardsBoundary = boundaryHandler.copyAndAddObject(LeesEdwardsBoundary());
        leesEdwardsBoundary->set(
            [velocity] (double time) { return time*velocity; },
            [velocity] (double time UNUSED) { return velocity; },
            getXMin(),getXMax(),getYMin(),getYMax());
        
        BaseParticle* p;
        p = particleHandler.copyAndAddObject(BaseParticle());
        p->setRadius(0.20028);
        p->setPosition(Vec3D(0.10549,1,0.0));
        p = particleHandler.copyAndAddObject(BaseParticle());
        p->setRadius(0.13451);
        p->setPosition(Vec3D(0.075661,0.68566,0.0));
        p = particleHandler.copyAndAddObject(BaseParticle());
        p->setRadius(0.14352);
        p->setPosition(Vec3D(0.92579,0.28903,0.0));
        p = particleHandler.copyAndAddObject(BaseParticle());
        p->setRadius(0.18295);
        p->setPosition(Vec3D(0.38665,0.74146,0.0));
        p = particleHandler.copyAndAddObject(BaseParticle());
        p->setRadius(0.21927);
        p->setPosition(Vec3D(0.62318,0.39102,0.0));
        p = particleHandler.copyAndAddObject(BaseParticle());
        p->setRadius(0.19289);
        p->setPosition(Vec3D(0.76649,0.76213,0.0));
        p = particleHandler.copyAndAddObject(BaseParticle());
        p->setRadius(0.11415);
        p->setPosition(Vec3D(0.56207,0.97227,0.0));
        p = particleHandler.copyAndAddObject(BaseParticle());
        p->setRadius(0.20264);
        p->setPosition(Vec3D(0.22567,0.39866,0.0));
        p = particleHandler.copyAndAddObject(BaseParticle());
        p->setRadius(0.1269);
        p->setPosition(Vec3D(0.39448,0.13762,0.0));
        p = particleHandler.copyAndAddObject(BaseParticle());
        p->setRadius(0.13683);
        p->setPosition(Vec3D(0.77937,0.080942,0.0));
    }
    
private:
    LeesEdwardsBoundary* leesEdwardsBoundary;
    LinearViscoelasticSpecies* species;
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
    LeesEdwardsSelfTest leesEdwardsSelfTest;
    leesEdwardsSelfTest.solve();
}
