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

#ifndef BOUNDARIES_CHUTEINSERTIONBOUNDARY_H
#define BOUNDARIES_CHUTEINSERTIONBOUNDARY_H

#include "InsertionBoundary.h"
#include "Math/Vector.h"

class BaseParticle;
class RNG;

    /*!
     * \class ChuteInsertionBoundary
     * \brief
     */
class ChuteInsertionBoundary : public InsertionBoundary
{
public:
    /*!
     * \brief
     */
    virtual ChuteInsertionBoundary* copy() const;
    
    /*!
     * \brief
     */
    void set(BaseParticle* particleToCopy, unsigned int maxFailed, Vec3D posMin, Vec3D posMax, double radMin, double radMax, double fixedParticleRadius, double inflowVelocity, double inflowVelocityVariance);
    
    /*!
     * \brief
     */
    virtual BaseParticle* generateParticle(RNG &random);
    
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
    
private:
    /*!
     * \brief
     */
    Vec3D posMin_, posMax_;

    /*!
     * \brief
     */
    double radMin_, radMax_;

    /*!
     * \brief
     */
    double fixedParticleRadius_, inflowVelocity_, inflowVelocityVariance_;
    
};

#endif
