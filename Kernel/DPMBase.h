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

#ifndef MD_H
#define MD_H

//If user has not provided a log lever we set it to INFO by default.
#ifndef LOG_MAIN_LEVEL
#define LOG_MAIN_LEVEL
#endif

//This is the class that defines the std_save routines
#include "FilesAndRunNumber.h"
//The vector class contains a 3D vector class.
#include "Math/Vector.h"
//This class defines the particle handler
#include "ParticleHandler.h"
//This class defines the wall handler
#include "WallHandler.h"
//This class defines the boundary handler
#include "BoundaryHandler.h"
//This class defines the interaction handler
#include "InteractionHandler.h"
//This class defines the interaction handler
#include "SpeciesHandler.h"
//#include "Species/Species.h"
//This class defines the posibleContact lists
#ifdef CONTACT_LIST_HGRID
    #include "PossibleContactList.h"
#endif
//This class defines the random number generator
#include "Math/RNG.h"


/*!
 * \class DPMBase
 * \brief A class that defines and solves a DPM problem
 * \bug When restarting the first time step is not saved, therefore there is a missing time step after a restart
 */
class DPMBase : public FilesAndRunNumber
{
public:
    /*!
     * \brief A public constructor, which sets defaults so the problem can be solved off the shelf. This is where the constructor is defined; sets up a basic two dimensional problem which can be solved off the shelf
     */
    void constructor();

    /*!
     * \brief default constructor
     */
    DPMBase();
    
    /*!
     * \brief
     */
    DPMBase(const FilesAndRunNumber& other);

    /*!
     * \brief
     */    
    DPMBase(const DPMBase& other);
    
    /*!
     * \brief destructor
     */
    virtual ~DPMBase();

    /*!
     * \brief The work horse of the code
     */
    void solve();

    void checkSettings();

    virtual void writeOutputFiles();

    /*!
     * \brief Read arguments before solving
     */
    void solve(int argc, char* argv[]);

    /*!
     * \brief This function allows the initial conditions of the particles to be set, by default locations is random. Remember particle properties must also be defined here.
     * \todo I (Anthony) wants to change this to be an external function. This has a lot of advantages espcially when using copy-constructors. This is a major change and will break other codes, so therefore has to be done carefully.
     * \details This sets up the particles initial conditions it is as you expect the user to override this. By default the particles are randomly disibuted
     */
    virtual void setupInitialConditions();

    /*!
     * \brief This creates a scipt which can be used to load the xballs problem to display the data just generated
     */
    virtual void writeXBallsScript() const;

    /*!
     * \brief Allows the user to set what is written into the info column in the data file. By default is store the Species ID number
     */
    virtual double getInfo(const BaseParticle& P) const;

    /*!
     * \brief Stores all MD data
     */
    virtual void writeRestartFile();

    /*!
     * \brief Loads all MD data
     */
    int readRestartFile();

    /*!
     * \brief
     */
    int readRestartFile(std::string filename);

//    /*!
//     * \brief Loads all MD data and plots statistics for all timesteps in the .data file
//     */
//    void statisticsFromRestartData(const char *name);

    /*!
     * \brief Writes all MD data into a restart file
     */
    virtual void write(std::ostream& os, bool writeAllParticles=true) const;

    /*!
     * \brief Reads all MD data into a restart file
     */
    virtual void read(std::istream& is);

    /*!
     * \brief Reads all MD data into a restart file; old version
     */
    virtual void readOld(std::istream &is);

    /*!
     * \brief This allows particle data to be reloaded from data files
     */
    bool readDataFile(const char* filename, unsigned int format = 0);

    /*!
     * \brief allows the user to read par.ini files (useful to read MDCLR files)
     */
    bool readParAndIniFiles(const char* filename);

    /*!
     * \brief
     */
    bool readNextDataFile(unsigned int format = 0);

    /*!
     * \brief
     */
    bool findNextExistingDataFile(Mdouble tmin, bool verbose = true);

    /*!
     * \brief
     */
    friend inline std::ostream& operator<<(std::ostream& os, const DPMBase &md);

    /*!
     * \brief
     */
    int readArguments(int argc, char *argv[]);

    /*!
     * \brief
     */
    virtual int readNextArgument(int& i, int argc, char *argv[]);

    /*!
     * \brief
     */
    virtual bool checkParticleForInteraction(const BaseParticle& P);

//getters and setters

    /*!
     * \brief Access function for the time
     */
    Mdouble getTime() const;

    unsigned int getNtimeSteps() const;

    /*!
     * \brief Access function for the time
     */
    void setTime(Mdouble time);

    /*!
     * \brief Allows the upper time limit to be changed
     */
    void setTimeMax(Mdouble new_tmax);

    /*!
     * \brief Allows the upper time limit to be accessed
     */
    Mdouble getTimeMax() const;

#ifdef CONTACT_LIST_HGRID
    PossibleContactList& getPossibleContactList();
#endif

    /*!
     * \brief Sets how often the data is saved using the number of saves wanted, tmax, and dt. See also \ref Files::setSaveCount
     */
    void setDoCGAlways(bool new_);

    /*!
     * \brief \todo{these functions should also update the mixed species}
     */

    /*!
     * \brief
     */
    void setRotation(bool new_);

    /*!
     * \brief
     */
    bool getRotation() const;

    /*!
     * \brief
     */
    bool getDoCGAlways() const;
    
    /*!
     * \brief Get xmin
     */
    Mdouble getXMin() const;

    /*!
     * \brief Get xmax
     */
    Mdouble getXMax() const;

    /*!
     * \brief Gets ymin
     */
    Mdouble getYMin() const;

    /*!
     * \brief Gets ymax
     */
    Mdouble getYMax() const;

    /*!
     * \brief Gets zmin
     */
    Mdouble getZMin() const;

    /*!
     * \brief Gets zmax
     */
    Mdouble getZMax() const;
    
    /*!
     * \brief Sets xmin and walls, assuming the standard definition of walls as in the default constructor
     */
    void setXMin(Mdouble new_xmin);

    /*!
     * \brief
     */    
    void setYMin(Mdouble new_ymin);
    
    /*!
     * \brief Sets ymin and walls, assuming the standard definition of walls as in the default constructor
     */
    void setZMin(Mdouble new_zmin);
    
    /*!
     * \brief Sets xmax and walls, assuming the standard definition of walls as in the default constructor
     */
    void setXMax(Mdouble new_xmax);
    
    /*!
     * \brief Sets ymax and walls, assuming the standard definition of walls as in the default constructor
     */
    void setYMax(Mdouble new_ymax);
    
    /*!
     * \brief Sets ymax and walls, assuming the standard definition of walls as in the default constructor
     */
    void setZMax(Mdouble new_zmax);
    
    /*!
     * \brief Allows the time step dt to be changed
     */
    void setTimeStep(Mdouble new_dt);

    /*!
     * \brief Allows the time step dt to be accessed
     */
    Mdouble getTimeStep() const;
    
    /*!
     * \brief Set the xball output mode
     */
    void setXBallsColourMode(int new_cmode);

    /*!
     * \brief Get the xball output mode
     */
    int getXBallsColourMode() const;
    
    /*!
     * \brief Set the scale of vectors in xballs
     */
    void setXBallsVectorScale(double new_vscale);

    /*!
     * \brief
     */
    double getXBallsVectorScale() const;

    /*!
     * \brief Set the additional arguments for xballs
     */
    void setXBallsAdditionalArguments(std::string new_);

    /*!
     * \brief
     */
    std::string getXBallsAdditionalArguments() const;
    
    /*!
     * \brief Set the scale of the xballs problem. The default is fit to screen
     */
    void setXBallsScale(Mdouble new_scale);

    /*!
     * \brief
     */
    double getXBallsScale() const;

    /*!
     * \brief Allows the gravitational acceleration to be changed
     */
    void setGravity(Vec3D new_gravity);

    /*!
     * \brief Allows the gravitational acceleration to be accessed
     */
    Vec3D getGravity() const;

    void setDimension(int new_dim);

    /*!
     * \brief Allows the dimension of the simulation to be changed
     */
    void setSystemDimensions(int new_dim);

    /*!
     * \brief Allows the dimension of the simulation to be accessed. Note there is also a particle dimension
     */
    int getSystemDimensions() const;

    ///Allows the dimension of the particle (f.e. for mass) to be changed
    void setParticleDimensions(int particleDimensions);

    ///Allows the dimension of the particle (f.e. for mass) to be accessed
    int getParticleDimensions() const;


    /*!
     * \brief Gets restart_version
     */
    std::string getRestartVersion() const;

    /*!
     * \brief Sets restart_version
     */
    void setRestartVersion(std::string new_);

    /*!
     * \brief Gets restarted
     */
    bool getRestarted() const;
    
    /*!
     * \brief
     */
    void setRestarted(bool new_);

    /*!
     * \brief Gets restarted
     */
    bool getAppend() const;

    /*!
     * \brief Sets restarted
     */
    void setAppend(bool new_);
    
    /*!
     * \brief Gets ene_ela
     */
    Mdouble getElasticEnergy() const;

    /*!
     * \brief
     */
    Mdouble getKineticEnergy() const;
    
    /*!
     * \brief
     */
    bool areInContact(const BaseParticle* pI, const BaseParticle* pJ) const;

//functions that should only be used in the class definitions 
protected:
    
    /*!
     * \brief This does the force computation
     */
    virtual void computeAllForces();

    /*!
     * \brief Computes the forces between particles (internal in the sense that the sum over all these forces is zero i.e. fully modelled forces)
     */
    virtual void computeInternalForces(BaseParticle* i);

    /*!
     * \brief Computes the forces between particles (internal in the sense that the sum over all these forces is zero i.e. fully modelled forces)
     */
    virtual void computeInternalForces(BaseParticle* P1, BaseParticle* P2);

    /*!
     * \brief This is were the computation of external forces takes place (e.g. gravity)
     */
    virtual void computeExternalForces(BaseParticle* PI);

    /*!
     * \brief This is were the walls are
     */
    virtual void computeWalls(BaseParticle* PI);

    /*!
     * \brief This is actions before the start of the main time loop
     */
    virtual void actionsBeforeTimeLoop();

    /*!
     * \brief This is actions before the start of the main time loop
     */
    virtual void hGridActionsBeforeTimeLoop();

    /*!
     * \brief This is action before the time step is started
     */
    virtual void hGridActionsBeforeTimeStep();

public:
    /*!
     * \brief
     */
    virtual void hGridInsertParticle(BaseParticle *obj UNUSED);

    /*!
     * \brief
     */
    virtual void hGridUpdateParticle(BaseParticle *obj UNUSED);

    /*!
     * \brief
     */
    virtual void hGridRemoveParticle(BaseParticle *obj UNUSED);

protected:

    /*!
     * \brief
     */
    virtual bool getHGridUpdateEachTimeStep() const;

    /*!
     * \brief This is action before the time step is started
     */
    virtual void actionsBeforeTimeStep();

    /*!
     * \brief This is actions at the end of the code, but before the files are closed
     */
    virtual void actionsAfterSolve();

    /*!
     * \brief This is action after the time step is finished
     */
    virtual void actionsAfterTimeStep();

    /*!
     * \brief This function outputs the location and velocity of the particle in a format the xballs progream can read
     */
    virtual void outputXBallsData(std::ostream& os) const;
    
    /*!
     * \brief
     */
    virtual void outputXBallsDataParticle(const unsigned int i,const unsigned int format, std::ostream& os) const;

    /*!
     * \brief This function gathers statistical data for ene file
     */
    virtual void writeEneHeader(std::ostream& os) const;

    /*!
     * \brief
     */
    virtual void writeFstatHeader(std::ostream& os) const;

    /*!
     * \brief This function outputs statistical data - The default is to compute the rotational kinetic engergy, linear kinetic energy, and the centre of mass.
     */
    virtual void writeEneTimestep(std::ostream& os) const;

    /*!
     * \brief Functions for statistics
     */
    virtual void initialiseStatistics();

    /*!
     * \brief
     */
    virtual void outputStatistics();

    void gatherContactStatistics();

    /*!
     * \brief
     */
    virtual void processStatistics(bool usethese UNUSED);

    /*!
     * \brief
     */
    virtual void finishStatistics();

    /*!
     * \brief This is were the integration is done
     */
    virtual void integrateBeforeForceComputation();

    /*!
     * \brief This is were the integration is done
     */
    virtual void integrateAfterForceComputation();

    /*!
     * \brief
     */
    virtual void checkInteractionWithBoundaries();

public:
    /*!
     * \brief
     */
    virtual void hGridUpdateMove(BaseParticle*, Mdouble);

    /*!
     * \brief
     * //Not unsigned index because of possible wall collisions.
     */
    virtual void gatherContactStatistics(int index1 UNUSED, int index2 UNUSED, Vec3D Contact UNUSED, Mdouble delta UNUSED, Mdouble ctheta UNUSED, Mdouble fdotn UNUSED, Mdouble fdott UNUSED, Vec3D P1_P2_normal_ UNUSED, Vec3D P1_P2_tangential UNUSED);

protected:
    /*!
     * \brief
     */
    virtual void hGridActionsBeforeIntegration();

    /*!
     * \brief
     */
    virtual void hGridActionsAfterIntegration();

    /*!
     * \brief Broad phase of contact detection goes here. Default check all contacts.
     */
    virtual void broadPhase(BaseParticle* i);

    /*!
     * \brief
     */
    void setFixedParticles(unsigned int n);

    /*!
     * \brief
     */
    void initialiseTangentialSprings();

    /*!
     * \brief Computes the mass of each particle
     */
    void computeParticleMasses();

    /*!
     * \brief prints time to std::cout
     */
    virtual void printTime() const;

    /*!
     * \brief
     */
    virtual bool continueSolve() const;

    /*!
     * \brief
     */
    void outputInteractionDetails() const;

    bool isTimeEqualTo(Mdouble time) const;

protected:

    /*!
     * \brief Removes periodic duplicate Particles
     * \details Removes particles created by CheckAndDuplicatePeriodicParticle(int i, int nWallPeriodic)). Note that between these two functions it is not allowed to create additional functions
     */
    void removeDuplicatePeriodicParticles();

    /*!
     * \brief Calls Check_and_Duplicate_Periodic_Particle for all Particles curently in Particles[] and returns the number of particles created
     */
    void checkAndDuplicatePeriodicParticles();

private:
    
    /*!
     * \brief The dimension of the simulation i.e. 2D or 3D
     */
    int systemDimensions_;

    int particleDimensions_; ///<determines if 2D or 3D volume is used for mass calculations

    /*!
     * \brief Gravitational acceleration
     */
    Vec3D gravity_;

    /*!
     * \brief These store the size of the domain, assume walls at the ends
     */
    Mdouble xMin_;
    Mdouble xMax_;
    Mdouble yMin_;
    Mdouble yMax_;
    Mdouble zMin_;
    Mdouble zMax_;

    /*!
     * \brief This stores the current time
     */
    Mdouble time_;

    /*!
     * \brief This stores the number of time steps
     */
    unsigned int ntimeSteps_;

    /*!
     * \brief The size of time step
     */
    Mdouble timeStep_;

    /*!
     * \brief This stores the final time, where the simulation ends
     */
    Mdouble timeMax_;

    /*!
     * \brief used in force calculations
     * \details stores the potential energy in the elastic springs (global, because it has to be calculated in the force loop
     */
    Mdouble elasticEnergy_;

    /*!
     * \brief
     */
    std::string restartVersion_; ///<to read new and old restart data

    /*!
     * \brief
     */
    bool restarted_; ///<to read new and old restart data

    /*!
     * \brief
     */
    bool append_;

    /*!
     * \brief
     */
    bool rotation_;


//This is the private data that is only used by the xballs output

    /*!
     * \brief sets the xballs argument cmode (see xballs.txt)
     */
    int xBallsColourMode_; 
    
    /*!
     * \brief sets the xballs argument vscale (see xballs.txt)
     */
    Mdouble xBallsVectorScale_;

    /*!
     * \brief sets the xballs argument scale (see xballs.txt)
     */
    Mdouble xBallsScale_;

    /*!
     * \brief std::string where additional xballs argument can be specified (see xballs.txt)
     */
    std::string xBallsAdditionalArguments_; 

//This belongs to the hGrid

/*!
 * \brief
 */
#ifdef CONTACT_LIST_HGRID
    PossibleContactList possibleContactList;
#endif

public:

    /*!
     * \brief These are the particle parameters like dissipation etc.
     */
    SpeciesHandler speciesHandler;

    /*!
     * \brief This is a random generator, often used by the initial conditions etc...
     */
    RNG random;

    /*!
     * \brief Object of the class particleHandler
     */
    ParticleHandler particleHandler;

    /*!
     * \brief Object of the class wallHandler
     */
    WallHandler wallHandler;

    /*!
     * \brief Object of the class BoundaryHandler
     */
    BoundaryHandler boundaryHandler;

    /*!
     * \brief Object of the class InteractionHandler
     */
    InteractionHandler interactionHandler;

};
#endif
