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

#include <iostream>
#include "Files.h"

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

Files::Files(const Files& other)
{
    //constructor();
    setName(other.getName());
}

File& Files::getDataFile()
{
    return dataFile_;
}

File& Files::getEneFile()
{
    return eneFile_;
}

File& Files::getFStatFile()
{
    return fStatFile_;
}

File& Files::getRestartFile()
{
    return restartFile_;
}

File& Files::getStatFile()
{
    return statFile_;
}
const File& Files::getDataFile() const
{
    return dataFile_;
}

const File& Files::getEneFile() const
{
    return eneFile_;
}

const File& Files::getFStatFile() const
{
    return fStatFile_;
}

const File& Files::getRestartFile() const
{
    return restartFile_;
}

const File& Files::getStatFile() const
{
    return statFile_;
}

const std::string& Files::getName() const
{
    return name_;
}

void Files::setSaveCount(unsigned int saveCount)
{
    getDataFile().setSaveCount(saveCount);
    getFStatFile().setSaveCount(saveCount);
    getRestartFile().setSaveCount(saveCount);
    getStatFile().setSaveCount(saveCount);
    getEneFile().setSaveCount(saveCount);
}

void Files::setName(const std::string& name)
{
    this->name_ = name;
    getDataFile().setName(name_ + ".data");
    getFStatFile().setName(name_ + ".fstat");
    getRestartFile().setName(name_ + ".restart");
    getStatFile().setName(name_ + ".stat");
    getEneFile().setName(name_ + ".ene");
}

void Files::setName(const char* name)
{
    setName(std::string(name));
}

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

void Files::resetFileCounter()
{
    getDataFile().setCounter(0);
    getFStatFile().setCounter(0);
    getRestartFile().setCounter(0);
    getStatFile().setCounter(0);
    getEneFile().setCounter(0);
}

void Files::setOpenMode(std::fstream::openmode openMode)
{
    getDataFile().setOpenMode(openMode);
    getFStatFile().setOpenMode(openMode);
    getRestartFile().setOpenMode(openMode);
    getStatFile().setOpenMode(openMode);
    getEneFile().setOpenMode(openMode);
}

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

///BaseParticle print function, which accepts an os std::stringstream as input. It prints human readable BaseParticle information to the std::stringstream
void Files::write(std::ostream& os) const
{
    os << " name " << name_<< std::endl;
    os << "dataFile " << dataFile_ << std::endl;
    os << "fStatFile " << fStatFile_ << std::endl;
    os << "eneFile " << eneFile_ << std::endl;
    os << "restartFile " << restartFile_ << std::endl;
    os << "statFile " << statFile_ << std::endl;
}

void Files::openFiles()
{
    getDataFile().open();
    getFStatFile().open();
    getRestartFile().open();
    //getStatFile().open();
    getEneFile().open();
}

void Files::closeFiles()
{
    getDataFile().close();
    getFStatFile().close();
    getRestartFile().close();
    getStatFile().close();
    getEneFile().close();
}

void Files::setNextSavedTimeStep(unsigned int nextSavedTimeStep)
{
    getDataFile().setNextSavedTimeStep(nextSavedTimeStep);
    getFStatFile().setNextSavedTimeStep(nextSavedTimeStep);
    getRestartFile().setNextSavedTimeStep(nextSavedTimeStep);
    getStatFile().setNextSavedTimeStep(nextSavedTimeStep);
    getEneFile().setNextSavedTimeStep(nextSavedTimeStep);
}
