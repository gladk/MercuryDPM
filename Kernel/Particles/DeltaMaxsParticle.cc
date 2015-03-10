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

#include "DeltaMaxsParticle.h"

DeltaMaxsParticle::DeltaMaxsParticle()
        : TangentialSpringParticle()
{
    deltaMaxs.reset();
}

DeltaMaxsParticle::DeltaMaxsParticle(const DeltaMaxsParticle &p)
        : TangentialSpringParticle(p)
{
    deltaMaxs = p.deltaMaxs;
}

DeltaMaxsParticle::DeltaMaxsParticle(const TangentialSpringParticle &p)
        : TangentialSpringParticle(p)
{
    deltaMaxs.reset();
}

DeltaMaxsParticle::~DeltaMaxsParticle()
{
}

///Particle copy method. It calls to copy contrustor of this Particle, usefull for polymorfism
DeltaMaxsParticle* DeltaMaxsParticle::copy() const
{
    return new DeltaMaxsParticle(*this);
}

CDeltaMaxs& DeltaMaxsParticle::get_DeltaMaxs()
{
    return deltaMaxs;
}

void DeltaMaxsParticle::write(std::ostream& os) const
{
    TangentialSpringParticle::write(os);
    os << " deltaMaxs " << deltaMaxs;
}

void DeltaMaxsParticle::read(std::istream& is)
{
    TangentialSpringParticle::read(is);
    std::string dummy;
    is >> dummy >> deltaMaxs;
}

void DeltaMaxsParticle::oldRead(std::istream& is)
{
    std::string dummy;
    is >> dummy;
    TangentialSpringParticle::read(is);
    is >> deltaMaxs;
}

std::string DeltaMaxsParticle::getName() const
{
    return "DeltaMaxsParticle";
}
