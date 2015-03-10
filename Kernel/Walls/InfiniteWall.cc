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

#include "InfiniteWall.h"
#include "Particles/BaseParticle.h"
#include "InteractionHandler.h"

InfiniteWall::InfiniteWall()
{
    factor_ = std::numeric_limits<double>::quiet_NaN();
    logger(DEBUG, "InfiniteWall::InfiniteWall ) finished");
}

/*!
 * \param[in] w InfiniteWall that has to be copied.
 * \details First copy the attributes of the BaseWall, then copy the ones that are
 * specific for the InfiniteWall.
 */
InfiniteWall::InfiniteWall(const InfiniteWall& w)
        : BaseWall(w)
{
    normal_ = w.normal_;
    factor_ = w.factor_;
    logger(DEBUG, "InfiniteWall::InfiniteWall(const InfiniteWall &p) finished");
}

InfiniteWall::~InfiniteWall()
{
    logger(DEBUG, "InfiniteWall::~InfiniteWall finished");
}

/*!
 * Wall copy method. It calls the copy contructor of this Wall, useful for polymorphism
 */
InfiniteWall* InfiniteWall::copy() const
{
    return new InfiniteWall(*this);
}

void InfiniteWall::clear()
{
}

/*
 * \param[in] normal A Vec3D that represents the normal to the wall.
 * \param[in] point A Vec3D which is a point on the wall.
 * \details Sets the wall such that for all points x on the wall it holds that 
 * normal*x=normal*point.
 */
void InfiniteWall::set(Vec3D normal, Vec3D point)
{
    setNormal(normal);
    setPosition(point);
}

/*!
 * \param[in] normal The vector normal to the wall.
 */
void InfiniteWall::setNormal(const Vec3D normal)
{
    factor_ = 1. / Vec3D::getLength(normal);
    normal_ = normal * factor_;
}

/*!
 * \details Defines a standard wall, given an outward normal vector s. t. normal*x=position
 * \param[in] normal A Vec3D that represents the normal vector to the wall.
 * \param[in] positionInNormalDirection The position of the wall in the direction
 *  of the normal vector.
 */
void InfiniteWall::set(Vec3D normal, Mdouble positionInNormalDirection)
{
    logger(WARN, "InfiniteWall::set(Vec3D, Mdouble) is deprecated. Use set(Vec3D, Vec3D) instead.");
    set(normal,positionInNormalDirection*normal);
}

/*!
 * \param[in] positionInNormalDirection The position of the wall in the direction 
 * of the normal vector. 
 */
void InfiniteWall::move(Mdouble positionInNormalDirection)
{
    logger(WARN, "InfiniteWall::move(Mdouble) is deprecated. Use move(Vec3D) instead.");
    setPosition(positionInNormalDirection * normal_ / factor_);
}

/*!
 * \param[in] otherPosition The position to which the distance must be computed to.
 * \return The distance of the wall to the particle.
 */ 
Mdouble InfiniteWall::getDistance(const Vec3D& otherPosition) const
{
    return Vec3D::dot(getPosition()-otherPosition, normal_);
}

/*!
 * \param[in] p BaseParticle for which the distance to the wall must be computed.
 * \param[out] distance Distance between the particle and the wall.
 * \param[out] normal_return The normal of this wall, will only be set if there is a collision.
 * \return A boolean value for whether or not there is a collision.
 * \details First the distance is checked. If there is no collision, this
 * function will return false and give the distance. If there is a collision, the
 * function will return true and give the distance and the normal vector of this wall.
 * Since this function should be called before calculating any 
 * Particle-Wall interactions, it can also be used to set the normal vector in 
 * case of curved walls.
 */
bool InfiniteWall::getDistanceAndNormal(const BaseParticle& p, Mdouble& distance, Vec3D& normal_return) const
{
    distance = getDistance(p.getPosition());
    if (distance >= p.getWallInteractionRadius())
        return false;
    normal_return = normal_;
    return true;
}

/*!
 * \param[in] is The input stream from which the InfiniteWall is read.
 */
void InfiniteWall::read(std::istream& is)
{
    BaseWall::read(is);
    std::string dummy;
    is >> dummy >> normal_;
    is >> dummy >>factor_;
}

/*!
 * \param[in] is The input stream from which the InfiniteWall old style is read.
 */
void InfiniteWall::oldRead(std::istream& is)
{
    std::string dummy;
    Vec3D velocity;
    Vec3D position;
    is >> dummy >> normal_ >> dummy >> position >> dummy >> velocity;
    setPosition(position);
    setVelocity(velocity);
}

/*!
 * \param[in] os The output stream the InfiniteWall is written to.
 */
void InfiniteWall::write(std::ostream& os) const
{
    BaseWall::write(os);
    os << " normal " << normal_
            << " factor "<<factor_;
}

/*!
 * \return The string "InfiniteWall", which is the name of this class.
 */
std::string InfiniteWall::getName() const
{
    return "InfiniteWall";
}

/*!
 * \return The 3D vector that represents the normal to the wall.
 */
Vec3D InfiniteWall::getNormal() const
{
    return normal_;
}

/*!
 * \param[in] p Pointer to the BaseParticle which we want to check the interaction for.
 * \param[in] timeStamp The time at which we want to look at the interaction.
 * \param[in] interactionHandler A pointer to the InteractionHandler in which the interaction can be found.
 * \return A pointer to the BaseInteraction that happened between this InfiniteWall
 * and the BaseParticle at the timeStamp.
 */
BaseInteraction* InfiniteWall::getInteractionWith(BaseParticle* p, Mdouble timeStamp, InteractionHandler* interactionHandler)
{
    Mdouble distance = getDistance(p->getPosition());
    if (distance < p->getWallInteractionRadius())
    {
        BaseInteraction* interaction = interactionHandler->getInteraction(p, this, timeStamp);
        interaction->setNormal(-normal_);
        interaction->setDistance(distance);
        interaction->setOverlap(p->getRadius() - distance);
        ///\todo{DK: What is the contact point for interactions with walls}
//        interaction->setContactPoint(p->getPosition()-(p->getRadius()- 1.0 * interaction->getOverlap())*interaction->getNormal());
        interaction->setContactPoint(p->getPosition()-(p->getRadius() - 0.5 * interaction->getOverlap())*interaction->getNormal());
        return interaction;
    }
    else
    {
        return nullptr;
    }
}

