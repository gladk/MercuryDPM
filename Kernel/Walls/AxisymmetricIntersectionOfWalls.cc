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

#include "AxisymmetricIntersectionOfWalls.h"
#include "Particles/BaseParticle.h"

AxisymmetricIntersectionOfWalls::AxisymmetricIntersectionOfWalls()
        : IntersectionOfWalls()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"AxisymmetricIntersectionOfWalls() finished"<<std::endl;
#endif
}

AxisymmetricIntersectionOfWalls::AxisymmetricIntersectionOfWalls(const AxisymmetricIntersectionOfWalls &p)
        : IntersectionOfWalls(p)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"AxisymmetricIntersectionOfWalls(const AxisymmetricIntersectionOfWalls &p) finished"<<std::endl;
#endif
}

///Wall copy method. It calls the copy contrustor of this Wall, usefull for polymorfism
AxisymmetricIntersectionOfWalls* AxisymmetricIntersectionOfWalls::copy() const
{
    return new AxisymmetricIntersectionOfWalls(*this);
}

bool AxisymmetricIntersectionOfWalls::getDistanceAndNormal(const BaseParticle &P, Mdouble &distance, Vec3D &normal_return) const
{
    ///\todo{Kay found a bug here}
    //transform to axisymmetric coordinates
    Vec3D PO = P.getPosition() -getPosition(); //move the coordinate system to the axis origin, so P0=(xhat,yhat,zhat)
    Vec3D tangential = PO - P.getPosition().Z * getOrientation(); //tangential is the projection into the (xhat,yhat) plane
    Vec3D transformedPosition=Vec3D(tangential.getLength(), 0.0, Vec3D::dot(getOrientation(), PO)); //now P=(r,phi,zhat) is cylindrical
    tangential /= P.getPosition().X;
    Vec3D normal_axisymmetric_coordinates;
    //determine wall distance, normal and contact in axissymmetric coordinates
    //and transform from axisymmetric coordinates
    if (!IntersectionOfWalls::getDistanceAndNormal(transformedPosition,P.getWallInteractionRadius(), distance, normal_axisymmetric_coordinates))
    {
        //if not in contact
        return false;
    }
    else
    {
        //if in contact
        normal_return = normal_axisymmetric_coordinates.Z * getOrientation() + tangential * normal_axisymmetric_coordinates.X;
        ///\todo {Radius is based on Particle, not wall, DK i commented this, Thomas will look at it, since now it looks strange (i.e. getVelocity already returns a cartesian velocity}
        /*
        if (!getVelocity().isZero())
        { //assuming most walls have zero velocity, this if statement saves time
            Vec3D angular = Vec3D::cross(axisOrientation, tangential);
            CartesianVelocity = Vec3D(getVelocity().Z * axisOrientation + tangential * getVelocity().X + angular * P.getPosition().X * getVelocity().Y);
        }
        */
        //std::cout << P.getPosition() << " v " << getVelocity() << " cv " << velocity << std::endl;
        return true;
    }
}

///reads wall
void AxisymmetricIntersectionOfWalls::read(std::istream& is)
{
    IntersectionOfWalls::read(is);
}

///reads wall
void AxisymmetricIntersectionOfWalls::oldRead(std::istream& is)
{
    IntersectionOfWalls::read(is);
}

///outputs wall
void AxisymmetricIntersectionOfWalls::write(std::ostream& os) const
{
    IntersectionOfWalls::write(os);
}

std::string AxisymmetricIntersectionOfWalls::getName() const
{
    return "AxisymmetricIntersectionOfWalls";
}

BaseInteraction* AxisymmetricIntersectionOfWalls::getInteractionWith(BaseParticle *P, Mdouble timeStamp, InteractionHandler* interactionHandler)
{
    Mdouble distance;
    Vec3D normal;

    if (getDistanceAndNormal(*P,distance,normal))
    {
        BaseInteraction* C = interactionHandler->getInteraction(P, this, timeStamp);
        C->setNormal(-normal);
        C->setDistance(distance);
        C->setOverlap(P->getRadius() - distance);
        ///todo{DK: What is the contact point for interactions with walls}
        C->setContactPoint(P->getPosition()-(P->getRadius()- 0.5 * C->getOverlap())*C->getNormal());
        return C;
    }
    else
    {
        return 0;
    }
}
