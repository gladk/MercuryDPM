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

#ifndef FrictionSpecies_H
#define FrictionSpecies_H
#include "SlidingFrictionSpecies.h"
#include "Math/ExtendedMath.h"
#include "Interactions/FrictionForceInteractions/FrictionInteraction.h"

/*!
 * \brief FrictionSpecies contains the parameters used to describe sliding, rolling and torsional friction.
 * \details See FrictionInteraction::computeForce for a description of the force law.
 */
///\todo TW: should this class be renamed SlidingRollingTorsionFrictionSpecies?
class FrictionSpecies : public SlidingFrictionSpecies
{
public:
    ///\brief The correct Interaction type for this FrictionForceSpecies
    typedef FrictionInteraction InteractionType;

    ///\brief The default constructor.
    FrictionSpecies();

    ///\brief The default copy constructor.
    FrictionSpecies(const FrictionSpecies &s);

    ///\brief The default destructor.
    virtual ~FrictionSpecies();

    /// \brief Reads the species properties from an input stream.
    void read(std::istream& is);

    /// \brief Writes the species properties to an output stream.
    void write(std::ostream& os) const;

    /// \brief Used in Species::getName to obtain a unique name for each Species.
    std::string getBaseName() const;

    /*!
     * \brief allocates a new SlidingFrictionInteraction object and returns a pointer to it.
     */
    BaseInteraction* getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);

    /*!
     * \brief Returns true if torques have to be calculated.
     */
    bool getUseAngularDOFs() const;

    ///\brief creates default values for mixed species
    void mix(FrictionSpecies* const S, FrictionSpecies* const T);

//setters and getters

    ///Allows the spring constant to be changed
    void setRollingStiffness(Mdouble new_kt);

    ///Allows the spring constant to be accessed
    Mdouble getRollingStiffness() const;

    ///Allows the tangential viscosity to be changed
    void setRollingDissipation(Mdouble new_dispt);

    ///Allows the tangential viscosity to be accessed
    Mdouble getRollingDissipation() const;

    ///Allows the (dynamic) Coulomb friction coefficient to be changed; also sets mu_s by default
    //mu has to be set to allow tangential forces (sets dispt=disp as default)
    void setRollingFrictionCoefficient(Mdouble new_mu);

    ///Allows the (dynamic) Coulomb friction coefficient to be accessed
    Mdouble getRollingFrictionCoefficient() const;

    ///Allows the static Coulomb rolling friction coefficient to be changed
    void setRollingFrictionCoefficientStatic(Mdouble new_mu);

    ///Allows the static Coulomb rolling friction coefficient to be accessed
    Mdouble getRollingFrictionCoefficientStatic() const;

    ///Allows the torsion stiffness to be changed
    void setTorsionStiffness(Mdouble new_kt);

    ///Allows the torsion stiffness to be accessed
    Mdouble getTorsionStiffness() const;

    ///Allows the torsion viscosity to be changed
    void setTorsionDissipation(Mdouble new_dispt);

    ///Allows the torsion viscosity to be accessed
    Mdouble getTorsionDissipation() const;

    ///Allows the (dynamic) Coulomb torsion friction coefficient to be changed; also sets mu_s by default
    //mu has to be set to allow tangential forces (sets dispt=disp as default)
    void setTorsionFrictionCoefficient(Mdouble new_mu);

    ///Allows the (dynamic) Coulomb torsion friction coefficient to be accessed
    Mdouble getTorsionFrictionCoefficient() const;

    ///Allows the static Coulomb torsion friction coefficient to be accessed
    void setTorsionFrictionCoefficientStatic(Mdouble new_mu);

    ///Allows the static Coulomb torsion friction coefficient to be accessed
    Mdouble getTorsionFrictionCoefficientStatic() const;

private:
   /*! 
     * \brief rolling stiffness. 
     * \details Typically set to 2/5 of the stiffness of the normal force, as  
     * both the rolling and the normal spring have the same oscillation  
     * frequency (and thus require the same timeStep)in this case. 
     */
    Mdouble rollingStiffness_;
    
    /*! 
     * \brief rolling dissipation coefficient. 
     * \details Typically set to 2/5 of the normal force dissipation, as both
     * the tangential and the normal spring have the same restitution coefficients 
     * (if the rolling and normal stiffnesses also have a ratio of 2/5). 
     */
    Mdouble rollingDissipation_; 
   
    
    /*! 
     * \brief (dynamic) Coulomb rolling friction coefficient. 
     */
    Mdouble rollingFrictionCoefficient_; 
    
    /*! 
     * \brief static Coulomb rolling friction coefficient  
     * (by default set equal to the dynamic one).
     */
    Mdouble rollingFrictionCoefficientStatic_; 
    
    /*! 
     * \brief rolling stiffness. 
     * \details Typically set to 2/5 of the stiffness of the normal force, as  
     * both the rolling and the normal spring have the same oscillation  
     * frequency (and thus require the same timeStep)in this case. 
     */
    Mdouble torsionStiffness_;
    
    /*! 
     * \brief rolling dissipation coefficient. 
     * \details Typically set to 2/5 of the normal force dissipation, as both
     * the tangential and the normal spring have the same restitution coefficients 
     * (if the rolling and normal stiffnesses also have a ratio of 2/5). 
     */
    Mdouble torsionDissipation_; 

    /*! 
     * \brief (dynamic) Coulomb torsion friction coefficient. 
     */
    Mdouble torsionFrictionCoefficient_; 
    
    /*! 
     * \brief static Coulomb torsion friction coefficient  
     * (by default set equal to the dynamic one).
     */
    Mdouble torsionFrictionCoefficientStatic_; 
};
#endif
