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

#ifndef STATISTICSVECTOR_H
#define STATISTICSVECTOR_H
#include "DPMBase.h"
#include "Math/Matrix.h"
#include "Math/MatrixSymmetric.h"
#include <string.h>
#include <fstream>
#include <math.h>
#include "Species/LinearViscoelasticSpecies.h"
/*!
 *\brief Creates averaged statistics (only valid if density field is homogenous along averaged direction)
 */
enum StatType
{
    O, X, Y, Z, XY, XZ, YZ, XYZ, RAZ, RZ, AZ, RA, R, A
};

#include "Math/NormalisedPolynomial.h"
#include "StatisticsPoint.h"

/*!
 * \class StatisticsVector 
 * \brief This class is used to extract statistical data from MD simulations. 
 * 
 * \details When calling statistics_from_fstat_and_data(), statistical data 
 * (such as density, pressure, ...) will be extracted at various points 
 * in the domain, aligned in a nx*ny*nz grid.
 * 
 * Set functions can be used to define the dimensions of the grid 
 * (default: nx=ny=nz=1) and the type and width of the coarse graining 
 * function (default: Gaussian, width w=r_max).  
*/
template<StatType T>
class StatisticsVector : public virtual DPMBase
{
public:
    /*!
     * \brief this is the actual constructor, sets up all basic things
     */
    void constructor();

    /*!
     * \brief 
     */
    void constructor(std::string name);

    /*!
     * \brief Basic constructor only calls constructor()
     */
    StatisticsVector()
    {
        constructor();
    }

    /*!
     * \brief 
     */
    StatisticsVector(std::string name)
    {
        constructor(name);
    }
    
    /*!
     * \brief Copy constructor
     */
    StatisticsVector(StatisticsVector& other);

    /*!
     * \brief Advanced constructor that accepts arguments from the command line
     */
    StatisticsVector(int argc, char *argv[]);
    /*!
     * \brief 
     */
    void readStatArguments(int argc, char *argv[]);

    /*!
     * \brief Outputs member variable values to a std::string
     */
    std::string printStat();

    /*!
     * \brief 
     */
    void set_statType();

    void writeOutputFiles();

    /*!
     * \brief 
     */
    void print_help();

    //set functions should be called before executing the statistical routine 

    /*!
     * \brief 
     */
    void setNX(int new_)
    {
        nx = new_;
    }

    /*!
     * \brief 
     */
    void set_hx(Mdouble hx)
    {
        setNX(static_cast<int>(std::ceil((getXMaxStat() - getXMinStat()) / hx)));
    }
    
    /*!
     * \brief 
     */
    void setNY(int new_)
    {
        ny = new_;
    }
    
    /*!
     * \brief 
     */
    void set_hy(Mdouble hy)
    {
        setNY(static_cast<int>(std::ceil((getYMaxStat() - getYMinStat()) / hy)));
    }
    
    /*!
     * \brief 
     */
    void setNZ(int new_)
    {
        nz = new_;
        if (getSystemDimensions() < 3)
            std::cerr << "Warning in set_nz: dimension less than 3" << std::endl;
    }
    
    /*!
     * \brief 
     */
    void set_hz(Mdouble hz)
    {
        setNZ(static_cast<int>(std::ceil((getZMaxStat() - getZMinStat()) / hz)));
    }
    
    /*!
     * \brief 
     */
    void setN(int n)
    {
        setNX(n);
        setNY(n);
        setNZ(n);
    }
    
    /*!
     * \brief 
     */
    void set_h(Mdouble h)
    {
        set_hx(h);
        set_hy(h);
        set_hz(h);
    }
    
    /*!
     * \brief 
     */
    int getNX()
    {
        return nx;
    }
    
    /*!
     * \brief 
     */
    int getNY()
    {
        return ny;
    }
    
    /*!
     * \brief 
     */
    int getNZ()
    {
        return nz;
    }
    
    /*!
     * \brief 
     */
    void setCGTimeMin(Mdouble t)
    {
        tminStat = t;
    }
    
    /*!
     * \brief 
     */
    void setTimeMaxStat(Mdouble t)
    {
        tmaxStat = t;
    }
    
    /*!
     * \brief 
     */
    void setCGTimeAveragingInterval(Mdouble t)
    {
        tintStat = t;
    }
    
    /*!
     * \brief 
     */
    Mdouble getCGTimeMin()
    {
        return tminStat;
    }
    
    /*!
     * \brief 
     */
    Mdouble getTimeMaxStat()
    {
        if (std::isnan(tmaxStat))
            return getTimeMax();
        else
            return tmaxStat;
    }
    
    /*!
     * \brief 
     */
    Mdouble getCGTimeAveragingInterval()
    {
        return tintStat;
    }
    
    /*!
     * \brief 
     */
    bool check_current_time_for_statistics()
    {
        return (getTime() > getCGTimeMin() && getTime() <= getTimeMaxStat() + getTimeStep());
    }
    

    /*!
     * \brief 
     */
    void setCGShape(const char* new_);

    /*!
     * \brief 
     */
    void setCGShape(CG new_);

    /*!
     * \brief 
     */
    CG getCGShape()
    {
        return CG_type;
    }
    
    /*!
     * \brief 
     */
    void setN(int nx_, int ny_, int nz_)
    {
        nx = nx_;
        ny = ny_;
        nz = nz_;
    }
    
    void getN(int& nx_, int& ny_, int& nz_)
    {
        nx_ = nx;
        ny_ = ny;
        nz_ = nz;
    }
    

    ///Set CG variables w2 and CG_invvolume
    void setCGWidth(Mdouble w)
    {
        setCGWidth2(mathsFunc::square(w));
    }
    

    /*!
     * \brief Set CG variables w2 and CG_invvolume
     */
    void setCGWidth2(Mdouble new_);

    /*!
     * \brief 
     */
    Mdouble getCGWidth()
    {
        return std::sqrt(w2);
    }
    
    /*!
     * \brief 
     */
    Mdouble getCGWidthSquared()
    {
        return w2;
    }
    
    /*!
     * \brief 
     */
    Mdouble getCutoff()
    {
        return cutoff;
    }
    
    /*!
     * \brief 
     */
    Mdouble getCutoff2()
    {
        return mathsFunc::square(cutoff);
    }
    

    //~ bool get_boundedDomain() {return boundedDomain;};
    //~ void set_boundedDomain(bool new_) {boundedDomain=new_;};
    
    /*!
     * \brief Output coarse graining variables
     */
    std::string print_CG();

    /*!
     * \brief Output average of statistical variables
     */
    StatisticsPoint<T> average(std::vector<StatisticsPoint<T> > &P);

    /*!
     * \brief Set all statistical variables to zero
     */
    virtual void reset_statistics();

    /*!
     * \brief get StatisticsPoint
     */
    void statistics_from_fstat_and_data();

    /*!
     * \brief 
     */
    void statistics_from_p3();

    /*!
     * \brief 
     */
    void jump_p3c();

    /*!
     * \brief 
     */
    void setDoTimeAverage(bool new_)
    {
        doTimeAverage = new_;
    }
    
    /*!
     * \brief 
     */
    bool getDoTimeAverage()
    {
        return doTimeAverage;
    }
    

    /*!
     * \brief 
     */
    void setStressTypeForFixedParticles(int new_)
    {
        StressTypeForFixedParticles = new_;
    }
    
    /*!
     * \brief 
     */
    int getStressTypeForFixedParticles()
    {
        return StressTypeForFixedParticles;
    }
    

    // to keep compatible with older versions
    /*!
     * \brief 
     */
    void set_infiniteStressForFixedParticles(bool new_)
    {
        StressTypeForFixedParticles = 2 + new_;
    }
    

    /*!
     * \brief 
     */
    void setMirrorAtDomainBoundary(Mdouble new_)
    {
        mirrorAtDomainBoundary = new_;
    }
    
    /*!
     * \brief 
     */
    Mdouble getMirrorAtDomainBoundary()
    {
        return mirrorAtDomainBoundary;
    }
    

    /*!
     * \brief 
     */
    void setDoVariance(bool new_)
    {
        doVariance = new_;
    }
    
    /*!
     * \brief 
     */
    bool getDoVariance()
    {
        return doVariance;
    }
    

    /*!
     * \brief 
     */
    void setDoGradient(bool new_)
    {
        doGradient = new_;
    }
    
    /*!
     * \brief 
     */
    bool getDoGradient()
    {
        return doGradient;
    }
    
    /*!
     * \brief 
     */
    void setSuperExact(bool new_)
    {
        superexact = new_;
    }
    
    /*!
     * \brief 
     */
    bool getSuperExact()
    {
        return superexact;
    }
    
    /*!
     * \brief 
     */
    void setDoIgnoreFixedParticles(bool new_)
    {
        ignoreFixedParticles = new_;
    }
    
    /*!
     * \brief 
     */
    bool getDoIgnoreFixedParticles()
    {
        return ignoreFixedParticles;
    }
    
    /*!
     * \brief 
     */
    void verbose()
    {
        verbosity = 2;
    }
    
    /*!
     * \brief 
     */
    void setVerbosityLevel(int new_)
    {
        verbosity = new_;
    }
    
    /*!
     * \brief 
     */
    int getVerbosityLevel() const
    {
        return verbosity;
    }

    /*!
     * \brief 
     */
    void setCGWidthalls(bool new_)
    {
        walls = new_;
    }
    
    /*!
     * \brief 
     */
    bool getCGWidthalls()
    {
        return walls;
    }

    /*!
     * \brief 
     */
    void setDoPeriodicWalls(bool new_)
    {
        periodicWalls = new_;
    }
    
    /*!
     * \brief 
     */
    bool getDoPeriodicWalls()
    {
        return periodicWalls;
    }
    

    /*!
     * \brief 
     */
    void setCGWidth_over_rmax(Mdouble new_)
    {
        w_over_rmax = new_;
    }
    
    /*!
     * \brief 
     */
    Mdouble getCGWidth_over_rmax()
    {
        return w_over_rmax;
    }
    

    /*!
     * \brief Set position of StatisticsPoint points and set variables to 0
     */
    void setPositions();

    /*!
     * \brief 
     */
    bool readNextDataFile(unsigned int format);

    /*!
     * \brief 
     */
    void gather_force_statistics_from_fstat_and_data();

    /*!
     * \brief 
     */
    void gather_force_statistics_from_p3c(int version);

    /*!
     * \brief 
     */
    void gather_force_statistics_from_p3w(int version, std::vector<int>& index);

    /*!
     * \brief 
     */
    void evaluate_force_statistics(int wp = 0);

    /*!
     * \brief 
     */
    void evaluate_wall_force_statistics(Vec3D P, int wp = 0);

    /*!
     * \brief 
     */
    void jump_fstat();

    /*!
     * \brief Initializes statistics, i.e. setting w2, setting the grid and writing the header lines in the .stat file
     */
    void initialiseStatistics();

    /*!
     * \brief Calculates statistics for Particles (i.e. not collisions)
     */
    void outputStatistics();

    /*!
     * \brief Calculates statistics for one collision (can be any kind of collision)
     */
    void gatherContactStatistics(int index1, int index2, Vec3D Contact, Mdouble delta, Mdouble ctheta, Mdouble fdotn, Mdouble fdott, Vec3D P1_P2_normal_, Vec3D P1_P2_tangential);

    /*!
     * \brief Processes all gathered statistics and resets them afterwards. (Processing means either calculating time averages or writing out statistics)
     */
    void processStatistics(bool usethese);

    /*!
     * \brief Finish all statistics (i.e. write out final data)
     */
    void finishStatistics();

    /*!
     * \brief Writes regular statistics
     */
    void write_statistics();

    /*!
     * \brief Writes out time averaged statistics
     */
    void write_time_average_statistics();

    /*!
     * \brief Calculates statistics for a single Particle
     */
    void evaluate_particle_statistics(std::vector<BaseParticle*>::iterator P, int wp = 0);

    /*!
     * \brief 
     */
    std::vector<StatisticsPoint<T> > getCGPoints()
    {
        return Points;
    }

    /*!
     * \brief Functions to acces and change the domain of statistics
     */
    Mdouble getXMinStat()
    {
        if (std::isnan(xminStat))
            return getXMin();
        else
            return xminStat;
    }
    
    /*!
     * \brief 
     */
    Mdouble getYMinStat()
    {
        if (std::isnan(yminStat))
            return getYMin();
        else
            return yminStat;
    }
    
    /*!
     * \brief 
     */
    Mdouble getZMinStat()
    {
        if (std::isnan(zminStat))
            return getZMin();
        else
            return zminStat;
    }
    
    /*!
     * \brief 
     */
    Mdouble getXMaxStat()
    {
        if (std::isnan(xmaxStat))
            return getXMax();
        else
            return xmaxStat;
    }
    
    /*!
     * \brief 
     */
    Mdouble getYMaxStat()
    {
        if (std::isnan(ymaxStat))
            return getYMax();
        else
            return ymaxStat;
    }
    
    /*!
     * \brief 
     */
    Mdouble getZMaxStat()
    {
        if (std::isnan(zmaxStat))
            return getZMax();
        else
            return zmaxStat;
    }
    
    /*!
     * \brief 
     */
    void setXMinStat(Mdouble xminStat_)
    {
        xminStat = xminStat_;
    }
    
    /*!
     * \brief 
     */
    void setYMinStat(Mdouble yminStat_)
    {
        yminStat = yminStat_;
    }
    
    /*!
     * \brief 
     */
    void setZMinStat(Mdouble zminStat_)
    {
        zminStat = zminStat_;
    }
    
    /*!
     * \brief 
     */
    void setXMaxStat(Mdouble xmaxStat_)
    {
        xmaxStat = xmaxStat_;
    }
    
    /*!
     * \brief 
     */
    void setYMaxStat(Mdouble ymaxStat_)
    {
        ymaxStat = ymaxStat_;
    }
    
    /*!
     * \brief 
     */
    void setZMaxStat(Mdouble zmaxStat_)
    {
        zmaxStat = zmaxStat_;
    }
    
    /*!
     * \brief 
     */
    int getNTimeAverage()
    {
        return nTimeAverage;
    }

    /*!
     * \brief 
     */
    Mdouble setInfinitelyLongDistance();

    void set_Polynomial(std::vector<Mdouble> new_coefficients, unsigned int new_dim)
    {
        CGPolynomial.set_polynomial(new_coefficients, new_dim);
    }
    
    /*!
     * \brief 
     */
    void set_Polynomial(Mdouble* new_coefficients, unsigned int num_coeff, unsigned int new_dim)
    {
        CGPolynomial.set_polynomial(new_coefficients, num_coeff, new_dim);
    }
    
    /*!
     * \brief 
     */
    void setPolynomialName(const char* new_name)
    {
        CGPolynomial.setName(new_name);
    }
    
    /*!
     * \brief 
     */
    std::string getPolynomialName()
    {
        return CGPolynomial.getName();
    }
    
    /*!
     * \brief 
     */
    void setDoDoublePoints(bool new_)
    {
        doDoublePoints = new_;
    }
    
    /*!
     * \brief 
     */
    bool getDoDoublePoints()
    {
        return doDoublePoints;
    }
    
    /*!
     * \brief 
     */
    void setNTimeAverageReset(int new_)
    {
        nTimeAverageReset = new_;
    }
    
    /*!
     * \brief 
     */
    bool getNTimeAverageReset()
    {
        return nTimeAverageReset;
    }
    
    /*!
     * \brief 
     */
    void set_rmin(Mdouble new_)
    {
        rmin = new_;
    }

    /*!
     * \brief 
     */
    void set_rmax(Mdouble new_)
    {
        rmax = new_;
    }

    /*!
     * \brief 
     */
    void set_hmax(Mdouble new_)
    {
        hmax = new_;
    }
    
    /*!
     * \brief 
     */
    Mdouble evaluatePolynomial(Mdouble r)
    {
        return CGPolynomial.evaluate(r);
    }
    
    /*!
     * \brief 
     */
    Mdouble evaluatePolynomialGradient(Mdouble r)
    {
        return CGPolynomial.evaluateGradient(r);
    }
    
    /*!
     * \brief 
     */
    Mdouble evaluateIntegral(Mdouble n1, Mdouble n2, Mdouble t)
    {
        return CGPolynomial.evaluateIntegral(n1, n2, t);
    }
    
    /*!
     * \brief 
     */
    unsigned int getStepSize() const
    {
        return stepSize_;
    }
    
    /*!
     * \brief 
     */
    void setStepSize(unsigned int stepSize)
    {
        stepSize_ = stepSize;
    }
    
    //Member Variables
protected:
    
    //General Variables
    /*!
     * \brief Possible values X,Y,Z,XY,XZ,YZ,XYZ,RAZ,RA,RZ,AZ,R,A are used to determine if the statistics are averaged; f.e. StatType X is averaged over y and z.
     */
    StatType statType;
    /*!
     * \brief Grid size #nx,#ny,#nz (by default the points of evaluation are placed in an grid on the domain [#xminStat,#xmaxStat]x[#yminStat,#ymaxStat]x[#zminStat,#zmaxStat]
     */
    int nx; ///see #nx
    int ny; ///see #nx
    int nz;
    /*!
     * \brief By default the domain is set to the domain of the MD problem (indicated by setting the stat-domain values to nan), but can be resized.
     */
    Mdouble xminStat, xmaxStat, yminStat, ymaxStat, zminStat, zmaxStat;
    /*!
     * \brief extension of grid size from mirrored points
     */
    int nxMirrored, nyMirrored, nzMirrored;

    //Storage of points and gradients
    /*!
     * \brief A vector that stores the values of the statistical variables at a given position.
     */
    std::vector<StatisticsPoint<T> > Points;
    /*!
     * \brief A vector that stores the gradient in x of all statistical variables at a given position.
     */
    std::vector<StatisticsPoint<T> > dx;
    /*!
     * \brief A vector that stores the gradient in y of all statistical variables at a given position.
     */
    std::vector<StatisticsPoint<T> > dy;
    /*!
     * \brief A vector that stores the gradient in z of all statistical variables at a given position.
     */
    std::vector<StatisticsPoint<T> > dz;

    //For time averaging
    /*!
     * \brief A vector used to sum up all statistical values in #Points for time-averaging
     */
    std::vector<StatisticsPoint<T> > timeAverage;

    /*!
     * \brief a vector used to sum up the variance in time of all statistical values
     */
    std::vector<StatisticsPoint<T> > timeVariance;

    /*!
     * \brief a vector used to sum up all statistical gradients in #dx for time-averaging
     */
    std::vector<StatisticsPoint<T> > dxTimeAverage;

    /*!
     * \brief a vector used to sum up all statistical gradients in #dy for time-averaging
     */
    std::vector<StatisticsPoint<T> > dyTimeAverage;

    /*!
     * \brief a vector used to sum up all statistical gradients in #dz for time-averaging
     */
    std::vector<StatisticsPoint<T> > dzTimeAverage;

    /*!
     * \brief Determines if output is averaged over time
     */
    bool doTimeAverage;

    /*!
     * \brief Determines after how many timesteps the time average is reset
     */
    int nTimeAverageReset;

    /*!
     * \brief Determines if variance is outputted
     */
    bool doVariance;

    /*!
     * \brief Determines if gradient is outputted
     */
    bool doGradient;

    /*!
     * \brief A counter needed to average over time
     */
    int nTimeAverage;

    //Coarse graining variables
    /*!
     * \brief coarse graining type (Gaussian, Heaviside, Polynomial)
     */
    CG CG_type;
    /*!
     * \brief Stores the Polynomial, if the cg function is an axisymmetric function Polynomial in r.
     */
    NORMALIZED_POLYNOMIAL<T> CGPolynomial;

    // ///<if true, then the course-graining function will be cut at the domain boundaries and resized to satisfy int(W) = 1
    // bool boundedDomain; 

    /*!
     * \brief coarse graining width squared; for HeavisideSphere and Gaussian
     */
    Mdouble w2;

    /*!
     * \brief The distance from the origin at which the cg function vanishes; cutoff=w for HeavisideSphere or Polynomial, 3*w for Gaussian
     */
    Mdouble cutoff, cutoff2;

    /*!
     * \brief if w is not set manually then w will be set by multiplying this value by the largest particle radius at t=0
     */
    Mdouble w_over_rmax;

    //Options that can be set before evaluation
    
    /*!
     * \brief Statistical output will only be created if t>tminStat
     */
    Mdouble tminStat;

    /*!
     * \brief Statistical output will only be created if t<tmaxStat
     */
    Mdouble tmaxStat;

    /*!
     * \brief Statistical output will only be created if tmaxStat-tintStat< t< tmaxStat
     */
    Mdouble tintStat;

    /*!
     * \brief defines the species for which statistics are extracted (-1 for all species)
     */
    Mdouble indSpecies;

    /*!
     * \brief defines the minimum radius of the particles for which statistics are extracted
     * \todo Thomas: maybe this fixed condition should be replaced by a condition function, bool include_statistics_if()
     */
    Mdouble rmin;

    /*!
     * \brief defines the maximum radius of the particles for which statistics are extracted
     */
    Mdouble rmax;

    /*!
     * \brief defines the maximum height of the particles for which statistics are extracted
     */
    Mdouble hmax;

    /*!
     * \brief Turns off walls before evaluation
     */
    bool walls;

    /*!
     * \brief Turns off periodic walls before evaluation (needed for averaging, because we do not yet check if particle is in domain)
     * \todo{Thomas: the case periodicWalls=true seems to mess up some statistics. Needs to be checked or removed}
     */
    bool periodicWalls;

    /*!
     * \brief Determines if fixed particles contribute to particle statistics (density, ...)
     */
    bool ignoreFixedParticles;

    /*!
     * \brief Stress type for fixed particles
     * \details 0 no Stress from fixed particles\n
     * 1 Stress from fixed particles distributed between Contact and flowing Particle COM (default)\n
     * 2 Stress from fixed particles distributed between fixed and flowing Particle COM\n
     * 3 Stress from fixed particles extends from flowing particle to infinity
     */
    int StressTypeForFixedParticles;

    /*!
     * \brief Determines how much is outputted to the terminal
     * \details 0 no output\n
     * 1 basic output (timesteps)\n
     * 2 full output (number of forces and particles, md and stat parameters)\n
     */
    int verbosity; 

    int format; ///< format of the data input file

    /*!
     * \brief 0: Statistics near the wall are equal to statistics away from the wall; 1: Statistics are mirrored at the domain boundaries; up to a maximum depth specified by this number
     */
    Mdouble mirrorAtDomainBoundary;
    
    ///
    bool isMDCLR;
    ///If true, cutoff radius for Gaussian is set to 5*w (from 3*w)
    bool superexact;

    //uses close points to allow calculation of gradients
    bool doDoublePoints;

    /*!
     * \brief
     */
    bool satisfiesInclusionCriteria(BaseParticle* P);

    //Variables communicate values between member functions #evaluate_force_statistics and #gatherContactStatistics)used to communicate values between member functions evaluate_force_statistics and gather_force_statistics	
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
    ///
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

    /*!
     * \brief
     */
    bool loadVelocityProfile(const char* filename);

    /*!
     * \brief
     */
    Vec3D getVelocityProfile(Vec3D Position);

    /*!
     * \brief
     */
    std::vector<Vec3D> VelocityProfile;

    /*!
     * \brief
     */
    Vec3D VelocityProfile_Min;

    /*!
     * \brief
     */
    Vec3D VelocityProfile_D;

    std::fstream p3p_file;///
    std::fstream p3c_file;///
    std::fstream p3w_file;///

    /*!
     * \brief
     */
    bool read_next_from_p3p_file();

    /*!
     * \brief
     */
    void auto_setdim();

    unsigned int stepSize_;///
    
};

#include "StatisticsPoint.hcc"
#include "StatisticsVector.hcc"

#endif
