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

#include <Species/LinearViscoelasticSpecies.h>
#include "InteractionHandler.h"
#include "Particles/BaseParticle.h"
#include "Interactions/BaseInteraction.h"
#include "Species/ParticleSpecies.h"
#include "ParticleHandler.h"
#include "DPMBase.h"

///Basic Particle constructors, creates an Particle at (0,0,0) with radius, mass and inertia equal to 1
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

///BaseParticle copy contstructor, which accepts as input a reference to a BaseParticle. It creates a copy of this BaseParticle and all it's information. Usually it is better to use the copy() function for polymorfism.
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

///BaseParticle destructor, needs to be implemented and checked if it removes tangential spring information
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

///BaseParticle copy method. It calls to copy constructor of this BaseParticle, usefull for polymorfism
BaseParticle* BaseParticle::copy() const
{
    return new BaseParticle(*this);
}

///Get BaseParticle volume function, which required a reference to the Species vector. It returns the volume of the BaseParticle.
Mdouble BaseParticle::getVolume() const
{
    if (handler_ == nullptr)
    {
        std::cerr << "Error in getVolume: no handler specified" << std::endl;
        exit(-1);
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
            {
            std::cerr << "In getVolume(vector<CSpecies>& Species) the dimension of the particle is not set" << std::endl;
            exit(-1);
        }
    }
}

///Fix BaseParticle function. It fixes a BaseParticle by setting its inverse mass and inertia and velocities to zero
void BaseParticle::fixParticle()
{
    mass_ = 1e20;
    invMass_ = 0.0;
    inertia_ = 1e20;
    invInertia_ = 0.0;
    setVelocity(Vec3D(0.0,0.0,0.0));
    setAngularVelocity(Vec3D(0.0,0.0,0.0));
}

///Is fixed BaseParticle function. It returns whether a BaseParticle is fixed or not, by checking its inverse Mass.
bool BaseParticle::isFixed() const
{
    return (invMass_ == 0.0);
}

///Unfix BaseParticle function, which required a reference to the Species vector. It unfixes a BaseParticle by compyting the Particles mass and inertia
void BaseParticle::unfix()
{
    invMass_ = 1.0;
    computeMass();
}

///Compute BaseParticle mass function, which required a reference to the Species vector. It computes the Particles mass, Inertia and the inverses.
/// this function is called, if BaseParticleHandler::addObject, SpeciesHandler::addObject, ParticleSpecies::setDensity, BaseParticle::setRadius or DPMBase::setParticleDimensions is called
void BaseParticle::computeMass()
{
    if (!isFixed())
    {
        switch (getParticleDimensions())
        {
            case 3:
                {
                setMass(4.0 / 3.0 * constants::pi * radius_ * radius_ * radius_ * getSpecies()->getDensity());
                setInertia(.4 * getMass() * mathsFunc::square(radius_));
                break;
            }
            case 2:
                {
                setMass(constants::pi * radius_ * radius_ * getSpecies()->getDensity());
                setInertia(.5 * getMass() * mathsFunc::square(radius_));
                break;
            }
            case 1:
                {
                setMass(2.0 * radius_ * getSpecies()->getDensity());
                setInertia(0.0);
                break;
            }
            default:
                {
                std::cerr << "In computeMass() the dimension of the particle is not set" << std::endl;
                exit(-1);
            }
        }
    }
}

///BaseParticle print function, which accepts an os std::stringstream as input. It prints human readable BaseParticle information to the std::stringstream
void BaseParticle::write(std::ostream& os) const
        {
    BaseInteractable::write(os);
    os << " radius " << radius_
            << " invMass " << invMass_
            << " invInertia " << invInertia_;
}

std::string BaseParticle::getName() const
{
    return "BaseParticle";
}

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

void BaseParticle::printHGrid(std::ostream& os) const
        {
    os << "Particle( HGRID_Level:" << HGridLevel_
            << ", HGRID_x:" << HGridX_
            << ", HGRID_y:" << HGridY_
            << ", HGRID_z:" << HGridZ_
            << ")";
}

unsigned int BaseParticle::getHGridLevel() const
{
    return HGridLevel_;
}
BaseParticle* BaseParticle::getHGridNextObject() const
{
    return HGridNextObject_;
}
BaseParticle* BaseParticle::getHGridPrevObject() const
{
    return HGridPrevObject_;
}
#ifdef CONTACT_LIST_HGRID
PossibleContact* BaseParticle::getFirstPossibleContact() const
{   
    return firstPossibleContact;
}
#endif
int BaseParticle::getHGridX() const
{
    return HGridX_;
}
int BaseParticle::getHGridY() const
{
    return HGridY_;
}
int BaseParticle::getHGridZ() const
{
    return HGridZ_;
}
Mdouble BaseParticle::getInertia() const
{
    return inertia_;
}
Mdouble BaseParticle::getInvInertia() const
{
    return invInertia_;
}
Mdouble BaseParticle::getInvMass() const
{
    return invMass_;
}
Mdouble BaseParticle::getKineticEnergy() const
{
    return 0.5 * mass_ * getVelocity().getLengthSquared();
}
Mdouble BaseParticle::getMass() const
{
    return mass_;
}
BaseParticle* BaseParticle::getPeriodicFromParticle() const
{
    return periodicFromParticle_;
}
Mdouble BaseParticle::getRadius() const
{
    return radius_;
}
Mdouble BaseParticle::getInteractionRadius() const
{
    return radius_ + getSpecies()->getInteractionDistance() * 0.5;
}
Mdouble BaseParticle::getWallInteractionRadius() const
{
    return radius_ + getSpecies()->getInteractionDistance();
}
const Vec3D& BaseParticle::getDisplacement() const
{
    return displacement_;
}
const Vec3D& BaseParticle::getPreviousPosition() const
{
    return previousPosition_;
}
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
void BaseParticle::setInertia(const Mdouble _new)
{
    if (_new >= 0)
    {
        inertia_ = _new;
        invInertia_ = 1.0 / _new;
    }
    else
    {
        std::cerr << "Error in set_inertia (" << _new << ")" << std::endl;
        exit(-1);
    }
}
void BaseParticle::setInfiniteInertia()
{
    inertia_ = 1e20;
    invInertia_ = 0;
} //> i.e. no rotations
void BaseParticle::setPeriodicFromParticle(BaseParticle* _new)
{
    periodicFromParticle_ = _new;
}
void BaseParticle::setHGridX(const int _new)
{
    HGridX_ = _new;
}
void BaseParticle::setHGridY(const int _new)
{
    HGridY_ = _new;
}
void BaseParticle::setHGridZ(const int _new)
{
    HGridZ_ = _new;
}
void BaseParticle::setHGridLevel(const unsigned int _new)
{
    HGridLevel_ = _new;
}
void BaseParticle::setHGridNextObject(BaseParticle* _new)
{
    HGridNextObject_ = _new;
}
void BaseParticle::setHGridPrevObject(BaseParticle* _new)
{
    HGridPrevObject_ = _new;
}
#ifdef CONTACT_LIST_HGRID
void BaseParticle::setFirstPossibleContact (PossibleContact* PC)
{
    firstPossibleContact=PC;
}
#endif
void BaseParticle::setRadius(const Mdouble _new)
{
    radius_ = _new;
    if (getHandler())
    {
        computeMass();
        getHandler()->checkExtrema(this);
    }
}
void BaseParticle::setMass(const Mdouble _new)
{
    if (_new >= 0.0)
    {
        if (invMass_ != 0.0)
        {
            mass_ = _new;
            invMass_ = 1.0 / _new;
        }
    }
    else
    {
        std::cerr << "Error in set_Mass(" << _new << ")" << std::endl;
        exit(-1);
    }
} //InvMass=0 is a flag for a fixed particle
void BaseParticle::setDisplacement(const Vec3D& _new)
{
    displacement_ = _new;
}
void BaseParticle::setPreviousPosition(const Vec3D& _new)
{
    previousPosition_ = _new;
}
void BaseParticle::movePrevious(const Vec3D& _new)
{
    previousPosition_ += _new;
}
void BaseParticle::accelerate(const Vec3D& _new)
{
    addVelocity(_new);
}
void BaseParticle::angularAccelerate(const Vec3D& _new)
{
    addAngularVelocity(_new);
}
void BaseParticle::addDisplacement(const Vec3D& _new)
{
    displacement_ += _new;
}

void BaseParticle::setHandler(ParticleHandler* handler)
{
    handler_ = handler;
    setSpecies(getHandler()->getDPMBase()->speciesHandler.getObject(getIndSpecies()));
}

ParticleHandler* BaseParticle::getHandler() const
{
    return handler_;
}

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
        C->setContactPoint(P->getPosition() - (P->getRadius() - 0.5 * C->getOverlap()) * C->getNormal());
        return C;
    }
    else
    {
        return 0;
    }
}

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

unsigned int BaseParticle::getParticleDimensions() const
{
    return getHandler()->getDPMBase()->getParticleDimensions();
}

void BaseParticle::setIndSpecies(unsigned int indSpecies)
{
    if (handler_ != nullptr)
    {
        setSpecies(getHandler()->getDPMBase()->speciesHandler.getObject(getIndSpecies()));
    }
    else
    {
        BaseInteractable::setIndSpecies(indSpecies);
    }
}

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
                setHandler(&dB->particleHandler);

        }
    }
}