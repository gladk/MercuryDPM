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

#include "ChuteInsertionBoundary.h"
#include "Particles/BaseParticle.h"
#include "Math/RNG.h"

ChuteInsertionBoundary* ChuteInsertionBoundary::copy() const
{
#ifdef DEBUG_CONSTRUCTOR
    cerr << "virtual ChuteInsertionBoundary* copy() const finished" << endl;
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

