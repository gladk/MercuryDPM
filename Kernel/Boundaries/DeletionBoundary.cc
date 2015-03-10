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

#include <limits>

#include "DeletionBoundary.h"
#include "ParticleHandler.h"
#include "Particles/BaseParticle.h"

DeletionBoundary::DeletionBoundary()
        : BaseBoundary()
{
    distance_ = std::numeric_limits<double>::quiet_NaN();
    scaleFactor_ = std::numeric_limits<double>::quiet_NaN();
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"DeletionBoundary () finished"<<std::endl;
#endif
}

DeletionBoundary::~DeletionBoundary()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout<<"DeletionBoundary::~DeletionBoundary() finished"<<std::endl;
#endif   
}

DeletionBoundary* DeletionBoundary::copy() const
{
    return new DeletionBoundary(*this);
}

void DeletionBoundary::set(const Vec3D& normal, Mdouble distance)
{
    scaleFactor_ = 1. / std::sqrt(Vec3D::dot(normal, normal));
    normal_ = normal * scaleFactor_;
    distance_ = distance * scaleFactor_;
}

void DeletionBoundary::move(Mdouble distance)
{
    distance_ = distance * scaleFactor_;
}

Mdouble DeletionBoundary::getDistance(const Vec3D &position) const
        {
    return distance_ - Vec3D::dot(position, normal_);
}

//Returns true if the particle is deleted
bool DeletionBoundary::checkBoundaryAfterParticleMoved(BaseParticle *p, ParticleHandler &pH)
{
    if (getDistance(p->getPosition()) < 0)
    {
        pH.removeObject(p->getIndex());
        return true;
    }
    else
        return false;
}

void DeletionBoundary::read(std::istream& is)
{
    BaseBoundary::read(is);
    std::string dummy;
    is >> dummy >> normal_
            >> dummy >> scaleFactor_
            >> dummy >> distance_;
}

void DeletionBoundary::oldRead(std::istream& is)
{
    std::string dummy;
    is >> dummy >> normal_ >> dummy >> scaleFactor_ >> dummy >> distance_;
}

void DeletionBoundary::write(std::ostream& os) const
        {
    BaseBoundary::write(os);
    os << " normal " << normal_
            << " scaleFactor " << scaleFactor_
            << " distance " << distance_;
}

std::string DeletionBoundary::getName() const
{
    return "DeletionBoundary";
}

