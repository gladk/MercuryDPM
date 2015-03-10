//Copyright (c) 2015, The MercuryDPM Developers Team. All rights reserved.
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

#ifndef TOOLS_MERCURYDATA_H
#define TOOLS_MERCURYDATA_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

/*!
 * Stores a single mercury particle
 * for usage by external converters.
 */
struct MercuryParticle
{
  public:
    /*!
     * X Y Z coordinates
     */
    double position[3];
    /*!
     * X Y Z velocities
     */
    double velocity[3];
    /*!
     * P Y R rotations?
     */
    double rotation[3];
    /*!
     * P Y R velocities?
     */
    double angularV[3];
    
    /*!
     * Radius of the particle
     */
    double radius;
    
    /*!
     * Index of the species of this particle.
     */
    std::size_t speciesID;
};

/*!
 * \brief Read a single particle from a istream.
 */
std::istream& operator>>(std::istream&, MercuryParticle&);
class MercuryDataFile;
class MercuryTimeStep;

/*!
 *  Full timestep in the Mercury 3D .data format.
 *  This holds all the particles used in this timestep.
 */
class MercuryTimeStep
{
  public:
    /*!
    * \brief Gets the time associated with this timestep
    * \return the time
    */
    double getTime() const
    {
      return time_;
    }
    
    /*!
    * \brief Gets the timestep ID
    * Returns the timestep ID, which is a consecutively ascending
    * number unique for the timestep.
    * \return a unique ID for this timestep
    */
    std::size_t getTimeStepID() const
    {
      return ID_;
    }
    
    /*!
    * \brief Gets the number of particles recorded in this timestep
    * \return the number of particles
    */
    std::size_t getNumberOfParticles() const
    {
      return numParticles_;
    }
    
    /*!
    * \brief Iterator functions for range based for loops
    */
    std::vector<MercuryParticle>::iterator begin()
    {
      return storage_.begin();
    }
    
    /*!
    * \brief Iterator functions for range based for loops
    */
    std::vector<MercuryParticle>::const_iterator begin() const
    {
      return storage_.begin();
    }
    
    /*!
    * \brief Iterator functions for range based for loops
    */
    std::vector<MercuryParticle>::iterator end()
    {
      return storage_.end();
    }
    
    /*!
    * \brief Iterator functions for range based for loops
    */
    std::vector<MercuryParticle>::const_iterator end() const
    {
      return storage_.end();
    }
    
    /*!
    * \brief Random access function into the particles.
    */
    MercuryParticle& operator[](std::size_t idx)
    {
      return storage_[idx];
    }
    
    /*!
    * \brief Random access function into the particles.
    */
    const MercuryParticle& operator[](std::size_t idx) const
    {
      return storage_[idx];
    }
    
  private:
    /*!
    * \brief Constructor used by the MercuryTimeStepIterator, to flag a functional timestep
    * \param[in] id The unique ID given to this particle
    * \param[in,out] pData A pointer to the MercuryDataFile, which contains the backing std::ifstream
    */
    MercuryTimeStep(std::size_t id, MercuryDataFile *pData)
    : time_(0), ID_(id), numParticles_(0), dataFile_(pData)
    {
    }
    
    /*!
    * \brief EOF-Timestep constructor used by MercuryTimeStepIterator (and MercuryDataFile::isMercury3DDataFile())
    */
    MercuryTimeStep()
    : time_(0), ID_(0), numParticles_(0), dataFile_(nullptr)
    {
    }
    
    /*!
    * Time associated with this MercuryTimeStep
    */
    double time_;
    /*!
    * Unique ID associated with this MercuryTimeStep
    */
    std::size_t ID_;
    /*!
    * Cached count of particles in this MercuryTimeStep
    */
    std::size_t numParticles_;
    /*!
    Minima and maxima of the AABB of the domain; none of these are used.
    */
    double xMin_, xMax_;
    double yMin_, yMax_;
    double zMin_, zMax_;
    /*!
     * Pointer to the base file, required for the backing std::ifstream
     * \todo Chris, please check if this parameter is needed; it causes a warning with [-Wunused-private-field] \author weinhartt 
     */
    MercuryDataFile *dataFile_;
    
    /*!
    * Backing storage vector used for the particles in this timestep
    */
    std::vector<MercuryParticle> storage_;
    
    friend std::istream& operator>>(std::istream&, MercuryTimeStep&);
    friend class MercuryTimeStepIterator;
    friend class MercuryDataFile;
};

/*!
* Reads the timestep header into the ts
* \param[in,out] in The istream where we read from
* \param[out] ts The timestream read into
* \return in for chaining.
*/
std::istream& operator>>(std::istream&, MercuryTimeStep&);

/*!
 * Lazy timestep iterator
 * Only loads the current timestep and discards this as soon as
 * it is incremented.
 * This is a ForwardIterator as described by the C++11 standard
 * This iterator invalidates any references to its dereferenced value
 * when incremented.
 */
class MercuryTimeStepIterator
{
  public:
    /*!
     * \brief Not-equals operator, as defined for ForwardIterators
     */
    bool operator!=(MercuryTimeStepIterator other) const
    {
      return (isEOFTimeStep_ != other.isEOFTimeStep_);
    }
    
    /*!
     * \brief Dereference operator, as defined for ForwardIterators
     */
    MercuryTimeStep& operator*()
    {
      return lastReadTimeStep_;
    }
    
    /*!
     * \brief Const dereference operator, as defined for constant ForwardIterators
     */
    const MercuryTimeStep& operator*() const
    {
      return lastReadTimeStep_;
    }
    
    /*!
     * \brief Pre-increment operator, as defined for ForwardIterators
     */
    void operator++();
  
  private:
    /*!
     * EOF TimeStepIterator constructor, as used by MercuryDataFile::end()
     */
    MercuryTimeStepIterator()
      : isEOFTimeStep_(true), dataFile_(nullptr)
    {
    }
    
    /*!
     * Beginning-of-file TimeStepIterator constructor, as used by MercuryDataFile::begin()
     * \param[in] pData Pointer to the MercuryDataFile, which is required for the backing std::ifstream
     */
    MercuryTimeStepIterator(MercuryDataFile *pData)
      : lastReadTimeStep_(0,pData), isEOFTimeStep_(false), dataFile_(pData)
    {
      ++(*this);
      lastReadTimeStep_.ID_ = 0;
    }
  
    /*!
     * The complete last read timestep, used for caching
     */
    MercuryTimeStep lastReadTimeStep_;
    /*!
     * Status flag for EOF checking
     */
    bool isEOFTimeStep_;
    /*!
     * Pointer to the MercuryDataFile and the backing std::ifstream, which is required for reading
     */
    MercuryDataFile *dataFile_;
    
    friend class MercuryDataFile;
};


/*!
 * Lazy loader for .data files.
 * This class allows you to iterate over all the
 * particles in it.
 */
class MercuryDataFile
{
  public:
    /*!
     * Opens the Mercury 3D .data file with filename
     * file.
     * \param[in] file The filename
     */
    MercuryDataFile(std::string file);

    /*!
     * \brief Gives the status of the backing std::ifstream
     * Returns true is no error has occurred. In any other case
     * (EOF, FailBit, ErrorBit), it returns false.
     * For more information, see std::ifstream::operator bool().
     * \returns true if the underlying file is readable, false otherwise.
     */
    operator bool() const
    {
      return file_.good();
    }
    
    /*!
     * \brief Checks if this file is a valid Mercury 3D data file.
     * This function jumps to the start of the file, reads the first
     * line and restores the readpointer.
     * It then checks if that line can be succesfully parsed
     * as a time step header. This does however not check if the
     * file is consistent or the particle entries are valid.
     * It can however serve as a first sanity check.
     * \returns true if the file appears to be a valid Mercury 3D .data file.
     */
    bool isMercury3DDataFile();
    
    /*!
     * \brief Returns a forwarditerator to the timesteps
     * Returns a forwarditerator to the timesteps,
     * invalidating any other valid iterators in the process
     * (since this is a lazy loader and does not actually
     * load the entire file in memory). End iterators do not
     * get invalidated. 
     * This function makes no guarantee for the validity of the file-state.
     */
    MercuryTimeStepIterator begin()
    {
      file_.seekg(0);
      return {this};
    }
    
    /*!
     * \brief Returns a forwarditerator one past the last timestep.
     */
    MercuryTimeStepIterator end() const
    {
      return {};
    }
  private:
    /*!
     * The backing file stream used to represent the data file.
     */
    std::ifstream file_;
    
    friend class MercuryTimeStep;
    friend class MercuryTimeStepIterator;
};


#endif