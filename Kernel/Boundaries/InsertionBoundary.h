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

#ifndef BOUNDARIES_INSERTIONBOUNDARY_H
#define BOUNDARIES_INSERTIONBOUNDARY_H

#include "BaseBoundary.h"

class MD;
class RNG;

  /*!
   * \class InsertionBoundary
   * \brief
   */
class InsertionBoundary : public BaseBoundary
{
public:
  /*!
   * \brief default constructor
   */
    InsertionBoundary();
  /*!
   * \brief destructor
   */    
    virtual ~InsertionBoundary();
    
  /*!
   * \brief
   * \param[in]
   */
    void set(BaseParticle* particleToCopy, unsigned int maxFailed);
    
  /*!
   * \brief
   */
    virtual BaseParticle* generateParticle(RNG &random)=0;

  /*!
   * \brief
   */
    virtual void checkBoundaryBeforeTimeStep(DPMBase* md);

  /*!
   * \brief
   * \return
   */    
    unsigned int getNumberOfParticlesInserted() const;
    
  /*!
   * \brief
   * \return
   */
    void setMaxFailed(unsigned int maxFailed);
    
  /*!
   * \brief
   * \return
   */
    unsigned int getMaxFailed() const;
    
  /*!
   * \brief
   * \param[in]
   */
    void setParticleToCopy(BaseParticle* particleToCopy);
    
  /*!
   * \brief
   */
    BaseParticle* getParticleToCopy() const;
    
  /*!
   * \brief reads wall
   */
    void read(std::istream& is);
    
  /*!
   * \brief outputs wall
   */
    void write(std::ostream& os) const;

private:
  /*!
   * \brief
   */
    BaseParticle* particleToCopy_;
  /*!
   * \brief
   */
    unsigned int maxFailed_;
  /*!
   * \brief
   */
    unsigned int numberOfParticlesInserted_;
};

#endif
