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

#include <math.h>

#include "DPMBase.h"
#include "Particles/BaseParticle.h"
#include "Species/LinearViscoelasticSpecies.h"
#include "Math/ExtendedMath.h"

/*!
 * Class to test if a particle can manually be turned around on the unit circle.
 * At the moment, the particle fist must be fixed before it can manipulated 
 * manually. This should be changed, since that is counterintuitive.
 */
class DrivenParticleClass : public DPMBase
{
public:
    void setupInitialConditions()
    {
        speciesHandler.copyAndAddObject(LinearViscoelasticSpecies());
        
        BaseParticle p0;
        //At the moment, it is still necessary to fix the particle before we can
        //move it manually.
        p0.fixParticle();
        p0.setPrescribedPosition([](double time)
        {
            return(Vec3D(cos(time), 
                         0.0, 
                         sin(time)));
        });
        particleHandler.copyAndAddObject(p0);
        
        setTimeStep(0.01);
        setTimeMax(2 * constants::pi);
        
        setXMax(1.0);
        setYMax(1.0);
        setZMax(1.0);
    }
    
    void actionsAfterTimeStep()
    {
        Vec3D pos = particleHandler.getObject(0)->getPosition();
        //check if the particle is still on the unit circle in the (x,z)-plane
        if (std::abs(pos.getLength() - 1) > 1e-6 )
            logger(FATAL, "The particle is not on the unit circle anymore!");
        if (std::abs(pos.X) - 1 > 1e-6)
            logger(FATAL, "The particle exceeded the bounds in x direction");
        if (std::abs(pos.Y) > 1e-6)
            logger(FATAL, "The particle exceeded the bounds in y direction");        
        if (std::abs(pos.Z) - 1 > 1e-6 )
            logger(FATAL, "The particle exceeded the bounds in z direction");
        //check if any of the entries in pos equals nan or -nan
        if (pos.X != pos.X || pos.Y != pos.Y || pos.Z != pos.Z)
            logger(FATAL, "The position of the particle contains an entry that is not a number.");
    }
};

int main(int argc, char *argv[])
{
    // Problem setup
    DrivenParticleClass problem;
    problem.setName("DrivenParticleUnitTest");
    problem.setFileType(FileType::NO_FILE);
    problem.solve();
    
    return 0;
}
