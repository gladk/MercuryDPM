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

#include "Coil.h"
#include "InteractionHandler.h"
#include "Particles/BaseParticle.h"

Coil::Coil()
        : BaseWall()
{
    start_.setZero();
    l_ = 1;
    r_ = 1;
    n_ = 1;
    omega_ = 1;
    offset_ = 0;
    thickness_ = 0;
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "Coil() finished" << std::endl;
#endif              
}

Coil::Coil(Vec3D Start, double L, double r, double N, double omega, double thickness)
        : BaseWall()
{
    start_ = Start;
    l_ = L;
    r_ = r;
    n_ = N;
    omega_ = omega;
    thickness_ = thickness;
    offset_ = 0.0;
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "Coil() finished" << std::endl;
#endif              
}

void Coil::set(Vec3D Start, double L, double r, double N, double omega, double thickness)
{
    start_ = Start;
    l_ = L;
    r_ = r;
    n_ = N;
    omega_ = omega;
    thickness_ = thickness;
    offset_ = 0.0;
}

Coil* Coil::copy() const
{
    return new Coil(*this);
}

bool Coil::getDistanceAndNormal(const BaseParticle &P, Mdouble &distance, Vec3D &normal_return) const
{
    double Rsqr = pow(P.getPosition().X - start_.X, 2) + pow(P.getPosition().Y - start_.Y, 2);
    if (Rsqr > pow(r_ + P.getWallInteractionRadius() + thickness_, 2) || Rsqr < pow(r_ - P.getWallInteractionRadius() - thickness_, 2) || P.getPosition().Z > l_ + start_.Z + P.getWallInteractionRadius() + thickness_ || P.getPosition().Z < start_.Z - P.getWallInteractionRadius() - thickness_)
    {
        //std::cout<<"Particle is out of first bound checking"<<std::endl;
        //std::cout<<"Rule 1: "<<pow(r-P.getRadius()-thickness_,2)<<"<"<<Rsqr<<"<"<<pow(r+P.getRadius()+thickness_,2)<<std::endl;
        //std::cout<<"Rule 2: "<<Start.Z-P.getRadius()-thickness_<<"<"<<P.getPosition().Z<<"<"<<L+Start.Z+P.getRadius()+thickness_<<std::endl;
        return false;
    }
    double R = sqrt(Rsqr);
    double alpha = atan2(P.getPosition().Y - start_.Y, P.getPosition().X - start_.X);
    double dz = P.getPosition().Z - start_.Z;
    
    ///To find the contact point we have to minimize (with respect to q)
    ///Distance^2=(x-x0-r*cos(2*Pi*(offset+N*q)))^2+(y-y0-r*sin(2*Pi*(offset+N*q)))^2+(z-z0-q*L)^2
    ///Using polar coordinated (i.e. x-x0=R*cos(alpha), y-y0=R*sin(alpha) and dz=z-z0)
    ///Distance^2=R^2+r^2-2*R*r*cos(alpha-2*Pi*(offset+N*q))+(dz-q*L)^2
    
    ///So we have to minimize:
    ///Distance^2=R^2+r^2-2*R*r*cos(alpha-2*Pi*(offset+N*q))+(dz-q*L)^2
    ///For this we use the Euler algoritm
    
    double q; //Current guess
    double dd; //Derivative at current guess
    double ddd; //Second derivative at current guess
    double q0 = dz / l_; //Minimum of the parabolic part
            
    ///The initial guess will be in the maximum of the cos closest to the minimum of the parabolic part
    ///Minima of the cos are at
    ///alpha-2*Pi*(offset+N*q)=2*k*Pi (k=integer)
    ///q=alpha/(2*Pi*N)-k/N-offset/N (k=integer)
    
    double k = round(alpha / 2.0 / constants::pi - (offset_ + n_ * q0));
    q = alpha / (2 * constants::pi * n_) - k / n_ - offset_ / n_;
    
    //Now apply Newton's method
    do
    {
        dd = -4.0 * R * r_ * constants::pi * n_ * sin(alpha - 2.0 * constants::pi * (n_ * q + offset_)) - 2.0 * l_ * (dz - q * l_);
        ddd = 8.0 * R * r_ * constants::sqr_pi * n_ * n_ * cos(alpha - 2.0 * constants::pi * (n_ * q + offset_)) + 2.0 * l_ * l_;
        q -= dd / ddd;
    } while (fabs(dd / ddd) > 1e-14);
    
    //Check if the location is actually on the coil, otherwise a point collision with the end of the coil calculated
    if (q < 0) //Left boundary of the coil
    {
        q = 0;
    }
    else if (q > 1) //right boundary of the coil
    {
        q = 1;
    }
    
    Mdouble distanceSquared = R * R + r_ * r_ - 2 * R * r_ * cos(alpha - 2 * constants::pi * (offset_ + n_ * q)) + pow(dz - q * l_, 2);
    //If distance is too large there is no contact
    if (distanceSquared >= (P.getWallInteractionRadius() + thickness_) * (P.getWallInteractionRadius() + thickness_))
    {
        //std::cout<<"Particle is out of second bound checking, distance^2="<<distance<<" max="<<(P.getRadius()+thickness_)*(P.getRadius()+thickness_)<<std::endl;
        return false;
    }
    
    Vec3D ContactPoint;
    distance = sqrt(distanceSquared) - thickness_;
    ContactPoint.X = start_.X + r_ * cos(2.0 * constants::pi * (offset_ + n_ * q));
    ContactPoint.Y = start_.Y + r_ * sin(2.0 * constants::pi * (offset_ + n_ * q));
    ContactPoint.Z = start_.Z + q * l_;
    normal_return = ContactPoint - P.getPosition();
    normal_return /= normal_return.getLength();
    return true;
}

BaseInteraction* Coil::getInteractionWith(BaseParticle *P, Mdouble timeStamp, InteractionHandler* interactionHandler)
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

///Allows the wall to be moved to a new position (also orthogonal to the normal), and setting the velocity
void Coil::move_time(Mdouble dt)
{
    offset_ += omega_ * dt;
}

///reads wall
void Coil::read(std::istream& is)
{
    BaseWall::read(is);
    std::string dummy;
    is >> dummy >> start_
            >> dummy >> l_
            >> dummy >> r_
            >> dummy >> n_
            >> dummy >> omega_
            >> dummy >> thickness_
            >> dummy >> offset_;
}

void Coil::oldRead(std::istream& is)
{
    std::string dummy;
    is >> dummy >> start_
            >> dummy >> l_
            >> dummy >> r_ >> dummy >> n_
            >> dummy >> omega_
            >> dummy >> offset_;
}

///outputs wall
void Coil::write(std::ostream& os) const
        {
    BaseWall::write(os);
    os << " Start " << start_
            << " Length " << l_
            << " Radius " << r_
            << " Revolutions " << n_
            << " Omega " << omega_
            << " Thickness " << thickness_
            << " Offset " << offset_;
}

std::string Coil::getName() const
{
    return "Coil";
}
