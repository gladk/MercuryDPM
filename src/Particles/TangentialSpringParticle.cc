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

TangentialSpringParticle::TangentialSpringParticle() : BaseParticle()
{
    tangentialSprings.reset();
}

TangentialSpringParticle::TangentialSpringParticle(const TangentialSpringParticle &p) : BaseParticle(p)
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
    for(std::vector<CTangentialSpring>::iterator it = tangentialSprings.begin(); it!=tangentialSprings.end();it++)
    {
        it->reverse();
    }        
}

CTangentialSprings& TangentialSpringParticle::get_TangentialSprings() {return tangentialSprings;}

void TangentialSpringParticle::print(std::ostream& os) const
{
	os<<"TSP ";
	BaseParticle::print(os);
	os<<" "<<tangentialSprings;
}

void TangentialSpringParticle::read(std::istream& is)
{ 
	std::string dummy;
	is >> dummy;
	BaseParticle::read(is);
	is >> tangentialSprings;
}

void TangentialSpringParticle::moveInHandler(int newPos)
{
    BaseParticle::moveInHandler(newPos);
    //Check if some TangentialSpring information has to be moved
    for(unsigned int i=0;i<get_TangentialSprings().size();)
    {
        /// \todo Is a time restriction neccesary here?
        if (get_TangentialSprings()[i].pParticle>newPos)
        {
            int otherId=get_TangentialSprings()[i].pParticle;
            TangentialSpringParticle* otherTSParticle=dynamic_cast<TangentialSpringParticle*>(getHandler()->getObject(otherId));
            //Copy Tangentalspring to new location
            CTangentialSpring CTS=get_TangentialSprings()[i];
            CTS.pParticle=newPos;
            CTS.reverse();
            otherTSParticle->get_TangentialSprings().push_back(CTS);

            //Remove now unused tangentialspring
            get_TangentialSprings()[i]=get_TangentialSprings().back();
            get_TangentialSprings().pop_back();
        }
        else
        {
            i++;
        }
    }    
}

void TangentialSpringParticle::oldRead(std::istream& is,std::string& x)
{ 
	Vec3D position,velocity,angle,angularVelocity;
	position.X=atof(x.c_str());	
	Mdouble radius,invMass,invInertia;
	is >> position.Y >> position.Z >> velocity >> radius >> angle >> angularVelocity >> invMass >> invInertia >> tangentialSprings;
	set_Position(position);
	set_Velocity(velocity);
	set_Angle(angle);
	set_AngularVelocity(angularVelocity);
	set_Radius(radius);
	if (invMass) set_Mass(1.0/invMass); else set_Mass(1e20); 
	if (invInertia) set_inertia(1.0/invInertia); else set_inertia(1e20); 
}

