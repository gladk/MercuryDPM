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

#include "BaseBoundary.h"

BaseBoundary::BaseBoundary()
{
    handler_ = 0;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseBoundary::BaseBoundary() finished"<<std::endl;
#endif
}

BaseBoundary::BaseBoundary(const BaseBoundary &b)
        : BaseObject(b)
{
    handler_ = b.handler_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseBoundary::BaseBoundary(const BaseBoundary &b) finished"<<std::endl;
#endif
}

BaseBoundary::~BaseBoundary()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "BaseBoundary::~BaseBoundary() finished"<<std::endl;
#endif 
}

void BaseBoundary::read(std::istream& is)
{
    BaseObject::read(is);
}

void BaseBoundary::write(std::ostream& os) const
{
    BaseObject::write(os);
}

void BaseBoundary::createPeriodicParticles(BaseParticle *P UNUSED, ParticleHandler &pH UNUSED)
{
}

bool BaseBoundary::checkBoundaryAfterParticleMoved(BaseParticle *P UNUSED, ParticleHandler &pH UNUSED)
{
    return false;
}

void BaseBoundary::checkBoundaryBeforeTimeStep(DPMBase* md UNUSED)
{
}

void BaseBoundary::setHandler(BoundaryHandler* handler)
{
    handler_ = handler;
}

BoundaryHandler* BaseBoundary::getHandler() const
{
    return handler_;
}
