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

#include "HopperInsertionBoundary.h"
#include "Particles/BaseParticle.h"
#include "Math/RNG.h"

HopperInsertionBoundary* HopperInsertionBoundary::copy() const
{
#ifdef DEBUG_CONSTRUCTOR
    cerr << "virtual HopperInsertionBoundary* copy() const finished" << endl;
#endif		
    return new HopperInsertionBoundary(*this);
}

void HopperInsertionBoundary::set(BaseParticle* particleToCopy, int maxFailed, double yMin, double yMax, double radMin, double radMax,
        double chuteAngle, double fixedParticleRadius, bool isHopperCentred_, int hopperDim, double hopperAngle, double hopperLength, double hopperExitLength,
        double hopperHeight, double lift, double fillPercent)
{
    setParticleToCopy(particleToCopy);
    setMaxFailed(maxFailed);
    yMin_ = yMin;
    yMax_ = yMax;
    radMin_ = radMin;
    radMax_ = radMax;
    chuteAngle_ = chuteAngle;
    fixedParticleRadius_ = fixedParticleRadius;
    isHopperCentred__ = isHopperCentred_;
    hopperDim_ = hopperDim;
    hopperAngle_ = hopperAngle;
    hopperLength_ = hopperLength;
    hopperExitLength_ = hopperExitLength;
    hopperHeight_ = hopperHeight;
    lift_ = lift;
    fillPercent_ = fillPercent;
}

BaseParticle* HopperInsertionBoundary::generateParticle(RNG &random)
{
    BaseParticle* P = getParticleToCopy()->copy();
    
    P->setRadius(random.getRandomNumber(radMin_, radMax_));
    
    //Define an orthogonal coordinate system this is usful in the hopper, see diagram in html documentation for details.
    static Mdouble s = sin(chuteAngle_);
    static Mdouble c = cos(chuteAngle_);
    static Mdouble Ht = tan(hopperAngle_);
    static Mdouble Hc = cos(hopperAngle_);
    static Vec3D AB = Vec3D(c, 0.0, s);
    static Vec3D AC = Vec3D(-s, 0.0, c);
    static Vec3D AD = Vec3D(0.0, 1.0, 0.0);
    
    //Point A is located in the centre of the hopper.
    static Vec3D A = Vec3D(isHopperCentred__ ? 40 : 0.0,
            (yMax_ - yMin_) / 2.0,
            s * (-0.5 * (hopperLength_ - hopperExitLength_)) + c * hopperHeight_)
            + AB * 0.5 * hopperLength_
            + AC * (-0.5 * hopperLength_ / Ht);
    
    Mdouble gamma = random.getRandomNumber((100.0 - fillPercent_) / 100.0, 1.0);
    //std::cout<<"gamma="<<gamma<<"fillPercent="<<fillPercent_<<std::endl;
    
    Mdouble delta;
    
    if (hopperDim_ == 1)
    {
        //For the one dimensional delta is a random distance between the two walls the -minus 2 particle radii is to stop 
        /// \bug for periodic walls this should be only minus one particle radius, this should be fixed at some point.
        /// Thomas' response: using one particle radius gives problems when the wall is 
        /// not orthogonal to the y-direction; the distance has to be slightly higher than one; if you can figure out the exact value, then correct it please.
        //delta = random(ymin+P0.Radius,ymax-P0.Radius);          
        delta = random.getRandomNumber(-0.5, 0.5) * (yMax_ - yMin_ - 2.0 * P->getRadius());
        
    }
    else
    {
        delta = (random.getRandomNumber(-1.0, 1.0) * (0.5 * gamma * hopperLength_ - P->getRadius() / Hc));
    }
    //std::cout<<A<<" "<<AC<<" "<<AB<<" "<<AD<<" "<<Hc<<" "<<Ht<<"gamma="<<gamma<<" "<<hopperLength_<<" "<<delta<<std::endl;
    P->setPosition(A
            + AC * (gamma * 0.5 * hopperLength_ / Ht)
            + AB * (random.getRandomNumber(-1.0, 1.0) * (0.5 * gamma * hopperLength_ - P->getRadius() / Hc))
            + AD * delta);
    
    P->move(Vec3D(0.0, 0.0, lift_));
    P->setVelocity(Vec3D(0.0, 0.0, 0.0));
    
    return P;
}

void HopperInsertionBoundary::read(std::istream& is)
{
    InsertionBoundary::read(is);
    std::string dummy;
    is >> dummy >> yMin_
            >> dummy >> yMax_
            >> dummy >> radMin_
            >> dummy >> radMax_
            >> dummy >> chuteAngle_
            >> dummy >> fixedParticleRadius_
            >> dummy >> isHopperCentred__
            >> dummy >> hopperDim_
            >> dummy >> hopperAngle_
            >> dummy >> hopperLength_
            >> dummy >> hopperExitLength_
            >> dummy >> hopperHeight_
            >> dummy >> lift_
            >> dummy >> fillPercent_;
}

void HopperInsertionBoundary::oldRead(std::istream& is)
{
    int maxFailed;
    std::string dummy;
    is >> dummy >> maxFailed
            >> dummy >> yMin_
            >> dummy >> yMax_
            >> dummy >> radMin_
            >> dummy >> radMax_
            >> dummy >> chuteAngle_
            >> dummy >> fixedParticleRadius_
            >> dummy >> isHopperCentred__
            >> dummy >> hopperDim_
            >> dummy >> hopperAngle_
            >> dummy >> hopperLength_
            >> dummy >> hopperExitLength_
            >> dummy >> hopperHeight_
            >> dummy >> lift_
            >> dummy >> fillPercent_;
    setMaxFailed(maxFailed);
}

void HopperInsertionBoundary::write(std::ostream& os) const
        {
    InsertionBoundary::write(os);
    os << " yMin " << yMin_
            << " yMax " << yMax_
            << " radMin " << radMin_
            << " radMax " << radMax_
            << " chuteAngle " << chuteAngle_
            << " fixedParticleRadius " << fixedParticleRadius_
            << " isHopperCentred_ " << isHopperCentred__
            << " hopperDim " << hopperDim_
            << " hopperAngle " << hopperAngle_
            << " hopperLength " << hopperLength_
            << " hopperExitLength " << hopperExitLength_
            << " hopperHeight " << hopperHeight_
            << " lift " << lift_
            << " fillPercent " << fillPercent_;
}

std::string HopperInsertionBoundary::getName() const
{
    return "HopperInsertionBoundary";
}

