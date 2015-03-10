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

#include "PeriodicBoundary.h"
#include "ParticleHandler.h"
#include "Particles/BaseParticle.h"
PeriodicBoundary::PeriodicBoundary()
        : BaseBoundary()
{
    closestToLeftBoundary_=true;
    distanceLeft_ = std::numeric_limits<double>::quiet_NaN();
    distanceRight_= std::numeric_limits<double>::quiet_NaN();
    scaleFactor_= std::numeric_limits<double>::quiet_NaN();

#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"PeriodicBoundary::PeriodicBoundary() finished"<<std::endl;
#endif
}

PeriodicBoundary::~PeriodicBoundary()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout<<"PeriodicBoundary::~PeriodicBoundary() finished"<<std::endl;
#endif   
}

PeriodicBoundary* PeriodicBoundary::copy() const
{
    return new PeriodicBoundary(*this);
}

void PeriodicBoundary::set(Vec3D normal, Mdouble distanceLeft, Mdouble distanceRight)
{
    // factor is used to set normal to unit length
    scaleFactor_ = 1. / std::sqrt(Vec3D::dot(normal, normal));
    normal_ = normal * scaleFactor_;
    distanceLeft_ = distanceLeft * scaleFactor_;
    distanceRight_ = distanceRight * scaleFactor_;
    shift_ = normal_ * (distanceRight_ - distanceLeft_);
}

void PeriodicBoundary::set(Vec3D normal, Mdouble distanceLeft, Mdouble distanceRight, Vec3D shiftDirection)
{
    // factor is used to set normal to unit length
    scaleFactor_ = 1. / std::sqrt(Vec3D::dot(normal, normal));
    normal_ = normal * scaleFactor_;
    distanceLeft_ = distanceLeft * scaleFactor_;
    distanceRight_ = distanceRight * scaleFactor_;
    // factor is used to set shift vector to correct length
    scaleFactor_ = (distanceRight_ - distanceLeft_) * Vec3D::dot(shiftDirection, normal_);
    shift_ = shiftDirection * scaleFactor_;
}

void PeriodicBoundary::moveLeft(Mdouble distanceLeft)
{
    distanceLeft_ = distanceLeft * scaleFactor_;
    shift_ = normal_ * (distanceRight_ - distanceLeft_);
}

void PeriodicBoundary::moveRight(Mdouble distanceRight)
{
    distanceRight_ = distanceRight * scaleFactor_;
    shift_ = normal_ * (distanceRight_ - distanceLeft_);
}

Mdouble PeriodicBoundary::getDistance(BaseParticle &p)
{
    return getDistance(p.getPosition());
}

Mdouble PeriodicBoundary::getDistance(const Vec3D &position)
{
    Mdouble distance = Vec3D::dot(position, normal_);
    
    if (distance - distanceLeft_ < distanceRight_ - distance)
    {
        closestToLeftBoundary_ = true;
        return distance - distanceLeft_;
    }
    else
    {
        closestToLeftBoundary_ = false;
        return distanceRight_ - distance;
    }
}

void PeriodicBoundary::shiftPosition(BaseParticle* p)
{
    if (closestToLeftBoundary_)
    {
        p->move(shift_);
        closestToLeftBoundary_ = false;
    }
    else
    {
        p->move(-shift_);
        closestToLeftBoundary_ = true;
    }
}

/*
 Vec3D PeriodicBoundary::getShiftedPosition(Vec3D &position)
 {
 if (left_wall)
 {
 return position + shift_;
 }
 else
 {
 return position - shift_;
 }
 }
 */

void PeriodicBoundary::shiftPositions(Vec3D &postition1, Vec3D &postion2)
{
    if (closestToLeftBoundary_)
    {
        postition1 += shift_;
        postion2 += shift_;
        closestToLeftBoundary_ = false;
    }
    else
    {
        postition1 -= shift_;
        postion2 -= shift_;
        closestToLeftBoundary_ = true;
    }
}

bool PeriodicBoundary::isClosestToLeftBoundary() const
{
    return closestToLeftBoundary_;
}

/*
 void PeriodicBoundary::getCloseTogether(Vec3D &P, Vec3D &Q)
 {
 Mdouble PQdotn = Vec3D::Dot(P - Q, normal_);
 Mdouble shift_norm2 = shift.GetLength2();
 //Check if P is so far from Q that a shift would move it closer
 if (mathsFunc::square(PQdotn) > .25 * shift_norm2)
 {
 //Now determine the direction of the shift
 if (PQdotn > 0.0)
 P -= shift;
 else
 P += shift;
 }
 }
 */
void PeriodicBoundary::read(std::istream& is)
{
    BaseBoundary::read(is);
    std::string dummy;
    is >> dummy >> normal_
            >> dummy >> scaleFactor_
            >> dummy >> distanceLeft_
            >> dummy >> distanceRight_
            >> dummy >> shift_;
}

void PeriodicBoundary::oldRead(std::istream& is)
{
    std::string dummy;
    is >> dummy >> normal_
            >> dummy >> scaleFactor_
            >> dummy >> distanceLeft_
            >> dummy >> distanceRight_
            >> dummy >> shift_;
}

void PeriodicBoundary::write(std::ostream& os) const
        {
    BaseBoundary::write(os);
    os << " normal " << normal_
            << " scaleFactor " << scaleFactor_
            << " distanceLeft " << distanceLeft_
            << " distanceRight " << distanceRight_
            << " shift " << shift_;
}

std::string PeriodicBoundary::getName() const
{
    return "PeriodicBoundary";
}

/*
 Vec3D& PeriodicBoundary::getNormal()
 {
 return normal_;
 }
 */

void PeriodicBoundary::createPeriodicParticles(BaseParticle *p, ParticleHandler &pH)
{
    if (getDistance(*p) < p->getInteractionRadius() + pH.getLargestParticle()->getInteractionRadius())
    {
        BaseParticle* F0 = p->copy();
        F0->copyInteractionsForPeriodicParticles(*p);

        shiftPosition(F0);
                
        //If Particle is double shifted, get correct original particle
        BaseParticle* From = p;
        while (From->getPeriodicFromParticle() != nullptr)
            From = From->getPeriodicFromParticle();
        F0->setPeriodicFromParticle(From);
        
        pH.addObject(F0);
    }
}

bool PeriodicBoundary::checkBoundaryAfterParticleMoved(BaseParticle *p, ParticleHandler &pH UNUSED)
{
    if (getDistance(*p) < 0)
    {
        shiftPosition(p);
    }
    return false;
}
