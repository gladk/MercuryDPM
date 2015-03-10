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

#include "Species/LinearViscoelasticIrreversibleAdhesiveSpecies.h"
#include "Species/LinearViscoelasticReversibleAdhesiveSpecies.h"
#include "DPMBase.h"
#include "Logger.h"
#include "Species/LinearPlasticViscoelasticReversibleAdhesiveSpecies.h"

/*!
 * A simple test where two adhesive particles collide; two adhesive force models are tested here, 
 * LinearViscoelasticIrreversibleAdhesiveInteraction and LinearViscoelasticReversibleAdhesiveInteraction.
 * The focus of this test is to see if energy is conserved during the collision; for the elastic force model,
 * no energy should be lost, whereas the irreversible force model should loose a fixed amount of energy.   
 */
class EnergyUnitTest : public DPMBase
{
public:

    void setupInitialConditions()
    {
        setFileType(FileType::NO_FILE);
        //to see the behaviour of elastic and kinetic energy, comment the line above and plot in gnuplot
        //p 'ReversibleAdhesiveEnergyUnitTest.ene' u 1:3 w lp, 'ReversibleAdhesiveEnergyUnitTest.ene' u 1:5 w lp, 'ReversibleAdhesiveEnergyUnitTest.ene' u 1:($3+$5) w lp
        //p 'IrreversibleAdhesiveEnergyUnitTest.ene' u 1:3 w lp, 'IrreversibleAdhesiveEnergyUnitTest.ene' u 1:5 w lp, 'IrreversibleAdhesiveEnergyUnitTest.ene' u 1:($3+$5) w lp
        setDimension(3);

        setGravity({0.0, 0.0, 0.0});

        setXMax(1.0);
        setYMax(0.5);
        setZMax(0.5);
        setXMin(-1.0);
        setYMin(-0.5);
        setZMin(-0.5);

        particleHandler.clear();
        BaseParticle P;
        P.setRadius(0.5);
        P.setPosition({-0.551, 0.0, 0.0});
        P.setVelocity({0.7, 0.0, 0.0});
        particleHandler.copyAndAddObject(P);
        P.setPosition({0.551, 0.0, 0.0});
        P.setVelocity({-0.7, 0.0, 0.0});
        particleHandler.copyAndAddObject(P);
    }
};

int main(int argc UNUSED, char *argv[] UNUSED)
{
    std::cout << "Test if the LinearViscoelasticReversibleAdhesiveInteraction conserves energy" << std::endl;
    EnergyUnitTest energyUnitTest;
    energyUnitTest.setName("ReversibleAdhesiveEnergyUnitTest");
    LinearViscoelasticReversibleAdhesiveSpecies species1;
    species1.setDensity(6.0 / constants::pi);
    species1.setStiffness(500);
    species1.setAdhesionStiffness(100);
    species1.setAdhesionForceMax(10);
    //std::cout << "maxVelocity=" << species1.getMaximumVelocity(0.5, 1.0) << std::endl;
    energyUnitTest.setTimeStep(0.002 * species1.getCollisionTime(1.0));
    //std::cout << "timeStep=" << energyUnitTest.getTimeStep() << std::endl;
    energyUnitTest.setSaveCount(3);
    energyUnitTest.setTimeMax(0.4);
    energyUnitTest.speciesHandler.copyAndAddObject(species1);
    energyUnitTest.solve();
    Mdouble lostEnergy = mathsFunc::square(0.7)-energyUnitTest.getElasticEnergy()-energyUnitTest.getKineticEnergy();
    if (!mathsFunc::isEqual(lostEnergy, 0.0, 1e-6))
    {
        logger(ERROR, "energy loss is %, but should be %", lostEnergy, 0.0);
    }

    std::cout << "Test if the LinearViscoelasticIrreversibleAdhesiveInteraction looses the right amount of energy" << std::endl;
    EnergyUnitTest energyUnitTest2;
    energyUnitTest2.setName("IrreversibleAdhesiveEnergyUnitTest");
    LinearViscoelasticIrreversibleAdhesiveSpecies species2;
    species2.setDensity(6.0 / constants::pi);
    species2.setStiffness(500);
    species2.setAdhesionStiffness(100);
    species2.setAdhesionForceMax(10);
    std::cout << "maxVelocity=" << species2.getMaximumVelocity(0.5, 1.0) << std::endl;
    //this test is 10 times more accurate than the previous one, as the error is so much bigger.
    energyUnitTest2.setTimeStep(0.0002 * species2.getCollisionTime(1.0));
    std::cout << "timeStep=" << energyUnitTest2.getTimeStep() << std::endl;
    energyUnitTest2.setSaveCount(3);
    energyUnitTest2.setTimeMax(0.2);
    energyUnitTest2.speciesHandler.copyAndAddObject(species2);
    energyUnitTest2.solve();
    lostEnergy = mathsFunc::square(0.7)-energyUnitTest2.getElasticEnergy()-energyUnitTest2.getKineticEnergy();
    /*!
     * \todo TW The inaccuracy of the calculation is much worse for the 
     * irreversible force model, as there is a jump in force at zero overlap; 
     * we should correct for that in the time stepping algorithm 
     * (i.e. find out what the mean force was over the duration of the timestep,
     * not using a left Riemann sum.
     */
    if (!mathsFunc::isEqual(lostEnergy, 0.5, 1e-4))
    {
        logger(ERROR, "energy loss is %, but should be %", lostEnergy, 0.5);
    }

    return 0;
}
