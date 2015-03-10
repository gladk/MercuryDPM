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

#ifndef BASEPARTICLE_H
#define BASEPARTICLE_H

#include <vector>
#include <iostream>
#include "Math/ExtendedMath.h"
#include "Math/Vector.h"
#include "BaseInteractable.h"
//#include "InteractionHandler.h"

class ParticleHandler;
class SpeciesHandler;
class HGrid;
class BaseInteraction;


#ifdef CONTACT_LIST_HGRID
class PossibleContact;
#endif
/*!
 * \class BaseParticle
 * \brief
 */
class BaseParticle : public BaseInteractable
{
public:
    /*!
     * \brief Basic Particle constructors, creates an Particle at (0,0,0) with radius, mass and inertia equal to 1
     */
    BaseParticle();

    /*!
     * \brief Particle copy constructor, which accepts as input a reference to a Particle. It creates a copy of this Particle and all it's information. Usually it is better to use the copy() function for polymorfism.
     */
    BaseParticle(const BaseParticle &p);

    /*!
     * \brief Particle destructor, needs to be implemented and checked if it removes tangential spring information
     */
    virtual ~BaseParticle();

    /*!
     * \brief Particle copy method. It calls to copy constructor of this Particle, useful for polymorfism
     */
    virtual BaseParticle* copy() const;

    /*!
     * \brief Get Particle volume function, which required a reference to the Species vector. It returns the volume of the Particle.
     */
    Mdouble getVolume() const;

    /*!
     * \brief Fix Particle function. It fixes a Particle by setting its inverse mass and inertia and velocities to zero
     */
    void fixParticle();

    /*!
     * \brief Is fixed Particle function. It returns whether a Particle is fixed or not, by checking its inverse Mass.
     */
    bool isFixed() const;

    /*!
     * \brief Unfix Particle function, which required a reference to the Species vector. It unfixes a Particle by computing the Particles mass and inertia
     */
    void unfix();

    /*!
     * \brief Compute Particle mass function, which required a reference to the Species vector. It computes the Particles mass, Inertia and the inverses.
     */
    void computeMass();

    /*!
     * \brief Particle read function, which accepts an std::stringstream as input.
     */
    virtual void read(std::istream& is);
    
    /*!
     * \brief 
     */
    virtual void oldRead(std::istream& is);
    
    /*!
     * \brief Particle print function, which accepts an std::stringstream as input.
     */
    virtual void write(std::ostream& os) const;
    
    /*!
     * \brief Returns the name of the object
     */
    virtual std::string getName() const;

    /*!
     * \brief 
     */
    void printHGrid(std::ostream& os) const;

    /*!
     * \brief 
     */
    unsigned int getHGridLevel() const;

    /*!
     * \brief 
     */
    BaseParticle* getHGridNextObject() const;

    /*!
     * \brief 
     */
    BaseParticle* getHGridPrevObject() const;

    #ifdef CONTACT_LIST_HGRID

    /*!
     * \brief 
     */
    PossibleContact* getFirstPossibleContact() const;
    #endif

    /*!
     * \brief 
     */
    int getHGridX() const;

    /*!
     * \brief 
     */
    int getHGridY() const;

    /*!
     * \brief 
     */
    int getHGridZ() const;

    /*!
     * \brief 
     */
    Mdouble getInertia() const;

    /*!
     * \brief 
     */
    Mdouble getInvInertia() const;

    /*!
     * \brief 
     */
    Mdouble getInvMass() const;

    /*!
     * \brief 
     */
    Mdouble getKineticEnergy() const;

    /*!
     * \brief 
     */
    Mdouble getMass() const;

    /*!
     * \brief 
     */
    BaseParticle* getPeriodicFromParticle() const;

    /*!
     * \brief 
     */
    Mdouble getRadius() const;

    /*!
     * \brief 
     */
    Mdouble getInteractionRadius() const;

    /*!
     * \brief 
     */
    Mdouble getWallInteractionRadius() const;

    /*!
     * \brief 
     */
    const Vec3D& getDisplacement() const;

    /*!
     * \brief 
     */
    const Vec3D& getPreviousPosition() const;

    /*!
     * \brief 
     */
    const Vec3D getDisplacement2(Mdouble xmin, Mdouble xmax, Mdouble ymin, Mdouble ymax, Mdouble zmin, Mdouble zmax, Mdouble t) const;

    /*!
     * \brief 
     */
    void setInertia(const Mdouble _new);

    /*!
     * \brief 
     */
    void setInfiniteInertia();

    /*!
     * \brief 
     */
    void setPeriodicFromParticle(BaseParticle* _new);

    /*!
     * \brief 
     */
    void setHGridX(const int _new);

    /*!
     * \brief 
     */
    void setHGridY(const int _new);

    /*!
     * \brief 
     */
    void setHGridZ(const int _new);

    /*!
     * \brief 
     */
    void setHGridLevel(const unsigned int _new);

    /*!
     * \brief 
     */
    void setHGridNextObject(BaseParticle* _new);

    /*!
     * \brief 
     */
    void setHGridPrevObject(BaseParticle* _new);

    #ifdef CONTACT_LIST_HGRID
    /*!
     * \brief 
     */
    void setFirstPossibleContact (PossibleContact* PC);
    #endif

    /*!
     * \brief 
     */
    void setRadius(const Mdouble _new);

    /*!
     * \brief 
     */
    void setMass(const Mdouble _new);


    /*!
     * \brief 
     */
    void setDisplacement(const Vec3D& _new);

    /*!
     * \brief 
     */
    void setPreviousPosition(const Vec3D& _new);

    /*!
     * \brief 
     */
    void movePrevious(const Vec3D& _new);

    /*!
     * \brief 
     */
    void accelerate(const Vec3D& _new);

    /*!
     * \brief 
     */
    void angularAccelerate(const Vec3D& _new);

    /*!
     * \brief 
     */
    void addDisplacement(const Vec3D& _new);

    /*!
     * \brief 
     */
    void setHandler(ParticleHandler* handler);

    /*!
     * \brief 
     */
    ParticleHandler* getHandler() const;

    /*!
     * \brief 
     */
    BaseInteraction* getInteractionWith(BaseParticle *P, Mdouble timeStamp, InteractionHandler* interactionHandler);

    /*!
     * \brief 
     */
    void integrateBeforeForceComputation(double timeStep);

    /*!
     * \brief 
     */
    void integrateAfterForceComputation(double timeStep);

    /*!
     * \brief
     */
    unsigned int getParticleDimensions() const;

    /*!
     * \brief
     */
     void setIndSpecies(unsigned int indSpecies);

private:

    /*!
     * \brief 
     */
    ParticleHandler* handler_;

#ifdef CONTACT_LIST_HGRID
    /*!
     * \brief 
     */
    PossibleContact* firstPossibleContact;
#endif

    ///Hgrid attributes
    int HGridX_, HGridY_, HGridZ_; ///Cell position in the grid
    unsigned int HGridLevel_; ///Grid level for the object
    BaseParticle* HGridNextObject_; ///Pointer to the next Particle in the same HGrid cell
    BaseParticle* HGridPrevObject_; ///Pointer to the previous Particle in the same HGrid cell
    
    ///Particle attributes
    Mdouble mass_; ///Particle mass_
    Mdouble invMass_; ///Inverse Particle mass (for computation optimization)
    Mdouble inertia_; ///Particle inertia_
    Mdouble invInertia_; ///Inverse Particle inverse inertia (for computation optimization)
    Mdouble radius_; ///Particle radius_ 
    BaseParticle * periodicFromParticle_; ///Pointer to originating Particle

    Vec3D displacement_; ///Displacement (only used in StatisticsVector, StatisticsPoint)
    Vec3D previousPosition_;///
};
#endif
