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
#include "../ExtendedMath.h"
#include "../Vector.h"

class ParticleHandler;
class HGrid;
class CSpecies;

#ifdef ContactListHgrid
    class PossibleContact;
#endif

class BaseParticle
{
public:
    ///Basic Particle contructors, creates an Particle at (0,0,0) with radius, mass and inertia equal to 1
    BaseParticle();
    
    ///Particle copy contstructor, which accepts as input a reference to a Particle. It creates a copy of this Particle and all it's information. Usually it is better to use the copy() function for polymorfism.
    BaseParticle(const BaseParticle &p);
    
    ///Particle destructor, needs to be implemented and checked if it removes tangential spring information
    virtual ~BaseParticle();
    
    ///Particle copy method. It calls to copy contrustor of this Particle, usefull for polymorfism
    virtual BaseParticle* copy() const;

    ///Get Particle volume function, which required a reference to the Species vector. It returns the volume of the Particle.
    Mdouble get_Volume(std::vector<CSpecies>& Species) const;
    
    ///Fix Particle function. It fixes a Particle by setting its inverse mass and inertia and velocities to zero
    void fixParticle();
    
    ///Is fixed Particle function. It returns wether a Particle is fixed or not, by cheking its inverse Mass.
    bool isFixed();
    
    ///Unfix Particle function, which required a reference to the Species vector. It un fixes a Particle by compyting the Particles mass and inertia
    void unfix(std::vector<CSpecies>& Species);
    
    ///Compute Particle mass function, which required a reference to the Species vector. It copmuters the Particles mass, Inertia and the inverses.
    void compute_particle_mass(std::vector<CSpecies>& Species);

 	///writes wall
	friend inline std::ostream& operator<<(std::ostream& os, const BaseParticle &b);
	
	///reads wall
	friend inline std::istream& operator>>(std::istream& is, BaseParticle &b);
	
    ///Particle read function, which accepts an os std::stringstream as input.
    virtual void read(std::istream& is);
    
    ///Particle print function, which accepts an os std::stringstream as input.
    virtual void print(std::ostream& os) const;
    
    virtual void moveInHandler(int newPos);
    
    void print_HGRID(std::ostream& os);
    
    int              get_HGRID_Level() const;
    BaseParticle*        get_HGRID_NextObject() const;
    BaseParticle*        get_HGRID_PrevObject() const;
#ifdef ContactListHgrid
    PossibleContact* get_FirstPossibleContact() const;
#endif
    int              get_HGRID_x() const;
    int              get_HGRID_y() const;
    int              get_HGRID_z() const;
    int              get_Index()     const;
    Mdouble          get_Inertia() const;
    Mdouble          get_InvInertia() const;
    Mdouble          get_InvMass() const;
    Mdouble          get_KineticEnergy() const;
    Mdouble          get_Mass() const;
    BaseParticle*    get_PeriodicFromParticle() const;
    const Vec3D&     get_Position() const;
    Mdouble          get_Radius() const;
    Mdouble          get_InteractionRadius() const;
    Mdouble          get_WallInteractionRadius() const;
    int              get_Species() const;
    int              get_IndSpecies() const;
    int              get_Id() const;
    const Vec3D&     get_Velocity() const;
    const Vec3D&     get_Angle() const;
    const Vec3D&     get_AngularVelocity() const; 
    const Vec3D&     get_Force() const;
    const Vec3D&     get_Torque() const;
    const Vec3D&     get_Displacement() const;
    const Vec3D&     get_PreviousPosition() const;
    ParticleHandler* getHandler() const;
    const Vec3D      get_Displacement2 (Mdouble xmin, Mdouble xmax, Mdouble ymin, Mdouble ymax, Mdouble zmin, Mdouble zmax, Mdouble t) const;
    
    void set_inertia                    (const Mdouble _new);
    void set_infiniteInertia            ();
    void set_periodicFromParticle       (BaseParticle* _new);
    void set_species                    (const int _new);
    void set_Index                      (const int _new);
    void set_HGRID_x                    (const int _new);
    void set_HGRID_y                    (const int _new);
    void set_HGRID_z                    (const int _new);
    void set_HGRID_Level                (const int _new);
    void set_HGRID_NextObject           (BaseParticle* _new);
    void set_HGRID_PrevObject           (BaseParticle* _new);
#ifdef ContactListHgrid
    void set_FirstPossibleContact       (const PossibleContact* PC);
#endif
    void set_Radius                     (const Mdouble _new);
    void set_IndSpecies                 (const int _new);
    void set_Id                         (const int _new);
    void set_Mass                       (const Mdouble _new);
    void set_Velocity                   (const Vec3D& _new);
    void set_Position                   (const Vec3D& _new);
    void set_Displacement               (const Vec3D& _new);
    void set_PreviousPosition           (const Vec3D& _new);
    void set_Angle                      (const Vec3D& _new);
    void set_AngularVelocity            (const Vec3D& _new);
    void set_Force                      (const Vec3D& _new);
    void set_Torque                     (const Vec3D& _new);
    
    void setHandler                    (ParticleHandler* handler);
    
    void move                           (const Vec3D& _new);
    void movePrevious                   (const Vec3D& _new);
    void accelerate                     (const Vec3D& _new);
    void rotate                         (const Vec3D& _new);
    void angularAccelerate              (const Vec3D& _new);
    void add_Displacement               (const Vec3D& _new);
    void add_Force                      (const Vec3D& _new);
    void add_Torque                     (const Vec3D& _new);

#ifdef USE_SIMPLE_VERLET_INTEGRATION
    void set_PrevPosition               (const Vec3D& _new);
    void prevMove                       (const Vec3D& _new);
    const Vec3D&    get_PrevPosition();
#endif
    
private:
#ifdef ContactListHgrid
    PossibleContact* firstPossibleContact;
#endif

    ParticleHandler* handler;
    //HGrid* hGrid;

    ///Hgrid attributes
    int HGRID_x, HGRID_y, HGRID_z;              ///Cell position in the grid
    int HGRID_Level;                            ///Grid level for the object
    BaseParticle* HGRID_NextObject;                 ///Pointer to the next Particle in the same HGrid cell
    BaseParticle* HGRID_PrevObject;                 ///Pointer to the previous Particle in the same HGrid cell
    
    ///Particle attributes
    int _index;                                  ///Index of the Particle in the ParticleHandler, used for outputting fstat data
    Mdouble mass;                               ///Particle mass
    Mdouble invMass;                            ///Inverse Particle mass (for computation optimization)
    Mdouble inertia;                            ///Particle inertia
    Mdouble invInertia;                         ///Inverse Particle inverse inertia (for computation optimization)
    Mdouble radius;                             ///Particle radius 
    BaseParticle * periodicFromParticle;        ///Pointer to originating Particle
    int indSpecies;                             ///Index of the Species of this Particle
    int _id;                                     ///non changing identifier of particle
    
    Vec3D angle;                                ///Current angular position
    Vec3D angularVelocity;                      ///Current angular velocity
    Vec3D position;                             ///Current particle position
    Vec3D velocity;                             ///Current particle velocity

    Vec3D force;                                ///Interaction force
    Vec3D torque;                               ///Torque

    Vec3D displacement;                         ///Displacement (only used in StatisticsVector, StatisticsPoint)
    Vec3D previousPosition;
#ifdef USE_SIMPLE_VERLET_INTEGRATION
    Vec3D prevPosition;                         ///Previous position
#endif 
};

inline std::ostream& operator<<(std::ostream& os, const BaseParticle &p)
{
	p.print(os);
	return os;
}

inline std::istream& operator>>(std::istream& is, BaseParticle &p)
{
	p.read(is);
	return(is);
}

#endif
