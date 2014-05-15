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

