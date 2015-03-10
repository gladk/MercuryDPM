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

#include "BaseBoundary.h"
#include "Math/Vector.h"

class BaseParticle;
class ParticleHandler;

    /*!
     * \class AngledPerioidicBoundary
     * \brief Defines a pair of periodic walls that are angled around the origin. 
     * \details The particles are in {x: normal_left*(x-origin)>0 && normal_right*(x-origin)<0, with normal* being the unit normal vector of the walls. If a particle moves outside these boundaries, it will be shifted.
     */
class AngledPeriodicBoundary : public BaseBoundary
{
public:
    /*!
     * \brief 
     */    
    virtual AngledPeriodicBoundary* copy() const;
    
    //todo constructors instead of set functions?
    //AngledPeriodicBoundary (Vec3D normal_left_, Vec3D normal_right_, Vec3D origin_) 
    
    /*!
     * \brief Defines a periodic wall.
     * \details given a normal vector s.t. all particles are within {x: position_left<=normal*x<position_right}. The shift vector is set assuming that the domain is rectangular (shift parallel to normal).
     *
     * \param[in]
    */
    void set(Vec3D normal_left_, Vec3D normal_right_, Vec3D origin_);
    
    /*!
     * \brief Returns the distance of the wall to the particle and 
    sets left_wall = true, if the left wall is the wall 
    closest to the particle.
    * \details Since this function should be called before calculating any Particle-Wall interactions, it can also be used to set the shift vector in case of curved walls.
    *
    */
    Mdouble distance(const BaseParticle &P);
    
    /*!
     * \brief 
     */
    //this function should be cheap, as it has to be computed for all particles
    Mdouble distance(const Vec3D &P);
    
    /*!
     * \brief shifts the particle (after distance set the left_wall value)
     * \todo add velocity, angular, springs shift
     */
    void shift_position(BaseParticle* P);
    
    /*!
     * \brief reads wall
     */
    void read(std::istream& is);
    
    /*!
     * \brief 
     */
    void oldRead(std::istream& is);
    
    /*!
     * \brief outputs wall
     */
    void write(std::ostream& os) const;
    
    /*!
     * \brief Returns the name of the object
     */
    virtual std::string getName() const;
    
    /*!
     * \brief 
     */
    Vec3D& get_normal();
    
    /*!
     * \brief 
     * \param[in]
     */
    void createPeriodicParticles(BaseParticle *P, ParticleHandler &pH);
    
    /*!
     * \brief 
     * \todo{TW: can't we do the shift in the createPeriodicParticles function only? We are checking the distance three times!}
     */
    bool checkBoundaryAfterParticleMoved(BaseParticle *P, ParticleHandler &pH UNUSED);
    
private:
    //values set by the user
    /*!
     * \brief 
     */
    Vec3D normal_left; ///< outward unit normal vector for left wall

    /*!
     * \brief outward unit normal vector for right wall
     */
    Vec3D normal_right;

    /*!
     * \brief common point of both walls
     */
    Vec3D origin; 
    //values set by the code
    /*!
     * \brief true if closest wall is the left wall
     */
    bool left_wall;
    /*!
     * \brief outward unit normal vector for left wall
     */
    Vec3D radialAxis_left; 
    /*!
     * \brief outward unit normal vector for right wall
     */
    Vec3D radialAxis_right; 
    /*!
     * \brief 
     */
    Vec3D diff_radial;
    /*!
     * \brief 
     */
    Vec3D diff_normal;
    /*!
     * \brief 
     */
    Vec3D common_axis;
    /*!
     * \brief 
     */
    Vec3D angularShift;
};
#endif
