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
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "Mercury2D::Mercury2D() finished"<<std::endl;
#endif
}

//The copy-constructor of DPMBase has to be called because the link from DPMBase to MercuryBase is virtual
Mercury2D::Mercury2D(const Mercury2D& other)
        : DPMBase(other), MercuryBase(other)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "Mercury2D::Mercury2D(Mercury2D& other) finished" << std::endl;
#endif
}

//The copy-constructor of DPMBase has to be called because the link from DPMBase to MercuryBase is virtual
Mercury2D::Mercury2D(const DPMBase& other)
        : DPMBase(other), MercuryBase()
{
    constructor();
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "Mercury2D::Mercury2D(DPMBase& other) finished" << std::endl;
#endif
}

void Mercury2D::constructor()
{
    setParticleDimensions(2);
    setSystemDimensions(2);
}

///Computes all collision between particles in the same bucket as cell (x,y,l), however the particles are in even in the same cell.
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

///Computes all collisions between BaseParticle *obj and particles in cell (x,y,l)
void Mercury2D::hGridFindContactsWithTargetCell(int x, int y, unsigned int l, BaseParticle *obj)
{
    //Check if the object is not in the same cell as being checked, CheckCell_current should handle these cases.
    if ((obj->getHGridX() == x) && (obj->getHGridY() == y) && (obj->getHGridLevel() == l))
    {
        return;
    }

    HGrid* hgrid=getHGrid();

    // Calculate the bucket
    unsigned int bucket = hgrid->computeHashBucketIndex(x, y, l);

    // Loop through all objects in the bucket to find nearby objects
    BaseParticle *p = hgrid->getFirstBaseParticleInBucket(bucket);
    while (p != nullptr)
    {
        ///\bug{TW: This check is not necessary, I believe. This is the most-expensive function in most codes (the two checks in this function slows down granular jet by 15%) and the selftests are not affected. DK: I do think this is neccesary, for example: If two cells hash to the same bucket and a particle in one of these cells check for collisions with the other cell. Then due to the hashingcollision it also gets all particles in it's own cell and thus generating false collisions.}
        //Check if the BaseParticle *p really is in the target cell (i.e. no hashing error has occurred)
        if ((p->getHGridX() == x) && (p->getHGridY() == y) && (p->getHGridLevel() == l))
        {
            computeInternalForces(obj, p);
        }
        p = p->getHGridNextObject();
    }
}

///Computes all collision between BaseParticle *obj and all other particles in the grid
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

///Updates the HGrid positions (x and y) of BaseParticle *obj
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
        if(obj->getHGridX()!=x||obj->getHGridY()!=y)
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

///Removes BaseParticle *obj from the HGrid
void Mercury2D::hGridRemoveParticle(BaseParticle *obj)
{
    HGrid* hGrid=getHGrid();
    if (hGrid)
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

///Tests if there are any collisions between BaseParticle *obj and particles in cell (x, y, l)
bool Mercury2D::hGridHasContactsInTargetCell(int x, int y, unsigned int l, const BaseParticle *obj) const
        {
    // Loop through all objects in the bucket to find nearby objects
    unsigned int bucket = getHGrid()->computeHashBucketIndex(x, y, l);
    
    const BaseParticle *p = getHGrid()->getFirstBaseParticleInBucket(bucket);
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

///Tests if there are any collisions between BaseParticle *obj and all other particles in the HGrid
bool Mercury2D::hGridHasParticleContacts(const BaseParticle *obj)
{
    if (getHGrid() == nullptr || getHGrid()->getNeedsRebuilding())
    {
        std::cout << "HGrid needs rebuilding for \"bool Mercury2D::hGridHasParticleContacts(BaseParticle *obj)\"" << std::endl;
        hGridRebuild();
    }
    
    Mdouble inv_size;
    int occupiedLevelsMask = getHGrid()->getOccupiedLevelsMask();
    
    for (unsigned int level = 0; level < getHGrid()->getNumberOfLevels(); occupiedLevelsMask >>= 1, level++)
    {
        // If no objects in rest of grid, stop now
        if (occupiedLevelsMask == 0)
        {
            //std::cout<<"Level "<<level<<" and higher levels are empty"<<std::endl;
            break;
        }
        
        // If no objects at this level, go on to the next level
        if ((occupiedLevelsMask & 1) == 0)
        {
            //std::cout<<"Level "<<level<<" is empty"<<std::endl;
            continue;
        }
        
        int xs, ys, xe, ye;
        inv_size = getHGrid()->getInvCellSize(level);
        xs = static_cast<int>(std::floor((obj->getPosition().X - obj->getInteractionRadius()) * inv_size - 0.5));
        xe = static_cast<int>(std::floor((obj->getPosition().X + obj->getInteractionRadius()) * inv_size + 0.5));
        ys = static_cast<int>(std::floor((obj->getPosition().Y - obj->getInteractionRadius()) * inv_size - 0.5));
        ye = static_cast<int>(std::floor((obj->getPosition().Y + obj->getInteractionRadius()) * inv_size + 0.5));
        
        //std::cout<<"Level="<<level<<" grid cells ["<<xs<<","<<xe<<"]x["<<ys<<","<<ye<<"]"<<std::endl;
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
void Mercury2D::InsertCell(int x, int y, unsigned int l, BaseParticle *obj)
{   
    // Loop through all objects in the bucket to find nearby objects
    unsigned int bucket = getHGrid()->computeHashBucketIndex(x,y,l);
    BaseParticle *p = getHGrid()->getFirstBaseParticleInBucket(bucket);

    while (p!=nullptr)
    {   
        if ((p->getHGridX() == x) && (p->getHGridY() == y) && (p->getHGridLevel() == l) && (obj!=p))
        {   
            getPossibleContactList().add_PossibleContact(obj,p);
        }
        p = p->getHGridNextObject();
    }
}

void Mercury2D::InsertObjAgainstGrid(BaseParticle *obj)
{   
    Mdouble inv_size;
    int occupiedLevelsMask_ = getHGrid()->getOccupiedLevelsMask();

    inv_size=getHGrid()->getInvCellSize(obj->getHGridLevel());

    double ownXMin=(obj->getHGridX()-0.5)*getHGrid()->getCellSize(obj->getHGridLevel());
    double ownXMax=(obj->getHGridX()+1.5)*getHGrid()->getCellSize(obj->getHGridLevel());
    double ownYMin=(obj->getHGridY()-0.5)*getHGrid()->getCellSize(obj->getHGridLevel());
    double ownYMax=(obj->getHGridY()+1.5)*getHGrid()->getCellSize(obj->getHGridLevel());

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

        int xs, xe, ys, ye;
        xs=static_cast<int>(std::floor(ownXMin*inv_size-0.5));
        xe=static_cast<int>(std::floor(ownXMax*inv_size+0.5));
        ys=static_cast<int>(std::floor(ownYMin*inv_size-0.5));
        ye=static_cast<int>(std::floor(ownYMax*inv_size+0.5));

        for(int x=xs;x<=xe;++x)
        {   
            for(int y=ys;y<=ye;++y)
            {   
                InsertCell(x, y, level, obj);
            }
        }
    } //end for level
}
#endif
