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
#include "Logger.h"

/*!
 * \brief   Enum class that indicates how particles in different levels (cross
 *          level checking) of the HGrid are checked for interactions. 
 * \details With this enum class, one can choose how they want to check particles
 *          in different levels for interactions, so called cross-level checking
 *          BOTTOMUP means that small particles are inserted into the coarser 
 *          HGrid-level, TOPDOWN means that big particles are inserted into a 
 *          finer HGrid-level when checking for interactions.
 */
enum HGridMethod
{
    BOTTOMUP, TOPDOWN
};

/*!
 * \brief   Enum that indicates what the ratio of the size of the cells in 
 *          different levels is.
 * \details The options for the distribution of cell-sizes are:
 *          - OLDHGRID Each cell is twice as big as the one below it, for example
 *            (1, 2, 4, 8, ...).
 *          - LINEAR The difference between the cell sizes at each level is the 
 *            same, for example (1, 2, 3, 4, ...).
 *          - EXPONENTIAL Each cell size is a constant factor c > 1 bigger than 
 *            the one below it, for example with c = 3, (1, 3, 9, 27, ...).
 *          - USER The cell sizes are defined by the user.
 */
enum HGridDistribution
{
    OLDHGRID, LINEAR, EXPONENTIAL, USER
};

/*!
 * \brief This is the base class for both Mercury2D and Mercury3D. Note the 
 *        actually abstract grid is defined in the class Grid defined below.
 * \todo  Discuss if we want to remove Mercury2D and Mercury3D and template 
 *        MercuryBase as the interface for the user.
 */
class MercuryBase : public virtual DPMBase
{
public:   
    /*!
     * \brief This is the default constructor. It sets sensible defaults.
     */
    MercuryBase();

    /*!
     * \brief This is the default destructor.
     */
    ~MercuryBase();

    /*!
     * \brief Copy-constructor.
     */
    MercuryBase(const MercuryBase& mercuryBase);

    /*!
     * \brief This is the actual constructor, it is called do both constructors above.
     */
    void constructor();

    /*!
     * \brief This sets up the broad phase information, has to be done at this 
     *        stage because it requires the particle size.
     */
    void hGridActionsBeforeTimeLoop() override;

    /*!
     * \brief Performs all necessary actions before a time-step, like updating 
     *        the particles and resetting all the bucket information, etc.
     */
    void hGridActionsBeforeTimeStep() override;

    /*!
     * \brief Reads the MercuryBase from an input stream, for example a restart file.
     */
    void read(std::istream& is) override;

    /*!
     * \brief Writes the MercuryBase to an output stream, for example a restart file.
     */
    void write(std::ostream& os, bool writeAllParticles = true) const override;

    /*!
     * \brief Returns hGridCurrentMaxRelativeDisplacement_.
     */
    Mdouble getHGridCurrentMaxRelativeDisplacement() const;

    /*!
     * \brief Returns hGridTotalCurrentMaxRelativeDisplacement_.
     */
    Mdouble getHGridTotalCurrentMaxRelativeDisplacement() const;

    /*!
     * \brief Sets whether or not the HGrid must be updated every time step.
     */
    void setHGridUpdateEachTimeStep(bool updateEachTimeStep);

    /*!
     * \brief Gets whether or not the HGrid is updated every time step.
     */
    bool getHGridUpdateEachTimeStep() const override;

    /*!
     * \brief Sets the maximum number of levels of the HGrid in this MercuryBase.
     */
    void setHGridMaxLevels(unsigned int HGridMaxLevels);

    /*!
     * \brief Gets the maximum number of levels of the HGrid in this MercuryBase.
     */
    unsigned int getHGridMaxLevels() const;

    /*!
     * \brief Gets whether the HGrid in this MercuryBase is BOTTOMUP or TOPDOWN.
     */
    HGridMethod getHGridMethod() const;

    /*!
     * \brief Sets the HGridMethod to either BOTTOMUP or TOPDOWN.
     */
    void setHGridMethod(HGridMethod hGridMethod);

    /*!
     * \brief Gets how the sizes of the cells of different levels are distributed.
     */
    HGridDistribution getHGridDistribution() const;

    /*!
     * \brief Sets how the sizes of the cells of different levels are distributed.
     */
    void setHGridDistribution(HGridDistribution hGridDistribution);

    /*!
     * \brief Gets the ratio of the smallest cell over the smallest particle.
     */
    Mdouble getHGridCellOverSizeRatio() const;

    /*!
     * \brief Sets the ratio of the smallest cell over the smallest particle.
     */
    void setHGridCellOverSizeRatio(Mdouble cellOverSizeRatio);

    /*!
     * \brief Gets if the HGrid needs rebuilding before anything else happens.
     */
    bool hGridNeedsRebuilding();
    
    /*!
     * \brief Gets the desired number of buckets, which is the maximum of the number of particles and 10.
     */
    virtual unsigned int getHGridTargetNumberOfBuckets() const;
    
    /*!
     * \brief Gets the desired size of the smallest cells of the HGrid.
     */
    virtual Mdouble getHGridTargetMinInteractionRadius() const;
    
    /*!
     * \brief Gets the desired size of the largest cells of the HGrid.
     */
    virtual Mdouble getHGridTargetMaxInteractionRadius() const;
    
    /*!
     * \brief Checks if given BaseParticle has an interaction with a BaseWall or other BaseParticle.
     */
    bool checkParticleForInteraction(const BaseParticle& P) override;
    
    /*!
     * \brief Virtual function that enables inheriting classes to implement a function to let the user set the cell size of the HGrid.
     */
    virtual Mdouble userHGridCellSize(unsigned int level);

protected:
    
    /*!
     * \brief This sets up the parameters required for the contact model.
     */
    void hGridRebuild();

    /*!
     * \brief Inserts a single Particle to current grid.
     */
    void hGridInsertParticle(BaseParticle* obj) override;

    /*!
     * \brief This checks particles in the HGRID to see if for closer enough
     *        for possible contact.
     */
    void broadPhase(BaseParticle* i) override;
    
    /*!
     * \brief This is a purely virtual function that checks if an object is in
     *        the grid, this code is dimension dependent so purely virtual at 
     *        this stage.
     */
    virtual void hGridFindOneSidedContacts(BaseParticle* obj) = 0;
    
    /*!
     * \brief Purely virtual function that checks if the given particle has a 
     *        possible contact with any other BaseParticle in the HGrid.
     */
    virtual bool hGridHasParticleContacts(const BaseParticle* obj)=0;

    /*!
     * \brief Computes the relative displacement of the given BaseParticle and 
     *        updates the currentMaxRelativeDisplacement_ accordingly. 
     */
    void hGridUpdateMove(BaseParticle * iP, Mdouble move) override;

    /*!
     * \brief Resets the currentMaxRelativeDisplacement_ to 0.
     */
    void hGridActionsBeforeIntegration() override;

    /*!
     * \brief Sets the totalCurrentMaxRelativeDisplacement_ as 2*currentMaxRelativeDisplacement_.
     */
    void hGridActionsAfterIntegration() override;
    
    /*!
     * \brief Gets the HGrid used by this problem.
     */
    HGrid* getHGrid();

    /*!
     * \brief Gets the HGrid used by this problem, const version.
     */
    const HGrid* getHGrid() const;

    /*!
     * \brief Reads the next command line argument.
     */
    bool readNextArgument(int& i, int argc, char *argv[]) override;

public:
    /*!
     * \brief Writes the info of the HGrid to the screen in a nice format.
     */
    void hGridInfo() const;
    
private:
    /*!
     * \brief A pointer to the HGrid associated with this MercuryBase.
     */
    HGrid* grid;
    
    /*!
     * \brief   Indicator of which way the interactions between different levels
     *          are tested.
     * \details If hGridMethod_ is TOPDOWN, then larger particles are inserted
     *          into the grid of smaller particles when looking for cross level 
     *          interactions. If hGridMethod_ is BOTTOMUP, then smaller 
     *          particles are inserted into the grid of larger particles when 
     *          looking for cross-level interactions.
     *          The default for the hGridMethod_ is TOPDOWN.
     */
    HGridMethod hGridMethod_;
    
    /*!
     * \brief Indicator for the distribution of the sizes of the cells of 
     *        different levels of the HGrid. The default is EXPONENTIAL.
     */
    HGridDistribution hGridDistribution_;
    
    /*!
     * \brief   Mdouble that denotes the maximum of the displacement of a 
     *          particle divided by the cell size of the level of that particle 
     *          at the current time step. This is useful to see whether a 
     *          particle could have moved more than one cell.
     */
    Mdouble currentMaxRelativeDisplacement_;
    
    /*!
     * \brief After each time step, this Mdouble is increased by 
     *        2*currentMaxRelativeDisplacement_.
     */
    Mdouble totalCurrentMaxRelativeDisplacement_;
   
    /*!
     * \brief   Boolean that indicates whether or not the grid needs to be updated.
     * \details The grid needs to be updated before the HGrid is constructed and
     *          whenever something of the HGrid changes, such as the distribution,
     *          the number of levels, etc.
     */
    bool gridNeedsUpdate_;
    
    /*!
     * \brief Boolean which indicates whether or not the cell in which a 
     *        particle is must be updated every timestep.
     */
    bool updateEachTimeStep_;
    
    /*!
     * \brief   Unsigned integer that indicates the maximum number of levels of 
     *          the HGrid.
     * \details The number of levels in the HGrid is currently dependent on the 
     *          maximum number of levels of the HGrid the user has given, either
     *          in the constructor or in setHGridMaxLevels, and whether the 
     *          BaseParticle are monodispersed or not. If they are 
     *          monodispersed, there is only one level in the HGrid, otherwise 
     *          there are usually as many levels as the maximum set by the user.
     *          Note, by default the code sorts out these parameters itself but
     *          they can be overridden by the user.
     */
    unsigned int hGridMaxLevels_;
    /*!
     * \brief   The maximum ratio between the size of the cells and the BaseParticle they contain.
     * \details The hGrifCellOverSizeRatio_ can be found by dividing the size of
     *          the smallest cell by the size of the smallest particle, so this 
     *          can be set to give particles more space within a cell.
     *          Currently, the default is set to 1.
     */
    Mdouble hGridCellOverSizeRatio_;
};

#endif
