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
 * \brief Inherits from InsertionBoundary
 * Some images are useful to better understand the structure of both the hopper-chute
 * combination, as of the hopper insertion boundary itself:
 * 
 * \image html hopper.jpg "Image clarifying the definitions of the data members hopperLength_, hopperExitLength, hopperHeight_ and hopperAngle_."
 * 
 * \image html hopper_add_particle.jpg "Image shows the AC and AB directions used to determine a particle position inside the hopper. NB: the X and Z directions are in the same plane, but pointed along the chute and normal to the chute, respectively."
 * 
 * \todo (BvdH) Better graphical diagrams are needed for further clarification, as 
 * well as some property renaming.
 */
class HopperInsertionBoundary : public InsertionBoundary
{
public:
        
  /*!
   * \brief Default constructor. Sets all properties to 0.
   */
    HopperInsertionBoundary();
    
  /*!
   * \brief Copy constructor
   */
    HopperInsertionBoundary(const HopperInsertionBoundary &other);
    
  /*!
   * \brief copy method, returns a pointer to a copy.
   */
    virtual HopperInsertionBoundary* copy() const;

    /*!
     * \brief Sets all boundary properties at once.
     */
    void set(BaseParticle* particleToCopy, int maxFailed, double yMin, double yMax, double radMin, double radMax,
            double chuteAngle, double fixedParticleRadius, bool isHopperCentred_, int hopperDim, double hopperAngle, double hopperLength, double hopperExitLength,
            double hopperHeight, double lift, double fillPercent);

  /*!
   * \brief This creates an inflow particle in the top 50% of the hopper i.e. between gamma=0.5 and gamma=1.0
   */
    virtual BaseParticle* generateParticle(RNG &random);

    /*!
     * \brief reads boundary properties from istream
     */
    void read(std::istream& is);
 
    /*!
     * \brief deprecated version of CubeInsertionBoundary::read().
     */
    MERCURY_DEPRECATED
    void oldRead(std::istream& is);
    
    /*!
     * \brief writes boundary properties to ostream
     */
    void write(std::ostream& os) const;
    
    /*!
     * \brief Returns the name of the object
     */
    virtual std::string getName() const;

    
    /*!
     * \brief  The minimum and maximum y-positions of the particle for a hopper 
     * with vertical walls in the y-direction ('1D hopper'). For the '2D hopper', 
     * these variables have no physical meaning, except for the fact that the insertion
     * boundary is centered in the y-direction at (yMax_ - yMin_)/2 .
     */
    double yMin_, yMax_;

  /*!
   * \brief Minimum and maximum radii of the inserted particles
   */
    double radMin_, radMax_;
  /*!
   * \brief Angle of the chute as compared to the horizontal plane
   */
    double chuteAngle_; 
    
    /*!
     * \brief Angle of the hopper as compared to the vertical plane
     */
    double hopperAngle_;
    
    /*!
     * \brief 
     */
    double fixedParticleRadius_;
    
  /*!
   * \brief The horizontal (AB-direction) width at the top of the hopper
   */
    double hopperLength_;
    
  /*!
   * \brief The horizontal (AB-direction) width at the square exit of the hopper
   */
    double hopperExitLength_;
    
  /*!
   * \brief The vertical (AC-direction) height of the hopper, measured from the top of 
   * the hopper to the start of the chute.
   */
    double hopperHeight_;
    
  /*!
   * \brief
   */
    bool isHopperCentred__;

    /*!
     * \brief Percentage of the height of the insertion boundary up to which it should 
     * be filled. The part to be filled reaches from the top of the hopper down to 
     * {fillPercent * (top - 'position A')}. 
     */
    double fillPercent_;
    
  /*!
   * \brief
   */
    double lift_;

    /*!
     * \brief Either 1 or 2. If 1, the insertion boundary has vertical walls in the 
     * y extrema. This is used e.g. for a hopper with periodic walls in the y-direction.
     * If 2, the insertion boundary has the form of  an inverted (truncated) pyramid, 
     * with equally inclined walls in both AB and Y directions.
     */
    int hopperDim_;
    
};

#endif
