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
