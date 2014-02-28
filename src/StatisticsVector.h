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

#ifndef FSTAT_H
#define FSTAT_H
#include "MD.h"
#include "MatrixSymmetric.h"
#include <string.h>
#include <fstream>
#include <math.h>
///Creates averaged statistics (only valid if density field is homogenous along averaged direction)
enum StatType {O, X, Y, Z, XY, XZ, YZ, XYZ, RAZ, RZ, AZ, RA, R, A};
#include "Polynomial.h"
#include "StatisticsPoint.h"

/** \brief This class is used to extract statistical data from MD simulations. 
 * 
 * When calling statistics_from_fstat_and_data(), statistical data 
 * (such as density, pressure, ...) will be extracted at various points 
 * in the domain, aligned in a nx*ny*nz grid.
 * 
 * Set functions can be used to define the dimensions of the grid 
 * (default: nx=ny=nz=1) and the type and width of the coarse graining 
 * function (default: Gaussian, width w=r_max).  **/
template <StatType T>
class StatisticsVector : public virtual MD
{
public:
	///Sets up all basic things
	void constructor();
	void constructor(std::string name);
	
	///Basic constructor only calls constructor()
	StatisticsVector() {constructor();}
	StatisticsVector(std::string name) {constructor(name);}

	///Copy constructor
	StatisticsVector(StatisticsVector& other);
		
	///Advanced constructor that accepts arguments from the command line
	StatisticsVector(unsigned int argc, char *argv[]);
	void readStatArguments(unsigned int argc, char *argv[]);

	///Outputs member variable values to a std::string
	std::string print();

	void set_statType();

	void print_help();

	//set functions should be called before executing the statistical routine 
	void set_nx(int new_) { nx = new_;};
	void set_hx(Mdouble hx) { set_nx( ceil((get_xmaxStat()-get_xminStat())/hx) ); };
	void set_ny(int new_) { ny = new_;};
	void set_hy(Mdouble hy) { set_ny( ceil((get_ymaxStat()-get_yminStat())/hy) ); };
	void set_nz(int new_) { nz = new_; if (get_dim()<3) std::cerr << "Warning in set_nz: dimension less than 3"<<std::endl; };
	void set_hz(Mdouble hz) { set_nz( ceil((get_zmaxStat()-get_zminStat())/hz) ); };
	void set_n(int n) { set_nx(n); set_ny(n); set_nz(n); };
	void set_h(Mdouble h) { set_hx(h); set_hy(h); set_hz(h);};
	int get_nx() {return nx;};
	int get_ny() {return ny;};
	int get_nz() {return nz;};
	void set_tminStat(Mdouble t) {tminStat=t;};
	void set_tmaxStat(Mdouble t) {tmaxStat=t;};
	void set_tintStat(Mdouble t) {tintStat=t;};
	Mdouble get_tminStat() {return tminStat;};
	Mdouble get_tmaxStat() {if(std::isnan(tmaxStat)) return get_tmax(); else return tmaxStat;};
	Mdouble get_tintStat() {return tintStat;};
	bool check_current_time_for_statistics(){return (get_t()>get_tminStat()&&get_t()<=get_tmaxStat()+get_dt());};
	
	void set_CG_type(const char* new_);
	void set_CG_type(CG new_);
	CG get_CG_type() {return CG_type;};
	
	void set_n(int nx_, int ny_, int nz_) {nx=nx_; ny=ny_; nz=nz_;};
	void get_n(int& nx_, int& ny_, int& nz_) {nx_=nx; ny_=ny; nz_=nz;};

	///Set CG variables w2 and CG_invvolume
	void set_w(Mdouble w) {set_w2(sqr(w));};

	///Set CG variables w2 and CG_invvolume
	void set_w2(Mdouble new_);

	Mdouble get_w() {return sqrt(w2);};
	Mdouble get_w2() {return w2;};
	Mdouble get_cutoff() {return cutoff;};
	Mdouble get_cutoff2() {return sqr(cutoff);};
	
	//~ bool get_boundedDomain() {return boundedDomain;};
	//~ void set_boundedDomain(bool new_) {boundedDomain=new_;};

	///Output coarse graining variables
	std::string print_CG();

	///Output average of statistical variables
	StatisticsPoint<T> average(std::vector<StatisticsPoint<T> > &P);

	///Set all statistical variables to zero
	virtual void reset_statistics();

	///get StatisticsPoint
	void statistics_from_fstat_and_data();
	void statistics_from_p3();
	void jump_p3c();
	
	void set_doTimeAverage(bool new_) {doTimeAverage = new_;};
	bool get_doTimeAverage() {return doTimeAverage;};
	
	void set_StressTypeForFixedParticles(int new_) {StressTypeForFixedParticles = new_;};
	int get_StressTypeForFixedParticles() {return StressTypeForFixedParticles;};
	
	// to keep compatible with older versions
	void set_infiniteStressForFixedParticles(bool new_) {StressTypeForFixedParticles = 2+new_;};
	
	void set_mirrorAtDomainBoundary(Mdouble new_) {mirrorAtDomainBoundary = new_;};
	Mdouble get_mirrorAtDomainBoundary() {return mirrorAtDomainBoundary;};
	
	void set_doVariance(bool new_) {doVariance = new_;};
	bool get_doVariance() {return doVariance;};

	void set_doGradient(bool new_) {doGradient = new_;};
	bool get_doGradient() {return doGradient;};
	
	void set_superexact(bool new_) {superexact = new_;};
	bool get_superexact() {return superexact;};
	
	void set_ignoreFixedParticles(bool new_) {ignoreFixedParticles = new_;};
	bool get_ignoreFixedParticles() {return ignoreFixedParticles;};
	
	void verbose() {verbosity = 2;};
	void set_verbosity(int new_) {verbosity = new_;};
	int get_verbosity() {return verbosity;};
	
	void set_walls(bool new_) {walls = new_;};
	bool get_walls() {return walls;};

	void set_periodicWalls(bool new_) {periodicWalls = new_;};
	bool get_periodicWalls() {return periodicWalls;};

	void set_w_over_rmax(Mdouble new_) {w_over_rmax = new_;};
	Mdouble get_w_over_rmax() {return w_over_rmax;};

	///Set position of StatisticsPoint points and set variables to 0
	void set_Positions();

	bool read_next_from_data_file (unsigned int format);

	void gather_force_statistics_from_fstat_and_data();
	void gather_force_statistics_from_p3c(int version);
	void gather_force_statistics_from_p3w(int version, std::vector<int>& index);
	void evaluate_force_statistics(int wp=0);
	void evaluate_wall_force_statistics(Vec3D P, int wp=0);
	void jump_fstat();
	
	///Initializes statistics, i.e. setting w2, setting the grid and writing the header lines in the .stat file
	void initialize_statistics();
	
	///Calculates statistics for Particles (i.e. not collisions)
	void output_statistics();
	///Calculates statistics for one collision (can be any kind of collision)
	void gather_statistics_collision(int index1,int index2, Vec3D Contact,  Mdouble delta, Mdouble ctheta, Mdouble fdotn, Mdouble fdott, Vec3D P1_P2_normal_, Vec3D P1_P2_tangential);
	///Processes all gathered statistics and resets them afterwards. (Processing means either calculating time averages or writing out statistics)
	void process_statistics(bool usethese);
	///Finish all statistics (i.e. write out final data)
	void finish_statistics();

	///Writes regular statistics
	void write_statistics();
	///Writes out time averaged statistics
	void write_time_average_statistics();	
	
	///Calculates statistics for a single Particle
	void evaluate_particle_statistics(std::vector<BaseParticle*>::iterator P, int wp=0);
	
	std::vector<StatisticsPoint<T> > get_Points(){return Points;};
	
	///Functions to acces and change the domain of statistics
	Mdouble get_xminStat() {if(std::isnan(xminStat)) return get_xmin(); else return xminStat;};
	Mdouble get_yminStat() {if(std::isnan(yminStat)) return get_ymin(); else return yminStat;};
	Mdouble get_zminStat() {if(std::isnan(zminStat)) return get_zmin(); else return zminStat;};
	Mdouble get_xmaxStat() {if(std::isnan(xmaxStat)) return get_xmax(); else return xmaxStat;};
	Mdouble get_ymaxStat() {if(std::isnan(ymaxStat)) return get_ymax(); else return ymaxStat;};
	Mdouble get_zmaxStat() {if(std::isnan(zmaxStat)) return get_zmax(); else return zmaxStat;};
	void set_xminStat(Mdouble xminStat_){xminStat=xminStat_;};
	void set_yminStat(Mdouble yminStat_){yminStat=yminStat_;};
	void set_zminStat(Mdouble zminStat_){zminStat=zminStat_;};
	void set_xmaxStat(Mdouble xmaxStat_){xmaxStat=xmaxStat_;};
	void set_ymaxStat(Mdouble ymaxStat_){ymaxStat=ymaxStat_;};
	void set_zmaxStat(Mdouble zmaxStat_){zmaxStat=zmaxStat_;};
	int get_nTimeAverage() {return nTimeAverage;};

	Mdouble setInfinitelyLongDistance();

	void set_Polynomial(std::vector<Mdouble> new_coefficients, unsigned int new_dim) {	
		CGPolynomial.set_polynomial(new_coefficients, new_dim);
	}

	void set_Polynomial(Mdouble* new_coefficients, unsigned int num_coeff, unsigned int new_dim) {	
		CGPolynomial.set_polynomial(new_coefficients, num_coeff, new_dim);
	}

	void set_PolynomialName(const char* new_name) {	
		CGPolynomial.set_name(new_name);
	}

	std::string get_PolynomialName() {	
		return CGPolynomial.get_name();
	}

	void set_doublePoints(bool new_) {doublePoints=new_;}

	bool get_doublePoints() {return doublePoints;}

	void set_TimeAverageReset(int new_) {nTimeAverageReset=new_;}

	bool get_TimeAverageReset() {return nTimeAverageReset;}

	void set_rmin(Mdouble new_) {rmin=new_;}
	void set_rmax(Mdouble new_) {rmax=new_;}
	void set_hmax(Mdouble new_) {hmax=new_;}

	Mdouble evaluatePolynomial(Mdouble r) {
		return CGPolynomial.evaluate(r);
	}

	Mdouble evaluatePolynomialGradient(Mdouble r) {
		return CGPolynomial.evaluateGradient(r);
	}

	Mdouble evaluateIntegral(Mdouble n1, Mdouble n2, Mdouble t) {
		return CGPolynomial.evaluateIntegral(n1,n2,t);
	}


//Member Variables
protected:

	//General Variables
	///Possible values X,Y,Z,XY,XZ,YZ,XYZ,RAZ,RA,RZ,AZ,R,A are used to determine if the statistics are averaged; f.e. StatType X is averaged over y and z.
	StatType statType;
	///Grid size #nx,#ny,#nz (by default the points of evaluation are placed in an grid on the domain [#xminStat,#xmaxStat]x[#yminStat,#ymaxStat]x[#zminStat,#zmaxStat]
	int nx;
	///see #nx
	int ny;
	///see #nx
	int nz;	
	///By default the points of evaluation are placed in an grid on the domain [#xminStat,#xmaxStat]x[#yminStat,#ymaxStat]x[#zminStat,#zmaxStat].
	///By default the domain is set to the domain of the MD problem (indicated by setting the stat-domain values to nan), but can be resized.
	Mdouble xminStat, xmaxStat, yminStat, ymaxStat, zminStat, zmaxStat;
	///extension of grid size from mirrored points
	int nxMirrored, nyMirrored, nzMirrored;	
	
	//Storage of points and gradients
	///A vector that stores the values of the statistical variables at a given position.
	std::vector<StatisticsPoint<T> > Points;
	///A vector that stores the gradient in x of all statistical variables at a given position.
	std::vector<StatisticsPoint<T> > dx; 
	///A vector that stores the gradient in y of all statistical variables at a given position.
	std::vector<StatisticsPoint<T> > dy; 
	///A vector that stores the gradient in z of all statistical variables at a given position.
	std::vector<StatisticsPoint<T> > dz; 

	//For time averaging
	///A vector used to sum up all statistical values in #Points for time-averaging
	std::vector<StatisticsPoint<T> > timeAverage; 
	///a vector used to sum up the variance in time of all statistical values
	std::vector<StatisticsPoint<T> > timeVariance; 
	///a vector used to sum up all statistical gradients in #dx for time-averaging
	std::vector<StatisticsPoint<T> > dxTimeAverage; 
	///a vector used to sum up all statistical gradients in #dy for time-averaging
	std::vector<StatisticsPoint<T> > dyTimeAverage; 
	///a vector used to sum up all statistical gradients in #dz for time-averaging
	std::vector<StatisticsPoint<T> > dzTimeAverage; 
	///Determines if output is averaged over time
	bool doTimeAverage;
	///Determines after how many timesteps the time average is reset
	int nTimeAverageReset; 
	///Determines if variance is outputted
	bool doVariance;
	///Determines if gradient is outputted
	bool doGradient;
	///A counter needed to average over time
	int nTimeAverage; 
	
	//Coarse graining variables
	///coarse graining type (Gaussian, Heaviside, Polynomial)
	CG CG_type; 
	///Stores the Polynomial, if the cg function is an axisymmetric function Polynomial in r.
	NORMALIZED_POLYNOMIAL<T> CGPolynomial;
	// ///<if true, then the course-graining function will be cut at the domain boundaries and resized to satisfy int(W) = 1
	// bool boundedDomain; 
	///coarse graining width squared; for HeavisideSphere and Gaussian
	Mdouble w2; 
	///The distance from the origin at which the cg function vanishes; cutoff=w for HeavisideSphere or Polynomial, 3*w for Gaussian
	Mdouble cutoff, cutoff2; 
	/// if w is not set manually then w will be set by multiplying this value by the largest particle radius at t=0
	Mdouble w_over_rmax; 
	
	//Options that can be set before evaluation

	///Statistical output will only be created if t>tminStat
	Mdouble tminStat; 
	///Statistical output will only be created if t<tmaxStat
	Mdouble tmaxStat; 
	///Statistical output will only be created if tmaxStat-tintStat< t< tmaxStat
	Mdouble tintStat; 
	///defines the species for which statistics are extracted (-1 for all species)
	Mdouble indSpecies; 
	///defines the minimum radius of the particles for which statistics are extracted
	///\todo Thomas: maybe this fixed condition should be replaced by a condition function, bool include_statistics_if()
	Mdouble rmin; 
	///defines the maximum radius of the particles for which statistics are extracted
	Mdouble rmax; 
	///defines the maximum height of the particles for which statistics are extracted
	Mdouble hmax; 
	///Turns off walls before evaluation
	bool walls; 
	///Turns off periodic walls before evaluation (needed for averaging, because we do not yet check if particle is in domain)
	///\todo{Thomas: the case periodicWalls=true seems to mess up some statistics. Needs to be checked or removed}
	bool periodicWalls; 
	///Determines if fixed particles contribute to particle statistics (density, ...)
	bool ignoreFixedParticles; 
	///0 no Stress from fixed particles
	///1 Stress from fixed particles distributed between Contact and flowing Particle COM (default)
	///2 Stress from fixed particles distributed between fixed and flowing Particle COM
	///3 Stress from fixed particles extends from flowing particle to infinity
	int StressTypeForFixedParticles;
	///0 no output
	///1 basic output (timesteps)
	///2 full output (number of forces and particles, md and stat parameters)

	int verbosity; //<Determines how much is outputted to the terminal 
	int format; //< format of the data input file
	Mdouble mirrorAtDomainBoundary; //< 0: Statistics near the wall are equal to statistics away from the wall; 1: Statistics are mirrored at the domain boundaries; up to a maximum depth specified by this number

	bool isMDCLR;
	///If true, cutoff radius for Gaussian is set to 5*w (from 3*w)
	bool superexact; 
	
	//uses close points to allow calculation of gradients
	bool doublePoints; 
	
	bool satisfiesInclusionCriteria(BaseParticle* P);


	//Variables communicate values between member functions #evaluate_force_statistics and #gather_statistics_collision)used to communicate values between member functions evaluate_force_statistics and gather_force_statistics	
	///Position of first contact point
	Vec3D P1; 
	///Position of second contact point
	Vec3D P2; 
	///Direction of contact
	Vec3D P1_P2_normal; 
	///Length of contact line
	Mdouble P1_P2_distance; 
	///Contact stress from normal forces along the line of contact
	Matrix3D P1_P2_NormalStress; 
	///
	Matrix3D P1_P2_ContactCoupleStress; 
	Vec3D P1_P2_Contact; 
	///Contact stress from tangential forces along the line of contact
	Matrix3D P1_P2_TangentialStress; 
	///Traction from normal forces at contact of flow with fixed particles or walls 
	Vec3D P1_P2_NormalTraction; 
	///Traction from tangential forces at contact of flow with fixed particles or walls
	Vec3D P1_P2_TangentialTraction; 
	///Fabric
	MatrixSymmetric3D P1_P2_Fabric; 
	///not yet working
	Vec3D P1_P2_CollisionalHeatFlux; 
	///not yet working
	Mdouble P1_P2_Dissipation; 
	///not yet working
	Mdouble P1_P2_Potential; 

	bool loadVelocityProfile (const char* filename);
	Vec3D getVelocityProfile (Vec3D Position);

	std::vector<Vec3D> VelocityProfile;
	Vec3D VelocityProfile_Min;
	Vec3D VelocityProfile_D;
	
	std::fstream p3p_file;
	std::fstream p3c_file;
	std::fstream p3w_file;
	bool read_next_from_p3p_file ();
	void auto_setdim (); 
	


};

#include "StatisticsPoint.hcc"
#include "StatisticsVector.hcc"

#endif
