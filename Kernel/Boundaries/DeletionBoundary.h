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
