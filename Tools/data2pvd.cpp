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

/*
* This program converts Mercury 3D .data files to ParaView
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
  //And were we using a 3D file format?
  if (!infile.isMercury3DDataFile())
  {
    logger(ERROR, "The file '%' does not seem to be a Mercury 3D .data file.\n"
                  "Mercury 2D .data files are unsupported. Please make sure you are reading\n"
                  "the correct file.", argv[1]);
  }
  
  if (std::string(argv[2]).back() == '/')
  {
    logger(ERROR, "The output prefix ends in a slash. This is not allowed.");
  }
  
  //Well, lets try to open the record file..
  std::ofstream outfile(argv[2] + std::string(".pvd"));
  //Succeeded?
  if (!outfile)
  {
    //Nope. Print an error and exit.
    logger(FATAL, "Could not open '%.pvd' for output.\n"
                 "Please make sure you have the appropriate rights.", argv[2]);
  }
  
  //So, write the headers...
  outfile <<
    "<?xml version=\"1.0\"?>\n"
    "<VTKFile type=\"Collection\" version=\"0.1\" byte_order=\"LittleEndian\">\n"
    " <Collection>\n";
  
  int exitCode = 0; //So we can flag errors!
  //And for every timestep...
  for (const MercuryTimeStep& ts : infile)
  {
    //Open a file containing the data.
    std::ostringstream filename;
    filename << argv[2] << "_" << ts.getTimeStepID() << ".vtu";
    
    std::ofstream tsfile(filename.str());
    if (!tsfile)
    {
      //Hmm. this is weird.
      //Normally, the master file would have failed.
      //Write whatever we got, and then exit..
      logger(WARN, "Could not open '%' for output\n"
                   "Please make sure you have the appropriate rights.", filename.str());
      exitCode = 6;
      break;
    }
    //Weird... we didn't expect an EOF / IOError here...    
    if (!infile) {
      logger(WARN,"An IO error occurred while processing '%'.", argv[1]);
      break;
    }
    
    //Okay, write the header first...
    //(and some information about the cells, which we aren't using.)
    tsfile <<
       "<?xml version=\"1.0\"?>\n"
       "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n"
       " <UnstructuredGrid>\n"
       "  <Piece NumberOfPoints=\"" << ts.getNumberOfParticles() << "\" NumberOfCells=\"0\">\n"
       "   <Cells>\n"
       "    <DataArray type=\"Int32\" name=\"connectivity\" format=\"ascii\">\n"
       "       0\n"
       "    </DataArray>\n"
       "    <DataArray type=\"Float32\" name=\"offset\" format=\"ascii\">\n"
       "       0\n"
       "    </DataArray>\n"
       "    <DataArray type=\"UInt8\" name=\"types\" format=\"ascii\">\n"
       "       1\n"
       "    </DataArray>\n"
       "   </Cells>\n"
       "   <Points>\n";
     
     //Start by writing the positions..
     tsfile << 
       "     <DataArray type=\"Float32\" NumberOfComponents=\"3\" format=\"ascii\">\n";
     for (const MercuryParticle& part : ts)
     {
       tsfile << part.position[0] << " "
              << part.position[1] << " "
              << part.position[2] << " "; 
     }
     tsfile << "\n"
       "     </DataArray>\n"
       "    </Points>\n"
       "    <PointData>\n";
     
     //And we want the positions as a dataarray as well
     //(@dducks: do we?)
     tsfile << 
       "    <DataArray type=\"Float32\" Name=\"Positions\" NumberOfComponents=\"3\" format=\"ascii\">\n";
     for (const MercuryParticle& part : ts)
     {
       tsfile << part.position[0] << " "
              << part.position[1] << " "
              << part.position[2] << " "; 
     }
     tsfile << "\n"
       "     </DataArray>\n";

     //Next step: Speeds!
     tsfile << "    <DataArray type=\"Float32\" Name=\"Velocities\" NumberOfComponents=\"3\" format=\"ascii\">\n";
     for (const MercuryParticle& part : ts)
     {
       tsfile << part.velocity[0] << " "
              << part.velocity[1] << " "
              << part.velocity[2] << " "; 
     }
     tsfile << "\n     </DataArray>\n";
     
     //But our particles have rotation too...
     //So positions...
     tsfile << "    <DataArray type=\"Float32\" Name=\"Rotations\" NumberOfComponents=\"3\" format=\"ascii\">\n";
     for (const MercuryParticle& part : ts)
     {
       tsfile << part.rotation[0] << " "
              << part.rotation[1] << " "
              << part.rotation[2] << " "; 
     }
     tsfile << "\n     </DataArray>\n";

     //And speeds for rotations..
     tsfile << "    <DataArray type=\"Float32\" Name=\"AngularVelocities\" NumberOfComponents=\"3\" format=\"ascii\">\n";
     for (const MercuryParticle& part : ts)
     {
       tsfile << part.angularV[0] << " "
              << part.angularV[1] << " "
              << part.angularV[2] << " "; 
     }
     tsfile << "\n     </DataArray>\n";

     //Lets write our radii.
     tsfile << "    <DataArray type=\"Float32\" Name=\"Radii\" NumberOfComponents=\"1\" format=\"ascii\">\n";
     for (const MercuryParticle& part : ts)
     {
       tsfile << part.radius << " ";
     }
     tsfile << "\n     </DataArray>\n";

     //And of course our species IDs...
     tsfile << "    <DataArray type=\"UInt32\" Name=\"Species\" NumberOfComponents=\"1\" format=\"ascii\">\n";
     for (const MercuryParticle& part : ts)
     {
       tsfile << part.speciesID << " ";
     }
     tsfile << "\n     </DataArray>\n";
     
     tsfile <<
     "   </PointData>\n"
     "   <CellData/>\n"
     "  </Piece>\n"
     " </UnstructuredGrid>\n"
     "</VTKFile>\n";
     tsfile.close();
     
     //Et voila! Un VTK Unstructured grid file.
     //Now just add it to the index file...
     
     //@dducks: Okay, the user may output in a subdirectory. 
     //Therefore we should strip anything off until the last slash.
     std::string strippedPath = filename.str();
     std::string::size_type slashPosition = strippedPath.rfind('/');
     if (slashPosition != std::string::npos) {
       //There is a slash in here
       strippedPath = strippedPath.substr(slashPosition+1);
     }
     
     outfile <<
     "  <DataSet group=\"\" part=\"0\" timestep=\"" << ts.getTimeStepID() <<"\" file=\"" << strippedPath << "\" />\n";
     //Done.
     //Note the timestep=getTimeStepID(). If somebody finds a way to get it working reliably in ParaView,
     //this could be changed into getTime(). ParaView docs say it should parse floats... but in practice, it doesn't.
     //(or I'm doing something stupid, which of course is also very possible)
  }
  
  //We can either be done, or an error has occurred which allows us to at least write the data so far..
  outfile << " </Collection>\n"
             "</VTKFile>\n";
  //So, write a footer
  //Flush it
  outfile.close();
  //and return a sensible error code (or 0 in case of success).
  return exitCode;
}
