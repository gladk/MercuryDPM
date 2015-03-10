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

#ifndef PeriodicBoundary_H
#define PeriodicBoundary_H

#include "BaseBoundary.h"
#include "Math/Vector.h"

class ParticleHandler;

  /*!
   * \brief Defines a pair of periodic walls.
   * \details The particles are in {x: position_left<=normal*x <position_right}, with normal being the outward unit normal vector of the right wall. If a particle moves outside these boundaries, it will be shifted.
   */

class PeriodicBoundary : public BaseBoundary
{
public:
  /*!
   * \brief default constructor
   */
    PeriodicBoundary();
    
  /*!
   * \brief destructor
   */
    ~PeriodicBoundary();
    
  /*!
   * \brief
   */
    PeriodicBoundary* copy() const;
    
  /*!
   * \brief Defines a periodic wall, given a normal vector s.t. all particles are within {x: position_left<=normal*x<position_right}. The shift vector is set assuming that the domain is rectangular (shift parallel to normal).
   * \param[in]
   */
    void set(Vec3D normal, Mdouble distanceLeft, Mdouble distanceRight);
    
  /*!
   * \brief For general parallelogramic domains, the direction of the shift vector can to be set manually.
   * \param[in]
   */
    void set(Vec3D normal, Mdouble distanceLeft, Mdouble distanceRight, Vec3D shiftDirection);
    
  /*!
   * \brief Allows the left periodic wall to be moved to a new position and automatically changes its shift value
   * \param[in]
   */
    void moveLeft(Mdouble distanceLeft);
    
  /*!
   * \brief Allows the right periodic wall to be moved to a new position and automatically changes its shift value
   * \param[in]
   */
    void moveRight(Mdouble distanceRight);
    
  /*!
   * \brief Returns the distance of the wall to the particle, and sets left_wall = true.
   * \details if the left wall is the wall closest to the particle. Since this function should be called before calculating any Particle-Wall interactions, it can also be used to set the shift vector in case of curved walls.
   * \param[in]
   */
    Mdouble getDistance(BaseParticle &p);
  /*!
   * \brief
   * \param[in]
   */    
    Mdouble getDistance(const Vec3D &position);
    
  /*!
   * \brief shifts the particle (after distance set the left_wall value)
   * \param[in]
   */
    void shiftPosition(BaseParticle* p);
    
    /*
    ///returns the shifted particle w/o actually shifting
    Vec3D getShiftedPosition(Vec3D &Position);
    */
    
  /*!
   * \brief shifts two particles
   */
    void shiftPositions(Vec3D &postition1, Vec3D &postion2);

  /*!
   * \brief
   */
    bool isClosestToLeftBoundary() const;

    /*
    ///shift P such that it is closest to Q
    void getCloseTogether(Vec3D &P, Vec3D &Q);
    */
    
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
    
    /*
    Vec3D& getNormal();
     */
    
  /*!
   * \brief
   */
    void createPeriodicParticles(BaseParticle *p, ParticleHandler &pH);
    
  /*!
   * \brief
   */
    bool checkBoundaryAfterParticleMoved(BaseParticle *p, ParticleHandler &pH UNUSED);
    
private:
  /*!
   * \brief true if closest wall is the left wall
   */
    bool closestToLeftBoundary_;
  /*!
   * \brief outward unit normal vector for right wall
   */
    Vec3D normal_;
  /*!
   * \brief position of left wall, s.t. normal*x=position_left
   */
    Mdouble distanceLeft_;
  /*!
   * \brief position of right wall, s.t. normal*x=position_right
   */
    Mdouble distanceRight_; 
  /*!
   * \brief This is the normal to rescale the normal vector to a unit vectors.
   */
    Mdouble scaleFactor_;  
  /*!
   * \brief shift from left to right boundary
   */
    Vec3D shift_;
};
#endif
