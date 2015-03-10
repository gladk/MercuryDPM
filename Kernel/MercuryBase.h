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

#ifndef MERCURYBASE_H
#define MERCURYBASE_H

#include <ostream>
#include <iostream>
#include <stdlib.h>

#include "DPMBase.h"
#include "HGrid.h"

/*!
 * \brief
 */
enum HGridMethod
{
    BOTTOMUP, TOPDOWN
};

/*!
 * \brief
 */
enum HGridDistribution
{
    OLDHGRID, LINEAR, EXPONENTIAL, USER
};

/*!
 * \class
 * \brief This is the base class for both Mercury2D and Mercury3D. Note the actually abstrate grid is defined in the case Grid defined below.
 * \todo Remove the dimension out of this class by templating and use of for loops for the check cells part
 */
class MercuryBase : public virtual DPMBase
{
public:   
    /*!
     * \brief This is the default constructor. All it does is set sensible defaults.
     */
    MercuryBase();

    /*!
     * \brief This is the default destructor.
     */
    ~MercuryBase();

    /*!
     * \brief Copy-constructor
     */
    MercuryBase(const MercuryBase& mercuryBase);

    /*!
     * \brief This is the actual constructor, it is called do both constructors above.
     */
    void constructor();

    /*!
     * \brief This sets up the broad phase information, has to be done at this stage becuase it requires the partcle size
     */
    void hGridActionsBeforeTimeLoop();

    /*!
     * \brief This resets all the bucket information
     */
    void hGridActionsBeforeTimeStep();

    /*!
     * \brief This function reads all HGRID data
     */
    void read(std::istream& is);

    /*!
     * \brief  This function outputs all HGRID data
     */
    void write(std::ostream& os, bool writeAllParticles=true) const;

    /*!
     * \brief 
     */
    Mdouble getHGridCurrentMaxRelativeDisplacement() const;

    /*!
     * \brief 
     */
    Mdouble getHGridTotalCurrentMaxRelativeDisplacement() const;

    /*!
     * \brief 
     */
    void setHGridUpdateEachTimeStep(bool updateEachTimeStep);

    /*!
     * \brief 
     */
    bool getHGridUpdateEachTimeStep() const;

    /*!
     * \brief 
     */
    void setHGridMaxLevels(unsigned int HGridMaxLevels);


    /*!
     * \brief 
     */
    unsigned int getHGridMaxLevels() const;

    /*!
     * \brief 
     */
    HGridMethod getHGridMethod() const;

    /*!
     * \brief 
     */
    void setHGridMethod(HGridMethod hGridMethod);

    /*!
     * \brief 
     */
    HGridDistribution getHGridDistribution() const;

    /*!
     * \brief 
     */
    void setHGridDistribution(HGridDistribution hGridDistribution);

    /*!
     * \brief 
     */
    Mdouble getHGridCellOverSizeRatio() const;

    /*!
     * \brief 
     */
    void setHGridCellOverSizeRatio(Mdouble cellOverSizeRatio);

    /*!
     * \brief 
     */
    bool hGridNeedsRebuilding();
    
    /*!
     * \brief 
     */
    virtual unsigned int getHGridTargetNumberOfBuckets() const;
    
    /*!
     * \brief 
     */
    virtual double getHGridTargetMinInteractionRadius() const;
    
    /*!
     * \brief 
     */
    virtual double getHGridTargetMaxInteractionRadius() const;
    
    /*!
     * \brief 
     */
    virtual bool checkParticleForInteraction(const BaseParticle& P);
    
    /*!
     * \brief
     */
    virtual double userHGridCellSize(unsigned int level);

protected:
    
    /*!
     * \brief This sets up the parameters required for the contact model
     */
    void hGridRebuild();

    /*!
     * \brief Inserts a single Particle to current grid
     */
    void hGridInsertParticle(BaseParticle *obj);

    /*!
     * \brief This makes the board_phase of contact point at the HGRID code
     */
    void broadPhase(BaseParticle *i);
    
    /*!
     * \brief This is a purely virtual function that checks if an object is in the grid, this code is again dim dependent so purly virtual at this stage.
     */
    virtual void hGridFindOneSidedContacts(BaseParticle *obj)=0;
    
    /*!
     * \brief 
     */
    virtual bool hGridHasParticleContacts(const BaseParticle *obj)=0;

    /*!
     * \brief 
     */
    void hGridUpdateMove(BaseParticle * iP, Mdouble move);

    /*!
     * \brief 
     */
    void hGridActionsBeforeIntegration();

    /*!
     * \brief 
     */
    void hGridActionsAfterIntegration();
    
    /*!
     * \brief 
     */
    HGrid* getHGrid();

    /*!
     * \brief 
     */
    const HGrid* getHGrid() const;

    /*!
     * \brief 
     */
    int readNextArgument(int& i, int argc, char *argv[]);

private:
    HGrid* grid;///
    
    HGridMethod hGridMethod_;///
    HGridDistribution hGridDistribution_;///

    Mdouble currentMaxRelativeDisplacement_;///
    Mdouble totalCurrentMaxRelativeDisplacement_;///
   
    bool gridNeedsUpdate_;
    bool updateEachTimeStep_;///
    unsigned int hGridMaxLevels_;///
    Mdouble hGridCellOverSizeRatio_; ///Can be set to give particles more space within a cell
};

#endif
