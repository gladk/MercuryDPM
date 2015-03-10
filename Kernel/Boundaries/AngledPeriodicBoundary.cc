// This file is part of MercuryDPM.
// 
// MercuryDPM is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// MercuryDPM is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.    See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with MercuryDPM.    If not, see <http://www.gnu.org/licenses/>.
// 
// Copyright 2013 The Mercury Developers Team
// For the list of developers, see <http://www.MercuryDPM.org/Team>

#include "AngledPeriodicBoundary.h"
#include "ParticleHandler.h"
#include "Particles/BaseParticle.h"


AngledPeriodicBoundary* AngledPeriodicBoundary::copy() const
{
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "virtual AngledPeriodicBoundary* copy() const finished" << std::endl;
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

void AngledPeriodicBoundary::shift_position(BaseParticle* P)
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

// ///returns the shifted particle w/o actually shifting
// Vec3D AngledPeriodicBoundary::getShiftedPosition(Vec3D &Position) {
//     if (left_wall) {
//         return Position + shift;
//     }
//     else {
//         return Position - shift;
//     }
// }

// ///shifts two particles
// void AngledPeriodicBoundary::shift_positions(Vec3D &PI, Vec3D &PJ) {
//     if (left_wall) {
//         PI += shift;
//         PJ += shift;
//         left_wall = false;
//     }
//     else {
//         PI -= shift;
//         PJ -= shift;
//         left_wall = true;
//     }
// }

// ///shift P such that it is closest to Q
// void AngledPeriodicBoundary::getCloseTogether(Vec3D &P,Vec3D &Q) {
//     Mdouble PQdotn = Vec3D::Dot(P-Q, normal);
//     Mdouble shift_norm2 = shift.Vec3D::GetLength2();
//     //Check if P is so far from Q that a shift would move it closer
//     if (mathsFunc::square(PQdotn) > .25 * shift_norm2) {
//         //Now determine the direction of the shift
//         if (PQdotn>0.0) P -= shift;
//         else P += shift;
//     }
// }

void AngledPeriodicBoundary::read(std::istream& is)
{
    BaseBoundary::read(is);
    std::string dummy;
    is >> dummy >> normal_left >> dummy >> normal_right >> dummy >> origin;
}

void AngledPeriodicBoundary::oldRead(std::istream& is)
{
    std::string dummy;
    is >> dummy >> normal_left >> dummy >> normal_right >> dummy >> origin;
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

void AngledPeriodicBoundary::createPeriodicParticles(BaseParticle *P, ParticleHandler &pH)
{
    //std::cout << "createPeriodicParticles" << std::endl;
    if (distance(*P) < P->getInteractionRadius() + pH.getLargestParticle()->getInteractionRadius())
    {
        BaseParticle* F0 = P->copy();
        shift_position(F0);
        
        /*//If the Particle includes TangentalSprings reverse them
        TangentialSpringParticle* TSParticle = dynamic_cast<TangentialSpringParticle*>(F0);
        if (TSParticle)
        {
            TSParticle->reverseTangentialSprings();
            for (std::vector<CTangentialSpring>::iterator it = TSParticle->get_TangentialSprings().begin(); it != TSParticle->get_TangentialSprings().end(); it++)
            {
                //std::cout << it->delta << std::endl;
                if (!left_wall)
                {
                    Mdouble normalDistance = Vec3D::dot(it->delta, normal_left);
                    Mdouble radialDistance = Vec3D::dot(it->delta, radialAxis_left);
                    it->delta += normalDistance * diff_normal + radialDistance * diff_radial;
                    normalDistance = Vec3D::dot(it->RollingSpring, normal_left);
                    radialDistance = Vec3D::dot(it->RollingSpring, radialAxis_left);
                    it->RollingSpring += normalDistance * diff_normal + radialDistance * diff_radial;
                    normalDistance = Vec3D::dot(it->TorsionSpring, normal_left);
                    radialDistance = Vec3D::dot(it->TorsionSpring, radialAxis_left);
                    it->TorsionSpring += normalDistance * diff_normal + radialDistance * diff_radial;
                }
                else
                {
                    Mdouble normalDistance = Vec3D::dot(it->delta, normal_right);
                    Mdouble radialDistance = Vec3D::dot(it->delta, radialAxis_right);
                    it->delta -= normalDistance * diff_normal + radialDistance * diff_radial;
                    normalDistance = Vec3D::dot(it->RollingSpring, normal_right);
                    radialDistance = Vec3D::dot(it->RollingSpring, radialAxis_right);
                    it->RollingSpring -= normalDistance * diff_normal + radialDistance * diff_radial;
                    normalDistance = Vec3D::dot(it->TorsionSpring, normal_right);
                    radialDistance = Vec3D::dot(it->TorsionSpring, radialAxis_right);
                    it->TorsionSpring -= normalDistance * diff_normal + radialDistance * diff_radial;
                }
                //std::cout << it->delta << std::endl;
            }
        }*/
        
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
    //std::cout << "checkBoundaryAfterParticleMoved" << std::endl;
    if (distance(*P) < 0)
        shift_position(P);
    return false;
}
