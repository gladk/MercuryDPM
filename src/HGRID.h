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
