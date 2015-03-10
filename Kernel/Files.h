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
 * \brief The class File provides an fstream variable for reading/writing.
 * \details The class also stores extra information like \n
 * - the file name,\n
 * - the file type (one file or multiple files), and\n
 * - the opening mode (input/ output/ appended output).
 */
class Files
{
public:
    
//Constructors
    /*!
     * \brief constructor
     */    
    Files();

    /*!
     * \brief destructor
     */
    virtual ~Files();

    /*!
     * \brief 
     * \todo add const
     */
    Files(const Files& other);
            
//setters and getters
    
    /*!
     * \brief const's taken out!
     */
    File& getDataFile();

    /*!
     * \brief 
     */
    File& getEneFile();

    /*!
     * \brief 
     */
    File& getFStatFile();

    /*!
     * \brief 
     */
    File& getRestartFile();

    /*!
     * \brief 
     */
    File& getStatFile();
    
    /*!
     * \brief 
     */
    const File& getDataFile() const;

    /*!
     * \brief 
     */
    const File& getEneFile() const;

    /*!
     * \brief 
     */
    const File& getFStatFile() const;

    /*!
     * \brief 
     */
    const File& getRestartFile() const;

    /*!
     * \brief 
     */
    const File& getStatFile() const;

    /*!
     * \brief 
     */
    const std::string& getName() const;

    /*!
     * \brief 
     */
    void setName(const std::string& name);

    /*!
     * \brief 
     */
    void setName(const char* name);

    /*!
     * \brief Sets File::saveCount_ for all file types (ene, data, fstat, restart, stat)
     */
    void setSaveCount(unsigned int saveCount);

    /*!
     * \brief 
     */
    void setFileType(FileType fileType);

    /*!
     * \brief 
     */
    void setOpenMode(std::fstream::openmode openMode);

//other member functions
    
    /*!
     * \brief 
     */
    void resetFileCounter();

    void read(std::istream& is);

    void write(std::ostream& os) const;

    void openFiles();

    void closeFiles();

    void setNextSavedTimeStep(unsigned int nextSavedTimeStep);

private:
    /*!
     * \brief File class to handle in- and output into a .data file
     */
    File dataFile_;

    /*!
     * \brief File class to handle in- and output into a .fstat file
     */
    File fStatFile_;

    /*!
     * \brief File class to handle in- and output into a .ene file
     */
    File eneFile_;

    /*!
     * \brief File class to handle in- and output into a .restart file
     */
    File restartFile_;

    /*!
     * \brief File class to handle in- and output into a .stat file
     */
    File statFile_;

    /*!
     * \brief the name of the problem, used, e.g., for the output files
     */
    std::string name_;
    
};

#endif /* OUTPUTFILES_H_ */
