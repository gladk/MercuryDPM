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

#ifndef MIXEDSPECIES_H
#define MIXEDSPECIES_H
#include "Species/FrictionForceSpecies/EmptyFrictionSpecies.h"
#include "Species/AdhesiveForceSpecies/EmptyAdhesiveSpecies.h"
#include "Interactions/Interaction.h"
class BaseInteraction;

/*!
 * \class MixedSpecies
 * \brief Contains contact force properties for contacts between 
 * particles with two different species.
 * \details See Species for details. 
*/
template<class NormalForceSpecies, class FrictionForceSpecies = EmptyFrictionSpecies, class AdhesiveForceSpecies = EmptyAdhesiveSpecies>
class MixedSpecies : public NormalForceSpecies, public FrictionForceSpecies, public AdhesiveForceSpecies
{
public:

    ///\brief The default constructor.
    MixedSpecies();

    ///\brief The default copy constructor.
    MixedSpecies(const MixedSpecies &s);

    ///\brief Creates a mixed species with the same force properties as a Species.
    MixedSpecies(const Species<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies> &s);

    ///\brief The default destructor.
    virtual ~MixedSpecies();

    /*!
     * \brief Creates a deep copy of the MixedSpecies from which it is called.
     */
    MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>* copy() const;

    /// \brief Reads the species properties from an input stream.
    void read(std::istream& is);

    /// \brief Writes the MixedSpecies properties to an output stream.
    void write(std::ostream& os) const;

    ///\brief Returns the name of the MixedSpecies as it is used in the restart file. 
    std::string getName() const;

    /*!
     * \brief When a contact between two particles is determined, an Interaction 
     * object is created, as the type of Interaction depends on the MixedSpecies type.
     */
    BaseInteraction* getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp);

    /*!
     * \brief Returns true if torques have to be calculated.
     */
    bool getUseAngularDOFs() const;

    /*!
     * \brief sets the MixedSpecies properties by mixing the properties of  
     * two particle species
     */
    void mixAll(BaseSpecies * const S, BaseSpecies * const T);

    ///Returns the particle distance below which adhesive forces can occur (needed for contact detection)
    Mdouble getInteractionDistance() const;
};

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::MixedSpecies()
: BaseSpecies(), NormalForceSpecies(), FrictionForceSpecies(), AdhesiveForceSpecies()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "MixedSpecies::MixedSpecies() finished" << std::endl;
#endif
}

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::MixedSpecies(const MixedSpecies &s)
: BaseSpecies(s), NormalForceSpecies(s), FrictionForceSpecies(s), AdhesiveForceSpecies(s)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "MixedSpecies::MixedSpecies(const MixedSpecies &p) finished" << std::endl;
#endif
}

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::MixedSpecies(const Species<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies> &s)
: BaseSpecies(s), NormalForceSpecies(s), FrictionForceSpecies(s), AdhesiveForceSpecies(s)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "MixedSpecies::MixedSpecies(const MixedSpecies &p) finished" << std::endl;
#endif
}

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::~MixedSpecies()
{
#ifdef DEBUG_DESTRUCTOR
    std::cout << "MixedSpecies::~MixedSpecies() finished" << std::endl;
#endif
}

///MixedSpecies copy method. It calls to copy constructor of this MixedSpecies, useful for polymorphism
template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>* MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>
::copy() const
{
    return new MixedSpecies(*this);
}

/*!
 * \details It prints human readable MixedSpecies information to the output stream, 
 * typically to Files::restartFile::fstream_.
 * The basic species information is written in ParticleSpecies::write; 
 * then the three force types write additional information to the stream.
 * \param[out] os output stream (typically the restart file)
 */
template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
void MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>
::write(std::ostream& os) const
{
    os << getName();
    os << " idA " << BaseObject::getId();
    os << " idB " << BaseObject::getIndex();
    NormalForceSpecies::write(os);
    FrictionForceSpecies::write(os);
    AdhesiveForceSpecies::write(os);
}

/*!
 * Called by SpeciesHandler::readObject
 * \param[in] is input stream (typically the restart file)
 */
template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
void MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>
::read(std::istream& is)
{
    //note: name is already read by SpeciesHandler::readObject
    std::string dummy;
    unsigned int id, index;
    is >> dummy >> id;
    is >> dummy >> index;
    BaseObject::setId(id);
    BaseObject::setIndex(index);
    NormalForceSpecies::read(is);
    FrictionForceSpecies::read(is);
    AdhesiveForceSpecies::read(is);
}

/*!
 * \details Returns the name of the MixedSpecies as it is used in the restart file. 
 * The name of the species is a concatenation of the names of the three force  
 * components, e.g.
 * > MixedSpecies<LinearViscoelasticNormalSpecies,SlidingFrictionSpecies,ReversibleAdhesiveSpecies> species;
 * > std::cout << species.getName();
 * will output "LinearViscoelasticSlidingFrictionReversibleAdhesiveMixedSpecies".
 * The EmptyFrictionSpecies and the EmptyAdhesiveSpecies return empty strings, such that 
 * > MixedSpecies<LinearViscoelasticNormalSpecies> species;
 * > std::cout << species.getName();
 * will output "LinearViscoelasticMixedSpecies".
 * 
 * \return The name of the MixedSpecies.
 */
template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
std::string MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::getName() const
{
    return NormalForceSpecies::getBaseName()
        + FrictionForceSpecies::getBaseName()
        + AdhesiveForceSpecies::getBaseName() + "MixedSpecies";
}

/*!
 * \details The input parameters of this function are directly passed into the constructor for the new interaction.
 * See Interaction for details.
 * \param[in] P first of the two objects that interact
 * \param[in] I second of the two objects that interact
 * \param[in] timestamp current value of DPMBase::time_
 * \return pointer to the newly created Interaction.
 */
template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
BaseInteraction* MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::getNewInteraction(BaseInteractable* P, BaseInteractable* I, Mdouble timeStamp)
{
    return new Interaction<typename NormalForceSpecies::InteractionType, typename FrictionForceSpecies::InteractionType, typename AdhesiveForceSpecies::InteractionType > (P, I, timeStamp);
}

/*!
 * \details Returns true for any FrictionForceSpecies except EmptyFrictionSpecies, 
 * because for spherical particles, torques are only caused by tangential forces. 
 * See SpeciesHandler::useAngularDOFs for more details
 * \return true iff torques have to be calculated
 */
template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
bool MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::getUseAngularDOFs() const
{
    return FrictionForceSpecies::getUseAngularDOFs();
}

/*!
 * \details Uses the harmonic mean for most properties. Calls the mix function 
 * for each of the force species from which MixedSpecies is derived.
 * \param[in] S the first  of two species whose properties are mixed to create the new species
 * \param[in] T the second of two species whose properties are mixed to create the new species
 */
template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
void MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::mixAll(BaseSpecies * const S, BaseSpecies * const T)
{
    NormalForceSpecies::mix(dynamic_cast<NormalForceSpecies*> (S), dynamic_cast<NormalForceSpecies*> (T));
    FrictionForceSpecies::mix(dynamic_cast<FrictionForceSpecies*> (S), dynamic_cast<FrictionForceSpecies*> (T));
    AdhesiveForceSpecies::mix(dynamic_cast<AdhesiveForceSpecies*> (S), dynamic_cast<AdhesiveForceSpecies*> (T));
}

///Returns the particle distance below which adhesive forces can occur (needed for contact detection)

template<class NormalForceSpecies, class FrictionForceSpecies, class AdhesiveForceSpecies>
Mdouble MixedSpecies<NormalForceSpecies, FrictionForceSpecies, AdhesiveForceSpecies>::getInteractionDistance() const
{
    return AdhesiveForceSpecies::getInteractionDistance();
}
#endif
