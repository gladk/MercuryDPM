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
class ChuteBottom : public Chute {
public:

    //constructors
    /*! 
     * \brief This is the default constructor. All it does is set sensible defaults.
     */
    ChuteBottom();

    /*!    
     * \brief Copy constructor, converts an existing DPMBase object into a ChuteBottom object
     */
    ChuteBottom(const DPMBase& other);

    /*!
     * \brief Copy constructor, converts an existing MercuryBase object into a ChuteBottom object
     */
    ChuteBottom(const MercuryBase& other);

    /*!
     * \brief Copy constructor, converts an existing Mercury3D object into a ChuteBottom object
     */
    ChuteBottom(const Mercury3D& other);

    /*!
     * \brief Copy constructor, converts an existing Chute object into a ChuteBottom object
     */
    ChuteBottom(const Chute& other);
    
    /*!
     * \brief Default copy constructor
     */
    ChuteBottom(const ChuteBottom& other);

    //setters and getters
    /*!
     * \brief Returns the thickness of the multilayer rough bottom
     */
    Mdouble getThickness();

    /*!
     * \brief Sets the thickness of the multilayer rough bottom
     */
    void setThickness(Mdouble thickness);

    /*!
     * \brief Returns TRUE if the bottom is periodic in Y
     */
    Mdouble getIsBottomPeriodic();

    /*!
     * \brief Sets whether the bottom should be periodic in Y
     */
    void setIsBottomPeriodic(bool isBottomPeriodic);

    //other public member functions

    /*!
     * \brief Makes a multilayered rough bottom with thickness thickness_
     */
    void makeRoughBottom(Chute &chute);

    /*!
     * \brief Sets up initial conditions before running a chute simulation
     */
    void setupInitialConditions();

    /*!
     * \brief Performs all necessary actions before the start of a time step (none in this case)
     */
    void actionsBeforeTimeStep();

private:
    /*!
     * \brief This is the actual constructor METHOD; it is called by all constructors above 
     * (except the default copy constructor).
     */
    void constructor();

    /*!
     * \brief Thickness of the multilayer chute rough bottom. See also documentation of 
     * ChuteBottom::makeRoughBottom().
     */
    Mdouble thickness_;

    /*!
     * \brief TRUE if the bottom is periodic in Y.
     */
    bool isBottomPeriodic_;
};
#endif
