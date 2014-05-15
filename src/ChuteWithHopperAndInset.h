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

#ifndef CHUTEWITHHOPPERANDINSET_H
#define CHUTEWITHHOPPERANDINSET_H
#include "ChuteWithHopper.h"
#include "Statistics.h"

class ChuteWithHopperAndInset : public ChuteWithHopper {

///protected variables
protected: 

/// private variables
private:
	/// The height of the inset
	Mdouble insetHeight;
	///The width of the inset
	Mdouble insetWidth;
	/// The angle of the inset (input in degrees, usage in radians)
	Mdouble insetAngle;

/// public variables
public:

	///The default constructor  
	ChuteWithHopperAndInset() {constructor();}
	
	///The actually constructor
	void constructor()
		{
		///in here all the variables get a default value
		insetHeight=0.2;
		insetWidth=0.1;
		insetAngle=15.0*constants::pi/180.0;
		
		}
	///todo check wether the inset and hopper are colliding 
	///todo check wether the resulting opening between the hopper and 
	///the inset is 
	
	/// set function for insetHeight, insetWidth, insetAngle
	void set_Inset(double height, double width, double angle){
		if (width>=0.0) {insetWidth = width;} else std::cerr << "WARNING : Inset width must be greater than or equal to zero" << std::endl;
		if (height>=0.0) {insetHeight = height;} else std::cerr << "WARNING : Inset height must be greater than or equal to zero" << std::endl;
		if (angle>0.0&&angle<90.0) {insetAngle = angle*constants::pi/180.0;} else std::cerr << "WARNING : Inset angle must be widthin (0,90)" << std::endl;
	}
	
	/// get function for insetHeight, insetWidth, insetAngle
	double get_InsetHeight(){return insetHeight;}
	double get_InsetWidth(){return insetWidth;}
	double get_InsetAngle(){return insetAngle;}
	
	virtual void setup_particles_initial_conditions()
	{
		ChuteWithHopper::setup_particles_initial_conditions();
		add_Inset();	
	}	
	
	void add_Inset() {
		//std::cout<<"in ChuteWithHopperAndInset::add_Inset()"<<std::endl;
		int n = get_NWall();
		set_NWall(n+2);
		
		double s = sin(get_ChuteAngle());
		double c = cos(get_ChuteAngle());
		
		Vec3D A, B, C, temp, normal;
		
		///The points A, B, C should be the corners of the triangular 
		///finite wall in clockwise order. 
		
		///Define the three points between which the first finite wall is created
		A = Vec3D(insetWidth, 0.0, insetHeight-insetWidth*(sin(insetAngle)/cos(insetAngle)));
		B = Vec3D(insetWidth, 0.0, 0.0);
		C = Vec3D(0.0, 0.0, 0.0);
				
		//std::cout<<"PUNTEN ABC INSET, A; "<<A<<", B: "<<B<<", C: "<<C<<std::endl;
		
		///Move points A,B,C a distance 'shift' down the chute, so the inset starts at the beginning of the chute
		//std::cout<<"dit zijn shift, A, B, C "<<shift<<' '<<A<<' '<<B<<' '<<C<<' '<<std::endl;
		A.X+= shift;
		B.X+= shift;
		C.X+= shift;
		//std::cout<<"dit zijn A, B, C na shift "<<A<<' '<<B<<' '<<C<<' '<<std::endl;
		
		//std::cout<<"Verplaatsen ???: "<<0.5*(HopperLength-HopperExitLength)<<std::endl;
		//std::cout<<" "<<std::endl;
		//std::cout<<"ChuteWithHopperAndInset::add_Inset:"<<std::endl;
		//std::cout<<"HopperHeight: 	  "<<HopperHeight<<std::endl;
		//std::cout<<"HopperExitLength: "<<HopperExitLength<<std::endl;
		//std::cout<<"HopperExitHeight: "<<HopperExitHeight<<std::endl;
		//std::cout<<"HopperAngle: 	  "<<HopperAngle<<std::endl;
		//std::cout<<"HopperLength: 	  "<<HopperLength<<std::endl;
		//std::cout<<" "<<std::endl;
		
		//double iets=0.5*(HopperLength-HopperExitLength); 
		//A = A+iets;
		//B = B+iets;
		//C = C+iets;
		
		
		///create a finite wall from B to A, from C to B and from A to C
		temp = B-A;
		normal  = Vec3D(temp.Z,0.0,-temp.X) / sqrt(temp.GetLength2());
		Walls[n].add_finite_wall(normal, Dot(normal,A));
		temp = C-B;
		normal  = Vec3D(temp.Z,0.0,-temp.X) / sqrt(temp.GetLength2());
		Walls[n].add_finite_wall(normal, Dot(normal,B));
		temp = A-C;
		normal = Vec3D(temp.Z,0.0,-temp.X)/sqrt(temp.GetLength2());
		Walls[n].add_finite_wall(normal,Dot(normal,C));
		
		
		
		
		///Define the three points between which the second finite wall is created
		A = Vec3D(insetWidth, 0.0, insetHeight-insetWidth*(sin(insetAngle)/cos(insetAngle)));
		B = Vec3D(0.0, 0.0, 0.0);
		C = Vec3D(0.0, 0.0, insetHeight);
		
		///Move points A,B,C a distance 'shift' down the chute, so the inset starts at the beginning of the chute
		A.X+= shift;
		B.X+= shift;
		C.X+= shift;
		
		///create a finite wall from B to A, from C to B and from A to C
		temp = B-A;
		normal  = Vec3D(temp.Z,0.0,-temp.X) / sqrt(temp.GetLength2());
		Walls[n+1].add_finite_wall(normal, Dot(normal,A));
		temp = C-B;
		normal  = Vec3D(temp.Z,0.0,-temp.X) / sqrt(temp.GetLength2());
		Walls[n+1].add_finite_wall(normal, Dot(normal,B));
		temp = A-C;
		normal = Vec3D(temp.Z,0.0,-temp.X)/sqrt(temp.GetLength2());
		Walls[n+1].add_finite_wall(normal,Dot(normal,C));
		
	}
	
	virtual void print(std::ostream& os,bool print_all) {
		ChuteWithHopper::print(os,print_all); 
		os 
			<< "insetHeight:" << insetHeight 
			<< ", insetWidth:" << insetWidth 
			<< ", insetAngle:" << insetAngle 
			<< ", insetAngleDeg:" << insetAngle/constants::pi*180.0
			<< std::endl;
	}
	/// Give a warning somewhere when the hopper is not raised (variable: ChuteWithHopper::lift) enough and the finite wall (inset) and hopper are colliding
};
#endif
