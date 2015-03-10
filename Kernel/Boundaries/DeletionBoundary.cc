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

