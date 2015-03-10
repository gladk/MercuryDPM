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
 void PeriodicBoundary::get_close_together(Vec3D &P, Vec3D &Q)
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
 Vec3D& PeriodicBoundary::get_normal()
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
