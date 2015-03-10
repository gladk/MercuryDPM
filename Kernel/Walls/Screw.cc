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

#include "Screw.h"
#include "InteractionHandler.h"
#include "Math/ExtendedMath.h"
#include "Particles/BaseParticle.h"

/*!
 * \details Make a Screw which is centered in the origin, has a length of 1, one
 * revelation, a radius of 1, turns with 1 revelation per second, is infinitely thin
 * and starts at its normal initial point.
 */
Screw::Screw()
{
    start_.setZero();
    l_ = 1.0;
    maxR_ = 1.0;
    n_ = 1.0;
    omega_ = 1.0;
    offset_ = 0.0;
    thickness_ = 0.0;
    logger(DEBUG, "Screw() constructor finished.");              
}

/*!
 * \param[in] other The Screw that has to be copied.
 */
Screw::Screw(const Screw& other)
    : BaseWall(other)
{
    start_ = other.start_;
    l_ = other.l_;
    maxR_ = other.maxR_;
    n_ = other.n_;
    omega_ = other.omega_;
    thickness_ = other.thickness_;
    offset_ = other.offset_;
    logger(DEBUG, "Screw(const Screw&) copy constructor finished.");
}

/*!
 * \param[in] start A Vec3D which denotes the centre of the lower end of the Screw.
 * \param[in] l The length of the Screw, must be positive.
 * \param[in] r The radius of the Screw, must be positive.
 * \param[in] n The number of revelations of the Screw, must be positive.
 * \param[in] omega The rotation speed of the Screw in rev/s.
 * \param[in] thickness The thickness of the Screw, must be non-negative.
 * \details Make a Screw by assigning all input parameters to the data-members of
 * this class, and setting the offset_ to 0.
 */
Screw::Screw(Vec3D start, Mdouble l, Mdouble r, Mdouble n, Mdouble omega, Mdouble thickness)
{
    start_ = start;
    l_ = l;
    maxR_ = r;
    n_ = n;
    omega_ = omega;
    thickness_ = thickness;
    offset_ = 0.0;
    logger(DEBUG, "Screw(Vec3D, Mdouble, Mdouble, Mdouble, Mdouble, Mdouble) constructor finished.");           
}

Screw::~Screw()
{
    logger(DEBUG, "~Screw() finished, destroyed the Screw.");
}

/*!
 * \return A pointer to a copy of this Screw.
 */
Screw* Screw::copy() const
{
    return new Screw(*this);
}

/*!
 * \param[in] p BaseParticle we want to calculate the distance and whether it collided of.
 * \param[out] distance The distance of the BaseParticle to this wall.
 * \param[out] normal_return If there was a collision, the normal vector to this wall will be placed here.
 * \return A boolean which says whether or not there was a collision.
 * \details This function computes whether or not there is a collision between 
 * a given BaseParticle and this Screw. If there is a collision, this
 * function also computes the distance between the BaseParticle and Screw
 * and the normal of the IntersectionOfWalls at the intersection point. 
 * \todo Make this function readable and explain the steps in the details.
 */
bool Screw::getDistanceAndNormal(const BaseParticle& p, Mdouble& distance, Vec3D& normal_return) const
{
    Mdouble Rsqr = pow(p.getPosition().X - start_.X, 2) + pow(p.getPosition().Y - start_.Y, 2);
    if (Rsqr > pow(maxR_ + p.getWallInteractionRadius() + thickness_, 2) || p.getPosition().Z > l_ + start_.Z + p.getWallInteractionRadius() + thickness_ || p.getPosition().Z < start_.Z - p.getWallInteractionRadius() - thickness_)
    {
        //std::cout<<"Particle is out of first bound checking"<<std::endl;
        //std::cout<<"Rule 1: "<<Rsqr<<"<"<<pow(r+P.getRadius()+thickness_,2)<<std::endl;
        //std::cout<<"Rule 2: "<<Start.Z-P.getRadius()-thickness_<<"<"<<P.getPosition().Z<<"<"<<L+Start.Z+P.getRadius()+thickness_<<std::endl;
        return false;
    }
    Mdouble R = sqrt(Rsqr);
    Mdouble alpha = atan2(p.getPosition().Y - start_.Y, p.getPosition().X - start_.X);
    Mdouble dz = p.getPosition().Z - start_.Z;
    
    //To find the contact point we have to minimize (with respect to r and q)
    //Distance^2=(x-x0-r*cos(2*Pi*(offset+N*q)))^2+(y-y0-r*sin(2*Pi*(offset+N*q)))^2+(z-z0-q*L)^2
    //Using polar coordinated (i.e. x-x0=R*cos(alpha), y-y0=R*sin(alpha) and dz=z-z0)
    //Distance^2=R^2+r^2-2*R*r*cos(alpha-2*Pi*(offset+N*q))+(dz-q*L)^2
    
    //Differentiation with respect to r and solve for zero:
    //0=2*r-2*R*cos(alpha-2*Pi*(offset+N*q)
    //r=R*cos(alpha-2*Pi*(offset+N*q))
    
    //Substitue back
    //Distance^2=R^2+R^2*cos^2(alpha-2*Pi*(offset+N*q))-2*R^2*cos(alpha-2*Pi*(offset+N*q))*cos(alpha-2*Pi*(offset+N*q))+(dz-q*L)^2
    //Distance^2=R^2*sin^2(alpha-2*Pi*(offset+N*q))+(dz-q*L)^2
    
    //So we have to minimize:
    //Distance^2=R^2*sin^2(alpha-2*Pi*(offset+N*q))^2+(dz-q*L)^2
    //For this we use the Euler algoritm
    
    Mdouble q; //Current guess
    Mdouble dd; //Derivative at current guess
    Mdouble ddd; //Second derivative at current guess
    Mdouble q0 = dz / l_; //Minimum of the parabolic part
            
    //The initial guess will be in the minimum of the sin closest to the minimum of the parabolic part
    //Minima of the sin are at
    //alpha-2*Pi*(offset+N*q)=k*Pi (k=integer)
    //q=alpha/(2*Pi*N)-k/(2*N)-offset/N (k=integer)
    
    Mdouble k = round(alpha / constants::pi - 2.0 * (offset_ + n_ * q0));
    q = alpha / (2.0 * constants::pi * n_) - k / (2.0 * n_) - offset_ / n_;
    
    //Now apply Newton's method
    do
    {
        dd = -2.0 * Rsqr * constants::pi * n_ * sin(2.0 * alpha - 4.0 * constants::pi * (n_ * q + offset_)) - 2.0 * l_ * (dz - q * l_);
        ddd = 8.0 * Rsqr * constants::sqr_pi * n_ * n_ * cos(2.0 * alpha - 4.0 * constants::pi * (n_ * q + offset_)) + 2.0 * l_ * l_;
        q -= dd / ddd;
    } while (fabs(dd / ddd) > 1e-14);
    
    //Calculate r
    Mdouble r = R * cos(2.0 * constants::pi * (offset_ + n_ * q) - alpha);
    
    //Check if the location is actually on the screw:
    //First posibility is that the radius is to large:
    if (fabs(r) > maxR_) //Left boundary of the coil
    {
        r = mathsFunc::sign(r) * maxR_;
        unsigned int steps = 0;
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
    if (distance >= (p.getWallInteractionRadius() + thickness_) * (p.getWallInteractionRadius() + thickness_))
    {
        return false;
    }
    
    Vec3D ContactPoint;
    distance = sqrt(distance) - thickness_;
    ContactPoint.X = start_.X + r * cos(2.0 * constants::pi * (offset_ + n_ * q));
    ContactPoint.Y = start_.Y + r * sin(2.0 * constants::pi * (offset_ + n_ * q));
    ContactPoint.Z = start_.Z + q * l_;
    normal_return = ContactPoint - p.getPosition();
    normal_return /= normal_return.getLength();
    return true;
}

/*!
 * \param[in] dt The time for which the Screw has to be turned.
 */
void Screw::move_time(Mdouble dt)
{
    offset_ += omega_ * dt;
}

/*!
 * \param[in,out] is Input stream from which the Screw must be read.
 */
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

/*!
 * \param[in,out] is Input stream from which the Screw must be read.
 * \details Read the Screw in old style, please note that the thickness is not 
 * read in this function, so it has either to be set manually or it is 0.0 from
 * the default constructor.
 */
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

/*!
 * \param[in,out] os Output stream to which the Screw must be written.
 */
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

/*!
 * \return The string "Screw".
 */
std::string Screw::getName() const
{
    return "Screw";
}

/*!
 * \param[in] p Pointer to the BaseParticle which we want to check the interaction for.
 * \param[in] timeStamp The time at which we want to look at the interaction.
 * \param[in] interactionHandler A pointer to the InteractionHandler in which the interaction can be found.
 * \return A pointer to the BaseInteraction that happened between this Screw
 * and the BaseParticle at the timeStamp.
 */
BaseInteraction* Screw::getInteractionWith(BaseParticle* p, Mdouble timeStamp, InteractionHandler* interactionHandler)
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
        c->setContactPoint(p->getPosition()-(p->getRadius() - 0.5 * c->getOverlap()) * c->getNormal());
        return c;
    }
    else
    {
        return nullptr;
    }
}
