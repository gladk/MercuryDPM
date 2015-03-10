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


#ifndef FILE_H
#define FILE_H
#include <fstream>
#include <cstdlib>

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
 * \brief The class Files contains five File objects for the data, fstat, ene, restart and stat output. FilesAndRunNumber inherits from Files.
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
    unsigned int getCounter() const;

    /*!
      * \brief Sets File::counter_
      */
    void setCounter(unsigned int counter);

    /*!
     * \brief Gets File::openMode_
     */
    std::fstream::openmode getOpenMode() const;

    /*!
     * \brief Sets File::openMode_
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
    unsigned int getNextSavedTimeStep() const;

    void setNextSavedTimeStep(unsigned int nextSavedTimeStep);

    bool saveCurrentTimestep(unsigned int ntimeSteps);

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

    bool openNextFile(std::fstream::openmode openMode);

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
     * \brief counts the number of the next file to be opened; needed if multiple files are written/read
     */
    unsigned int counter_;

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
     * \brief determines how many time steps are skipped before the next stat file is written
     */
    unsigned int nextSavedTimeStep_;
};

#endif /* FILE_H */
