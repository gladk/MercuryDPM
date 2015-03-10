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

#include <limits>
#include <string.h>

#include "MercuryBase.h"
#include "Math/Helpers.h"
#include "Particles/BaseParticle.h"
#include "Walls/BaseWall.h"

MercuryBase::MercuryBase()
{
    constructor();
    logger(DEBUG, "MercuryBase::MercuryBase() constructor finished");
}

MercuryBase::~MercuryBase()
{
    if (grid != nullptr)
    {
        delete grid;
        grid = nullptr;
    }
    logger(DEBUG, "MercuryBase::~MercuryBase() destructor finished.");
}

/*!
 * \param[in] mercuryBase The MercuryBase that needs to be copied.
 * \details Copy constructor that copies almost all properties of the given 
 *          MercuryBase into the new MercuryBase. Please note that the grid is not
 *          literally copied, but everything to make that grid is copied and the 
 *          gridNeedsUpdate_ is set to true, so that the grid is made before 
 *          anything else happens.
 */
MercuryBase::MercuryBase(const MercuryBase& mercuryBase)
{
    grid = nullptr;
    gridNeedsUpdate_ = true;
    
    hGridMethod_ = mercuryBase.hGridMethod_;
    hGridDistribution_ = mercuryBase.hGridDistribution_;
    
    currentMaxRelativeDisplacement_ = mercuryBase.currentMaxRelativeDisplacement_;
    totalCurrentMaxRelativeDisplacement_ = mercuryBase.totalCurrentMaxRelativeDisplacement_;
    
    updateEachTimeStep_ = mercuryBase.updateEachTimeStep_;
    hGridMaxLevels_ = mercuryBase.hGridMaxLevels_;
    hGridCellOverSizeRatio_ = mercuryBase.hGridCellOverSizeRatio_;
    
    logger(DEBUG, "HGRID_base(HGrid_base& other) constructor finished.");
}

/*!
 * \details Function that is called by the default constructor, it sets all 
 *          parameters for the MercuryBase to sensible defaults.
 */
void MercuryBase::constructor()
{
    grid = nullptr;
    gridNeedsUpdate_ = true;
    hGridMaxLevels_ = 3;
    hGridCellOverSizeRatio_ = 1.0;
    updateEachTimeStep_ = true;
    hGridDistribution_ = EXPONENTIAL;
    hGridMethod_ = TOPDOWN;
}

/*!
 * \details Performs all actions that need to be done before the time loop. At the
 *          moment, this means nothing.
 */
void MercuryBase::hGridActionsBeforeTimeLoop()
{
}

/*!
 * \param[in,out] is The input stream from which the MercuryBase must be read.
 * \details This function reads first the properties that are DPMBase related from
 *          the given input stream, after that it reads the hGridMaxLevels_ and 
 *          the hGridCellOverSizeRatio_ from the input stream.
 */
void MercuryBase::read(std::istream& is)
{
    DPMBase::read(is);
    
    std::stringstream line(std::stringstream::in | std::stringstream::out);
    helpers::getLineFromStringStream(is, line);
    
    std::string dummy;
    
    line >> dummy >> dummy
            >> dummy >> hGridMaxLevels_
            >> dummy >> hGridCellOverSizeRatio_;
}

/*!
 * \param[in,out] os            The output stream to which this MercuryBase must
 *                              be written.
 * \param[in] writeAllParticles A boolean which indicates whether or not all BaseParticle
 *                              from the ParticleHandler must be written to the 
 *                              ostream. If it is set to true or if there are at 
 *                              most 4 BaseParticle, all BaseParticle are written.
 *                              If it is set to false, only the first two BaseParticle
 *                              are written, followed by ...
 * \details Function that writes this MercuryBase to an output stream, for example
 *          a restart file. First writes the domain information, then the walls,
 *          followed by the boundaries and particles, and finally the HGrid information.
 */
void MercuryBase::write(std::ostream& os, bool writeAllParticles) const
{
    DPMBase::write(os, writeAllParticles);
    if (grid != nullptr)
    {
        os << "NUM_BUCKETS " << grid->getNumberOfBuckets() << " "
                << "hGridMaxLevels " << getHGridMaxLevels() << " "
                << "cellOverSizeRatio " << getHGridCellOverSizeRatio() << std::endl;
    }
    else
    {
        os << "NUM_BUCKETS " << 0.0 << " "
                << "hGridMaxLevels " << getHGridMaxLevels() << " "
                << "cellOverSizeRatio " << getHGridCellOverSizeRatio() << std::endl;
    }
}

/*!
 * \return  currentMaxRelativeDisplacement_, which is the highest relative speed
 *          of a BaseParticle compared to the cell size of the grid in which the 
 *          BaseParticle is.
 */
Mdouble MercuryBase::getHGridCurrentMaxRelativeDisplacement() const
{
    return currentMaxRelativeDisplacement_;
}

/*!
 * \return totalCurrentMaxRelativeDisplacement_, which is the cumulative value of 
 *          2*currentMaxRelativeDisplacement_ 
 */
Mdouble MercuryBase::getHGridTotalCurrentMaxRelativeDisplacement() const
{
    return totalCurrentMaxRelativeDisplacement_;
}

/*!
 * \param[in] updateEachTimeStep    A boolean which indicates if the HGrid must be 
 *                                  updated every time step.
 */
void MercuryBase::setHGridUpdateEachTimeStep(bool updateEachTimeStep)
{
    updateEachTimeStep_ = updateEachTimeStep;
}

/*!
 * \return A boolean which indicates if the HGrid must be updated every time step.
 */
bool MercuryBase::getHGridUpdateEachTimeStep() const
{
    return updateEachTimeStep_;
}

/*!
 * \details Rebuild the HGrid with the current data. First compute the cell sizes
 *          of the new HGrid, then delete the old HGrid and build the new HGrid. 
 *          Finally add all particles to the new HGrid.
 *          First check if either the ParticleHandler is empty or the particle 
 *          distribution is monodispersed. If this is the case, make a grid that
 *          contains only one level which has a cell size defined by the diameter
 *          of the particle * hGridCellOverSizeRatio_.
 *          Otherwise, make cell sizes depending on the distribution given in 
 *          hGridDistribution_. LINEAR and EXPONENTIAL first compute the smallest
 *          and biggest cell size, and then compute the ones in between according
 *          to the distribution. USER just takes the cell sizes given by the user.
 *          OLDHGRID computes the smallest cell size and then makes the cells of
 *          each subsequent level twice as big.
 * \todo Convert all double to Mdouble.
 */
void
MercuryBase::hGridRebuild()
{

    std::vector<double> cellSizes;

    double minParticleInteractionRadius = getHGridTargetMinInteractionRadius();
    double maxParticleInteractionRadius = getHGridTargetMaxInteractionRadius();
    if(minParticleInteractionRadius == 0.0 || minParticleInteractionRadius == maxParticleInteractionRadius)
    {
        //this case is executed if the particleHandler is empty (minParticleInteractionRadius == 0)
        //or if the particle distribution is monodispersed. 
        //nextafter(d,std::numeric_limits<max>) chooses the smallest double that is bigger than d.
        double maxCellSize = nextafter(2.0 * maxParticleInteractionRadius * getHGridCellOverSizeRatio(), std::numeric_limits<double>::max());
        cellSizes.push_back(maxCellSize);
        if(getHGridMaxLevels() != 1)
        {
            logger(WARN, "Warning: While rebuilding the hgrid: the number of levels was set to one, as the particle distribution is monodispersed");
        }
    }
    else
    {
        switch(getHGridDistribution())
        {
        case LINEAR:
        {
            double minCellSize = nextafter(2.0 * minParticleInteractionRadius * getHGridCellOverSizeRatio(), 0.0);
            double maxCellSize = nextafter(2.0 * maxParticleInteractionRadius * getHGridCellOverSizeRatio(), std::numeric_limits<double>::max());
            //std::cout << "HGrid: using a linear cell size distribution from " << minCellSize << " to " << maxCellSize << " over " << getHGridMaxLevels() << " levels" << std::endl;
            for(unsigned int i = 0; i + 1 < getHGridMaxLevels(); i++)
            {
                ///\todo cast i to double explicitly.
                cellSizes.push_back(minCellSize + (maxCellSize - minCellSize) * (i + 1.0) / getHGridMaxLevels());
            }
            //The last cell is added separately because in some cases accuracy was lost when calculating it.
            cellSizes.push_back(maxCellSize);
            break;
        }
        case EXPONENTIAL:
        {
            double minCellSize = nextafter(2.0 * minParticleInteractionRadius * getHGridCellOverSizeRatio(), 0.0);
            double maxCellSize = nextafter(2.0 * maxParticleInteractionRadius * getHGridCellOverSizeRatio(), std::numeric_limits<double>::max());
            //std::cout << "HGrid: using an exponential cell size distribution from " << minCellSize << " to " << maxCellSize << " over " << getHGridMaxLevels() << " levels" << std::endl;
            for(unsigned int i = 0; i + 1 < getHGridMaxLevels(); i++)
            {
                ///\todo explicitly cast i to double
                cellSizes.push_back(minCellSize * std::pow(maxCellSize / minCellSize, (i + 1.0) / getHGridMaxLevels()));
            }
            //The last cell is added separately because in some cases accuracy was lost when calculating it.
            cellSizes.push_back(maxCellSize);
            break;
        }
        case USER:
        {
            for(unsigned int i = 0; i < getHGridMaxLevels(); i++)
            {
                cellSizes.push_back(userHGridCellSize(i));
            }
            break;
        }
        case OLDHGRID:
        {
            double minCellSize = nextafter(2.0 * minParticleInteractionRadius * getHGridCellOverSizeRatio(), 0.0);

            //std::cout<<"HGrid: using the old HGrid cell size distribution starting from " <<minCellSize<<std::endl;
            for(unsigned int i = 0; i < getHGridMaxLevels(); i++)
            {
                cellSizes.push_back(minCellSize * std::pow(2, i));
            }
            break;
        }
        }
    }

    if(grid != nullptr)
    {
        delete grid;
    }

    grid = new HGrid(getHGridTargetNumberOfBuckets(), getHGridCellOverSizeRatio(), cellSizes);

    for(std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it != particleHandler.end(); ++it)
    {
        hGridInsertParticle(*it);
        //\todo{This is really ugly fix to force the particle to update}
        (*it)->setHGridX(9999);
        hGridUpdateParticle(*it);
    }
    gridNeedsUpdate_ = false;
}

/*!
 * \param[in] obj A pointer to the BaseParticle that needs to be inserted in the HGrid.
 */
void MercuryBase::hGridInsertParticle(BaseParticle *obj)
{
    if (grid != nullptr)
    {
        grid->insertParticleToHgrid(obj);
    }
}

#ifdef CONTACT_LIST_HGRID
void MercuryBase::broadPhase(BaseParticle *i UNUSED)
{
}
#else
/*!
 * \param[in] i A pointer to the BaseParticle for which we want to see if there 
 *              are interactions on its own level.
 */
void MercuryBase::broadPhase(BaseParticle *i)
{
    hGridFindOneSidedContacts(i);
}
#endif

/*!
 * \details The actions that are done before each time step, it rebuilds the HGrid
 *          if necessary, otherwise it computes which cell each particle is in.
 */
void MercuryBase::hGridActionsBeforeTimeStep()
{
    ///\todo IFCD Look up if stepsBeforeUpdate is actually used.
    static int stepsBeforeUpdate = 0;
    if (hGridNeedsRebuilding())
    {
        //std::cout<<"HGrid needs rebuilding for void HGRID_base::hGridActionsBeforeTimeStep()"<<std::endl;
        hGridRebuild();
        ///\todo TW: I believe the following two steps should be in hGridRebuild
        totalCurrentMaxRelativeDisplacement_ = 0;
        stepsBeforeUpdate = 0;
    }
    else
    {
#ifndef CONTACT_LIST_HGRID
        getHGrid()->clearBucketIsChecked();
#endif
        if (getHGridUpdateEachTimeStep() || getHGridTotalCurrentMaxRelativeDisplacement() >= getHGridCellOverSizeRatio() - 1)
        {
#ifndef CONTACT_LIST_HGRID
            getHGrid()->clearFirstBaseParticleInBucket();
#endif
            totalCurrentMaxRelativeDisplacement_ = 0;
            for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it != particleHandler.end(); ++it)
            {
                hGridUpdateParticle(*it);
            }
            stepsBeforeUpdate = 0;
        }
        else
        {
            stepsBeforeUpdate++;
        }
    }
    
}

/*!
 * \param[in] iP    A pointer to the BaseParticle for which we want to compare 
 *                  the relative speed to the currentMaxRelativeDisplacement_ to.
 * \param[in] move  An Mdouble that represents the distance the BaseParticle has
 *                  moved.    
 */
void MercuryBase::hGridUpdateMove(BaseParticle* iP, Mdouble move)
{
    Mdouble currentRelativeDisplacement = move / (getHGrid()->getCellSize(iP->getHGridLevel()));
    if (currentRelativeDisplacement > currentMaxRelativeDisplacement_)
    {
        currentMaxRelativeDisplacement_ = currentRelativeDisplacement;
    }
}

void MercuryBase::hGridActionsBeforeIntegration()
{
    currentMaxRelativeDisplacement_ = 0.0;
}

void MercuryBase::hGridActionsAfterIntegration()
{
    totalCurrentMaxRelativeDisplacement_ += 2.0 * currentMaxRelativeDisplacement_;
}

/*!
 * \return A pointer to the HGrid associated with this MercuryBase.
 */
HGrid* MercuryBase::getHGrid()
{
    return grid;
}

/*!
 * \return A pointer to the (constant) HGrid associated with this MercuryBase.
 */
const HGrid* MercuryBase::getHGrid() const
{
    return grid;
}

/*!
 * \param[in] level The level of the cell we want to know the size set by the user for.
 * \return The size of the cells at the given level.
 * \todo Convert the double to Mdouble.
 */
double MercuryBase::userHGridCellSize(unsigned int level)
{
    logger(WARN, "In double MercuryBase::userHGridCellSize(unsigned int level) with level= %", level);
    logger(WARN, "If you want to use user defined HGrid cell sizes, this function should be redefined");
    return 0.0;
}

/*!
 * \param[in] i     The ordinal number in the input arguments we want to read.
 * \param[in] argc  The number of arguments in the command line, not used.
 * \param[in] argv  The command line input arguments.
 * \return A boolean which is true if the argument is found and false otherwise.
 */
bool MercuryBase::readNextArgument(int& i, int argc, char *argv[])
{
    if (!strcmp(argv[i], "-hGridMaxLevels"))
    {
        setHGridMaxLevels(static_cast<unsigned int>(atoi(argv[i + 1])));
    }
    else if (!strcmp(argv[i], "-cellOverSizeRatio"))
    {
        setHGridCellOverSizeRatio(atof(argv[i + 1]));
    }
    else
    {
        return DPMBase::readNextArgument(i, argc, argv); //if argv[i] is not found, check the commands in MD
    }
    return true; //returns true if argv[i] is found
}

/*!
 * \return The HGridMethod used by this MercuryBase.
 */
HGridMethod MercuryBase::getHGridMethod() const
{
    return hGridMethod_;
}

/*!
 * \param[in] hGridMethod The HGridMethod that will be used in this MercuryBase.
 */
void MercuryBase::setHGridMethod(HGridMethod hGridMethod)
{
    hGridMethod_ = hGridMethod;
}

/*!
 * \return The HGridDistribution (distribution of cell sizes) used by this MercuryBase.
 */
HGridDistribution MercuryBase::getHGridDistribution() const
{
    return hGridDistribution_;
}

/*!
 * \param[in] hGridDistribution The distribution of the cell sizes that will be 
 *                              used by this MercuryBase.
 */
void MercuryBase::setHGridDistribution(HGridDistribution hGridDistribution)
{
    if (hGridDistribution_ != hGridDistribution)
    {
        gridNeedsUpdate_ = true;
        hGridDistribution_ = hGridDistribution;
    }
}

/*!
 * \return The maximum ratio between the cells and the size of the BaseParticle 
 *         it contains.
 */
Mdouble MercuryBase::getHGridCellOverSizeRatio() const
{
    return hGridCellOverSizeRatio_;
}

/*!
 * \param[in] hGridCellOverSizeRatio The desired maximum ratio between the cells
 *                                   and the size of the BaseParticle it contains.
 */
void MercuryBase::setHGridCellOverSizeRatio(Mdouble hGridCellOverSizeRatio)
{
    if (std::abs(hGridCellOverSizeRatio_-hGridCellOverSizeRatio)<1e-10)
    {
        gridNeedsUpdate_ = true;
        hGridCellOverSizeRatio_ = hGridCellOverSizeRatio;
    }
}

/*!
 * \param[in] hGridMaxLevels The maximum number of levels that will be used in the HGrid.
 */
void MercuryBase::setHGridMaxLevels(unsigned int hGridMaxLevels)
{
    if (hGridMaxLevels_ != hGridMaxLevels)
    {
        gridNeedsUpdate_ = true;
        hGridMaxLevels_ = hGridMaxLevels;
    }
}

/*!
 * \return The maximum number of levels in the HGrid.
 */
unsigned int MercuryBase::getHGridMaxLevels() const
{
    return hGridMaxLevels_;
}

/*!
 * \return A boolean which indicates if the HGrid needs to be rebuilt.
 */
bool MercuryBase::hGridNeedsRebuilding()
{
    if (grid == nullptr)
    {
        logger(VERBOSE, "HGrid needs updating, because there is no grid.");
        return true;
    }
    else if (gridNeedsUpdate_)
    {
        logger(VERBOSE, "HGrid needs updating, because some of its initialisation parameters have changed.");
        return true;
    }
    else if (grid->getNeedsRebuilding())
    {
        logger(VERBOSE, "HGrid needs updating, because said so by the grid itself");
        return true;
    }
    else if (getHGrid()->getNumberOfBuckets() > 10 * getHGridTargetNumberOfBuckets() || 10 * getHGrid()->getNumberOfBuckets() < getHGridTargetNumberOfBuckets())
    {
        logger(VERBOSE, "HGrid needs updating, because of number of buckets, current = %, target = %.", grid->getNumberOfBuckets(), particleHandler.getNumberOfObjects());
        return true;
    }
    else if (particleHandler.getLargestParticle() != nullptr && 2.0 * particleHandler.getLargestParticle()->getInteractionRadius() > getHGrid()->getCellSizes().back() * grid->getCellOverSizeRatio())
    {
        logger(VERBOSE, "HGrid needs updating, because of maximum cell size, current = %, required = %.", grid->getCellSizes().back() * hGridCellOverSizeRatio_, particleHandler.getLargestParticle()->getInteractionRadius());
        return true;
    }
    else
    {
        //std::cout<<"HGrid does not need updating, because of number of buckets, current="<<grid->NUM_BUCKETS<<" target="<<particleHandler.getNumberOfObjects()<<std::endl;
        //std::cout<<"HGrid does not need updating, because of maximum cell size, current="<<grid->cellSizes_.back()*grid->cellOverSizeRatio_<<" required="<<2.0*particleHandler.getLargestParticle()->getInteractionRadius()<<std::endl;
        return false;
    }
}

/*!
 * \return The number of possible values the hash function can have.
 */
unsigned int MercuryBase::getHGridTargetNumberOfBuckets() const
{
    unsigned int nParticles = particleHandler.getNumberOfObjects();
    if (nParticles > 10)
    {
        return nParticles;
    }
    else
    {
        return 10;
    }
}

/*!
 * \return The interaction radius of the smallest particle multiplied by the maximum
 *         ratio between the cell size and particle size.
 */
double MercuryBase::getHGridTargetMinInteractionRadius() const
{
    if (particleHandler.getNumberOfObjects() == 0)
    {
        return 0.0;
    }
    else
    {
        return particleHandler.getSmallestParticle()->getInteractionRadius() * getHGridCellOverSizeRatio();
    }
}

/*!
 * \return The interaction radius of the largest particle multiplied by the maximum
 *         ratio between the cell size and particle size.
 */
double MercuryBase::getHGridTargetMaxInteractionRadius() const
{
    if (particleHandler.getNumberOfObjects() == 0)
    {
        return 0.0;
    }
    else
    {
        return particleHandler.getLargestParticle()->getInteractionRadius() * getHGridCellOverSizeRatio();
    }
}
/*!
 * \param[in] p The BaseParticle for which we want to check if there is an interaction.
 * \return A boolean which is false if there was an interaction and true if there was no interaction.
 * \details Check for the given BaseParticle if there is an interaction with any
 *          other object. First check all walls, then all particles.
 * \todo IFCD: I think it might be better if it returns true if there is an interaction.
 */
bool MercuryBase::checkParticleForInteraction(const BaseParticle& p)
{
    Mdouble distance;
    Vec3D normal;

    //Check if it has no collision with walls
    for (std::vector<BaseWall*>::const_iterator it = wallHandler.begin(); it != wallHandler.end(); it++)
    {
        if ((*it)->getDistanceAndNormal(p, distance, normal))
        {
            logger(VERBOSE, "Collision with wall %.", **it);
            return false;
        }
        else
        {
            logger(VERBOSE, "No collision with wall %.", **it);
        }
    }
    
    //Check if it has no collision with other particles
    if (hGridHasParticleContacts(&p))
    {
        logger(VERBOSE, "Collision with particle.");
        return false;
    }
    else
    {
        logger(VERBOSE, "No collision with particles.");
    }
    return true;
}


void MercuryBase::hGridInfo() const
{
    std::cout << "Status of hGrid parameters:" << std::endl;
    std::cout << "HGridMethod hGridMethod_=" << hGridMethod_ << std::endl; 
    std::cout << "HGridDistribution hGridDistribution_=" << hGridDistribution_ << std::endl; 
    std::cout << "Mdouble currentMaxRelativeDisplacement_=" << currentMaxRelativeDisplacement_ << std::endl; 
    std::cout << "Mdouble totalCurrentMaxRelativeDisplacement_=" << totalCurrentMaxRelativeDisplacement_ << std::endl; 
    std::cout << "bool gridNeedsUpdate_=" << gridNeedsUpdate_ << std::endl; 
    std::cout << "bool updateEachTimeStep_=" << updateEachTimeStep_ << std::endl; 
    std::cout << "unsigned int hGridMaxLevels_=" << hGridMaxLevels_ << std::endl; 
    std::cout << "Mdouble hGridCellOverSizeRatio_=" << hGridCellOverSizeRatio_ << std::endl; 
    if (grid != nullptr) 
    {
        grid->info();
    } else
    {
        std::cout << "hGrid does not exist yet" << std::endl;    
    }
}
