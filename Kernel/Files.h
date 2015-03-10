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
     * \brief Sets File::saveCurrentTimestep_ for all file types (ene, data, fstat, restart, stat)
     */
    void setSaveCurrentTimestep(bool saveCurrentTimestep);

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
