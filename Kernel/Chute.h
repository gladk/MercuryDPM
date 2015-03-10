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

#ifndef CHUTE_H
#define CHUTE_H
#include "Mercury3D.h"

class InsertionBoundary;

/*!
 * \brief 0 grid-like fixed-particle bottom, 1 random fixed-particle bottom, 2 bottom slice
 */
enum RoughBottomType
{
    MONOLAYER_ORDERED, MONOLAYER_DISORDERED, MULTILAYER
};
/*!
 * \class Chute
 * \brief Creates chutes with different bottoms
 * \details Chute adds three new effects to the HGrid: the gravity direction can be set using the ChuteAngle variable, a (smooth or rough) bottom wall 
 is created by default, and some basic inflow and outflow routines are added
*/
class Chute : public Mercury3D
{
public:
    /*!
     * \brief This is the default constructor. All it does is set sensible defaults.
     */
    Chute();
    
    /*!
     * \brief Copy-constructor for creates an HGRID problem from an existing MD problem
     */
    Chute(const DPMBase& other);

    /*!
     * \brief 
     */    
    Chute(const MercuryBase& other);

    /*!
     * \brief 
     */    
    Chute(const Mercury3D& other);

    /*!
     * \brief This is the actual constructor; it is called do both constructors above.
     */
    void constructor();

    /*!
     * \brief 
     * \param[in]
     * \param[out]
     */
    int readNextArgument(int& i, int argc, char *argv[]);

    /*!
     * \brief 
     */
    void setupSideWalls();
    
    /*!
     * \brief This makes the chute periodic, in y
     */
    void makeChutePeriodic();
    
    /*!
     * \brief Get whether the chute is periodic
     */
    bool getIsPeriodic() const;
    
    /*!
     * \brief Initialize particle position, velocity, radius
     */
    void setupInitialConditions();

    /*!
     * \brief This function reads all chute data
     */
    void read(std::istream& is);

    /*!
     * \brief This function writes all chute data
     */
    virtual void write(std::ostream& os) const;

    /*!
     * \brief This function std::couts all chute data
     */
    void write(std::ostream& os, bool writeAllParticles = true) const;

//setters and getters

    /*!
     * \brief Allows radius of fixed particles to be changed
     */
    void setFixedParticleRadius(Mdouble fixedParticleRadius);
    
    /*!
     * \brief Allows radius of fixed particles to be accessed
     */
    Mdouble getFixedParticleRadius() const;
    
    /*!
     * \brief Changes RandomisedBottom
     */
    void setRoughBottomType(RoughBottomType roughBottomType);

    /*!
     * \brief Changes RandomisedBottom
     */
    void setRoughBottomType(std::string roughBottomTypeString);

    /*!
     * \brief Accesses RandomisedBottom
     */
    RoughBottomType getRoughBottomType() const;
    
    /*!
     * \brief Sets gravity vector according to chute angle (in degrees)
     */
    void setChuteAngle(Mdouble chuteAngle);
    
    /*!
     * \brief Sets gravity vector according to chute angle (in degrees)
     */
    void setChuteAngleAndMagnitudeOfGravity(Mdouble chuteAngle, Mdouble gravity);

    //void setChuteAngle(Mdouble new_, Mdouble gravity){if (new_>=0.0&&new_<=90.0) {ChuteAngle = new_; setGravity(Vec3D(sin(ChuteAngle*pi/180.0), 0.0, -cos(ChuteAngle*pi/180.0))*gravity);} else std::cerr << "WARNING : Chute angle must be within [0,90]" << std::endl;}
    /*!
     * \brief Gets chute angle (in radians)
     * \return
     * \todo Thomas: This should return the angle in degrees
     */
    Mdouble getChuteAngle() const;

    /*!
     * \brief 
     * \return
     */    
    Mdouble getChuteAngleDegrees() const;
    
    /*!
     * \brief Allows radius of fixed particles to be changed
     * \param[in]
     */
    void setMaxFailed(unsigned int maxFailed);
    
    /*!
     * \brief Allows radius of fixed particles to be accessed
     * \return
     */
    unsigned int getMaxFailed() const;

    /*!
     * \brief Allows radius of inflow particles to be changed
     * \param[in]
     */
    void setInflowParticleRadius(Mdouble InflowParticleRadius);
    
    /*!
     * \brief Allows radius of inflow particles to be set to a range of values
     * \param[in]
     */
    void setInflowParticleRadius(Mdouble minInflowParticleRadius, Mdouble maxInflowParticleRadius);

    /*!
     * \brief 
     * \param[in]
     */    
    void setMinInflowParticleRadius(Mdouble minInflowParticleRadius);

    /*!
     * \brief 
     * \param[in]
     */    
    void setMaxInflowParticleRadius(Mdouble maxInflowParticleRadius);

    /*!
     * \brief Allows radius of inflow particles to be accessed
     * \return
     */
    Mdouble getInflowParticleRadius() const;
    
    /*!
     * \brief Allows radius of inflow particles to be accessed
     * \return
     */
    const Mdouble getMinInflowParticleRadius() const;
    
    /*!
     * \brief Allows radius of inflow particles to be accessed
     * \return
     */
    const Mdouble getMaxInflowParticleRadius() const;
    
    /*!
     * \brief Changes inflow height
     * \param[in]
     */
    void setInflowHeight(Mdouble inflowHeight);
    
    /*!
     * \brief Accesses inflow height
     * \return
     */
    Mdouble getInflowHeight() const;
    
    /*!
     * \brief Changes inflow velocity
     * \param[in]
     */
    void setInflowVelocity(Mdouble inflowVelocity);
    
    /*!
     * \brief Accesses inflow velocity
     * \return
     */
    Mdouble getInflowVelocity() const;
    
    /*!
     * \brief Changes inflow velocity variance
     * \param[in]
     */
    void setInflowVelocityVariance(Mdouble inflowVelocityVariance);
    
    /*!
     * \brief Accesses inflow velocity variance
     * \return
     */
    Mdouble getInflowVelocityVariance() const;

    /*!
     * \brief Access function that set the width of the chute
     * \param[in]
     */
    void setChuteWidth(Mdouble chuteWidth);
    
    /*!
     * \brief 
     * \return
     */
    Mdouble getChuteWidth() const;
    
    /*!
     * \brief 
     * \param[in]
     */
    virtual void setChuteLength(Mdouble chuteLength);

    /*!
     * \brief 
     * \return
     */    
    Mdouble getChuteLength() const;

    /*!
     * \brief 
     * \return
     */
    int getNCreated() const;

    /*!
     * \brief 
     */
    void increaseNCreated();

    /*!
     * \brief 
     * \param[in]
     */
    void setInsertionBoundary(InsertionBoundary* insertionBoundary);

protected:
    /*!
     * \brief 
     */
    void actionsBeforeTimeStep();

    /*!
     * \brief Here we define the outflow
     */
    void cleanChute();

    /*!
     * \brief Create the bottom of chute out of particles
     */
    virtual void createBottom();

    /*!
     * \brief 
     */
    void printTime() const;

private:

    /*!
     * \brief chute angle in degrees
     */
    Mdouble chuteAngle_;
    /*!
     * \brief radius of the fixed particles at the bottom
     */
    Mdouble fixedParticleRadius_;
    /*!
     * \brief minimal radius of inflowing particles
     */
    Mdouble minInflowParticleRadius_;
    /*!
     * \brief maximal radius of inflowing particles
     */
    Mdouble maxInflowParticleRadius_;
    /*!
     * \brief Average inflow velocity in x-direction
     */
    Mdouble inflowVelocity_;
    /*!
     * \brief Inflow velocity variance in x-direction
     */
    Mdouble inflowVelocityVariance_;
    /*!
     * \brief Height of inflow
     */
    Mdouble inflowHeight_;
    /*!
     * \brief distinguishes between grid-like (0), one-layer randomised (1), and thick random dense (2) bottom
     */
    RoughBottomType roughBottomType_;
    /*!
     * \brief indicates how many attempts are undertake to insert a new particle before each timestep
     */
    unsigned int maxFailed_;
    /*!
     * \brief 
     */
    InsertionBoundary* insertionBoundary_;
    /*!
     * \brief 
     */
    bool isChutePeriodic_;
    
};

#endif
