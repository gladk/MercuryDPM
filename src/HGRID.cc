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

#include "HGRID.h"
#include "Particles/BaseParticle.h"

HGrid::HGrid()
{
    objectBucket = 0; 
    bucketIsChecked = 0;
}

///constructor: initializes parameters and allocates space for internal variables
HGrid::HGrid(int num_buckets, double cellOverSizeRatio, std::vector<double>& cellSizes)
{
    NUM_BUCKETS = num_buckets;
    cellOverSizeRatio_=cellOverSizeRatio;
    
    objectBucket = new BaseParticle*[NUM_BUCKETS];
    bucketIsChecked = new bool[NUM_BUCKETS];
            
    for(int i=0;i<cellSizes.size();i++)
    {
        cellSizes_.push_back(cellSizes[i]);
        std::cout<<"Level="<<i<<" Grid size="<<cellSizes_.back()<<std::endl;
    }    
}

///constructor: initializes parameters and allocates space for internal variables
HGrid::~HGrid() {
    if (objectBucket) { delete [] objectBucket; objectBucket=0; }
    if (bucketIsChecked) { delete [] bucketIsChecked; bucketIsChecked=0; }
}

void HGrid::Initialize_inv_size()
{
    invCellSizes_.clear();
    for (unsigned int level = 0; level<cellSizes_.size(); level++)
    {
        invCellSizes_.push_back(1.0/cellSizes_[level]);    
    }
}

/////////////////////////////////////////////////////////////////////////////////
///This insert a particle given by CParticle in to the HGrid (i.e. it sets up the particle grid properts updates the level information on the grid)
/////////////////////////////////////////////////////////////////////////////////
void HGrid::InsertParticleToHgrid(BaseParticle *obj)
{
    // Find lowest level where object fully fits inside cell, taking RATIO into account
    Mdouble diameter = obj->get_InteractionRadius() * 2.0 ;
    unsigned int level=0;
    while(level<cellSizes_.size()&&cellSizes_[level]<=diameter * cellOverSizeRatio_)
    {
        level++;
    }

    if (level >= cellSizes_.size()) 
    { 
        std::cerr << "ATTENTION !!! Object is larger (d=" << diameter << ", cellOverSizeRatio=" << cellOverSizeRatio_ << ") than largest grid cell allows (" << cellSizes_.back() << ")!" << std::endl; 
        exit(-1);
    }   
    
    obj->set_HGRID_Level(level);        
    // indicate level is in use - not levels with no particles no collision detection is performed
    this->occupiedLevelsMask |= (1 << level);
}

                    //-------------------------------------------------
    
/// Computes hash bucket index in range [0, NUM_BUCKETS-1]
int HGrid::ComputeHashBucketIndex(int x, int y, int z, int l)
{
    const int h1 = 0x8da6b343; // Large multiplicative constants;
    const int h2 = 0xd8163841; // here arbitrarily chosen primes
    const int h3 = 0xcb1ab31f;
    const int h4 = 0x165667b1;
        
    int n = h1 * x + h2 * y + h3 * z + h4 * l;
    n = n % NUM_BUCKETS;
        
    if (n < 0) n += NUM_BUCKETS;
                                                        
    return n;
}	

int HGrid::ComputeHashBucketIndex(int x, int y, int l)
{
    const int h1 = 0x8da6b343; // Large multiplicative constants;
    const int h2 = 0xd8163841; // here arbitrarily chosen primes
    const int h4 = 0x165667b1;
        
    int n = h1 * x + h2 * y + h4 * l;
    n = n % NUM_BUCKETS;
        
    if (n < 0) n += NUM_BUCKETS;
                                                        
    return n;
}


///\bug{I don't know if this function still works since we do not use (m/c)alloc anymore}
void HGrid::reset_num_buckets(int new_num_buckets){
    NUM_BUCKETS=new_num_buckets;
    objectBucket = (BaseParticle**)realloc(objectBucket, NUM_BUCKETS*sizeof(BaseParticle*));
    bucketIsChecked = (bool*)realloc(bucketIsChecked, NUM_BUCKETS*sizeof(bool));
}

