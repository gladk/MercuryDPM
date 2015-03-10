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
#include "AngledPeriodicBoundary.h"
#include "ParticleHandler.h"
#include "Particles/BaseParticle.h"


AngledPeriodicBoundary* AngledPeriodicBoundary::copy() const
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "AngledPeriodicBoundary::copy() const finished" << std::endl;
#endif                
    return new AngledPeriodicBoundary(*this);
}

void AngledPeriodicBoundary::set(Vec3D normal_left_, Vec3D normal_right_, Vec3D origin_)
{
    origin = origin_;
    normal_left = normal_left_ / Vec3D::getLength(normal_left_);
    normal_right = normal_right_ / Vec3D::getLength(normal_right_);
    common_axis = Vec3D::cross(normal_left, normal_right);
    common_axis /= Vec3D::getLength(common_axis);
    radialAxis_left = Vec3D::cross(normal_left, common_axis);
    radialAxis_right = Vec3D::cross(normal_right, common_axis);
    diff_normal = normal_right - normal_left;
    diff_radial = radialAxis_right - radialAxis_left;
    ///\todo{I cannot calculate angular shift; right now this works only for quarter walls; maybe this has to wait till quaternions are implemented.}
    //angularShift = 0;
    // std::cout << "common_axis " << common_axis
    // << ", radialAxis_left " << radialAxis_left
    // << ", radialAxis_right " << radialAxis_right
    // << ", angularShift " << angularShift
    // << std::endl; 
}

Mdouble AngledPeriodicBoundary::distance(const BaseParticle &P)
{
    return distance(P.getPosition());
}

Mdouble AngledPeriodicBoundary::distance(const Vec3D &P)
{
    Vec3D position = P - origin;
    Mdouble distance_left = Vec3D::dot(position, normal_left);
    Mdouble distance_right = -Vec3D::dot(position, normal_right);
    
    if (distance_left < distance_right)
    {
        left_wall = true;
        //std::cout << "left wall, " << position << ", distance " << distance_left << "<" << distance_right << std::endl;
        return distance_left;
    }
    else
    {
        left_wall = false;
        //std::cout << "right wall, " << position << ", distance " << distance_right << "<" << distance_left << std::endl;
        return distance_right;
    }
}

void AngledPeriodicBoundary::shiftPosition(BaseParticle* P)
{
    Vec3D position = P->getPosition() - origin;
    if (left_wall)
    {
        Mdouble normalDistance = Vec3D::dot(position, normal_left);
        Mdouble radialDistance = Vec3D::dot(position, radialAxis_left);
        P->move(normalDistance * diff_normal + radialDistance * diff_radial);
        Mdouble normalVelocity = Vec3D::dot(P->getVelocity(), normal_left);
        Mdouble radialVelocity = Vec3D::dot(P->getVelocity(), radialAxis_left);
        P->accelerate(normalVelocity * diff_normal + radialVelocity * diff_radial);
        Mdouble normalAngularDistance = Vec3D::dot(P->getOrientation(), normal_left);
        Mdouble radialAngularDistance = Vec3D::dot(P->getOrientation(), radialAxis_left);
        ///\todo{TW: not sure how to calculate the angular position in common_axis direction}
        P->rotate(normalAngularDistance * diff_normal + radialAngularDistance * diff_radial);
        left_wall = false;
        ///\todo tangential spring
        //std::cout << "shift to right wall, " << P->getPosition() << std::endl;
    }
    else
    {
        Mdouble normalDistance = Vec3D::dot(position, normal_right);
        Mdouble radialDistance = Vec3D::dot(position, radialAxis_right);
        P->move(-normalDistance * diff_normal - radialDistance * diff_radial);
        Mdouble normalVelocity = Vec3D::dot(P->getVelocity(), normal_right);
        Mdouble radialVelocity = Vec3D::dot(P->getVelocity(), radialAxis_right);
        P->accelerate(-normalVelocity * diff_normal - radialVelocity * diff_radial);
        Mdouble normalAngularDistance = Vec3D::dot(P->getOrientation(), normal_right);
        Mdouble radialAngularDistance = Vec3D::dot(P->getOrientation(), radialAxis_right);
        P->rotate(-normalAngularDistance * diff_normal - radialAngularDistance * diff_radial);
        left_wall = true;
        //std::cout << "shift to left wall, " << P->getPosition() << std::endl;
    }
}

void AngledPeriodicBoundary::shiftPositions(Vec3D& P1, Vec3D& P2)
{
    ///\todo TW: this still doesn't shift all data
    Vec3D position1 = P1 - origin;
    Vec3D position2 = P2 - origin;
    Mdouble normalDistance;
    Mdouble radialDistance;
    if (left_wall)
    {
        normalDistance = Vec3D::dot(position1, normal_left);
        radialDistance = Vec3D::dot(position1, radialAxis_left);
        P1+=(normalDistance * diff_normal + radialDistance * diff_radial);
        normalDistance = Vec3D::dot(position2, normal_left);
        radialDistance = Vec3D::dot(position2, radialAxis_left);
        P2+=(normalDistance * diff_normal + radialDistance * diff_radial);
        left_wall = false;
    }
    else
    {
        normalDistance = Vec3D::dot(position1, normal_right);
        radialDistance = Vec3D::dot(position1, radialAxis_right);
        P1+=(-normalDistance * diff_normal - radialDistance * diff_radial);
        normalDistance = Vec3D::dot(position2, normal_right);
        radialDistance = Vec3D::dot(position2, radialAxis_right);
        P2+=(-normalDistance * diff_normal - radialDistance * diff_radial);
        left_wall = true;
    }
}


void AngledPeriodicBoundary::read(std::istream& is)
{
    BaseBoundary::read(is);
    std::string dummy;
    is >> dummy >> normal_left >> dummy >> normal_right >> dummy >> origin;
    set(normal_left, normal_right, origin);
}

void AngledPeriodicBoundary::oldRead(std::istream& is)
{
    std::string dummy;
    is >> dummy >> normal_left >> dummy >> normal_right >> dummy >> origin;
    set(normal_left, normal_right, origin);
}

void AngledPeriodicBoundary::write(std::ostream& os) const
        {
    BaseBoundary::write(os);
    os << " normal_left " << normal_left
            << " normal_right " << normal_right
            << " origin " << origin;
}

std::string AngledPeriodicBoundary::getName() const
{
    return "AngledPeriodicBoundary";
}

Vec3D& AngledPeriodicBoundary::getNormal()
{
    if (left_wall)
        return normal_left;
    else
        return normal_right;
}

Mdouble AngledPeriodicBoundary::getOpeningAngle()
{
    return acos(Vec3D::dot(normal_left, normal_right));
}

void AngledPeriodicBoundary::createPeriodicParticles(BaseParticle *P, ParticleHandler &pH)
{
    if (distance(*P) < P->getInteractionRadius() + pH.getLargestParticle()->getInteractionRadius())
    {
        BaseParticle* F0 = P->copy();
        shiftPosition(F0);

        //If Particle is Mdouble shifted, get correct original particle
        BaseParticle* From = P;
        while (From->getPeriodicFromParticle() != nullptr)
            From = From->getPeriodicFromParticle();
        F0->setPeriodicFromParticle(From);
        
        pH.addObject(F0);
    }
}

bool AngledPeriodicBoundary::checkBoundaryAfterParticleMoved(BaseParticle *P, ParticleHandler &pH UNUSED)
{
    if (distance(*P) < 0)
        shiftPosition(P);
    return false;
}
