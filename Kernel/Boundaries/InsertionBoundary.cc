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

