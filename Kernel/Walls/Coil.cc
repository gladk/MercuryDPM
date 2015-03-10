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

/*!
 * \details Default constructor, make a Coil which is centered in the origin, has
 * length 1, radius 1, 1 revelation, rotates with 1 rev/s, has thickness 0 and has
 * not rotated.
 */
Coil::Coil()
{
    start_.setZero();
    l_ = 1.0;
    r_ = 1.0;
    n_ = 1.0;
    omega_ = 1.0;
    offset_ = 0.0;
    thickness_ = 0.0;
    logger(DEBUG, "Coil() constructor finished");              
}

/*!
 * \param[in] other Coil to be copied into this coil
 */
Coil::Coil(const Coil& other) : BaseWall(other)
{
    start_ = other.start_;
    l_ = other.l_;
    r_ = other.r_;
    n_ = other.n_;
    omega_ = other.omega_;
    offset_ = other.offset_;
    thickness_ = other.thickness_;
    logger(DEBUG, "Coil(const Coil&) copy constructor finished");
}

/*!
 * \param[in] start A Vec3D which denotes the centre of the lower end of the Coil.
 * \param[in] l The length of the Coil, must be positive.
 * \param[in] r The radius of the Coil, must be positive.
 * \param[in] n The number of revelations of the Coil, must be positive.
 * \param[in] omega The rotation speed of the Coil in rev/s.
 * \param[in] thickness The thickness of the "spiral" of the Coil, must be non-negative.
 * \details Make a Coil by assigning all input parameters to the data-members of
 * this class, and setting the offset_ to 0.
 */
Coil::Coil(Vec3D start, Mdouble l, Mdouble r, Mdouble n, Mdouble omega, Mdouble thickness)
{
    start_ = start;
    l_ = l;
    r_ = r;
    n_ = n;
    omega_ = omega;
    thickness_ = thickness;
    offset_ = 0.0;
    logger(DEBUG, "Coil(params) constructor with parameters finished.");             
}

Coil::~Coil()
{
    logger(DEBUG, "~Coil() destructor finished.");
}

/*!
 * \details Set the parameters of this Coil by assigning all input parameters to
 *  the data-members of this class, and setting the offset_ to 0.
 * 
 * \param[in] start A Vec3D which denotes the centre of the lower end of the Coil.
 * \param[in] length        The length of the Coil, must be positive.
 * \param[in] radius        The radius of the Coil, must be positive.
 * \param[in] numberOfRevelations   The number of revelations of the Coil, must be positive.
 * \param[in] omega         The rotation speed of the Coil in rev/s.
 * \param[in] thickness     The thickness of the "spiral" of the Coil, must be non-negative.
 * 
 */
void Coil::set(Vec3D start, Mdouble length, Mdouble radius, Mdouble numberOfRevelations, Mdouble omega, Mdouble thickness)
{
    start_ = start;
    l_ = length;
    r_ = radius;
    n_ = numberOfRevelations;
    omega_ = omega;
    thickness_ = thickness;
    offset_ = 0.0;
}

/*!
 * \return A pointer to a copy of this Coil.
 */
Coil* Coil::copy() const
{
    return new Coil(*this);
}

/*!
 * \param[in] p BaseParticle we want to calculate the distance and whether it collided of.
 * \param[out] distance The distance of the BaseParticle to this Coil.
 * \param[out] normal_return If there was a collision, the normal vector to this Coil will be placed here.
 * \return A boolean which says whether or not there was a collision.
 * \details This function computes whether or not there is a collision between 
 * a given BaseParticle and this Coil. If there is a collision, this
 * function also computes the distance between the BaseParticle and Coil
 * and the normal of the IntersectionOfWalls at the intersection point. 
 * \todo Make this function readable and explain the steps in the details.
 */
bool Coil::getDistanceAndNormal(const BaseParticle& p, Mdouble& distance, Vec3D& normal_return) const
{
    Mdouble Rsqr = pow(p.getPosition().X - start_.X, 2) + pow(p.getPosition().Y - start_.Y, 2);
    if (Rsqr > pow(r_ + p.getWallInteractionRadius() + thickness_, 2) || Rsqr < pow(r_ - p.getWallInteractionRadius() - thickness_, 2) || p.getPosition().Z > l_ + start_.Z + p.getWallInteractionRadius() + thickness_ || p.getPosition().Z < start_.Z - p.getWallInteractionRadius() - thickness_)
    {
        //std::cout<<"Particle is out of first bound checking"<<std::endl;
        //std::cout<<"Rule 1: "<<pow(r-P.getRadius()-thickness_,2)<<"<"<<Rsqr<<"<"<<pow(r+P.getRadius()+thickness_,2)<<std::endl;
        //std::cout<<"Rule 2: "<<Start.Z-P.getRadius()-thickness_<<"<"<<P.getPosition().Z<<"<"<<L+Start.Z+P.getRadius()+thickness_<<std::endl;
        return false;
    }
    Mdouble R = sqrt(Rsqr);
    Mdouble alpha = atan2(p.getPosition().Y - start_.Y, p.getPosition().X - start_.X);
    Mdouble dz = p.getPosition().Z - start_.Z;
    
    //To find the contact point we have to minimize (with respect to q)
    //Distance^2=(x-x0-r*cos(2*Pi*(offset+N*q)))^2+(y-y0-r*sin(2*Pi*(offset+N*q)))^2+(z-z0-q*L)^2
    //Using polar coordinated (i.e. x-x0=R*cos(alpha), y-y0=R*sin(alpha) and dz=z-z0)
    //Distance^2=R^2+r^2-2*R*r*cos(alpha-2*Pi*(offset+N*q))+(dz-q*L)^2
    
    //So we have to minimize:
    //Distance^2=R^2+r^2-2*R*r*cos(alpha-2*Pi*(offset+N*q))+(dz-q*L)^2
    //For this we use the Euler algoritm
    
    Mdouble q; //Current guess
    Mdouble dd; //Derivative at current guess
    Mdouble ddd; //Second derivative at current guess
    Mdouble q0 = dz / l_; //Minimum of the parabolic part
            
    //The initial guess will be in the maximum of the cos closest to the minimum of the parabolic part
    //Minima of the cos are at
    //alpha-2*Pi*(offset+N*q)=2*k*Pi (k=integer)
    //q=alpha/(2*Pi*N)-k/N-offset/N (k=integer)
    
    Mdouble k = round(alpha / 2.0 / constants::pi - (offset_ + n_ * q0));
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
    if (distanceSquared >= (p.getWallInteractionRadius() + thickness_) * (p.getWallInteractionRadius() + thickness_))
    {
        //std::cout<<"Particle is out of second bound checking, distance^2="<<distance<<" max="<<(P.getRadius()+thickness_)*(P.getRadius()+thickness_)<<std::endl;
        return false;
    }
    
    Vec3D ContactPoint;
    distance = sqrt(distanceSquared) - thickness_;
    ContactPoint.X = start_.X + r_ * cos(2.0 * constants::pi * (offset_ + n_ * q));
    ContactPoint.Y = start_.Y + r_ * sin(2.0 * constants::pi * (offset_ + n_ * q));
    ContactPoint.Z = start_.Z + q * l_;
    normal_return = ContactPoint - p.getPosition();
    normal_return /= normal_return.getLength();
    return true;
}

/*!
 * \param[in] p Pointer to the BaseParticle which we want to check the interaction for.
 * \param[in] timeStamp The time at which we want to look at the interaction.
 * \param[in] interactionHandler A pointer to the InteractionHandler in which the interaction can be found.
 * \return A pointer to the BaseInteraction that happened between this Coil
 * and the BaseParticle at the timeStamp.
 */
BaseInteraction* Coil::getInteractionWith(BaseParticle* p, Mdouble timeStamp, InteractionHandler* interactionHandler)
{
    Mdouble distance;
    Vec3D normal;
    if (getDistanceAndNormal(*p,distance,normal))
    {
        BaseInteraction* c = interactionHandler->getInteraction(p, this, timeStamp);
        c->setNormal(-normal);
        c->setDistance(distance);
        c->setOverlap(p->getRadius() - distance);
        ///\todo{DK: What is the contact point for interactions with walls}
        c->setContactPoint(p->getPosition() - (p->getRadius()- 0.5 * c->getOverlap()) * c->getNormal());
        return c;
    }
    else
    {
        return nullptr;
    }
}

/*!
 * \param[in] dt The time for which the Screw has to be turned.
 */
void Coil::move_time(Mdouble dt)
{
    offset_ += omega_ * dt;
}

/*!
 * \param[in,out] is The input stream from which the Coil is read.
 */
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

/*!
 * \param[in,out] is The input stream from which the Coil is read.
 */
void Coil::oldRead(std::istream& is)
{
    std::string dummy;
    is >> dummy >> start_
            >> dummy >> l_
            >> dummy >> r_ >> dummy >> n_
            >> dummy >> omega_
            >> dummy >> offset_;
}

/*!
 * \param[in,out] os The outpus stream to which the Coil is written.
 */
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

/*!
 * \return The string "Coil".
 */
std::string Coil::getName() const
{
    return "Coil";
}
