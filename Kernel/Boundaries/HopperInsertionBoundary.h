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

#ifndef BOUNDARIES_HOPPERINSERTIONBOUNDARY_H
#define BOUNDARIES_HOPPERINSERTIONBOUNDARY_H

#include "InsertionBoundary.h"

class BaseParticle;
class RNG;

/*! 
 * \class HopperInsertionBoundary
 * \brief
 * \details
 */
class HopperInsertionBoundary : public InsertionBoundary
{
public:
  /*!
   * \brief
   */
    virtual HopperInsertionBoundary* copy() const;

  /*!
   * \brief
   * \param[in]
   */
    void set(BaseParticle* particleToCopy, int maxFailed, double yMin, double yMax, double radMin, double radMax,
            double chuteAngle, double fixedParticleRadius, bool isHopperCentred_, int hopperDim, double hopperAngle, double hopperLength, double hopperExitLength,
            double hopperHeight, double lift, double fillPercent);

  /*!
   * \brief This creates an inflow particle in the top 50% of the hopper i.e. between gamma=0.5 and gamma=1.0
   * \details Gamma is random number in the z direction and delta in the y direction. In the 2D (hopper) case the particles are generated with equal probability in the y-direction, i.e. delta is from the edge of the domain. In the 3D (hopper) case a third vector AD is generated and delta is again created for the sloping walls of the hopper.
   * \image html hopper_add_particle.jpg "Image shows the vectors in 2-dimension used to find a position inside the hopper"
   * \param[in]
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
   * \brief 
   *\return Returns the name of the object
   */
    virtual std::string getName() const;

  /*!
   * \brief
   */
    double yMin_, yMax_;

  /*!
   * \brief
   */
    double radMin_, radMax_;
  /*!
   * \brief
   */
    double chuteAngle_, hopperAngle_, fixedParticleRadius_;
  /*!
   * \brief
   */
    double hopperLength_, hopperExitLength_, hopperHeight_;
  /*!
   * \brief
   */
    bool isHopperCentred__;
  /*!
   * \brief
   */
    double fillPercent_;
  /*!
   * \brief
   */
    double lift_;
  /*!
   * \brief
   */
    int hopperDim_;
    
};

#endif
