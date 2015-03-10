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

#include "Mercury3D.h"
#include "Particles/BaseParticle.h"

Mercury3D::Mercury3D()
{
    constructor();
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "Mercury3D::Mercury3D() finished"<<std::endl;
#endif
}

//The copy-constructor of DPMBase has to be called because the link from DPMBase to MercuryBase is virtual
Mercury3D::Mercury3D(const Mercury3D& other)
        : DPMBase(other), MercuryBase(other)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "Mercury3D::Mercury3D(Mercury3D& other) finished"<<std::endl;
#endif
}

//The copy-constructor of DPMBase has to be called because the link from DPMBase to MercuryBase is virtual
Mercury3D::Mercury3D(const DPMBase& other)
        : DPMBase(other), MercuryBase()
{
    constructor();
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "Mercury3D::Mercury3D(DPMBase& other) finished"<<std::endl;
#endif
}

void Mercury3D::constructor()
{
    setParticleDimensions(3);
    setSystemDimensions(3);
}

///Computes all collision between particles in the same bucket as cell (x,y,z,l), however the particles are in even in the same cell.
void Mercury3D::hGridFindContactsWithinTargetCell(int x, int y, int z, unsigned int l)
{
    HGrid* hgrid=getHGrid();
    unsigned int bucket = hgrid->computeHashBucketIndex(x, y, z, l);
    
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
            if ((p1->getHGridX() == p2->getHGridX()) && (p1->getHGridY() == p2->getHGridY()) && (p1->getHGridZ() == p2->getHGridZ()) && (p1->getHGridLevel() == p2->getHGridLevel()))
            {
                computeInternalForces(p1, p2);
            }
            p2 = p2->getHGridNextObject();
        }
        p1 = p1->getHGridNextObject();
    }
    hgrid->setBucketIsChecked(bucket);
}

///Computes all collisions between BaseParticle *obj and particles in cell (x,y,z,l)
void Mercury3D::hGridFindContactsWithTargetCell(int x, int y, int z, unsigned int l, BaseParticle *obj)
{
    
    //Check if the object is not in the same cell as being checked, CheckCell_current should handle these cases.
    if ((obj->getHGridX() == x) && (obj->getHGridY() == y) && (obj->getHGridZ() == z) && (obj->getHGridLevel() == l))
    {
        return;
    }

    HGrid* hgrid=getHGrid();

    // Calculate the bucket
    unsigned int bucket = hgrid->computeHashBucketIndex(x, y, z, l);

    // Loop through all objects in the bucket to find nearby objects
    BaseParticle *p = hgrid->getFirstBaseParticleInBucket(bucket);
    while (p != nullptr)
    {
        ///\bug{TW: This check is not necessary, I believe. This is the most-expensive function in most codes (the two checks in this function slows down granular jet by 15%) and the selftests are not affected. DK: I do think this is neccesary, for example: If two cells hash to the same bucket and a particle in one of these cells check for collisions with the other cell. Then due to the hashingcollision it also gets all particles in it's own cell and thus generating false collisions.}
        //Check if the BaseParticle *p really is in the target cell (i.e. no hashing error has occurred)
        if ((p->getHGridX() == x) && (p->getHGridY() == y) && (p->getHGridZ() == z) && (p->getHGridLevel() == l))
        {
            computeInternalForces(obj, p);
        }
        p = p->getHGridNextObject();
    }
}

///Computes all collision between BaseParticle *obj and all other particles in the grid
void Mercury3D::hGridFindOneSidedContacts(BaseParticle *obj)
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
                    int z = obj->getHGridZ();
                    
                    hGridFindContactsWithinTargetCell(x, y, z, level);
                    hGridFindContactsWithTargetCell(x + 1, y - 1, z, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y, z, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y + 1, z, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y - 1, z + 1, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y, z + 1, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y + 1, z + 1, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y - 1, z - 1, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y, z - 1, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y + 1, z - 1, level, obj);
                    hGridFindContactsWithTargetCell(x, y + 1, z, level, obj);
                    hGridFindContactsWithTargetCell(x, y, z - 1, level, obj);
                    hGridFindContactsWithTargetCell(x, y + 1, z - 1, level, obj);
                    hGridFindContactsWithTargetCell(x, y + 1, z + 1, level, obj);
                }
                else
                {
                    int xs, ys, zs, xe, ye, ze;
                    Mdouble inv_size = hgrid->getInvCellSize(level);
                    xs = static_cast<int>(std::floor((obj->getPosition().X - obj->getInteractionRadius()) * inv_size - 0.5));
                    xe = static_cast<int>(std::floor((obj->getPosition().X + obj->getInteractionRadius()) * inv_size + 0.5));
                    ys = static_cast<int>(std::floor((obj->getPosition().Y - obj->getInteractionRadius()) * inv_size - 0.5));
                    ye = static_cast<int>(std::floor((obj->getPosition().Y + obj->getInteractionRadius()) * inv_size + 0.5));
                    zs = static_cast<int>(std::floor((obj->getPosition().Z - obj->getInteractionRadius()) * inv_size - 0.5));
                    ze = static_cast<int>(std::floor((obj->getPosition().Z + obj->getInteractionRadius()) * inv_size + 0.5));
                    for (int x = xs; x <= xe; ++x)
                    {
                        for (int y = ys; y <= ye; ++y)
                        {
                            for (int z = zs; z <= ze; ++z)
                            {
                                hGridFindContactsWithTargetCell(x, y, z, level, obj);
                            }
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
                    int z = obj->getHGridZ();
                    
                    hGridFindContactsWithinTargetCell(x, y, z, level);
                    hGridFindContactsWithTargetCell(x + 1, y - 1, z, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y, z, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y + 1, z, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y - 1, z + 1, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y, z + 1, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y + 1, z + 1, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y - 1, z - 1, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y, z - 1, level, obj);
                    hGridFindContactsWithTargetCell(x + 1, y + 1, z - 1, level, obj);
                    hGridFindContactsWithTargetCell(x, y + 1, z, level, obj);
                    hGridFindContactsWithTargetCell(x, y, z - 1, level, obj);
                    hGridFindContactsWithTargetCell(x, y + 1, z - 1, level, obj);
                    hGridFindContactsWithTargetCell(x, y + 1, z + 1, level, obj);
                }
                else
                {
                    int xs, ys, zs, xe, ye, ze;
                    Mdouble inv_size = getHGrid()->getInvCellSize(level);
                    xs = static_cast<int>(std::floor((obj->getPosition().X - obj->getInteractionRadius()) * inv_size - 0.5));
                    xe = static_cast<int>(std::floor((obj->getPosition().X + obj->getInteractionRadius()) * inv_size + 0.5));
                    ys = static_cast<int>(std::floor((obj->getPosition().Y - obj->getInteractionRadius()) * inv_size - 0.5));
                    ye = static_cast<int>(std::floor((obj->getPosition().Y + obj->getInteractionRadius()) * inv_size + 0.5));
                    zs = static_cast<int>(std::floor((obj->getPosition().Z - obj->getInteractionRadius()) * inv_size - 0.5));
                    ze = static_cast<int>(std::floor((obj->getPosition().Z + obj->getInteractionRadius()) * inv_size + 0.5));
                    for (int x = xs; x <= xe; ++x)
                    {
                        for (int y = ys; y <= ye; ++y)
                        {
                            for (int z = zs; z <= ze; ++z)
                            {
                                hGridFindContactsWithTargetCell(x, y, z, level, obj);
                            }
                        }
                    }
                }
            }
            break;
        }
    }
}

///Updates the HGrid positions (x, y and z) of BaseParticle *obj
void Mercury3D::hGridUpdateParticle(BaseParticle *obj)
{
    HGrid* hGrid=getHGrid();
    if (hGrid)
    {
        unsigned int l = obj->getHGridLevel();
        Mdouble inv_size = hGrid->getInvCellSize(l);

        int x = static_cast<int>(std::floor(obj->getPosition().X * inv_size));
        int y = static_cast<int>(std::floor(obj->getPosition().Y * inv_size));
        int z = static_cast<int>(std::floor(obj->getPosition().Z * inv_size));

#ifdef CONTACT_LIST_HGRID
        if(obj->getHGridX()!=x||obj->getHGridY()!=y||obj->getHGridZ()!=z)
        {
            int bucket = hGrid->computeHashBucketIndex(x, y, z, l);

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
            obj->setHGridZ(z);
            InsertObjAgainstGrid(obj);
        }
#else
        unsigned int bucket = hGrid->computeHashBucketIndex(x, y, z, l);

        obj->setHGridNextObject(hGrid->getFirstBaseParticleInBucket(bucket));
        obj->setHGridPrevObject(nullptr);
        if (hGrid->getFirstBaseParticleInBucket(bucket))
        {
            hGrid->getFirstBaseParticleInBucket(bucket)->setHGridPrevObject(obj);
        }

        hGrid->setFirstBaseParticleInBucket(bucket, obj);

        obj->setHGridX(x);
        obj->setHGridY(y);
        obj->setHGridZ(z);
#endif
    }
}

///Removes BaseParticle *obj from the HGrid
void Mercury3D::hGridRemoveParticle(BaseParticle *obj)
{
    unsigned int bucket = getHGrid()->computeHashBucketIndex(obj->getHGridX(), obj->getHGridY(), obj->getHGridZ(), obj->getHGridLevel());
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

///Tests if there are any collisions between BaseParticle *obj and particles in cell (x, y, z, l)
bool Mercury3D::hGridHasContactsInTargetCell(int x, int y, int z, unsigned int l, const BaseParticle *obj) const
{
    // Loop through all objects in the bucket to find nearby objects
    unsigned int bucket = getHGrid()->computeHashBucketIndex(x, y, z, l);
    
    const BaseParticle *p = getHGrid()->getFirstBaseParticleInBucket(bucket);
    while (p != nullptr)
    {
        if ((p->getHGridX() == x) && (p->getHGridY() == y) && (p->getHGridZ() == z) && (p->getHGridLevel() == l))
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
bool Mercury3D::hGridHasParticleContacts(const BaseParticle *obj)
{
    if (getHGrid() == nullptr || getHGrid()->getNeedsRebuilding())
    {
        std::cout << "HGrid needs rebuilding for \"bool Mercury3D::hGridHasParticleContacts(BaseParticle *obj)\"" << std::endl;
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
        
        int xs, ys, zs, xe, ye, ze;
        inv_size = getHGrid()->getInvCellSize(level);
        xs = static_cast<int>(std::floor((obj->getPosition().X - obj->getInteractionRadius()) * inv_size - 0.5));
        xe = static_cast<int>(std::floor((obj->getPosition().X + obj->getInteractionRadius()) * inv_size + 0.5));
        ys = static_cast<int>(std::floor((obj->getPosition().Y - obj->getInteractionRadius()) * inv_size - 0.5));
        ye = static_cast<int>(std::floor((obj->getPosition().Y + obj->getInteractionRadius()) * inv_size + 0.5));
        zs = static_cast<int>(std::floor((obj->getPosition().Z - obj->getInteractionRadius()) * inv_size - 0.5));
        ze = static_cast<int>(std::floor((obj->getPosition().Z + obj->getInteractionRadius()) * inv_size + 0.5));
        
        //std::cout<<"Level="<<level<<" grid cells ["<<xs<<","<<xe<<"]x["<<ys<<","<<ye<<"]x["<<zs<<","<<ze<<"]"<<std::endl;
        for (int x = xs; x <= xe; ++x)
        {
            for (int y = ys; y <= ye; ++y)
            {
                for (int z = zs; z <= ze; ++z)
                {
                    if (hGridHasContactsInTargetCell(x, y, z, level, obj))
                    {
                        return true;
                    }
                }
            }
        }
    } //end for level
    
    return false;
}

#ifdef CONTACT_LIST_HGRID
void Mercury3D::InsertCell(int x, int y, int z, unsigned int l, BaseParticle *obj)
{   
    // Loop through all objects in the bucket to find nearby objects
    unsigned int bucket = getHGrid()->computeHashBucketIndex(x,y,z,l);
    BaseParticle *p = getHGrid()->getFirstBaseParticleInBucket(bucket);

    while (p!=nullptr)
    {   
        if ((p->getHGridX() == x) && (p->getHGridY() == y) && (p->getHGridZ() == z) && (p->getHGridLevel() == l) && (obj!=p))
        {   
            getPossibleContactList().add_PossibleContact(obj,p);
        }
        p = p->getHGridNextObject();
    }
}

void Mercury3D::InsertObjAgainstGrid(BaseParticle *obj)
{   
    Mdouble inv_size;
    int occupiedLevelsMask_ = getHGrid()->getOccupiedLevelsMask();

    inv_size=getHGrid()->getInvCellSize(obj->getHGridLevel());

    double ownXMin=(obj->getHGridX()-0.5)*getHGrid()->getCellSize(obj->getHGridLevel());
    double ownXMax=(obj->getHGridX()+1.5)*getHGrid()->getCellSize(obj->getHGridLevel());
    double ownYMin=(obj->getHGridY()-0.5)*getHGrid()->getCellSize(obj->getHGridLevel());
    double ownYMax=(obj->getHGridY()+1.5)*getHGrid()->getCellSize(obj->getHGridLevel());
    double ownZMin=(obj->getHGridZ()-0.5)*getHGrid()->getCellSize(obj->getHGridLevel());
    double ownZMax=(obj->getHGridZ()+1.5)*getHGrid()->getCellSize(obj->getHGridLevel());

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

        int xs, xe, ys, ye, zs, ze;
        xs=static_cast<int>(std::floor(ownXMin*inv_size-0.5));
        xe=static_cast<int>(std::floor(ownXMax*inv_size+0.5));
        ys=static_cast<int>(std::floor(ownYMin*inv_size-0.5));
        ye=static_cast<int>(std::floor(ownYMax*inv_size+0.5));
        zs=static_cast<int>(std::floor(ownZMin*inv_size-0.5));
        ze=static_cast<int>(std::floor(ownZMax*inv_size+0.5));

        for(int x=xs;x<=xe;++x)
        {   
            for(int y=ys;y<=ye;++y)
            {   
                for(int z=zs;z<=ze;++z)
                {   
                    InsertCell(x, y, z, level, obj);
                }
            }
        }
    } //end for level
}
#endif
