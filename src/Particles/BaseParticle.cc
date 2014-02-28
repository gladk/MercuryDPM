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


#include "BaseParticle.h"


#include "../ParticleHandler.h"
//#include "../HGRID.h"
#include "../CSpecies.h"



///Basic Particle contructors, creates an Particle at (0,0,0) with radius, mass and inertia equal to 1
BaseParticle::BaseParticle()
{
    position.set_zero();
    velocity.set_zero();
    displacement.set_zero();
    force.set_zero();
    radius = 1.0;
    angle.set_zero();
    angularVelocity.set_zero();
    torque.set_zero();
    mass = invMass = 1.0;
    inertia = invInertia = 1.0;
    HGRID_NextObject=NULL;
    
    indSpecies = 0;
    periodicFromParticle=NULL;
#ifdef ContactListHgrid
    firstPossibleContact=0;
#endif
    HGRID_NextObject = 0;
    HGRID_PrevObject = 0;
    HGRID_x = 99999;
    HGRID_y = 99999;
    HGRID_z = 99999;        
    handler=0;
    
    #ifdef CONSTUCTOR_OUTPUT
        std::cerr << "BaseParticle() finished"<<std::endl;
    #endif        
}

///BaseParticle copy contstructor, which accepts as input a reference to a BaseParticle. It creates a copy of this BaseParticle and all it's information. Usually it is better to use the copy() function for polymorfism.
BaseParticle::BaseParticle(const BaseParticle &p)
{
    position = p.position;
    velocity = p.velocity;
    displacement = p.displacement;
    force = p.force;
    radius = p.radius;
    angle = p.angle;
    angularVelocity = p.angularVelocity;
    torque = p.torque;
    mass = p.get_Mass();
    invMass = p.get_InvMass();
    inertia = p.get_Inertia();
    invInertia = p.get_InvInertia();
    
    HGRID_NextObject = 0;
    HGRID_PrevObject = 0;
    HGRID_x = 99999;
    HGRID_y = 99999;
    HGRID_z = 99999;
    HGRID_Level = p.HGRID_Level;
    
    handler=0;
    
    indSpecies = p.indSpecies;
    periodicFromParticle=p.periodicFromParticle;
#ifdef ContactListHgrid
    firstPossibleContact=0;
#endif
    
    #ifdef CONSTUCTOR_OUTPUT
        std::cerr << "BaseParticle(BaseParticle &p) finished"<<std::endl;
    #endif            
}
    
///BaseParticle destructor, needs to be implemented and checked if it removes tangential spring information
BaseParticle::~BaseParticle()
{
    if(handler)
    {
        handler->checkExtremaOnDelete(this);
	}
    #ifdef CONSTUCTOR_OUTPUT
        std::cerr << "virtual ~BaseParticle finished"<<std::endl;
    #endif    
}

///BaseParticle copy method. It calls to copy contrustor of this BaseParticle, usefull for polymorfism
BaseParticle* BaseParticle::copy() const
{
    return new BaseParticle(*this);
}

///Get BaseParticle volume function, which required a reference to the Species vector. It returns the volume of the BaseParticle.
Mdouble BaseParticle::get_Volume(std::vector<CSpecies>& Species) const
{
    switch(Species[indSpecies].dim_particle)
    {
        case 3 :
            return (4.0/3.0 * constants::pi * radius * radius * radius); 
        case 2 :
            return (constants::pi * radius * radius);                              
        case 1 :
            return (2.0 * radius);                                     
        default :
        {
            std::cerr<<"In get_Volume(vector<CSpecies>& Species) the dimension of the particle is not set"<<std::endl;
            exit(-1);
        }
    }
} 

///Fix BaseParticle function. It fixes a BaseParticle by setting its inverse mass and inertia and velocities to zero
void BaseParticle::fixParticle(){
    mass=1e20; invMass=0.0; velocity.set_zero(); inertia=1e20; invInertia=0.0; angularVelocity.set_zero();
#ifdef USE_SIMPLE_VERLET_INTEGRATION
    position = PrevPosition;
#endif     
}

///Is fixed BaseParticle function. It returns wether a BaseParticle is fixed or not, by cheking its inverse Mass.
bool BaseParticle::isFixed(){return (invMass==0.0);}

///Unfix BaseParticle function, which required a reference to the Species vector. It un fixes a BaseParticle by compyting the BaseParticles mass and inertia
void BaseParticle::unfix(std::vector<CSpecies>& Species){
    invMass=1.0;
    compute_particle_mass(Species);
}

///Compute BaseParticle mass function, which required a reference to the Species vector. It copmuters the BaseParticles mass, Inertia and the inverses.
void BaseParticle::compute_particle_mass(std::vector<CSpecies>& Species) {
    if (!isFixed()) {
        switch(Species[indSpecies].dim_particle)
        {
            case 3 :
            { 
                set_Mass(4.0/3.0 * constants::pi * radius * radius * radius * Species[indSpecies].rho); 
                set_inertia(.4 * get_Mass() * sqr(radius));
                break;
            }
            case 2 :
            { 
                set_Mass(constants::pi * radius * radius * Species[indSpecies].rho);                              
                set_inertia(.5 * get_Mass() * sqr(radius));
                break;
            }
            case 1 :
            {
                set_Mass(2.0 * radius * Species[indSpecies].rho);                                     
                set_inertia(0.0);
                break;
            }
            default :
            {
                std::cerr<<"In compute_particle_mass(vector<CSpecies>& Species) the dimension of the particle is not set"<<std::endl;
                exit(-1);
            }
        }
    }
}

///BaseParticle print function, which accepts an os std::stringstream as input. It prints human readable BaseParticle information to the std::stringstream
void BaseParticle::print(std::ostream& os) const
{
	os << "BP "<< position << " " << velocity << " " << radius << " " << angle << " " << angularVelocity << " " << invMass << " " << invInertia << " " <<indSpecies;
}

void BaseParticle::read(std::istream& is)
{ 
	is >> position >> velocity >> radius >> angle >> angularVelocity >> invMass >> invInertia >> indSpecies;
	if (invMass) mass = 1.0/invMass; else mass = 1e20; 
	if (invInertia) inertia = 1.0/invInertia; else inertia = 1e20; 
}

void BaseParticle::print_HGRID(std::ostream& os) {
    os << "Particle( HGRID_Level:" << HGRID_Level 
        << ", HGRID_x:" << HGRID_x 
        << ", HGRID_y:" << HGRID_y 
        << ", HGRID_z:" << HGRID_z 
        << ")";
}    

void BaseParticle::moveInHandler(int newPos)
{
    _index=newPos;
}

int              BaseParticle::get_HGRID_Level() const         {return HGRID_Level;}
BaseParticle*        BaseParticle::get_HGRID_NextObject() const     {return HGRID_NextObject;}
BaseParticle*        BaseParticle::get_HGRID_PrevObject() const     {return HGRID_PrevObject;}
#ifdef ContactListHgrid
PossibleContact* BaseParticle::get_FirstPossibleContact() const {return firstPossibleContact;}
#endif
int              BaseParticle::get_HGRID_x() const              {return HGRID_x;}
int              BaseParticle::get_HGRID_y() const              {return HGRID_y;}
int              BaseParticle::get_HGRID_z() const              {return HGRID_z;}
int              BaseParticle::get_Index()   const              {return _index;}
Mdouble          BaseParticle::get_Inertia() const              {return inertia;}
Mdouble          BaseParticle::get_InvInertia() const           {return invInertia;}
Mdouble          BaseParticle::get_InvMass() const              {return invMass;}
Mdouble          BaseParticle::get_KineticEnergy() const        {return 0.5 * mass * velocity.GetLength2();}
Mdouble          BaseParticle::get_Mass() const                 {return mass;}
BaseParticle*        BaseParticle::get_PeriodicFromParticle() const {return periodicFromParticle;}
const Vec3D&     BaseParticle::get_Position() const             {return position;}
Mdouble          BaseParticle::get_Radius() const               {return radius;}
Mdouble          BaseParticle::get_InteractionRadius() const    {
    if (handler) return radius + handler->getSpecies(indSpecies).get_InteractionDistance()/2.0; else return radius;
}
Mdouble          BaseParticle::get_WallInteractionRadius() const    {
    if (handler) return radius + handler->getSpecies(indSpecies).get_InteractionDistance(); else return radius;
}
int              BaseParticle::get_Species() const              {return indSpecies;}
int              BaseParticle::get_IndSpecies() const           {return indSpecies;}
int              BaseParticle::get_Id() const                   {return _id;}
const Vec3D&     BaseParticle::get_Velocity() const             {return velocity;}
const Vec3D&     BaseParticle::get_Angle() const                {return angle;}
const Vec3D&     BaseParticle::get_AngularVelocity() const      {return angularVelocity;}
const Vec3D&     BaseParticle::get_Force() const                {return force;}
const Vec3D&     BaseParticle::get_Torque() const               {return torque;}
const Vec3D&     BaseParticle::get_Displacement() const         {return displacement;}
const Vec3D&     BaseParticle::get_PreviousPosition() const     {return previousPosition;}
ParticleHandler* BaseParticle::getHandler() const               {return handler;}
const Vec3D      BaseParticle::get_Displacement2(Mdouble xmin, Mdouble xmax, Mdouble ymin, Mdouble ymax, Mdouble zmin, Mdouble zmax, Mdouble t) const
{
    Vec3D disp = get_Position()-get_PreviousPosition();
    if (xmax>xmin && fabs(disp.X)>.5*(xmax-xmin)) {
        if (disp.X>0) disp.X -=    xmax-xmin;
        else disp.X += xmax-xmin;
    }
    if (ymax>ymin && fabs(disp.Y)>.5*(ymax-ymin)) {
        if (disp.Y>0) disp.Y -=    ymax-ymin;
        else disp.Y += ymax-ymin;
    }
    if (zmax>zmin && fabs(disp.Z)>.5*(zmax-zmin)) {
        if (disp.Z>0) disp.Z -=    zmax-zmin;
        else disp.Z += zmax-zmin;
    }
    disp /= t;
    return disp;    
}
    
void BaseParticle::set_inertia                    (const Mdouble _new)            {if (_new>=0){inertia=_new; invInertia=1.0/_new;} else { std::cerr << "Error in set_inertia ("<<_new<<")"<< std::endl; exit(-1); }}
void BaseParticle::set_infiniteInertia            ()                              {inertia=1e20; invInertia=0;} //> i.e. no rotations
void BaseParticle::set_periodicFromParticle       (BaseParticle* _new)                {periodicFromParticle=_new;}
void BaseParticle::set_species                    (const int _new)                {indSpecies=_new;}
void BaseParticle::set_Index                      (const int _new)                {_index=_new;}
void BaseParticle::set_HGRID_x                    (const int _new)                {HGRID_x=_new;}
void BaseParticle::set_HGRID_y                    (const int _new)                {HGRID_y=_new;}
void BaseParticle::set_HGRID_z                    (const int _new)                {HGRID_z=_new;}
void BaseParticle::set_HGRID_Level                (const int _new)                {HGRID_Level=_new;}
void BaseParticle::set_HGRID_NextObject           (BaseParticle* _new)                {HGRID_NextObject=_new;}
void BaseParticle::set_HGRID_PrevObject           (BaseParticle* _new)                {HGRID_PrevObject=_new;}
#ifdef ContactListHgrid
void BaseParticle::set_FirstPossibleContact       (const PossibleContact* PC)     {firstPossibleContact=PC;}
#endif
void BaseParticle::set_Radius                     (const Mdouble _new)
{
    radius=_new;
    if(handler)
        handler->checkExtrema(this);
}
void BaseParticle::set_IndSpecies                 (const int _new)                {indSpecies=_new;}
void BaseParticle::set_Id                         (const int _new)                {_id=_new;}
void BaseParticle::set_Mass                       (const Mdouble _new)            {if(_new>=0.0) {if(invMass){mass=_new;invMass=1.0/_new;}} else { std::cerr << "Error in set_Mass(" << _new << ")" << std::endl; exit(-1); }} //InvMass=0 is a flag for a fixed particle
void BaseParticle::set_Velocity                   (const Vec3D& _new)             {velocity=_new;}
void BaseParticle::set_Position                   (const Vec3D& _new)             {position=_new;}
void BaseParticle::set_Displacement               (const Vec3D& _new)             {displacement=_new;}
void BaseParticle::set_PreviousPosition           (const Vec3D& _new)             {previousPosition=_new;}
void BaseParticle::set_Angle                      (const Vec3D& _new)             {angle=_new;}
void BaseParticle::set_AngularVelocity            (const Vec3D& _new)             {angularVelocity=_new;}
void BaseParticle::set_Force                      (const Vec3D& _new)             {force=_new;}
void BaseParticle::set_Torque                     (const Vec3D& _new)             {torque=_new;}
void BaseParticle::setHandler                     (ParticleHandler* _new)         {handler=_new;}

void BaseParticle::move                           (const Vec3D& _new)             {position+=_new;}
void BaseParticle::movePrevious                   (const Vec3D& _new)             {previousPosition+=_new;}
void BaseParticle::accelerate                     (const Vec3D& _new)             {velocity+=_new;}
void BaseParticle::rotate                         (const Vec3D& _new)             {angle+=_new;}
void BaseParticle::angularAccelerate              (const Vec3D& _new)             {angularVelocity+=_new;}
void BaseParticle::add_Displacement               (const Vec3D& _new)             {displacement+=_new;}
void BaseParticle::add_Force                      (const Vec3D& _new)             {force+=_new;}
void BaseParticle::add_Torque                     (const Vec3D& _new)             {torque+=_new;}

#ifdef USE_SIMPLE_VERLET_INTEGRATION
void BaseParticle::set_PrevPosition               (const Vec3D& _new)             {prevPosition=_new;}
void BaseParticle::prevMove                       (const Vec3D& _new)             {prevPosition+=_new;}
const Vec3D&    BaseParticle::get_PrevPosition()              {return prevPosition;}
#endif
