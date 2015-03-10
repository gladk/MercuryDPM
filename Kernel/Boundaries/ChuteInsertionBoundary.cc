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

#include "ChuteInsertionBoundary.h"
#include "Particles/BaseParticle.h"
#include "Math/RNG.h"

ChuteInsertionBoundary* ChuteInsertionBoundary::copy() const
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "ChuteInsertionBoundary::copy() const finished" << std::endl;
#endif		
    return new ChuteInsertionBoundary(*this);
}

void ChuteInsertionBoundary::set(BaseParticle* particleToCopy, unsigned int maxFailed, Vec3D posMin, Vec3D posMax, double radMin, double radMax, double fixedParticleRadius, double inflowVelocity, double inflowVelocityVariance)
{
    setParticleToCopy(particleToCopy);
    setMaxFailed(maxFailed);
    posMin_ = posMin;
    posMax_ = posMax;
    radMin_ = radMin;
    radMax_ = radMax;
    fixedParticleRadius_ = fixedParticleRadius;
    inflowVelocity_ = inflowVelocity;
    inflowVelocityVariance_ = inflowVelocityVariance;
}

BaseParticle* ChuteInsertionBoundary::generateParticle(RNG &random)
{
    BaseParticle* P = getParticleToCopy()->copy();
    
    P->setRadius(random.getRandomNumber(radMin_, radMax_));
    
    Vec3D position, velocity;

    position.X = posMin_.X + P->getRadius();

    if ((posMax_.Y - posMin_.Y) == 2.0 * radMax_)
    {
        position.Y = posMin_.Y + P->getRadius();
    }
    else
    {
        position.Y = random.getRandomNumber(posMin_.Y + P->getRadius(), posMax_.Y - P->getRadius());
    }
    position.Z = random.getRandomNumber(posMin_.Z + fixedParticleRadius_ + P->getRadius(), posMax_.Z - P->getRadius());
    
    //The velocity components are first stored in a Vec3D, because if you pass them directly into set_Velocity the compiler is allowed to change the order in which the numbers are generated
    velocity.X = inflowVelocity_ * random.getRandomNumber(-inflowVelocityVariance_, inflowVelocityVariance_) + inflowVelocity_;
    velocity.Y = inflowVelocity_ * random.getRandomNumber(-inflowVelocityVariance_, inflowVelocityVariance_);
    velocity.Z = inflowVelocity_ * random.getRandomNumber(-inflowVelocityVariance_, inflowVelocityVariance_);
    
    P->setPosition(position);
    P->setVelocity(velocity);
    
    return P;
}

void ChuteInsertionBoundary::read(std::istream& is)
{
    InsertionBoundary::read(is);
    std::string dummy;
    is >> dummy >> posMin_
            >> dummy >> posMax_
            >> dummy >> radMin_
            >> dummy >> radMax_
            >> dummy >> fixedParticleRadius_
            >> dummy >> inflowVelocity_
            >> dummy >> inflowVelocityVariance_;
}

void ChuteInsertionBoundary::oldRead(std::istream& is)
{
    unsigned int maxFailed;
    std::string dummy;
    is >> dummy >> maxFailed
            >> dummy >> posMin_
            >> dummy >> posMax_
            >> dummy >> radMin_
            >> dummy >> radMax_
            >> dummy >> fixedParticleRadius_
            >> dummy >> inflowVelocity_
            >> dummy >> inflowVelocityVariance_;
    setMaxFailed(maxFailed);
}

void ChuteInsertionBoundary::write(std::ostream& os) const
        {
    InsertionBoundary::write(os);
    os << " posMin " << posMin_
            << " posMax " << posMax_
            << " radMin " << radMin_
            << " radMax " << radMax_
            << " fixedParticleRadius " << fixedParticleRadius_
            << " inflowVelocity " << inflowVelocity_
            << " inflowVelocityVariance " << inflowVelocityVariance_;
    
}

std::string ChuteInsertionBoundary::getName() const
{
    return "ChuteInsertionBoundary";
}

