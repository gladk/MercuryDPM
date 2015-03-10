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


#include "HertzianViscoelasticInteraction.h"
#include "Species/NormalForceSpecies/HertzianViscoelasticNormalSpecies.h"
#include "BaseInteractable.h"
#include "InteractionHandler.h"
#include <iomanip>
#include <fstream>
/*!
 * \param[in] P
 * \param[in] I
 * \param[in] timeStamp
 */
HertzianViscoelasticInteraction::HertzianViscoelasticInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
        : BaseInteraction(P, I, timeStamp)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"HertzianViscoelasticInteraction::HertzianViscoelasticInteraction() finished"<<std::endl;
#endif
}
/*!
 * \param[in] p
 */
HertzianViscoelasticInteraction::HertzianViscoelasticInteraction(const HertzianViscoelasticInteraction& p)
        : BaseInteraction(p)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"HertzianViscoelasticInteraction::HertzianViscoelasticInteraction(const HertzianViscoelasticInteraction& p) finished"<<std::endl;
#endif
}
/*!
 *
 */
HertzianViscoelasticInteraction::~HertzianViscoelasticInteraction()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout<<"HertzianViscoelasticInteraction::~HertzianViscoelasticInteraction() finished"<<std::endl;
#endif
}

/*!
 * \param[in,out] os
 */
void HertzianViscoelasticInteraction::write(std::ostream& os) const
        {
    BaseInteraction::write(os);
}
/*!
 * \param[in,out] is
 */
void HertzianViscoelasticInteraction::read(std::istream& is)
{
    BaseInteraction::read(is);
}
/*!
 * \return std::string
 */
std::string HertzianViscoelasticInteraction::getBaseName() const
{
    return "HertzianViscoelastic";
}
/*!
 *
 */
void HertzianViscoelasticInteraction::computeNormalForce()
{
    ///\todo TWnow
    // Compute the relative velocity vector of particle P w.r.t. I
    setRelativeVelocity(getP()->getVelocityAtContact(getContactPoint()) - getI()->getVelocityAtContact(getContactPoint()));
    // Compute the projection of vrel onto the normal (can be negative)
    setNormalRelativeVelocity(Vec3D::dot(getRelativeVelocity(), getNormal()));

    if (getOverlap() > 0) //if contact forces
    {
        const HertzianViscoelasticNormalSpecies* species = getSpecies();

        //R = 2.0 * PI->getRadius() * PJ->getRadius() / (PI->getRadius() + PJ->getRadius());
        //a = sqrt(R * deltan);
        //Mdouble kn = 4. / 3. * pSpecies->k * a;
        //fdotn += kn * deltan + pSpecies->dissipation_ * vdotn;
        Mdouble effectiveDiameter = 2.0*getEffectiveRadius();
        Mdouble stiffness = 4. / 3. * species->getElasticModulus() * std::sqrt(effectiveDiameter * getOverlap());
        Mdouble normalForce = stiffness * getOverlap() - species->getDissipation() * getNormalRelativeVelocity();

        setAbsoluteNormalForce(std::abs(normalForce)); //used for further corce calculations;
        setForce(getNormal() * normalForce);
        setTorque(Vec3D(0.0, 0.0, 0.0));
    }
    else
    {
        setAbsoluteNormalForce(0.0);
        setForce(Vec3D(0.0, 0.0, 0.0));
        setTorque(Vec3D(0.0, 0.0, 0.0));
    }
}
/*!
 * \return Mdouble
 */
Mdouble HertzianViscoelasticInteraction::getElasticEnergy() const
{
   if (getOverlap() > 0)
        return 0.5 * (getSpecies()->getElasticModulus() * mathsFunc::square(getOverlap()));
    else
        return 0.0;
    ///\todo TW This is not correct
}
/*!
 * \return const HertzianViscoelasticNormalSpecies*
 */
const HertzianViscoelasticNormalSpecies* HertzianViscoelasticInteraction::getSpecies() const
{
    return dynamic_cast<const HertzianViscoelasticNormalSpecies*>(getBaseSpecies());
}
