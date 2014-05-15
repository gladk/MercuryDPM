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

#ifndef HGRID_3D_H
#define HGRID_3D_H


#include "HGRID_base.h"
#include <iostream>

///This adds on the hierarchical grid code for 3D problems.

class HGRID_3D : public HGRID_base
	{
public:

	///This is the default constructor. All it does is set senible defaults.
	HGRID_3D()
	{
        constructor();
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "HGRID_3D() finished"<<std::endl;
		#endif
	}
	
	///\bug Why is the constructor call commented out. This does not make sence to be Anthony. Dinant: I think this has something to do with somebody copying a 2D MD problem into a 3D Hgrid, then the 3D_Hgrid constructor changes properties of the MD class. Solution in my opinion is to change the HGRID_3D::constructor()

	///Copy-constructor for creates an HGRID problem from an existing MD problem

	//The copy-constructor of MD has to be called because the link from MD to HGRID_base is virtual
	HGRID_3D(MD& other) : MD(other), HGRID_base(other)
	{
		/*constructor();*/
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "HGRID_3D(MD& other) finished"<<std::endl;
		#endif	
	}
	HGRID_3D(HGRID_base& other) : MD(other), HGRID_base(other)
	{
		/*constructor();*/
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "HGRID_3D(HGRID_base& other) finished"<<std::endl;
		#endif	
	}
	
	///This is the actually constructor it is called do both constructors above.
	void constructor(){
		set_dim_particle(3);
		set_dim(3);
	}

protected:

	///This adds a partcile to the Grid, called in the grid setup routies
	void HGRID_UpdateParticleInHgrid  (BaseParticle *obj);
	void HGRID_RemoveParticleFromHgrid(BaseParticle *obj);

	///Check collisions for a general cell
	virtual void CheckCell(int x, int y, int z, int l, BaseParticle *obj, HGrid *grid);

	///Checks for a collision in the particles own cell
	virtual void CheckCell_current(int x, int y, int z, int l, HGrid *grid);

	///Check if an Particle has a collision in the grid; avoids multiple checks
	void CheckObjAgainstGrid(HGrid *grid, BaseParticle *obj);
		
	///Check if an Particle has a collision in the grid
	void CheckObjAgainstWholeGrid(HGrid *grid, BaseParticle *obj);

	/// Tests obj against all particles in cell
	/// similar to CheckCell, but links to TestObject instead of compute_internal_forces
	bool TestCell(int x, int y, int z, int l, BaseParticle *obj, HGrid *grid);

	/// Tests obj against all neighbouring particles
	/// similar to CheckObjAgainstGrid, but links to TestCell instead of CheckCell
	bool TestObjAgainstGrid(HGrid *grid, BaseParticle *obj);

#ifdef ContactListHgrid	
	void InsertCell(int x, int y, int z, int l, Particle *obj, HGrid *grid);
	void InsertObjAgainstGrid(HGrid *grid, BaseParticle *obj);
#endif

	};
//end class defintion

#endif
