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

#include <limits>

#include "InfiniteWall.h"
#include "Particles/BaseParticle.h"
#include "InteractionHandler.h"

InfiniteWall::InfiniteWall()
        : BaseWall()
{
    position_ = std::numeric_limits<double>::quiet_NaN();
    factor_ = std::numeric_limits<double>::quiet_NaN();
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"InfiniteWall::InfiniteWall ) finished"<<std::endl;
#endif
}

InfiniteWall::InfiniteWall(const InfiniteWall &p)
        : BaseWall(p)
{
    normal_ = p.normal_;
    position_ = p.position_;
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
    set(normal, Vec3D::dot(normal, point));
}

///Defines a standard wall, given an outward normal vector s. t. normal*x=position
void InfiniteWall::set(Vec3D normal, Mdouble position)
{
    //factor is used to set n to unit length 
    factor_ = 1. / sqrt(Vec3D::dot(normal, normal));
    normal_ = normal * factor_;
    position_ = position * factor_;
}

///Allows the wall to be moved to a new position
void InfiniteWall::move(Mdouble position)
{
    position_ = position * factor_;
}

///Allows the wall to be moved to a new position (also orthogonal to the normal), and setting the velocity
void InfiniteWall::move(Vec3D velocity, Mdouble dt)
{
    setVelocity(velocity);
    position_ += Vec3D::dot(getVelocity(), normal_) * dt;
}

///Allows the wall to be moved with time
void InfiniteWall::move_time(Mdouble dt)
{
    position_ += Vec3D::dot(getVelocity(), normal_) * dt;
}

///Returns the distance of the wall to the particle. 
Mdouble InfiniteWall::get_distance(const Vec3D &position) const
{
    return position_ - Vec3D::dot(position, normal_);
}

///Since this function should be called before calculating any Particle-Wall interactions, it can also be used to set the normal vector in case of curved walls.
bool InfiniteWall::getDistanceAndNormal(const BaseParticle &P, Mdouble &distance, Vec3D &normal_return) const
{
    distance = get_distance(P.getPosition());
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
    is >> dummy >> position_;
    is >> dummy >>factor_;
}

void InfiniteWall::oldRead(std::istream& is)
{
    std::string dummy;
    Vec3D velocity;
    is >> dummy >> normal_ >> dummy >> position_ >> dummy >> velocity;
    setVelocity(velocity);
}

///outputs wall
void InfiniteWall::write(std::ostream& os) const
{
    BaseWall::write(os);
    os << " normal " << normal_
            << " position " << position_
            << " factor "<<factor_;
}

std::string InfiniteWall::getName() const
{
    return "InfiniteWall";
}

///access function for normal
Vec3D InfiniteWall::get_Normal() const
{
    return normal_;
}
///access function for position
Mdouble InfiniteWall::getPosition() const
{
    return position_;
}

BaseInteraction* InfiniteWall::getInteractionWith(BaseParticle *P, Mdouble timeStamp, InteractionHandler* interactionHandler)
{
    Mdouble distance = get_distance(P->getPosition());
    if (distance < P->getWallInteractionRadius())
    {
        BaseInteraction* C = interactionHandler->getInteraction(P, this, timeStamp);
        C->setNormal(-normal_);
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

