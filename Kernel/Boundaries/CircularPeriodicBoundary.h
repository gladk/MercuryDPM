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

#ifndef CircularPeriodicBoundary_H
#define CircularPeriodicBoundary_H

#include "BaseBoundary.h"

class ParticleHandler;
class BaseParticle;

/*!
 * \class CircularPeriodicBoundary
 * \brief used to create a circular periodic boundary
 */
class CircularPeriodicBoundary : public BaseBoundary
{
public:
    /*!
     * \brief default constructor
     */    
    CircularPeriodicBoundary();

    /*!
     * \brief Constructor
     */    
    CircularPeriodicBoundary(double innerRadius);
    
    /*!
     * \brief destructor
     */
    ~CircularPeriodicBoundary();
    
    /*!
     * \brief
     */
    CircularPeriodicBoundary* copy() const;
    
    /*!
     * \brief
     * \param[in]
     */
    void rotateParticle(BaseParticle *P, double angle);
    
    /*!
     * \brief 
     * \param[in]
     */
    void createPeriodicParticles(BaseParticle *P, ParticleHandler &pH);
    
    /*!
     * \brief
     * \param[in]
     * \return
     */
    bool checkBoundaryAfterParticleMoved(BaseParticle *P, ParticleHandler &pH);
    
    /*!
     * \brief reads the CircularPeriodicBoundary
     */
    void read(std::istream& is);
    
    /*!
     * \brief
     */
    void oldRead(std::istream& is);
    
    /*!
     * \brief outputs the CircularPeriodicBoundary
     */
    void write(std::ostream& os) const;
    
    /*!
     * \brief Returns the name of the object
     * \return string type
     */
    virtual std::string getName() const;
    
private:
    
    /*!
     * \brief
     */
    double innerRadius_;
    
    //A particle is between to Radii in plane (i) and has two (straight) walls (plane 0 defines centre)
    //If it is close to its inner Radius it should be copied once with a positive rotation of 2*pi/2^i
    //If it is close to one of its straight walls is should be rotative with +/- 2*pi/2^i
    
    //Particles can only apply forces to real particles
    
    //If a particle crosses a straight wall it should simply be shifted
    //If a particle crosses its inner Radius it should be coppied
    //If a particle crosses its outer Radius it may need to be deleted
    
};
#endif
