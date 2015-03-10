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

#ifndef BaseBoundary_H
#define BaseBoundary_H

#include "BaseObject.h"
#include "Math/ExtendedMath.h"

class BoundaryHandler;
class ParticleHandler;
class BaseParticle;
class DPMBase;

/*!
 * \class BaseBoundary
 * \brief 
 * \details Inherits from BaseObject
 */
class BaseBoundary : public BaseObject
{
public:
    /*!
     * \brief default constructor.
     */
    BaseBoundary();

    /*!
     * \brief copy constructor
     */
    BaseBoundary(const BaseBoundary &b);
    
    /*!
     * \brief destructor
     */
    virtual ~BaseBoundary();

    /*!
     * \brief Used to create a copy of the object
     * NB: purely virtual function
     */
    virtual BaseBoundary* copy() const = 0;
    
    /*!
     * \brief Reads the object's id_ from given istream
     * NB: purely virtual function
     */
    void read(std::istream& is) = 0;
    
    /*!
     * \brief Adds object's id_ to given ostream
     * NB: purely virtual function
     */
    void write(std::ostream& os) const = 0;

    /*!
     * \brief Creates periodic copies of given particle in case of periodic boundaries
     */    
    virtual void createPeriodicParticles(BaseParticle *P UNUSED, ParticleHandler &pH UNUSED);

    /*!
     * \brief Checks if given particle passed the boundary 
     */
    virtual bool checkBoundaryAfterParticleMoved(BaseParticle *P UNUSED, ParticleHandler &pH UNUSED);

    /*!
     * \brief Fills a (3D) boundary with particles 
     */
    virtual void checkBoundaryBeforeTimeStep(DPMBase* md UNUSED);

    /*!
     * \brief Sets the boundary's BoundaryHandler
     */
    void setHandler(BoundaryHandler* handler);

    /*!
     * \brief Returns the boundary's BoundaryHandler
     */
    BoundaryHandler* getHandler() const;

private:

    /*!
     * \brief pointer to the boundary's BoundaryHandler
     */
    BoundaryHandler* handler_;
};
#endif
