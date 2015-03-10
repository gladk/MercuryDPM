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

#include <limits>

#include "InfiniteWall.h"
#include "Particles/BaseParticle.h"
#include "InteractionHandler.h"

InfiniteWall::InfiniteWall()
        : BaseWall()
{
    factor_ = std::numeric_limits<double>::quiet_NaN();
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"InfiniteWall::InfiniteWall ) finished"<<std::endl;
#endif
}

InfiniteWall::InfiniteWall(const InfiniteWall &p)
        : BaseWall(p)
{
    normal_ = p.normal_;
    factor_ = p.factor_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"InfiniteWall::InfiniteWall(const InfiniteWall &p) finished"<<std::endl;
#endif
}

InfiniteWall::~InfiniteWall()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout<<"InfiniteWall::~InfiniteWall finished"<<std::endl;
#endif	    
}

///Wall copy method. It calls the copy contrustor of this Wall, usefull for polymorfism
InfiniteWall* InfiniteWall::copy() const
{
    return new InfiniteWall(*this);
}

void InfiniteWall::clear()
{
}

///Adds a wall to the set of finite walls, given an outward normal vector s.t. normal*x=normal*point
void InfiniteWall::set(Vec3D normal, Vec3D point)
{
    setNormal(normal);
    setPosition(point);
}

void InfiniteWall::setNormal(const Vec3D normal)
{
    factor_ = 1. / sqrt(Vec3D::dot(normal, normal));
    normal_ = normal * factor_;
}

///Defines a standard wall, given an outward normal vector s. t. normal*x=position
void InfiniteWall::set(Vec3D normal, Mdouble positionInNormalDirection)
{
    set(normal,positionInNormalDirection*normal);
}

///Allows the wall to be moved to a new position
void InfiniteWall::move(Mdouble positionInNormalDirection)
{
    setPosition(positionInNormalDirection * normal_ / factor_);
}

///Returns the distance of the wall to the particle. 
Mdouble InfiniteWall::getDistance(const Vec3D &otherPosition) const
{
    return Vec3D::dot(getPosition()-otherPosition, normal_);
}

///Since this function should be called before calculating any Particle-Wall interactions, it can also be used to set the normal vector in case of curved walls.
bool InfiniteWall::getDistanceAndNormal(const BaseParticle &P, Mdouble &distance, Vec3D &normal_return) const
{
    distance = getDistance(P.getPosition());
    if (distance >= P.getWallInteractionRadius())
        return false;
    normal_return = normal_;
    return true;
}

///reads wall
void InfiniteWall::read(std::istream& is)
{
    BaseWall::read(is);
    std::string dummy;
    is >> dummy >> normal_;
    is >> dummy >>factor_;
}

void InfiniteWall::oldRead(std::istream& is)
{
    std::string dummy;
    Vec3D velocity;
    Vec3D position;
    is >> dummy >> normal_ >> dummy >> position >> dummy >> velocity;
    setPosition(position);
    setVelocity(velocity);
}

///outputs wall
void InfiniteWall::write(std::ostream& os) const
{
    BaseWall::write(os);
    os << " normal " << normal_
            << " factor "<<factor_;
}

std::string InfiniteWall::getName() const
{
    return "InfiniteWall";
}

///access function for normal
Vec3D InfiniteWall::getNormal() const
{
    return normal_;
}

BaseInteraction* InfiniteWall::getInteractionWith(BaseParticle *P, Mdouble timeStamp, InteractionHandler* interactionHandler)
{
    Mdouble distance = getDistance(P->getPosition());
    if (distance < P->getWallInteractionRadius())
    {
        BaseInteraction* C = interactionHandler->getInteraction(P, this, timeStamp);
        C->setNormal(-normal_);
        C->setDistance(distance);
        C->setOverlap(P->getRadius() - distance);
        ///todo{DK: What is the contact point for interactions with walls}
//        C->setContactPoint(P->getPosition()-(P->getRadius()- 1.0 * C->getOverlap())*C->getNormal());
        C->setContactPoint(P->getPosition()-(P->getRadius()- 0.5 * C->getOverlap())*C->getNormal());
        return C;
    }
    else
    {
        return 0;
    }
}

