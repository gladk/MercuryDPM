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

#include "CylindricalWall.h"
#include "InteractionHandler.h"
#include "Particles/BaseParticle.h"

CylindricalWall::CylindricalWall()
        : BaseWall()
{
    radius_ = std::numeric_limits<double>::quiet_NaN();
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"CylindricalWall () finished"<<std::endl;
#endif
}

CylindricalWall::CylindricalWall(const CylindricalWall &p)
        : BaseWall(p)
{
    radius_ = p.radius_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"CylindricalWall (const CylindricalWall &p) finished"<<std::endl;
#endif
}

CylindricalWall::CylindricalWall(double radius)
        : BaseWall()
{
    this->radius_ = radius;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"CylindricalWall (double radius) finished"<<std::endl;
#endif
}

///Wall copy method. It calls the copy contrustor of this Wall, usefull for polymorfism
CylindricalWall* CylindricalWall::copy() const
{
    return new CylindricalWall(*this);
}

void CylindricalWall::clear()
{
}

///Defines a standard wall, given an outward normal vector s. t. normal*x=position
void CylindricalWall::set(Mdouble radius)
{
    this->radius_ = radius;
}

///Since this function should be called before calculating any Particle-Wall interactions, it can also be used to set the normal vector in case of curved walls.
bool CylindricalWall::getDistanceAndNormal(const BaseParticle &P, Mdouble &distance, Vec3D &normal_return) const
        {
    double R = sqrt(pow(P.getPosition().X, 2) + pow(P.getPosition().Y, 2));
    distance = radius_ - R;
    if (distance >= P.getRadius())
        return false;
    
    normal_return.X = P.getPosition().X;
    normal_return.Y = P.getPosition().Y;
    normal_return.Z = 0.0;
    normal_return /= normal_return.getLength();
    
    return true;
}

///reads wall
void CylindricalWall::read(std::istream& is)
{
    BaseWall::read(is);
    std::string dummy;
    is >> dummy >> radius_;
}

void CylindricalWall::oldRead(std::istream& is)
{
    std::string dummy;
    is >> dummy >> radius_;
}

///outputs wall
void CylindricalWall::write(std::ostream& os) const
        {
    BaseWall::write(os);
    os << "  radius " << radius_;
}

std::string CylindricalWall::getName() const
{
    return "CylindricalWall";
}

///access function for radius
double CylindricalWall::getRadius() const
{
    return radius_;
}

BaseInteraction* CylindricalWall::getInteractionWith(BaseParticle *P, Mdouble timeStamp, InteractionHandler* interactionHandler)
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
        C->setContactPoint(P->getPosition() - (P->getRadius() - 0.5 * C->getOverlap()) * C->getNormal());
        return C;
    }
    else
    {
        return 0;
    }
}
