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

#ifndef HGRID_2D_H
#define HGrid_2D_H

#include "HGRID_base.h"
#include <iostream>

///This adds on the hierarchical grid code for 2D problems.
class HGRID_2D : public HGRID_base
	{
public:

	///This is the default constructor. All it does is set senible defaults.
	HGRID_2D()
	{
        constructor();
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "HGRID_2D() finished"<<std::endl;
		#endif
	}
	
	///Copy-constructor for creates an HGRID problem from an existing MD problem
	//The copy-constructor of MD has to be called because the link from MD to HGRID_base is virtual
	HGRID_2D(MD& other) : MD(other), HGRID_base(other)
	{
		constructor();
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "HGRID_2D(MD& other) finished" << std::endl;
		#endif
	}
	HGRID_2D(HGRID_base& other) : MD(other), HGRID_base(other)
	{
		constructor();
		#ifdef CONSTUCTOR_OUTPUT
			std::cerr << "HGRID_2D(HGRID_base& other) finished" << std::endl;
		#endif
	}
	
	///This is the actually constructor it is called do both constructors above.
	void constructor() {}

protected:

	///This adds a partcile to the Grid, called in the grid setup routies
	void HGRID_UpdateParticleInHgrid  (BaseParticle *obj);
	void HGRID_RemoveParticleFromHgrid(BaseParticle *obj);

	///Check collisions for a general cell
	virtual void CheckCell(int x, int y, int l, BaseParticle *obj, HGrid *grid);

	///Checks for a collision in the particles own cell
	virtual void CheckCell_current(int x, int y, int l, HGrid *grid);

	///Check if an Particle has a collision in the grid; avoids multiple checks
	void CheckObjAgainstGrid(HGrid *grid, BaseParticle *obj);
		
	///Check if an Particle has a collision in the grid
	void CheckObjAgainstWholeGrid(HGrid *grid, BaseParticle *obj);

	/// Tests obj against all particles in cell
	/// similar to CheckCell, but links to TestObject instead of compute_internal_forces
	bool TestCell(int x, int y, int l, BaseParticle *obj, HGrid *grid);

	/// Tests obj against all neighbouring particles
	/// similar to CheckObjAgainstGrid, but links to TestCell instead of CheckCell
	bool TestObjAgainstGrid(HGrid *grid, BaseParticle *obj);

#ifdef ContactListHgrid	
	void InsertCell(int x, int y, int l, Particle *obj, HGrid *grid);
	void InsertObjAgainstGrid(HGrid *grid, Particle *obj);
#endif
	};
//end class defintion

#endif
