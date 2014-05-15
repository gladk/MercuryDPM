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

#include "HGRID_2D.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///Checks for collision in the particles own cell
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HGRID_2D::CheckCell_current(int x, int y, int l, HGrid *grid)
{
	int bucket = grid->ComputeHashBucketIndex(x,y,l);
	if (grid->bucketIsChecked[bucket]) { return; }
	BaseParticle* p1 = grid->objectBucket[bucket];

	while (p1!=NULL)
	{
		BaseParticle* p2 = p1->get_HGRID_NextObject();
		while (p2!=NULL)
		{
            ///\bug{TW: This check is not necessary, I believe. This is the most-expensive function in most codes (the two checks in this function slows down granular jet by 15%) and the selftests are not affected. DK: I do think this is neccesary, for example: If two cells hash to the same bucket and a particle in one of these cells check for collisions with the other cell. Then due to the hashingcollision it also gets all particles in it's own cell and thus generating false collisions.}
            //Check if the particle realy is in the target cell (i.e. no hashing error has occured)
			if ((p1->get_HGRID_x() == p2->get_HGRID_x()) && (p1->get_HGRID_y() == p2->get_HGRID_y()) && (p1->get_HGRID_Level() == p2->get_HGRID_Level()))
            {
                compute_internal_forces(p1,p2);
            }
			p2 = p2->get_HGRID_NextObject();
		}
		p1 = p1->get_HGRID_NextObject();
	}
	grid->bucketIsChecked[bucket] = true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///Check for collisions with a general cell
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HGRID_2D::CheckCell(int x, int y, int l, BaseParticle *obj, HGrid *grid)
{
	int bucket = grid->ComputeHashBucketIndex(x,y,l);

    if ((obj->get_HGRID_x() == x) && (obj->get_HGRID_y() == y) && (obj->get_HGRID_Level() == l)) {return; }

	// Loop through all objects in the bucket to find nearby objects
	BaseParticle *p = grid->objectBucket[bucket];

	while (p!=NULL)
	{
        ///\bug{TW: This check is not necessary, I believe. This is the most-expensive function in most codes (the two checks in this function slows down granular jet by 15%) and the selftests are not affected. DK: I do think this is neccesary, for example: If two cells hash to the same bucket and a particle in one of these cells check for collisions with the other cell. Then due to the hashingcollision it also gets all particles in it's own cell and thus generating false collisions.}
		//Check if the particle realy is in the target cell (i.e. no hashing error has occured)
		if ((p->get_HGRID_x() == x) && (p->get_HGRID_y() == y) && (p->get_HGRID_Level() == l)) 
		{
			compute_internal_forces(obj,p);
		}
		p = p->get_HGRID_NextObject();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Test collisions between object and all objects in hgrid
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HGRID_2D::CheckObjAgainstGrid(HGrid *grid, BaseParticle *obj)
{
	unsigned int startLevel		= obj->get_HGRID_Level();
	
	int x, y;
	Mdouble inv_size;
    
	switch(getHGridMethod())
    {
    case BOTTOMUP:
        {	
            int occupiedLevelsMask	= grid->occupiedLevelsMask >> obj->get_HGRID_Level();
            for (unsigned int level = startLevel; level < grid->cellSizes_.size(); occupiedLevelsMask >>= 1, level++) 
            {
                // If no objects in rest of grid, stop now
                if (occupiedLevelsMask == 0) break;

                // If no objects at this level, go on to the next level
                if ((occupiedLevelsMask & 1) == 0) continue;
                
                if (level == startLevel)	
                {
                    x = obj->get_HGRID_x();
                    y = obj->get_HGRID_y();
                    CheckCell_current(x, y, level, grid);
                    CheckCell(x  , y+1, level, obj, grid);
                    CheckCell(x+1, y-1, level, obj, grid);			
                    CheckCell(x+1, y  , level, obj, grid);
                    CheckCell(x+1, y+1, level, obj, grid);
                } 
                else
                {
                    int xs,ys,xe,ye;
                    inv_size = grid->invCellSizes_[level];
                    xs = (int) floor((obj->get_Position().X-obj->get_InteractionRadius()) * inv_size -0.5);
                    xe = (int) floor((obj->get_Position().X+obj->get_InteractionRadius()) * inv_size +0.5);
                    ys = (int) floor((obj->get_Position().Y-obj->get_InteractionRadius()) * inv_size -0.5);
                    ye = (int) floor((obj->get_Position().Y+obj->get_InteractionRadius()) * inv_size +0.5);
                    for(x=xs;x<=xe;x++)
                    {
                        for(y=ys;y<=ye;y++)
                        {
                            CheckCell(x,y,level,obj,grid);
                        }
                    }
                }
            }
        break;
        }
    case TOPDOWN:
        {	
            int occupiedLevelsMask	= grid->occupiedLevelsMask;
            for (unsigned int level = 0; level <= startLevel; occupiedLevelsMask >>= 1, level++) 
            {
                // If no objects in rest of grid, stop now
                if (occupiedLevelsMask == 0) break;

                // If no objects at this level, go on to the next level
                if ((occupiedLevelsMask & 1) == 0) continue;
                
                if (level == startLevel)	
                {
                    x = obj->get_HGRID_x();
                    y = obj->get_HGRID_y();
                    CheckCell_current(x, y, level, grid);
                    CheckCell(x  , y+1, level, obj, grid);
                    CheckCell(x+1, y-1, level, obj, grid);			
                    CheckCell(x+1, y  , level, obj, grid);
                    CheckCell(x+1, y+1, level, obj, grid);
                } 
                else
                {
                    int xs,ys,xe,ye;
                    inv_size = grid->invCellSizes_[level];
                    xs = (int) floor((obj->get_Position().X-obj->get_InteractionRadius()) * inv_size-0.5);
                    xe = (int) floor((obj->get_Position().X+obj->get_InteractionRadius()) * inv_size+0.5);
                    ys = (int) floor((obj->get_Position().Y-obj->get_InteractionRadius()) * inv_size-0.5);
                    ye = (int) floor((obj->get_Position().Y+obj->get_InteractionRadius()) * inv_size+0.5);
                    for(x=xs;x<=xe;x++)
                    {
                        for(y=ys;y<=ye;y++)
                        {
                            CheckCell(x,y,level,obj,grid);
                        }
                    }		
                }
            }
        break;
        }
    }    
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Add object to grid square, and remember cell and level numbers,
/// treating level as a third dimension coordinate
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void HGRID_2D::HGRID_UpdateParticleInHgrid(BaseParticle *obj)
{
	int l = obj->get_HGRID_Level();
	Mdouble inv_size = grid->invCellSizes_[l];

	int x=(int)floor(obj->get_Position().X * inv_size);
	int y=(int)floor(obj->get_Position().Y * inv_size);

#ifdef ContactListHgrid
	if(obj->get_HGRID_x()!=x||obj->get_HGRID_y()!=y||obj->get_PeriodicFromParticle())
	{
		if(!obj->get_PeriodicFromParticle())
		{
			int bucket = grid->ComputeHashBucketIndex(x, y, l);
					
			//First the object has to be removed
			HGRID_RemoveParticleFromHgrid(obj);

			//Also remove all contact associated with it
			getPossibleContactList().remove_ParticlePosibleContacts(obj);
			
			//And now reinserted
			obj->set_HGRID_NextObject(grid->objectBucket[bucket]);
			if(grid->objectBucket[bucket])
				grid->objectBucket[bucket]->set_HGRID_PrevObject(obj);
			obj->set_HGRID_PrevObject(0);
			grid->objectBucket[bucket] = obj;
		}
		
		obj->set_HGRID_x(x);
		obj->set_HGRID_y(y);
		
		InsertObjAgainstGrid(grid, obj);
	}
#else
	int bucket = grid->ComputeHashBucketIndex(x, y, l);

	obj->set_HGRID_NextObject(grid->objectBucket[bucket]);
	if(grid->objectBucket[bucket])
		grid->objectBucket[bucket]->set_HGRID_PrevObject(obj);
	obj->set_HGRID_PrevObject(0);
	grid->objectBucket[bucket] = obj;

	obj->set_HGRID_x(x);
	obj->set_HGRID_y(y);
#endif
}

void HGRID_2D::HGRID_RemoveParticleFromHgrid(BaseParticle *obj)
{
	int bucket = grid->ComputeHashBucketIndex(obj->get_HGRID_x(),obj->get_HGRID_y(), obj->get_HGRID_Level());
	if(obj->get_HGRID_PrevObject())
		obj->get_HGRID_PrevObject()->set_HGRID_NextObject(obj->get_HGRID_NextObject());
	else
		if(grid->objectBucket[bucket]==obj)
			grid->objectBucket[bucket]=obj->get_HGRID_NextObject();
		
	if(obj->get_HGRID_NextObject())
		obj->get_HGRID_NextObject()->set_HGRID_PrevObject(obj->get_HGRID_PrevObject());	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Tests obj against all particles in cell
/// similar to CheckCell, but links to TestObject instead of compute_internal_forces
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool HGRID_2D::TestCell(int x, int y, int l, BaseParticle *obj, HGrid *grid)
{
	bool Test = true;
		
	// Loop through all objects in the bucket to find nearby objects
	int bucket = grid->ComputeHashBucketIndex(x,y,l);
	BaseParticle *p = grid->objectBucket[bucket];
	
	while (Test && p!=NULL)
	{
		if ((p->get_HGRID_x() == x) && (p->get_HGRID_y() == y) && (p->get_HGRID_Level() == l)) 
		{
			Test = Test && TestObject(obj,p);		
		}
		p = p->get_HGRID_NextObject();
	}

	return Test;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Tests obj against all neighbouring particles
/// similar to CheckObjAgainstGrid, but links to TestCell instead of CheckCell
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool HGRID_2D::TestObjAgainstGrid(HGrid *grid, BaseParticle *obj)
{

	bool Test = true;
	
	int x, y;
	Mdouble inv_size;
	int occupiedLevelsMask = grid->occupiedLevelsMask;
	
	for (unsigned int level = 0; level < grid->cellSizes_.size() && Test; occupiedLevelsMask >>= 1, level++) 
	{
		// If no objects in rest of grid, stop now
		if (occupiedLevelsMask == 0) break;
		
		// If no objects at this level, go on to the next level
		if ((occupiedLevelsMask & 1) == 0) continue;
		
		// Treat level as a third dimension coordinate
		inv_size = grid->invCellSizes_[level];
		
		x = (int)floor(obj->get_Position().X * inv_size);
		y = (int)floor(obj->get_Position().Y * inv_size);

		Test = Test 
				&& TestCell(x  , y  , level, obj, grid)
				&& TestCell(x  , y-1, level, obj, grid)
				&& TestCell(x  , y+1, level, obj, grid)
				&& TestCell(x-1, y  , level, obj, grid)
				&& TestCell(x+1, y  , level, obj, grid)
				&& TestCell(x-1, y-1, level, obj, grid)
				&& TestCell(x-1, y+1, level, obj, grid)
				&& TestCell(x+1, y-1, level, obj, grid)
				&& TestCell(x+1, y+1, level, obj, grid);
	} //end for level

	return Test;
} 

#ifdef ContactListHgrid
void HGRID_2D::InsertCell(int x, int y, int l, BaseParticle *obj, HGrid *grid)
{
	// Loop through all objects in the bucket to find nearby objects
	int bucket = grid->ComputeHashBucketIndex(x,y,l);
	BaseParticle *p = grid->objectBucket[bucket];
	
	while (p!=NULL)
	{
		if ((p->get_HGRID_x() == x) && (p->get_HGRID_y() == y) && (p->get_HGRID_Level() == l) && (obj!=p)) 
		{
			getPossibleContactList().add_PossibleContact(obj,p);
		}
		p = p->get_HGRID_NextObject();
	}
}

void HGRID_2D::InsertObjAgainstGrid(HGrid *grid, BaseParticle *obj)
{
	int xmin,xmax;
	int ymin,ymax;
	Mdouble inv_size;
	int occupiedLevelsMask = grid->occupiedLevelsMask;
	
	inv_size=grid->inv_size[obj->get_HGRID_Level()];
	
	double ownXMin=(obj->get_HGRID_x()-0.5)/inv_size;
	double ownXMax=(obj->get_HGRID_x()+1.5)/inv_size;
	double ownYMin=(obj->get_HGRID_y()-0.5)/inv_size;
	double ownYMax=(obj->get_HGRID_y()+1.5)/inv_size;
	
	for (int level = 0; level < grid->HGRID_MAX_LEVELS; occupiedLevelsMask >>= 1, level++) 
	{
		// If no objects in rest of grid, stop now
		if (occupiedLevelsMask == 0) break;
		
		// If no objects at this level, go on to the next level
		if ((occupiedLevelsMask & 1) == 0) continue;
		
		// Treat level as a third dimension coordinate
		inv_size = grid->inv_size[level];
		
		xmin=(int)floor(ownXMin*inv_size-0.5);
		xmax=(int)floor(ownXMax*inv_size+0.5);
		ymin=(int)floor(ownYMin*inv_size-0.5);
		ymax=(int)floor(ownYMax*inv_size+0.5);
		
		for(int i=xmin;i<=xmax;i++)
			for(int j=ymin;j<=ymax;j++)
				InsertCell(i, j, level, obj, grid); 
	} //end for level
}
#endif
