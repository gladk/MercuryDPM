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

#ifndef FILE_H
#define FILE_H
#include <fstream>

/*!
 * \brief FileType is used to define how files are opened random fixed-particle bottom
 */
enum class FileType : unsigned char
{
    NO_FILE = 0,
    ONE_FILE = 1,
    MULTIPLE_FILES = 2,
    MULTIPLE_FILES_PADDED = 3
};

std::ostream& operator<<(std::ostream&os, FileType fileType);
std::istream& operator>>(std::istream&is, FileType&fileType);


/*!
 * \class File
 * \brief The class Files contains five File objects for the data, fstat, ene, restart and stat output. STD_save inherits from Files.
 */
class File
{
public:
    
//constructors
    /*!
     * \brief default constructor
     */
    File();

    /*!
     * \brief destructor
     */
    virtual ~File();

//setters and getters
    /*!
     * \brief
     */
    std::fstream& getFstream();

    /*!
     * \brief Returns the file name (with the file counter added, e.g., "problem.data")
     */
    const std::string& getName() const;

    /*!
     * \brief Returns the full file name (with the file counter added, e.g., "problem.data.0000")
     */
    const std::string getFullName() const;

    /*!
     * \brief Sets the file name, e.g. "problem.data". 
     * \details Note the file name is usually set by MD::setName(), which sets names for all file types (data, restart, fstat, stat, ene)
     */
    void setName(const std::string& name);

    /*!
     * \brief Gets File::fileType_
     */
    FileType getFileType() const;

    /*!
     * \brief Sets File::fileType_
     */
    void setFileType(FileType fileType);

    /*!
     * \brief Gets File::counter_
     */
    int getCounter() const;

    /*!
     * \brief Is called at the beginning of solve to set File::counter_ back to -1
     */
    void resetCounter();

    /*!
     * \brief Sets File::openMode_
     */
    std::fstream::openmode getOpenMode() const;

    /*!
     * \brief Gets File::openMode_
     */
    void setOpenMode(std::fstream::openmode openMode);

    /*!
     * \brief Gets File::openMode_
     */
    unsigned int getSaveCount() const;

    /*!
     * \brief Sets File::openMode_
     */
    void setSaveCount(unsigned int saveCount);

    /*!
     * \brief Gets File::openMode_
     */
    bool getSaveCurrentTimestep() const;

    /*!
     * \brief Sets File::openMode_
     */
    void setSaveCurrentTimestep(bool saveCurrentTimestep);

    /*!
     * \brief read function, which accepts an std::stringstream as input.
     */
    void read(std::istream& is);


    /*!
     * \brief print function, which accepts an std::stringstream as input.
     */
    void write(std::ostream& os) const;

    friend std::ostream& operator <<(std::ostream& os, const File& o);

    friend std::istream& operator >>(std::istream& is, File &o);

//member functions (other than set/get)
    
    /*!
     * \brief Opens the file by calling fstream_.open()
     */
    bool open();

    /*!
     * \brief First calls setOpenMode(openMode), then open(). Used to simplify the syntax.
     */
    bool open(std::fstream::openmode openMode);

    /*!
     * \brief This function should be called before a timestep is written, as it opens a new file in case multiple files are used
     */
    bool openNextFile();

    /*!
     * \brief Closes the file by calling fstream_.close()
     */
    void close();

private:
    /*!
     * \brief File name; by default they are derived from problem_name.
     */
    std::string name_;

    /*!
     * \brief Stream object used to read/write data files
     */
    std::fstream fstream_;

    /*!
     * \brief fileType_ indicates how files are opened
     * \details - FileType::NO_FILE: file will not be created/read\n
     * - FileType::ONE_FILE: all data will be written into/ read from a single file called name_\n
     * - FileType::MULTIPLE_FILES: each timestep will be written into/read from separate files numbered consecutively: name_.0, name_.1, ..\n
     * - FileType::MULTIPLE_FILES: each timestep will be written into/read from separate files numbered consecutively, with numbers padded by zeros to a minimum of four digits: name_.0000, name_.0001, ..
     */
    FileType fileType_;

    /*!
     * \brief Counter needed if multiple files are written/read (-1 by default; as it is increased before files are written)
     */
    int counter_;

    /*!
     * \brief Indicates how the file should be opened (std::fstream::out by default)
     * \details - std::fstream::out for output files \n
     * - std::fstream::out||std::fstream::app for appended output files\n
     * - std::fstream::in for input files
     */
    std::fstream::openmode openMode_;

    /*!
     * \brief determines how many time steps are skipped before the next stat file is written
     */
    unsigned int saveCount_;

    /*!
     * \brief determines if the current timestep is written into the data file
     */
    bool saveCurrentTimestep_;

};

#endif /* FILE_H */
