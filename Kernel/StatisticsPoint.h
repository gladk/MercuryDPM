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

#ifndef STATISTICSPOINT_H
#define STATISTICSPOINT_H

///enum used to store the type of coarse-graining function used 
enum CG
{
    HeavisideSphere, Gaussian, Polynomial
};

template<StatType T>
class StatisticsVector;

template<StatType T>
struct StatisticsPoint;

template<StatType T>
std::ostream& operator<<(std::ostream& os, const StatisticsPoint<T> &stat);

///This class stores statistical values for a given spatial position; to be used in combination with StatisticsVector
template<StatType T>
struct StatisticsPoint
{
public:
    
    //Constructors and Destructors	
    
    ///Constructor sets sensible values
    StatisticsPoint()
    {
        this->Position.setZero();
        mirrorParticle = -1;
        //~ this->set_zero(); 
    }
    
    ///Copy constructor; simply copies everything
    StatisticsPoint(const StatisticsPoint &other)
    {
        *this = other;
        mirrorParticle = -1;
    }
    
    //set and get access functions
    ///see StatisticsVector::setCGShape
    static void set_gb(StatisticsVector<T>* new_gb)
    {
        gb = new_gb;
    }
    ///see StatisticsVector::setCGShape
    void setCGShape(const char* CG_type)
    {
        this->gb->setCGShape(CG_type);
    }
    ///see StatisticsVector::getCGShape
    CG getCGShape() const
    {
        return this->gb->getCGShape();
    }
    ///see StatisticsVector::setCGWidth2
    void setCGWidth2(Mdouble new_)
    {
        this->gb->setCGWidth2(new_);
    }
    ///see StatisticsVector::getCGWidthSquared
    Mdouble getCGWidthSquared() const
    {
        return this->gb->getCGWidthSquared();
    }
    ///see StatisticsVector::getCGWidth
    Mdouble getCGWidth() const
    {
        return this->gb->getCGWidth();
    }
    ///see StatisticsVector::getCutoff
    Mdouble getCutoff()
    {
        return this->gb->getCutoff();
    }
    ///see StatisticsVector::getCutoff2
    Mdouble getCutoff2()
    {
        return this->gb->getCutoff2();
    }
    ///see StatisticsVector::getXMaxStat
    Mdouble getXMaxStat()
    {
        return this->gb->getXMaxStat();
    }
    ///see StatisticsVector::getYMaxStat
    Mdouble getYMaxStat()
    {
        return this->gb->getYMaxStat();
    }
    ///see StatisticsVector::getZMaxStat
    Mdouble getZMaxStat()
    {
        return this->gb->getZMaxStat();
    }
    ///see StatisticsVector::getXMinStat
    Mdouble getXMinStat()
    {
        return this->gb->getXMinStat();
    }
    ///see StatisticsVector::getYMinStat
    Mdouble getYMinStat()
    {
        return this->gb->getYMinStat();
    }
    ///see StatisticsVector::getZMinStat
    Mdouble getZMinStat()
    {
        return this->gb->getZMinStat();
    }
    ///see StatisticsVector::get_n
    void getN(int& nx_, int& ny_, int& nz_)
    {
        this->gb->getN(nx_, ny_, nz_);
    }
    ///see StatisticsVector::evaluatePolynomial
    Mdouble evaluatePolynomial(Mdouble r)
    {
        return gb->evaluatePolynomial(r);
    }
    ///see StatisticsVector::evaluatePolynomialGradient
    Mdouble evaluatePolynomialGradient(Mdouble r)
    {
        return gb->evaluatePolynomialGradient(r);
    }
    ///see StatisticsVector::evaluateIntegral
    Mdouble evaluateIntegral(Mdouble n1, Mdouble n2, Mdouble t)
    {
        return gb->evaluateIntegral(n1, n2, t);
    }
    
    ///sets #CG_invvolume
    void setCGInverseVolume();
    int nonaveragedDim();
    double averagingVolume();
    ///returns #CG_invvolume
    Mdouble getCGInverseVolume()
    {
        return this->CG_invvolume;
    }
    ///sets #CG_invvolume if CG_type=Gaussian
    void set_Gaussian_invvolume(int dim);
    ///computes #CG_invvolume if CG_type=Gaussian
    double compute_Gaussian_invvolume(int dim);
    ///sets #CG_invvolume if CG_type=HeaviSideSphere
    void set_Heaviside_invvolume();

    ///sets #CG_invvolume if CG_type=Polynomial
    void set_Polynomial_invvolume(int dim);

    ///sets #Position
    void setPosition(Vec3D new_)
    {
        this->Position = new_;
    }
    ///returns #Position
    Vec3D getPosition() const
    {
        return this->Position;
    }
    
    ///Outputs statistical variables to ostream
    friend std::ostream& operator<<<T>(std::ostream& os, const StatisticsPoint<T> &stat);

    ///Sets all statistical variables to zero
    void set_zero();

    ///Squares all statistical variables; needed for variance
    StatisticsPoint<T> getSquared();

    ///Defines a equal operator needed for copy constructor
    inline StatisticsPoint<T>& operator=(const StatisticsPoint<T> &P);

    ///Defines a plus operator needed to average values (\f$ \bar{v} = (\sum_{i=1}^n v_i)/n \f$ )
    inline StatisticsPoint<T>& operator+=(const StatisticsPoint<T> &P);

    ///Defines a plus operator needed to calculate variance
    inline StatisticsPoint<T>& operator-=(const StatisticsPoint<T> &P);

    ///Defines a division operator needed to average values (\f$ \bar{v} = (\sum_{i=1}^n v_i)/n \f$ )
    inline StatisticsPoint<T>& operator/=(const Mdouble a);

    ///Defines a division operator needed to time-average values (because the displacement does not have a value at the first timestep, this is slightly different than /=)
    inline void firstTimeAverage(const int n);

    ///returns the coarse graining distance in the coordinates that are not averaged about
    Mdouble getDistanceSquaredNonAveraged(const Vec3D &P);

    ///Returns the dot product of two vectors in the coordinates that are not averaged about
    Mdouble dotNonAveraged(const Vec3D &P, const Vec3D &Q);

    ///Returns a vector where the averaged directions are zero
    Vec3D clearAveragedDirections(Vec3D P);

    ///Returns the cross product of two vectors in the coordinates that are not averaged about
    Vec3D crossNonAveraged(Vec3D P, Vec3D &Q);

    ///Returns the cross product of two vectors in the coordinates that are not averaged about
    Matrix3D matrixCrossNonAveraged(Vec3D P, Matrix3D &Q);

    ///Returns the value of the course graining function phi(P,PI)
    Mdouble CG_function(const Vec3D &PI);

    ///returns the value of the course graining function phi(P,PI)
    ///averaged along a line
    Mdouble CG_function_2D(const Vec3D &PI);

    ///Returns the value of the course graining function phi(P,PI)
    ///averaged along a plane
    Mdouble CG_function_1D(const Vec3D &PI);

    ///gradient of phi 
    ///\todo{Only implemented for Gaussian}
    Vec3D CG_gradient(const Vec3D &P, const Mdouble phi);

    ///gradient of phi 
    ///\todo{Only implemented for Gaussian and statavg Z}
    Vec3D CG_integral_gradient(Vec3D &P1, Vec3D &P2, Vec3D &P1_P2_normal, Mdouble P1_P2_distance);
    Mdouble CG_integral_gradient_1D(Vec3D &P1, Vec3D &P2, Vec3D &P1_P2_normal, Mdouble P1_P2_distance);

    ///Returns the value of the coarse graining integral \f$ \psi(P,PI,PJ) = \int_0^1 \phi(P - PI + s PJ) ds \f$ 
    Mdouble CG_integral(Vec3D &P1, Vec3D &P2, Vec3D &P1_P2_normal, Mdouble P1_P2_distance, Vec3D& rpsi);

    ///Returns the value of the coarse graining integral \f$ \psi(P,PI,PJ) = \int_0^1 \phi(P - PI + s PJ) ds \f$ 
    ///averaged along a line
    Mdouble CG_integral_2D(Vec3D &P1, Vec3D &P2, Vec3D &P1_P2_normal, Mdouble P1_P2_distance, Mdouble& rpsi_scalar);

    ///Returns the value of the coarse graining integral \f$ \psi(P,PI,PJ) = \int_0^1 \phi(P - PI + s PJ) ds \f$
    ///averaged along a plane
    Mdouble CG_integral_1D(Vec3D &P1, Vec3D &P2, Vec3D &P1_P2_normal, Mdouble P1_P2_distance, Mdouble& rpsi_scalar);

    ///Outputs statistical variables in human-readable format
    std::string print() const;
    ///Outputs root of statistical variables in human-readable format
    std::string print_sqrt() const;
    ///Outputs names of statistical variables in computer-readable format
    std::string write_variable_names();
    ///Outputs statistical variables in computer-readable format
    std::string write() const;

public:
    //Values of macroscopical fields at Position
    ///Particle volume fraction, \f$\sum_i m_i/\rho_i \phi_i\f$
    Mdouble Nu;
    ///Density, \f$\sum_i m_i \phi_i\f$
    Mdouble Density;
    ///Momentum, \f$\sum_i m_i v_{ia} \phi_i\f$
    Vec3D Momentum;
    ///Momentum from linear displacement, \f$\sum_i m_i u_{ia} \phi_i\f$, where \f$\vec{u}_i=(\vec{r}_i(t)-\vec{r}_i(t-\tau))/\tau\f$, with \f$\tau=savecount \Delta t\f$ the time intervall between outputs.
    Vec3D DisplacementMomentum;
    ///Linear displacement, \f$2/(2 \rho_i^2) \sum_{ij} m_i m_j \phi_j (v_{ija} \partial_b \phi_i + v_{ijb} \partial_a \phi_i) \f$
    MatrixSymmetric3D Displacement;
    ///Momentum flux, \f$\sum_i m_i v_{ia} v_{ib} \phi_i\f$
    MatrixSymmetric3D MomentumFlux;
    ///Momentum flux from linear displacement, \f$\sum_i m_i u_{ia} u_{ib} \phi_i\f$
    MatrixSymmetric3D DisplacementMomentumFlux;
    ///Energy flux, \f$2/2 \sum_i m_i v_{ia} v_{ia} v_{ib} \phi_i\f$
    Vec3D EnergyFlux;
    ///Stress from normal forces, \f$\sum_{ij} f^n_{ija} l_{ijb} \psi_{ij}\f$
    Matrix3D NormalStress;
    ///Stress from tangential forces, \f$\sum_{ij} f^t_{ija} l_{ijb} \psi_{ij}\f$
    Matrix3D TangentialStress;
    ///Traction from normal forces, \f$\sum_{ij} f^n_{ija} l_{ijb} \phi_i\f$
    Vec3D NormalTraction;
    ///Traction from tangential forces, \f$\sum_{ij} f^t_{ija} l_{ijb} \phi_i\f$
    Vec3D TangentialTraction;
    ///Fabric tensor, \f$\sum_{ij} m_i/\rho_i n_{ija} n_{ijb} \psi_{ij}\f$
    MatrixSymmetric3D Fabric;
    ///Heat flux from collisions, \f$\sum_{ij} (v_{ia}+v_ja)/2 f_{ija} l_{ijb} \psi_i\f$
    ///\todo{Heat flux calculations have not been checked, only implemented}
    Vec3D CollisionalHeatFlux;
    ///Dissipation form collisions, \f$\sum_{ij} (v_{ia}+v_ja)/2 f_{ija} \psi_i\f$ 
    ///\todo{Dissipation calculations have not been checked, only implemented}
    Mdouble Dissipation;
    ///Elastic energy \f$2/4 \sum_{ij} (k \delta_{ij}^2 +k^t {\delta^t_{ij}}^2) \phi_i\f$
    ///\todo{Potential calculations have not been checked, only implemented; fstat file does not include the torsional and rolling spring and hence cannot account for their potential}
    Mdouble Potential;

    Vec3D LocalAngularMomentum;
    Matrix3D LocalAngularMomentumFlux;
    Matrix3D ContactCoupleStress;

    ///Prefactor of CG function which depends on $w
    Mdouble CG_invvolume;

    ///indicates that a position is a (fake) particles used for periodic walls
    int mirrorParticle;

private:
    ///Pointer to StatisticsVector (to obtain global parameters)
    static StatisticsVector<T>* gb;
    ///Position at which evaluation occurs
    Vec3D Position;
    
};
#endif
