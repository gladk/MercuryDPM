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

#ifndef DELETIONBOUNDARY_H
#define DELETIONBOUNDARY_H

#include "BaseBoundary.h"
#include "Math/Vector.h"

class ParticleHandler;
class BaseParticle;

    /*!
     * \class DeletionBoundary
     * \brief Used for removing particles from the problem.
     * Inherits from BaseBoundary.
     */
class DeletionBoundary : public BaseBoundary
{
public:
    /*!
     * \brief default constructor
     */
    DeletionBoundary();
    /*!
     * \brief destructor
     */    
    ~DeletionBoundary();
    /*!
     * \brief Copy method; creates copy on the heap and returns a pointer to it.
     */    
    DeletionBoundary* copy() const;
    
    /*!
     * \brief Sets boundary position based on a normal and distance.
     */
    void set(const Vec3D& normal, Mdouble distance);
    
    /*!
     * \brief Sets the boundary's distance property to the given one.
     */
    void move(Mdouble position);
    
    /*!
     * \brief Returns the shortest distance between the boundary and given position.
     */
    Mdouble getDistance(const Vec3D &position) const;
    
    /*!
     * \brief Checks if particle passed the boundary and deletes the particle if so.
     */
    bool checkBoundaryAfterParticleMoved(BaseParticle *p, ParticleHandler &pH);

    /*!
     * \brief Reads some boundary properties from an std::istream.
     */
    void read(std::istream& is);

    /*!
     * \brief Deprecated read method. use DeletionBoundary::read() instead.
     */
    MERCURY_DEPRECATED
    void oldRead(std::istream& is);

    /*!
     * \brief Writes the boundary properties to an std::ostream.
     */
    void write(std::ostream& os) const;
    
    /*!
     * \brief Returns the name of the object
     */
    virtual std::string getName() const;
    
private:
    /*!
     * \brief outward unit normal vector
     */
    Vec3D normal_; 
    /*!
     * \brief This is the factor to rescale the given normal vector to a unit vectors. 
     * \details NB: Not only the normal vector is rescaled by this factor, also 
     * the 'distance' from the origin of the boundary is scaled by this factor! Also,
     * once the boundary position is set with DeletionBoundary::set(), the arguments 
     * of any reset of the distance_ property  (i.e. usage of DeletionBoundary::move()) 
     * will be rescaled by the same factor!
     */
    Mdouble scaleFactor_; 
    /*!
     * \brief The boundary's distance from the origin.
     */
    Mdouble distance_;
};

#endif
