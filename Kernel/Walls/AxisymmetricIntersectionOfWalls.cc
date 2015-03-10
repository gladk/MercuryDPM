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

#include "AxisymmetricIntersectionOfWalls.h"
#include "Particles/BaseParticle.h"

AxisymmetricIntersectionOfWalls::AxisymmetricIntersectionOfWalls()
{
    logger(DEBUG, "AxisymmetricIntersectionOfWalls() finished");
}

/*!
 * \param[in] other The AxisymmetricIntersectionOfWalls that must be copied.
 */
AxisymmetricIntersectionOfWalls::AxisymmetricIntersectionOfWalls(const AxisymmetricIntersectionOfWalls &p)
        : IntersectionOfWalls(p)
{
    logger(DEBUG, "AxisymmetricIntersectionOfWalls(const AxisymmetricIntersectionOfWalls &p) finished");
}

AxisymmetricIntersectionOfWalls::~AxisymmetricIntersectionOfWalls()
{
    logger(DEBUG, "~AxisymmetricIntersectionOfWalls() finished.");
}

/*!
 * \param[in] other The AxisymmetricIntersectionOfWalls that must be copied.
 */
AxisymmetricIntersectionOfWalls& AxisymmetricIntersectionOfWalls::operator=(const AxisymmetricIntersectionOfWalls& other)
{
    if (this == &other)
    {
        return *this;
    }
    else
    {
        return *(other.copy());
    }
}

/*!
 * \return pointer to a IntersectionOfWalls object allocated using new.
 */
AxisymmetricIntersectionOfWalls* AxisymmetricIntersectionOfWalls::copy() const
{
    return new AxisymmetricIntersectionOfWalls(*this);
}

/*!
 * \details First, the particle is translated by the vector position_, then the 
 * distance normal and tangential to the orientation is computed. This normal 
 * and tangential direction is interpreted as the x and z coordinate. With the 
 * particle shifted into the XZ plane, the distance and normal is computed, as 
 * if the AxisymmetricIntersectionOfWalls would be a simple IntersectionOfWalls.
 * Finally, the object and the normal is rotated back to the original position.
 * 
 * See also AxisymmetricIntersectionOfWalls for details.
 */
bool AxisymmetricIntersectionOfWalls::getDistanceAndNormal(const BaseParticle& p, Mdouble& distance, Vec3D& normalReturn) const
{
    //transform to axisymmetric coordinates
    //move the coordinate system to the axis origin, so pOrigin=(xhat,yhat,zhat)
    Vec3D pOrigin = p.getPosition() -getPosition(); 
    Mdouble normal = Vec3D::dot(pOrigin, getOrientation());
    //tangential is the projection into the (xhat,yhat) plane
    Vec3D tangentialUnitVector = pOrigin - normal * getOrientation();
    Mdouble tangential = tangentialUnitVector.getLength();
    if (tangential!=0.0)
        tangentialUnitVector /= tangential;
    else //in this case the tangential vector is irrelevant
        logger(WARN, "Warning: Particle % is exactly on the symmetry axis of wall %", p.getIndex(), getIndex());
    Vec3D transformedPosition = Vec3D(tangential, 0.0, normal); //now P=(r,phi,zhat) is cylindrical
    Vec3D transformedNormal;
    //determine wall distance, normal and contact in axissymmetric coordinates
    //and transform from axisymmetric coordinates
    if (!IntersectionOfWalls::getDistanceAndNormal(transformedPosition, p.getWallInteractionRadius(), distance, transformedNormal))
    {
        //if not in contact
        return false;
    }
    else
    {
        //if in contact
        normalReturn = transformedNormal.Z * getOrientation() + transformedNormal.X * tangentialUnitVector;
        return true;
    }
}

/*!
 * \param[in] is The input stream from which the AxisymmetricIntersectionOfWalls
 * is read, usually a restart file.
 */
void AxisymmetricIntersectionOfWalls::read(std::istream& is)
{
    IntersectionOfWalls::read(is);
}

/*!
 * \param[in] os The output stream where the AxisymmetricIntersectionOfWalls must be written
 *  to, usually a restart file.
 */
void AxisymmetricIntersectionOfWalls::write(std::ostream& os) const
{
    IntersectionOfWalls::write(os);
}

/*!
 * \return The string "AxisymmetricIntersectionOfWalls".
 */
std::string AxisymmetricIntersectionOfWalls::getName() const
{
    return "AxisymmetricIntersectionOfWalls";
}


/*!
 * \param[in] p Pointer to the BaseParticle which we want to check the interaction for.
 * \param[in] timeStamp The time at which we want to look at the interaction.
 * \param[in] interactionHandler A pointer to the InteractionHandler in which 
 *            the interaction can be found.
 * \return A pointer to the Interaction between this AxisymmetricIntersectionOfWalls
 * and the BaseParticle at the timeStamp.
 */
BaseInteraction* AxisymmetricIntersectionOfWalls::getInteractionWith(BaseParticle* p, Mdouble timeStamp, InteractionHandler* interactionHandler)
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
        c->setContactPoint(p->getPosition()-(p->getRadius()- 0.5 * c->getOverlap())*c->getNormal());
        return c;
    }
    else
    {
        return 0;
    }
}
