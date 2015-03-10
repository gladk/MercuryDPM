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

#ifndef CHUTEWITHHOPPER_H
#define CHUTEWITHHOPPER_H
#include "Chute.h"

/*!
 * \class ChuteWithHopper
 * \brief ChuteWithHopper has a hopper as inflow.
 * \details The hopper has two parts as follows to create the finite hopper walls, we take vector between two wall points in xz-plane, then rotate clockwise and make unit length.
 * \image html hopper.jpg "Sketch of the hopper"
 *  A,B,C denote three points on the left and right hopper walls which are used to construct the hopper. Shift denotes the space by which the chute has to be shifted to the right such that the hopper is in the domain. Note: the wall direction has to be set separately either period of walls.
 */
class ChuteWithHopper : public Chute
{
public:

//Constructors:
    /*!
     * \brief This is a copy constructor for Chute problems 
     * \bug This copy construct is untested
     */
    ChuteWithHopper(const Chute& other);

    /*!
     * \brief Copy constructor, converts an existing Mercury3D object into a ChuteWithHopper object
     */
    ChuteWithHopper(const Mercury3D& other);

    /*!
     * \brief Copy constructor, converts an existing MercuryBase object into a ChuteWithHopper object
     */
    ChuteWithHopper(const MercuryBase& other);

    /*!
     * \brief Copy constructor, converts an existing DPMBase object into a ChuteWithHopper object
     */
    ChuteWithHopper(const DPMBase& other);

    /*!
     * \brief This is the default constructor
     */
    ChuteWithHopper();

//Setters and getters:

    /*!
     * \brief Sets the hopper filling percentage. 
     */
    void setHopperFillingPercentage(Mdouble hopperFillingPercentage);

    /*!
     * \brief Sets the vertical distance of the lowest hopper point relative to the start
     * of the chute.
     */
    void setHopperLowestPoint(Mdouble hopperLowestPoint);

    /*!
     * \brief Returns the vertical distance of the lowest hopper point relative to the start
     * of the chute.
     */
    Mdouble getHopperLowestPoint();

    /*!
     * \brief Allows chute length to be accessed
     */
    Mdouble getChuteLength();

    /*!
     * \brief sets xMax to chuteLength+hopperlength_, and thus specifies the length off the runoff chute
     */
    void setChuteLength(Mdouble chuteLength);

    /*!
     * \brief Sets an extra shift in X-direction of the whole system
     */
    void setIsHopperCentred(bool isHopperCentred);

    /*!
     * \brief Sets the height above which the hopper is filled with new particles
     */
    void setHopperLowerFillingHeight(Mdouble hopperLowerFillingHeight);

    /*!
     * \brief Sets the shift in X-direction of the whole setup after rotation
     */
    void setHopperShift(Mdouble hopperShift);

    /*!
     * \brief This lifts the hopper above the plane of the chute (after rotation)
     */
    void setHopperLift(Mdouble hopperLift);

    /*!
     * \brief Returns the hopper's lift above the chute bottom plane 
     */
    Mdouble getHopperLift();
    
    /*!
     * \brief Returns the shift in X-direction of the whole setup after rotation
     */
    Mdouble getHopperShift();

    /*!
     * \brief Sets whether the hopper should have vertical (1) or inclined (2) walls
     * in Y-direction
     */
    void setHopperDimension(Mdouble hopperDimension);

    /*!
     * \brief Sets the alignment of hopper with chute bottom
     */
    void setIsHopperAlignedWithBottom(bool isHopperAlignedWithBottom);

    /*!
     * \brief Returns the angle of the hopper entrance relative to the vertical
     */
    Mdouble getHopperAngle();

    /*!
     * \brief Returns the width of the hopper entrance
     */
    Mdouble getHopperLength();

    /*!
     * \brief Returns the width of the hopper exit
     */
    Mdouble getHopperExitLength();

    /*!
     * \brief Returns the height of the hopper relative to the chute start
     */
    Mdouble getHopperHeight();

    /*!
     * \brief Returns the height of the lowest hopper point above the chute
     */
    Mdouble getHopperExitHeight();
    
    /*!
     * \brief Returns whether the setup is shifted another 40 units in X-direction
     */
    bool getIsHopperCentred();
    
    /*!
     * \brief Returns the vertical percentage of the hopper insertion boundary which 
     * is filled 
     */
    Mdouble getHopperFillingPercentage();
    
    /*!
     * \brief Returns whether the hopper has vertical (1) or inclined (2) walls in Y-direction
     */
    unsigned int getHopperDimension();

//Other member functions:

    /*!
     * \brief Sets up the initial conditions for the problem
     */
    virtual void setupInitialConditions();

    /*!
     * \brief Sets the hopper's geometrical properties
     */
    void setHopper(Mdouble ExitLength, Mdouble ExitHeight, Mdouble Angle, Mdouble Length, Mdouble Height);

    /*!
     * \brief Returns the theoretical maximum particle velocity due to gravity
     */
    Mdouble getMaximumVelocityInducedByGravity();
    
    /*!
     * \brief Returns smallest particle radius over maximum gravitational velocity
     */
    Mdouble getTimeStepRatio();
    
    /*!
     * \brief Reads setup properties from an istream
     */
    virtual void read(std::istream& is);

    /*!
     * \brief Writes setup properties to an ostream
     */
    void write(std::ostream& os, bool writeAllParticles=true) const;

    /*!
     * \brief Reads setup properties from a string
     */
    bool readNextArgument(int& i, int argc, char *argv[]);

protected:
    /*!
     * \brief This creates the hopper on top of the chute, see diagram in class description for details of the points.
     */
    void addHopper();

private:

    /*!
     * \brief This is the actually constructor, get called by all constructors above
     */
    void constructor();

//protected:
    /*!
     * \brief Dimension of the hopper in vertical direction
     */
    Mdouble hopperLength_;
    /*!
     * \brief Dimension of the hopper in horizontal direction
     */
    Mdouble hopperHeight_;
    /*!
     * \brief Angle between the two pieces of the hopper walls
     */
    Mdouble hopperAngle_;
    /*!
     * \brief Dimension of the hopper exit in vertical direction
     */
    Mdouble hopperExitLength_;
    /*!
     * \brief Dimension of the hopper exit in vertical direction
     */
    Mdouble hopperExitHeight_;
    /*!
     * \brief The x position where the Chute starts (defined as the beginning of the hopper)
     */
    Mdouble hopperShift_;
    /*!
     * \brief Relative height (in [0,1)) above which the hopper is replenished with new particles
     */
    Mdouble hopperLowerFillingHeight_;
    /*!
     * \brief If this flag is set, the hopper will be constructed in the xy-center of the domain, and not next to the xmin-domain boundary; by default off
     */
    bool isHopperCentred_;

    /*!
     * \brief This is the vertical distance the chute is lifted above the plane.
     */
    Mdouble hopperLift_;
    /*!
     * \brief This is the dimension of the hopper, my default it is one dimensional and hence does not have side wall
     */
    unsigned int hopperDimension_;
    /*!
     * \brief This is the flag, which sets if the chute bottom is aligned with the hopper, by default it is
     */
    bool isHopperAlignedWithBottom_;
    /*!
     * \brief This is which percentage of the hopper is used for creating new partices;
     */
    Mdouble hopperFillingPercentage_;
    /*!
     * \brief The NEGATIVE z coordinate of the right C point (when the left C point is in the origin)
     */
    Mdouble hopperLowestPoint_;
};
#endif
