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

#include "HGrid.h"
#include "Particles/BaseParticle.h"

HGrid::HGrid()
{
    needsRebuilding_=true;
    numberOfBuckets_=10;
    cellOverSizeRatio_=1.0;
    occupiedLevelsMask_=0;
}

///constructor: initialises parameters and allocates space for internal variables
HGrid::HGrid(unsigned int num_buckets, double cellOverSizeRatio, std::vector<double>& cellSizes)
{
    needsRebuilding_ = false;
    numberOfBuckets_ = num_buckets;
    cellOverSizeRatio_ = cellOverSizeRatio;
    occupiedLevelsMask_=0;
    
    firstBaseParticleInBucket_.resize(numberOfBuckets_, nullptr);
    bucketIsChecked_.resize(numberOfBuckets_, false);

    //std::cout<<"Creating HGrid "<<cellSizes.size()<<" levels:"<<std::endl;
    for (unsigned int i = 0; i < cellSizes.size(); i++)
    {
        //std::cout<<"Level="<<i<<" size="<<cellSizes[i]<<std::endl;
    	cellSizes_.push_back(cellSizes[i]);
        invCellSizes_.push_back(1.0 / cellSizes[i]);
    }
}

///constructor: initialises parameters and allocates space for internal variables
HGrid::~HGrid()
{
}

/////////////////////////////////////////////////////////////////////////////////
///This insert a particle given by CParticle in to the HGrid (i.e. it sets up the particle grid properts updates the level information on the grid)
/////////////////////////////////////////////////////////////////////////////////
void HGrid::insertParticleToHgrid(BaseParticle *obj)
{
    if(!needsRebuilding_)
    {
        // Find lowest level where object fully fits inside cell, taking cellOverSizeRatio_ into account
        Mdouble diameter = obj->getInteractionRadius() * 2.0;
        unsigned int level = 0;
        while (level < cellSizes_.size() && cellSizes_[level] <= diameter * cellOverSizeRatio_)
        {
            level++;
        }

        if (level >= cellSizes_.size())
        {
            std::cerr << "ATTENTION !!! Object ( id=" << obj->getId() << " index=" << obj->getIndex() << ") is larger (d=" << diameter << ", cellOverSizeRatio=" << cellOverSizeRatio_ << ") than largest grid cell allows (" << cellSizes_.back() << ")!" << std::endl;
            needsRebuilding_ = true;
        }

        obj->setHGridLevel(level);
        // indicate level is in use - not levels with no particles no collision detection is performed
        this->occupiedLevelsMask_ |= (1 << level);
    }
}

//-------------------------------------------------

/// Computes hash bucket index in range [0, NUM_BUCKETS-1]
unsigned int HGrid::computeHashBucketIndex(int x, int y, int z, unsigned int l) const
        {
    const int h1 = static_cast<int>(0x8da6b343); // Large multiplicative constants;
    const int h2 = static_cast<int>(0xd8163841); // here arbitrarily chosen primes
    const int h3 = static_cast<int>(0xcb1ab31f);
    const int h4 = static_cast<int>(0x165667b1);
    
    int n = h1 * x + h2 * y + h3 * z + h4 * static_cast<int>(l);
    n = n % static_cast<int>(numberOfBuckets_);
    
    if (n < 0)
        n += numberOfBuckets_;
    
    return static_cast<unsigned int>(n);
}

unsigned int HGrid::computeHashBucketIndex(int x, int y, unsigned int l) const
        {
    const int h1 = static_cast<int>(0x8da6b343); // Large multiplicative constants;
    const int h2 = static_cast<int>(0xd8163841); // here arbitrarily chosen primes
    const int h4 = static_cast<int>(0x165667b1);
    
    int n = h1 * x + h2 * y + h4 * static_cast<int>(l);
    n = n % static_cast<int>(numberOfBuckets_);
    
    if (n < 0)
        n += numberOfBuckets_;
    
    return static_cast<unsigned int>(n);
}

unsigned int HGrid::getNumberOfBuckets() const
{
    return numberOfBuckets_;
}

void HGrid::clearBucketIsChecked()
{
    for (std::vector<bool>::iterator it = bucketIsChecked_.begin(); it != bucketIsChecked_.end(); ++it)
    {
        (*it) = false;
    }
}

const std::vector<double>& HGrid::getCellSizes() const
{
    return cellSizes_;
}

const std::vector<double>& HGrid::getInvCellSizes() const
{
    return invCellSizes_;
}

void HGrid::clearFirstBaseParticleInBucket()
{
    for (std::vector<BaseParticle*>::iterator it = firstBaseParticleInBucket_.begin(); it != firstBaseParticleInBucket_.end(); ++it)
    {
        (*it) = nullptr;
    }
}

bool HGrid::getBucketIsChecked(unsigned int i) const
        {
    return bucketIsChecked_[i];
}

const BaseParticle* HGrid::getFirstBaseParticleInBucket(unsigned int i) const
        {
    return firstBaseParticleInBucket_[i];
}

BaseParticle* HGrid::getFirstBaseParticleInBucket(unsigned int i)
{
    return firstBaseParticleInBucket_[i];
}

void HGrid::setBucketIsChecked(unsigned int i)
{
    bucketIsChecked_[i] = true;
}

unsigned int HGrid::getNumberOfLevels() const
{
    return cellSizes_.size();
}

void HGrid::setFirstBaseParticleInBucket(unsigned int i, BaseParticle* p)
{
    firstBaseParticleInBucket_[i] = p;
}

double HGrid::getCellSize(unsigned int i) const
        {
    return cellSizes_[i];
}

double HGrid::getInvCellSize(unsigned int i) const
        {
    return invCellSizes_[i];
}

bool HGrid::getNeedsRebuilding() const
{
    return needsRebuilding_;
}

Mdouble HGrid::getCellOverSizeRatio() const
{
    return cellOverSizeRatio_;
}

int HGrid::getOccupiedLevelsMask() const
{
    return occupiedLevelsMask_;
}
