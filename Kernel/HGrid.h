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

#include <vector>
#include "Math/ExtendedMath.h"

class BaseParticle;

/*!
 * \class HGrid
 * \brief This is the HGRID class  - This is the actually HGRID code
 */
class HGrid
{
public:
    /*!
     * \brief default constructor
     */    
    HGrid();

    //-------------------------------------------------
    /*!
     * \brief constructor: initialises parameters and allocates space for internal variables
     */
    HGrid(unsigned int num_buckets, double cellOverSizeRatio, std::vector<double>& cellSizes);

    /*!
     * \brief destructor
     */
    ~HGrid();

    /*!
     * \brief 
     */
    void Initialize_inv_size();

    /*!
     * \brief This insert a particle given by CParticle in to the HGrid (i.e. it sets up the particle grid properts updates the level information on the grid)
     */
    void insertParticleToHgrid(BaseParticle *obj);

    /*!
     * \brief Computes hash bucket index in range [0, NUM_BUCKETS-1]
     */
    unsigned int computeHashBucketIndex(int x, int y, int z, unsigned int l) const;

    /*!
     * \brief Computes hash bucket index in range [0, NUM_BUCKETS-1]
     */
    unsigned int computeHashBucketIndex(int x, int y, unsigned int l) const;

    /*!
     * \brief 
     */
    void clearBucketIsChecked();
    
    /*!
     * \brief 
     */
    void clearFirstBaseParticleInBucket();

    /*!
     * \brief 
     */
    void setFirstBaseParticleInBucket(unsigned int i, BaseParticle* p);

    /*!
     * \brief 
     */
    void setBucketIsChecked(unsigned int i);

    /*!
     * \brief 
     */
    bool getBucketIsChecked(unsigned int i) const;

    /*!
     * \brief 
     */
    Mdouble getCellOverSizeRatio() const;

    /*!
     * \brief 
     */
    double getCellSize(unsigned int i) const;

    /*!
     * \brief 
     */
    const std::vector<double>& getCellSizes() const;

    /*!
     * \brief 
     */
    const BaseParticle* getFirstBaseParticleInBucket(unsigned int i) const;

    /*!
     * \brief 
     */
    BaseParticle* getFirstBaseParticleInBucket(unsigned int i);

    /*!
     * \brief 
     */
    double getInvCellSize(unsigned int i) const;

    /*!
     * \brief 
     */
    const std::vector<double>& getInvCellSizes() const;

    /*!
     * \brief 
     */
    bool getNeedsRebuilding() const;

    /*!
     * \brief 
     */
    unsigned int getNumberOfBuckets() const;

    /*!
     * \brief 
     */
    unsigned int getNumberOfLevels() const;

    /*!
     * \brief 
     */
    int getOccupiedLevelsMask() const;

private:
    bool needsRebuilding_; ///flag sets if the HGrid needs to be rebuild
    unsigned int numberOfBuckets_; ///The number of buckets in the current HGrid
    Mdouble cellOverSizeRatio_;///
    int occupiedLevelsMask_; /// Marks if there are particle at certain levels, l-th bit of occupiedLevelsMask is 1 if level l is contains particles (Implies max 32 hgrid levels)
    std::vector<double> cellSizes_; ///The sizes of the cells in the different grids
    std::vector<double> invCellSizes_; ///The inverse sizes of the cells in the different grids
    std::vector<BaseParticle*> firstBaseParticleInBucket_; ///Stores a pointer to first element in hash bucket b; initially all nullptr
    std::vector<bool> bucketIsChecked_; ///bucketIsChecked stores if hash bucket b is checked already; initially all false
};
#endif
