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

#ifndef HGRID_H
#define HGRID_H

#include <iostream>
#include <vector>
#include <stdlib.h>
#include "ExtendedMath.h"

class BaseParticle;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///This is the HGRID class  - This is the actually HGRID code
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class HGrid
{
public:
	
	HGrid();
	
	//----- parameters ------
	///Number of buckets used for hashing
	int NUM_BUCKETS; 
	
    
    std::vector<double> cellSizes_;
    std::vector<double> invCellSizes_;
	
	//----- internal variables ----------
	///l-th bit of occupiedLevelsMask is 1 if level l is contains particles; initially zero (Implies max 32 hgrid levels)
	int occupiedLevelsMask; 
	
	///objectBucket[b] stores pointer to first element in hash bucket b; initially all NULL
	BaseParticle **objectBucket; 
	
	///bucketIsChecked[b] stores if hash bucket b is checked already; initially all zero
	bool *bucketIsChecked; 
	
	//-------------------------------------------------
	///constructor: initializes parameters and allocates space for internal variables
	HGrid(int num_buckets, double cellOverSizeRatio, std::vector<double>& cellSizes);
    
	///constructor: initializes parameters and allocates space for internal variables
	~HGrid();
	
	void Initialize_inv_size();
	
	
/////////////////////////////////////////////////////////////////////////////////
///This insert a particle given by CParticle in to the HGrid (i.e. it sets up the particle grid properts updates the level information on the grid)
/////////////////////////////////////////////////////////////////////////////////
	void InsertParticleToHgrid(BaseParticle *obj);
		
	/// Computes hash bucket index in range [0, NUM_BUCKETS-1]
	int ComputeHashBucketIndex(int x, int y, int z, int l);
	
    /// Computes hash bucket index in range [0, NUM_BUCKETS-1]
	int ComputeHashBucketIndex(int x, int y, int l);
	
    Mdouble cellOverSizeRatio_;
    
	///\bug{I don't know if this function still works since we do not use (m/c)alloc anymore}
	void reset_num_buckets(int new_num_buckets);
};
#endif
