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

#include "CircularPeriodicBoundary.h"
#include "ParticleHandler.h"
#include "Particles/BaseParticle.h"

CircularPeriodicBoundary::CircularPeriodicBoundary()
        : BaseBoundary()
{
    innerRadius_ = 1.0;
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "CircularPeriodicBoundary::CircularPeriodicBoundary() finished" << std::endl;
#endif				
}

CircularPeriodicBoundary::CircularPeriodicBoundary(double innerRadius)
        : BaseBoundary()
{
    this->innerRadius_ = innerRadius;
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "CircularPeriodicBoundary::CircularPeriodicBoundary(double innerRadius) finished" << std::endl;
#endif				
}

CircularPeriodicBoundary::~CircularPeriodicBoundary()
{
	#ifdef DEBUG_DESTRUCTOR
    std::cerr << "CircularPeriodicBoundary::~CircularPeriodicBoundary() finished" << std::endl;
#endif	
}

CircularPeriodicBoundary* CircularPeriodicBoundary::copy() const
{
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "CircularPeriodicBoundary::copy() const finished" << std::endl;
#endif				
    return new CircularPeriodicBoundary(*this);
}

void CircularPeriodicBoundary::rotateParticle(BaseParticle *P, double angle)
{
    double R = sqrt(pow(P->getPosition().X, 2) + pow(P->getPosition().Y, 2));
    double alphaPos = atan2(P->getPosition().Y, P->getPosition().X);
    double V = sqrt(pow(P->getVelocity().X, 2) + pow(P->getVelocity().Y, 2));
    double alphaVel = atan2(P->getVelocity().Y, P->getVelocity().X);
    alphaPos += angle;
    alphaVel += angle;
    
    P->setPosition(Vec3D(cos(alphaPos) * R, sin(alphaPos * R), P->getPosition().Z));
    P->setVelocity(Vec3D(cos(alphaVel) * V, sin(alphaVel * V), P->getVelocity().Z));
    ///todo{Do we need to update rotations and rotational velocitys?}
}

void CircularPeriodicBoundary::createPeriodicParticles(BaseParticle *P, ParticleHandler &pH)
{
    double R = sqrt(pow(P->getPosition().X, 2) + pow(P->getPosition().Y, 2));
    double alpha = atan2(P->getPosition().Y, P->getPosition().X);
    unsigned int i = static_cast<unsigned int>(std::floor(std::log(R / innerRadius_) / std::log(2.0))) + 1;
    double pieSize = 2.0 / pow(2.0, i) * constants::pi;
    //std::cout<<"R="<<R<<" alpha="<<alpha<<" i="<<i<<" pieSize="<<pieSize<<std::endl;
    
    //Check if the particle is close to it's inner Radius or is is close to zero alpha (small y)
    if (i > 0 && (R - (P->getInteractionRadius() + pH.getLargestParticle()->getInteractionRadius()) < pow(2.0, i - 1) * innerRadius_ || P->getPosition().Y < (P->getInteractionRadius() + pH.getLargestParticle()->getInteractionRadius())))
    {
        //std::cout<<"Going to shift because "<<R-P->getRadius()<<"<"<<pow(2,i-1)*innerRadius<<" or "<<P->getPosition().Y<<"<"<<P->getRadius()<<std::endl;
        //std::cout<<*P<<" has been shifted"<<std::endl;
        
        BaseParticle* F0 = P->copy();
        rotateParticle(F0, pieSize);
        
        //If Particle is Mdouble shifted, get correct original particle			
        BaseParticle* From = P;
        while (From->getPeriodicFromParticle() != nullptr)
            From = From->getPeriodicFromParticle();
        F0->setPeriodicFromParticle(From);
        
        //std::cout<<*F0<<" is the result"<<std::endl;
        pH.addObject(F0);
    }
    //Check here only for i>0 becuase for i=1 they both give the same particle
    if (i > 1 && R * R * (1 - pow(cos(alpha - pieSize), 2)) < P->getInteractionRadius() + pH.getLargestParticle()->getInteractionRadius())
    {
        //std::cout<<*P<<" has been shifted back"<<std::endl;
        
        BaseParticle* F0 = P->copy();
        rotateParticle(F0, -pieSize);
        
        //If Particle is Mdouble shifted, get correct original particle			
        BaseParticle* From = P;
        while (From->getPeriodicFromParticle() != nullptr)
            From = From->getPeriodicFromParticle();
        F0->setPeriodicFromParticle(From);
        
        //std::cout<<*F0<<" is the result"<<std::endl;
        pH.addObject(F0);
    }
}

bool CircularPeriodicBoundary::checkBoundaryAfterParticleMoved(BaseParticle *P, ParticleHandler &pH)
{
    double R = sqrt(pow(P->getPosition().X, 2) + pow(P->getPosition().Y, 2));
    double alpha = atan2(P->getPosition().Y, P->getPosition().X);
    int i = static_cast<int>(std::floor(std::log(R / innerRadius_) / std::log(2.0))) + 1;
    double pieSize = 2.0 / pow(2.0, i) * constants::pi;
    
    double oldR = sqrt(pow(P->getPosition().X - P->getDisplacement().X, 2) + pow(P->getPosition().Y - P->getDisplacement().Y, 2));
    ///\todo{TW: Dinant, please confirm that i and oldI should be integer}
    int oldI = static_cast<int>(std::floor(std::log(oldR / innerRadius_) / std::log(2.0))) + 1;
    
    if (i > 0 && i > oldI) //Particle moves outward so it may have to be deleted
    {
        //std::cout<<"Particle="<<P->getIndex()<<" moving outward with alpha="<<alpha<<" and pieSize="<<pieSize<<" ";
        if (alpha < 0 || alpha > pieSize)
        {
            if (alpha > 2.0 * pieSize)
            {
                //std::cout<<"and it is rotated into the pie"<<std::endl;
                rotateParticle(P, -pieSize);
            }
            else
            {
                //Delete particle
                //std::cout<<"and it should be deleted"<<std::endl;
                pH.removeObject(P->getIndex());
                return true;
            }
        }
        //else
        //std::cout<<"and nothing happens"<<std::endl;
    }
    else if (i >= 0 && i < oldI) //Particle moves inward so it has to be coppied
    {
        //std::cout<<"Particle="<<P->getIndex()<<" moving inward and is thus coppied with alpha="<<alpha<<" and pieSize="<<pieSize<<std::endl;
        //std::cout<<"i="<<i<<" oldI="<<oldI<<" R="<<R<<" oldR="<<oldR<<std::endl;
        //std::cout<<"Position="<<P->getPosition()<<" Displacement="<<P->get_Displacement()<<std::endl;
        BaseParticle* F0 = P->copy();
        F0->setDisplacement(Vec3D(0.0, 0.0, 0.0));
        if (alpha < 0)
        {
            rotateParticle(P, pieSize);
            rotateParticle(F0, 0.5 * pieSize);
        }
        else if (alpha < 0.5 * pieSize)
        {
            rotateParticle(F0, 0.5 * pieSize);
        }
        else if (alpha < pieSize)
        {
            rotateParticle(F0, -0.5 * pieSize);
        }
        else
        {
            rotateParticle(P, -pieSize);
            rotateParticle(F0, -0.5 * pieSize);
        }
        pH.addObject(F0);
    }
    else if (i > 0 && alpha < 0)
    {
        //std::cout<<"Particle="<<P->getIndex()<<" i="<<i<<" R="<<R<<" alpha="<<alpha<<" positive rotated pieSize="<<pieSize<<std::endl;			
        rotateParticle(P, pieSize);
    }
    else if (i > 0 && alpha > pieSize)
    {
        //std::cout<<"Particle="<<P->getIndex()<<" i="<<i<<" R="<<R<<" alpha="<<alpha<<" negative rotated pieSize="<<pieSize<<std::endl;			
        rotateParticle(P, -pieSize);
    }
    return false;
}

void CircularPeriodicBoundary::read(std::istream& is)
{
    BaseBoundary::read(is);
    std::string dummy;
    is >> dummy >> innerRadius_;
}

void CircularPeriodicBoundary::oldRead(std::istream& is)
{
    std::string dummy;
    is >> dummy >> innerRadius_;
}

void CircularPeriodicBoundary::write(std::ostream& os) const
        {
    BaseBoundary::write(os);
    os << " innerRadius " << innerRadius_;
}

std::string CircularPeriodicBoundary::getName() const
{
    return "CircularPeriodicBoundary";
}
