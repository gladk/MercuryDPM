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

#include "Screw.h"
#include "InteractionHandler.h"
#include "Math/ExtendedMath.h"
#include "Particles/BaseParticle.h"

Screw::Screw()
        : BaseWall()
{
    start_.setZero();
    l_ = 1;
    maxR_ = 1;
    n_ = 1;
    omega_ = 1;
    offset_ = 0;
    thickness_ = 0;
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "Screw() finished" << std::endl;
#endif              
}

Screw::Screw(Vec3D Start, double L, double R, double N, double omega, double thickness)
        : BaseWall()
{
    start_ = Start;
    l_ = L;
    maxR_ = R;
    n_ = N;
    omega_ = omega;
    thickness_ = thickness;
    offset_ = 0.0;
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "Screw() finished" << std::endl;
#endif              
}

Screw* Screw::copy() const
{
    return new Screw(*this);
}

bool Screw::getDistanceAndNormal(const BaseParticle &P, Mdouble &distance, Vec3D &normal_return) const
{
    double Rsqr = pow(P.getPosition().X - start_.X, 2) + pow(P.getPosition().Y - start_.Y, 2);
    if (Rsqr > pow(maxR_ + P.getWallInteractionRadius() + thickness_, 2) || P.getPosition().Z > l_ + start_.Z + P.getWallInteractionRadius() + thickness_ || P.getPosition().Z < start_.Z - P.getWallInteractionRadius() - thickness_)
    {
        //std::cout<<"Particle is out of first bound checking"<<std::endl;
        //std::cout<<"Rule 1: "<<Rsqr<<"<"<<pow(r+P.getRadius()+thickness_,2)<<std::endl;
        //std::cout<<"Rule 2: "<<Start.Z-P.getRadius()-thickness_<<"<"<<P.getPosition().Z<<"<"<<L+Start.Z+P.getRadius()+thickness_<<std::endl;
        return false;
    }
    double R = sqrt(Rsqr);
    double alpha = atan2(P.getPosition().Y - start_.Y, P.getPosition().X - start_.X);
    double dz = P.getPosition().Z - start_.Z;
    
    ///To find the contact point we have to minimize (with respect to r and q)
    ///Distance^2=(x-x0-r*cos(2*Pi*(offset+N*q)))^2+(y-y0-r*sin(2*Pi*(offset+N*q)))^2+(z-z0-q*L)^2
    ///Using polar coordinated (i.e. x-x0=R*cos(alpha), y-y0=R*sin(alpha) and dz=z-z0)
    ///Distance^2=R^2+r^2-2*R*r*cos(alpha-2*Pi*(offset+N*q))+(dz-q*L)^2
    
    ///Differentiation with respect to r and solve for zero:
    ///0=2*r-2*R*cos(alpha-2*Pi*(offset+N*q)
    ///r=R*cos(alpha-2*Pi*(offset+N*q))
    
    ///Substitue back
    ///Distance^2=R^2+R^2*cos^2(alpha-2*Pi*(offset+N*q))-2*R^2*cos(alpha-2*Pi*(offset+N*q))*cos(alpha-2*Pi*(offset+N*q))+(dz-q*L)^2
    ///Distance^2=R^2*sin^2(alpha-2*Pi*(offset+N*q))+(dz-q*L)^2
    
    ///So we have to minimize:
    ///Distance^2=R^2*sin^2(alpha-2*Pi*(offset+N*q))^2+(dz-q*L)^2
    ///For this we use the Euler algoritm
    
    double q; //Current guess
    double dd; //Derivative at current guess
    double ddd; //Second derivative at current guess
    double q0 = dz / l_; //Minimum of the parabolic part
            
    ///The initial guess will be in the minimum of the sin closest to the minimum of the parabolic part
    ///Minima of the sin are at
    ///alpha-2*Pi*(offset+N*q)=k*Pi (k=integer)
    ///q=alpha/(2*Pi*N)-k/(2*N)-offset/N (k=integer)
    
    double k = round(alpha / constants::pi - 2.0 * (offset_ + n_ * q0));
    q = alpha / (2.0 * constants::pi * n_) - k / (2.0 * n_) - offset_ / n_;
    
    //Now apply Newton's method
    do
    {
        dd = -2.0 * Rsqr * constants::pi * n_ * sin(2.0 * alpha - 4.0 * constants::pi * (n_ * q + offset_)) - 2.0 * l_ * (dz - q * l_);
        ddd = 8.0 * Rsqr * constants::sqr_pi * n_ * n_ * cos(2.0 * alpha - 4.0 * constants::pi * (n_ * q + offset_)) + 2.0 * l_ * l_;
        q -= dd / ddd;
    } while (fabs(dd / ddd) > 1e-14);
    
    //Calculate r
    double r = R * cos(2.0 * constants::pi * (offset_ + n_ * q) - alpha);
    
    //Check if the location is actually on the screw:
    //First posibility is that the radius is to large:
    if (fabs(r) > maxR_) //Left boundary of the coil
    {
        r = mathsFunc::sign(r) * maxR_;
        int steps = 0;
        //This case reduces to the coil problem
        do
        {
            dd = -4.0 * R * r * constants::pi * n_ * sin(alpha - 2.0 * constants::pi * (n_ * q + offset_)) - 2.0 * l_ * (dz - q * l_);
            ddd = 8.0 * R * r * constants::sqr_pi * n_ * n_ * cos(alpha - 2.0 * constants::pi * (n_ * q + offset_)) + 2.0 * l_ * l_;
            q -= dd / ddd;
            steps++;
        } while (fabs(dd / ddd) > 1e-14);
    }
    //Second possibility is that it occured before the start of after the end
    if (q < 0)
    {
        q = 0;
        r = R * cos(alpha - 2.0 * constants::pi * (offset_ + q * n_));
        if (fabs(r) > maxR_)
        {
            r = mathsFunc::sign(r) * maxR_;
        }
    }
    else if (q > 1)
    {
        q = 1;
        r = R * cos(alpha - 2.0 * constants::pi * (offset_ + q * n_));
        if (fabs(r) > maxR_)
        {
            r = mathsFunc::sign(r) * maxR_;
        }
    }
    
    distance = R * R * pow(sin(alpha - 2 * constants::pi * (offset_ + n_ * q)), 2) + pow(dz - q * l_, 2);
    //If distance is to large there is no contact
    if (distance >= (P.getWallInteractionRadius() + thickness_) * (P.getWallInteractionRadius() + thickness_))
    {
        return false;
    }
    
    Vec3D ContactPoint;
    distance = sqrt(distance) - thickness_;
    ContactPoint.X = start_.X + r * cos(2.0 * constants::pi * (offset_ + n_ * q));
    ContactPoint.Y = start_.Y + r * sin(2.0 * constants::pi * (offset_ + n_ * q));
    ContactPoint.Z = start_.Z + q * l_;
    normal_return = ContactPoint - P.getPosition();
    normal_return /= normal_return.getLength();
    return true;
}

///Allows the wall to be moved to a new position (also orthogonal to the normal), and setting the velocity
void Screw::move_time(Mdouble dt)
{
    offset_ += omega_ * dt;
}

///reads wall
void Screw::read(std::istream& is)
{
    BaseWall::read(is);
    std::string dummy;
    is >> dummy >> start_
            >> dummy >> l_
            >> dummy >> maxR_
            >> dummy >> n_
            >> dummy >> omega_
            >> dummy >> thickness_
            >> dummy >> offset_;
}

void Screw::oldRead(std::istream& is)
{
    std::string dummy;
    is >> dummy >> start_
            >> dummy >> l_
            >> dummy >> maxR_
            >> dummy >> n_
            >> dummy >> omega_
            >> dummy >> offset_;
}

///outputs wall
void Screw::write(std::ostream& os) const
        {
    BaseWall::write(os);
    os << " start " << start_
            << " Length " << l_
            << " Radius " << maxR_
            << " Revolutions " << n_
            << " Omega " << omega_
            << " Thickness " << thickness_
            << " Offset " << offset_;
}

std::string Screw::getName() const
{
    return "Screw";
}

BaseInteraction* Screw::getInteractionWith(BaseParticle *P, Mdouble timeStamp, InteractionHandler* interactionHandler)
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
