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

bool AxisymmetricIntersectionOfWalls::getDistanceAndNormal(const BaseParticle &P, Mdouble &distance, Vec3D &normalReturn) const
{
    ///\todo test
    //transform to axisymmetric coordinates
    Vec3D PO = P.getPosition() -getPosition(); //move the coordinate system to the axis origin, so P0=(xhat,yhat,zhat)
    Mdouble normal = Vec3D::dot(PO, getOrientation());
    Vec3D tangentialUnitVector = PO - normal * getOrientation(); //tangential is the projection into the (xhat,yhat) plane
    Mdouble tangential = tangentialUnitVector.getLength();
    if (tangential!=0.0)
        tangentialUnitVector /= tangential;
    else //in this case the tangential vector is irrelevant
        std::cout << "Warning: Particle " << P.getIndex() << " is exactly on the symmetry axis of wall " << this->getIndex() << std::endl;
    Vec3D transformedPosition = Vec3D(tangential, 0.0, normal); //now P=(r,phi,zhat) is cylindrical
    Vec3D transformedNormal;
    //determine wall distance, normal and contact in axissymmetric coordinates
    //and transform from axisymmetric coordinates
    if (!IntersectionOfWalls::getDistanceAndNormal(transformedPosition, P.getWallInteractionRadius(), distance, transformedNormal))
    {
        //if not in contact
        return false;
    }
    else
    {
        //if in contact
        normalReturn = transformedNormal.Z * getOrientation() + transformedNormal.X * tangentialUnitVector;
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
