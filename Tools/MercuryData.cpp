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


#include "MercuryData.h"

std::istream& operator>>(std::istream& in, MercuryParticle& p)
{
  in >> p.position[0]
     >> p.position[1]
     >> p.position[2];
  
  in >> p.velocity[0]
     >> p.velocity[1]
     >> p.velocity[2];
     
  in >> p.radius;
  
  in >> p.rotation[0]
     >> p.rotation[1]
     >> p.rotation[2];

  in >> p.angularV[0]
     >> p.angularV[1]
     >> p.angularV[2];
     
  in >> p.speciesID;
  
  return in;
}

std::istream& operator>>(std::istream& in, MercuryTimeStep& s)
{
  in >> s.numParticles_ >> s.time_ >> s.xMin_ >> s.yMin_ >> s.zMin_ >> s.xMax_ >> s.yMax_ >> s.zMax_;
  return in;
}

void MercuryTimeStepIterator::operator++()
{
  lastReadTimeStep_.ID_++; //increase the timestep id
  
  std::string line;
  std::getline(dataFile_->file_, line);
  
  std::istringstream lineStream (line);
  lineStream >> lastReadTimeStep_;
  if (lineStream.eof())
  {
    //We are shorter than expected.
    //Are we a 2D format?
    
  }

  if (!dataFile_->file_)
  {
    isEOFTimeStep_ = true;
  }
    
  lastReadTimeStep_.storage_.resize(lastReadTimeStep_.numParticles_);
  for (MercuryParticle& part : lastReadTimeStep_)
  {
    std::getline(dataFile_->file_, line);
    lineStream.clear();
    lineStream.str(line);
    
    lineStream >> part;
  }
}

MercuryDataFile::MercuryDataFile(std::string name)
  : file_(name)
{
}

bool MercuryDataFile::isMercury3DDataFile()
{
  //Store the position...
  std::ios::pos_type currentPosition = file_.tellg();
  file_.seekg(0); //Jump to the start...
  std::string line;
  //Get the first line..
  std::getline(file_, line);
  //And go to where we were...
  file_.seekg(currentPosition); //Nothing happened...
  
  std::istringstream lineStream(line);
  
  MercuryTimeStep step;
  lineStream >> step;
  
  //Did we reach the end yet?
  bool isValid = lineStream.good();
  double dummy;
  lineStream >> dummy;
  isValid = isValid && !lineStream.good();
  //Now we must have reached the end...
  return isValid;
}

