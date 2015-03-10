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

#include "BaseBoundary.h"
#include "BoundaryHandler.h"

/*!
 * \details Default constructor
 */
BaseBoundary::BaseBoundary()
{
    handler_ = nullptr;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseBoundary::BaseBoundary() finished"<<std::endl;
#endif
}

/*!
 * \details Copy constructor
 */
///Note: shallow copy! Otherwise the HGrid causes a stack overflow.
BaseBoundary::BaseBoundary(const BaseBoundary &b)
        : BaseObject(b)
{
    handler_ = b.handler_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseBoundary::BaseBoundary(const BaseBoundary &b) finished"<<std::endl;
#endif
}

/*!
 * \details Destructor
 */
BaseBoundary::~BaseBoundary()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout << "BaseBoundary::~BaseBoundary() finished"<<std::endl;
#endif 
}

/*!
 * \details Reads the object's id_ from the given istream
 * \param[in,out] is    istream the id_ is read from
 */
void BaseBoundary::read(std::istream& is)
{
    BaseObject::read(is);
}

/*!
 * \details Adds the object's id_ to the given ostream
 * \param[in] os    ostream the id_ is added to
 */
void BaseBoundary::write(std::ostream& os) const
{
    BaseObject::write(os);
}

/*!
 * \details Used to create periodic copies of particles in classes which 
 * implement periodic boundary conditions
 * NB: virtual function
 * \param[in] P     particle of which periodic copies are to be created
 * \param[out] pH   the particle handler
 */
void BaseBoundary::createPeriodicParticles(BaseParticle *P UNUSED, ParticleHandler &pH UNUSED)
{
}

/*!
 * \details checks whether given particle passed the boundary, and if so, does something
 * special with it.
 * NB: virtual function
 * \param[in] P     Particle checked
 * \param[out] pH   the particle handler.
 * \return returns TRUE if given particle actually did pass the boundary
 */
bool BaseBoundary::checkBoundaryAfterParticleMoved(BaseParticle *P UNUSED, ParticleHandler &pH UNUSED)
{
    return false;
}

/*!
 * \details Is used to fill the insides of a 3D boundary with particles untill 
 * it is filled up. 
 * \param[in] md    the problem's DPMBase object
 */
void BaseBoundary::checkBoundaryBeforeTimeStep(DPMBase* md UNUSED)
{
}

/*!
 * \details Sets the pointer to the BounadaryHandler the boundary belongs to
 * \param[in] handler   pointer to the boundary handler
 */
void BaseBoundary::setHandler(BoundaryHandler* handler)
{
    handler_ = handler;
}

/*!
 * \details Returns the pointer to the BoundaryHandler the boundary belongs to
 * \return pointer to the handler
 */
BoundaryHandler* BaseBoundary::getHandler() const
{
    return handler_;
}
