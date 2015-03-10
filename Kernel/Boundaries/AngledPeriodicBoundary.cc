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

/*!
 * \details 
 */
AngledPeriodicBoundary* AngledPeriodicBoundary::copy() const
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "AngledPeriodicBoundary::copy() const finished" << std::endl;
#endif                
    return new AngledPeriodicBoundary(*this);
}

/*!
 * \details given a normal vector s.t. all particles are within {x: position_left<=normal*x<position_right}. 
 * The shift vector is set assuming that the domain is rectangular (shift parallel 
 * to normal).
 * \param[in] normalLeft
 */
void AngledPeriodicBoundary::set(Vec3D normalLeft, Vec3D normalRight, Vec3D origin)
{
    origin_ = origin;
    leftNormal_ = normalLeft / Vec3D::getLength(normalLeft);
    rightNormal_ = normalRight / Vec3D::getLength(normalRight);
    commonAxis_ = Vec3D::cross(leftNormal_, rightNormal_);
    commonAxis_ /= Vec3D::getLength(commonAxis_);
    leftRadialAxis_ = Vec3D::cross(leftNormal_, commonAxis_);
    rightRadialAxis_ = Vec3D::cross(rightNormal_, commonAxis_);
    differenceNormal_ = rightNormal_ - leftNormal_;
    differenceRadialAxis_ = rightRadialAxis_ - leftRadialAxis_;
    ///\todo{I cannot calculate angular shift; right now this works only for quarter 
    // walls; maybe this has to wait till quaternions are implemented.}
    //angularShift = 0;
    // std::cout << "common_axis " << common_axis
    // << ", radialAxis_left " << radialAxis_left
    // << ", radialAxis_right " << radialAxis_right
    // << ", angularShift " << angularShift
    // << std::endl; 
}

/*!
 * \details 
 */
Mdouble AngledPeriodicBoundary::distance(const BaseParticle &P)
{
    return distance(P.getPosition());
}

/*!
 * \details 
 */
Mdouble AngledPeriodicBoundary::distance(const Vec3D &P)
{
    Vec3D position = P - origin_;
    Mdouble distance_left = Vec3D::dot(position, leftNormal_);
    Mdouble distance_right = -Vec3D::dot(position, rightNormal_);
    
    if (distance_left < distance_right)
    {
        leftWall_ = true;
        //std::cout << "left wall, " << position << ", distance " << distance_left << "<" << distance_right << std::endl;
        return distance_left;
    }
    else
    {
        leftWall_ = false;
        //std::cout << "right wall, " << position << ", distance " << distance_right << "<" << distance_left << std::endl;
        return distance_right;
    }
}

/*!
 * \details 
 */
void AngledPeriodicBoundary::shiftPosition(BaseParticle* P)
{
    Vec3D position = P->getPosition() - origin_;
    if (leftWall_)
    {
        Mdouble normalDistance = Vec3D::dot(position, leftNormal_);
        Mdouble radialDistance = Vec3D::dot(position, leftRadialAxis_);
        P->move(normalDistance * differenceNormal_ + radialDistance * differenceRadialAxis_);
        Mdouble normalVelocity = Vec3D::dot(P->getVelocity(), leftNormal_);
        Mdouble radialVelocity = Vec3D::dot(P->getVelocity(), leftRadialAxis_);
        P->accelerate(normalVelocity * differenceNormal_ + radialVelocity * differenceRadialAxis_);
        Mdouble normalAngularDistance = Vec3D::dot(P->getOrientation(), leftNormal_);
        Mdouble radialAngularDistance = Vec3D::dot(P->getOrientation(), leftRadialAxis_);
        ///\todo{TW: not sure how to calculate the angular position in common_axis direction}
        P->rotate(normalAngularDistance * differenceNormal_ + radialAngularDistance * differenceRadialAxis_);
        leftWall_ = false;
        ///\todo tangential spring
        //std::cout << "shift to right wall, " << P->getPosition() << std::endl;
    }
    else
    {
        Mdouble normalDistance = Vec3D::dot(position, rightNormal_);
        Mdouble radialDistance = Vec3D::dot(position, rightRadialAxis_);
        P->move(-normalDistance * differenceNormal_ - radialDistance * differenceRadialAxis_);
        Mdouble normalVelocity = Vec3D::dot(P->getVelocity(), rightNormal_);
        Mdouble radialVelocity = Vec3D::dot(P->getVelocity(), rightRadialAxis_);
        P->accelerate(-normalVelocity * differenceNormal_ - radialVelocity * differenceRadialAxis_);
        Mdouble normalAngularDistance = Vec3D::dot(P->getOrientation(), rightNormal_);
        Mdouble radialAngularDistance = Vec3D::dot(P->getOrientation(), rightRadialAxis_);
        P->rotate(-normalAngularDistance * differenceNormal_ - radialAngularDistance * differenceRadialAxis_);
        leftWall_ = true;
        //std::cout << "shift to left wall, " << P->getPosition() << std::endl;
    }
}

/*!
 * \details 
 */
void AngledPeriodicBoundary::shiftPositions(Vec3D& P1, Vec3D& P2)
{
    ///\todo TW: this still doesn't shift all data
    Vec3D position1 = P1 - origin_;
    Vec3D position2 = P2 - origin_;
    Mdouble normalDistance;
    Mdouble radialDistance;
    if (leftWall_)
    {
        normalDistance = Vec3D::dot(position1, leftNormal_);
        radialDistance = Vec3D::dot(position1, leftRadialAxis_);
        P1+=(normalDistance * differenceNormal_ + radialDistance * differenceRadialAxis_);
        normalDistance = Vec3D::dot(position2, leftNormal_);
        radialDistance = Vec3D::dot(position2, leftRadialAxis_);
        P2+=(normalDistance * differenceNormal_ + radialDistance * differenceRadialAxis_);
        leftWall_ = false;
    }
    else
    {
        normalDistance = Vec3D::dot(position1, rightNormal_);
        radialDistance = Vec3D::dot(position1, rightRadialAxis_);
        P1+=(-normalDistance * differenceNormal_ - radialDistance * differenceRadialAxis_);
        normalDistance = Vec3D::dot(position2, rightNormal_);
        radialDistance = Vec3D::dot(position2, rightRadialAxis_);
        P2+=(-normalDistance * differenceNormal_ - radialDistance * differenceRadialAxis_);
        leftWall_ = true;
    }
}

/*!
 * \details 
 */
void AngledPeriodicBoundary::read(std::istream& is)
{
    BaseBoundary::read(is);
    std::string dummy;
    is >> dummy >> leftNormal_ >> dummy >> rightNormal_ >> dummy >> origin_;
    set(leftNormal_, rightNormal_, origin_);
}

/*!
 * \details 
 */
void AngledPeriodicBoundary::oldRead(std::istream& is)
{
    std::string dummy;
    is >> dummy >> leftNormal_ >> dummy >> rightNormal_ >> dummy >> origin_;
    set(leftNormal_, rightNormal_, origin_);
}

/*!
 * \details 
 */
void AngledPeriodicBoundary::write(std::ostream& os) const
        {
    BaseBoundary::write(os);
    os << " normal_left " << leftNormal_
            << " normal_right " << rightNormal_
            << " origin " << origin_;
}

/*!
 * \details 
 */
std::string AngledPeriodicBoundary::getName() const
{
    return "AngledPeriodicBoundary";
}

/*!
 * \details 
 */
Vec3D& AngledPeriodicBoundary::getNormal()
{
    if (leftWall_)
        return leftNormal_;
    else
        return rightNormal_;
}

/*!
 * \details 
 */
Mdouble AngledPeriodicBoundary::getOpeningAngle()
{
    return acos(Vec3D::dot(leftNormal_, rightNormal_));
}

/*!
 * \details 
 */
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

/*!
 * \details 
 */
bool AngledPeriodicBoundary::checkBoundaryAfterParticleMoved(BaseParticle *P, ParticleHandler &pH UNUSED)
{
    if (distance(*P) < 0)
        shiftPosition(P);
    return false;
}
