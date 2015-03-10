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
//This class defines the Species handler
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
 * \brief The DPMBase header includes quite a few header files, defining all the
 *        handlers, which are essential. Moreover, it defines and solves a DPM problem.
 *        It is inherited from FilesAndRunNumber (public). 
 * \bug When restarting the first time step is not saved, therefore there is a missing
 *      time step after a restart
 */
class DPMBase : public FilesAndRunNumber
{
public:
    /*!
     * \brief A function which initialises the member variables to default values, so
     *        that the problem can be solved off the shelf; sets up a basic two dimensional
     *        problem which can be solved off the shelf. It is called in the constructor DPMBase().
     */
    void constructor();

    /*!
     * \brief Constructor that calls the "void constructor()"
     */
    DPMBase();
    
    /*!
     * \brief Copy constructor type-1
     */
    DPMBase(const FilesAndRunNumber& other);

    /*!
     * \brief Copy constructor type-2
     */    
    DPMBase(const DPMBase& other);
    
    /*!
     * \brief virtual destructor
     */
    virtual ~DPMBase();

    /*!
     * \brief The work horse of the code
     */
    void solve();
    
    /*!
     * \brief Checks if the essentials are set properly to go ahead with solving the problem.
     * \details 1. Checks if at least one species exists in the SpeciesHandler.
     *          2. Checks if particle dimensions are set.
     *          3. Checks is system dimensions are set.
     *          4. Checks if the time step is set or not.
     */
    void checkSettings();

    /*!
     * \brief Writes the simulation data onto all the files i.e. .data, .ene, .fstat ...
     */
    virtual void writeOutputFiles();

    /*!
     * \brief The solve function is the work horse of the code with the user input.
     */
    void solve(int argc, char* argv[]);

    /*!
     * \brief This function allows to set the initial conditions for our problem to be solved, 
     *        by default particle locations are randomly set. Remember particle properties must
     *        also be defined here.
     * \todo I (Anthony) wants to change this to be an external function. This has a lot of advantages
     *       especially when using copy-constructors. This is a major change and will break other codes,
     *       so therefore has to be done carefully.
     * \details This sets up the particles initial conditions it is as you expect the user to override
     *          this. By default the particles are randomly disibuted
     */
    virtual void setupInitialConditions();

    /*!
     * \brief This writes a script which can be used to load the xballs problem to display the data just generated
     */
    virtual void writeXBallsScript() const;

    /*!
     * \brief A virtual method that allows the user to overrride and set what is written into
     *        the info column in the data file. By default it returns the Species ID number
     */
    virtual double getInfo(const BaseParticle& P) const;

    /*!
     * \brief Stores all the particle data for current save time step. 
     *        Calls the write function.
     */
    virtual void writeRestartFile();

    /*!
     * \brief Reads all the particle data corresponding to the current saved time step.
     *        Which is what the restart file basically stores. The problem description with the 
     *        latest particle data.
     */
    int readRestartFile();

    /*!
     * \brief Also reads all the particle data corresponding to the current saved time step.
     */
    int readRestartFile(std::string fileName);

//    /*!
//     * \brief Loads all MD data and plots statistics for all timesteps in the .data file
//     */
//    void statisticsFromRestartData(const char *name);

    /*!
     * \brief Writes all particle data into a restart file
     */
    virtual void write(std::ostream& os, bool writeAllParticles=true) const;

    /*!
     * \brief Reads all particle data into a restart file
     */
    virtual void read(std::istream& is);

    /*!
     * \brief Reads all particle data into a restart file; old version
     */
    virtual void readOld(std::istream &is);

    /*!
     * \brief This allows particle data to be reloaded from data files
     * \details E.g. If one has a data file. This function loads data from the .data
     *          file i.e. you get position, velocity, angular velocty, radius .. info.
     *          See also MD::readRestartFile
     *          For XBalls: 
     *          Can read in format_ 14 - 8 or format_ 7 data format.
     *          This code saves in format_ 8 for 2D and format_ 14 for 3D.
     *          So if no extra parameters are specified it will assume things many parameters,
     *          like density cannot be set using the data file.
     * \todo change from deprecated const char* to std::string
     */
    bool readDataFile(const char* fileName, unsigned int format = 0);

    /*!
     * \brief Allows the user to read par.ini files (useful to read MDCLR files)
     */
    bool readParAndIniFiles(const char* fileName);

    /*!
     * \brief Reads the next data file with default format=0. However, one can 
     *        modify the format based on whether the particle data corresponds to
     *        3D or 2D data. See XBalls/xballs.txt 
     */
    bool readNextDataFile(unsigned int format = 0);

    /*!
     * \brief Useful when fileType is chosen as Multiple Files or Multiple files with padded.
     */
    bool findNextExistingDataFile(Mdouble tMin, bool verbose = true);

    /*!
     * \brief Operator overloading of DPMBase class.
     */
    friend inline std::ostream& operator<<(std::ostream& os, const DPMBase &md);

    /*!
     * \brief Can interpret main function input arguments that are passed by the
     *        driver codes
     */
    bool readArguments(int argc, char *argv[]);

    /*!
     * \brief 
     */
    virtual bool readNextArgument(int& i, int argc, char *argv[]);

    /*!
     * \brief Checks if the particle having any interaction with walls or other particles.
     */
    virtual bool checkParticleForInteraction(const BaseParticle& P);

    //getters and setters

    /*!
     * \brief Access function for the time
     */
    Mdouble getTime() const;

    /*!
     * \brief Returns the current counter of time steps.
     */
    unsigned int getNtimeSteps() const;

    /*!
     * \brief Access function for the time
     */
    void setTime(Mdouble time);

    /*!
     * \brief Allows the upper time limit to be changed
     */
    void setTimeMax(Mdouble newTMax);

    /*!
     * \brief Allows the user to access the total simulation time during the simulation.
     *        Cannot change it though.
     */
    Mdouble getTimeMax() const;

    #ifdef CONTACT_LIST_HGRID
        PossibleContactList& getPossibleContactList();
    #endif

    /*!
     * \brief 
     */
    void setDoCGAlways(bool newDoCGFlag);

    /*!
     * \todo{these functions should also update the mixed species}
     */

    /*!
     * \brief Allows to set the flag for enabling or disabling particle rotation 
     *        in the simulations.
     */
    void setRotation(bool newRotFlag);

    /*!
     * \brief Returns a flag indicating if particle rotation is enabled or disabled.
     */
    bool getRotation() const;

    /*!
     * \brief 
     */
    bool getDoCGAlways() const;
    
    /*!
     * \brief If the length of the problem domain in x-direction is XMax - XMin, 
     *        then getXMin() returns XMin,
     */
    Mdouble getXMin() const;

    /*!
     * \brief If the length of the problem domain in x-direction is XMax - XMin, 
     *        then getXMax() returns XMax,
     */
    Mdouble getXMax() const;

    /*!
     * \brief If the length of the problem domain in y-direction is YMax - YMin,
     *        then getYMin() returns YMin,
     */
    Mdouble getYMin() const;

    /*!
     * \brief If the length of the problem domain in y-direction is YMax - YMin,
     *        then getYMax() returns XMax,
     */
    Mdouble getYMax() const;

    /*!
     * \brief If the length of the problem domain in z-direction is ZMax - ZMin,
     *        then getZMin() returns ZMin,
     */
    Mdouble getZMin() const;

    /*!
     * \brief If the length of the problem domain in z-direction is ZMax - ZMin,
     *        then getZMax() returns ZMax,
     */
    Mdouble getZMax() const;
    
    /*!
     * \brief If the length of the problem domain in x-direction is XMax - XMin, 
     *        this method sets XMin.
     */
    void setXMin(Mdouble newXMin);

    /*!
     * \brief If the length of the problem domain in y-direction is YMax - YMin,
     *        this method sets YMin. 
     */       
    void setYMin(Mdouble newYMin);
    
    /*!
     * \brief If the length of the problem domain in z-direction is ZMax - ZMin, 
     *        this method sets ZMin.
     */
    void setZMin(Mdouble newZMin);
    
    /*!
     * \brief If the length of the problem domain in x-direction is XMax - XMin, 
     *        this method sets XMax.
     */
    void setXMax(Mdouble newXMax);
    
    /*!
     * \brief If the length of the problem domain in y-direction is YMax - YMin, 
     *        this method sets YMax.
     */
    void setYMax(Mdouble newYMax);
    
    /*!
     * \brief If the length of the problem domain in z-direction is XMax - XMin, 
     *        this method sets ZMax.
     */       
    void setZMax(Mdouble newZMax);
    
    /*!
     * \brief Allows the time step dt to be changed
     */
    void setTimeStep(Mdouble newDt);

    /*!
     * \brief Allows the time step dt to be accessed
     */
    Mdouble getTimeStep() const;
    
    /*!
     * \brief Set the xball output mode
     */
    void setXBallsColourMode(int newCMode);

    /*!
     * \brief Get the xball colour mode (CMode)
     */
    int getXBallsColourMode() const;
    
    /*!
     * \brief Set the scale of vectors in xballs.
     */
    void setXBallsVectorScale(double newVScale);

    /*!
     * \brief Returns the scale of vectors used in xballs. 
     */
    double getXBallsVectorScale() const;

    /*!
     * \brief Set the additional arguments for xballs
     */
    void setXBallsAdditionalArguments(std::string newXBArgs);

    /*!
     * \brief
     */
    std::string getXBallsAdditionalArguments() const;
    
    /*!
     * \brief Sets the scale of the view (either normal, zoom in or zoom out) to
     *        display in xballs. The default is fit to screen
     */
    void setXBallsScale(Mdouble newScale);

    /*!
     * \brief Returns the scale of the view in xballs.
     */
    double getXBallsScale() const;

    /*!
     * \brief Allows to modify the gravity vector.
     */
    void setGravity(Vec3D newGravity);

    /*!
     * \brief Returns the gravity vector.
     */
    Vec3D getGravity() const;

    /*!
     * \brief Sets the system and particle dimension.
     */
    void setDimension(unsigned int newDim);

    /*!
     * \brief Allows for the dimension of the simulation to be changed.
     */
    void setSystemDimensions(unsigned int newDim);

    /*!
     * \brief Returns the dimension of the simulation. Note there is also a particle
     *        dimension.
     */
    unsigned int getSystemDimensions() const;

    /*!
    * \brief Allows the dimension of the particle (f.e. for mass) to be changed. 
     *       e.g. discs or spheres.
    */
    void setParticleDimensions(unsigned int particleDimensions);

    /*!
     * \brief Returns the particle dimensions.
     */
    unsigned int getParticleDimensions() const;

    /*!
     * \brief This is to take into account for different Mercury versions.
     *        Returns the version of the restart file.
     */
    std::string getRestartVersion() const;

    /*!
     * \brief Sets restart_version
     */
    void setRestartVersion(std::string newRV);

    /*!
     * \brief Returns the flag denoting if the simulation was restarted or not.
     */
    bool getRestarted() const;
    
    /*!
     * \brief Allows to set the flag stating if the simulation is to be restarted or not.
     */
    void setRestarted(bool newRestartedFlag);

    /*!
     * \brief Returns the flag denoting if the append option is on or off.
     */
    bool getAppend() const;

    /*!
     * \brief Allows to set the append option.
     */
    void setAppend(bool newAppendFlag);
    
    /*!
     * \brief Returns the global elastic energy within the system.
     */
    Mdouble getElasticEnergy() const;

    /*!
     * \brief Returns the global kinetic energy stored in the system.
     */
    Mdouble getKineticEnergy() const;
    
    /*!
     * \brief Checks if two particle are in contact or is there any positive overlap
     */
    bool areInContact(const BaseParticle* pI, const BaseParticle* pJ) const;

    /// \bug Why are the hGRID actions public, this seems wrong. Someone please comment [Ant].
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
    
    /*!
     * \brief
     */
    virtual void hGridUpdateMove(BaseParticle*, Mdouble);

    /*!
     * \brief
     * //Not unsigned index because of possible wall collisions.
     */
    virtual void gatherContactStatistics(unsigned int index1 UNUSED, int index2 UNUSED, Vec3D Contact UNUSED, Mdouble delta UNUSED, Mdouble ctheta UNUSED, Mdouble fdotn UNUSED, Mdouble fdott UNUSED, Vec3D P1_P2_normal_ UNUSED, Vec3D P1_P2_tangential UNUSED);    

//functions that should only be used in the class definitions 
protected:
    
    /*!
     * \brief Computes all the forces acting on the particles by using the setTorque
     * and setForce methods. See BaseInteractible.cc    
     */
    virtual void computeAllForces();

    /*!
     * \brief Computes the forces between particles (internal in the sense that the
     *        sum over all these forces is zero i.e. fully modelled forces)
     */
    virtual void computeInternalForces(BaseParticle* i);

    /*!
     * \brief Computes the forces between particles (internal in the sense that 
     *        the sum over all these forces is zero i.e. fully modelled forces)
     */
    virtual void computeInternalForces(BaseParticle* P1, BaseParticle* P2);

    /*!
     * \brief Computes the external forces acting on particles (e.g. gravitational)
     */
    virtual void computeExternalForces(BaseParticle* PI);

    /*!
     * \brief Computes the forces on the particles due to the walls (normals are outward normals)
     */
    virtual void computeForcesDueToWalls(BaseParticle* PI);
    
    /*!
     * \brief A virtual function where the users can add extra code which is executed
     *  only when the code is restarted.
     */
    virtual void actionsOnRestart();

    /*!
     * \brief A virtual function. Allows one to carry out any operations before the start
     *        of the time loop.
     */
    virtual void actionsBeforeTimeLoop();

    /*!
     * \brief A virtual function that allows one to carry out hGrid operations before the
     *       start of the time loop.
     */
    virtual void hGridActionsBeforeTimeLoop();

    /*!
     * \brief A virtual function that allows one to set or execute hGrid parameters or 
     *        operations before every simulation time step.
     */
    virtual void hGridActionsBeforeTimeStep();
    
    /*!
     * \brief 
     */
    virtual bool getHGridUpdateEachTimeStep() const;

    /*!
     * \brief A virtual function which allows to define operations to be executed before
     *        the new time step.
     */
    virtual void actionsBeforeTimeStep();

    /*!
     * \brief A virtual function which allows to define operations to be executed after
     *       the solve().
     */
    virtual void actionsAfterSolve();

    /*!
     * \brief A virtual function which allows to define operations to be executed after
     *       time step.
     */
    virtual void actionsAfterTimeStep();

    /*!
     * \brief This function writes the location of the walls and particles in a format the
     *        XBalls program can read. See XBalls/xballs.txt. However, MercuryDPM supports a much better viewer 
     *        now called Paraview. See the tutorials section in the documentation.
     */
    virtual void outputXBallsData(std::ostream& os) const;
    
    /*!
     * \brief This function writes out the particle locations into an output stream in a 
     *        format the XBalls program can read.
     */
    virtual void outputXBallsDataParticle(const unsigned int i,const unsigned int format, std::ostream& os) const;

    /*!
     * \brief Writes a header with a certain format for ENE file
     */
    virtual void writeEneHeader(std::ostream& os) const;

    /*!
     * \brief Writes a header with a certain format for FStat file
     */
    virtual void writeFstatHeader(std::ostream& os) const;

    /*!
     * \brief This function enables one to write the global energy available in the
     *        system after each time step. The default is to compute the rotational and 
     *        translational kinetic energy, potential energy and the centre of mass.
     */
    virtual void writeEneTimestep(std::ostream& os) const;
    
     // Functions for statistics
    /*!
     * 
     */
    virtual void initialiseStatistics();

    /*!
     * \brief
     */
    virtual void outputStatistics();

    /*!
     *
     */
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
     * \brief This is were the integration is done, at the moment it is
     *        velocity Verlet integration and is done before the forces are
     *        computed. See http://en.wikipedia.org/wiki/Verlet_integration#Velocity_Verlet
     */
    virtual void integrateBeforeForceComputation();

    /*!
     * \brief Integration is done after force computations. We apply the Velocity verlet scheme.
     *        See http://en.wikipedia.org/wiki/Verlet_integration#Velocity_Verlet
     */
    virtual void integrateAfterForceComputation();

    /*!
     * \brief There are a range of boundaries one could implement depending on ones' problem.
     *        This methods checks for interactions between particles and such range of boundaries.
     *        See BaseBoundary.h and all the boundaries in the Boundaries folder
     */
    virtual void checkInteractionWithBoundaries();

    /*!
     * \brief
     */
    virtual void hGridActionsBeforeIntegration();

    /*!
     * \brief
     */
    virtual void hGridActionsAfterIntegration();

    /*!
     * \brief By broad one means to screen and determine an approximate number of particles that a given 
     *        particle can be in contact with. Hence the word "Broad phase" of contact detection.
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
     * \brief Displays the current simulation time onto your screen for example.
     */
    virtual void printTime() const;

    /*!
     * \brief
     */
    virtual bool continueSolve() const;

    /*!
     * \brief Displays the interaction details corresponding to the pointer objects
     *        in the interaction handler.     
     */
    void outputInteractionDetails() const;

    /*!
     * \brief Checks if the input variable "time" is the current time in the simulation
     */
    bool isTimeEqualTo(Mdouble time) const;

    /*!
     * \brief Removes periodic duplicate Particles
     */
    void removeDuplicatePeriodicParticles();

    /*!
     * \brief In case of periodic boundaries, the below methods checks and adds particles
     *        when necessary into the particle handler. See DPMBase.cc and PeriodicBoundary.cc
     *        for more details.
     */
    void checkAndDuplicatePeriodicParticles();

private:
    
    /*!
     * \brief The dimensions of the simulation i.e. 2D or 3D
     */
    unsigned int systemDimensions_;

    /*!
     * \brief determines if 2D or 3D particle volume is used for mass calculations
     */
    unsigned int particleDimensions_;

    /*!
     * \brief Gravity vector
     */
    Vec3D gravity_;

    /*!
     * \brief If the length of the problem domain in x-direction is XMax - XMin, 
     *        the above variable stores XMin.
     */
    Mdouble xMin_;
    
    /*!
     * \brief If the length of the problem domain in x-direction is XMax - XMin,
     *        the above variable stores XMax.
     */
    Mdouble xMax_;
    
    /*!
     * \brief If the length of the problem domain in y-direction is YMax - YMin,
     *        the above variable stores YMin.
     */    
    Mdouble yMin_;
    
    /*!
     * \brief If the length of the problem domain in y-direction is YMax - XMin,
     *        the above variable stores YMax.
     */
    Mdouble yMax_;
    
    /*!
     * \brief If the length of the problem domain in z-direction is ZMax - ZMin,
     *        the above variable stores ZMin.
     */
    Mdouble zMin_;
    
    /*!
     * \brief If the length of the problem domain in z-direction is ZMax - ZMin,
     *        the above variable stores ZMax.
     */
    Mdouble zMax_;

    /*!
     * \brief Stores the current simulation time
     */
    Mdouble time_;

    /*!
     * \brief Stores the number of time steps
     */
    unsigned int ntimeSteps_;

    /*!
     * \brief Stores the simulation time step
     */
    Mdouble timeStep_;

    /*!
     * \brief Stores the duration of the simulation
     */
    Mdouble timeMax_;

    /*!
     * \brief used in force calculations
     * \details Stores the potential energy in the elastic springs (global, because
     *          it has to be calculated in the force loop
     */
    Mdouble elasticEnergy_;

    /*!
     * \brief Previous versions of MercuryDPM had a different restart file format, 
     *        the below member variable allows one to specify the version in order 
     *        to choose between the available version support.
     */
    std::string restartVersion_;

    /*!
     * \brief A bool to check if the simulation was restarted or not.
     */
    bool restarted_; 

    /*!
     * \brief A flag to determine if the file has to be appended or not. See DPMBase::Solve()
     *        for example.
     */
    bool append_;

    /*!
     * \brief A flag to turn on/off particle rotation.
     */
    bool rotation_;

    //This is the private data that is only used by the xballs output

    /*!
     * \brief XBalls is a package to view the particle data. As an alternative MercuryDPM also supports Paraview.
     * The below variable is used to set the argument cmode in xballs script (see XBalls/xballs.txt)
     */
    int xBallsColourMode_; 
    
    /*!
     * \brief sets the xballs argument vscale (see XBalls/xballs.txt)
     */
    Mdouble xBallsVectorScale_;

    /*!
     * \brief sets the xballs argument scale (see XBalls/xballs.txt)
     */
    Mdouble xBallsScale_;

    /*!
     * \brief A string of additional arguments for xballs can be specified (see XBalls/xballs.txt). e.g. "-solidf -v0"
     */
    std::string xBallsAdditionalArguments_; 

    //This belongs to the hGrid
    
    // defines a Macro for creating an instance of class PossibleContactList. See PossbileContactList.h     
    #ifdef CONTACT_LIST_HGRID
        PossibleContactList possibleContactList;
    #endif

public:

    /*!
     * \brief A handler to that stores the species type i.e. elastic, linear visco-elastic... et cetera.
     */
    SpeciesHandler speciesHandler;

    /*!
     * \brief This is a random generator, often used for setting up the initial conditions etc...
     */
    RNG random;

    /*!
     * \brief An object of the class ParticleHandler, contains the pointers to all the particles created.
     */
    ParticleHandler particleHandler;

    /*!
     * \brief An object of the class WallHandler. Contains pointers to all the walls created.
     */
    WallHandler wallHandler;

    /*!
     * \brief An object of the class BoundaryHandler which concerns insertion and deletion of particles into or from regions.
     */
    BoundaryHandler boundaryHandler;

    /*!
     * \brief An object of the class InteractionHandler
     */
    InteractionHandler interactionHandler;

};
#endif
