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
