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

/*!
 * \brief Variation on the PeriodicBoundary with maser-like properties 
 * \details Creates a boundary which is similar to a PeriodicBoundary, except that
 * particles which leave 
 * 
 * 
 */
class MaserBoundary : public BaseBoundary
{
public:
    /*!
     * \brief Creates a copy on the heap.
     */
    MaserBoundary* copy() const;

    /*!
     * \brief Sets all boundary properties at once.
     */
    void set(Vec3D normal, Mdouble distanceLeft, Mdouble distanceRight);

    /*!
     * \brief reads boundary properties from istream
     */
    void read(std::istream& is);

    /*!
     * \brief writes boundary properties to ostream
     */
    void write(std::ostream& os) const;

    /*!
     * \brief Returns the name of the object
     */
    virtual std::string getName() const;

    /*!
     * \brief Returns the distance of the wall to the particle
     */
    Mdouble getDistance(BaseParticle &p);

    /*!
     * \brief Returns the distance of the wall to the position
     */
    Mdouble getDistance(const Vec3D &position);

    /*!
     * \brief shifts the particle to its 'periodic' position
     */
    void shiftPosition(BaseParticle* p);

    /*!
     * \brief Creates periodic particle when the particle is a maser particle 
     * and is sufficiently close to one of the boundary walls.
     */
    void createPeriodicParticles(BaseParticle *p, ParticleHandler &pH);

    /*!
     * \brief Shifts the particle to its 'periodic' position if it is a maser particle
     * and has crossed either of the walls. Creates a 'normal' particle at its current
     * position if it is a maser particle which crossed the RIGHT boundary wall.
     */
    bool checkBoundaryAfterParticleMoved(BaseParticle *p, ParticleHandler &pH UNUSED);

    /*!
     * \brief Converts a 'normal' particle into a maser particle. 
     */
    void addParticleToMaser(BaseParticle *P);

private:
    /*!
     * \brief Normal unit vector of both maser walls. Points in the flowing direction.
     */
    Vec3D normal_;
    /*!
     * \brief position of left boundary wall, s.t. normal*x=position_left
     */
    Mdouble distanceLeft_;
    /*!
     * \brief position of right boundary wall, s.t. normal*x=position_right
     */
    Mdouble distanceRight_;
    /*!
     * \brief Dummy variable used when checking the proximity of particles to the 
     * boundary walls. TRUE if closest wall is the left boundary wall.
     */
    bool closestToLeftBoundary_;
    /*!
     * \brief Direction in which particles are to be shifted when they cross the boundary.
     * \details I.e., the vector pointing from a point the left boundary wall to the equivalent point
     * on the right one.
     */
    Vec3D shift_;
    
    /*!
     * \brief List of 'normal' particles' species, and their maser counterparts
     */
    std::map<const ParticleSpecies*,const ParticleSpecies*> speciesConversionNormalToMaser_;
    
    /*!
     * \brief List of 'maser' particles' species, and their normal counterparts
     */
    std::map<const ParticleSpecies*,const ParticleSpecies*> speciesConversionMaserToNormal_;
};
#endif
