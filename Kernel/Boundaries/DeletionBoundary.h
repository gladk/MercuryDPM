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
     * \brief
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
     * \brief
     */    
    DeletionBoundary* copy() const;
    
    /*!
     * \brief
     * \param[in]
     */
    void set(const Vec3D& normal, Mdouble distance);
    
    /*!
     * \brief
     * \param[in]
     */
    void move(Mdouble position);
    
    /*!
     * \brief
     * \param[in]
     */
    Mdouble getDistance(const Vec3D &position) const;
    
    /*!
     * \brief
     * \return Returns true if the particle is deleted
     */
    bool checkBoundaryAfterParticleMoved(BaseParticle *p, ParticleHandler &pH);

    /*!
     * \brief reads the DeletionBoundary
     */
    void read(std::istream& is);

    /*!
     * \brief
     */
    void oldRead(std::istream& is);

    /*!
     * \brief outputs the DeletionBoundary
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
     * \brief This is the normal to rescale the normal vector to a unit vectors.  
     */
    Mdouble scaleFactor_; 
    /*!
     * \brief position n*x=p
     */
    Mdouble distance_;
};

#endif
