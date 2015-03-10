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

#ifndef BASEWALL_H
#define BASEWALL_H

#include "BaseInteractable.h"

class WallHandler;
class BaseParticle;

//Note the getVelocity can for some walls be dependent on which point on the wall is meant.
class BaseWall : public BaseInteractable
{
public:
    /*!
     * \brief
     */
    BaseWall();
    /*!
     * \brief
     */
    BaseWall(const BaseWall &p);
    /*!
     * \brief
     */
    virtual ~BaseWall();
    /*!
     * \brief
     */
    virtual BaseWall* copy() const = 0;
    /*!
     * \brief
     */
    void read(std::istream& is) = 0;
    /*!
     * \brief
     */
    void write(std::ostream& os) const = 0;
    /*!
     * \brief
     */
    virtual void clear();
    /*!
     * \brief
     */
    virtual bool getDistanceAndNormal(const BaseParticle& P, Mdouble& distance, Vec3D& normal_return) const = 0;

    /*!
     * \brief
     */
    void setHandler(WallHandler* handler);

    /*!
     * \brief
     */
    WallHandler* getHandler() const;

    ///\todo TW: this function should be taken out and replaced by setSpecies
    void setIndSpecies(unsigned int indSpecies);

    /*!
     * In addition to the functionality of BaseInteractable::setSpecies, this function sets the pointer to the
     * wallHandler, which is needed to retrieve species information.
     */
    ///\todo TW: this function should also check if the particle is the correct particle for the species type
    void setSpecies(const ParticleSpecies* species);

private:

    WallHandler* handler_; ///
};
#endif
