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
    dataFile.getFstream().precision(13);
    fStatFile.getFstream().precision(13);
    eneFile.getFstream().precision(13);
    restartFile.getFstream().precision(15);
    statFile.getFstream().precision(5);
    statFile.getFstream().setf(std::ios::left);
}

Files::~Files()
{

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
 * \returns File& (A reference of object type File i.e. File& dataFile)
 */
File& Files::getDataFile()
{
    return dataFile;
}
/*! 
 * \returns File& (A reference of object type File i.e. File& eneFile)
 */
File& Files::getEneFile()
{
    return eneFile;
}
/*! 
 * \returns File& (A reference of object type File i.e. File& fStatFile)
 */
File& Files::getFStatFile()
{
    return fStatFile;
}
/*! 
 * \returns File& (A reference of object type File i.e. File& restartFile)
 */
File& Files::getRestartFile()
{
    return restartFile;
}
/*! 
 * \returns File& (A reference of object type File i.e. File& statFile)
 */
File& Files::getStatFile()
{
    return statFile;
}
/*! 
 * \returns const File& (A const reference of object type File i.e. const File& dataFile)
 */
const File& Files::getDataFile() const
{
    return dataFile;
}

/*! 
 * \returns const File& (A const reference of object type File i.e. const File& eneFile)
 */
const File& Files::getEneFile() const
{
    return eneFile;
}
/*! 
 * \returns const File& (A const reference of object type File i.e. const File& fStatFile)
 */
const File& Files::getFStatFile() const
{
    return fStatFile;
}
/*! 
 * \returns const File& (A const reference of object type File i.e. const File& restartFile)
 */
const File& Files::getRestartFile() const
{
    return restartFile;
}
/*! 
 * \returns const File& (A const reference of object type File i.e. const File& statFile)
 */
const File& Files::getStatFile() const
{
    return statFile;
}
/*! 
 * \returns const File& (A const reference of object type std::string i.e. const std::string& name_)
 */
const std::string& Files::getName() const
{
    return name_;
}
/*!
 * \details sets the number of time steps skipped between each save for ALL data files.
 */
void Files::setSaveCount(unsigned int saveCount)
{
    dataFile.setSaveCount(saveCount);
    fStatFile.setSaveCount(saveCount);
    restartFile.setSaveCount(saveCount);
    statFile.setSaveCount(saveCount);
    eneFile.setSaveCount(saveCount);
}
/*!
 * \param[in] name
 */
void Files::setName(const std::string& name)
{
    name_ = name; // was before this->name_ = name
    dataFile.setName(name_ + ".data");
    fStatFile.setName(name_ + ".fstat");
    restartFile.setName(name_ + ".restart");
    statFile.setName(name_ + ".stat");
    eneFile.setName(name_ + ".ene");
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
    dataFile.setFileType(fileType);
    fStatFile.setFileType(fileType);
    restartFile.setFileType(fileType);
    statFile.setFileType(fileType);
    eneFile.setFileType(fileType);
}

//void Files::openNextFile(const int counter)
//{
//    getDataFile().openNextFile(counter);
//    fStatFile.openNextFile(counter);
//    restartFile.openNextFile(counter);
//    statFile.openNextFile(counter);
//    getEneFile().openNextFile(counter);
//}
/*!
 * \details This implicitly calls the setCounter() function defined in File.h
 */
void Files::resetFileCounter()
{
    dataFile.setCounter(0);
    fStatFile.setCounter(0);
    restartFile.setCounter(0);
    statFile.setCounter(0);
    eneFile.setCounter(0);
}
/*!
 * \param[in] openmode 
 */
void Files::setOpenMode(std::fstream::openmode openMode)
{
    dataFile.setOpenMode(openMode);
    fStatFile.setOpenMode(openMode);
    restartFile.setOpenMode(openMode);
    statFile.setOpenMode(openMode);
    eneFile.setOpenMode(openMode);
}
/*!
 * \param[in,out] is (a reference of the input stream)
 */
void Files::read(std::istream& is)
{
    std::string dummy;
    is >> dummy >> name_;
    is >> dummy >> dataFile;
    is >> dummy >> fStatFile;
    is >> dummy >> eneFile;
    is >> dummy >> restartFile;
    is >> dummy >> statFile;
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
    os << "dataFile    " << dataFile << std::endl;
    os << "fStatFile   " << fStatFile << std::endl;
    os << "eneFile     " << eneFile << std::endl;
    os << "restartFile " << restartFile << std::endl;
    os << "statFile    " << statFile << std::endl;
}
/*!
 *
 */
void Files::openFiles()
{
    dataFile.open();
    fStatFile.open();
    restartFile.open();
    //statFile.open();
    eneFile.open();
}
/*!
 * 
 */
void Files::closeFiles()
{
    dataFile.close();
    fStatFile.close();
    restartFile.close();
    statFile.close();
    eneFile.close();
}
/*!
 * \param[in] nextSavedTimeStep
 */
void Files::setNextSavedTimeStep(unsigned int nextSavedTimeStep)
{
    dataFile.setNextSavedTimeStep(nextSavedTimeStep);
    fStatFile.setNextSavedTimeStep(nextSavedTimeStep);
    restartFile.setNextSavedTimeStep(nextSavedTimeStep);
    statFile.setNextSavedTimeStep(nextSavedTimeStep);
    eneFile.setNextSavedTimeStep(nextSavedTimeStep);
}
