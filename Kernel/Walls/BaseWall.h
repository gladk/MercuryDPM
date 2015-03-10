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

/*!
 * \brief Basic class for walls.
 * \details Class from which all walls inherit. Please note the getVelocity can 
 * for some walls be dependent on which point on the wall is meant.
 */
class BaseWall : public BaseInteractable
{
public:
    /*!
     * \brief Default constructor. It makes an empty BaseWall.
     */
    BaseWall();
    
    /*!
     * \brief Copy constructor.
     */
    BaseWall(const BaseWall& w);
    
    /*!
     * \brief Default destructor. 
     */   
    virtual ~BaseWall();
    
    /*!
     * \brief Pure virtual function that copies a BaseWall.
     * \return A pointer to the new BaseWall.
     */
    virtual BaseWall* copy() const = 0;
    
    /*!
     * \brief Function that reads a BaseWall from an input stream, usually a restart file.
     */
    void read(std::istream& is);
    
    /*!
     * \brief Function that writes a BaseWall to an output stream, usually a restart file.
     */
    void write(std::ostream& os) const;
    
    /*!
     * \brief A function that removes all data from this BaseWall, so sets handler_ to nullptr.
     * \deprecated Please don't use any clear() anymore, it will be gone soon.
     */
    MERCURY_DEPRECATED
    virtual void clear();
    
    /*!
     * \brief Pure virtual function that computes the distance of a BaseParticle to this wall and returns the normal of this wall if there is a collision.
     * \details Beware, the distance and normal are output parameters, not return values!
     * \param[in] P Reference to the BaseParticle we want to compute the distance to the BaseWall of.
     * \param[out] distance Distance of the BaseParticle to the BaseWall.
     * \param[out] normal_return The normal of the wall. Is only given if there is a collision.
     * \return A boolean which indicates if there is a collision between the BaseParticle and the wall.
     */
    virtual bool getDistanceAndNormal(const BaseParticle& P, Mdouble& distance, Vec3D& normal_return) const = 0;

    /*!
     * \brief A function which sets the WallHandler for this BaseWall.
     */
    void setHandler(WallHandler* handler);

    /*!
     * \brief A function which returns the WallHandler that handles this BaseWall.
     */
    WallHandler* getHandler() const;

    /*!
     * \deprecated TW: this function should be taken out and replaced by setSpecies
     * \brief Define the species of this wall using the index of the species in the SpeciesHandler in this DPMBase.
     */
    void setIndSpecies(unsigned int indSpecies);

    /*!
     * \brief Define the species of this wall.
     * \todo TW: this function should also check if the particle is the correct particle for the species type.
     */
    void setSpecies(const ParticleSpecies* species);

private:

    /*!
     * A pointer to the WallHandler that handles this BaseWall.
     */
    WallHandler* handler_; ///
};
#endif
