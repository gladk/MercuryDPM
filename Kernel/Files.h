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


#ifndef OUTPUTFILES_H_
#define OUTPUTFILES_H_
#include "File.h"

/*!
 * \class Files
 * Files is defined in the header Files.h, which includes definitions from another header called "File.h"
 * \brief Every simulation requires data files to store all the information necessary for visualisation and analysis.
 *  The class File provides an fstream variable for reading/writing i.e. input and output.
 * \details The class also allows one to store additional information like \n
 * - the name of the file,\n
 * - the type of the file (no file or one file or multiple files), File::fileType_, and\n
 * - the opening mode which basically defines if one is using the file for reading or writing purposes (input/ output/ appended output).
 */
class Files
{
public:
    
//Constructors
    /*!
     * \brief A constructor
     */    
    Files();

    /*!
     * \brief A destructor, watch out its a virtual destructor.
     */
    virtual ~Files();

    /*!
     * \brief Copy constructor
     * \todo add const? I believe it has been added.
     */
    Files(const Files& other);
            
    //setters and getters
    
    /*!
     * \brief The non const version. Allows one to edit the File::dataFile_
     */
    File& getDataFile();

    /*!
     * \brief The non const version. Allows to edit the File::eneFile_
     */
    File& getEneFile();

    /*!
     * \brief The non const version. Allows to edit the File::fstatFile_
     */
    File& getFStatFile();

    /*!
     * \brief The non const version. Allows to edit the File::restartFile_
     */
    File& getRestartFile();

    /*!
     * \brief The non const version. Allows to edit the File::statFile_
     */
    File& getStatFile();
    
    /*!
     * \brief The const version. Does not allow for any editing of the File::dataFile_
     */
    const File& getDataFile() const;

    /*!
     * \brief The const version. Does not allow for any editing of the File::eneFile_
     */
    const File& getEneFile() const;

    /*!
     * \brief The const version. Does not allow for any editing of the File::fstatFile_
     */
    const File& getFStatFile() const;

    /*!
     * \brief The const version. Does not allow for any editing of the File::restartFile_
     */
    const File& getRestartFile() const;

    /*!
     * \brief The const version. Does not allow for any editing of the File::statFile_
     */
    const File& getStatFile() const;

    /*!
     * \brief Returns the name of the file. Does not allow to change it though.
     */
    const std::string& getName() const;

    /*!
     * \brief Allows to set the name of all the files (ene, data, fstat, restart, stat)
     */
    void setName(const std::string& name);

    /*!
     * \brief Calls setName(std::string)
     */
    void setName(const char* name);

    /*!
     * \brief Sets File::saveCount_ for all files (ene, data, fstat, restart, stat)
     */
    void setSaveCount(unsigned int saveCount);

    /*!
     * \brief Sets File::fileType_ for all files (ene, data, fstat, restart, stat)
     */
    void setFileType(FileType fileType);

    /*!
     * \brief Sets File::openMode_ for all files (ene, data, fstat, restart, stat)
     */
    void setOpenMode(std::fstream::openmode openMode);

    //other member functions
    
    /*!
     * \brief Resets the file counter for each file i.e. for ene, data, fstat, restart, stat)
     */
    void resetFileCounter();
    /*!
     * \brief Extracts data from the input stream (which is basically a file you want to read from) into name_, restartFile_ ....
     */
    void read(std::istream& is);
    /*!
    * \brief Writes data into a file from the member variables name_, restartFile_, dataFile_ etc.
    */
    void write(std::ostream& os) const;
    /*!
    * \brief Opens all the files (ene, data, fstat, restart, stat) for reading and writing purposes.
    */
    void openFiles();
    /*!
    * \brief Closes all files (ene, data, fstat, restart, stat) that were opened to read or write.
    */
    void closeFiles();
    /*!
    * \brief Sets the next time step for all the files (ene, data, fstat, restart, stat) at which the data is to be written or saved.
    */
    void setNextSavedTimeStep(unsigned int nextSavedTimeStep);

private:
    /*!
     * \brief An instance of class File to handle in- and output into a .data file
     */
    File dataFile_;

    /*!
     * \brief An instance of class File to handle in- and output into a .fstat file
     */
    File fStatFile_;

    /*!
     * \brief An instance of class File to handle in- and output into a .ene file
     */
    File eneFile_;

    /*!
     * \brief An instance of class File to handle in- and output into a .restart file
     */
    File restartFile_;

    /*!
     * \brief An instance of class File to handle in- and output into a .stat file
     */
    File statFile_;

    /*!
     * \brief the name of the problem, used, e.g., for the files
     */
    std::string name_;
    
};

#endif /* OUTPUTFILES_H_ */
