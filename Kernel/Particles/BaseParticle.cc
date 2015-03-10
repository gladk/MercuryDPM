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

#include <Species/LinearViscoelasticSpecies.h>
#include "InteractionHandler.h"
#include "Particles/BaseParticle.h"
#include "Interactions/BaseInteraction.h"
#include "Species/ParticleSpecies.h"
#include "ParticleHandler.h"
#include "DPMBase.h"

/*!
 * \details default constructor, creates an Particle at (0,0,0) with radius, 
 * mass and inertia equal to 1
 */
BaseParticle::BaseParticle()
{
    handler_ = 0;
    displacement_.setZero();
    radius_ = 1.0;
    mass_ = invMass_ = 1.0;
    inertia_ = invInertia_ = 1.0;
    HGridNextObject_ = nullptr;
    
    periodicFromParticle_ = nullptr;
#ifdef CONTACT_LIST_HGRID
    firstPossibleContact=0;
#endif
    HGridNextObject_ = 0;
    HGridPrevObject_ = 0;
    HGridLevel_ = 99999;
    HGridX_ = 99999;
    HGridY_ = 99999;
    HGridZ_ = 99999;
    
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "BaseParticle::BaseParticle() finished"<<std::endl;
#endif        
}

/*!
 * \details copy constructor.
 * Consider using the copy() function for polymorphism.
 * \param[in,out] p  Reference to the BaseParticle this one should become a copy of.
 */
BaseParticle::BaseParticle(const BaseParticle &p)
        : BaseInteractable(p)
{
    handler_ = nullptr;
    displacement_ = p.displacement_;
    radius_ = p.radius_;
    mass_ = p.getMass();
    invMass_ = p.getInvMass();
    inertia_ = p.getInertia();
    invInertia_ = p.getInvInertia();
    
    HGridNextObject_ = 0;
    HGridPrevObject_ = 0;
    HGridX_ = 99999;
    HGridY_ = 99999;
    HGridZ_ = 99999;
    HGridLevel_ = p.HGridLevel_;
    
    periodicFromParticle_ = p.periodicFromParticle_;
#ifdef CONTACT_LIST_HGRID
    firstPossibleContact=0;
#endif
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "BaseParticle::BaseParticle(BaseParticle &p) finished"<<std::endl;
#endif            
}

/*!
 * \details Destructor
 */
BaseParticle::~BaseParticle()
{
    if (getHandler())
    {
        getHandler()->checkExtremaOnDelete(this);
    }
#ifdef DEBUG_DESTRUCTOR
    std::cerr << "BaseParticle::~BaseParticle() finished"<<std::endl;
#endif    
}

/*!
 * \details Copy method. Uses copy constructor to create a copy on the heap. 
 * Useful for polymorphism.
 * \return pointer to the particle's copy
 */
BaseParticle* BaseParticle::copy() const
{
    return new BaseParticle(*this);
}

/*!
 * \details Returns the volume of the BaseParticle, which is calculated using
 * its number of dimensions and radius.
 * \return the actual volume
 */
Mdouble BaseParticle::getVolume() const
{
    if (handler_ == nullptr)
    {
        logger(ERROR,"[BaseParticle::getVolume] no particle handler specified");
        return 0;
    }
    switch (getParticleDimensions())
    {
        case 3:
            return (4.0 / 3.0 * constants::pi * radius_ * radius_ * radius_);
        case 2:
            return (constants::pi * radius_ * radius_);
        case 1:
            return (2.0 * radius_);
        default:
            logger(ERROR, "[BaseParticle::getVolume] dimension of the particle is not set");
            return 0;
    }
}

/*!
 * \details Fixes a BaseParticle by setting its inverse mass and inertia and velocities to zero.
 */
void BaseParticle::fixParticle()
{
    mass_ = 1e20;
    invMass_ = 0.0;
    inertia_ = 1e20;
    invInertia_ = 0.0;
    setVelocity(Vec3D(0.0,0.0,0.0));
    setAngularVelocity(Vec3D(0.0,0.0,0.0));
}

/*!
 * \details Checks whether a BaseParticle is fixed or not, by checking its inverse Mass.
 * \return TRUE if particle is fixed, i.e. if the inverse mass (invMass_) is 0.
 */
bool BaseParticle::isFixed() const
{
    return (invMass_ == 0.0);
}

/*!
 * \details Unfixes the particle by computing the Particles mass and inertia, using the 
 * species and radius.
 */
 
void BaseParticle::unfix()
{
    invMass_ = 1.0;
    getSpecies()->computeMass(this);
}

/*!
 * \details BaseParticle print method, which accepts an os std::ostream as 
 * input. It prints human readable BaseParticle information to the std::ostream
 * \param[out] os    stream to which the info is written
 */
void BaseParticle::write(std::ostream& os) const
        {
    BaseInteractable::write(os);
    os << " radius " << radius_
            << " invMass " << invMass_
            << " invInertia " << invInertia_;
}

/*!
 * \details Returns the name of the object; in this case 'BaseParticle'.
 * \return the object name.
 */
std::string BaseParticle::getName() const
{
    return "BaseParticle";
}

/*!
 * \details Particle read function. Has an std::istream as argument, from which 
 * it extracts the radius_, invMass_ and invInertia_, respectively. From these 
 * the mass_ and inertia_ are deduced. An additional set of properties is read 
 * through the call to the parent's method BaseInteractable::read().
 * \param[in,out] is    istream with particle properties.
 */
void BaseParticle::read(std::istream& is)
{
    BaseInteractable::read(is);
    std::string dummy;
    is >> dummy >> radius_ >> dummy >> invMass_ >> dummy >> invInertia_;
    if (invMass_ != 0.0)
        mass_ = 1.0 / invMass_;
    else
        mass_ = 1e20;
    if (invInertia_ != 0.0)
        inertia_ = 1.0 / invInertia_;
    else
        inertia_ = 1e20;
}

/*!
 * \details This is the previously used version of the read function. Now just kept
 * for legacy purposes. 
 * \deprecated Should be gone in Mercury 2.0. Use BaseParticle::read() instead.
 */
void BaseParticle::oldRead(std::istream& is)
{
    ///todo{fix this function}

    unsigned int indSpecies;
    Vec3D orientation;
    Vec3D position;
    is >> invMass_ >> invInertia_ >> indSpecies;
    setPosition(position);
    setOrientation(orientation);
    setIndSpecies(indSpecies);
    if (invMass_ != 0.0)
        mass_ = 1.0 / invMass_;
    else
        mass_ = 1e20;
    if (invInertia_ != 0.0)
        inertia_ = 1.0 / invInertia_;
    else
        inertia_ = 1e20;
}

/*!
 * \details Adds the particle's HGridLevel_ and HGRid x/y/z positions to an 
 * std::ostream.
 * \param[out] os    the ostream which has the mentioned properties added.
 */
void BaseParticle::printHGrid(std::ostream& os) const
        {
    os << "Particle( HGRID_Level:" << HGridLevel_
            << ", HGRID_x:" << HGridX_
            << ", HGRID_y:" << HGridY_
            << ", HGRID_z:" << HGridZ_
            << ")";
}

/*!
 * \details Returns the particle's HGridLevel_
 * \return the HGridLevel_
 */
unsigned int BaseParticle::getHGridLevel() const
{
    return HGridLevel_;
}

/*!
 * \details Returns the next object in the particle's HGrid cell
 * \return pointer to the next object in the particle's HGrid cell
 */
BaseParticle* BaseParticle::getHGridNextObject() const
{
    return HGridNextObject_;
}

/*!
 * \details Returns the previous object in the particle's HGrid cell
 * \return pointer to the previous object in the particle's HGrid
 * cell
 */
BaseParticle* BaseParticle::getHGridPrevObject() const
{
    return HGridPrevObject_;
}

/*!
 * \details 
 */
#ifdef CONTACT_LIST_HGRID
PossibleContact* BaseParticle::getFirstPossibleContact() const
{   
    return firstPossibleContact;
}
#endif

/*!
 * \details Get the particle's HGrid cell's X-coordinate
 * \return the particle's HGrid cell's X-coordinate
 */
int BaseParticle::getHGridX() const
{
    return HGridX_;
}

/*!
 * \details Get the particle's HGrid cell's Y-coordinate
 * \return the particle's HGrid cell's Y-coordinate
 */
int BaseParticle::getHGridY() const
{
    return HGridY_;
}

/*!
 * \details Get the particle's HGrid cell's Z-coordinate
 * \return the particle's HGrid cell's Z-coordinate
 */
int BaseParticle::getHGridZ() const
{
    return HGridZ_;
}

/*!
 * \details Returns the particle's inertia_
 * \return the particle's inertia_
 */
Mdouble BaseParticle::getInertia() const
{
    return inertia_;
}

/*!
 * \details Returns the particle's inverse inertia_
 * \return the particles invInertia_
 */
Mdouble BaseParticle::getInvInertia() const
{
    return invInertia_;
}

/*!
 * \details Returns the particle's inverse mass_
 * \return the particle's invMass_
 */
Mdouble BaseParticle::getInvMass() const
{
    return invMass_;
}

/*!
 * \details Calculates the particle's kinetic energy
 * \return the particle's kinetic energy
 */
Mdouble BaseParticle::getKineticEnergy() const
{
    return 0.5 * mass_ * getVelocity().getLengthSquared();
}

/*!
 * \details Returns the mass_ of the particle
 * \return the mass_ of the particle
 */
Mdouble BaseParticle::getMass() const
{
    return mass_;
}

/*!
 * \details Returns a pointer to the 'original' particle if the current one is
 * a 'periodic copy' used for a periodic boundary implementation.
 * \return pointer to original particle
 */
BaseParticle* BaseParticle::getPeriodicFromParticle() const
{
    return periodicFromParticle_;
}

/*!
 * \details Returns the particle's radius
 * \return particle's radius
 */
Mdouble BaseParticle::getRadius() const
{
    return radius_;
}

/*!
 * \details Calculates the interaction radius of the particle (when it comes to 
 * interaction with other particles), including the effect of a possible additional
 * 'interaction distance' besides the 'normal' radius. The ineraction radius 
 * differs from the radius_, for example, when dealing with wet particles (i.e. 
 * particles with an additional liquid layer, which is dealt with in the particle's 
 * species).
 * \return the particle's interaction radius for particle-particle interaction
 */
Mdouble BaseParticle::getInteractionRadius() const
{
    return radius_ + getSpecies()->getInteractionDistance() * 0.5;
}

/*!
 * \details The interaction radius of the particle (when it comes to interaction
 * with walls). See also BaseParticle::getInteractionRadius().
 * \return the particle's interaction radius for particle-wall interaction
 */
Mdouble BaseParticle::getWallInteractionRadius() const
{
    return radius_ + getSpecies()->getInteractionDistance();
}

/*!
 * \details Returns the particle's displacement_, which is the difference between
 * the current particle's position and its position in the previous time step.
 * \return (reference to) the particle displacement vector
 */
const Vec3D& BaseParticle::getDisplacement() const
{
    return displacement_;
}

/*!
 * \details Returns the particle's position in the previous time step.
 * \return (reference to) the previous position of the particle
 */
const Vec3D& BaseParticle::getPreviousPosition() const
{
    return previousPosition_;
}

/*!
 * \todo Rewrite, redefine (TW). Is only used in StatisticsVector.hcc, consider 
 * moving to that class.
 */ 
const Vec3D BaseParticle::getDisplacement2(Mdouble xmin, Mdouble xmax, Mdouble ymin, Mdouble ymax, Mdouble zmin, Mdouble zmax, Mdouble t) const
        {
    Vec3D disp = getPosition() - getPreviousPosition();
    if (xmax > xmin && fabs(disp.X) > .5 * (xmax - xmin))
    {
        if (disp.X > 0)
            disp.X -= xmax - xmin;
        else
            disp.X += xmax - xmin;
    }
    if (ymax > ymin && fabs(disp.Y) > .5 * (ymax - ymin))
    {
        if (disp.Y > 0)
            disp.Y -= ymax - ymin;
        else
            disp.Y += ymax - ymin;
    }
    if (zmax > zmin && fabs(disp.Z) > .5 * (zmax - zmin))
    {
        if (disp.Z > 0)
            disp.Z -= zmax - zmin;
        else
            disp.Z += zmax - zmin;
    }
    disp /= t;
    return disp;
}

/*!
 * \details Sets the particle's inertia_ and invInertia_. 
 * \param[in] newInertia  the new inertia_ to be set.
 */
void BaseParticle::setInertia(const Mdouble newInertia)
{
    if (newInertia >= 0)
    {
        inertia_ = newInertia;
        invInertia_ = 1.0 / newInertia;
    }
    else
    {
        logger(ERROR, "Error in set_inertia (%)", newInertia);
    }
}

/*!
 * \details Sets the inertia_ to 1e20 and the invInertia_ (which is actually
 * used in the calculations) to 0.
 */
void BaseParticle::setInfiniteInertia()
{
    inertia_ = 1e20;
    invInertia_ = 0;
} //> i.e. no rotations

/*!
 * \details Lets you set which particle this one is actually a periodic copy of
 * (used in periodic boundary condition implementations). 
 * \param[in] p  pointer to the 'original' particle this one is a periodic 
 * copy of.
 */
void BaseParticle::setPeriodicFromParticle(BaseParticle* p)
{
    periodicFromParticle_ = p;
}

/*!
 * \details Set the x-index of the particle's HGrid cell position
 * \param[in] x  x-index of particle's HGrid cell
 */
void BaseParticle::setHGridX(const int x)
{
    HGridX_ = x;
}

/*!
 * \details Set the y-index of the particle's HGrid cell position
 * \param[in] y  y-index of particle's HGrid cell
 */
void BaseParticle::setHGridY(const int y)
{
    HGridY_ = y;
}

/*!
 * \details Set the z-index of the particle's HGrid cell position
 * \param[in] z  z-index of particle's HGrid cell
 */
void BaseParticle::setHGridZ(const int z)
{
    HGridZ_ = z;
}

/*!
 * \details Sets the particle's HGridLevel_
 * \param[in] level  the particle's HGridLevel_
 */
void BaseParticle::setHGridLevel(const unsigned int level)
{
    HGridLevel_ = level;
}

/*!
 * \details Sets the pointer to the next object in the particle's HGrid cell
 * \param[in] p     pointer to the next object
 */
void BaseParticle::setHGridNextObject(BaseParticle* p)
{
    HGridNextObject_ = p;
}

/*!
 * \details Sets the pointer to the previous object in the particle's HGrid cell
 * \param[in] p     pointer to the previous object
 */
void BaseParticle::setHGridPrevObject(BaseParticle* p)
{
    HGridPrevObject_ = p;
}

/*!
 * \details 
 */
#ifdef CONTACT_LIST_HGRID
void BaseParticle::setFirstPossibleContact (PossibleContact* PC)
{
    firstPossibleContact=PC;
}
#endif

/*!
 * \details Sets the radius of the particle, and from that computes the new mass
 * (using its species) and checks whether it now is either the smallest or biggest 
 * particle in its ParticleHandler. 
 * \param[in] radius    the new radius
 */
void BaseParticle::setRadius(const Mdouble radius)
{
    radius_ = radius;
    if (getHandler())
    {
        getSpecies()->computeMass(this);
        getHandler()->checkExtrema(this);
    }
}

/*!
 * \details Sets the mass_ of the particle
 * \param[in] mass  the new particle's  mass
 * \todo (AT) The mass could theoretically now be set without resetting the radius
 * to match the species' density. The BaseParticle::setRadius() function DOES reset
 * the mass to match the density. I'd say both should implement the adaptation of
 * the other, with a match CHECK implemented in at least one of them (to prevent 
 * an infinite loop).
 */
void BaseParticle::setMass(const Mdouble mass)
{
    if (mass >= 0.0)
    {
        if (invMass_ != 0.0)
        {
            mass_ = mass;
            invMass_ = 1.0 / mass;
        }
    }
    else
    {
        std::cerr << "Error in set_Mass(" << mass << ")" << std::endl;
        exit(-1);
    }
} //InvMass=0 is a flag for a fixed particle

/*!
 * \details This is used to set the particle displacement_ 
 * \param[in] disp  the displacement vector
 */
void BaseParticle::setDisplacement(const Vec3D& disp)
{
    displacement_ = disp;
}

/*!
 * \details This is used to set the particle's previous position
 * \param[in] pos   the particle's previous position vector.
 */
void BaseParticle::setPreviousPosition(const Vec3D& pos)
{
    previousPosition_ = pos;
}

/*!
 * \details Lets you add a vector to the particle's previousPosition_ vector.
 * \param[in] posMove   the vector to be added to the current previousPosition_
 * vector.
 */
void BaseParticle::movePrevious(const Vec3D& posMove)
{
    previousPosition_ += posMove;
}

/*!
 * \details increases the the particle's velocity_ (BaseInteractable member) 
 * by adding the given vector.
 * \param[in] vel   vector to be added to the velocity_
 */
void BaseParticle::accelerate(const Vec3D& vel)
{
    addVelocity(vel);
}

/*!
 * \details increases the particle's angularVelocity_ (BaseInteractable member)
 * by adding the given vector.
 * \param[in] angVel    vector to be added to the angularVelocity_
 */
void BaseParticle::angularAccelerate(const Vec3D& angVel)
{
    addAngularVelocity(angVel);
}

/*!
 * \details Lets you add a vector to the particle's displacement_ vector.
 * \param[in] addDisp   vector to be added.
 */
void BaseParticle::addDisplacement(const Vec3D& addDisp)
{
    displacement_ += addDisp;
}

/*!
 * \details Assigns the particle to a ParticleHandler, and assigns a species to 
 * it based on the particles indSpecies_ (BaseInteractable data member).
 * \param[in] handler   pointer to the ParticleHandler
 */
void BaseParticle::setHandler(ParticleHandler* handler)
{
    handler_ = handler;
    setSpecies(getHandler()->getDPMBase()->speciesHandler.getObject(getIndSpecies()));
}

/*!
 * \details Returns the particle's ParticleHandler
 * \return pointer to the particle's ParticleHandler
 */
ParticleHandler* BaseParticle::getHandler() const
{
    return handler_;
}

/*!
 * \details Creates/updates a BaseInteraction object, treating the interaction between
 * this particle and a given one, in case there is an overlap between the two.
 * \param[in] P             particle to check the interaction with
 * \param[in] timeStamp     time stamp to be assigned to the interaction object (i.e., 
 * the current time) 
 * \param[in,out] interactionHandler    BaseInteraction container from where the 
 * interaction is retrieved, and to which it is assigned (if it is a new interaction). 
 * \return the pointer to the interaction object (if the particles overlap), or 0 
 * (if they don't overlap).
 */
BaseInteraction* BaseParticle::getInteractionWith(BaseParticle *P, Mdouble timeStamp, InteractionHandler* interactionHandler)
{
    //get the normal (from P away from the contact)
    Vec3D branchVector = P->getPosition() - getPosition();
    //Get the square of the distance between particle i and particle j
    Mdouble distanceSquared = Vec3D::getLengthSquared(branchVector);
    Mdouble sumOfInteractionRadii = P->getInteractionRadius() + getInteractionRadius();
    if (distanceSquared < (sumOfInteractionRadii * sumOfInteractionRadii))
    {
        BaseInteraction* C = interactionHandler->getInteraction(P, this, timeStamp);
        Mdouble distance = std::sqrt(distanceSquared);
        C->setNormal(branchVector / distance);
        C->setOverlap(P->getRadius() + getRadius() - distance);
        C->setDistance(distance);
        ///\todo We should consider setting the contact point to
        ///r=P->getRadius()/(getRadius+P->getRadius())
        ///P->getPosition() - (P->getRadius() - ratio * C->getOverlap()) * C->getNormal()
        ///\author weinhartt
        C->setContactPoint(P->getPosition() - (P->getRadius() - 0.5 * C->getOverlap()) * C->getNormal());
        return C;
    }
    else
    {
        return 0;
    }
}

/*!
 * \details First step of Velocity Verlet integration (see also 
 * http://en.wikipedia.org/wiki/Verlet_integration#Velocity_Verlet).
 * \param[in] time          current time
 * \param[in] timeStep      current time step
 */
void BaseParticle::integrateBeforeForceComputation(double time, double timeStep)
{
    if (getInvMass() == 0.0)
    {
        BaseInteractable::integrateBeforeForceComputation(time, timeStep);
    }
    else
    {
        accelerate(getForce() * getInvMass() * 0.5 * timeStep);
        setDisplacement(getVelocity() * timeStep);
        move(getDisplacement());
        getHandler()->getDPMBase()->hGridUpdateMove(this, getDisplacement().getLength());
        if (getHandler()->getDPMBase()->getRotation())
        {
            angularAccelerate(getTorque() * getInvInertia() * 0.5 * timeStep);
            rotate(getAngularVelocity() * timeStep);
        }
    }
}

/*!
 * \details Second step of Velocity Verlet integration (see also 
 * http://en.wikipedia.org/wiki/Verlet_integration#Velocity_Verlet).
 * \param[in] time      current time
 * \param[in] timeStep  current time step
 */
void BaseParticle::integrateAfterForceComputation(double time, double timeStep)
{
    if (getInvMass() == 0.0)
    {
        BaseInteractable::integrateAfterForceComputation(time, timeStep);
    }
    else
    {
        accelerate(getForce() * getInvMass() * 0.5 * timeStep);
        if (getHandler()->getDPMBase()->getRotation())
        {
            angularAccelerate(getTorque() * getInvInertia() * 0.5 * timeStep);
        }
    }
}

/*!
 * \details Returns the amount of dimensions of the particle (2 or 3, basically)
 * \return the number of dimension of the particle
 */
unsigned int BaseParticle::getParticleDimensions() const
{
    return getHandler()->getDPMBase()->getParticleDimensions();
}

/*!
 * \details Set the particle's species and species' index. Logs a warning if
 * no ParticleHandler is assigned. 
 * \param[in] indSpecies    the species' index
 */
void BaseParticle::setIndSpecies(unsigned int indSpecies)
{
    if (handler_ != nullptr)
    {
        setSpecies(getHandler()->getDPMBase()->speciesHandler.getObject(indSpecies));
        ///\todo TW do we have to update the species stored in the interactions here?
    }
    else
    {
        BaseInteractable::setIndSpecies(indSpecies);
        logger(WARN, "setIndSpecies called on a particle with no particle handler.\n"
                     "Therefore I can't request the given species from the species handler.\n"
                     " PartID = %", getId());
    }
}

/*!
 * \details Sets the particle's species
 * \param[in] species   pointer to the ParticleSpecies object, to be set as the 
 * particle's species.
 */
void BaseParticle::setSpecies(const ParticleSpecies* species)
{
    BaseInteractable::setSpecies(species);

    //set pointer to particle Handler, which is needed to retrieve species information
    if (getHandler()== nullptr)
    {
        SpeciesHandler* sH = species->getHandler();
        if (sH!= nullptr)
        {
            DPMBase* dB = sH->getDPMBase();
            if (dB!= nullptr)
            {
                setHandler(&dB->particleHandler);
            }
        }
    }
}
