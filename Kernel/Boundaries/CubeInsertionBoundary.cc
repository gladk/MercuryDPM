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

#include "CubeInsertionBoundary.h"
#include "Particles/BaseParticle.h"
#include "Math/RNG.h"

CubeInsertionBoundary* CubeInsertionBoundary::copy() const
{
#ifdef DEBUG_CONSTRUCTOR
    cerr << "virtual CubeInsertionBoundary* copy() const finished" << endl;
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

