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

InsertionBoundary::InsertionBoundary()
{   
    numberOfParticlesInserted_=0;
    particleToCopy_=nullptr;
    maxFailed_=0;
}

InsertionBoundary::~InsertionBoundary()
{
}


void InsertionBoundary::set(BaseParticle* particleToCopy, unsigned int maxFailed)
{
    particleToCopy_ = particleToCopy;
    maxFailed_ = maxFailed;
}

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
        delete p0;
    }
}

unsigned int InsertionBoundary::getNumberOfParticlesInserted() const
{
    return numberOfParticlesInserted_;
}

void InsertionBoundary::setMaxFailed(unsigned int maxFailed)
{
    maxFailed_=maxFailed;
}

unsigned int InsertionBoundary::getMaxFailed() const
{
    return maxFailed_;
}

void InsertionBoundary::setParticleToCopy(BaseParticle* particleToCopy)
{
    particleToCopy_=particleToCopy;
}

BaseParticle* InsertionBoundary::getParticleToCopy() const
{
    return particleToCopy_;
}

///reads wall
void InsertionBoundary::read(std::istream& is)
{
    BaseBoundary::read(is);
    std::string dummy;
    is >> dummy >> maxFailed_;
}

///outputs wall
void InsertionBoundary::write(std::ostream& os) const
        {
    BaseBoundary::write(os);
    os << " maxFailed " << maxFailed_;
}

