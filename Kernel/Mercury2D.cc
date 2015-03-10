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

#include "Mercury2D.h"
#include "Particles/BaseParticle.h"

Mercury2D::Mercury2D()
{
    constructor();
    logger(DEBUG, "Mercury2D::Mercury2D() constructor finished");
}

/*!
 * \param[in] other Mercury2D that must be copied.
 * \details Copy constructor. Note that the copy-constructor of DPMBase has to 
 * be called because the link from DPMBase to MercuryBase is virtual.
 */
Mercury2D::Mercury2D(const Mercury2D& other)
        : DPMBase(other), MercuryBase(other)
{
    logger(DEBUG, "Mercury2D::Mercury2D(Mercury2D& other) copy constructor finished");
}

/*!
 * \param[in] other DPMBase from which this Mercury2D must copy the properties.
 * \details Makes a Mercury2D class from a DPMBase class, so it adds the HGrid 
 * functionality to the class. Note that the copy-constructor of DPMBase has to 
 * be called because the link from DPMBase to MercuryBase is virtual.
 */
Mercury2D::Mercury2D(const DPMBase& other)
        : DPMBase(other), MercuryBase()
{
    constructor();
    logger(DEBUG, "Mercury2D::Mercury2D(DPMBase& other) finished");
}

void Mercury2D::constructor()
{
    setParticleDimensions(2);
    setSystemDimensions(2);
}

/*!
 * \param[in] x Coordinate of this cell in x direction.
 * \param[in] y Coordinate of this cell in y direction.
 * \param[in] l Level in the HGrid of this cell.
 * \details Computes all collision between particles in the same bucket as cell
 * (x,y,l), for all particles from the same cell.
 */
void Mercury2D::hGridFindContactsWithinTargetCell(int x, int y, unsigned int l)
{
    HGrid* hgrid=getHGrid();
    unsigned int bucket = hgrid->computeHashBucketIndex(x, y, l);
    
    //Check if this function is already applied to this bucket
    if (hgrid->getBucketIsChecked(bucket))
    {
        return;
    }
    
    BaseParticle* p1 = hgrid->getFirstBaseParticleInBucket(bucket);
    while (p1 != nullptr)
    {
        BaseParticle* p2 = p1->getHGridNextObject();
        while (p2 != nullptr)
        {
            ///\bug{TW: This check is not necessary, I believe. This is the most-expensive function in most codes (the two checks in this function slows down granular jet by 15%) and the selftests are not affected.}
            ///\bug{DK: I do think this is necessary, for example: If two cells hash to the same bucket and a particle in one of these cells check for collisions with the other cell. Then due to the hashing collision it also gets all particles in it's own cell and thus generating false collisions.}
            //Check if the BaseParticle* p1 and BaseParticle* p2 are really in the same cell (i.e. no hashing error has occurred)
            if ((p1->getHGridX() == p2->getHGridX()) && (p1->getHGridY() == p2->getHGridY()) && (p1->getHGridLevel() == p2->getHGridLevel()))
            {
                computeInternalForces(p1, p2);
            }
            p2 = p2->getHGridNextObject();
        }
        p1 = p1->getHGridNextObject();
    }
    hgrid->setBucketIsChecked(bucket);
}

/*!
 * \param[in] x Coordinate in x direction of the cell we want to find collisions for.
 * \param[in] y Coordinate in y direction of the cell we want to find collisions for.
 * \param[in] l The level in the HGrid of the cell we want to find collisions for.
 * \param[in] obj The BaseParticle we want to check for collisions in the cell (x,y,l).
 * \details Computes all collisions between given BaseParticle and particles in 
 * cell (x,y,l), where the given BaseParticle does currently not belong to that cell.
 * First this function makes sure this BaseParticle is not in the target cell,
 * then it computes the internal forces between the given BaseParticle and the
 * BaseParticle in the target cell.
 */
void Mercury2D::hGridFindContactsWithTargetCell(int x, int y, unsigned int l, BaseParticle* obj)
{
    //Check if the object is not in the same cell as being checked, CheckCell_current should handle these cases.
    if ((obj->getHGridX() == x) && (obj->getHGridY() == y) && (obj->getHGridLevel() == l))
    {
        return;
    }

    HGrid* hgrid = getHGrid();

    // Calculate the bucket
    unsigned int bucket = hgrid->computeHashBucketIndex(x, y, l);

    // Loop through all objects in the bucket to find nearby objects
    BaseParticle *p = hgrid->getFirstBaseParticleInBucket(bucket);
    while (p != nullptr)
    {
//        \bug{TW: This check is not necessary, I believe. This is the most-expensive function in most codes (the two checks in this function slows down granular jet by 15%) and the selftests are not affected. DK: I do think this is neccesary, for example: If two cells hash to the same bucket and a particle in one of these cells check for collisions with the other cell. Then due to the hashingcollision it also gets all particles in it's own cell and thus generating false collisions. TW: ok, so let's leave it in, issue closed.}
        //Check if the BaseParticle *p really is in the target cell (i.e. no hashing error has occurred)
        if ((p->getHGridX() == x) && (p->getHGridY() == y) && (p->getHGridLevel() == l))
        {
            computeInternalForces(obj, p);
        }
        p = p->getHGridNextObject();
    }
}

/*!
 * \param[in] obj The BaseParticle we want to check for collisions in the HGrid.
 * \details Computes all collision between given BaseParticle and all other 
 * particles in the grid (of all levels).
 */
void Mercury2D::hGridFindOneSidedContacts(BaseParticle *obj)
{
    HGrid* hgrid=getHGrid();
    unsigned int startLevel = obj->getHGridLevel();

    switch (getHGridMethod())
    {
        case BOTTOMUP:
            {
            int occupiedLevelsMask = hgrid->getOccupiedLevelsMask() >> obj->getHGridLevel();
            for (unsigned int level = startLevel; level < hgrid->getNumberOfLevels(); occupiedLevelsMask >>= 1, level++)
            {
                // If no objects in rest of grid, stop now
                if (occupiedLevelsMask == 0)
                {
                    break;
                }
                
                // If no objects at this level, go on to the next level
                if ((occupiedLevelsMask & 1) == 0)
                {
                    continue;
                }
                
                if (level == startLevel)
                {
                    int x = obj->getHGridX();
                    int y = obj->getHGridY();
                    
                    hGridFindContactsWithinTargetCell(x, y, level);
                    hGridFindContactsWithTargetCell(x, y + 1, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y - 1, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y + 1, level, obj);
                }
                else
                {
                    int xs, ys, xe, ye;
                    Mdouble inv_size = hgrid->getInvCellSize(level);
                    xs = static_cast<int>(std::floor((obj->getPosition().X - obj->getInteractionRadius()) * inv_size - 0.5));
                    xe = static_cast<int>(std::floor((obj->getPosition().X + obj->getInteractionRadius()) * inv_size + 0.5));
                    ys = static_cast<int>(std::floor((obj->getPosition().Y - obj->getInteractionRadius()) * inv_size - 0.5));
                    ye = static_cast<int>(std::floor((obj->getPosition().Y + obj->getInteractionRadius()) * inv_size + 0.5));
                    for (int x = xs; x <= xe; ++x)
                    {
                        for (int y = ys; y <= ye; ++y)
                        {
                            hGridFindContactsWithTargetCell(x, y, level, obj);
                        }
                    }
                }
            }
            break;
        }
        case TOPDOWN:
            {
            int occupiedLevelsMask = hgrid->getOccupiedLevelsMask();
            for (unsigned int level = 0; level <= startLevel; occupiedLevelsMask >>= 1, level++)
            {
                // If no objects in rest of grid, stop now
                if (occupiedLevelsMask == 0)
                {
                    break;
                }
                
                // If no objects at this level, go on to the next level
                if ((occupiedLevelsMask & 1) == 0)
                {
                    continue;
                }
                
                if (level == startLevel)
                {
                    int x = obj->getHGridX();
                    int y = obj->getHGridY();

                    hGridFindContactsWithinTargetCell(x, y, level);
                    hGridFindContactsWithTargetCell(x, y + 1, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y - 1, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y + 1, level, obj);
                }
                else
                {
                    int xs, ys, xe, ye;
                    Mdouble inv_size = hgrid->getInvCellSize(level);
                    xs = static_cast<int>(std::floor((obj->getPosition().X - obj->getInteractionRadius()) * inv_size - 0.5));
                    xe = static_cast<int>(std::floor((obj->getPosition().X + obj->getInteractionRadius()) * inv_size + 0.5));
                    ys = static_cast<int>(std::floor((obj->getPosition().Y - obj->getInteractionRadius()) * inv_size - 0.5));
                    ye = static_cast<int>(std::floor((obj->getPosition().Y + obj->getInteractionRadius()) * inv_size + 0.5));
                    for (int x = xs; x <= xe; ++x)
                    {
                        for (int y = ys; y <= ye; ++y)
                        {
                            hGridFindContactsWithTargetCell(x, y, level, obj);
                        }
                    }
                }
            }
            break;
        }
    }
}

/*!
 * \param[in] obj The BaseParticle we want to update the position in the grid for.
 * \details Updates the HGrid positions (x and y) of the given BaseParticle.
 */
void Mercury2D::hGridUpdateParticle(BaseParticle *obj)
{
    HGrid* hGrid=getHGrid();
    if (hGrid)
    {
        unsigned int l = obj->getHGridLevel();
        Mdouble inv_size = hGrid->getInvCellSize(l);

        int x = static_cast<int>(std::floor(obj->getPosition().X * inv_size));
        int y = static_cast<int>(std::floor(obj->getPosition().Y * inv_size));

#ifdef CONTACT_LIST_HGRID
        if(obj->getHGridX() != x || obj->getHGridY() != y)
        {   
            int bucket = hGrid->computeHashBucketIndex(x, y, l);

            //First the object has to be removed
            hGridRemoveParticle(obj);

            //Also remove all contact associated with it
            getPossibleContactList().remove_ParticlePosibleContacts(obj);

            //And now reinserted
            obj->setHGridNextObject(hGrid->getFirstBaseParticleInBucket(bucket));
            obj->setHGridPrevObject(nullptr);
            if(hGrid->getFirstBaseParticleInBucket(bucket))
            {
                hGrid->getFirstBaseParticleInBucket(bucket)->setHGridPrevObject(obj);
            }
            hGrid->setFirstBaseParticleInBucket(bucket,obj);

            obj->setHGridX(x);
            obj->setHGridY(y);
            InsertObjAgainstGrid(obj);
        }
#else
        unsigned int bucket = hGrid->computeHashBucketIndex(x, y, l);

        obj->setHGridNextObject(hGrid->getFirstBaseParticleInBucket(bucket));
        obj->setHGridPrevObject(nullptr);
        if (hGrid->getFirstBaseParticleInBucket(bucket))
        {
            hGrid->getFirstBaseParticleInBucket(bucket)->setHGridPrevObject(obj);
        }

        hGrid->setFirstBaseParticleInBucket(bucket, obj);

        obj->setHGridX(x);
        obj->setHGridY(y);
#endif
    }
}

/*!
 * \param[in] obj   The BaseParticle that has to be removed from the HGrid.
 * \details Removes BaseParticle from the HGrid. First check which bucket the 
 * BaseParticle is in, then set the pointers correctly again.
 */
void Mercury2D::hGridRemoveParticle(BaseParticle* obj)
{
    HGrid* hGrid = getHGrid();
    if (hGrid != nullptr)
    {
	    unsigned int bucket = getHGrid()->computeHashBucketIndex(obj->getHGridX(), obj->getHGridY(), obj->getHGridLevel());
		if (obj->getHGridPrevObject())
		{
			obj->getHGridPrevObject()->setHGridNextObject(obj->getHGridNextObject());
		}
		else
		{
			if (getHGrid()->getFirstBaseParticleInBucket(bucket) == obj)
			{
				getHGrid()->setFirstBaseParticleInBucket(bucket, obj->getHGridNextObject());
			}
		}
		
		if (obj->getHGridNextObject())
		{
			obj->getHGridNextObject()->setHGridPrevObject(obj->getHGridPrevObject());
		}
	}
}

/*!
 * \param[in] x     The coordinate of the target cell in x direction.
 * \param[in] y     The coordinate of the target cell in y direction.
 * \param[in] l     The level in the HGrid of the target cell.
 * \param[in] obj   A pointer to the BaseParticle for which we want to find contacts.
 * \return          A boolean which indicates whether or not there are contacts
 *                  between given BaseParticle and any other BaseParticle in the
 *                  target cell.
 * \details Tests if there are any collisions between given BaseParticle and 
 * particles in cell (x, y, l). Do this by going through all BaseParticle in the
 * bucket, and return true if there is a contact between the given BaseParticle 
 * and the BaseParticle we're checking for. If there is no contact, go on to the 
 * next BaseParticle until either a contact has been found or all BaseParticle
 * have been tested. If there were no contacts, return false.
 */
///
bool Mercury2D::hGridHasContactsInTargetCell(int x, int y, unsigned int l, const BaseParticle *obj) const
{
    // Loop through all objects in the bucket to find nearby objects
    unsigned int bucket = getHGrid()->computeHashBucketIndex(x, y, l);
    
    const BaseParticle* p = getHGrid()->getFirstBaseParticleInBucket(bucket);
    while (p != nullptr)
    {
        if ((p->getHGridX() == x) && (p->getHGridY() == y) && (p->getHGridLevel() == l))
        {
            if (areInContact(obj, p))
            {
                return true;
            }
        }
        p = p->getHGridNextObject();
    }
    return false;
}

/*!
 * \param[in] obj   A pointer to the BaseParticle which we want to check for contacts.
 * \return          A boolean which indicates whether or not there are contacts between this BaseParticle.
 * \details Tests if there are any collisions between given BaseParticle and all
 * other particles in the HGrid. It does this by first rebuilding the HGrid if
 * necessary, and then for all levels first computing what the area for checking
 * needs to be, and then check for interactions in the appropriate grid cells.
 */
bool Mercury2D::hGridHasParticleContacts(const BaseParticle* obj)
{
    if (getHGrid() == nullptr || getHGrid()->getNeedsRebuilding())
    {
        logger(INFO, "HGrid needs rebuilding for \"bool Mercury2D::hGridHasParticleContacts(BaseParticle *obj)\"");
        hGridRebuild();
    }
    
    Mdouble inv_size;
    int occupiedLevelsMask = getHGrid()->getOccupiedLevelsMask();
    
    for (unsigned int level = 0; level < getHGrid()->getNumberOfLevels(); occupiedLevelsMask >>= 1, level++)
    {
        // If no objects in rest of grid, stop now
        if (occupiedLevelsMask == 0)
        {
            logger(VERBOSE, "Level % and higher levels are empty.", level);
            break;
        }
        
        // If no objects at this level, go on to the next level
        if ((occupiedLevelsMask & 1) == 0)
        {
            logger(VERBOSE, "Level % is empty", level);
            continue;
        }
        
        int xs, ys, xe, ye;
        inv_size = getHGrid()->getInvCellSize(level);
        xs = static_cast<int>(std::floor((obj->getPosition().X - obj->getInteractionRadius()) * inv_size - 0.5));
        xe = static_cast<int>(std::floor((obj->getPosition().X + obj->getInteractionRadius()) * inv_size + 0.5));
        ys = static_cast<int>(std::floor((obj->getPosition().Y - obj->getInteractionRadius()) * inv_size - 0.5));
        ye = static_cast<int>(std::floor((obj->getPosition().Y + obj->getInteractionRadius()) * inv_size + 0.5));
        
        logger(VERBOSE, "Level % grid cells [%,%] x [%,%]", level, xs, xe, ys, ye);
        for (int x = xs; x <= xe; ++x)
        {
            for (int y = ys; y <= ye; ++y)
            {
                if (hGridHasContactsInTargetCell(x, y, level, obj))
                {
                    return true;
                }
            }
        }
    } //end for level
    
    return false;
}

#ifdef CONTACT_LIST_HGRID
/*!
 * \param[in] x     The coordinate in x direction of the cell we want to know possible contacts for.
 * \param[in] y     The coordinate in y direction of the cell we want to know possible contacts for.
 * \param[in] l     The level in the HGrid of the cell we want to know possible contacts for.
 * \param[in] obj   A pointer to the BaseParticle for which we want to add to the list of possible contacts.
 */
void Mercury2D::InsertCell(int x, int y, unsigned int l, BaseParticle* obj)
{   
    // Loop through all objects in the bucket to find nearby objects
    unsigned int bucket = getHGrid()->computeHashBucketIndex(x,y,l);
    BaseParticle *p = getHGrid()->getFirstBaseParticleInBucket(bucket);

    while (p!=nullptr)
    {   
        if ((p->getHGridX() == x) && (p->getHGridY() == y) && (p->getHGridLevel() == l) && (obj != p))
        {   
            getPossibleContactList().add_PossibleContact(obj, p);
        }
        p = p->getHGridNextObject();
    }
}

/*!
 * \param[in] obj A pointer to the BaseParticle we want to find possible interactions with.
 * \details Add the object to possible interactions for all levels. Do this by 
 * first finding the cell of the given BaseParticle. Then go through all levels 
 * and find all possible interactions, which are then added to the list.
 */
void Mercury2D::InsertObjAgainstGrid(BaseParticle* obj)
{   
    Mdouble inv_size;
    int occupiedLevelsMask_ = getHGrid()->getOccupiedLevelsMask();

    inv_size=getHGrid()->getInvCellSize(obj->getHGridLevel());

    double ownXMin = (obj->getHGridX()-0.5) * getHGrid()->getCellSize(obj->getHGridLevel());
    double ownXMax = (obj->getHGridX() + 1.5) * getHGrid()->getCellSize(obj->getHGridLevel());
    double ownYMin = (obj->getHGridY() - 0.5) * getHGrid()->getCellSize(obj->getHGridLevel());
    double ownYMax = (obj->getHGridY() + 1.5) * getHGrid()->getCellSize(obj->getHGridLevel());

    for (int level = 0; level < getHGrid()->getNumberOfLevels(); occupiedLevelsMask_ >>= 1, level++)
    {   
        // If no objects in rest of grid, stop now
        if (occupiedLevelsMask_ == 0)
        {   
            break;
        }

        // If no objects at this level, go on to the next level
        if ((occupiedLevelsMask_ & 1) == 0)
        {   
            continue;
        }

        // Treat level as a third dimension coordinate
        inv_size = getHGrid()->getInvCellSize(level);

        //Insert this particle in the list of possible interactions for this level
        //in the appropriate cells.
        int xs, xe, ys, ye;
        xs = static_cast<int>(std::floor(ownXMin * inv_size - 0.5));
        xe = static_cast<int>(std::floor(ownXMax * inv_size + 0.5));
        ys = static_cast<int>(std::floor(ownYMin * inv_size - 0.5));
        ye = static_cast<int>(std::floor(ownYMax * inv_size + 0.5));

        for(int x = xs; x <= xe; ++x)
        {   
            for(int y = ys; y <= ye; ++y)
            {   
                InsertCell(x, y, level, obj);
            }
        }
    } //end for level
}
#endif
