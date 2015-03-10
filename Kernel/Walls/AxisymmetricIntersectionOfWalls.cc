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
