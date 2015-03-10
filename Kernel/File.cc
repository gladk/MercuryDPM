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


#include "File.h"
#include<sstream>
#include<iostream>

///\todo TW: I am not able to define the operator >> for enum class type (the implementation works fine for the old enum)
// ///Allows to read a FileType using the operator>>, e.g., is >> fileType;
// ///It converts unsigned int to the enum FileType
//std::istream& operator >>(std::istream& is, FileType& i)
//{
//    unsigned int tmp;
//    if (is >> tmp)
//        i = static_cast<FileType>(tmp);
//    return is;
//}
//
// ///Allows to write a FileType using the operator<<, e.g., os << fileType;
// ///It converts the enum FileType to unsigned int
//std::ostream& operator <<(std::ostream& is, FileType& i)
//{
//    is << static_cast<unsigned int>(i);
//    return is;
//}
/*!
 * \param[in,out] os output stream to which the fileType is written
 * \param[in] fileType the fileType that has to be written to the output stream
 * \return the output stream "os" that is returned after adding the fileType string
 */
std::string to_string_padded(unsigned int value)
{
    std::string str;
    if (value < 10)
        str="000"+std::to_string(value);
    else if (value < 100)
        str="00"+std::to_string(value);
    else if (value < 1000)
        str="0"+std::to_string(value);
    else 
        str=std::to_string(value);
    return str;
}

std::ostream& operator<<(std::ostream&os, FileType fileType)
{
    if (fileType == FileType::NO_FILE)
        os << "NO_FILE";
    else if (fileType == FileType::ONE_FILE)
        os << "ONE_FILE";
    else if (fileType == FileType::MULTIPLE_FILES)
        os << "MULTIPLE_FILES";
    else if (fileType == FileType::MULTIPLE_FILES_PADDED)
        os << "MULTIPLE_FILES_PADDED";
    else
    {
        std::cerr << "FileType not recognized" << std::endl;
        exit(-1);
    }
    return os;
}

/*!
 * \param[in,out] is The input stream from which the fileType is read
 * \param[in] fileType The fileType that has to be read from the input stream
 * \return the input stream "is" (that is returned after the fileType string is read out)
 */
std::istream& operator>>(std::istream&is, FileType&fileType)
{
    std::string fileTypeString;
    is >> fileTypeString;
    if (!fileTypeString.compare("NO_FILE"))
        fileType = FileType::NO_FILE;
    else if (!fileTypeString.compare("ONE_FILE"))
        fileType = FileType::ONE_FILE;
    else if (!fileTypeString.compare("MULTIPLE_FILES"))
        fileType = FileType::MULTIPLE_FILES;
    else if (!fileTypeString.compare("MULTIPLE_FILES_PADDED"))
        fileType = FileType::MULTIPLE_FILES_PADDED;
    else
    {
        std::cerr << "FileType not recognized" << std::endl;
        exit(-1);
    }
    return is;
}

/*!
 * \details A File constructor which initialises FILE::saveCount_=0, sets the default filename as FILE::name_ = "", sets the 
 * default FileType to be as FileType::ONE_FILE and few other variables as listed below.   
 */
File::File()
{
    saveCount_ = 0;

    // file name has to be set by the user
    name_ = "";
    
    // file closed by default
    
    // output into a single file by default
    fileType_ = FileType::ONE_FILE;
    
    // file counter set to 0 by default
    counter_ = 0;
    nextSavedTimeStep_ = 0;

    // file output file by default
    openMode_ = std::fstream::out;
}

/*!
 * 
 */
File::~File()
{
}

/*!
 * \details Returns fstream (file stream) of any file for input and output tasks.
 * \return std::fstream&
 */
std::fstream& File::getFstream()
{
    return fstream_;
}

//void File::setFstream(const std::fstream& file)
//{
//    this->fstream_ = file;
//}
/*!
 * \return Returns a constant reference of type const std::string&
 */
const std::string& File::getName() const
{
    return name_;
}
/*!
 * \details In case of FileType:fileType_== multiple files or multiple files padded, multiple files are generated and are named as problem.data.0, problem.data.1 or problem.data.0000, problem.data.0001 
 * \return Returns a constant of type std::string
 */
const std::string File::getFullName() const
{
    //get the full file name
    std::stringstream fullFileName("");
    fullFileName << name_;
    if (getFileType() == FileType::MULTIPLE_FILES)
    {
        fullFileName << "." << getCounter();
    }
    else if (getFileType() == FileType::MULTIPLE_FILES_PADDED)
    {
        fullFileName << ".";
        if (getCounter() < 1000)
            fullFileName << "0";
        if (getCounter() < 100)
            fullFileName << "0";
        if (getCounter() < 10)
            fullFileName << "0";
        fullFileName << getCounter();
    }
    return fullFileName.str();
}
/*!
 * \param[in] name (Takes in the to be name of the File)
 */
void File::setName(const std::string& name)
{
    this->name_ = name;
}
/*! 
 * \return Returns the FileType (File::fileType_)
 */
FileType File::getFileType() const
{
    return fileType_;
}
/*!
 * \param[in] fileType
 */
void File::setFileType(FileType fileType)
{
    fileType_ = fileType;
}
/*!
 * \return unsigned int counter_
 */
unsigned int File::getCounter() const
{
    return counter_;
}
/*!
 * \param[in] counter
 */
void File::setCounter(unsigned int counter)
{
    counter_ = counter;
}
/*!
 * \return bool (True or False)
 */
bool File::openNextFile()
{
    //++counter_;

    if (getFileType() == FileType::MULTIPLE_FILES || getFileType() == FileType::MULTIPLE_FILES_PADDED)
    {
        File::close();
        return File::open();
    }
//    else if (getFileType() == FileType::ONE_FILE && !getFstream().is_open())
//    {
//        return File::open();
//    }
    else
    {
        return true;
    }
}
/*!
 * \param[in] openmode 
 * \return bool
 */
bool File::openNextFile(std::fstream::openmode openMode)
{
    setOpenMode(openMode);
    return openNextFile();
}
/*!
 * \return std::fstream::openmode
 */
std::fstream::openmode File::getOpenMode() const
{
    return openMode_;
}
/*!
 * \param[in] openmode
 */
void File::setOpenMode(std::fstream::openmode openMode)
{
    openMode_ = openMode;
}
/*!
 * \return unsigned int saveCount_
 */
unsigned int File::getSaveCount() const
{
    return saveCount_;
}
/*!
 * \details File::setSaveCount assigns the number of time steps to be skipped before the data is written to an existing file or a new file.
 * \param[in] saveCount 
 */
void File::setSaveCount(unsigned int saveCount)
{
    saveCount_ = saveCount;
}
/*!
 * \details Returns the time step at which the next write or read operation has to happen
 * \return unsigned int nextSaveTimeStep_
 */
unsigned int File::getNextSavedTimeStep() const
{
    return nextSavedTimeStep_;
}
/*!
 * \details Allows one to set the time step at which the next write or read operation has to happen
 * \param[in] nextSavedTimeStep
 */
void File::setNextSavedTimeStep(unsigned int nextSavedTimeStep)
{
    nextSavedTimeStep_ = nextSavedTimeStep;
}
/*!
 * \details 
 * \param[in] ntimeSteps
 * \return True or False (a bool)
 */
bool File::saveCurrentTimestep(unsigned int ntimeSteps)
{
    //check if this timestep should be written, if the file type is not NO_FILE, then open the file and check if the file was successfully opened
    //std::cout << (ntimeSteps>=nextSavedTimeStep_) << " " <<  (getFileType()!= FileType::NO_FILE) << std::endl;
    return ntimeSteps>=nextSavedTimeStep_ && getFileType()!= FileType::NO_FILE && open();
}
/*!
 * \details Returns a bool to check if the file is open or closed. It also increments the nextSavedTimeStep with the saveCount
 * \return bool (True or False)
 * \bug Deepak checked by using fstream_.fail() instead of !fstrea_.is_open(), however this breaks selftests, Thomas will look at this
 */
bool File::open()
{  
    if (getName().compare("") == 0)
    {
        std::cerr << "Error: Name must be set before opening file" << std::endl;
        throw;
    }

    //close old file if multi-file output
    if (fileType_==FileType::MULTIPLE_FILES||fileType_==FileType::MULTIPLE_FILES_PADDED)
        fstream_.close();
    
    //open new file for multi-file output
    
    if (!fstream_.is_open())
    {
        fstream_.open(getFullName().c_str(), openMode_);
        if (!fstream_.is_open())
        {
            std::cerr << "Error in opening " << getFullName() <<" with open mode "<< openMode_ << std::endl;
            return false;
        }
    }
    ///\todo tw: DEBUG_OUTPUT is currently only defined in DPMBase.h
    #ifdef DEBUG_OUTPUT
        std::cout << "open " << getFullName() << std::endl;
    #endif
    nextSavedTimeStep_ += saveCount_;
    counter_++;  
    return true;
}
/*!
 * \param[in] openMode
 */
bool File::open(std::fstream::openmode openMode)
{
    setOpenMode(openMode);
    return open();
}
/*!
 *
 */
void File::close()
{
    fstream_.close();
    //std::cerr << "Closing " << getFullName() << std::endl;
}
/*!
 * \details Read function, which accepts an input stream object as input and assigns the member variables i.e. name_, fileType_,
 * saveCount_, counter_ and nextSavedTimeStep_
 * \param[in,out] is
 */
void File::read(std::istream& is)
{
    std::string dummy;
    is >> dummy;
    if (!dummy.compare("name"))
        is >> name_ >> dummy;
    is >> fileType_;
    is >> dummy >> saveCount_;
    is >> dummy >> counter_;
    is >> dummy >> nextSavedTimeStep_;
}
/*!
 * \details BaseParticle print function, which accepts an output stream object as input 
 *  and writes the info to the std::ostream
 * \param[in,out] os
 */
void File::write(std::ostream& os) const
{
    //only write name if it differs from the default name
    if (getFullName().compare(name_))
        os << "name " << name_ << " ";
    os << "fileType " << fileType_;
    os << " saveCount " << saveCount_;
    os << " counter " << counter_;
    os << " nextSavedTimeStep " << nextSavedTimeStep_;
    ///\todo TW: openMode_ is not saved, maybe it should not even be stored but set every time you open a file
}
/*!
 * \param[in,out] os 
 * \param[in] o
 * \return std::ostream& os
 */
std::ostream& operator <<(std::ostream& os, const File& o)
{
    o.write(os);
    return os;
}
/*!
 * \param[in,out] is
 * \param[in] o
 * \return std::istream&
 */
std::istream& operator >>(std::istream& is, File &o)
{
    o.read(is);
    return (is);
}
