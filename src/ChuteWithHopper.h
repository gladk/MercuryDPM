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

#ifndef CHUTEWITHHOPPER_H
#define CHUTEWITHHOPPER_H
#include "Chute.h"
#include "FiniteWall.h"
//#include "Statistics.h"

///ChuteWithHopper has a hopper as inflow. The hopper has two parts as follows
///to create the finite hopper walls, we take vector between two wall points in xz-plane, then rotate clockwise and make unit length.
/// \image html hopper.jpg "Sketch of the hopper"
///  A,B,C denote three points on the left and right hopper walls which are used to construct the hopper
///       shift denotes the space by which the chute has to be shifted to the right such that the hopper is in the domain
/// Note the wall direction has to be set seperately either period of walls.
class ChuteWithHopper : public Chute {
protected: 
	///Dimension of the hopper in vertical direction
	Mdouble HopperLength;
	///Dimension of the hopper in horizontal direction
	Mdouble HopperHeight;
	///Angle between the two pieces of the hopper walls
	Mdouble HopperAngle;
	///Dimension of the hopper exit in vertical direction
	Mdouble HopperExitLength;
	///Dimension of the hopper exit in vertical direction
	Mdouble HopperExitHeight;
	///The x position where the Chute starts (defined as the beginning of the hopper)
	Mdouble shift;
	///Relative height (in [0,1)) above which the hopper is replenished with new particles
	Mdouble lowerFillHeight;
	///If this flag is set, the hopper will be constructed in the xy-center of the domain, and not next to the xmin-domain boundary; by default off
	bool centerHopper;
private:
	///This is the vertical distance the chute is lifted above the plane.
	Mdouble lift;
	
	///This is the dimension of the hopper, my default it is one dimensional and hence does not have side wall
	unsigned int hopper_dim;
	
	///This is the flag, which sets if the chute bottom is aligned with the hopper, by default it is
	bool align_base;
	
	///This is which percentage of the hopper is used for creating new partices;
	Mdouble fill_percent;
	
	///The z coordinate of the right C point (when the left C point is in the origin)
	Mdouble HopperLowestPoint;



public:
	
	///This is a copy constructor for Chute problems \bug This copy construct is untested
	ChuteWithHopper(Chute& other) : 		MD(other), Chute(other) {constructor();}
	ChuteWithHopper(HGRID_3D& other) : 		MD(other), Chute(other) {constructor();}
	ChuteWithHopper(HGRID_base& other) : 	MD(other), Chute(other) {constructor();}
	ChuteWithHopper(MD& other) : 			MD(other), Chute(other) {constructor();}
	
	///This is the default constructor  
	ChuteWithHopper() {constructor();}
	
	///This is the actually constructor, get called by all constructors above
	void constructor()
		{
		lowerFillHeight=0.5;
		lift=0.0;
		set_Hopper(0.01, 0.01, 60.0, 0.08, 0.04);
		shift = 0.0;	
		hopper_dim=1;
		align_base=true;

		fill_percent=50.0;
		centerHopper=false;
		
		}
		
	void set_HopperFillPercentage(Mdouble new_fill){fill_percent=new_fill;}
	
	virtual void setup_particles_initial_conditions()
	{
		Chute::setup_particles_initial_conditions();
		//std::cout << shift << " " << get_xmax() << " " << getParticleHandler().getNumberOfObjects() << std::endl;
		add_hopper();	
	}	
	
	/// This creates the hopper on top of the chute, see diagram in class description for details of the points.
	void add_hopper() {
		//hopper walls
		//to create the finite hopper walls, we take vector between two wall points in xz-plane, then rotate clockwise and make unit length
		// A\       /A  
		//   \     /       A,B,C denote three points on the left and right hopper walls which are used to construct the hopper
		//    \   /        shift denotes the space by which the chute has to be shifted to the right such that the hopper is in the domain
		//   B|   |B
		//    |   |
		//    |   |C
		//   C|   
		
		Vec3D A, B, C, temp, normal;

		Mdouble s = sin(get_ChuteAngle());
		Mdouble c = cos(get_ChuteAngle());
		HopperLowestPoint = HopperExitHeight - HopperExitLength * tan(ChuteAngle);
		
		///"0.5*(HopperLength+HopperExitLength) / tan(HopperAngle)" is the minimum heigth of the hopper, to make sure things should flow down and not to the sides. 
		//HopperHeight is now an input variable
		//HopperHeight = HopperLowestPoint + 1.1 * 0.5*(HopperLength+HopperExitLength) / tan(HopperAngle);

		Mdouble HopperCornerHeight = HopperHeight - 0.5*(HopperLength-HopperExitLength) / tan(HopperAngle);
		// Waarom had ik deze ook al weer gecomment?
		//if (HopperCornerHeight<=0.0) { HopperHeight += -HopperCornerHeight + P0.get_Radius(); HopperCornerHeight = P0.get_Radius(); }

		
		//first we create the left hopper wall
		
		//coordinates of A,B,C in (vertical parallel to flow,vertical normal to flow, horizontal) direction
		A = Vec3D(-0.5*(HopperLength-HopperExitLength), 0.0, HopperHeight);
		B = Vec3D(0.0, 0.0, HopperCornerHeight);
		C = Vec3D(0.0, 0.0, 0.0);
		
		
		
		//now rotate the coordinates of A,B,C to be in (x,y,z) direction
		A = Vec3D(c*A.X-s*A.Z, 0.0, s*A.X+c*A.Z);
		B = Vec3D(c*B.X-s*B.Z, 0.0, s*B.X+c*B.Z);
		C = Vec3D(c*C.X-s*C.Z, 0.0, s*C.X+c*C.Z);
		// the position of A determines shift and zmax
		///todo{Why shift by arbitrary number of 40, when centerHopper=True}
		if (centerHopper) set_shift(-A.X+40);
		else set_shift(-A.X);
		set_zmax(A.Z);
		A.X +=shift;
		B.X +=shift;
		C.X +=shift;
		
		//This lifts the hopper a distance above the chute
		A.Z+=lift;
		B.Z+=lift;
		C.Z+=lift;
		
		//create a finite wall from B to A and from C to B on the left hand side
		FiniteWall w_Left;
		temp = B-A;
		normal  = Vec3D(temp.Z,0.0,-temp.X) / sqrt(temp.GetLength2());
		w_Left.add_finite_wall(normal, Dot(normal,A));
		temp = C-B;
		normal  = Vec3D(temp.Z,0.0,-temp.X) / sqrt(temp.GetLength2());
		w_Left.add_finite_wall(normal, Dot(normal,B));
		temp = A-C;
		normal = Vec3D(temp.Z,0.0,-temp.X)/sqrt(temp.GetLength2());
		w_Left.add_finite_wall(normal,Dot(normal,C));
		getWallHandler().copyAndAddWall(w_Left);

		//next, do the same for the right wall
		A = Vec3D(HopperLength-0.5*(HopperLength-HopperExitLength), 0.0, HopperHeight);
		B = Vec3D(0.5*(HopperLength+HopperExitLength)-0.5*(HopperLength-HopperExitLength), 0.0, HopperCornerHeight);
		C = Vec3D(0.5*(HopperLength+HopperExitLength)-0.5*(HopperLength-HopperExitLength), 0.0, HopperLowestPoint);
	
		//This rotates the right points
		A = Vec3D(c*A.X-s*A.Z+shift, 0.0, s*A.X+c*A.Z);
		B = Vec3D(c*B.X-s*B.Z+shift, 0.0, s*B.X+c*B.Z);
		C = Vec3D(c*C.X-s*C.Z+shift, 0.0, s*C.X+c*C.Z);
		
		//This lifts the hopper a distance above the chute
		A.Z+=lift;
		B.Z+=lift;
		C.Z+=lift;
		
		//create a finite wall from B to A and from C to B on the right hand side
		FiniteWall w_Right;
		temp = A-B;
		normal  = Vec3D(temp.Z,0.0,-temp.X) / sqrt(temp.GetLength2());
		w_Right.add_finite_wall(normal, Dot(normal,A));
		temp = B-C;
		normal  = Vec3D(temp.Z,0.0,-temp.X) / sqrt(temp.GetLength2());
		w_Right.add_finite_wall(normal, Dot(normal,B));
		temp = C-A;
		normal  = Vec3D(temp.Z,0.0,-temp.X) / sqrt(temp.GetLength2());
		w_Right.add_finite_wall(normal, Dot(normal,C));
		getWallHandler().copyAndAddWall(w_Right);
		
		set_zmax(A.Z);
		
		if (hopper_dim == 2)
			{
				//coordinates of A,B,C in (vertical parallel to flow,vertical normal to flow, horizontal) direction
				A = Vec3D(0.0, (get_ymax()-get_ymin()-HopperLength)/2.0, HopperHeight);
				B = Vec3D(0.0, (get_ymax()-get_ymin()-HopperExitLength)/2.0, HopperCornerHeight);
				C = Vec3D(0.0, (get_ymax()-get_ymin()-HopperExitLength)/2.0, 0.0);
		
		
		
				//now rotate the coordinates of A,B,C to be in (x,y,z) direction		
				A = Vec3D(c*A.X-s*A.Z, A.Y, s*A.X+c*A.Z);
				B = Vec3D(c*B.X-s*B.Z, B.Y, s*B.X+c*B.Z);
				C = Vec3D(c*C.X-s*C.Z, C.Y, s*C.X+c*C.Z);
				// the position of A determines shift and zmax
				A.X +=shift;
				B.X +=shift;
				C.X +=shift;
		
				//This lifts the hopper a distance above the chute
				A.Z+=lift;
				B.Z+=lift;
				C.Z+=lift;
				
				
				
				//create a finite wall from B to A and from C to B
				FiniteWall w_Back;
				temp = B-A;
				normal=Cross(Vec3D(-c,0,-s),temp)/sqrt(temp.GetLength2());
				//normal  = Vec3D(0.0,temp.Z,-temp.Y) / sqrt(temp.GetLength2());
				w_Back.add_finite_wall(normal, Dot(normal,A));
				temp = C-B;
				//normal  = Vec3D(0.0,temp.Z,-temp.Y) / sqrt(temp.GetLength2());
				normal=Cross(Vec3D(-c,0,-s),temp)/sqrt(temp.GetLength2());
				w_Back.add_finite_wall(normal, Dot(normal,B));
				temp = A-C;
				//normal = Vec3D(0.0,temp.Z,-temp.Y)/sqrt(temp.GetLength2());
				normal=Cross(Vec3D(-c,0,-s),temp)/sqrt(temp.GetLength2());
				w_Back.add_finite_wall(normal,Dot(normal,C));
				getWallHandler().copyAndAddWall(w_Back);			
				
				//Now for the right y-wall
				A = Vec3D(0.0, (get_ymax()-get_ymin()+HopperLength)/2.0,HopperHeight);
				B = Vec3D(0.0, (get_ymax()-get_ymin()+HopperExitLength)/2.0,HopperCornerHeight);
				C = Vec3D(0.0, (get_ymax()-get_ymin()+HopperExitLength)/2.0,0.0);
				
				//now rotate the coordinates of A,B,C to be in (x,y,z) direction		
				A = Vec3D(c*A.X-s*A.Z, A.Y, s*A.X+c*A.Z);
				B = Vec3D(c*B.X-s*B.Z, B.Y, s*B.X+c*B.Z);
				C = Vec3D(c*C.X-s*C.Z, C.Y, s*C.X+c*C.Z);
				// the position of A determines shift and zmax
				A.X +=shift;
				B.X +=shift;
				C.X +=shift;
		
				//This lifts the hopper a distance above the chute
				A.Z+=lift;
				B.Z+=lift;
				C.Z+=lift;
				
				//create a finite wall from B to A and from C to B
				FiniteWall w_Front;
				temp = A-B;
				normal=Cross(Vec3D(-c,0,-s),temp)/sqrt(temp.GetLength2());
				//normal  = Vec3D(0.0,-temp.Z,temp.Y) / sqrt(temp.GetLength2());
				w_Front.add_finite_wall(normal, Dot(normal,A));
				temp = B-C;
				//normal  = Vec3D(0.0,-temp.Z,temp.Y) / sqrt(temp.GetLength2());
				normal=Cross(Vec3D(-c,0,-s),temp)/sqrt(temp.GetLength2());
				w_Front.add_finite_wall(normal, Dot(normal,B));
				temp = C-A;
				//normal = Vec3D(0.0,-temp.Z,temp.Y)/sqrt(temp.GetLength2());
				normal=Cross(Vec3D(-c,0,-s),temp)/sqrt(temp.GetLength2());
				w_Front.add_finite_wall(normal,Dot(normal,C));
				getWallHandler().copyAndAddWall(w_Front);
			}

		
		//now shift the fixed particles at the bottom so that they begin where the chute begins
		for (std::vector<BaseParticle*>::iterator it= getParticleHandler().begin(); it!=getParticleHandler().end(); ++it) { 
			(*it)->move(Vec3D(shift,0.0,0.0));
			#ifdef USE_SIMPLE_VERLET_INTEGRATION
				(*it)->movePrevious(Vec3D(shift,0.0,0.0));
			#endif
		}
	}
	
	
	///This creates an inflow particle in the top 50% of the hopper i.e. between gamma=0.5 and gamma=1.0
	/// Gamma is random number in the z direction and delta in the y direction
	/// In the 2D (hopper) case the particles are generated with equal probability in the y-direction, i.e. delta is from the edge of the domain
	/// In the 3D (hopper) case a third vector AD is generated and delta is again created for the sloping walls of the hopper
	/// \image html hopper_add_particle.jpg "Image shows the vectors in 2-dimension used to find a position inside the hopper"
	virtual	void create_inflow_particle()
	{
		
		
		//use this formula to obtain bidispersed particles
		//P0.Radius = random(0.0,1.0)<0.1?MinInflowParticleRadius:MaxInflowParticleRadius;
		
		//the following formula yields polydispersed particle radii:

		P0.set_Radius(random.get_RN(MinInflowParticleRadius,MaxInflowParticleRadius));
		P0.compute_particle_mass(Species);
		
		//Define an orthogonal coordinate system this is usful in the hopper, see diagram in html documentation for details.
		static Mdouble s = sin(get_ChuteAngle());
		static Mdouble c = cos(get_ChuteAngle());
		static Mdouble Ht = tan(HopperAngle);
		static Mdouble Hc = cos(HopperAngle);
		static Vec3D AB = Vec3D(c,0.0,s);
		static Vec3D AC = Vec3D(-s,0.0,c);
		static Vec3D AD = Vec3D(0.0,1.0,0.0);
		
		//Point A is located in the centre of the hopper.
		static Vec3D A = Vec3D
			(
			centerHopper?40:0.0, 
			(get_ymax()-get_ymin())/2.0, 
			s*(-0.5*(HopperLength-HopperExitLength)) + c*HopperHeight
			) 
			+ AB*0.5*HopperLength 
			+ AC*(-0.5*HopperLength/Ht);

		Mdouble gamma = random.get_RN((100.0-fill_percent)/100.0,1.0);

		
		//		Mdouble gamma = random(lowerFillHeight,1.0);

		Mdouble delta;
		
		if (hopper_dim==1)
			{
			
				
				//For the one dimensional delta is a random distance between the two walls the -minus 2 particle radii is to stop 
				/// \bug for periodic walls this should be only minus one particle radius, this should be fixed at some point.
				/// Thomas' response: using one particle radius gives problems when the wall is 
				/// not orthogonal to the y-direction; the distance has to be slightly higher than one; if you can figure out the exact value, then correct it please.
				//delta = random(ymin+P0.Radius,ymax-P0.Radius);

				delta = random.get_RN(-0.5,0.5)*(get_ymax()-get_ymin()-2.0*P0.get_Radius());


			}
		else
			{
				
				delta= (random.get_RN(-1.0,1.0)*(0.5*gamma*HopperLength -P0.get_Radius()/Hc));

			}
		P0.set_Position( A 
			+ AC * (gamma*0.5*HopperLength/Ht)

			+ AB * (random.get_RN(-1.0,1.0)*(0.5*gamma*HopperLength - P0.get_Radius()/Hc)) 
			+ AD*delta);
			
		
        P0.move(Vec3D(0.0,0.0,lift));
	}
	
	void set_HopperLowestPoint(Mdouble point){
		HopperLowestPoint=point;
	}
	Mdouble get_HopperLowestPoint(){
		return HopperLowestPoint;
	}
	
	
	void set_Hopper(Mdouble ExitLength, Mdouble ExitHeight, Mdouble Angle, Mdouble Length, Mdouble Height){

		if (ExitLength>=0.0) {HopperExitLength = ExitLength;} else std::cerr << "WARNING : Hopper exit length must be greater than or equal to zero" << std::endl;
		if (ExitHeight>=0.0) {HopperExitHeight = ExitHeight;} else std::cerr << "WARNING : Hopper exit height must be greater than or equal to zero" << std::endl;
		
		set_HopperLowestPoint(HopperExitHeight - HopperExitLength * tan(get_ChuteAngle()) );
		
		if (Angle>0.0&&Angle<90.0) {HopperAngle = Angle*constants::pi/180.0;} else std::cerr << "WARNING : Hopper angle must in (0,90)" << std::endl;

		if (Length>ExitLength) {HopperLength = Length;} else std::cerr << "WARNING : Hopper length must be greater than exit length" << std::endl;
		
		//std::cout<<"HopperLowestPoint: "<<get_HopperLowestPoint()<<", HopperLength: "<<HopperLength<<", HopperExitLength: "<<HopperExitLength<<", HopperAngle: "<<HopperAngle<<", Height: "<<fixed<<setprecision(15)<<setw(20)<<Height<<"comparing height: "<<setprecision(15)<<setw(20)<<(get_HopperLowestPoint() + 0.5*(HopperLength+HopperExitLength) / tan(HopperAngle))<<std::endl;
		
		Mdouble HeightCompare = (get_HopperLowestPoint() + 0.5*(HopperLength+HopperExitLength) / tan(HopperAngle));
		//This a semi-ugly fix to check wether Height>=Heightcompare and not take into account rounding errors
		if ((Height-HeightCompare)>-1e-6*HeightCompare) {HopperHeight = Height;} else std::cerr << "WARNING : For these settings, hopper height must be greater then or equal to "<<HeightCompare << ", see drawing" << std::endl;
		///todo: check wether HopperCornerHeight >=0, if not change hopperangle, line 105, I do not yet understand what the criteria is...
		
		//std::cout<<" "<<std::endl;
		//std::cout<<"ChuteWithHopper::set_Hopper:"<<std::endl;
		//std::cout<<"HopperHeight: 	  "<<HopperHeight<<std::endl;
		//std::cout<<"HopperExitLength: "<<HopperExitLength<<std::endl;
		//std::cout<<"HopperExitHeight: "<<HopperExitHeight<<std::endl;
		//std::cout<<"HopperAngle: 	  "<<HopperAngle<<std::endl;
		//std::cout<<"HopperLength: 	  "<<HopperLength<<std::endl;
		//std::cout<<" "<<std::endl;
		
	}
	
	///Allows chute length to be accessed

	Mdouble get_MaximumVelocityInducedByGravity(){
	  Mdouble height = HopperHeight+(get_xmax()-shift)*sin(ChuteAngle); 

	  return sqrt(2.0*9.8*height);
	}
	
	///Allows chute length to be accessed

	Mdouble get_ChuteLength(){return get_xmax()-shift;}

	///Allows chute length to be changed
	void set_ChuteLength(Mdouble new_){if (new_>=0.0) {set_xmax(new_+shift); set_xmin(0.0);} else std::cerr << "WARNING : Chute length unchanged, value must be greater than or equal to zero" << std::endl;}

	void set_centerHopper(bool new_){centerHopper=new_; }

	void set_lowerFillHeight(Mdouble new_){lowerFillHeight=new_; }
	
	void set_shift(Mdouble new_){if (new_>=0.0) {set_xmax(get_xmax()+new_-shift); shift = new_;} else std::cerr << "WARNING : Shift length unchanged, value must be greater than or equal to zero" << std::endl;}

	///This function reads all chute data
	virtual void read(std::istream& is)  { 
		Chute::read(is); 
		is >> HopperExitLength >> HopperExitHeight >> HopperLength 
			>> HopperAngle >> HopperHeight >> shift;
	}
	
	///This function writes all chute data
	virtual void write(std::ostream& os)  { 
		Chute::write(os); 
		os << HopperExitLength << " " << HopperExitHeight << " " << HopperLength 
			<< " " << HopperAngle << " " << HopperHeight << " " << shift << " " << std::endl;
	}
	
	virtual void print(std::ostream& os) {
		Chute::print(os); 
		os 
			<< "HopperExitLength:" << HopperExitLength 
			<< ", HopperExitHeight:" << HopperExitHeight 
			<< ", HopperLength:" << HopperLength 
			<< ", HopperAngle:" << HopperAngle 
			<< ", HopperHeight:" << HopperHeight 
			<< std::endl;
	}
	
	///This lifts the hopper above the plane of the chute
	void lift_hopper(Mdouble distance){lift=distance;}
	Mdouble get_lift_hopper(){return lift;}
	
	void set_hopper_dim(Mdouble new_hopper_dim){hopper_dim=new_hopper_dim;}
	
	void set_align_base(bool new_align){align_base=new_align;}

	int readNextArgument(unsigned int& i, unsigned int argc, char *argv[]) {
		if (!strcmp(argv[i],"-hopperLength")) {
			HopperLength=(atof(argv[i+1]));
		} else if (!strcmp(argv[i],"-hopperHeight")) {
			HopperHeight=(atof(argv[i+1]));
		} else if (!strcmp(argv[i],"-hopperAngle")) {
			HopperAngle=(atof(argv[i+1]));
		} else if (!strcmp(argv[i],"-hopperExitLength")) {
			HopperExitLength=(atof(argv[i+1]));
		} else if (!strcmp(argv[i],"-hopperExitHeight")) {
			HopperExitHeight=(atof(argv[i+1]));
		} else if (!strcmp(argv[i],"-lowerFillHeight")) {
			lowerFillHeight=(atof(argv[i+1]));
		} else if (!strcmp(argv[i],"-centerHopper")) {
			centerHopper=(atoi(argv[i+1]));
		} else if (!strcmp(argv[i],"-alignBase")) {
			align_base=(atoi(argv[i+1]));
		} else if (!strcmp(argv[i],"-shift")) {
			shift=(atof(argv[i+1]));
		} else if (!strcmp(argv[i],"-lift")) {
			lift=(atof(argv[i+1]));
		} else return Chute::readNextArgument(i, argc, argv); //if argv[i] is not found, check the commands in Chute
		return true; //returns true if argv[i] is found
	}
	
};
#endif
