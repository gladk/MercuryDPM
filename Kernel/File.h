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
 * \brief With FileType options, one is able to choose if data is to be read/written from/into no or single or multiple files.
 */
enum class FileType : unsigned char
{
    /*!
     * \brief file will not be created/read\n
     */
    NO_FILE = 0,
    /*!
     * \brief all data will be written into/ read from a single file called name_\n
     */
    ONE_FILE = 1,
    /*!
     * \brief each time-step will be written into/read from separate files numbered consecutively: name_.0, name_.1, .. so on \n
     */
    MULTIPLE_FILES = 2,
    /*!
     * \brief each time-step will be written into/read from separate files numbered consecutively, with numbers padded by zeros to a minimum of four digits: name_.0000, name_.0001, ..
     */
    MULTIPLE_FILES_PADDED = 3
};



std::string to_string_padded(unsigned int value);


/*!
 * \brief Writes the FileType as a human-readable string into the output stream 'os' 
 */
std::ostream& operator<<(std::ostream&os, FileType fileType);

/*!
 * \brief Reads the FileType from an input stream 'is'
 */
std::istream& operator>>(std::istream&is, FileType&fileType);


/*!
 * \class File 
 * The instance of class File, for e.g. member variables of class Files (objects of type File) in Files.h, allows to manipulate the the input and output stream.
 * In layman terms: In MercuryDPM, in order to read and write data into or from files we use the methods defined in Files.h. These methods implicitly
 * calls the methods of class File and thereby allowing us to write or read data. 
 * \brief 
 */
class File
{
public:
    
//constructors
    /*!
     * \brief constructor
     */
    File();

    /*!
     * \brief destructor
     */
    virtual ~File();

    //set and get functions
    /*!
     * \brief Allows to access the member variable File::fstream_
     */
    std::fstream& getFstream();

    /*!
     * \brief Allows to access the file name, e.g., "problem.data".
     */
    const std::string& getName() const;

    /*!
     * \brief Also allows to access the file name, however with additional information which is the file counter, e.g., "problem.data.0000"
     */
    const std::string getFullName() const;

    /*!
     * \brief Sets the file name, e.g. "Name.data". 
     * \details Note the file name is usually set by MD::setName(), which sets names for all file types (data, restart, fstat, stat, ene)
     */
    void setName(const std::string& name);

    /*!
     * \brief Gets the file type e.g. NOFILE, ONEFILE and MULTIPLE FILES. File::fileType_
     */
    FileType getFileType() const;

    /*!
     * \brief Sets the type of file needed to write into or read from. File::fileType_
     */
    void setFileType(FileType fileType);

    /*!
     * \brief In case of multiple files, File::getCounter() returns the the number (FILE::Counter_) of the next file i.e. to be opened for reading or writing; NOTE: needed only if FILE::fileType_ is multiple files
     */
    unsigned int getCounter() const;

    /*!
      * \brief Allows the user to set the file counter according to his need. Sets File::counter_.
      */
    void setCounter(unsigned int counter);

    /*!
     * \brief Allows the user to know the file mode i.e. gets File::openMode_
     */
    std::fstream::openmode getOpenMode() const;

    /*!
     * \brief Allows the user to Sets File::openMode_
     */
    void setOpenMode(std::fstream::openmode openMode);

    /*!
     * \brief Gets File::saveCount_
     */
    unsigned int getSaveCount() const;

    /*!
     * \brief Sets File::saveCount_
     */
    void setSaveCount(unsigned int saveCount);

    /*!
     * \brief Gets File::nextSavedTimeStep_
     */
    unsigned int getNextSavedTimeStep() const;

    /*!
    * \brief Sets File::nextSavedTimeStep_
    */
    void setNextSavedTimeStep(unsigned int nextSavedTimeStep);

    /*!
     * \brief
     */
    bool saveCurrentTimestep(unsigned int ntimeSteps);

    /*!
     * \brief read function, which accepts an input stream <a href="http://en.cppreference.com/w/cpp/io/basic_istreams.html">std::istream</a>. 
     */
    void read(std::istream& is);

    /*!
     * \brief print function, which accepts an std::stringstream as input.
     */
    void write(std::ostream& os) const;
    
    /*!
    * \brief Operator overloading used to write data obtained from an object of class File into an output stream.
    * It also returns a reference to the output stream.
    */
    friend std::ostream& operator <<(std::ostream& os, const File& o);

    /*!
     * \brief Operator overloading used to read data from the input stream into member variables of an object of class File. It also returns a reference
     * of type std::istream&
     */
    friend std::istream& operator >>(std::istream& is, File &o);

//member functions (other than set/get)
    
    /*!
     * \brief Checks if the file stream fstream_ has any issues while opening. Alongside, it also increments the nextSavedTimeStep and the counter.
     * the counter is useful when the fileType_ is set to Multiple or Multiple with padded. 
     */
    bool open();

    /*!
     * \brief First calls setOpenMode(openMode), then open().
     */
    bool open(std::fstream::openmode openMode);

    /*!
     * \brief This function should be called before a data corresponding to the new time step is written or read.
     *  It is essential in case of multiple files, as it opens a new file for every increment in the File::counter_ 
     */
    bool openNextFile();

    /*!
     * \brief Similar to File::openNextFile(), but also lets the user set the mode of opening, see <a href="http://en.cppreference.com/w/cpp/io/basic_fstreams.html">std::fstream::openmode</a>
     */
    bool openNextFile(std::fstream::openmode openMode);

    /*!
     * \brief Closes the file by calling fstream_.close()
     */
    void close();

private:
    /*!
     * \brief  name of the file.
     */
    std::string name_;

    /*!
     * \brief Stream object used to read/write data files
     */
    std::fstream fstream_;

    /*!
     * \brief fileType_ indicates the type of the files. Whether it is No file, one file or multiple file as described in the
     * description of the class.
     */
    FileType fileType_;

    /*!
     * \brief counts the number of the next file to be opened; needed if multiple files are written/read
     */
    unsigned int counter_;

    /*!
     * \brief A variable to indicate how the file should be opened i.e. in, out, ... see http://en.cppreference.com (std::fstream::out by default)  
     */
    std::fstream::openmode openMode_;
   
    /*!
     * \brief Allows one to define the number of timesteps to be skipped to make a snap shot. E.g. TMax = 100, saveCount_ = 10, timeStep = 1; It stores data at t={0,10,20,30,40...100}.
     * And if TMax =101, it stores data at t={0,10,20,30,...100,101}
     */
    unsigned int saveCount_;

    /*!
     * \brief the time step at which the next write or read operation has to happen.
     */
    unsigned int nextSavedTimeStep_;
};

#endif /* FILE_H */
