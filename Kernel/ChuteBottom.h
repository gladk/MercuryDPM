// This file is part of MercuryDPM.
// 
// MercuryDPM is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// MercuryDPM is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with MercuryDPM.  If not, see <http://www.gnu.org/licenses/>.
// 
// Copyright 2013 The Mercury Developers Team
// For the list of developers, see <http://www.MercuryDPM.org/Team>

#ifndef CHUTEBOTTOM_H
#define CHUTEBOTTOM_H
#include "Chute.h"
#include "Boundaries/PeriodicBoundary.h"
#include "Walls/InfiniteWall.h"


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
