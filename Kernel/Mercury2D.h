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

#ifndef MERCURY2D_H
#define MERCURY2D_H

#include "MercuryBase.h"
#include <iostream>

/*!
 * \class Mercury2D
 * \brief This adds on the hierarchical grid code for 2D problems.
 */
class Mercury2D : public MercuryBase
{
public:
    /*!
     * \brief This is the default constructor. All it does is set sensible defaults.
     */
    Mercury2D();

    /*!
     * \brief Copy-constructor for creates an Mercury2D problem from an existing MD problem
     */
    Mercury2D(const DPMBase& other);

    /*!
     * \brief Copy-constructor
     */
    Mercury2D(const Mercury2D& other);

    /*!
     * \brief This is the actual constructor.
     */
    void constructor();

protected:
    /*!
     * \brief Finds contacts between the BaseParticle and the target cell
     */
    virtual void hGridFindContactsWithinTargetCell(int x, int y, unsigned int l);

    /*!
     * \brief Finds contacts between particles the target cell
     */
    virtual void hGridFindContactsWithTargetCell(int x, int y, unsigned int l, BaseParticle *obj);

    /*!
     * \brief Finds contacts with the BaseParticle; avoids multiple checks
     */
    void hGridFindOneSidedContacts(BaseParticle *obj);

    /*!
     * \brief Tests if the BaseParticle has contacts with other Particles in the target cell
     */
    bool hGridHasContactsInTargetCell(int x, int y, unsigned int l, const BaseParticle *obj) const;

    /*!
     * \brief Test if a BaseParticle has any contacts in the HGrid
     */
    bool hGridHasParticleContacts(const BaseParticle *obj);

    /*!
     * \brief Removes a BaseParticle to the HGrid
     */
    void hGridRemoveParticle(BaseParticle *obj);

    /*!
     * \brief Updates the cell (not the level) of a BaseParticle
     */
    void hGridUpdateParticle(BaseParticle *obj);
    
#ifdef CONTACT_LIST_HGRID	
    /*!
     * \brief 
     */
    void InsertCell(int x, int y, unsigned int l, BaseParticle *obj);

    /*!
     * \brief 
     */
    void InsertObjAgainstGrid(BaseParticle *obj);
#endif    
};
#endif
