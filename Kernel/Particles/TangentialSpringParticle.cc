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

#include "TangentialSpringParticle.h"
#include "../ParticleHandler.h"

TangentialSpringParticle::TangentialSpringParticle()
        : BaseParticle()
{
    tangentialSprings.reset();
}

TangentialSpringParticle::TangentialSpringParticle(const TangentialSpringParticle &p)
        : BaseParticle(p)
{
    //std::cout<<"TSP copy constructor"<<std::endl;
    tangentialSprings = p.tangentialSprings;
}

TangentialSpringParticle::~TangentialSpringParticle()
{
}

///Particle copy method. It calls to copy contrustor of this Particle, usefull for polymorfism
TangentialSpringParticle* TangentialSpringParticle::copy() const
{
    //std::cout<<"TangentialSpringParticle copy"<<std::endl;
    return new TangentialSpringParticle(*this);
}

void TangentialSpringParticle::reverseTangentialSprings()
{
    for (std::vector<CTangentialSpring>::iterator it = tangentialSprings.begin(); it != tangentialSprings.end(); it++)
    {
        it->reverse();
    }
}

CTangentialSprings& TangentialSpringParticle::get_TangentialSprings()
{
    return tangentialSprings;
}

void TangentialSpringParticle::write(std::ostream& os) const
{
    BaseParticle::write(os);
    os << " tangentialSprings " << tangentialSprings;
}

void TangentialSpringParticle::read(std::istream& is)
{
    BaseParticle::read(is);
    std::string dummy;
    is >> dummy >> tangentialSprings;
}

void TangentialSpringParticle::oldRead(std::istream& is)
{
    std::string dummy;
    is >> dummy;
    BaseParticle::read(is);
    is >> tangentialSprings;
}

std::string TangentialSpringParticle::getName() const
{
    return "TangentialSpringParticle";
}

void TangentialSpringParticle::moveInHandler(const unsigned int newPos)
{
    BaseParticle::moveInHandler(newPos);
    //Check if some TangentialSpring information has to be moved
    for (unsigned int i = 0; i < get_TangentialSprings().size();)
    {
        /// \todo Is a time restriction neccesary here?
        if (get_TangentialSprings()[i].pParticle > static_cast<int>(newPos))
        {
            unsigned int otherId = static_cast<unsigned int>(get_TangentialSprings()[i].pParticle);
            TangentialSpringParticle* otherTSParticle = dynamic_cast<TangentialSpringParticle*>(getHandler()->getObject(otherId));
            //Copy Tangentalspring to new location
            CTangentialSpring CTS = get_TangentialSprings()[i];
            CTS.pParticle = static_cast<int>(newPos);
            CTS.reverse();
            otherTSParticle->get_TangentialSprings().push_back(CTS);
            
            //Remove now unused tangentialspring
            get_TangentialSprings()[i] = get_TangentialSprings().back();
            get_TangentialSprings().pop_back();
        }
        else
        {
            i++;
        }
    }
}

void TangentialSpringParticle::oldRead(std::istream& is, std::string& x)
{
    Vec3D position, velocity, orientation, angularVelocity;
    position.X = atof(x.c_str());
    Mdouble radius, invMass, invInertia;
    is >> position.Y >> position.Z >> velocity >> radius >> orientation >> angularVelocity >> invMass >> invInertia >> tangentialSprings;
    setPosition(position);
    setVelocity(velocity);
    setOrientation(orientation);
    setAngularVelocity(angularVelocity);
    setRadius(radius);
    if (invMass!=0.0)
        setMass(1.0 / invMass);
    else
        setMass(1e20);
    if (invInertia!=0.0)
        setInertia(1.0 / invInertia);
    else
        setInertia(1e20);
}

