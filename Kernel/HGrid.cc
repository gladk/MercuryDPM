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
#include "Logger.h"
#include "Particles/BaseParticle.h"

HGrid::HGrid()
{
    needsRebuilding_ = true;
    numberOfBuckets_ = 10;
    cellOverSizeRatio_ = 1.0;
    occupiedLevelsMask_ = 0;
    logger(DEBUG, "HGrid::HGrid() finished");
}

/*!
 * \param[in] num_buckets       The number of buckets that are used by this HGrid.
 * \param[in] cellOverSizeRatio The maximum ratio between the size of the 
 *                              cell over the size of the particle.
 * \param[in] cellSizes         The sizes of the cells we want to set.
 * \details                     Constructor: initialises parameters and allocates 
 *                              space for internal variables. 
 */
HGrid::HGrid(unsigned int num_buckets, double cellOverSizeRatio, std::vector<double>& cellSizes)
{
    needsRebuilding_ = false;
    numberOfBuckets_ = num_buckets;
    cellOverSizeRatio_ = cellOverSizeRatio;
    occupiedLevelsMask_ = 0;
    
    firstBaseParticleInBucket_.resize(numberOfBuckets_, nullptr);
    bucketIsChecked_.resize(numberOfBuckets_, false);

    //std::cout<<"Creating HGrid "<<cellSizes.size()<<" levels:"<<std::endl;
    for (unsigned int i = 0; i < cellSizes.size(); i++)
    {
        //std::cout<<"Level="<<i<<" size="<<cellSizes[i]<<std::endl;
    	cellSizes_.push_back(cellSizes[i]);
        invCellSizes_.push_back(1.0 / cellSizes[i]);
    }
    logger(DEBUG, "HGrid::HGrid(unsigned int, double, vector<double>&) constructor finished.");
    /*  std::cout << "HGrid::HGrid(" << num_buckets << ", " << cellOverSizeRatio << ", [";
        for (auto p: cellSizes) std::cout << p << " "; 
        std::cout << "]) finished" << std::endl;*/
}

HGrid::~HGrid()
{
    logger(DEBUG, "HGrid::~HGrid() destructor finished");
}

/*!
 * \param[in] obj A pointer to the BaseParticle we want to add to the HGrid.
 * \details Inserts the given BaseParticle into the HGrid, i.e. it sets up the 
 *          particle grid properties and updates the level information on the grid.
 *          First find which level is big enough to fit the BaseParticle in, then 
 *          add the BaseParticle to that level and set that level as occupied in 
 *          the occupiedLevelsMask_.
 * \bug What happens if the particle is too big for the biggest cell? It just says
 *      that it needs to rebuild the HGrid, but the particle is not inserted and
 *      there seems to be no indication to the rest of the code that it has not 
 *      been inserted. For now giving a warning, since code of users may rely on
 *      it that nothing happens.
 */
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
            logger(WARN, "WARNING: object (id = %, index = %) is larger (d = %, cellOverSizeRatio = %) than largest grid cell (%) allows.", obj->getId(), obj->getIndex(), diameter, cellOverSizeRatio_, cellSizes_.back());
            needsRebuilding_ = true;
        }

        obj->setHGridLevel(level);
        // indicate level is in use - not levels with no particles no collision detection is performed
        this->occupiedLevelsMask_ |= (1 << level);
    }
    else
    {
        logger(WARN, "WARNING: the HGrid needs to be rebuild before insertParticleToHgrid may be called!");
    }
}

/*!
 * \details Computes a hash from parameters, the result is in range [0, numberOfBuckets_-1].
 * \param[in] x The coordinate of the cell in x direction for which the hash must be computed.
 * \param[in] y The coordinate of the cell in y direction for which the hash must be computed.
 * \param[in] z The coordinate of the cell in z direction for which the hash must be computed.
 * \param[in] l The level in the HGrid of the cell for which the hash must be computed.
 * \return The hash value for the given cell (x,y,z,l), which is in the range [0,numberOfBuckets_-1].
 */
unsigned int HGrid::computeHashBucketIndex(int x, int y, int z, unsigned int l) const
{
    const unsigned int h1 = 0x8da6b343u; // Large multiplicative constants;
    const unsigned int h2 = 0xd8163841u; // here arbitrarily chosen primes
    const unsigned int h3 = 0xcb1ab31fu;
    const unsigned int h4 = 0x165667b1u;
    
    unsigned long int n = h1 * x + h2 * y + h3 * z + h4 * l;
    n = n % numberOfBuckets_;
    
    return static_cast<unsigned int>(n);
}

/*!
 * \details Computes a hash from parameters, the result is in range [0, numberOfBuckets_-1].
 * \param[in] x The coordinate of the cell in x direction for which the hash must be computed.
 * \param[in] y The coordinate of the cell in y direction for which the hash must be computed.
 * \param[in] l The level in the HGrid of the cell for which the hash must be computed.
 * \return The hash value for the given cell (x,y,l), which is in the range [0,numberOfBuckets_-1].
 */
unsigned int HGrid::computeHashBucketIndex(int x, int y, unsigned int l) const
{
    const unsigned int h1 = 0x8da6b343u; // Large multiplicative constants;
    const unsigned int h2 = 0xd8163841u; // here arbitrarily chosen primes
    const unsigned int h4 = 0x165667b1u;
    
    unsigned long int n = h1 * x + h2 * y + h4 * l;
    n = n % numberOfBuckets_;
    
    return static_cast<unsigned int>(n);
}

/*!
 * \return The number of buckets in this HGrid.
 */
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

/*!
 * \return A vector with the sizes of the cells of different levels.
 */
const std::vector<double>& HGrid::getCellSizes() const
{
    return cellSizes_;
}

/*!
 * \return A vector with the inverse sizes (1/size) of the cells of different levels.
 */
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

/*!
 * \param[in] i The ordinal number of the bucket we want to know for whether or not it has been checked.
 * \return A boolean which is true if the bucket is checked and false otherwise.
 */
bool HGrid::getBucketIsChecked(unsigned int i) const
{
    return bucketIsChecked_[i];
}

/*!
 * \param[in] i The ordinal number of the bucket for which we want to get the first particle of.
 * \return A pointer to the (constant) BaseParticle which is the first Baseparticle in the given bucket.
 */
const BaseParticle* HGrid::getFirstBaseParticleInBucket(unsigned int i) const
{
    return firstBaseParticleInBucket_[i];
}

/*!
 * \param[in] i The ordinal number of the bucket for which we want to get the first particle of.
 * \return A pointer to the BaseParticle which is the first Baseparticle in the given bucket.
 */
BaseParticle* HGrid::getFirstBaseParticleInBucket(unsigned int i)
{
    return firstBaseParticleInBucket_[i];
}

/*!
 * \param[in] i The ordinal number of the bucket we want to mark as checked.
 */
void HGrid::setBucketIsChecked(unsigned int i)
{
    bucketIsChecked_[i] = true;
}

/*!
 * \return The number of levels in this HGrid.
 */
unsigned int HGrid::getNumberOfLevels() const
{
    return cellSizes_.size();
}

/*!
 * \param[in] i The ordinal number of the bucket we want to set the first BaseParticle for.
 * \param[in] p A pointer to the BaseParticle we want to place in the given bucket.
 */
void HGrid::setFirstBaseParticleInBucket(unsigned int i, BaseParticle* p)
{
    firstBaseParticleInBucket_[i] = p;
}

/*!
 * \param[in] i The level we want to know the cell size of.
 * \return The size of the cells at the given level.
 */
double HGrid::getCellSize(unsigned int i) const
{
    return cellSizes_[i];
}

/*!
 * \param[in] i The level we want to know the inverse cell size of.
 * \return The inverse size, i.e. 1/size, of the cells at the given level.
 */
double HGrid::getInvCellSize(unsigned int i) const
{
    return invCellSizes_[i];
}

/*!
 * \return A boolean which indicates whether or not the HGrid needs rebuilding.
 */
bool HGrid::getNeedsRebuilding() const
{
    return needsRebuilding_;
}

/*!
 * \return The ratio between the size of the smallest cell and the smallest BaseParticle.
 */
Mdouble HGrid::getCellOverSizeRatio() const
{
    return cellOverSizeRatio_;
}

/*!
 * \return The integer that represents the bit-vector that indicates which levels have at least one particle.
 */
int HGrid::getOccupiedLevelsMask() const
{
    return occupiedLevelsMask_;
}

void HGrid::info() const
{
    std::cout << "Current status of hGrid parameters:" << std::endl;
    std::cout << "bool needsRebuilding_=" << needsRebuilding_ << std::endl; 
    std::cout << "unsigned int numberOfBuckets_=" << numberOfBuckets_ << std::endl; 
    std::cout << "Mdouble cellOverSizeRatio_=" << cellOverSizeRatio_ << std::endl;
    std::cout << "int occupiedLevelsMask_=" << occupiedLevelsMask_ << std::endl; 
    std::cout << "std::vector<double> cellSizes_[" << cellSizes_.size() << "]="; 
    for (auto p: cellSizes_) std::cout << p << " "; 
    std::cout << std::endl; 
    std::cout << "std::vector<double> invCellSizes_[" << invCellSizes_.size() << "]="; 
    for (auto p: invCellSizes_) std::cout << p << " "; 
    std::cout << std::endl; 
    std::cout << "std::vector<BaseParticle> firstBaseParticleInBucket_[" << firstBaseParticleInBucket_.size() << "]=... (not shown)"; 
    //for (auto p: firstBaseParticleInBucket_) std::cout << p << " "; 
    std::cout << std::endl; 
    std::cout << "std::vector<bool> bucketIsChecked_[" << bucketIsChecked_.size() << "]=... (not shown)"; 
    //for (auto p: bucketIsChecked_) std::cout << p << " "; 
    std::cout << std::endl; 
}
