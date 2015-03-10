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


#ifndef MASERBOUNDARY_H
#define MASERBOUNDARY_H

#include <map>

#include "BaseBoundary.h"
#include "Math/Vector.h"

class ParticleSpecies;

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

    std::map<const ParticleSpecies*,const ParticleSpecies*> speciesConversionNormalToMaser_;
    std::map<const ParticleSpecies*,const ParticleSpecies*> speciesConversionMaserToNormal_;
};
#endif
