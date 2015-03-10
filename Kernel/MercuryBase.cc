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
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "MercuryBase::MercuryBase() finished"<<std::endl;
#endif
}

MercuryBase::~MercuryBase()
{
    if (grid)
    {
        delete grid;
        grid = nullptr;
    }
}

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
    
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "HGRID_base(HGrid_base& other) finished " << std::endl;
#endif
}

/// This is the actually constructor for this problem. It is defined here because it is called by both the copy constructor and normal constructor
void MercuryBase::constructor()
{
    ///Set sensible default values for all the required parameters
    grid = nullptr;
    gridNeedsUpdate_ = true;
    hGridMaxLevels_ = 3;
    hGridCellOverSizeRatio_ = 1.0;
    updateEachTimeStep_ = true;
    hGridDistribution_ = EXPONENTIAL;
    hGridMethod_ = TOPDOWN;
}

///This sets up the broad phase information, has to be done at this stage becuase it requires the partcle size
void MercuryBase::hGridActionsBeforeTimeLoop()
{
}

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

void MercuryBase::write(std::ostream& os, bool writeAllParticles) const
        {
    DPMBase::write(os, writeAllParticles);
    if (grid != 0)
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

Mdouble MercuryBase::getHGridCurrentMaxRelativeDisplacement() const
{
    return currentMaxRelativeDisplacement_;
}

Mdouble MercuryBase::getHGridTotalCurrentMaxRelativeDisplacement() const
{
    return totalCurrentMaxRelativeDisplacement_;
}

void MercuryBase::setHGridUpdateEachTimeStep(bool updateEachTimeStep)
{
    updateEachTimeStep_ = updateEachTimeStep;
}

bool MercuryBase::getHGridUpdateEachTimeStep() const
{
    return updateEachTimeStep_;
}

///This set up the Broad grid
void MercuryBase::hGridRebuild()
{
    //Create grid object
    if (grid != nullptr)
    {
        delete grid;
    }
    
    std::vector<double> cellSizes;
    
    double minParticleInteractionRadius = getHGridTargetMinInteractionRadius();
    double maxParticleInteractionRadius = getHGridTargetMaxInteractionRadius();
    switch (getHGridDistribution())
    {
        case LINEAR:
            {
            double minCellSize = nextafter(2.0 * minParticleInteractionRadius * getHGridCellOverSizeRatio(), 0.0);
            double maxCellSize = nextafter(2.0 * maxParticleInteractionRadius * getHGridCellOverSizeRatio(), std::numeric_limits<double>::max());
            
            //std::cout << "HGrid: using a linear cell size distribution from " << minCellSize << " to " << maxCellSize << " over " << getHGridMaxLevels() << " levels" << std::endl;
            for (unsigned int i = 0; i < getHGridMaxLevels() - 1; i++)
            {
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
            for (unsigned int i = 0; i < getHGridMaxLevels() - 1; i++)
            {
                cellSizes.push_back(minCellSize * std::pow(maxCellSize / minCellSize, (i + 1.0) / getHGridMaxLevels()));
            }
            //The last cell is added separately because in some cases accuracy was lost when calculating it.
            cellSizes.push_back(maxCellSize);
            break;
        }
        case USER:
            {
            for (unsigned int i = 0; i < getHGridMaxLevels(); i++)
            {
                cellSizes.push_back(userHGridCellSize(i));
            }
            break;
        }
        case OLDHGRID:
            {
            double minCellSize = nextafter(2.0 * minParticleInteractionRadius * getHGridCellOverSizeRatio(), 0.0);
            
            //std::cout<<"HGrid: using the old HGrid cell size distribution starting from " <<minCellSize<<std::endl;
            for (unsigned int i = 0; i < getHGridMaxLevels(); i++)
            {
                cellSizes.push_back(minCellSize * std::pow(2, i));
            }
            break;
        }
    }
    
    grid = new HGrid(getHGridTargetNumberOfBuckets(), getHGridCellOverSizeRatio(), cellSizes);
    
    for (std::vector<BaseParticle*>::iterator it = particleHandler.begin(); it != particleHandler.end(); ++it)
    {
        hGridInsertParticle(*it);
///todo{This is really ugly fix to force the particle to update}
        (*it)->setHGridX(9999);
        hGridUpdateParticle(*it);
    }
    gridNeedsUpdate_ = false;
}

void MercuryBase::hGridInsertParticle(BaseParticle *obj)
{
    if (grid)
    {
        grid->insertParticleToHgrid(obj);
    }
}

#ifdef CONTACT_LIST_HGRID
void MercuryBase::broadPhase(BaseParticle *i UNUSED)
{
}
#else
void MercuryBase::broadPhase(BaseParticle *i)
{
    hGridFindOneSidedContacts(i);
}
#endif

///This is the actions before the time step, it basically compute which cell each particle is in
void MercuryBase::hGridActionsBeforeTimeStep()
{
    static int stepsBeforeUpdate = 0;
    if (hGridNeedsRebuilding())
    {
        //std::cout<<"HGrid needs rebuilding for void HGRID_base::hGridActionsBeforeTimeStep()"<<std::endl;
        hGridRebuild();
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
    currentMaxRelativeDisplacement_ = 0;
}

void MercuryBase::hGridActionsAfterIntegration()
{
    totalCurrentMaxRelativeDisplacement_ += 2.0 * currentMaxRelativeDisplacement_;
}

HGrid* MercuryBase::getHGrid()
{
    return grid;
}

const HGrid* MercuryBase::getHGrid() const
{
    return grid;
}

double MercuryBase::userHGridCellSize(unsigned int level)
{
    std::cerr << "In double MercuryBase::userHGridCellSize(unsigned int level) with level=" << level << std::endl;
    std::cerr << "If you want to use user defined HGrid cell sizes, this function should be redefined" << std::endl;
    return 0.0;
}

int MercuryBase::readNextArgument(int& i, int argc, char *argv[])
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

HGridMethod MercuryBase::getHGridMethod() const
{
    return hGridMethod_;
}

void MercuryBase::setHGridMethod(HGridMethod hGridMethod)
{
    hGridMethod_ = hGridMethod;
}

HGridDistribution MercuryBase::getHGridDistribution() const
{
    return hGridDistribution_;
}

void MercuryBase::setHGridDistribution(HGridDistribution hGridDistribution)
{
    if (hGridDistribution_ != hGridDistribution)
    {
        gridNeedsUpdate_ = true;
        hGridDistribution_ = hGridDistribution;
    }
}

Mdouble MercuryBase::getHGridCellOverSizeRatio() const
{
    return hGridCellOverSizeRatio_;
}

void MercuryBase::setHGridCellOverSizeRatio(Mdouble hGridCellOverSizeRatio)
{
    if (std::abs(hGridCellOverSizeRatio_-hGridCellOverSizeRatio)<1e-10)
    {
        gridNeedsUpdate_ = true;
        hGridCellOverSizeRatio_ = hGridCellOverSizeRatio;
    }
}

void MercuryBase::setHGridMaxLevels(unsigned int hGridMaxLevels)
{
    if (hGridMaxLevels_ != hGridMaxLevels)
    {
        gridNeedsUpdate_ = true;
        hGridMaxLevels_ = hGridMaxLevels;
    }
}

unsigned int MercuryBase::getHGridMaxLevels() const
{
    return hGridMaxLevels_;
}
bool MercuryBase::hGridNeedsRebuilding()
{
    if (grid == nullptr)
    {
        //std::cout << "HGrid needs updating, because there is no grid." << std::endl;
        return true;
    }
    else if (gridNeedsUpdate_)
    {
        //std::cout << "HGrid needs updating, because some of its initialisation parameters have changed." << std::endl;
        return true;
    }
    else if (grid->getNeedsRebuilding())
    {
        //std::cout << "HGrid needs updating, because said so by the grid itself" << std::endl;
        return true;
    }
    else if (getHGrid()->getNumberOfBuckets() > 10 * getHGridTargetNumberOfBuckets() || 10 * getHGrid()->getNumberOfBuckets() < getHGridTargetNumberOfBuckets())
    {
        //std::cout << "HGrid needs updating, because of number of buckets, current=" << grid->NUM_BUCKETS << " target=" << particleHandler.getNumberOfObjects() << std::endl;
        return true;
    }
    else if (particleHandler.getLargestParticle() != nullptr && 2.0 * particleHandler.getLargestParticle()->getInteractionRadius() > getHGrid()->getCellSizes().back() * grid->getCellOverSizeRatio())
    {
        //std::cout << "HGrid needs updating, because of maximum cell size, current=" << grid->cellSizes_.back() * grid->cellOverSizeRatio_ << " required=" << 2.0 * particleHandler.getLargestParticle()->getInteractionRadius() << std::endl;
        return true;
    }
    else
    {
        //std::cout<<"HGrid does not need updating, because of number of buckets, current="<<grid->NUM_BUCKETS<<" target="<<particleHandler.getNumberOfObjects()<<std::endl;
        //std::cout<<"HGrid does not need updating, because of maximum cell size, current="<<grid->cellSizes_.back()*grid->cellOverSizeRatio_<<" required="<<2.0*particleHandler.getLargestParticle()->getInteractionRadius()<<std::endl;
        return false;
    }
}

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

bool MercuryBase::checkParticleForInteraction(const BaseParticle& p)
{
    Mdouble distance;
    Vec3D normal;

    //Check if it has no collision with walls
    for (std::vector<BaseWall*>::const_iterator it = wallHandler.begin(); it != wallHandler.end(); it++)
    {
        if ((*it)->getDistanceAndNormal(p, distance, normal))
        {
            //std::cout<<"failure: Collision with wall: "<<**it<<std::endl;
            return false;
        }
        else
        {
            //std::cout<<"No collision with wall: "<<**it<<std::endl;
        }
    }
    
    //Check if it has no collision with other particles
    if (hGridHasParticleContacts(&p))
    {
        //std::cout<<"failure: Collision with particle"<<std::endl;
        return false;
    }
    else
    {
        //std::cout<<"No collision with particles"<<std::endl;
    }
    return true;
}

