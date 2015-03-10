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

#include "CubeInsertionBoundary.h"
#include "Particles/BaseParticle.h"
#include "Math/RNG.h"

CubeInsertionBoundary* CubeInsertionBoundary::copy() const
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "CubeInsertionBoundary::copy() const finished" << std::endl;
#endif		
    return new CubeInsertionBoundary(*this);
}

void CubeInsertionBoundary::set(BaseParticle* particleToCopy, int maxFailed, Vec3D posMin, Vec3D posMax, Vec3D velMin, Vec3D velMax, double radMin, double radMax)
{
    setParticleToCopy(particleToCopy);
    setMaxFailed(maxFailed);
    posMin_ = posMin;
    posMax_ = posMax;
    velMin_ = velMin;
    velMax_ = velMax;
    radMin_ = radMin;
    radMax_ = radMax;
}

BaseParticle* CubeInsertionBoundary::generateParticle(RNG &random)
{
    BaseParticle* P = getParticleToCopy()->copy();
    Vec3D pos, vel;
    double radius;
    pos.X = random.getRandomNumber(posMin_.X, posMax_.X);
    pos.Y = random.getRandomNumber(posMin_.Y, posMax_.Y);
    pos.Z = random.getRandomNumber(posMin_.Z, posMax_.Z);
    vel.X = random.getRandomNumber(velMin_.X, velMax_.X);
    vel.Y = random.getRandomNumber(velMin_.Y, velMax_.Y);
    vel.Z = random.getRandomNumber(velMin_.Z, velMax_.Z);
    radius = random.getRandomNumber(radMin_, radMax_);
    P->setPosition(pos);
    P->setVelocity(vel);
    P->setRadius(radius);
    return P;
}

void CubeInsertionBoundary::read(std::istream& is)
{
    InsertionBoundary::read(is);
    std::string dummy;
    is >> dummy >> posMin_
            >> dummy >> posMax_
            >> dummy >> velMin_
            >> dummy >> velMax_
            >> dummy >> radMin_
            >> dummy >> radMax_;
}

void CubeInsertionBoundary::oldRead(std::istream& is)
{
    int maxFailed;
    std::string dummy;
    is >> dummy >> maxFailed
            >> dummy >> posMin_
            >> dummy >> posMax_
            >> dummy >> velMin_
            >> dummy >> velMax_
            >> dummy >> radMin_
            >> dummy >> radMax_;
    setMaxFailed(maxFailed);
}

void CubeInsertionBoundary::write(std::ostream& os) const
        {
    InsertionBoundary::write(os);
    os << " posMin " << posMin_
            << " posMax " << posMax_
            << " velMin " << velMin_
            << " velMax " << velMax_
            << " radMin " << radMin_
            << " radMax " << radMax_;
}

std::string CubeInsertionBoundary::getName() const
{
    return "CubeInsertionBoundary";
}

