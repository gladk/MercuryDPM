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

#ifndef BaseBoundary_H
#define BaseBoundary_H

#include "BaseObject.h"
#include "Math/ExtendedMath.h"

class BoundaryHandler;
class ParticleHandler;
class BaseParticle;
class DPMBase;

/*!
 * \class BaseBoundary
 * \brief
 * \details
 */
class BaseBoundary : public BaseObject
{
public:
    /*!
     * \brief default constructor.
     */
    BaseBoundary();

    /*!
     * \brief copy constructor
     */
    BaseBoundary(const BaseBoundary &b);
    
    /*!
     * \brief destructor
     */
    virtual ~BaseBoundary();

    /*!
     * \brief 
     * \return
     */
    virtual BaseBoundary* copy() const = 0;
    
    /*!
     * \brief
     */
    void read(std::istream& is) = 0;
    
    /*!
     * \brief
     */
    void write(std::ostream& os) const = 0;

    /*!
     * \brief
     * \param[in]
     */    
    virtual void createPeriodicParticles(BaseParticle *P UNUSED, ParticleHandler &pH UNUSED);

    /*!
     * \brief
     * \param[in]
     */
    virtual bool checkBoundaryAfterParticleMoved(BaseParticle *P UNUSED, ParticleHandler &pH UNUSED);

    /*!
     * \brief
     * \param[in]
     */
    virtual void checkBoundaryBeforeTimeStep(DPMBase* md UNUSED);

    /*!
     * \brief
     * \param[in]
     */
    void setHandler(BoundaryHandler* handler);

    /*!
     * \brief
     * \return
     */
    BoundaryHandler* getHandler() const;

private:

    /*!
     * \brief
     */
    BoundaryHandler* handler_;
};
#endif
