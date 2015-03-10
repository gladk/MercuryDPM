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
