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


#include <string.h>
#include "ChuteWithHopper.h"
#include "Boundaries/HopperInsertionBoundary.h"
#include "Walls/IntersectionOfWalls.h"

///This is a copy constructor for Chute problems \bug This copy construct is untested
ChuteWithHopper::ChuteWithHopper(const Chute& other)
: DPMBase(other), Chute(other)
{
    constructor();
}

ChuteWithHopper::ChuteWithHopper(const Mercury3D& other)
: DPMBase(other), Chute(other)
{
    constructor();
}

ChuteWithHopper::ChuteWithHopper(const MercuryBase& other)
: DPMBase(other), Chute(other)
{
    constructor();
}

ChuteWithHopper::ChuteWithHopper(const DPMBase& other)
: DPMBase(other), Chute(other)
{
    constructor();
}

ChuteWithHopper::ChuteWithHopper()
{
    constructor();
}

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

void ChuteWithHopper::setHopperFillingPercentage(Mdouble hopperFillingPercentage)
{
    hopperFillingPercentage_ = hopperFillingPercentage;
}

void ChuteWithHopper::setupInitialConditions()
{
    setupSideWalls();

    HopperInsertionBoundary b1;
    b1.set(new BaseParticle, getMaxFailed(), getYMin(), getYMax(), getMinInflowParticleRadius(), getMaxInflowParticleRadius(),
            getChuteAngle(), getFixedParticleRadius(), isHopperCentred_, hopperDimension_, hopperAngle_, hopperLength_,
            hopperExitLength_,hopperHeight_, hopperLift_, hopperFillingPercentage_);
    boundaryHandler.copyAndAddObject(b1);
    setInsertionBoundary(dynamic_cast<InsertionBoundary*>(boundaryHandler.getLastObject()));

    createBottom();

    addHopper();
}

void ChuteWithHopper::addHopper()
{
    //hopper walls
    //to create the finite hopper walls, we take vector between two wall points in xz-plane, then rotate clockwise and make unit length
    // A\       /A
    //   \     /       A,B,C denote three points on the left and right hopper walls which are used to construct the hopper
    //    \   /        shift denotes the space by which the chute has to be shifted to the right such that the hopper is in the domain
    //   B|   |B
    //    |   |
    //    |   |C
    //   C|

    Vec3D A, B, C, temp, normal;

    Mdouble s = sin(getChuteAngle());
    Mdouble c = cos(getChuteAngle());
    hopperLowestPoint_ = hopperExitHeight_ - hopperExitLength_ * tan(getChuteAngle());

    ///"0.5*(hopperLength_+hopperExitLength_) / tan(hopperAngle_)" is the minimum heigth of the hopper, to make sure things should flow down and not to the sides.
    //hopperHeight_ is now an input variable
    //hopperHeight_ = hopperLowestPoint_ + 1.1 * 0.5*(hopperLength_+hopperExitLength_) / tan(hopperAngle_);

    Mdouble HopperCornerHeight = hopperHeight_ - 0.5 * (hopperLength_ - hopperExitLength_) / tan(hopperAngle_);
    // Waarom had ik deze ook al weer gecomment?
    //if (HopperCornerHeight<=0.0) { hopperHeight_ += -HopperCornerHeight + P0.getRadius(); HopperCornerHeight = P0.getRadius(); }

    //first we create the left hopper wall

    //coordinates of A,B,C in (vertical parallel to flow,vertical normal to flow, horizontal) direction
    A = Vec3D(-0.5 * (hopperLength_ - hopperExitLength_), 0.0, hopperHeight_);
    B = Vec3D(0.0, 0.0, HopperCornerHeight);
    C = Vec3D(0.0, 0.0, 0.0);

    //now rotate the coordinates of A,B,C to be in (x,y,z) direction
    A = Vec3D(c * A.X - s * A.Z, 0.0, s * A.X + c * A.Z);
    B = Vec3D(c * B.X - s * B.Z, 0.0, s * B.X + c * B.Z);
    C = Vec3D(c * C.X - s * C.Z, 0.0, s * C.X + c * C.Z);
    // the position of A determines shift and zmax
    ///todo{Why shift by arbitrary number of 40, when isHopperCentred_=True}
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
    w_Left.addObject(normal, Vec3D::dot(normal, A));
    temp = C - B;
    normal = Vec3D(temp.Z, 0.0, -temp.X) / sqrt(temp.getLengthSquared());
    w_Left.addObject(normal, Vec3D::dot(normal, B));
    temp = A - C;
    normal = Vec3D(temp.Z, 0.0, -temp.X) / sqrt(temp.getLengthSquared());
    w_Left.addObject(normal, Vec3D::dot(normal, C));
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
    w_Right.addObject(normal, Vec3D::dot(normal, A));
    temp = B - C;
    normal = Vec3D(temp.Z, 0.0, -temp.X) / sqrt(temp.getLengthSquared());
    w_Right.addObject(normal, Vec3D::dot(normal, B));
    temp = C - A;
    normal = Vec3D(temp.Z, 0.0, -temp.X) / sqrt(temp.getLengthSquared());
    w_Right.addObject(normal, Vec3D::dot(normal, C));
    wallHandler.copyAndAddObject(w_Right);

    setZMax(A.Z);

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
        w_Back.addObject(normal, Vec3D::dot(normal, A));
        temp = C - B;
        //normal  = Vec3D(0.0,temp.Z,-temp.Y) / sqrt(temp.GetLength2());
        normal = Vec3D::cross(Vec3D(-c, 0, -s), temp) / sqrt(temp.getLengthSquared());
        w_Back.addObject(normal, Vec3D::dot(normal, B));
        temp = A - C;
        //normal = Vec3D(0.0,temp.Z,-temp.Y)/sqrt(temp.GetLength2());
        normal = Vec3D::cross(Vec3D(-c, 0, -s), temp) / sqrt(temp.getLengthSquared());
        w_Back.addObject(normal, Vec3D::dot(normal, C));
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
        w_Front.addObject(normal, Vec3D::dot(normal, A));
        temp = B - C;
        //normal  = Vec3D(0.0,-temp.Z,temp.Y) / sqrt(temp.GetLength2());
        normal = Vec3D::cross(Vec3D(-c, 0, -s), temp) / sqrt(temp.getLengthSquared());
        w_Front.addObject(normal, Vec3D::dot(normal, B));
        temp = C - A;
        //normal = Vec3D(0.0,-temp.Z,temp.Y)/sqrt(temp.GetLength2());
        normal = Vec3D::cross(Vec3D(-c, 0, -s), temp) / sqrt(temp.getLengthSquared());
        w_Front.addObject(normal, Vec3D::dot(normal, C));
        wallHandler.copyAndAddObject(w_Front);
    }

    //now shift the fixed particles at the bottom so that they begin where the chute begins
    for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it != particleHandler.end(); ++it)
    {
        (*it)->move(Vec3D(hopperShift_, 0.0, 0.0));
    }
}

void ChuteWithHopper::setHopperLowestPoint(Mdouble hopperLowestPoint)
{
    hopperLowestPoint_ = hopperLowestPoint;
}
Mdouble ChuteWithHopper::getHopperLowestPoint()
{
    return hopperLowestPoint_;
}

void ChuteWithHopper::setHopper(Mdouble ExitLength, Mdouble ExitHeight, Mdouble Angle, Mdouble Length, Mdouble Height)
{

    if (ExitLength >= 0.0)
    {
        hopperExitLength_ = ExitLength;
    }
    else
        std::cerr << "WARNING : Hopper exit length must be greater than or equal to zero" << std::endl;
    if (ExitHeight >= 0.0)
    {
        hopperExitHeight_ = ExitHeight;
    }
    else
        std::cerr << "WARNING : Hopper exit height must be greater than or equal to zero" << std::endl;

    setHopperLowestPoint(hopperExitHeight_ - hopperExitLength_ * tan(getChuteAngle()));

    if (Angle > 0.0 && Angle < 90.0)
    {
        hopperAngle_ = Angle * constants::pi / 180.0;
    }
    else
        std::cerr << "WARNING : Hopper angle must in (0,90)" << std::endl;

    if (Length > ExitLength)
    {
        hopperLength_ = Length;
    }
    else
        std::cerr << "WARNING : Hopper length must be greater than exit length" << std::endl;

    //std::cout<<"hopperLowestPoint_: "<<getHopperLowestPoint()<<", hopperLength_: "<<hopperLength_<<", hopperExitLength_: "<<hopperExitLength_<<", hopperAngle_: "<<hopperAngle_<<", Height: "<<fixed<<setprecision(15)<<setw(20)<<Height<<"comparing height: "<<setprecision(15)<<setw(20)<<(getHopperLowestPoint() + 0.5*(hopperLength_+hopperExitLength_) / tan(hopperAngle_))<<std::endl;

    Mdouble HeightCompare = (getHopperLowestPoint() + 0.5 * (hopperLength_ + hopperExitLength_) / tan(hopperAngle_));
    //This a semi-ugly fix to check wether Height>=Heightcompare and not take into account rounding errors
    if ((Height - HeightCompare) > -1e-6 * HeightCompare)
    {
        hopperHeight_ = Height;
    }
    else
        std::cerr << "WARNING : For these settings, hopper height must be greater then or equal to " << HeightCompare << ", see drawing" << std::endl;
    ///todo: check wether HopperCornerHeight >=0, if not change hopperangle, line 105, I do not yet understand what the criteria is...

    //std::cout<<" "<<std::endl;
    //std::cout<<"ChuteWithHopper::set_Hopper:"<<std::endl;
    //std::cout<<"hopperHeight_: 	  "<<hopperHeight_<<std::endl;
    //std::cout<<"hopperExitLength_: "<<hopperExitLength_<<std::endl;
    //std::cout<<"hopperExitHeight_: "<<hopperExitHeight_<<std::endl;
    //std::cout<<"hopperAngle_: 	  "<<hopperAngle_<<std::endl;
    //std::cout<<"hopperLength_: 	  "<<hopperLength_<<std::endl;
    //std::cout<<" "<<std::endl;

}

Mdouble ChuteWithHopper::getMaximumVelocityInducedByGravity()
{
    Mdouble height = hopperHeight_ + (getXMax() - hopperShift_) * sin(getChuteAngle());

    return sqrt(2.0 * 9.8 * height);
}

Mdouble ChuteWithHopper::getChuteLength()
{
    return getXMax() - hopperShift_;
}

void ChuteWithHopper::setChuteLength(Mdouble chuteLength)
{
    if (chuteLength >= 0.0)
    {
        setXMax(chuteLength + hopperShift_);
        setXMin(0.0);
    }
    else
        std::cerr << "WARNING : Chute length unchanged, value must be greater than or equal to zero" << std::endl;
}

void ChuteWithHopper::setIsHopperCentred(bool isHopperCentred)
{
    isHopperCentred_ = isHopperCentred;
}

void ChuteWithHopper::setHopperLowerFillingHeight(Mdouble hopperLowerFillingHeight)
{
    hopperLowerFillingHeight_ = hopperLowerFillingHeight;
}

void ChuteWithHopper::setHopperShift(Mdouble hopperShift)
{
    if (hopperShift >= 0.0)
    {
        //keeps the ChuteLength constant
        setXMax(getXMax() + hopperShift - hopperShift_);
        hopperShift_ = hopperShift;
    }
    else
        std::cerr << "WARNING : Shift length unchanged, value must be greater than or equal to zero" << std::endl;
}

void ChuteWithHopper::read(std::istream& is)
{
    Chute::read(is);
    is >> hopperExitLength_ >> hopperExitHeight_ >> hopperLength_
            >> hopperAngle_ >> hopperHeight_ >> hopperShift_;
}

void ChuteWithHopper::write(std::ostream& os, bool writeAllParticles) const
{
    Chute::write(os, writeAllParticles);
    os << hopperExitLength_ << " " << hopperExitHeight_ << " " << hopperLength_
            << " " << hopperAngle_ << " " << hopperHeight_ << " " << hopperShift_ << " " << std::endl;
}

Mdouble ChuteWithHopper::getHopperAngle()
{
    return hopperAngle_;
}

Mdouble ChuteWithHopper::getHopperLength()
{
    return hopperLength_;
}

Mdouble ChuteWithHopper::getHopperExitLength()
{
    return hopperExitLength_;
}

Mdouble ChuteWithHopper::getHopperHeight()
{
    return hopperHeight_;
}

Mdouble ChuteWithHopper::getHopperExitHeight()
{
    return hopperExitHeight_;
}

bool ChuteWithHopper::getIsHopperCentred()
{
    return isHopperCentred_;
}

Mdouble ChuteWithHopper::getHopperFillingPercentage()
{
    return hopperFillingPercentage_;
}

unsigned int ChuteWithHopper::getHopperDimension()
{
    return hopperDimension_;
}

///This lifts the hopper above the plane of the chute
void ChuteWithHopper::setHopperLift(Mdouble hopperLift)
{
    hopperLift_ = hopperLift;
}

Mdouble ChuteWithHopper::getHopperLift()
{
    return hopperLift_;
}

Mdouble ChuteWithHopper::getHopperShift()
{
    return hopperShift_;
}

void ChuteWithHopper::setHopperDimension(Mdouble hopperDimension)
{
    hopperDimension_ = hopperDimension;
}

void ChuteWithHopper::setIsHopperAlignedWithBottom(bool isHopperAlignedWithBottom)
{
    isHopperAlignedWithBottom_ = isHopperAlignedWithBottom;
}

int ChuteWithHopper::readNextArgument(int& i, int argc, char *argv[])
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

