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
 * \brief enum for determining the type of rough bottom (if any) of the chute. Either of
 * the enum options can be assigned to the roughBottomType_ data member using Chute::setRoughBottomType().
 * The options are as follows:
 *    * MONOLAYER_ORDERED:      Bottom will be a rectangularly ordered monolayer of particles
 *    * MONOLAYER_DISORDERED:   Bottom will be a disordered monolayer of particles
 *    * MULTILAYER:             Bottom will be a multilayer of particles
 *    * FLAT:                   Flat bottom (not rough)
 * See also the documentation of Chute::createBottom().
 * 
 * \todo consider converting to an enum class. 
 * \todo consider changing to just 'bottomType' and add 'FLAT' (or something similar)
 * as an option so that from ALL bottom types can be chosen with just ONE enumerator
 * IFCD: I did add FLAT, but since there are multiple users using RoughBottomType,
 * it might be a bad idea to change the interface with BottomType. Please also
 * check if I have forgotten places to add "FLAT" to make this a complete interface.
 */
enum RoughBottomType
{
    MONOLAYER_ORDERED, MONOLAYER_DISORDERED, MULTILAYER, FLAT
};
/*!
 * \class Chute
 * \brief Creates chutes with different bottoms. Inherits from Mercury3D (-> MercuryBase -> DPMBase).
 * \details Chute adds three new effects as compared to a 'normal' Mercury3D object: 
 * the gravity direction can be set using the ChuteAngle variable, a (smooth or 
 * rough) bottom wall is created by default, and some basic inflow and outflow 
 * routines are added.
*/
class Chute : public Mercury3D
{
public:
    /*!
     * \brief This is the default constructor. All it does is set sensible defaults.
     */
    Chute();
    
    /*!
     * \brief Copy constructor, converts an existing DPMBase problem into a Chute problem
     */
    Chute(const DPMBase& other);

    /*!
     * \brief Copy constructor, converts an existing MercuryBase problem into a Chute problem
     */  
    Chute(const MercuryBase& other);

    /*!
     * \brief Copy constructor, converts an existing Mercury3D problem into a Chute problem
     */   
    Chute(const Mercury3D& other);
    
    /*!
     * \brief Default copy constructor
     */ 
    Chute(const Chute& other);

    /*!
     * \brief This is the actual constructor METHOD; it is called by all constructors above 
     * (except the default copy constructor).
     */
    void constructor();

    /*!
     * \brief This method can be used for reading object properties from a string.
     */
    bool readNextArgument(int& i, int argc, char *argv[]);

    /*!
     * \brief Creates chute side walls (either solid or periodic)
     */
    void setupSideWalls();
    
    /*!
     * \brief This makes the chute periodic in Y
     */
    void makeChutePeriodic();
    
    /*!
     * \brief Returns whether the chute is periodic in Y
     */
    bool getIsPeriodic() const;
    
    /*!
     * \brief Creates bottom, side walls and a particle insertion boundary
     */
    void setupInitialConditions();

    /*!
     * \brief Reads all chute properties from an istream
     */
    void read(std::istream& is);

    /*!
     * \brief This function writes the Chute properties to an ostream, and adds 
     * the properties of ALL chute particles as well
     */
    void write(std::ostream& os, bool writeAllParticles = true) const;

//setters and getters

    /*!
     * \brief Sets the particle radius of the fixed particles which constitute the 
     * (rough) chute bottom
     */
    void setFixedParticleRadius(Mdouble fixedParticleRadius);
    
    /*!
     * \brief Returns the particle radius of the fixed particles which constitute the 
     * (rough) chute bottom
     */
    Mdouble getFixedParticleRadius() const;
    
    /*!
     * \brief Sets the type of rough bottom of the chute
     */
    void setRoughBottomType(RoughBottomType roughBottomType);

    /*!
     * \brief Sets the type of rough bottom of the chute, using a string with the
     * EXACT enum type as input
     */
    void setRoughBottomType(std::string roughBottomTypeString);

    /*!
     * \brief Returns the type of (rough) bottom of the chute
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
     * \brief Returns the chute angle (in radians)
     */
    Mdouble getChuteAngle() const;

    /*!
     * \brief Returns the chute angle (in degrees)
     */    
    Mdouble getChuteAngleDegrees() const;
    
    /*!
     * \brief Sets the number of times a particle will be tried to be added to the insertion boundary
     */
    void setMaxFailed(unsigned int maxFailed);
    
    /*!
     * \brief Returns the number of times a particle will be tried to be added to the insertion boundary
     */
    unsigned int getMaxFailed() const;

    /*!
     * \brief Sets the radius of the inflow particles to a single one (i.e. ensures
     * a monodisperse inflow).
     */
    void setInflowParticleRadius(Mdouble inflowParticleRadius);
    
    /*!
     * \brief Sets the minimum and maximum radius of the inflow particles
     */
    void setInflowParticleRadius(Mdouble minInflowParticleRadius, Mdouble maxInflowParticleRadius);

    /*!
     * \brief sets the minimum radius of inflow particles
     */    
    void setMinInflowParticleRadius(Mdouble minInflowParticleRadius);

    /*!
     * \brief Sets the maximum radius of inflow particles
     */    
    void setMaxInflowParticleRadius(Mdouble maxInflowParticleRadius);

    /*!
     * \brief Returns the average radius of inflow particles
     */
    Mdouble getInflowParticleRadius() const;
    
    /*!
     * \brief returns the minimum radius of inflow particles
     */
    Mdouble getMinInflowParticleRadius() const;
    
    /*!
     * \brief Returns the maximum radius of inflow particles
     */
    Mdouble getMaxInflowParticleRadius() const;
    
    /*!
     * \brief Sets maximum inflow height (Z-direction)
     */
    void setInflowHeight(Mdouble inflowHeight);
    
    /*!
     * \brief Returns the maximum inflow height (Z-direction)
     */
    Mdouble getInflowHeight() const;
    
    /*!
     * \brief Sets the average inflow velocity
     */
    void setInflowVelocity(Mdouble inflowVelocity);
    
    /*!
     * \brief Returns the average inflow velocity
     */
    Mdouble getInflowVelocity() const;
    
    /*!
     * \brief Sets the inflow velocity variance
     */
    void setInflowVelocityVariance(Mdouble inflowVelocityVariance);
    
    /*!
     * \brief Returns the inflow velocity variance
     */
    Mdouble getInflowVelocityVariance() const;

    /*!
     * \brief Sets the chute width (Y-direction)
     */
    void setChuteWidth(Mdouble chuteWidth);
    
    /*!
     * \brief Returns the chute width (Y-direction)
     */
    Mdouble getChuteWidth() const;
    
    /*!
     * \brief Sets the chute length (X-direction)
     */
    virtual void setChuteLength(Mdouble chuteLength);

    /*!
     * \brief Returns the chute length (X-direction)
     */    
    Mdouble getChuteLength() const;

    /*!
     * \brief 
     */
    int getNCreated() const;

    /*!
     * \brief 
     */
    void increaseNCreated();

    /*!
     * \brief Sets the chute insertion boundary 
     */
    void setInsertionBoundary(InsertionBoundary* insertionBoundary);

protected:
    /*!
     * \brief Calls Chute::cleanChute().
     */
    void actionsBeforeTimeStep();

    /*!
     * \brief Deletes all outflow particles once every 100 time steps
     */
    void cleanChute();

    /*!
     * \brief Creates the chute bottom, which can be either flat or one of three flavours of rough
     */
    virtual void createBottom();

    /*!
     * \brief prints time, max time and number of particles
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
     * \brief Inflow velocity variance in x-direction (in ratio of inflowVelocity_)
     */
    Mdouble inflowVelocityVariance_;
    /*!
     * \brief Height of inflow
     */
    Mdouble inflowHeight_;
    /*!
     * \brief Determines the type of rough bottom created (if any). See also the enum
     * RoughBottomType at the beginning of this header file.
     */
    RoughBottomType roughBottomType_;
    /*!
     * \brief indicates how many attempts are made to insert a new particle
     * into the insertion boundary before the boundary is considered filled.
     */
    unsigned int maxFailed_;
    /*!
     * \brief (Pointer to) the Chute's insertion boundary
     */
    InsertionBoundary* insertionBoundary_;
    /*!
     * \brief Determines whether the chute has periodic (TRUE) or solid (FALSE) walls
     * in the Y-direction
     */
    bool isChutePeriodic_;
    
};

#endif
