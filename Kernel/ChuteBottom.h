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

#ifndef CHUTEBOTTOM_H
#define CHUTEBOTTOM_H

#include "Chute.h"

/*!
 * \class ChuteBottom
 * \brief  Used by Chute::createBottom to create an unordered particle layer.
 * 
 * \details It creates a thick layer of particles in a box of the same x and y
 * dimensions as the Chute, and places a thin slice of that layer 
 * as fixed particles at the base of the chute.
 */
class ChuteBottom : public Chute
{
    
public:

//constructors
/*! 
 * \brief This is the default constructor. All it does is set sensible defaults.
 */
    ChuteBottom();

    /*!    
     * \brief Copy-constructor for creates an HGRID problem from an existing MD problem
     */
    ChuteBottom(const DPMBase& other);

    /*!
     * \brief
     */
    ChuteBottom(const MercuryBase& other);

    /*!
     * \brief
     */
    ChuteBottom(const Mercury3D& other);

    /*!
     * \brief
     */
    ChuteBottom(const Chute& other);

//setters and getters
    /*!
     * \brief
     */
    Mdouble getThickness();

    /*!
     * \brief
     */
    void setThickness(Mdouble thickness);

    /*!
     * \brief
     */
    Mdouble getIsBottomPeriodic();

    /*!
     * \brief
     */
    void setIsBottomPeriodic(bool isBottomPeriodic);

//other public member functions

    /*!
     * \brief
     */
    void makeRoughBottom(ParticleHandler &ChuteParticleHandler);

    /*!
     * \brief
     */
    void setupInitialConditions();

    /*!
     * \brief
     */
    void actionsBeforeTimeStep();

private:
    /*!
     * \brief This is the actual constructor it is called do both constructors above.
     */
    void constructor();

    /*!
     * \brief
     */
    Mdouble thickness_;

    /*!
     * \brief
     */
    bool isBottomPeriodic_;
};
#endif
