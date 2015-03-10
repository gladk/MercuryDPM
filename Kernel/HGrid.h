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
