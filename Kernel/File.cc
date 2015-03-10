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

File::~File()
{
}

///not fully const
std::fstream& File::getFstream()
{
    return fstream_;
}

//void File::setFstream(const std::fstream& file)
//{
//    this->fstream_ = file;
//}

const std::string& File::getName() const
{
    return name_;
}

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

void File::setName(const std::string& name)
{
    this->name_ = name;
}

FileType File::getFileType() const
{
    return fileType_;
}

void File::setFileType(FileType fileType)
{
    this->fileType_ = fileType;
}

unsigned int File::getCounter() const
{
    return counter_;
}

void File::setCounter(unsigned int counter)
{
    counter_ = counter;
}

bool File::openNextFile()
{
    ++counter_;
    
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


std::fstream::openmode File::getOpenMode() const
{
    return openMode_;
}

void File::setOpenMode(std::fstream::openmode openMode)
{
    openMode_ = openMode;
}

unsigned int File::getSaveCount() const
{
    return saveCount_;
}

void File::setSaveCount(unsigned int saveCount)
{
    saveCount_ = saveCount;
}

unsigned int File::getNextSavedTimeStep() const
{
    return nextSavedTimeStep_;
}

void File::setNextSavedTimeStep(unsigned int nextSavedTimeStep)
{
    nextSavedTimeStep_ = nextSavedTimeStep;
}

bool File::saveCurrentTimestep(unsigned int ntimeSteps)
{
    //check if this timestep should be written, if the file type is not NO_FILE, then open the file and check if the file was successfully opened
    //std::cout << (ntimeSteps>=nextSavedTimeStep_) << " " <<  (getFileType()!= FileType::NO_FILE) << std::endl;
    return ntimeSteps>=nextSavedTimeStep_ && getFileType()!= FileType::NO_FILE && open();
}


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
	///bug{DK we should check for fstream_.fail() however this break selftest, Thomas will ook at this}
    if (!fstream_.is_open())
    {
        fstream_.open(getFullName().c_str(), openMode_);
        if (!fstream_.is_open())
        {
            std::cerr << "Error in opening " << getFullName() << std::endl;
            return false;
        }
    }

    nextSavedTimeStep_ += saveCount_;
    counter_++;
    return true;
}

bool File::open(std::fstream::openmode openMode)
{
    setOpenMode(openMode);
    return open();
}

void File::close()
{
    fstream_.close();
    //std::cerr << "Closing " << getFullName() << std::endl;
}

void File::read(std::istream& is)
{
    std::string dummy;
    is >> dummy >> name_;
    is >> dummy >> fileType_;
    is >> dummy >> saveCount_;
    is >> dummy >> counter_;
    is >> dummy >> nextSavedTimeStep_;
}

///BaseParticle print function, which accepts an os std::stringstream as input. It prints human readable BaseParticle information to the std::stringstream
void File::write(std::ostream& os) const
{
    os << "name " << name_;
    os << " fileType " << fileType_;
    os << " saveCount " << saveCount_;
    os << " counter " << counter_;
    os << " nextSavedTimeStep " << nextSavedTimeStep_;
}

std::ostream& operator <<(std::ostream& os, const File& o)
{
    o.write(os);
    return os;
}

std::istream& operator >>(std::istream& is, File &o)
{
    o.read(is);
    return (is);
}
