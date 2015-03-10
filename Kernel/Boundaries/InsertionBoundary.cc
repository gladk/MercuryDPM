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

#include "InsertionBoundary.h"
#include "DPMBase.h"
#include "Particles/BaseParticle.h"

/*!
 * \details Default constructor, sets all data members to 0 or nullptr.
 */
InsertionBoundary::InsertionBoundary()
{   
    numberOfParticlesInserted_ = 0;
    particleToCopy_ = nullptr;
    maxFailed_ = 0;
}

/*!
 * \details Copy constructor
 */
InsertionBoundary::InsertionBoundary(const InsertionBoundary& other) 
{
    numberOfParticlesInserted_ = other.numberOfParticlesInserted_;
    maxFailed_ = other.maxFailed_;
    particleToCopy_ = other.particleToCopy_->copy();
}

/*!
 * \details Destructor that deletes the BaseParticle that is copied and inserted
 *          at every insertion.
 */
InsertionBoundary::~InsertionBoundary()
{
    delete particleToCopy_;
}

/*!
 * \details Sets the particle that will be inserted and the maximum number of 
 * times for which insertion may fail.
 * \param[in] particleToCopy  Particle that will be copied and inserted in the domain
 * \param[in] maxFailed       Number of times that the wall may fail to insert a particle
 */
void InsertionBoundary::set(BaseParticle* particleToCopy, unsigned int maxFailed)
{
    particleToCopy_ = particleToCopy->copy();
    maxFailed_ = maxFailed;
}

/*!
 * \details Is used to fill the insides of the boundary with particles until 
 * it is filled up. 
 * \param[in,out] md    the problem's DPMBase object
 * \todo rename to something like "insertUntilMaxFailed"?
 */
void InsertionBoundary::checkBoundaryBeforeTimeStep(DPMBase* md)
{
    unsigned int failed = 0;
    BaseParticle* p0;
    //try max_failed times to find new insertable particle
    while (failed <= maxFailed_)
    {
        p0 = generateParticle(md->random);
        //std::cout<<"Trying "<<*p0<<std::endl;
        
        if (md->checkParticleForInteraction(*p0))
        {
            md->particleHandler.copyAndAddObject(p0);
            failed = 0;
            ++numberOfParticlesInserted_;
            //std::cout<<"succes"<<std::endl;
        }
        else
        {
            failed++;
            //std::cout<<"failure"<<std::endl;
        }
        //Irana: got rid of the delete, since it is not made with new (particle on stack instead of heap)
        //delete p0;
    }
}

/*!
 * \details Returns the number of particles inserted in the boundary
 * \return  the number of particles inserted
 */
unsigned int InsertionBoundary::getNumberOfParticlesInserted() const
{
    return numberOfParticlesInserted_;
}

/*!
 * \details Sets the maximum number of times InsertionBoundary::checkBoundaryBeforeTimeStep()
 * may try to insert a particle and fail, before the insertion of particles stops.
 * \param[in] maxFailed     the maximum number of particle insertion trials
 */
void InsertionBoundary::setMaxFailed(unsigned int maxFailed)
{
    maxFailed_=maxFailed;
}

/*!
 * \details Return maxFailed_ (see InsertionBoundary::setMaxFailed).
 * \return the maximum number of particle insertion trials
 */
unsigned int InsertionBoundary::getMaxFailed() const
{
    return maxFailed_;
}

/*!
 * \details Sets the pointer to the particle, copies of which are inserted 
 * \param[in] particleToCopy    pointer to the particle to be inserted
 */
void InsertionBoundary::setParticleToCopy(BaseParticle* particleToCopy)
{
    particleToCopy_ = particleToCopy->copy();
}

/*!
 * \details returns pointer to the particle copies of which are to be inserted
 */
BaseParticle* InsertionBoundary::getParticleToCopy() const
{
    return particleToCopy_;
}

/*!
 * \details reads the boundary's id_ and maxFailed_ from the given istream
 * \param[in,out] is    stream the data members are read from
 */
void InsertionBoundary::read(std::istream& is)
{
    BaseBoundary::read(is);
    std::string dummy;
    is >> dummy >> maxFailed_;
}

/*!
 * \details adds the boundary's id_ and maxFailed_ to the given ostream
 * \param[in,out] is    stream the data members are to be added to
 */
void InsertionBoundary::write(std::ostream& os) const
{
    BaseBoundary::write(os);
    os << " maxFailed " << maxFailed_;
}

