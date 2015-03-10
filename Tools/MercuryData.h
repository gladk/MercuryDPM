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
 * \author dducks
 */
template<std::size_t NDIMS>
struct MercuryParticle
{
  public:
    /*!
     * X Y Z coordinates
     */
    double position[NDIMS];
    /*!
     * X Y Z velocities
     */
    double velocity[NDIMS];
    /*!
     * P Y R rotations?
     */
    double rotation[NDIMS];
    /*!
     * P Y R velocities?
     */
    double angularV[NDIMS];
    
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
 * Stores a single 2D mercury particle
 * for usage by external converters.
 * Because VTK can't deal with 2D particles,
 * This means that we'll have to zero-extend
 * these particles...
 */
template<>
struct MercuryParticle<2>
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
template<std::size_t NDIMS>
std::istream& operator>>(std::istream& in, MercuryParticle<NDIMS>& part)
{
  std::size_t i;
  for (i = 0; i < NDIMS; i++)
    in >> part.position[i];
  
  for (i = 0; i < NDIMS; i++)
    in >> part.velocity[i];
    
  in >> part.radius;
  
  for (i = 0; i < NDIMS; i++)
    in >> part.rotation[i];
    
  for (i = 0; i < NDIMS; i++)
    in >> part.angularV[i];
    
  in >> part.speciesID;
  
  return in;
}

template<>
std::istream& operator>><2>(std::istream& in, MercuryParticle<2>& part)
{
  std::size_t i;
  in >> part.position[0] >> part.position[2];
  part.position[1] = 0;
  
  in >> part.velocity[0] >> part.velocity[2];
  part.velocity[1] = 0;
  
  in >> part.radius;

  in >> part.rotation[1];
  part.rotation[0] = part.rotation[2] = 0;  

  in >> part.angularV[1];
  part.angularV[0] = part.angularV[2] = 0;
    
  in >> part.speciesID;
  
  return in;
}


class MercuryDataFile;

template<std::size_t NDIMS>
class MercuryTimeStepIterator;
/*!
 *  Full timestep in the Mercury 3D .data format.
 *  This holds all the particles used in this timestep.
 * \author dducks
 */
template<std::size_t NDIMS>
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
    * \sa size()
    */
    std::size_t getNumberOfParticles() const
    {
      return numParticles_;
    }
    
    /*!
    * \brief Gets the number of particles recorded in this timestep
    * \return the number of particles
    * \sa getNumberOfParticles()
    */
    std::size_t size() const
    {
      return numParticles_;
    }
    
    /*!
    * \brief returns the number of dimensions used.
    * \return the number of dimensions.
    */
    constexpr std::size_t getNumberOfDimensions() const
    {
      return NDIMS;
    }
    
    /*!
    * \brief Iterator functions for range based for loops
    */
    typename std::vector< MercuryParticle<NDIMS> >::iterator begin()
    {
      return storage_.begin();
    }
    
    /*!
    * \brief Iterator functions for range based for loops
    */
    typename std::vector< MercuryParticle<NDIMS> >::const_iterator begin() const
    {
      return storage_.begin();
    }
    
    /*!
    * \brief Iterator functions for range based for loops
    */
    typename std::vector< MercuryParticle<NDIMS> >::iterator end()
    {
      return storage_.end();
    }
    
    /*!
    * \brief Iterator functions for range based for loops
    */
    typename std::vector< MercuryParticle<NDIMS> >::const_iterator end() const
    {
      return storage_.end();
    }
    
    /*!
    * \brief Random access function into the particles.
    */
    MercuryParticle<NDIMS>& operator[](std::size_t idx)
    {
      return storage_[idx];
    }
    
    /*!
    * \brief Random access function into the particles.
    */
    const MercuryParticle<NDIMS>& operator[](std::size_t idx) const
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
    double min_[NDIMS], max_[NDIMS];
    /*!
     * Pointer to the base file, required for the backing std::ifstream
     * \todo Chris, please check if this parameter is needed; it causes a warning with [-Wunused-private-field] \author weinhartt 
     */
    MercuryDataFile *dataFile_;
    
    /*!
    * Backing storage vector used for the particles in this timestep
    */
    std::vector< MercuryParticle<NDIMS> > storage_;
    
    template<std::size_t NDIMS2>
    friend std::istream& operator>>(std::istream&, MercuryTimeStep<NDIMS2>&);
    
    friend class MercuryTimeStepIterator<NDIMS>;
    
    friend class MercuryDataFile;
};

/*!
* Reads the timestep header into the ts
* \param[in,out] in The istream where we read from
* \param[out] step The timestream read into
* \return in for chaining.
*/
template<std::size_t NDIMS>
std::istream& operator>>(std::istream& in, MercuryTimeStep<NDIMS>& step)
{
  std::size_t i;
  in >> step.numParticles_ >> step.time_;
  
  for (i = 0; i < NDIMS; i++)
    in >> step.min_[i];
    
  for (i = 0; i < NDIMS; i++)
    in >> step.max_[i];

  return in;
}

/*!
 * Lazy timestep iterator
 * Only loads the current timestep and discards this as soon as
 * it is incremented.
 * This is a ForwardIterator as described by the C++11 standard
 * This iterator invalidates any references to its dereferenced value
 * when incremented.
 * \author dducks
 */
template<std::size_t NDIMS>
class MercuryTimeStepIterator
{
  public:
    /*!
     * \brief Not-equals operator, as defined for ForwardIterators
     */
    bool operator!=(MercuryTimeStepIterator<NDIMS> other) const
    {
      return (isEOFTimeStep_ != other.isEOFTimeStep_);
    }
    
    /*!
     * \brief Dereference operator, as defined for ForwardIterators
     */
    MercuryTimeStep<NDIMS>& operator*()
    {
      return lastReadTimeStep_;
    }
    
    /*!
     * \brief Const dereference operator, as defined for constant ForwardIterators
     */
    const MercuryTimeStep<NDIMS>& operator*() const
    {
      return lastReadTimeStep_;
    }
    
    /*!
     * \brief Pre-increment operator, as defined for ForwardIterators
     * This method populates the timestep, including all the particles
     * in there. It also resizes the backing storage mechanism.
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
    MercuryTimeStep<NDIMS> lastReadTimeStep_;
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
 * \author dducks
 */
class MercuryDataFile
{
  public:
    /*!
     * Opens the Mercury 3D .data file with filename
     * file.
     * \param[in] name The filename
     */
    MercuryDataFile(std::string name)
      : file_(name)
    { }

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
     * \sa MercuryDataFile::isMercury2DDataFile()
     */
    template<std::size_t NDIMS>
    bool isMercuryDataFile()
    {
      //Store the position, so we can jump back at the end of the function..
      std::ios::pos_type currentPosition = file_.tellg();
      //and jump to the start
      file_.seekg(0);
      //get the first line
      std::string line;
      std::getline(file_, line);
      file_.seekg(currentPosition); //and pretend nothing has happened
      
      std::istringstream lineStream(line);
      
      //We'll try to find out if there were exactly enough arguments.
      MercuryTimeStep<NDIMS> step;
      lineStream >> step;
      
      //Did we reach the end yet?
      bool isValid = lineStream.good();
      double dummy;
      lineStream >> dummy;
      
      //now we should have reached it.
      isValid = isValid && !lineStream.good();
      return isValid;
    }
    
    /*!
     * Proxy class because the compiler needs more information
     * about the iterable type (namely the dimension). This can't
     * be inferred as it needs two levels of inferrence while the
     * standard allows for only one.
     */
    template<std::size_t NDIMS>
    class IteratorProxy
    {
    private:
      IteratorProxy(MercuryDataFile* pData)
        : data_(pData)
      { }
      
      MercuryDataFile* data_;
    public:
      MercuryTimeStepIterator<NDIMS> begin()
      { 
        return data_->begin<NDIMS>();
      }
      MercuryTimeStepIterator<NDIMS> end()
      {
        return data_->end<NDIMS>();
      }
      friend class MercuryDataFile;
    };
    
    template<std::size_t NDIMS>
    IteratorProxy<NDIMS> as()
    { 
      return {this};
    };
    
    /*!
     * \brief Returns a forwarditerator to the timesteps
     * Returns a forwarditerator to the timesteps,
     * invalidating any other valid iterators in the process
     * (since this is a lazy loader and does not actually
     * load the entire file in memory). End iterators do not
     * get invalidated. 
     * This function makes no guarantee for the validity of the file-state.
     */
    template<std::size_t NDIMS>
    MercuryTimeStepIterator<NDIMS> begin()
    {
      file_.seekg(0);
      return {this};
    }
    
    /*!
     * \brief Returns a forwarditerator one past the last timestep.
     */
    template<std::size_t NDIMS>
    MercuryTimeStepIterator<NDIMS> end() const
    {
      return {};
    }
  private:
    /*!
     * The backing file stream used to represent the data file.
     */
    std::ifstream file_;
    
    template<std::size_t NDIMS>
    friend class MercuryTimeStep;
    template<std::size_t NDIMS>
    friend class MercuryTimeStepIterator;
};

template<std::size_t NDIMS>
void MercuryTimeStepIterator<NDIMS>::operator++()
{
  lastReadTimeStep_.ID_++;
      
  std::string line;
  std::getline(dataFile_->file_, line);
  
  std::istringstream lineStream(line);
  
  lineStream >> lastReadTimeStep_;
  
  //I hope we didn't went beyond end of file...
  if (lineStream.eof())
  {
//   logger(WARN, "The timestep header detected an EOF.. Usually this"
//                " means that the format was not what it appeared to be."
//                "\nproceed with caution!");
  }
  //Resize the backing storage container to make sure we can actually
  //fit all the particles in there.
  lastReadTimeStep_.storage_.resize(lastReadTimeStep_.numParticles_);  
  //Well, now that we're set up, read all the particles
  for (MercuryParticle<NDIMS>& part : lastReadTimeStep_)
  {
    //line by line, because no data format can be trusted.
    std::getline(dataFile_->file_, line);
    lineStream.clear();
    lineStream.str(line);
   
    lineStream >> part;
  }
  
  if (dataFile_->file_.eof())
    isEOFTimeStep_ = true;
}

#endif