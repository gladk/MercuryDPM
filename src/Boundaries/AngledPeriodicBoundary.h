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

#ifndef AngledPeriodicBoundary_H
#define AngledPeriodicBoundary_H

#include "../BaseBoundary.h"
#include "../Particles/TangentialSpringParticle.h"
#include "../Particles/BaseParticle.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
/// Defines a pair of periodic walls that are angled around the origin. 
/// The particles are in {x: normal_left*(x-origin)>0 && normal_right*(x-origin)<0, with normal* being the unit normal vector of the walls. 
/// If a particle moves outside these boundaries, it will be shifted.
////////////////////////////////////////////////////////////////////////////////////////////////////
class AngledPeriodicBoundary : public BaseBoundary {
public:

    virtual AngledPeriodicBoundary* copy() const
    {
        #ifdef CONSTUCTOR_OUTPUT
            std::cerr << "virtual AngledPeriodicBoundary* copy() const finished" << std::endl;
        #endif                
        return new AngledPeriodicBoundary(*this);
    }
    
    
    //todo constructors instead of set functions?
    //AngledPeriodicBoundary (Vec3D normal_left_, Vec3D normal_right_, Vec3D origin_) 

    ///Defines a periodic wall, given a 
    ///normal vector s.t. all particles are within
    ///{x: position_left<=normal*x<position_right}.
    ///The shift vector is set assuming that the domain 
    ///is rectangular (shift parallel to normal).
    void set (Vec3D normal_left_, Vec3D normal_right_, Vec3D origin_) {
        origin=origin_;
        normal_left = normal_left_ / GetLength(normal_left_);
        normal_right = normal_right_ / GetLength(normal_right_);
        common_axis = Cross(normal_left, normal_right);
        common_axis /= GetLength(common_axis);
        radialAxis_left = Cross(normal_left, common_axis);
        radialAxis_right = Cross(normal_right, common_axis);
        diff_normal = normal_right-normal_left;
        diff_radial = radialAxis_right-radialAxis_left;
        ///\todo{I cannot calculate angular shift; right now this works only for quarter walls; maybe this has to wait till quaternions are implemented.}
        //angularShift = 0;
        // std::cout << "common_axis " << common_axis
        // << ", radialAxis_left " << radialAxis_left
        // << ", radialAxis_right " << radialAxis_right
        // << ", angularShift " << angularShift
        // << std::endl; 
    }
    ///Returns the distance of the wall to the particle, and 
    ///sets left_wall = true, if the left wall is the wall 
    ///closest to the particle.
    ///Since this function should be called before 
    ///calculating any Particle-Wall interactions, 
    ///it can also be used to set the shift vector 
    ///in case of curved walls.
    Mdouble distance(BaseParticle &P) {
        return distance(P.get_Position());
    }

    //this function should be cheap, as it has to be computed for all particles
    Mdouble distance(const Vec3D &P) {
        Vec3D position = P-origin;
        Mdouble distance_left = Dot(position,normal_left);
        Mdouble distance_right = -Dot(position,normal_right);

        if (distance_left<distance_right) {
            left_wall = true;
            //std::cout << "left wall, " << position << ", distance " << distance_left << "<" << distance_right << std::endl;
            return distance_left;
        } else {
            left_wall = false;
            //std::cout << "right wall, " << position << ", distance " << distance_right << "<" << distance_left << std::endl;
            return distance_right;
        }
    }

    ///shifts the particle (after distance set the left_wall value)
    ///\todo add velocity, angular, springs shift
    void shift_position(BaseParticle* P) {
        Vec3D position = P->get_Position()-origin;
        if (left_wall) {
            Mdouble normalDistance = Dot(position,normal_left);
            Mdouble radialDistance = Dot(position,radialAxis_left);
            P->move(normalDistance*diff_normal+radialDistance*diff_radial);
            Mdouble normalVelocity = Dot(P->get_Velocity(),normal_left);
            Mdouble radialVelocity = Dot(P->get_Velocity(),radialAxis_left);
            P->accelerate(normalVelocity*diff_normal+radialVelocity*diff_radial);
            Mdouble normalAngularDistance = Dot(P->get_Angle(),normal_left);
            Mdouble radialAngularDistance = Dot(P->get_Angle(),radialAxis_left);
            ///\todo{TW: not sure how to calculate the angular position in common_axis direction}
            P->rotate(normalAngularDistance*diff_normal+radialAngularDistance*diff_radial);
            left_wall = false;
            ///\todo tangential spring
            //std::cout << "shift to right wall, " << P->get_Position() << std::endl;
        }
        else {
            Mdouble normalDistance = Dot(position,normal_right);
            Mdouble radialDistance = Dot(position,radialAxis_right);
            P->move(-normalDistance*diff_normal-radialDistance*diff_radial);
            Mdouble normalVelocity = Dot(P->get_Velocity(),normal_right);
            Mdouble radialVelocity = Dot(P->get_Velocity(),radialAxis_right);
            P->accelerate(-normalVelocity*diff_normal-radialVelocity*diff_radial);
            Mdouble normalAngularDistance = Dot(P->get_Angle(),normal_right);
            Mdouble radialAngularDistance = Dot(P->get_Angle(),radialAxis_right);
            P->rotate(-normalAngularDistance*diff_normal-radialAngularDistance*diff_radial);
            left_wall = true;
            //std::cout << "shift to left wall, " << P->get_Position() << std::endl;
        }
    }

    // ///returns the shifted particle w/o actually shifting
    // Vec3D get_shifted_position(Vec3D &Position) {
    //     if (left_wall) {
    //         return Position + shift;
    //     }
    //     else {
    //         return Position - shift;
    //     }
    // }
    
    // ///shifts two particles
    // void shift_positions(Vec3D &PI, Vec3D &PJ) {
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
    // void get_close_together(Vec3D &P,Vec3D &Q) {
    //     Mdouble PQdotn = Dot(P-Q, normal);
    //     Mdouble shift_norm2 = shift.GetLength2();
    //     //Check if P is so far from Q that a shift would move it closer
    //     if (sqr(PQdotn) > .25 * shift_norm2) {
    //         //Now determine the direction of the shift
    //         if (PQdotn>0.0) P -= shift;
    //         else P += shift;
    //     }
    // }
        

    ///reads wall
    void read(std::istream& is) { 
        std::string dummy;
        is >> dummy >> normal_left >> dummy >> normal_right >> dummy >> origin;
    }
    
    ///outputs wall
    void print(std::ostream& os) const {
        os << "AngledPeriodicBoundary normal_left " << normal_left 
            << " normal_right " << normal_right 
            << " origin " << origin;
    }
        
    Vec3D& get_normal() {
        if (left_wall) return normal_left;
        else return normal_right;
    }
    
    ///\todo{TW: shouldn't that return a boolean?}
    int createPeriodicParticles(BaseParticle *P, ParticleHandler &pH)
    {
        //std::cout << "createPeriodicParticles" << std::endl;
        if (distance(*P)<P->get_InteractionRadius()+pH.getLargestParticle()->get_InteractionRadius())
        {
            BaseParticle* F0=P->copy();
            shift_position(F0);
                        
            //If the Particle includes TangentalSprings reverse them
            TangentialSpringParticle* TSParticle=dynamic_cast<TangentialSpringParticle*>(F0);
            if(TSParticle) {
                TSParticle->reverseTangentialSprings();
                for(std::vector<CTangentialSpring>::iterator it = TSParticle->get_TangentialSprings().begin(); it!=TSParticle->get_TangentialSprings().end();it++)
                {
                    //std::cout << it->delta << std::endl;
                    if (!left_wall) {
                        Mdouble normalDistance = Dot(it->delta,normal_left);
                        Mdouble radialDistance = Dot(it->delta,radialAxis_left);
                        it->delta += normalDistance*diff_normal+radialDistance*diff_radial;
                        normalDistance = Dot(it->RollingSpring,normal_left);
                        radialDistance = Dot(it->RollingSpring,radialAxis_left);
                        it->RollingSpring += normalDistance*diff_normal+radialDistance*diff_radial;
                        normalDistance = Dot(it->TorsionSpring,normal_left);
                        radialDistance = Dot(it->TorsionSpring,radialAxis_left);
                        it->TorsionSpring += normalDistance*diff_normal+radialDistance*diff_radial;
                    } else {
                        Mdouble normalDistance = Dot(it->delta,normal_right);
                        Mdouble radialDistance = Dot(it->delta,radialAxis_right);
                        it->delta -= normalDistance*diff_normal+radialDistance*diff_radial;
                        normalDistance = Dot(it->RollingSpring,normal_right);
                        radialDistance = Dot(it->RollingSpring,radialAxis_right);
                        it->RollingSpring -= normalDistance*diff_normal+radialDistance*diff_radial;
                        normalDistance = Dot(it->TorsionSpring,normal_right);
                        radialDistance = Dot(it->TorsionSpring,radialAxis_right);
                        it->TorsionSpring -= normalDistance*diff_normal+radialDistance*diff_radial;
                    }
                    //std::cout << it->delta << std::endl;
                }        
            }
    
            //If Particle is Mdouble shifted, get correct original particle
            BaseParticle* From=P;
            while(From->get_PeriodicFromParticle()!=NULL)
                From=From->get_PeriodicFromParticle();        
            F0->set_periodicFromParticle(From);

            pH.addObject(F0);
            return 1;
        }
        return 0;
    }        

    ///\todo{TW: can't we do the shift in the createPeriodicParticles function only? We are checking the distance three times!}
    bool checkBoundaryAfterParticleMoved(BaseParticle *P, ParticleHandler &pH UNUSED)
    {
        //std::cout << "checkBoundaryAfterParticleMoved" << std::endl;
        if (distance(*P)<0)
            shift_position(P);
        return false;
    }
    
 private:
    //values set by the user
    Vec3D normal_left;         ///< outward unit normal vector for left wall
    Vec3D normal_right;        ///< outward unit normal vector for right wall
    Vec3D origin;                    ///< common point of both walls
    //values set by the code
    bool left_wall;                ///< true if closest wall is the left wall
    Vec3D radialAxis_left;         ///< outward unit normal vector for left wall
    Vec3D radialAxis_right;        ///< outward unit normal vector for right wall
    Vec3D diff_radial;
    Vec3D diff_normal;   
    Vec3D common_axis;
    Vec3D angularShift;
};
#endif
