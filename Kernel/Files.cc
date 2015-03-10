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


#include <iostream>
#include "Files.h"

/*!
 * \details The constructor which sets the file precision for DataFile, FStatFile, EneFile, RestartFile, StatFile.
 */
Files::Files()
{
    //constructor();
    getDataFile().getFstream().precision(13);
    getFStatFile().getFstream().precision(13);
    getEneFile().getFstream().precision(13);
    getRestartFile().getFstream().precision(15);
    getStatFile().getFstream().precision(5);
    getStatFile().getFstream().setf(std::ios::left);
}

Files::~Files()
{
    // TODO Auto-generated destructor stub
}
/*!
* \details Also sets the name of Files to be the same as the one from which one makes a copy.
* \param[in] other (A const reference of the object to be copied)
*/
Files::Files(const Files& other)
{
    //constructor();
    setName(other.getName());
}
/*! 
 * \returns File& (A reference of object type File i.e. File& dataFile_)
 */
File& Files::getDataFile()
{
    return dataFile_;
}
/*! 
 * \returns File& (A reference of object type File i.e. File& eneFile_)
 */
File& Files::getEneFile()
{
    return eneFile_;
}
/*! 
 * \returns File& (A reference of object type File i.e. File& fStatFile_)
 */
File& Files::getFStatFile()
{
    return fStatFile_;
}
/*! 
 * \returns File& (A reference of object type File i.e. File& restartFile_)
 */
File& Files::getRestartFile()
{
    return restartFile_;
}
/*! 
 * \returns File& (A reference of object type File i.e. File& statFile_)
 */
File& Files::getStatFile()
{
    return statFile_;
}
/*! 
 * \returns const File& (A const reference of object type File i.e. const File& dataFile_)
 */
const File& Files::getDataFile() const
{
    return dataFile_;
}
/*! 
 * \returns const File& (A const reference of object type File i.e. const File& eneFile_)
 */
const File& Files::getEneFile() const
{
    return eneFile_;
}
/*! 
 * \returns const File& (A const reference of object type File i.e. const File& fstatFile_)
 */
const File& Files::getFStatFile() const
{
    return fStatFile_;
}
/*! 
 * \returns const File& (A const reference of object type File i.e. const File& restartFile_)
 */
const File& Files::getRestartFile() const
{
    return restartFile_;
}
/*! 
 * \returns const File& (A const reference of object type File i.e. const File& statFile_)
 */
const File& Files::getStatFile() const
{
    return statFile_;
}
/*! 
 * \returns const File& (A const reference of object type std::string i.e. const std::string& name_)
 */
const std::string& Files::getName() const
{
    return name_;
}
/*!
 * \param[in] saveCount
 */
void Files::setSaveCount(unsigned int saveCount)
{
    getDataFile().setSaveCount(saveCount);
    getFStatFile().setSaveCount(saveCount);
    getRestartFile().setSaveCount(saveCount);
    getStatFile().setSaveCount(saveCount);
    getEneFile().setSaveCount(saveCount);
}
/*!
 * \param[in] name
 */
void Files::setName(const std::string& name)
{
    name_ = name; // was before this->name_ = name
    getDataFile().setName(name_ + ".data");
    getFStatFile().setName(name_ + ".fstat");
    getRestartFile().setName(name_ + ".restart");
    getStatFile().setName(name_ + ".stat");
    getEneFile().setName(name_ + ".ene");
}
/*!
 * \param[in] name
 */
void Files::setName(const char* name)
{
    setName(std::string(name));
}
/*!
 * \details Calls the setFileType() function from the File.h, which basically sets the File::fileType_
 * \param[in] fileType (an object of enum class FileType)
 */
void Files::setFileType(FileType fileType)
{
    getDataFile().setFileType(fileType);
    getFStatFile().setFileType(fileType);
    getRestartFile().setFileType(fileType);
    getStatFile().setFileType(fileType);
    getEneFile().setFileType(fileType);
}

//void Files::openNextFile(const int counter)
//{
//    getDataFile().openNextFile(counter);
//    getFStatFile().openNextFile(counter);
//    getRestartFile().openNextFile(counter);
//    getStatFile().openNextFile(counter);
//    getEneFile().openNextFile(counter);
//}
/*!
 * \details This implicitly calles the setCounter() function defined in File.h
 */
void Files::resetFileCounter()
{
    getDataFile().setCounter(0);
    getFStatFile().setCounter(0);
    getRestartFile().setCounter(0);
    getStatFile().setCounter(0);
    getEneFile().setCounter(0);
}
/*!
 * \param[in] openmode 
 */
void Files::setOpenMode(std::fstream::openmode openMode)
{
    getDataFile().setOpenMode(openMode);
    getFStatFile().setOpenMode(openMode);
    getRestartFile().setOpenMode(openMode);
    getStatFile().setOpenMode(openMode);
    getEneFile().setOpenMode(openMode);
}
/*!
 * \param[in,out] is (a reference of the input stream)
 */
void Files::read(std::istream& is)
{
    std::string dummy;
    is >> dummy >> name_;
    is >> dummy >> dataFile_;
    is >> dummy >> fStatFile_;
    is >> dummy >> eneFile_;
    is >> dummy >> restartFile_;
    is >> dummy >> statFile_;
}
/*!
 * \details 
 * \param[in,out] os 
 * We pass the std::ostream& os (which is basically the file into which you want the data to be written into) as an input argument,
 * As the output stream is modified in the write function below, it is also the output variable (do not confuse \param[out] with the return variable)
 */
void Files::write(std::ostream& os) const
{
    os << " name " << name_<< std::endl;
    os << "dataFile    " << dataFile_ << std::endl;
    os << "fStatFile   " << fStatFile_ << std::endl;
    os << "eneFile     " << eneFile_ << std::endl;
    os << "restartFile " << restartFile_ << std::endl;
    os << "statFile    " << statFile_ << std::endl;
}
/*!
 *
 */
void Files::openFiles()
{
    getDataFile().open();
    getFStatFile().open();
    getRestartFile().open();
    //getStatFile().open();
    getEneFile().open();
}
/*!
 * 
 */
void Files::closeFiles()
{
    getDataFile().close();
    getFStatFile().close();
    getRestartFile().close();
    getStatFile().close();
    getEneFile().close();
}
/*!
 * \param[in] nextSavedTimeStep
 */
void Files::setNextSavedTimeStep(unsigned int nextSavedTimeStep)
{
    getDataFile().setNextSavedTimeStep(nextSavedTimeStep);
    getFStatFile().setNextSavedTimeStep(nextSavedTimeStep);
    getRestartFile().setNextSavedTimeStep(nextSavedTimeStep);
    getStatFile().setNextSavedTimeStep(nextSavedTimeStep);
    getEneFile().setNextSavedTimeStep(nextSavedTimeStep);
}
