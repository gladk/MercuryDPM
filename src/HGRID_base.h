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
