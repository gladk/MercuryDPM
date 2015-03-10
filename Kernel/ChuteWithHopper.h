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

#ifndef CHUTEWITHHOPPER_H
#define CHUTEWITHHOPPER_H
#include "Chute.h"

/*!
 * \class ChuteWithHopper
 * \brief ChuteWithHopper has a hopper as inflow.
 * \details The hopper has two parts as follows to create the finite hopper walls, we take vector between two wall points in xz-plane, then rotate clockwise and make unit length.
 * \image html hopper.jpg "Sketch of the hopper"
 *  A,B,C denote three points on the left and right hopper walls which are used to construct the hopper. Shift denotes the space by which the chute has to be shifted to the right such that the hopper is in the domain. Note: the wall direction has to be set seperately either period of walls.
 */
class ChuteWithHopper : public Chute
{
public:

//Constructors:
    /*!
     * \brief This is a copy constructor for Chute problems \bug This copy construct is untested
     */
    ChuteWithHopper(const Chute& other);

    /*!
     * \brief
     */
    ChuteWithHopper(const Mercury3D& other);

    /*!
     * \brief
     */
    ChuteWithHopper(const MercuryBase& other);

    /*!
     * \brief
     */
    ChuteWithHopper(const DPMBase& other);

    /*!
     * \brief This is the default constructor
     */
    ChuteWithHopper();

//Setters and getters:

    /*!
     * \brief
     */
    void setHopperFillingPercentage(Mdouble hopperFillingPercentage);

    /*!
     * \brief
     */
    void setHopperLowestPoint(Mdouble hopperLowestPoint);

    /*!
     * \brief
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
     * \brief
     */
    void setIsHopperCentred(bool isHopperCentred);

    /*!
     * \brief
     */
    void setHopperLowerFillingHeight(Mdouble hopperLowerFillingHeight);

    /*!
     * \brief
     */
    void setHopperShift(Mdouble hopperShift);

    /*!
     * \brief This lifts the hopper above the plane of the chute
     */
    void setHopperLift(Mdouble hopperLift);

    /*!
     * \brief
     */
    Mdouble getHopperLift();
    
    /*!
     * \brief
     */
    Mdouble getHopperShift();

    /*!
     * \brief
     */
    void setHopperDimension(Mdouble hopperDimension);

    /*!
     * \brief
     */
    void setIsHopperAlignedWithBottom(bool isHopperAlignedWithBottom);

    /*!
     * \brief
     */
    Mdouble getHopperAngle();

    /*!
     * \brief
     */
    Mdouble getHopperLength();

    /*!
     * \brief
     */
    Mdouble getHopperExitLength();

    /*!
     * \brief
     */
    Mdouble getHopperHeight();

    /*!
     * \brief
     */
    Mdouble getHopperExitHeight();
    
    /*!
     * \brief
     */
    bool getIsHopperCentred();
    
    /*!
     * \brief
     */
    Mdouble getHopperFillingPercentage();
    
    /*!
     * \brief
     */
    unsigned int getHopperDimension();

//Other member functions:

    /*!
     * \brief
     */
    virtual void setupInitialConditions();

    /*!
     * \brief
     */
    void setHopper(Mdouble ExitLength, Mdouble ExitHeight, Mdouble Angle, Mdouble Length, Mdouble Height);

    /*!
     * \brief
     */
    Mdouble getMaximumVelocityInducedByGravity();

    /*!
     * \brief This function reads all chute data
     */
    virtual void read(std::istream& is);

    /*!
     * \brief
     */
    void write(std::ostream& os, bool writeAllParticles=true) const;

    /*!
     * \brief
     */
    int readNextArgument(int& i, int argc, char *argv[]);

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
     * \brief The z coordinate of the right C point (when the left C point is in the origin)
     */
    Mdouble hopperLowestPoint_;
};
#endif
