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

#ifndef MASERBOUNDARY_H
#define MASERBOUNDARY_H

#include <map>

#include "BaseBoundary.h"
#include "Math/Vector.h"

class BaseSpecies;

class MaserBoundary : public BaseBoundary
{
public:
    /*!
     * \brief
     */
    MaserBoundary* copy() const;

    /*!
     * \brief Defines a periodic wall, given a normal vector s.t. all particles are within {x: position_left<=normal*x<position_right}. The shift vector is set assuming that the domain is rectangular (shift parallel to normal).
     * \param[in]
     */
    void set(Vec3D normal, Mdouble distanceLeft, Mdouble distanceRight);

    /*!
     * \brief reads wall
     */
    void read(std::istream& is);

    /*!
     * \brief outputs wall
     */
    void write(std::ostream& os) const;

    /*!
     * \brief Returns the name of the object
     */
    virtual std::string getName() const;

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
     * \brief
     */
    void shiftPosition(BaseParticle* p);

    /*!
     * \brief
     */
    void createPeriodicParticles(BaseParticle *p, ParticleHandler &pH);

    /*!
     * \brief
     */
    bool checkBoundaryAfterParticleMoved(BaseParticle *p, ParticleHandler &pH UNUSED);

    /*!
     * \brief
     */
    void addParticleToMaser(BaseParticle *P);

private:
    /*!
     * \brief Normal pointing in the flowing direction
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
     * \brief true if closest wall is the left wall
     */
    bool closestToLeftBoundary_;
    /*!
     * \brief shift from left to right boundary
     */
    Vec3D shift_;

    std::map<const BaseSpecies*,const BaseSpecies*> speciesConversionNormalToMaser_;
    std::map<const BaseSpecies*,const BaseSpecies*> speciesConversionMaserToNormal_;
};
#endif
