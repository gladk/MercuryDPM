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

#include "InfiniteWallWithHole.h"
#include "InteractionHandler.h"
#include "Particles/BaseParticle.h"

InfiniteWallWithHole::InfiniteWallWithHole()
        : BaseWall()
{
    factor_=std::numeric_limits<double>::quiet_NaN();
    position_=std::numeric_limits<double>::quiet_NaN();
    holeRadius_=std::numeric_limits<double>::quiet_NaN();
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"InfiniteWall () finished"<<std::endl;
#endif
}

InfiniteWallWithHole::InfiniteWallWithHole(Vec3D normal, Mdouble position, Mdouble holeRadius)
        : BaseWall()
{
    set(normal, position, holeRadius);
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"InfiniteWallWithHole(Vec3D normal, Mdouble position, Mdouble holeRadius) finished"<<std::endl;
#endif
}

InfiniteWallWithHole::InfiniteWallWithHole(const InfiniteWallWithHole &p)
        : BaseWall(p)
{
    normal_ = p.normal_;
    position_ = p.position_;
    factor_ = p.factor_;
    holeRadius_ = p.holeRadius_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"InfiniteWall (const InfiniteWall &p) finished"<<std::endl;
#endif
}

///Wall copy method. It calls the copy contrustor of this Wall, usefull for polymorfism
InfiniteWallWithHole* InfiniteWallWithHole::copy() const
{
    return new InfiniteWallWithHole(*this);
}

void InfiniteWallWithHole::clear()
{
}

///Defines a standard wall, given an outward normal vector s. t. normal*x=position
void InfiniteWallWithHole::set(Vec3D normal, Mdouble position, Mdouble holeRadius)
{
    //factor is used to set n to unit length 
    factor_ = 1. / sqrt(Vec3D::dot(normal, normal));
    normal_ = normal * factor_;
    position_ = position * factor_;
    holeRadius_ = holeRadius;
}

///Allows the wall to be moved to a new position
void InfiniteWallWithHole::move(Mdouble position)
{
    position_ = position * factor_;
}

///Allows the wall to be moved to a new position (also orthogonal to the normal), and setting the velocity
///\todo InfiniteWallWithHole::move and InfiniteWall::move can be removed once the position is set by the general position vector
void InfiniteWallWithHole::move(Vec3D velocity, Mdouble dt)
{
    setVelocity(velocity);
    position_ += Vec3D::dot(getVelocity(), normal_) * dt;
}

///\todo TW: generalize the move function for all walls
///Allows the wall to be moved with time
void InfiniteWallWithHole::move_time(Mdouble dt)
{
    position_ += Vec3D::dot(getVelocity(), normal_) * dt;
}

///Returns the distance of the wall to the particle. 
Mdouble InfiniteWallWithHole::getWallDistance(const Vec3D &position) const
{
    return position_ - Vec3D::dot(position, normal_);
}

Mdouble InfiniteWallWithHole::getHoleDistance(const Vec3D &position) const
{
    return holeRadius_ - sqrt(pow(position.X, 2) + pow(position.Y, 2));
}

///Since this function should be called before calculating any Particle-Wall interactions, it can also be used to set the normal vector in case of curved walls.
bool InfiniteWallWithHole::getDistanceAndNormal(const BaseParticle &P, Mdouble &distance, Vec3D &normal_return) const
{
    double wallDistance = getWallDistance(P.getPosition());
    if (wallDistance >= P.getRadius())
        return false;
    
    double holeDistance = getHoleDistance(P.getPosition());
    if (holeDistance >= P.getRadius())
        return false;
    
    if (wallDistance > 0 && holeDistance > 0)
    {
        distance = sqrt(pow(wallDistance, 2) + pow(holeDistance, 2));
        Vec3D ContactPoint;
        double alpha = atan2(P.getPosition().Y, P.getPosition().X);
        ContactPoint.X = holeRadius_ * cos(alpha);
        ContactPoint.Y = holeRadius_ * sin(alpha);
        ContactPoint.Z = position_;
        //std::cout<<"ContactPoint="<<ContactPoint<<" Particle position="<<P.getPosition()<<std::endl;
        normal_return = ContactPoint - P.getPosition();
        normal_return /= normal_return.getLength();
        //std::cout<<"Corner collision normal="<<normal_return<<std::endl;
        return true;
    }
    else if (wallDistance > holeDistance)
    {
        distance = wallDistance;
        normal_return = normal_;
        //std::cout<<"Wall collision normal="<<normal_return<<std::endl;
        return true;
    }
    else
    {
        distance = holeDistance;
        normal_return.X = P.getPosition().X / (holeRadius_ - holeDistance);
        normal_return.Y = P.getPosition().Y / (holeRadius_ - holeDistance);
        normal_return.Z = 0;
        //std::cout<<"Hole collision normal="<<normal_return<<std::endl;
        return true;
    }
}

BaseInteraction* InfiniteWallWithHole::getInteractionWith(BaseParticle *P, Mdouble timeStamp, InteractionHandler* interactionHandler)
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

void InfiniteWallWithHole::read(std::istream& is)
{
    BaseWall::read(is);
    std::string dummy;
    is >> dummy >> normal_
            >> dummy >> factor_
            >> dummy >> position_
            >> dummy >> holeRadius_;
}

///reads wall
void InfiniteWallWithHole::oldRead(std::istream& is)
{
    std::string dummy;
    Vec3D velocity;
    is >> dummy >> normal_ >> dummy >> position_ >> dummy >> velocity;
    setVelocity(velocity);
}

///outputs wall
void InfiniteWallWithHole::write(std::ostream& os) const
        {
    BaseWall::write(os);
    os << " normal " << normal_
            << " factor " << factor_
            << " position " << position_
            << " holeRadius " << holeRadius_;
}

std::string InfiniteWallWithHole::getName() const
{
    return "InfiniteWallWithHole";
}

///access function for normal
Vec3D InfiniteWallWithHole::getNormal()
{
    return normal_;
}

///access function for position
Mdouble InfiniteWallWithHole::getPosition()
{
    return position_;
}
