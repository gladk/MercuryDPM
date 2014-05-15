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

#ifndef HGRIDBASE_H
#define HGRIDBASE_H

#include <ostream>
#include <iostream>
#include <stdlib.h>

#include "MD.h"
#include "HGRID.h"

enum HGridMethod {BOTTOMUP,TOPDOWN};
enum HGridDistribution {OLDHGRID,LINEAR,EXPONENTIAL,PARTICLESPERCELL,OPTIMAL};
	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///This is the base class for both #HGRID_2D and #HGRID_3D. Note the actually abstrate grid is defined in the case Grid defined below
///\todo Remove the dimension out of this class by templating and use of for loops for the check cells part
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class HGRID_base : public virtual MD
{
  public:
	
	///This is the default constructor. All it does is set senible defaults.
	HGRID_base();

    ///This is the default destructor.
	~HGRID_base();
	
	///Copy-constructor for creates an HGRID problem from an existing MD problem
	HGRID_base(MD& other);
	
	///This is the actually constructor it is called do both constructors above.
	void constructor();
	
	///This sets up the broad phase information, has to be done at this stage becuase it requires the partcle size
	void HGRID_actions_before_time_loop();
	
	///This resets all the bucket information
	void HGRID_actions_before_time_step();
	
	///This sets the number of buckets for the HGRID
	void set_HGRID_num_buckets(unsigned int new_num_buckets);
	
	///set number of buckets to the smallest power of two bigger than the number of particles
	void set_HGRID_num_buckets_to_power();
	
	///set number of buckets to the smallest power of two bigger than N
	void set_HGRID_num_buckets_to_power(unsigned int N);
		
	///This function reads all HGRID data
	void read(std::istream& is);
	
	///This function writes all HGRID data
	void write(std::ostream& os);
	
	///This function outputs all HGRID data
    void print(std::ostream& os, bool print_all);
  
	Mdouble getHGridCurrentMaxRelativeDisplacement();
	Mdouble getHGridTotalCurrentMaxRelativeDisplacement();
	
	void setHGridUpdateEachTimeStep(bool updateEachTimeStep);
	bool getHGridUpdateEachTimeStep();
    
	void setHGridMaxLevels(int HGridMaxLevels);
	int getHGridMaxLevels();    
    
    HGridMethod getHGridMethod();
    void setHGridMethod(HGridMethod hGridMethod);
    
    HGridDistribution getHGridDistribution();
    void setHGridDistribution(HGridDistribution hGridDistribution);    
    
    Mdouble getHGridCellOverSizeRatio();
    void setHGridCellOverSizeRatio(Mdouble cellOverSizeRatio);    

  protected:
	
	///This sets up the parameters required for the contact model
	void InitBroadPhase();
	
	///Inserts a single Particle to current grid
	void HGRID_InsertParticleToHgrid(BaseParticle *obj);
			
	///This makes the board_phase of contact point at the HGRID code
#ifdef ContactListHgrid
	void broad_phase(BaseParticle *i UNUSED){};
#else
	void broad_phase(BaseParticle *i){CheckObjAgainstGrid(grid, i);}
#endif
	
	///This is a purly virtual function that checks if an object is in the grid, this code is again dim dependent so purly virtual at this stage.
	virtual void CheckObjAgainstGrid(HGrid *grid, BaseParticle *obj)=0;
	
	///criterium for inserting a particle (returns false, if particles overlap;)
	virtual bool TestObject(BaseParticle *pI, BaseParticle *pJ);

	void HGRID_update_move(BaseParticle * iP, Mdouble move);
	
	void HGRID_actions_before_integration();
    
	void HGRID_actions_after_integration();
    
	int readNextArgument(unsigned int& i, unsigned int argc, char *argv[]);
	
  public:
    HGrid* grid;
    
  private:
    HGridMethod hGridMethod_;
    HGridDistribution hGridDistribution_;
    
	Mdouble currentMaxRelativeDisplacement_;
    Mdouble totalCurrentMaxRelativeDisplacement_;
	
    bool updateEachTimeStep_;
	int NUM_BUCKETS;
	int hGridMaxLevels_;
	Mdouble cellOverSizeRatio_; //Can be set to give particles more space within a cell
};



#endif
