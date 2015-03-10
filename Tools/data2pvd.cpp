//Copyright (c) 2015, The MercuryDPM Developers Team. All rights reserved.
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
#include <fstream>

#include <Logger.h>

#include "MercuryData.h"
#include "VTKData.h"

/*! \brief Templated version to automagically generate VTK output files. */
template<std::size_t NDIMS>
int transformMercuryToVTK(MercuryDataFile& file, std::string prefix);

/*
* This program converts Mercury .data files to ParaView
* .pvd and VTK .vtu XML-files. Every timestep is written to
* an independent .vtu Unstructured Grid file.
*
* Because of ParaView limitations, the actual time information
* is discarded; Using the timestep attribute of the DataSet
* element in the main .pvd file does not produce desired
* results.
*
*/

int main(int argc, char** argv)
{
  //Check to see if we actually received two arguments
  if (argc != 3)
  {
    //We didn't. Print a usage and exit the program.
    logger(FATAL, "Usage: % [infile] [outfilePrefix]\n"
                 "  This program converts MercuryDPM .data files to ParaView .pvd data files.\n"
                 "  which can then be used directly into ParaView, to visualize your particles.\n"
                 "\n"
                 "   infile: MercuryDPM .data file\n"
                 "\n"
                 "   outfilePrefix: Prefix to prepend to the outputfiles generated.\n"
                 "     The following files will be generated:\n"
                 "       - prefix.pvd\n"
                 "       - prefix_0.vtu\n"
                 "       - prefix_1.vtu\n"
                 "           ( ... )\n"
                 "       - prefix_987654321.vtu\n"
                 "     depending on the amount of timesteps.", argv[0]);
  }
  
  // Open our Mercury 3D data file.
  MercuryDataFile infile(argv[1]);
  
  // Was it readable?
  if (!infile)
  {
    logger(FATAL, "Could not open ''%' for input.\n"
                 "Please make sure the file exists and you have the appropriate rights.", argv[1]);
  }
  //make sure we don't end in a slash, as this both breaks the creation of a relative path right now,
  //and it's really not what you want to end up with...
  if (std::string(argv[2]).back() == '/')
  {
    logger(ERROR, "The output prefix ends in a slash. This is not allowed.");
  }

  //And were we using a 3D file format?
  if (infile.isMercuryDataFile<3>())
  {
    logger(VERBOSE, "Assuming 3D data format.");
    return transformMercuryToVTK<3>(infile, argv[2]);
  }//Or a 2d format?
  else if (infile.isMercuryDataFile<2>())
  {
    logger(VERBOSE, "Assuming 2D data format.");
    return transformMercuryToVTK<2>(infile, argv[2]);
  }//halp...
  else
  {
    logger(ERROR, "The file '%' does not seem to be a Mercury .data file.\n"
                  "Please make sure you are reading the correct file.", argv[1]);
    return 4;
  }
}

/*! this function reads the NDIMS-dimensional Mercury .data file,
 * and writes all the corresponding VTK output files.
 */
template<std::size_t NDIMS>
int transformMercuryToVTK(MercuryDataFile& infile, std::string prefix)
{
  //We really want to describe our exit code.
  int exitCode = 0;
  
  //We'll set up a descriptor.
  //Let the compiler figure out what the types / dimensions are, that's too
  //much work anyway. Oh and make sure position is used as both a datafield
  //and the actual position of the object.
  VTKPointDescriptor< MercuryParticle<NDIMS> > descriptor;
  descriptor
    .addProperty( "Position",    & MercuryParticle< NDIMS >::position , true)
    .addProperty( "Velocity",    & MercuryParticle< NDIMS >::velocity  )
    .addProperty( "Rotation",    & MercuryParticle< NDIMS >::rotation  )
    .addProperty( "AngVelocity", & MercuryParticle< NDIMS >::angularV  )
    .addProperty( "Radius",      & MercuryParticle< NDIMS >::radius    )
    .addProperty( "Species",     & MercuryParticle< NDIMS >::speciesID );
    
  // We want a Collection file which lists all individual files
  VTKCollection collection( prefix + ".pvd" );
  // First, make sure it is sane.
  if (!collection)
    logger( FATAL, "Could not open '%.pvd' for output.\n"
                   "Please make sure you have the appropriate permissions and try again.", prefix);
  
  std::size_t timestepCount = 0;
  
  //Now, read all the timesteps as if they were NDIMS long.
  for (const MercuryTimeStep<NDIMS> & ts : infile.as<NDIMS>())
  {
    //Generate the filename for the individual data files.
    std::ostringstream filename;
    filename << prefix << '_' << ts.getTimeStepID() << ".vtu";
    
    //We'll set up a datafile containing the individual timestep.
    VTKUnstructuredGrid< MercuryParticle<NDIMS> > timeStepFile(filename.str(), &descriptor);
    if (!timeStepFile) //but not after we've done some sanity checking!
    {
      logger(WARN, "Could not open '%' for output.\n"
                   "Please make sure you have the appropriate permissions and try again.", filename.str());
      exitCode = 6;
      break;
    }
    
    if (!infile) //Some sanity checking? More sanity checking! ALL the sanity checking!!!
    {
      logger(WARN, "An IOError occurred during the reading of the input file.\n"
                   "Please make sure you are feeding this tool mercury data files.");
      exitCode = 5; 
      break;
    }
    //Okay. we done.
    //Let's write.
    timeStepFile.write(ts);
    
    //So, a user may give an output path which is in a different directory. 
    // However, since the index files resides in the same output directory
    // as the timestep files, we need to specify relative paths.
    std::string strippedPath = filename.str();
    //so, we try to find the last / - because to hell with everybody with other
    //path seperators...
    std::string::size_type slashPosition = strippedPath.rfind('/');
    //and take only the last part. It's not the last character, trust me.
    //we checked for that in main().
    if (slashPosition != std::string::npos)
      strippedPath = strippedPath.substr(slashPosition + 1);
    //And now put the relative path in the listing file
    collection.append(strippedPath);
    
    timestepCount++;
  }
  
  logger(INFO, "Written % timesteps in a %D system.", timestepCount, NDIMS);
  
  return exitCode;
}
