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

#include "HGRID_base.h"

HGRID_base::HGRID_base()
{
    constructor();
    #ifdef CONSTUCTOR_OUTPUT
        std::cerr << "HGRID_base() finished"<<std::endl;
    #endif
}

HGRID_base::~HGRID_base() {	
    if (grid) { delete grid; grid=NULL; } 
}

HGRID_base::HGRID_base(MD& other) : MD(other)
{
    constructor();
    #ifdef CONSTUCTOR_OUTPUT
        std::cerr << "HGRID_base(MD& other) finished " << std::endl;
    #endif
}

/// This is the actually constructor for this problem. It is defined here because it is called by both the copy constructor and normal constructor
void HGRID_base::constructor()
{
	///Set senible default values for all the required parameters
	NUM_BUCKETS=250000;
	hGridMaxLevels_=3;
	grid = NULL;
    
    cellOverSizeRatio_=1.0;
    
	setHGridUpdateEachTimeStep(true);
    setHGridDistribution(EXPONENTIAL);
    setHGridMethod(TOPDOWN);
    
}


///This sets up the broad phase information, has to be done at this stage becuase it requires the partcle size
void HGRID_base::HGRID_actions_before_time_loop()
{
    InitBroadPhase();
}

void HGRID_base::set_HGRID_num_buckets(unsigned int new_num_buckets)
{
    if (new_num_buckets>0)
    {
        NUM_BUCKETS=new_num_buckets;
    }
    else
    {
        std::cerr << "Error in set_HGRID_num_buckets" << std::endl;
        exit(-1);
    }
}

void HGRID_base::set_HGRID_num_buckets_to_power()
{
    set_HGRID_num_buckets_to_power(getParticleHandler().getNumberOfObjects());
}

void HGRID_base::set_HGRID_num_buckets_to_power(unsigned int N)
{
    unsigned int NUM_BUCKETS = 32;
    while (NUM_BUCKETS<2*N)
    {
        NUM_BUCKETS *= 2;
    }
    set_HGRID_num_buckets(NUM_BUCKETS);
}

void HGRID_base::read(std::istream& is)
{ 
    MD::read(is); 

    std::stringstream line (std::stringstream::in | std::stringstream::out);
    getLineFromStringStream(is,line);

    std::string dummy;
    line>> dummy >> NUM_BUCKETS 
        >> dummy >> hGridMaxLevels_
        >> dummy >> cellOverSizeRatio_;

}

void HGRID_base::write(std::ostream& os)
{ 
    MD::write(os); 
    os	<< "NUM_BUCKETS " << NUM_BUCKETS << " "
        << "hGridMaxLevels " << hGridMaxLevels_ << " "
        << "cellOverSizeRatio " << cellOverSizeRatio_ << std::endl;
}

void HGRID_base::print(std::ostream& os, bool print_all=false)
{
    MD::print(os,print_all);
    os << " NUM_BUCKETS:" << NUM_BUCKETS << ", hGridMaxLevels:" << hGridMaxLevels_ <<", cellOverSizeRatio:" << cellOverSizeRatio_<<std::endl;
}

Mdouble HGRID_base::getHGridCurrentMaxRelativeDisplacement()
{
    return currentMaxRelativeDisplacement_;
}

Mdouble HGRID_base::getHGridTotalCurrentMaxRelativeDisplacement()
{
    return totalCurrentMaxRelativeDisplacement_;
}

void HGRID_base::setHGridUpdateEachTimeStep(bool updateEachTimeStep)
{
    updateEachTimeStep_=updateEachTimeStep;
}

bool HGRID_base::getHGridUpdateEachTimeStep(){
    return updateEachTimeStep_;
}	 

///This set up the Broad grid
void HGRID_base::InitBroadPhase() 
{
    std::vector<double> cellSizes;
    switch(getHGridDistribution())
    {
        case LINEAR:
        {
            double minCellSize=0.99999*2.0*getSmallestParticle()->get_InteractionRadius()*cellOverSizeRatio_;
            double maxCellSize=1.00001*2.0*getLargestParticle()->get_InteractionRadius()*cellOverSizeRatio_;
            std::cout<<"HGrid: using a linear cell size distribution from "<<minCellSize<<" to "<<maxCellSize<<" over "<<hGridMaxLevels_<<" levels"<<std::endl;
            for (int i=0; i<hGridMaxLevels_; i++)
            {
                cellSizes.push_back(minCellSize+(maxCellSize-minCellSize)*(i+1.0)/hGridMaxLevels_);
            }
            break;            
        }
        case EXPONENTIAL:
        {
            double minCellSize=0.99999*2.0*getSmallestParticle()->get_InteractionRadius()*cellOverSizeRatio_;
            double maxCellSize=1.00001*2.0*getLargestParticle()->get_InteractionRadius()*cellOverSizeRatio_;
            std::cout<<"HGrid: using an exponential cell size distribution from "<<minCellSize<<" to "<<maxCellSize<<" over "<<hGridMaxLevels_<<" levels"<<std::endl;
            for (int i=0; i<hGridMaxLevels_; i++)
            {
                cellSizes.push_back(minCellSize*pow(maxCellSize/minCellSize,(i+1.0)/hGridMaxLevels_));
            }
            break;
        }
        case OLDHGRID:
        {
            double minCellSize=2.0*getSmallestParticle()->get_InteractionRadius()*cellOverSizeRatio_;
            for (int i=0; i<hGridMaxLevels_; i++)
            {
                cellSizes.push_back(minCellSize*pow(2,i));
            }
            break;            
        }
        
    }

	//Create grid object
	if (grid!=NULL)
    {
        delete grid;
    }
	grid = new HGrid(NUM_BUCKETS, cellOverSizeRatio_, cellSizes);
	
	grid->occupiedLevelsMask = 0;
	
	grid->Initialize_inv_size();

	//New steps in initbroad_phase is to not only Insert the Particles in the grid but also update them
	for (int i=0; i<grid->NUM_BUCKETS; i++)
	{
		grid->objectBucket[i]		= NULL;
		grid->bucketIsChecked[i]	= false;
	}
	
	for (std::vector<BaseParticle*>::iterator it = getParticleHandler().begin(); it!=getParticleHandler().end(); ++it)
	{
		grid->InsertParticleToHgrid(*it);
/// /todo{This is really ugly}
		(*it)->set_HGRID_x(9999);
		HGRID_UpdateParticleInHgrid(*it);
	}

}

void HGRID_base::HGRID_InsertParticleToHgrid(BaseParticle *obj)
{
	grid->InsertParticleToHgrid(obj);
}


///This is the actions before the time step, it basically compute which cell each particle is in
void HGRID_base::HGRID_actions_before_time_step()
{
	static int stepsBeforeUpdate=0;
	
	for (int i=0; i<grid->NUM_BUCKETS; i++)
	{
		grid->bucketIsChecked[i]	= false;
	}

	if(getHGridUpdateEachTimeStep()||totalCurrentMaxRelativeDisplacement_>=cellOverSizeRatio_-1)
	{
#ifndef ContactListHgrid
		for (int i=0; i<grid->NUM_BUCKETS; i++)
		{
			grid->objectBucket[i]		= NULL;
		}
#endif
		totalCurrentMaxRelativeDisplacement_=0;
		for (std::vector<BaseParticle*>::iterator it = getParticleHandler().begin(); it!=getParticleHandler().end(); ++it)
		{
			HGRID_UpdateParticleInHgrid(*it);
		}
		stepsBeforeUpdate=0;
	}
	else
	{
		stepsBeforeUpdate++;
	}
}

bool HGRID_base::TestObject(BaseParticle *pI, BaseParticle *pJ)
{
    //PI==PJ check is required because this function is called for all possible close Particle combination (even itself)
    return pI->get_Index()==pJ->get_Index() || GetDistance2(pI->get_Position(),pJ->get_Position())>=sqr(pI->get_InteractionRadius()+pJ->get_InteractionRadius());
}


void HGRID_base::HGRID_update_move(BaseParticle * iP, Mdouble move)
{
    Mdouble currentRelativeDisplacement=move/(grid->cellSizes_[iP->get_HGRID_Level()]);
    if(currentRelativeDisplacement>currentMaxRelativeDisplacement_)
    {
        currentMaxRelativeDisplacement_=currentRelativeDisplacement;
    }
}
	
void HGRID_base::HGRID_actions_before_integration()
{
    currentMaxRelativeDisplacement_=0;
}

void HGRID_base::HGRID_actions_after_integration()
{
    totalCurrentMaxRelativeDisplacement_+=2.0*currentMaxRelativeDisplacement_;
}

int HGRID_base::readNextArgument(unsigned int& i, unsigned int argc, char *argv[])
{
	if (!strcmp(argv[i],"-NUM_BUCKETS"))
    {
		NUM_BUCKETS = atoi(argv[i+1]);
	}
    else if (!strcmp(argv[i],"-hGridMaxLevels"))
    {
		hGridMaxLevels_ = atoi(argv[i+1]);
	}
    else if (!strcmp(argv[i],"-cellOverSizeRatio"))
    {
		cellOverSizeRatio_ = atof(argv[i+1]);
	}
    else
    {
        return MD::readNextArgument(i, argc, argv); //if argv[i] is not found, check the commands in HGRID_3D
    }
	return true; //returns true if argv[i] is found
}

HGridMethod HGRID_base::getHGridMethod()
{
    return hGridMethod_;
}

void HGRID_base::setHGridMethod(HGridMethod hGridMethod)
{
    hGridMethod_=hGridMethod;
}

HGridDistribution HGRID_base::getHGridDistribution()
{
    return hGridDistribution_;
}

void HGRID_base::setHGridDistribution(HGridDistribution hGridDistribution)
{
    hGridDistribution_=hGridDistribution;
}

Mdouble HGRID_base::getHGridCellOverSizeRatio()
{
    return cellOverSizeRatio_;
}

void HGRID_base::setHGridCellOverSizeRatio(Mdouble cellOverSizeRatio)
{
    cellOverSizeRatio_=cellOverSizeRatio;
}

void HGRID_base::setHGridMaxLevels(int hGridMaxLevels)
{
    if (hGridMaxLevels>0)
    {
        hGridMaxLevels_=hGridMaxLevels;
    }
    else
    {
        std::cout<<"Error in void HGRID_base::set_HGridMaxLevel(int hGridMaxLevels), hGridMaxLevels should be strictly positive, while it is now "<<hGridMaxLevels<<std::endl;
    }
}

int HGRID_base::getHGridMaxLevels()
{
    return hGridMaxLevels_;
}
