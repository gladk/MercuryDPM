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


#ifndef AngledPeriodicBoundary_H
#define AngledPeriodicBoundary_H

#include "BaseBoundary.h"
#include "Math/Vector.h"

class BaseParticle;
class ParticleHandler;

/*!
 * \class AngledPerioidicBoundary
 * \brief Defines a pair of periodic walls that are angled around the origin. 
 * \details The particles are in {x: normal_left*(x-origin)>0 && normal_right*(x-origin)<0, 
 * with normal* being the unit normal vector of the walls. If a particle moves outside 
 * these boundaries, it will be shifted.
 */
class AngledPeriodicBoundary : public BaseBoundary
{
public:
    
    /*!
     * \brief 
     */    
    AngledPeriodicBoundary* copy() const final;
    
    //todo constructors instead of set functions?
    //AngledPeriodicBoundary (Vec3D normal_left_, Vec3D normal_right_, Vec3D origin_) 
    
    /*!
     * \brief Defines a periodic wall.
    */
    void set(Vec3D normalLeft, Vec3D normalRight, Vec3D origin);
    
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
    void shiftPosition(BaseParticle* P);

    void shiftPositions(Vec3D&  P1, Vec3D& P2);

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
    Vec3D& getNormal();

    /*!
     * \brief angle between walls in radians
     */
    Mdouble getOpeningAngle();

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
    Vec3D leftNormal_; ///< outward unit normal vector for left wall

    /*!
     * \brief outward unit normal vector for right wall
     */
    Vec3D rightNormal_;

    /*!
     * \brief common point of both walls
     */
    Vec3D origin_; 
    //values set by the code
    /*!
     * \brief true if closest wall is the left wall
     */
    bool leftWall_;
    /*!
     * \brief outward unit normal vector for left wall
     */
    Vec3D leftRadialAxis_; 
    /*!
     * \brief outward unit normal vector for right wall
     */
    Vec3D rightRadialAxis_; 
    /*!
     * \brief 
     */
    Vec3D differenceRadialAxis_;
    /*!
     * \brief 
     */
    Vec3D differenceNormal_;
    /*!
     * \brief 
     */
    Vec3D commonAxis_;
    /*!
     * angularShift_ is currently unused; waiting for Quaternions to be implemented
     */
    //Vec3D angularShift_;
};
#endif
