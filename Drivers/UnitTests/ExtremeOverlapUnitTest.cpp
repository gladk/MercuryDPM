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
#include "DPMBase.h"
#include "Particles/BaseParticle.h"
#include "Walls/InfiniteWall.h"
#include <iostream>
#include <Species/LinearViscoelasticSlidingFrictionSpecies.h>
#include <Logger.h>

/*!
 * \brief Makes sure that the behavior is still sensible if the overlap of two 
 * particles grows extremely large.
 * \details In this test, two particles are initialized such that the smaller 
 * particle is completely contained in the larger particle. Such an extreme 
 * overlap does not make physical sense, but we should make sure that the code 
 * does not do something extremely stupid. Therefore, we check that 
 * a) the normal force acts such that the two particle repel each other (if you 
 *    are not careful, you can create code where the normal force becomes 
 *    attractive, and the small particles moves towards the large particles center);
 * b) the forces still act equal and opposite (in particular the frictional 
 *    forces), so no energy is created. 
 */
class ExtremeOverlapUnitTest : public DPMBase{
public:
	
    /*!
     * defines the initial position and velocity of the two particles such that 
     * the smaller particle is completely contained inside the larger particle,
     * and that the small particle has a tangential velocity.
     */
	void setupInitialConditions() final
	{
        //Define the radii of the two particles
		Mdouble r0=0.01;
        Mdouble r1=0.001;

        //Define the bounding box (for viewing the output only; no walls are created)
		setYMax(2.0*(r0+r1));
		setYMin(0.0);
		setZMax(r0);
		setZMin(-r0);
		setXMax(r0);
		setXMin(-r0);

        //Create both particles and set their positions, radii, and velocities.
		BaseParticle p;
		p.setPosition(Vec3D(0.0,r0,0.0));
		p.setVelocity(Vec3D(0.0,0.0,0.0));
		p.setRadius(r0);
		particleHandler.copyAndAddObject(p);
		p.setPosition(Vec3D(0.0,1.5*r0,0.0));
		p.setVelocity(Vec3D(1.0,0.0,0));
		p.setRadius(r1);
		particleHandler.copyAndAddObject(p);
	}
    
//    /*!
//     * print the time, the position of both particles and the position of the 
//     * contact point to the screen. This allows you to plot these variables in 
//     * gnuplot; you can see that the contact point is not between the two 
//     * particles anymore.
//     */
//    void printTime() const final
//	{
//        if (interactionHandler.getNumberOfObjects()>0)
//        std::cout 
//        << getTime() << " "
//        << particleHandler.getObject(0)->getPosition().Z << " "
//        << particleHandler.getObject(1)->getPosition().Z << " "
//        << interactionHandler.getObject(0)->getContactPoint().Z <<std::endl;
//    }

};

int main(int argc, char *argv[])
{
	ExtremeOverlapUnitTest OverlapProblem;
    
    //set some contact parameters; the contact is elastic (so we can check 
    //energy conservation), and very soft (so the particles repel each other slowly))
    //To check Energy conservation, use gnuplot:
    //  p 'ExtremeOverlapUnitTest.ene' u 1:($2+$3+$4+$5)
    auto species = OverlapProblem.speciesHandler.copyAndAddObject(LinearViscoelasticSlidingFrictionSpecies());
    species->setDensity(2000);
    species->setStiffness(1e1);
    species->setDissipation(0.0);
    species->setSlidingFrictionCoefficient(1.0);
    species->setSlidingStiffness(2.0/7.0*species->getStiffness());
    species->setSlidingDissipation(2.0/7.0*species->getDissipation());
    
    OverlapProblem.setName("ExtremeOverlapUnitTest");
    OverlapProblem.setFileType(FileType::NO_FILE);
	OverlapProblem.dataFile.setFileType(FileType::ONE_FILE);
	OverlapProblem.setSaveCount(10000);
    OverlapProblem.setDimension(2);
    OverlapProblem.setTimeStep(1e-8);
	OverlapProblem.setTimeMax(0.01);
	OverlapProblem.solve(argc,argv);
    
    Vec3D position = OverlapProblem.particleHandler.getObject(0)->getPosition();
    Vec3D positionToCompare = Vec3D(-1.031389999146e-06, 0.009506389407855, 0);
    if (!position.isEqualTo(positionToCompare, 1e-10))
        logger(FATAL,"Large particle is in the wrong position. It is at % and should be %",position,positionToCompare);
    else
        std::cout << "Test passed" << std::endl;
    position = OverlapProblem.particleHandler.getObject(1)->getPosition();
    positionToCompare = Vec3D(0.01010314899993, 0.01487096023445, 0);
    if (!position.isEqualTo(positionToCompare, 1e-10))
        logger(FATAL,"Large particle is in the wrong position. It is at % and should be %",position,positionToCompare);
    else
        std::cout << "Test passed" << std::endl;
    //std::cout.precision(13); std::cout << position << std::endl;
    return 0;
}
