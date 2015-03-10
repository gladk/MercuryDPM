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


#include "BaseInteraction.h"
#include "InteractionHandler.h"
#include "Particles/BaseParticle.h"
#include "Species/BaseSpecies.h"
#include "DPMBase.h"
#include<iomanip>
#include<fstream>

/*!
 * \details This is the constructor which creates a new interactions between two 
 * BaseInteractable objects. The timeStamp is time the interactions is created 
 * and is used to check if the interations is current or has ended.
 * It adds 
 * \param[in] P         BaseInteractable pointer which is the first object involved in the interaction normally a particle.
 * \param[in] I         BaseInteractable pointer which is the second object involved in the interaction often a wall or particle.
 * \param[in] timeStamp Mdouble which is the time the interaction starts.
 */
BaseInteraction::BaseInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
    : BaseObject()
{
    P_ = P;
    P->addInteraction(this);
    I_ = I;
    I->addInteraction(this);
    normal_.setZero();
    overlap_ = 0;
    timeStamp_ = timeStamp;
    species_ = 0;
    force_.setZero();
    torque_.setZero();
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseInteraction::BaseInteraction() finished"<<std::endl;
#endif
}

/*!
 * \details This an copy constructor for a BaseInteraction.
 * \param[in] p BaseInteraction
 */
BaseInteraction::BaseInteraction(const BaseInteraction &p)
    : BaseObject(p)
{
    P_ = p.P_;
    I_ = p.I_;
    normal_ = p.normal_;
    overlap_ = p.overlap_;
    force_ = p.force_;
    torque_ = p.torque_;
    species_ = p.species_;
    timeStamp_ = p.timeStamp_;
}

/*!
 * \details Destructor for BaseInteraction. Also removes the interaction from the
 *          list of interactions for both objects involved in the interaction. 
 */
BaseInteraction::~BaseInteraction()
{
    P_->removeInteraction(this);
    I_->removeInteraction(this);
}

/*!
 * \details BaseInteaction write function. Writes out all the information 
 *          required to recreate this interaction. 
 *          To write this interaction to the screen call write(std::cout).
 *          See also BaseInteraction::read
 * \param[in] os     std::ostream to which the information is written. Note, is
 *                   any ostram is can be file or screen.
 */
void BaseInteraction::write(std::ostream& os) const
{
    os << getName();
    if (dynamic_cast<BaseParticle*>(I_) != nullptr)
        os << " particleIds " << P_->getId() << " " << I_->getId();
        ///\todo should we output id's here? os << " id " << getId() << " particleIds " << P_->getId() << " " << I_->getId();
    else
        os << " particleWallIds " << P_->getId() << " " << I_->getId();
    os <<" timeStamp "<<timeStamp_<< " force " << force_ << " torque " << torque_;
}

/*!
 * \details BaseInteaction read functions. Reads in all the information about an
 *          interaction.
 *          Note, this can be from any istream but would normally be a file
 *          See also BaseInteaction::write
 * \param[in] is     std::istream to which the information is read from.
 */
void BaseInteraction::read(std::istream& is)
{
    //the rest gets read by the interaction handler
    std::string dummy;
    is >> dummy >> force_ >> dummy >> torque_;
}

/*!
 * \details Functions which returns the name of the Interaction here is called
 *          BaseInteraction; but, it should be later overridden by the actual
 *          interaction classes. 
 */
std::string BaseInteraction::getName() const
{
    return "BaseInteraction";
}

/*!
 * \details sets the normal of the interaction, in direction from I to P.       
 * Must be a unit normal vector. This is not checked by the class.
 * \param[in] normal Vec3D which is the normal of the interaction.
 */
void BaseInteraction::setNormal(Vec3D normal)
{
    normal_ = normal;
}

/*!
 * \details set the distance of the interaction.
 * \param[in] distance  Mdouble which is the distance to set.
 */
void BaseInteraction::setDistance(Mdouble distance)
{
    distance_ = distance;
}

/*!
 * \details set the overlap between the two interactable object involved in the 
 *          interactions.
 * \param[in] overlap   Mdouble which is the overlap to set.
 */
void BaseInteraction::setOverlap(Mdouble overlap)
{
    overlap_ = overlap;
}

/*
 * \details set the contact point between the two interactable objects involved
 * \param[in] contactPoint  Vec3D vector which will become the new contact point.
 */
void BaseInteraction::setContactPoint(Vec3D contactPoint)
{
    contactPoint_ = contactPoint;
}

/*!
 * \details Updated the time stamp of the interaction. The time stamp being old
 *          is how ended interactions are detected.
 * \param[in] timeStamp The new timeStamp for the interactions should be the 
 *                      current time step.
 */
void BaseInteraction::setTimeStamp(Mdouble timeStamp)
{
    timeStamp_ = timeStamp;
}

/*!
 * \details set the handler which this interactions belongs to.
 * \param[in] handler   InteractionHandler* pointer to the interaction handler,
 *                      this interaction will belong.
 */
void BaseInteraction::setHandler(InteractionHandler* handler)
{
    handler_ = handler;
}

/*!
 * \details Returns a pointer to the InteractionHandler that this interaction
 *          belongs.
 * \return  Constant pointer to the InteractionHandler.
 */
InteractionHandler* BaseInteraction::getHandler() const
{
    return handler_;
}

/*!
 * \details Removes the interaction from its InteractionHandler. Does no other
 *          cleaning up as it does not remove it from the particles.
 */
void BaseInteraction::removeFromHandler()
{
    handler_->removeObject(getIndex());
}

/*!
 * \details This functions copies the interactions of a real original particle.
 *          It first works out which of P and I is not the original particle. 
 *          Then it create a new interactions between the new ghost copy and 
 *          which every object is not the original particle from the P and I of 
 *          the interaction.
 *          Note, at the end the ghost will be I in the new interaction and 
 *          original item being interacted with will be P.
 * \todo    Can this be simpler if we replace the particle with the ghost.
 * \param[in] original  BaseInteractable* to the original particles who periodic
 *                      image is being created from.
 * \param[in] ghost     BaseInteractble* to the new ghost (periodic partcles) 
 *                      that has just been created. 
 */
void BaseInteraction::copySwitchPointer(const BaseInteractable* original, BaseInteractable* ghost) const
{
    //Copy the interaction of ghost
    BaseInteraction* C = copy();
    //Finds out which of P and I is that the particle from whom the ghost is begin created.
    //The object being interacted with is set to P
    if (C->getP() == original)
    {
        //Reverse some force history
        C->reverseHistory();
        //Set the P to the original particle 
        C->P_ =C->getI();
    }
    //The new ghost particle is set to I in the interaction. 
    C->I_ = ghost;

    //Add the the interaction to both original and the ghost
    C->getP()->addInteraction(C);
    C->getI()->addInteraction(C);
    handler_->addObject(C);
}

/*!
 * \details Return a reference to a Vec3D which contains the current value of
 *          the force associated with this interaction.
 * \return  A reference to a Vec3D containing the total force.
 */
const Vec3D& BaseInteraction::getForce() const
{
    return force_;
}

/*!
 * \details Return a reference to a Vec3D which contains the current value of
 *          the torque associated with the interaction.
 * \return  A reference to a Vec3D containing the total torque.
 */
const Vec3D& BaseInteraction::getTorque() const
{
    return torque_;
}

/*!
 * \details Returns a reference to a Vec3D which contains the current value of 
 *          the normal associated with the interactions.
 * \return  A reference to a Vec3D containing the current normal.
 */
const Vec3D& BaseInteraction::getNormal() const
{
    return normal_;
}

/*!
 * \details Returns a reference to a Vec3D which contains the current value of
 *          the contact point for the particles in interaction. Normally, does
 *          not change about the interaction is created.
 * \return  A reference to a Vec3D containing the contact point.
 */
const Vec3D& BaseInteraction::getContactPoint() const
{
    return contactPoint_;
}

/*!
 * \details Returns a Mdouble to the current overlap for the objects involved in
 *          the interaction.
 * \return Mdouble which is the value of the overlap.
 */
Mdouble BaseInteraction::getOverlap() const
{
    return overlap_;
}

/*!
 * \details Returns a pointer to the first object in the interactions; normally
 *           the particle.
 * \return   Pointer to BaseInteraction but normally will be a BaseParticle.
 */
BaseInteractable* BaseInteraction::getP()
{
    return P_;
}

/*!
 * \details Returns a pointer to the second object in the interaction; often 
 *          a particle or a wall i.e. a general interactale hence I.
 * \return  Pointer to BaseInteraction often a particle or a wall.
 */
BaseInteractable* BaseInteraction::getI()
{
    return I_;
}

/*!
 * \details Returns a constant pointer to the first object in the interactions; normally
 *           the particle.
 * \return   Constant pointer to BaseInteraction but normally will be a BaseParticle.
 */
const BaseInteractable* BaseInteraction::getP() const
{
    return P_;
}

/*!
 * \details Returns a constant pointer to the second object in the interaction; often 
 *          a particle or a wall i.e. a general interactale hence I.
 * \return  Constant pointer to BaseInteraction often a particle or a wall.
 */
const BaseInteractable* BaseInteraction::getI() const
{
    return I_;
}

/*!
 * \details Returns the current time stamp of interaction. This should be the
 *          last time the interaction was active and used to find the end of the
 *          interaction.
 * \return  Mdouble which is the last time the interaction was active.
 */
Mdouble BaseInteraction::getTimeStamp() const
{
    return timeStamp_;
}

/*!
 * \details Various variables in the force law need to be integrated. This is 
 *          the place where this code goes.
 *          Note, it is empty and abstract at this point; but, not virtual as it
 *          as not called from within this class. It is only called by higher
 *          classes.
 * \param[in] timeStep  This is the current time step and is passed in case the
 *                      variables that are integrated depend on the absolute
 *                      time of the system
 *                      \todo Ant: When is this case; is there an examepl? 
 */
void BaseInteraction::integrate(Mdouble timeStep UNUSED)
{

}

/*!
 * \details Sets the species for the interactions. 
 *          Note, this can be either a normal Species or a MixedSpecies; 
 *          depending on if this interaction is between interactables of the
 *          same or different types.
 * \param[in] BaseSpecies* pointer to the actually species of the interaction.
 */
void BaseInteraction::setSpecies(BaseSpecies *species)
{
    species_ = species;
}

/*!
 * \details Changes the first object involved in the interaction; normally a 
 *          particle.
 *          Note, set is slightly misleading as it removed the interaction from
 *          old particle and adds it to the new particle.
 * \param[in] P     BaseInteractable* An interactable object that is involved in
 *                  the interaction.
 */
void BaseInteraction::setP(BaseInteractable* P)
{
    P_->removeInteraction(this);
    P_=P;
    P_->addInteraction(this);
}

/*!
 * \details Changes the second object involved in the interaction; often a
 *          general interactable not always a particle.
 *          Note, set is slightly misleading as it removed the interaction from
 *          old interactable and adds it to the new interactable.
 * \param[in] I     BaseInteractable* An interactable object that is involved in
 *                  the interaction.
 */
void BaseInteraction::setI(BaseInteractable* I)
{
    I_->removeInteraction(this);
    I_=I;
    I_->addInteraction(this);
}

/*!
 * \details Writes the FStat information that is required for the coarse-
 *          graining package MercuryCG if you want stress and force information.
 *          Note, it takes a general ostream but is normally a file i.e. 
 *          ofstream
 * \param[in] os    This is the ostream that the FStat information will be 
 *                  written to. Normally, a file but could be a gerneral
 *                  ostream.
 */
void BaseInteraction::writeToFStat(std::ostream& os) const
{
    BaseParticle* IParticle = dynamic_cast<BaseParticle*>(I_);
    BaseParticle* PParticle = dynamic_cast<BaseParticle*>(P_);

    Vec3D tangentialForce = getTangentialForce();
    Mdouble tangentialOverlap = getTangentialOverlap();

    Mdouble scalarNormalForce = Vec3D::dot(force_, getNormal());
    Mdouble scalarTangentialForce = tangentialForce.getLength();
    Vec3D tangential;
    if (scalarTangentialForce!=0.0)
        tangential = tangentialForce/scalarTangentialForce;
    else
        tangential = Vec3D(0.0,0.0,0.0);

    ///\todo TW centre is used just for backward compatibility; replace centre by contact law; we also have to correct it in StatisticsVector::gatherContactStatistics.
    ///There also seems to be an issue with the normal being defined differently for walls
    Vec3D centre;
    if (IParticle!=0)
        centre = 0.5 * (getP()->getPosition() + getI()->getPosition());
    else
        centre = getP()->getPosition() - normal_ * (PParticle->getRadius() - overlap_);

    if (PParticle!=0 && !PParticle->isFixed())
    {
        os << timeStamp_
            << " " << P_->getIndex()
            << " " << static_cast<int>((IParticle==0?(-I_->getIndex()-1):I_->getIndex()))
            << " " << centre
            << " " << getOverlap()
            << " " << tangentialOverlap
            << " " << scalarNormalForce
            << " " << scalarTangentialForce
            << " " << (IParticle==0?-normal_:normal_)
            << " " << (IParticle==0?-tangential:tangential) << std::endl;
        ///\todo the flip in normal/tangential direction for walls should not be done; this is an old bug
    }
    if (IParticle!=0 && !IParticle->isFixed() && IParticle->getPeriodicFromParticle()==0)
    {
        os << timeStamp_
            << " " << I_->getIndex()
            << " " << P_->getIndex()
            << " " << centre
            << " " << getOverlap()
            << " " << tangentialOverlap
            << " " << scalarNormalForce
            << " " << scalarTangentialForce
            << " " << -normal_
            << " " << -tangential << std::endl;
    }

}

/*
 * \details Returns the distance between the two interactable objects involved
 *          in the interaction.
 * \return  Mdouble which is the distance between the two interacting objects.
 */
Mdouble BaseInteraction::getDistance() const
{
    return distance_;
}

/*!
 * \details Returns tangential overlap.
 *          Note, at this level there cannot be a tangential overlap
 *          hence by default it returns 0. This function will be overridden by 
 *          interactions that have tangential components.
 * \return  Positive Mdouble that is the tangential overlap.
 */
Mdouble BaseInteraction::getTangentialOverlap() const
{
    return 0;
}

/*!
 * \details Returns the vector that is the tangential force
 *          Note, at this level there cannot be a tangential force therefore by
 *          default it return the zero vector. This function will be overridden
 *          by interactions that have tangential components.
 * \return  Vec3D that contains the current tangential force of the interaction.
 */
const Vec3D BaseInteraction::getTangentialForce() const
{
    return Vec3D(0.0,0.0,0.0);
}

/*!
 * \details Returns the relative velocity between the two interactable objects
 *          involved in the interaction.
 * \return  A reference to Vec3D that contains the relative velocity.
 */
const Vec3D& BaseInteraction::getRelativeVelocity() const
{
    return relativeVelocity_;
}

/*!
 * \details Returns the norm (length) of the relative normal velocity.
 *          Note, this can be negative or positive it is not a speed.
 *          \todo Ant: Check this comment.
 * \return  Mdouble that contains the norm (length) of the relative velocity.
 */
Mdouble BaseInteraction::getNormalRelativeVelocity() const
{
    return normalRelativeVelocity_;
}

/*!
 * \details Return the absolve normal force. This is the magnitude of the normal
 *          force.
 *          \todo Ant: Check this comment.
 * \return  Mdouble that contains the absolute norm (length) of the normal 
 *          force.
 */
Mdouble BaseInteraction::getAbsoluteNormalForce() const
{
    return absoluteNormalForce_;
}

/*!
 * \details add an increment to total force in the interaction. This is used by
 *          tangential and non-contact forces (e.g. adhesive forces) as this are
 *          'added' after the normal forces have been computed.
 */
void BaseInteraction::addForce(Vec3D force)
{
    force_+=force;
}

/*!
 * \details add an increment to total torque in the interaction. This is used by
 *          tangential and non-contact forces (e.g. adhesive forces) as this are
 *          'added' after the normal forces have been computed.
 */
void BaseInteraction::addTorque(Vec3D torque)
{
    torque_+=torque;
}

/*!
 * \details set the absolute values of the force. This is used by the normal 
 *          forces as these are always called first and then the tangential and
 *          non-contact (e.g. adhesive forces) forces are added.
 *          See also BaseInteraction::addForce.
 */
void BaseInteraction::setForce(Vec3D force)
{
    force_=force;
}

/*!
 * \details set the absolute values of the torque. This is used by the normal 
 *          forces as these are always called first and then the tangential and
 *          non-contact (e.g. adhesive forces) forces/torques are added.
 *          See also BaseInteraction::addTorque.
 */
void BaseInteraction::setTorque(Vec3D torque)
{
    torque_=torque;
}

/*!
 * \details set the relative velocity between the two particles involved in the
 *          interaction.
 * \param[in] relativeVelocity  This is Vec3D that contains the relative 
 *                              velocity between the two interactable objects.
 */
void BaseInteraction::setRelativeVelocity(Vec3D relativeVelocity)
{
    relativeVelocity_=relativeVelocity;
}

/*!
 * \details set the norm (length) of the normal relative velocity.
 * \param[in] normalRelativeVelocity    Mdouble containing the normal (length)
 *                                      of the normal velocity between the 
 *                                      interactable objects.
 */
void BaseInteraction::setNormalRelativeVelocity(Mdouble normalRelativeVelocity)
{
    normalRelativeVelocity_=normalRelativeVelocity;
}

/*!
 * \details set absolute normal force.
 * \param[in] absoluteNormalForce   Mdouble contain the value of the absolute
 *                                  normal force.
 */
void BaseInteraction::setAbsoluteNormalForce(Mdouble absoluteNormalForce)
{
    absoluteNormalForce_ = absoluteNormalForce;
}

/*!
 * \details Returns a BaseSpecies pointer to the current species.
 *          Note, this will be either a Species or a MixedSpecies done of which
 *          are derived from BaseSpecies.
 * \return  A BaseSpecies pointer to the species associated with this 
 *          interaction.
 */
const BaseSpecies* BaseInteraction::getBaseSpecies() const
{
    return species_;
}

/*!
 * \details The children of this class will implement this function; however,
 *          it is blank.
 *          This function will do the actually force calculation for this
 *          interaction.
 *          Note, it is not virtual as it is not called from within this class.
 */
void BaseInteraction::computeForce()
{}

/*!
 * \details The children of this class will implement this function; however, it
 *          is blank.
 *          This function will contain the calculation for th elastic energy.
 *          Note, it is not virtual as it is not called from within this class.
 */
Mdouble BaseInteraction::getElasticEnergy() const
{
    return 0.0;
}

/*!
 * \details The children of this class will implement this function; however, it
 *          is blank.
 *          This function will contain code that changes some history
 *          information if a copy changes the P <-> I order; as can happen in
 *          creating a periodic particle.
 *          See also PeriodicBoundary::createPeriodicParticles and
 *          BaseInteraction::copySwitchPointer
 *          Note, it is not virtual as it is not called from within this class.
 */
void BaseInteraction::reverseHistory()
{
}

void BaseInteraction::rotateHistory(Matrix3D& rotationMatrix)
{
    contactPoint_=rotationMatrix*contactPoint_;
    relativeVelocity_=rotationMatrix*relativeVelocity_;
    force_=rotationMatrix*force_;
    torque_=rotationMatrix*torque_;
    normal_=rotationMatrix*normal_;
    ///\todo some of these might be unneccesary
}

/*!
 * \details Computes the effective radius of a particle. This is used by many of
 *          the later interaction models.
 *          This functions assumes P is the particle and I is either a particle
 *          or a wall.
 *          Effective Radius = \f$R_I*R_P/(R_I+R_P)\f$
 *          See also BaseInteraction::getEffectiveCorrectedRadius()
 * \return  A Mdouble which is the effective radius of the particles.
 */
Mdouble BaseInteraction::getEffectiveRadius() const
{
    const BaseParticle* PParticle = dynamic_cast<const BaseParticle*>(getP());
    const BaseParticle* IParticle = dynamic_cast<const BaseParticle*>(getI());
    if (PParticle==nullptr)
    {
        std::cerr << "BaseInteraction::getEffectiveCorrectedRadius(): first interactable P is not a particle" << std::endl;
        exit(-1);
    }
    //Compute the reduced diameter
    if (IParticle==nullptr) //if particle-wall
    {
        return PParticle->getRadius();
    }
    else
    {
        Mdouble radiusP = PParticle->getRadius();
        Mdouble radiusI = IParticle->getRadius();
        return radiusP * radiusI / (radiusP + radiusI);
    }
}

/*!
 * \details Computes the effective corrected radius of a particle. This is used 
 *          by some of the later interaction models. This correction is for the
 *          overlap.
 *          This functions assumes P is the particle and I is either a particle
 *          or a wall.
 *          See also BaseInteraction::getEffectiveCorrectedRadius()
 * \return  A Mdouble which is the effective radius of the particles.
 */
Mdouble BaseInteraction::getEffectiveCorrectedRadius()
{
    BaseParticle* PParticle = dynamic_cast<BaseParticle*>(getP());
    BaseParticle* IParticle = dynamic_cast<BaseParticle*>(getI());
    if (PParticle==nullptr)
    {
        std::cerr << "BaseInteraction::getEffectiveCorrectedRadius(): first interactable P is not a particle" << std::endl;
        exit(-1);
    }
    //Compute the reduced diameter
    if (IParticle==nullptr) //if particle-wall
    {
        return PParticle->getRadius() - 0.5*getOverlap();
    }
    else
    {
        Mdouble radiusP = PParticle->getRadius() - 0.5*getOverlap();
        Mdouble radiusI = IParticle->getRadius() - 0.5*getOverlap();
        return radiusP * radiusI / (radiusP + radiusI);
    }
}

/*!
 * \todo Thomas please document.
 */
void BaseInteraction::gatherContactStatistics()
{
    BaseParticle* IParticle = dynamic_cast<BaseParticle*>(I_);
    BaseParticle* PParticle = dynamic_cast<BaseParticle*>(P_);

    Vec3D tangentialForce = getTangentialForce();
    Mdouble tangentialOverlap = getTangentialOverlap();

    Mdouble scalarNormalForce = Vec3D::dot(force_, getNormal());
    Mdouble scalarTangentialForce = tangentialForce.getLength();
    Vec3D tangential;
    if (scalarTangentialForce!=0.0)
        tangential = tangentialForce/scalarTangentialForce;
    else
        tangential = Vec3D(0.0,0.0,0.0);

    ///\todo TW centre is used just for backward compatibility; replace centre by contact law; we also have to correct it in StatisticsVector::gatherContactStatistics.
    ///There also seems to be an issue with the normal being defined differently for walls
    Vec3D centre;
    if (IParticle!=0)
        centre = 0.5 * (getP()->getPosition() + getI()->getPosition());
    else
        centre = getP()->getPosition() - normal_ * (PParticle->getRadius() - overlap_);

    if (PParticle!=0 && !PParticle->isFixed())
    {
        getHandler()->getDPMBase()->gatherContactStatistics(
            P_->getIndex(),
            static_cast<int>((IParticle==0?(-I_->getIndex()-1):I_->getIndex())),
            centre,
            getOverlap(),
            tangentialOverlap,
            scalarNormalForce,
            scalarTangentialForce,
            (IParticle==0?-normal_:normal_),
            (IParticle==0?-tangential:tangential));
    }
    if (IParticle!=0 && !IParticle->isFixed() && IParticle->getPeriodicFromParticle()==0)
    {
        getHandler()->getDPMBase()->gatherContactStatistics(
            I_->getIndex(),
            static_cast<int>(P_->getIndex()),
            centre,
            getOverlap(),
            tangentialOverlap,
            scalarNormalForce,
            scalarTangentialForce,
            -normal_,
            -tangential);

    }
}
