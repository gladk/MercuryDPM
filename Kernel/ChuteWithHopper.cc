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

#include "ChuteWithHopper.h"
#include "Boundaries/HopperInsertionBoundary.h"
#include "Walls/IntersectionOfWalls.h"
#include <cstring>
#include <cmath>

//This is a copy constructor for Chute problems 
///\bug This copy construct is untested
/*!
 * \details Copy constructor with a Chute object as an argument. This constructor
 * basically 'upgrades' the Chute object to one of the ChuteWithHopper class.
 * NB: The copy constructor of DPMBase has to be called because the link from DPMBase 
 * to MercuryBase is virtual.
 * \param[in] other     object of Chute class
 */
ChuteWithHopper::ChuteWithHopper(const Chute& other)
: DPMBase(other), Chute(other)
{
    constructor();
}

/*!
 * \details Copy constructor with a Mercury3D object as an argument. This constructor
 * basically 'upgrades' the Mercury3D object to one of the ChuteWithHopper class.
 * NB: The copy constructor of DPMBase has to be called because the link from DPMBase 
 * to MercuryBase is virtual.
 * \param[in] other     object of Mercury3D class
 */
ChuteWithHopper::ChuteWithHopper(const Mercury3D& other)
: DPMBase(other), Chute(other)
{
    constructor();
}

/*!
 * \details Copy constructor with a MercuryBase object as an argument. This constructor
 * basically 'upgrades' the MercuryBase object to one of the ChuteWithHopper class.
 * NB: The copy constructor of DPMBase has to be called because the link from DPMBase 
 * to MercuryBase is virtual.
 * \param[in] other     object of MercuryBase class
 */
ChuteWithHopper::ChuteWithHopper(const MercuryBase& other)
: DPMBase(other), Chute(other)
{
    constructor();
}

/*!
 * \details Copy constructor with a DPMBase object as an argument. This constructor
 * basically 'upgrades' the DPMBase object to one of the ChuteWithHopper class.
 * NB: The copy constructor of DPMBase has to be called because the link from DPMBase 
 * to MercuryBase is virtual.
 * \param[in] other     object of DPMBase class
 */
ChuteWithHopper::ChuteWithHopper(const DPMBase& other)
: DPMBase(other), Chute(other)
{
    constructor();
}

/*!
 * \details Default constructor. Calls the constructor() method.
 */
ChuteWithHopper::ChuteWithHopper()
{
    constructor();
}

/*!
 * \details constructor METHOD, which sets all ChuteWithHopper properties to something sensible.
 */
void ChuteWithHopper::constructor()
{
    hopperLowerFillingHeight_ = 0.5;
    hopperLift_ = 0.0;
    setHopper(0.01, 0.01, 60.0, 0.08, 0.04);
    hopperShift_ = 0.0;
    hopperDimension_ = 1;
    isHopperAlignedWithBottom_ = true;

    hopperFillingPercentage_ = 50.0;
    isHopperCentred_ = false;

}

/*!
 * \details Sets the hopper filling percentage. See also the documentation of the 
 * HopperInsertionBoundary class.
 * \param[in] hopperFillingPercentage   Percentage of the height of the hopper insertion boundary up to which it should 
 *                                      be filled. The part to be filled reaches from the top of the hopper down to 
 *                                      {hopperFillingPercentage * (top - 'position A')}. 
 */
void ChuteWithHopper::setHopperFillingPercentage(Mdouble hopperFillingPercentage)
{
    hopperFillingPercentage_ = hopperFillingPercentage;
}

/*!
 * \details Sets up the problem initial conditions:
 *      1.  Creates chute side walls
 *      2.  Creates a hopper insertion boundary
 *      3.  Creates a chute bottom
 *      4.  Creates a hopper at the start of the chute
 */
void ChuteWithHopper::setupInitialConditions()
{
    
    // check time step ratio
    getTimeStepRatio();
    
    // create chute side walls (either periodic or solid, based on (the inherited) 
    // boolean Chute::isChutePeriodic_ data member).
    setupSideWalls();
    
    // create insertion boundary for the hopper and set a fill percentage
    HopperInsertionBoundary b1;
    BaseParticle* p1 = new BaseParticle();
    p1->setSpecies(speciesHandler.getObject(0));
    b1.set(p1, getMaxFailed(), getYMin(), getYMax(), getMinInflowParticleRadius(), getMaxInflowParticleRadius(),
            getChuteAngle(), getFixedParticleRadius(), isHopperCentred_, hopperDimension_, hopperAngle_, hopperLength_,
            hopperExitLength_,hopperHeight_, hopperLift_, hopperFillingPercentage_);
    boundaryHandler.copyAndAddObject(b1);
    setInsertionBoundary(dynamic_cast<InsertionBoundary*>(boundaryHandler.getLastObject()));
    
    // create the chute bottom
    createBottom();

    // create the hopper
    addHopper();
}

/*!
 * \details Creates the actual hopper and rotates it so as to fit in a geometry in
 * which the chute is parallel to the X-axis.
 */
void ChuteWithHopper::addHopper()
{
    //hopper walls
    //to create the finite hopper walls, we take vector between two wall points in xz-plane, then rotate clockwise and make unit length
    // A\       /A
    //   \     /       A,B,C denote three points on the left and right hopper walls which are used to construct the hopper
    //    \   /        shift denotes the space by which the chute has to be shifted to the right such that the hopper is in the domain
    //   B|   |B
    //    |   |
    //   C|   |
    //        |C

    Vec3D A, B, C, temp, normal;

    Mdouble s = sin(getChuteAngle());
    Mdouble c = cos(getChuteAngle());
    // height of the lowest part of the hopper (right C in diagram above) as compared to 
    // the vertical position of the start of the chute (left C in diagram above).
    hopperLowestPoint_ = hopperExitHeight_ - hopperExitLength_ * tan(getChuteAngle()); 
    
    // "0.5*(hopperLength_+hopperExitLength_) / tan(hopperAngle_)" is the minimum heigth of the hopper, to make sure things should flow down and not to the sides.
    // hopperHeight_ is now an input variable
    // hopperHeight_ = hopperLowestPoint_ + 1.1 * 0.5*(hopperLength_+hopperExitLength_) / tan(hopperAngle_);

    Mdouble HopperCornerHeight = hopperHeight_ - 0.5 * (hopperLength_ - hopperExitLength_) / tan(hopperAngle_);
    // Waarom had ik deze ook al weer gecomment?
    //if (HopperCornerHeight<=0.0) { hopperHeight_ += -HopperCornerHeight + P0.getRadius(); HopperCornerHeight = P0.getRadius(); }

    // first we create the LEFT hopper wall

    // coordinates of A,B,C in (vertical parallel to flow, vertical normal to flow, horizontal) direction
    A = Vec3D(-0.5 * (hopperLength_ - hopperExitLength_), 0.0, hopperHeight_);
    B = Vec3D(0.0, 0.0, HopperCornerHeight);
    C = Vec3D(0.0, 0.0, 0.0);

    // now rotate the coordinates of A,B,C to be in (x,y,z) direction
    A = Vec3D(c * A.X - s * A.Z, 0.0, s * A.X + c * A.Z);
    B = Vec3D(c * B.X - s * B.Z, 0.0, s * B.X + c * B.Z);
    C = Vec3D(c * C.X - s * C.Z, 0.0, s * C.X + c * C.Z);
   
    // the position of A determines hopper shift and zmax
    ///\todo Why shift A by arbitrary number of 40, when isHopperCentred_ = True?
    /// NB: this is probably the same shift of 40 as happens in HopperInsertionBoundary::generateParticle(RNG)
    /// with the calculation of 'point A'.
    /// NB2: seems to be just generating a nice 'view' of the hopper (in the center
    /// of the viewer...?)
    if (isHopperCentred_)
        setHopperShift(-A.X + 40);
    else
        setHopperShift(-A.X);
    
    setZMax(A.Z);
    A.X += hopperShift_;
    B.X += hopperShift_;
    C.X += hopperShift_;

    //This lifts the hopper a distance above the chute
    A.Z += hopperLift_;
    B.Z += hopperLift_;
    C.Z += hopperLift_;

    //create a finite wall from B to A and from C to B on the left hand side
    IntersectionOfWalls w_Left;
    temp = B - A;
    normal = Vec3D(temp.Z, 0.0, -temp.X) / sqrt(temp.getLengthSquared());
    w_Left.addObject(normal, A);
    temp = C - B;
    normal = Vec3D(temp.Z, 0.0, -temp.X) / sqrt(temp.getLengthSquared());
    w_Left.addObject(normal, B);
    temp = A - C;
    normal = Vec3D(temp.Z, 0.0, -temp.X) / sqrt(temp.getLengthSquared());
    w_Left.addObject(normal, C);
    wallHandler.copyAndAddObject(w_Left);

    //next, do the same for the right wall
    A = Vec3D(hopperLength_ - 0.5 * (hopperLength_ - hopperExitLength_), 0.0, hopperHeight_);
    B = Vec3D(0.5 * (hopperLength_ + hopperExitLength_) - 0.5 * (hopperLength_ - hopperExitLength_), 0.0, HopperCornerHeight);
    C = Vec3D(0.5 * (hopperLength_ + hopperExitLength_) - 0.5 * (hopperLength_ - hopperExitLength_), 0.0, hopperLowestPoint_);

    //This rotates the right points
    A = Vec3D(c * A.X - s * A.Z + hopperShift_, 0.0, s * A.X + c * A.Z);
    B = Vec3D(c * B.X - s * B.Z + hopperShift_, 0.0, s * B.X + c * B.Z);
    C = Vec3D(c * C.X - s * C.Z + hopperShift_, 0.0, s * C.X + c * C.Z);

    //This lifts the hopper a distance above the chute
    A.Z += hopperLift_;
    B.Z += hopperLift_;
    C.Z += hopperLift_;

    //create a finite wall from B to A and from C to B on the right hand side
    IntersectionOfWalls w_Right;
    temp = A - B;
    normal = Vec3D(temp.Z, 0.0, -temp.X) / sqrt(temp.getLengthSquared());
    w_Right.addObject(normal, A);
    temp = B - C;
    normal = Vec3D(temp.Z, 0.0, -temp.X) / sqrt(temp.getLengthSquared());
    w_Right.addObject(normal, B);
    temp = C - A;
    normal = Vec3D(temp.Z, 0.0, -temp.X) / sqrt(temp.getLengthSquared());
    w_Right.addObject(normal, C);
    wallHandler.copyAndAddObject(w_Right);

    setZMax(A.Z);
    
    // if hopperDimension_ == 2, create inclined hopper walls (like in the X-direction) also in the Y-direction.
    // (Else, place vertical (possibly periodic) walls in Y-direction. -> not mentioned here, where is this arranged? (BvdH))
    if (hopperDimension_ == 2)
    {
        //coordinates of A,B,C in (vertical parallel to flow,vertical normal to flow, horizontal) direction
        A = Vec3D(0.0, (getYMax() - getYMin() - hopperLength_) / 2.0, hopperHeight_);
        B = Vec3D(0.0, (getYMax() - getYMin() - hopperExitLength_) / 2.0, HopperCornerHeight);
        C = Vec3D(0.0, (getYMax() - getYMin() - hopperExitLength_) / 2.0, 0.0);

        //now rotate the coordinates of A,B,C to be in (x,y,z) direction
        A = Vec3D(c * A.X - s * A.Z, A.Y, s * A.X + c * A.Z);
        B = Vec3D(c * B.X - s * B.Z, B.Y, s * B.X + c * B.Z);
        C = Vec3D(c * C.X - s * C.Z, C.Y, s * C.X + c * C.Z);
        // the position of A determines shift and zmax
        A.X += hopperShift_;
        B.X += hopperShift_;
        C.X += hopperShift_;

        //This lifts the hopper a distance above the chute
        A.Z += hopperLift_;
        B.Z += hopperLift_;
        C.Z += hopperLift_;

        //create a finite wall from B to A and from C to B
        IntersectionOfWalls w_Back;
        temp = B - A;
        normal = Vec3D::cross(Vec3D(-c, 0, -s), temp) / sqrt(temp.getLengthSquared());
        //normal  = Vec3D(0.0,temp.Z,-temp.Y) / sqrt(temp.GetLength2());
        w_Back.addObject(normal, A);
        temp = C - B;
        //normal  = Vec3D(0.0,temp.Z,-temp.Y) / sqrt(temp.GetLength2());
        normal = Vec3D::cross(Vec3D(-c, 0, -s), temp) / sqrt(temp.getLengthSquared());
        w_Back.addObject(normal, B);
        temp = A - C;
        //normal = Vec3D(0.0,temp.Z,-temp.Y)/sqrt(temp.GetLength2());
        normal = Vec3D::cross(Vec3D(-c, 0, -s), temp) / sqrt(temp.getLengthSquared());
        w_Back.addObject(normal, C);
        wallHandler.copyAndAddObject(w_Back);

        //Now for the right y-wall
        A = Vec3D(0.0, (getYMax() - getYMin() + hopperLength_) / 2.0, hopperHeight_);
        B = Vec3D(0.0, (getYMax() - getYMin() + hopperExitLength_) / 2.0, HopperCornerHeight);
        C = Vec3D(0.0, (getYMax() - getYMin() + hopperExitLength_) / 2.0, 0.0);

        //now rotate the coordinates of A,B,C to be in (x,y,z) direction
        A = Vec3D(c * A.X - s * A.Z, A.Y, s * A.X + c * A.Z);
        B = Vec3D(c * B.X - s * B.Z, B.Y, s * B.X + c * B.Z);
        C = Vec3D(c * C.X - s * C.Z, C.Y, s * C.X + c * C.Z);
        // the position of A determines shift and zmax
        A.X += hopperShift_;
        B.X += hopperShift_;
        C.X += hopperShift_;

        //This lifts the hopper a distance above the chute
        A.Z += hopperLift_;
        B.Z += hopperLift_;
        C.Z += hopperLift_;

        //create a finite wall from B to A and from C to B
        IntersectionOfWalls w_Front;
        temp = A - B;
        normal = Vec3D::cross(Vec3D(-c, 0, -s), temp) / sqrt(temp.getLengthSquared());
        //normal  = Vec3D(0.0,-temp.Z,temp.Y) / sqrt(temp.GetLength2());
        w_Front.addObject(normal, A);
        temp = B - C;
        //normal  = Vec3D(0.0,-temp.Z,temp.Y) / sqrt(temp.GetLength2());
        normal = Vec3D::cross(Vec3D(-c, 0, -s), temp) / sqrt(temp.getLengthSquared());
        w_Front.addObject(normal, B);
        temp = C - A;
        //normal = Vec3D(0.0,-temp.Z,temp.Y)/sqrt(temp.GetLength2());
        normal = Vec3D::cross(Vec3D(-c, 0, -s), temp) / sqrt(temp.getLengthSquared());
        w_Front.addObject(normal, C);
        wallHandler.copyAndAddObject(w_Front);
    }

    //now shift the chute as well, i.e. apply the shift to all the fixed particles 
    // at the bottom of the chute
    for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it != particleHandler.end(); ++it)
    {
        (*it)->move(Vec3D(hopperShift_, 0.0, 0.0));
    }
}

/*!
 * \details Sets the height difference between left hopper bottom (where the chute starts) and 
 * right hopper bottom (which 'hovers' above the chute).
 * \param[in] hopperLowestPoint     the lowest point of the right side of the hopper
 */
void ChuteWithHopper::setHopperLowestPoint(Mdouble hopperLowestPoint)
{
    hopperLowestPoint_ = hopperLowestPoint;
}
Mdouble ChuteWithHopper::getHopperLowestPoint()
{
    return hopperLowestPoint_;
}

/*!
 * \details Sets all the geometrical properties of the hopper at once
 * \param[in] ExitLength    Horizontal width of the (rectangular) hopper exit
 * \param[in] ExitHeight    The vertical position of the lowest point of the right side 
 *                          of the hopper relative to the chute bottom
 * \param[in] Angle         The angle of the hopper inlet, relative to the vertical 
 *                          (in DEGREES)
 * \param[in] Length        Horizontal width of the hopper entrance (i.e., at the top
 *                          of the hopper)
 * \param[in] Height        The (vertical) height of the hopper relative to the start
 *                          of the chute
 */
void ChuteWithHopper::setHopper(Mdouble ExitLength, Mdouble ExitHeight, Mdouble Angle, Mdouble Length, Mdouble Height)
{
    // HopperCornerHeight: helper variable, just here to check some things
    Mdouble HopperCornerHeight = Height - 0.5 * (Length - ExitLength) / std::tan(Angle * constants::pi / 180.0);
    
    if (ExitLength >= 0.0)
    {
        hopperExitLength_ = ExitLength;
    }
    else
    {
        logger(ERROR,"[ChuteWithHopper::setHopper()] Hopper exit length must be greater than or equal to zero");
        exit(-1);
    }
    
    // hopperExitHeight_
    if (ExitHeight < 0.0) 
    {
        logger(ERROR,"[ChuteWithHopper::setHopper()] Hopper exit height must be greater than or equal to zero");
        exit(-1);
    }
    else if(ExitHeight > HopperCornerHeight + std::tan(getChuteAngle())*ExitLength )
    {
        logger(ERROR,"[ChuteWithHopper::setHopper()] Hopper exit height (%) may not exceed height of hopper corner above chute bottom (%)", ExitHeight, HopperCornerHeight + std::tan(getChuteAngle())*ExitLength );
        exit(-1);
    }
    else //(ExitHeight >= 0.0) /// \todo write check: ExitHeight may NOT exceed vertical distance between chute base and hopper corner!
    {
        hopperExitHeight_ = ExitHeight;
    }
    
    setHopperLowestPoint(hopperExitHeight_ - hopperExitLength_ * tan(getChuteAngle()));

    if (Angle > 0.0 && Angle < 90.0)
    {
        hopperAngle_ = Angle * constants::pi / 180.0;
    }
    else
    {
        logger(ERROR,"[ChuteWithHopper::setHopper()] Hopper angle must in (0,90)");
        exit(-1);
    }

    if (Length > ExitLength)
    {
        hopperLength_ = Length;
    }
    else
    {
        logger(ERROR,"[ChuteWithHopper::setHopper()] Hopper length must be greater than exit length");
        exit(-1);
    }
    
    // check hopper 'corner height', i.e. the vertical position of point 'B' as compared to the start of the hopper
    // Mdouble HopperCornerHeight = Height - 0.5 * (Length - ExitLength) / std::tan(hopperAngle_ * constants::pi / 180.0);
    if (HopperCornerHeight <= 0.0) 
    {
        // hopperHeight_ += -HopperCornerHeight + problem.getMaxInflowParticleRadius();
        // HopperCornerHeight = problem.getMaxInflowParticleRadius();
        logger(ERROR,"[ChuteWithHopper::setHopper()] height of hopper corner (%) may not be below 0. Increase hopper height to fix.", HopperCornerHeight);
        exit(-1);
    }
    
    logger(VERBOSE, " ");
    logger(VERBOSE, "[ChuteWithHopper::setHopper()] Setting the following hopper geometrical properties:");
    logger(VERBOSE, " hopperLowestPoint_: %, ", getHopperLowestPoint());
    logger(VERBOSE, " hopperLength_: %, ", hopperLength_);
    logger(VERBOSE, " hopperExitLength_: %, ", hopperExitLength_);
    logger(VERBOSE, " hopperAngle_: %, ", hopperAngle_);
    logger(VERBOSE, " Height: %, ", Height);
    logger(VERBOSE, " comparing height: % ", getHopperLowestPoint() + 0.5*(hopperLength_+hopperExitLength_) / tan(hopperAngle_));
    logger(VERBOSE, " ");
    
    Mdouble HeightCompare = (getHopperLowestPoint() + 0.5 * (hopperLength_ + hopperExitLength_) / tan(hopperAngle_));
    //This a semi-ugly fix to check whether Height>=Heightcompare and does not take into account rounding errors
    if ((Height - HeightCompare) > -1e-6 * HeightCompare)
    {
        hopperHeight_ = Height;
    }
    else
    {
        logger(ERROR,"[ChuteWithHopper::setHopper()] For these settings, hopper height must be greater then or equal to %, see drawing", HeightCompare);
        exit(-1);
    ///\todo: check whether HopperCornerHeight >=0, if not change hopperangle, line 105, I do not yet understand what the criteria is...
    }
    
    logger(VERBOSE, " ");
    logger(VERBOSE, "[ChuteWithHopper::setHopper()] Hopper geometry: ");
    logger(VERBOSE, "hopperHeight_: \t %", hopperHeight_);
    logger(VERBOSE, "hopperExitLength_: \t %", hopperExitLength_);
    logger(VERBOSE, "hopperExitHeight_: \t %", hopperExitHeight_);
    logger(VERBOSE, "hopperAngle_: \t %", hopperAngle_);
    logger(VERBOSE, "hopperLength_: \t %", hopperLength_);
    logger(VERBOSE, " ");

}

/*!
 * \details Returns the maximum velocity a particle could theoretically reach by gravity,
 * would it fall from the top of the hopper straight to the bottom of the system.
 * \return      The maximum velocity possible due to gravity
 */
Mdouble ChuteWithHopper::getMaximumVelocityInducedByGravity()
{
    Mdouble height = hopperHeight_ + (getXMax() - hopperShift_) * sin(getChuteAngle());

    return sqrt(2.0 * getGravity().getLength() * height);
}

/*!
 * \details Returns the ratio of minimum particle radius over maximum distance 
 * travelled per time step due to gravitational acceleration, and returns a warning
 * when this is smaller then a certain threshold.
 * \todo Consider generalising this method by implementing it in the MercuryBase class. \author BvdH
 * NB: this method is used in ChuteWithHopper::setupInitialConditions().
 */
Mdouble ChuteWithHopper::getTimeStepRatio() 
{
    Mdouble dx = getTimeStep() * getMaximumVelocityInducedByGravity();
    Mdouble rmin = getMinInflowParticleRadius();
    
    if (rmin/dx < 10.)
        logger(WARN,"[ChuteWithHopper::getTimeStepRatio()] ratio of minimum particle radius over max distance travelled per time step due to gravity is only %; consider reducing the time step size!",rmin/dx);
    
    return rmin/dx;
}

/*!
 * \details Returns the length of the chute. The start of the view, at x = 0, is  
 * hopperShift_ to the left of the start of the chute. 
 * \return      the hopper length
 */
Mdouble ChuteWithHopper::getChuteLength()
{
    return getXMax() - hopperShift_;
}

/*!
 * \details Sets xMax_ such that the total chute length matches the argument.
 * \param[in] chuteLength       The chute length according to which xMax is to be 
 *                              adapted
 */
void ChuteWithHopper::setChuteLength(Mdouble chuteLength)
{
    if (chuteLength >= 0.0)
    {
        setXMax(chuteLength + hopperShift_);
        setXMin(0.0);
    }
    else
        logger(WARN,"[ChuteWithHopper::setChuteLength()] Chute length unchanged, value must be greater than or equal to zero");
}

/*!
 * \details Sets the isHopperCentered_ property, which determines whether the whole
 * setup is shifted another 40 units of length in the X-direction. See also ChuteWithHopper::addHopper().
 * \param[in] isHopperCentred       if TRUE, the whole setup is shifted 40 units of length 
 *                                  towards the positive X-direction.
 */
void ChuteWithHopper::setIsHopperCentred(bool isHopperCentred)
{
    isHopperCentred_ = isHopperCentred;
}

/*!
 * \details Sets the relative height (in [0,1)) above which the hopper is replenished with new particles
 */
void ChuteWithHopper::setHopperLowerFillingHeight(Mdouble hopperLowerFillingHeight)
{
    hopperLowerFillingHeight_ = hopperLowerFillingHeight;
}

/*!
 * \details Sets the distance the whole setup is shifted in the X-direction relative from
 * the position at which the start of the CHUTE is at X = 0.
 * \param[in] hopperShift       The hopper shift to be set
 */
void ChuteWithHopper::setHopperShift(Mdouble hopperShift)
{
    if (hopperShift >= 0.0)
    {
        //keeps the ChuteLength constant
        setXMax(getXMax() + hopperShift - hopperShift_);
        hopperShift_ = hopperShift;
    }
    else
        logger(WARN, "[ChuteWithHopper::setHopperShift()] Shift length unchanged, value must be greater than or equal to zero");
}

/*!
 * \details Reads the setup properties from an istream
 * \param[in,out] is    the istream
 */
void ChuteWithHopper::read(std::istream& is)
{
    Chute::read(is);
    is >> hopperExitLength_ >> hopperExitHeight_ >> hopperLength_
            >> hopperAngle_ >> hopperHeight_ >> hopperShift_;
}


/*!
 * \details Writes object's properties to an ostream
 * \param[in] os                    the ostream
 * \param[in] writeAllParticles     If TRUE, the properties of ALL particles in the 
 *                                  particleHandler are written to the ostream. If 
 *                                  FALSE, only the properties of the first two 
 *                                  particles in the handler are written to the 
 *                                  ostream (see DPMBase::write(std::ostream& os, bool writeAllParticles)).        
 */
void ChuteWithHopper::write(std::ostream& os, bool writeAllParticles) const
{
    Chute::write(os, writeAllParticles);
    os << hopperExitLength_ << " " << hopperExitHeight_ << " " << hopperLength_
            << " " << hopperAngle_ << " " << hopperHeight_ << " " << hopperShift_ << " " << std::endl;
}

/*!
 * \details Returns the hopper angle (relative to the vertical) in RADIANS
 */
Mdouble ChuteWithHopper::getHopperAngle()
{
    return hopperAngle_;
}

/*!
 * \details Returns the horizontal width of the hopper input (at the top of the hopper)
 */
Mdouble ChuteWithHopper::getHopperLength()
{
    return hopperLength_;
}

/*!
 * \details Returns the width of the (rectangular) hopper exit
 */
Mdouble ChuteWithHopper::getHopperExitLength()
{
    return hopperExitLength_;
}

/*!
 * \details Returns the (vertical) height of the hopper relative to the start of the chute
 */
Mdouble ChuteWithHopper::getHopperHeight()
{
    return hopperHeight_;
}

/*!
 * \details Returns the (vertical) height of the lowest point of the hopper (hopperLowestPoint_)
 * above the (inclined) chute bottom
 */
Mdouble ChuteWithHopper::getHopperExitHeight()
{
    return hopperExitHeight_;
}

/*!
 * \details Returns the isHopperCentered_ property, which determines whether the whole
 * setup is shifted another 40 units of length in the X-direction. See also ChuteWithHopper::addHopper().
 * \return      if TRUE, the whole setup is shifted 40 units of length towards the positive X-direction.
 */
bool ChuteWithHopper::getIsHopperCentred()
{
    return isHopperCentred_;
}

/*!
 * \details Returns the percentage of the height of the hopper insertion boundary 
 * up to which it should be filled. The part to be filled reaches from the top of 
 * the hopper down to {fillPercent * (top - 'position A')}. See also the documentation of 
 * the HopperInsertionBoundary class.
 */
Mdouble ChuteWithHopper::getHopperFillingPercentage()
{
    return hopperFillingPercentage_;
}

/*!
 * \details Returns the hopperDimension_ property, which determines whether the hopper 
 * entrance walls in the Y-direction are inclined (2) or vertical (1).
 */
unsigned int ChuteWithHopper::getHopperDimension()
{
    return hopperDimension_;
}

/*!
 * \details Sets the amount the hopper is lifted above the X-axis (in Z-direction, 
 * i.e. AFTER rotation of the system to have the chute parallel to the X-axis) 
 */
void ChuteWithHopper::setHopperLift(Mdouble hopperLift)
{
    hopperLift_ = hopperLift;
}

/*!
 * \details Returns the amount the hopper is lifted above the X-axis (in Z-direction, 
 * i.e. AFTER rotation of the system to have the chute parallel to the X-axis) 
 */
Mdouble ChuteWithHopper::getHopperLift()
{
    return hopperLift_;
}

/*!
 * \details Returns the distance the whole setup is shifted in the X-direction relative from
 * the position at which the start of the CHUTE is at X = 0.
 */
Mdouble ChuteWithHopper::getHopperShift()
{
    return hopperShift_;
}

/*!
 * \details Sets the hopperDimension_ property, which determines whether the hopper 
 * entrance walls in the Y-direction are inclined (2) or vertical (1).
 */
void ChuteWithHopper::setHopperDimension(Mdouble hopperDimension)
{
    hopperDimension_ = hopperDimension;
}

/*!
 * \details This sets the flag, which determines if the chute bottom is aligned with the hopper
 */
void ChuteWithHopper::setIsHopperAlignedWithBottom(bool isHopperAlignedWithBottom)
{
    isHopperAlignedWithBottom_ = isHopperAlignedWithBottom;
}

/*!
 * \details this reads parameters from a string.
 * \param[in] i         the index of the input parameter to be read
 * \param[in] argc      number of input parameters
 * \param[in] argv[]    pointer to the (first character of the) actual string
 */
bool ChuteWithHopper::readNextArgument(int& i, int argc, char *argv[])
{
    if (!strcmp(argv[i], "-hopperLength"))
    {
        hopperLength_ = (atof(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-hopperHeight"))
    {
        hopperHeight_ = (atof(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-hopperAngle"))
    {
        hopperAngle_ = (atof(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-hopperExitLength"))
    {
        hopperExitLength_ = (atof(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-hopperExitHeight"))
    {
        hopperExitHeight_ = (atof(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-hopperLowerFillingHeight_"))
    {
        hopperLowerFillingHeight_ = (atof(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-isHopperCentred"))
    {
        isHopperCentred_ = (atoi(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-alignBase"))
    {
        isHopperAlignedWithBottom_ = (atoi(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-shift"))
    {
        hopperShift_ = (atof(argv[i + 1]));
    }
    else if (!strcmp(argv[i], "-lift"))
    {
        hopperLift_ = (atof(argv[i + 1]));
    }
    else
        return Chute::readNextArgument(i, argc, argv); //if argv[i] is not found, check the commands in Chute
    return true; //returns true if argv[i] is found
}

