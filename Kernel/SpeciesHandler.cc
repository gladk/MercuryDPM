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


#include "SpeciesHandler.h"
#include "DPMBase.h"

#include "Species/LinearViscoelasticSpecies.h"
#include "Species/LinearPlasticViscoelasticSpecies.h"
#include "Species/LinearViscoelasticFrictionSpecies.h"
#include "Species/LinearViscoelasticSlidingFrictionSpecies.h"
#include "Species/LinearPlasticViscoelasticFrictionSpecies.h"
#include "Species/LinearPlasticViscoelasticSlidingFrictionSpecies.h"

#include "Species/LinearViscoelasticIrreversibleAdhesiveSpecies.h"
#include "Species/LinearPlasticViscoelasticIrreversibleAdhesiveSpecies.h"
#include "Species/LinearViscoelasticFrictionIrreversibleAdhesiveSpecies.h"
#include "Species/LinearViscoelasticSlidingFrictionIrreversibleAdhesiveSpecies.h"
#include "Species/LinearPlasticViscoelasticFrictionIrreversibleAdhesiveSpecies.h"
#include "Species/LinearPlasticViscoelasticSlidingFrictionIrreversibleAdhesiveSpecies.h"

#include "Species/LinearViscoelasticReversibleAdhesiveSpecies.h"
#include "Species/LinearPlasticViscoelasticReversibleAdhesiveSpecies.h"
#include "Species/LinearViscoelasticFrictionReversibleAdhesiveSpecies.h"
#include "Species/LinearViscoelasticSlidingFrictionReversibleAdhesiveSpecies.h"
#include "Species/LinearPlasticViscoelasticFrictionReversibleAdhesiveSpecies.h"
#include "Species/LinearPlasticViscoelasticSlidingFrictionReversibleAdhesiveSpecies.h"

#include "Species/LinearViscoelasticFrictionLiquidBridgeWilletSpecies.h"

/*!
 * \details Constructor of the SpeciesHandler class. It creates an empty SpeciesHandler.
 */
SpeciesHandler::SpeciesHandler()
{
    logger(DEBUG, "SpeciesHandler::SpeciesHandler() finished");
}

/*!
 * \param[in] other The SpeciesHandler that has to be copied.
 * \details This is not a copy constructor! This constructor copies only all 
 *          BaseSpecies and MixedSpecies and copies the pointer to the DPMBase. 
 *          It sets all other data members to 0 or nullptr.
 */
SpeciesHandler::SpeciesHandler(const SpeciesHandler& other)
{
    clear();
    setDPMBase(other.getDPMBase());
    copyContentsFromOtherHandler(other);
    for (BaseSpecies* mixSpec : other.mixedObjects_) 
    {
      mixedObjects_.push_back(mixSpec->copy());
      mixedObjects_.back()->setHandler(this);
    }
    logger(DEBUG, "SpeciesHandler::SpeciesHandler(const SpeciesHandler &other) finished");
}

/*!
 * \param[in] rhs The BoundaryHandler on the right hand side of the assignment.
 * \return The SpeciesHandler that is a copy of the input SpeciesHandler rhs.
 * \details This is not a copy assignment operator! It copies only all 
 *          BaseSpecies and MixedSpecies and copies the pointer to the DPMBase. 
 *          It sets all other data members to 0 or nullptr.
 */
SpeciesHandler SpeciesHandler::operator =(const SpeciesHandler& rhs)
{
    if (this != &rhs)
    {
        clear();
        copyContentsFromOtherHandler(rhs);
        for (BaseSpecies* mixSpec : mixedObjects_)
        {
            delete mixSpec;
        }
        mixedObjects_.clear();
        for (BaseSpecies* mixSpec : rhs.mixedObjects_) 
        {
            mixedObjects_.push_back(mixSpec->copy());
            mixedObjects_.back()->setHandler(this);
        }
    }

    logger(DEBUG, "SpeciesHandler SpeciesHandler::operator =(const SpeciesHandler& rhs)");
    return *this;
}
/*!
 * \details Destructor: first destroys the objects of the BaseHandler, then destroys the mixedObjects
 */
SpeciesHandler::~SpeciesHandler()
{
    clear(); //this deletes everything that comes from the BaseHandler.
    /*for (BaseSpecies* o : mixedObjects_)
    {
        delete o;
    }*/
    for (BaseSpecies* mixSpec : mixedObjects_)
    {
        delete mixSpec;
    }
    mixedObjects_.clear();
    logger(DEBUG, "SpeciesHandler::~SpeciesHandler() finished");
}
/*!
 * \param[in] is The input stream from which the information is read.
 * \details First determine the type of the object we want to read, then read
 * the actual object. After that, clear the mixed objects and read the mixed objects.
 */ 
void SpeciesHandler::readObject(std::istream& is)
{
    std::string type;
    is >> type;
    if (type == "LinearViscoelasticSpecies")
    {
        LinearViscoelasticSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type == "LinearPlasticViscoelasticSpecies")
    {
        LinearPlasticViscoelasticSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type == "LinearViscoelasticSlidingFrictionSpecies")
    {
        LinearViscoelasticSlidingFrictionSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type == "LinearPlasticViscoelasticSlidingFrictionSpecies")
    {
        LinearPlasticViscoelasticSlidingFrictionSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type == "LinearViscoelasticFrictionSpecies")
    {
        LinearViscoelasticFrictionSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type == "LinearPlasticViscoelasticFrictionSpecies")
    {
        LinearPlasticViscoelasticFrictionSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type == "LinearViscoelasticIrreversibleAdhesiveSpecies")
    {
        LinearViscoelasticIrreversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type == "LinearPlasticViscoelasticIrreversibleAdhesiveSpecies")
    {
        LinearPlasticViscoelasticIrreversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type == "LinearViscoelasticSlidingFrictionIrreversibleAdhesiveSpecies")
    {
        LinearViscoelasticSlidingFrictionIrreversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type == "LinearPlasticViscoelasticSlidingFrictionIrreversibleAdhesiveSpecies")
    {
        LinearPlasticViscoelasticSlidingFrictionIrreversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type == "LinearViscoelasticFrictionIrreversibleAdhesiveSpecies")
    {
        LinearViscoelasticFrictionIrreversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type == "LinearPlasticViscoelasticFrictionIrreversibleAdhesiveSpecies")
    {
        LinearPlasticViscoelasticFrictionIrreversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }        
    else if (type == "LinearViscoelasticReversibleAdhesiveSpecies")
    {
        LinearViscoelasticReversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type == "LinearPlasticViscoelasticReversibleAdhesiveSpecies")
    {
        LinearPlasticViscoelasticReversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type == "LinearViscoelasticSlidingFrictionReversibleAdhesiveSpecies")
    {
        LinearViscoelasticSlidingFrictionReversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type == "LinearPlasticViscoelasticSlidingFrictionReversibleAdhesiveSpecies")
    {
        LinearPlasticViscoelasticSlidingFrictionReversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type == "LinearViscoelasticFrictionReversibleAdhesiveSpecies")
    {
        LinearViscoelasticFrictionReversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type == "LinearPlasticViscoelasticFrictionReversibleAdhesiveSpecies")
    {
        LinearPlasticViscoelasticFrictionReversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type == "LinearViscoelasticFrictionLiquidBridgeWilletSpecies")
    {
        LinearViscoelasticFrictionLiquidBridgeWilletSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type == "k") //for backwards compatibility
    {
        addObject(readOldObject(is));
    }
    else
    {        
        std::stringstream line(std::stringstream::in | std::stringstream::out);
        helpers::getLineFromStringStream(is, line);
        logger(ERROR, "Species type % not understood in restart file. %", type, line.str());
    }

    //remove the default mixed species
    for (unsigned int i = 0; i + 1 < getNumberOfObjects(); i++)
    {
        ///\todo TW why does deleting these objects create a segmentation fault
        ///How do you create the segmentation fault? \author weinhartt
        ///\todo IFCD how does the numbering of mixedSpecies_ work?
        ///the numbering of mixed species is 01, 02, 12, 03, 13, 23, 04, 14, 24, 34, 
        ///i.e. if you add the n-th ParticleSpecies, then you have to add n-1 MixedSpecies. 
        ///So here I remove the last n-1 MixedSpecies and add n-1 new ones. \author weinhartt 
        delete mixedObjects_.back();
        mixedObjects_.pop_back();
    }

    //Read the mixed species.
    for (unsigned int i = 0; i+1 < getNumberOfObjects(); i++)
    {
        is >> type;
        if (type == "LinearViscoelasticMixedSpecies")
        {
            LinearViscoelasticMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type == "LinearPlasticViscoelasticMixedSpecies")
        {
            LinearPlasticViscoelasticMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type == "LinearViscoelasticSlidingFrictionMixedSpecies")
        {
            LinearViscoelasticSlidingFrictionMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type == "LinearPlasticViscoelasticSlidingFrictionMixedSpecies")
        {
            LinearPlasticViscoelasticSlidingFrictionMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type == "LinearViscoelasticFrictionMixedSpecies")
        {
            LinearViscoelasticFrictionMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type == "LinearPlasticViscoelasticFrictionMixedSpecies")
        {
            LinearPlasticViscoelasticFrictionMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type == "LinearViscoelasticIrreversibleAdhesiveMixedSpecies")
        {
            LinearViscoelasticIrreversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type == "LinearPlasticViscoelasticIrreversibleAdhesiveMixedSpecies")
        {
            LinearPlasticViscoelasticIrreversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type == "LinearViscoelasticSlidingFrictionIrreversibleAdhesiveMixedSpecies")
        {
            LinearViscoelasticSlidingFrictionIrreversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type == "LinearPlasticViscoelasticSlidingFrictionIrreversibleAdhesiveMixedSpecies")
        {
            LinearPlasticViscoelasticSlidingFrictionIrreversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type == "LinearViscoelasticFrictionIrreversibleAdhesiveMixedSpecies")
        {
            LinearViscoelasticFrictionIrreversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type == "LinearPlasticViscoelasticFrictionIrreversibleAdhesiveMixedSpecies")
        {
            LinearPlasticViscoelasticFrictionIrreversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }        
        else if (type == "LinearViscoelasticReversibleAdhesiveMixedSpecies")
        {
            LinearViscoelasticReversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type == "LinearPlasticViscoelasticReversibleAdhesiveMixedSpecies")
        {
            LinearPlasticViscoelasticReversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type == "LinearViscoelasticSlidingFrictionReversibleAdhesiveMixedSpecies")
        {
            LinearViscoelasticSlidingFrictionReversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type == "LinearPlasticViscoelasticSlidingFrictionReversibleAdhesiveMixedSpecies")
        {
            LinearPlasticViscoelasticSlidingFrictionReversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type == "LinearViscoelasticFrictionReversibleAdhesiveMixedSpecies")
        {
            LinearViscoelasticFrictionReversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type == "LinearPlasticViscoelasticFrictionReversibleAdhesiveMixedSpecies")
        {
            LinearPlasticViscoelasticFrictionReversibleAdhesiveMixedSpecies species;
            is >> species;
        }
        else if (type == "LinearViscoelasticFrictionLiquidBridgeWilletMixedSpecies")
        {
            LinearViscoelasticFrictionLiquidBridgeWilletMixedSpecies species;
            is >> species;
        }
        else if (type == "k") //for backwards compatibility
        {
            mixedObjects_.push_back(readOldObject(is));
        }
        else
        {
            logger(ERROR, "Species type: % not understood in restart file", type);
        }
    }
}

/*!
 * \param[in] is The input stream from which the information is read.
 * \return A pointer to the ParticleSpecies that has just been read.
 * \details To read the old object, we first make a stringstream of the line that
 * describes this ParticleSpecies. After that, we read the properties one by one, 
 * first the stiffness and after that the other properties. We stop when we either
 * reach the end of the file(eof) or if a string is not recognized as a property.
 */
ParticleSpecies* SpeciesHandler::readOldObject(std::istream& is)
{
    //read in next line
    std::stringstream line(std::stringstream::in | std::stringstream::out);
    helpers::getLineFromStringStream(is, line);

    //read each property
    std::string property;
    unsigned int particleDimension=0;
    Mdouble density=0.0, stiffness=0.0, dissipation=0.0, slidingFrictionCoefficient=0.0, slidingFrictionCoefficientStatic=0.0, slidingStiffness=0.0, slidingDissipation=0.0;
    line >> stiffness;
    while (true)
    {
        line >> property;
        if (property == "disp")
            line >> dissipation;
        else if (property == "rho")
            line >> density;
        else if (property == "kt")
            line >> slidingStiffness;
        else if (property == "dispt")
            line >> slidingDissipation;
        else if (property == "mu")
            line >> slidingFrictionCoefficient;
        else if (property == "mus")
            line >> slidingFrictionCoefficientStatic;
        else if (property == "dim_particle")
        {
            line >> particleDimension;
            getDPMBase()->setParticleDimensions(particleDimension);
        }
        else if (property == "(mixed)")
        {
            density = 0;
        }
        else
        {
            logger(WARN, "Warning: % is not a species property", property);
            break;
        }
        if (line.eof())
            break;
    }

    //create the correct species
    if (slidingFrictionCoefficient == 0.0)
    {
        LinearViscoelasticSpecies* species = new LinearViscoelasticSpecies;
        species->setDensity(density);
        species->setStiffness(stiffness);
        species->setDissipation(dissipation);
        return species;
    }
    else
    {
        LinearViscoelasticSlidingFrictionSpecies* species = new LinearViscoelasticSlidingFrictionSpecies;
        species->setDensity(density);
        species->setStiffness(stiffness);
        species->setDissipation(dissipation);
        species->setSlidingStiffness(slidingStiffness);
        species->setSlidingDissipation(slidingDissipation);
        species->setSlidingFrictionCoefficient(slidingFrictionCoefficient);
        if (slidingFrictionCoefficientStatic == 0.0)
            slidingFrictionCoefficientStatic = slidingFrictionCoefficient;
        species->setSlidingFrictionCoefficientStatic(slidingFrictionCoefficientStatic);
        return species;
    }
}

/*!
 * \param[in] id1 Id of the first species. 
 * \param[in] id2 Id of the second species.
 * \return An unsigned integer that denotes the Id of the mixed species.
 * \details The numbering of the mixed species is 0-1,  0-2, 1-2,  0-3, 1-3, 2-3,  0-4, 1-4, 2-4, 3-4, ...,
 * where each pair of numbers a and b denotes the mixed species between ParticleSpecies a and b.
 * Thus, first compute which id has a higher value, the id of the mixed species
 * is then given by (maxId*(maxId-1))/2 + minId.
 */
unsigned int SpeciesHandler::getMixedId(const unsigned int id1, const unsigned int id2) const
{
    unsigned int maxId = std::max(id1,id2);
    return (maxId*(maxId-1))/2 + std::min(id1,id2);
}

/*!
 * \param[in] S A pointer to the first ParticleSpecies.
 * \param[in] T A pointer to the second ParticleSpecies.
 * \return A pointer to the MixedSpecies between the ParticleSpecies S and T.
 */
template<class U> typename U::MixedSpeciesType* SpeciesHandler::getMixedObject(const U* S, const U* T)
{
    return dynamic_cast<typename U::MixedSpeciesType*>(getMixedObject(S->getIndex(),T->getIndex()));
}

/*!
 * \return A reference to the vector of pointers of all the mixedObjects.
 */
const std::vector<BaseSpecies*>& SpeciesHandler::getMixedObjects() const
{
    return mixedObjects_;
}

/*!
 * \param[in] id1 Id of the first BaseSpecies.
 * \param[in] id2 Id of the second BaseSpecies.
 * \return A pointer to an object that is a MixedSpecies of both input Species.
 * \todo This function should probably be made private. The user should use the function
 * SpeciesHandler::getMixedObject(const U* S, const U* T), which deals with pointers.
 */
BaseSpecies* SpeciesHandler::getMixedObject(const unsigned int id1, const unsigned int id2)
{
    if (id1 == id2)
    {
      return getObject(id1);
    }
    else
    {
        if (std::max(id1,id2) >= getNumberOfObjects())
        {
            logger(ERROR, "In: Object* SpeciesHandler::getMixedObject(const unsigned int id) const. No Object exist with index %, number of objects is %", std::max(id1, id2), getNumberOfObjects());
            return nullptr;
        }
        else
        {
            return mixedObjects_[getMixedId(id1,id2)];
        }        
    }
}
/*!
 * \param[in] S A pointer to the ParticleSpecies that has to be added.
 * \details First, add the ParticleSpecies to the vector of ParticleSpecies (object_), 
 * then construct all MixedSpecies. Tell the ParticleSpecies that this is its 
 * handler and compute all masses and whether it should use angular degrees of freedom.
 * 
 * Note: The MixedSpecies objects are initialized with 
 * averaged values from both species: e.g., the mixedSpecies between Species A 
 * and B will have a stiffness \$fk=(1/k_a+1/k_b)^{-1}\$f, you have to change 
 * the MixedSpecies properties if you don't like these defaults.
 */
void SpeciesHandler::addObject(ParticleSpecies* const S)
{
    BaseHandler<ParticleSpecies>::addObject(S);
    //logger(INFO, "Part / Mix: % / %", objects_.size(), mixedObjects_.size());
    ///\todo TW don't put logger messages that only make sense for one application!
    for (unsigned int id = 0; id + 1 < getNumberOfObjects(); ++id)
    {
        mixedObjects_.push_back(S->copyMixed());
        mixedObjects_.back()->setIndex(id);
        mixedObjects_.back()->setId(getNumberOfObjects()-1);
        mixedObjects_.back()->mixAll(S,getObject(id));
    }
    S->setHandler(this);
    getDPMBase()->particleHandler.computeAllMasses(S->getIndex());
	getDPMBase()->setRotation(useAngularDOFs());
	getDPMBase()->setRotation(useAngularDOFs());
}

/*!
 * \param[in] id The identity of the ParticleSpecies that must be removed.
 * \details First remove the object itself, then remove all mixed species for this
 * ParticleSpecies.
 */
void SpeciesHandler::removeObject(unsigned const int id)
{
    BaseHandler<ParticleSpecies>::removeObject(id);
    for (unsigned int id2 = 0; id2 < getNumberOfObjects(); ++id2)
    {
        mixedObjects_.erase(mixedObjects_.begin()+getMixedId(id, id2));
    }
	getDPMBase()->setRotation(useAngularDOFs());
}
/*!
 * \param[in] os The output stream where the object needs to be written to.
 * \details First write "Species" and the amount of species in this handler,
 * then write all ParticleSpecies and MixedSpecies.
 */
void SpeciesHandler::write(std::ostream& os) const
{
    os << "Species " << getNumberOfObjects() << std::endl;
    std::vector<BaseSpecies*>::const_iterator it2 = mixedObjects_.begin();
    for (std::vector<ParticleSpecies*>::const_iterator it = begin(); it != end(); ++it)
    {
        os << (**it) << std::endl;
        for (unsigned int id2 =0; id2 < (*it)->getIndex(); id2++)
        {
            os << (**it2) << std::endl;
            it2++;
        }
    }
    /*
    This behaves like the code above but is actually readable
    @weinhartt is there a reason?
    
    there is a reason. Curernt architecture does not allow this
    \todo define new restart format. @dducks
    
    The code is written such that it writes PS0, then PS1 and MS01, then PS2 and 
    MS02, MS12, and so on (PS=ParticleSpecies, MS=MixedSpecies). We can change 
    the restart format, but we should write the read(os) function such that 
    it can also read the old format @weinhartt
    
    for (ParticleSpecies * spec : objects_) {
      os << *spec << std::endl;
    }
    for (BaseSpecies * spec : mixedObjects_) {
       os << *spec << std::endl;
    }
    */
}
/*!
 * \return The string "SpeciesHandler"
 */
std::string SpeciesHandler::getName() const
{
    return "SpeciesHandler";
}

/*!
 * \return The boolean which says whether or not AnuglarDOFs must be used in this handler.
 */
bool SpeciesHandler::useAngularDOFs()
{
    for (unsigned int i = 0; i < getNumberOfObjects(); i++)
    {
        if (getObject(i)->getUseAngularDOFs())
            return true;
        for (unsigned int j = 0; j + 1 < i; j++)
        {
            if (getMixedObject(i, j)->getUseAngularDOFs())
                return true;
        }
    }
    return false;
}

//instantiate templated functions
//template<class U> typename U::MixedSpeciesType* SpeciesHandler::getMixedObject(const U* S, const U* T)

template LinearViscoelasticSpecies::MixedSpeciesType* SpeciesHandler::getMixedObject(const LinearViscoelasticSpecies*,const LinearViscoelasticSpecies*);
template LinearPlasticViscoelasticSpecies::MixedSpeciesType* SpeciesHandler::getMixedObject(const LinearPlasticViscoelasticSpecies*,const LinearPlasticViscoelasticSpecies*);
template LinearViscoelasticSlidingFrictionSpecies::MixedSpeciesType* SpeciesHandler::getMixedObject(const LinearViscoelasticSlidingFrictionSpecies*,const LinearViscoelasticSlidingFrictionSpecies*);
template LinearPlasticViscoelasticSlidingFrictionSpecies::MixedSpeciesType* SpeciesHandler::getMixedObject(const LinearPlasticViscoelasticSlidingFrictionSpecies*,const LinearPlasticViscoelasticSlidingFrictionSpecies*);
template LinearViscoelasticFrictionSpecies::MixedSpeciesType* SpeciesHandler::getMixedObject(const LinearViscoelasticFrictionSpecies*,const LinearViscoelasticFrictionSpecies*);
template LinearPlasticViscoelasticFrictionSpecies::MixedSpeciesType* SpeciesHandler::getMixedObject(const LinearPlasticViscoelasticFrictionSpecies*,const LinearPlasticViscoelasticFrictionSpecies*);

template LinearViscoelasticIrreversibleAdhesiveSpecies::MixedSpeciesType * SpeciesHandler::getMixedObject(const LinearViscoelasticIrreversibleAdhesiveSpecies*,const LinearViscoelasticIrreversibleAdhesiveSpecies*);
template LinearPlasticViscoelasticIrreversibleAdhesiveSpecies::MixedSpeciesType* SpeciesHandler::getMixedObject(const LinearPlasticViscoelasticIrreversibleAdhesiveSpecies*,const LinearPlasticViscoelasticIrreversibleAdhesiveSpecies*);
template LinearViscoelasticSlidingFrictionIrreversibleAdhesiveSpecies::MixedSpeciesType* SpeciesHandler::getMixedObject(const LinearViscoelasticSlidingFrictionIrreversibleAdhesiveSpecies*,const LinearViscoelasticSlidingFrictionIrreversibleAdhesiveSpecies*);
template LinearPlasticViscoelasticSlidingFrictionIrreversibleAdhesiveSpecies::MixedSpeciesType* SpeciesHandler::getMixedObject(const LinearPlasticViscoelasticSlidingFrictionIrreversibleAdhesiveSpecies*,const LinearPlasticViscoelasticSlidingFrictionIrreversibleAdhesiveSpecies*);
template LinearViscoelasticFrictionIrreversibleAdhesiveSpecies::MixedSpeciesType* SpeciesHandler::getMixedObject(const LinearViscoelasticFrictionIrreversibleAdhesiveSpecies*,const LinearViscoelasticFrictionIrreversibleAdhesiveSpecies*);
template LinearPlasticViscoelasticFrictionIrreversibleAdhesiveSpecies::MixedSpeciesType* SpeciesHandler::getMixedObject(const LinearPlasticViscoelasticFrictionIrreversibleAdhesiveSpecies*,const LinearPlasticViscoelasticFrictionIrreversibleAdhesiveSpecies*);

template LinearViscoelasticReversibleAdhesiveSpecies::MixedSpeciesType * SpeciesHandler::getMixedObject(const LinearViscoelasticReversibleAdhesiveSpecies*,const LinearViscoelasticReversibleAdhesiveSpecies*);
template LinearPlasticViscoelasticReversibleAdhesiveSpecies::MixedSpeciesType* SpeciesHandler::getMixedObject(const LinearPlasticViscoelasticReversibleAdhesiveSpecies*,const LinearPlasticViscoelasticReversibleAdhesiveSpecies*);
template LinearViscoelasticSlidingFrictionReversibleAdhesiveSpecies::MixedSpeciesType* SpeciesHandler::getMixedObject(const LinearViscoelasticSlidingFrictionReversibleAdhesiveSpecies*,const LinearViscoelasticSlidingFrictionReversibleAdhesiveSpecies*);
template LinearPlasticViscoelasticSlidingFrictionReversibleAdhesiveSpecies::MixedSpeciesType* SpeciesHandler::getMixedObject(const LinearPlasticViscoelasticSlidingFrictionReversibleAdhesiveSpecies*,const LinearPlasticViscoelasticSlidingFrictionReversibleAdhesiveSpecies*);
template LinearViscoelasticFrictionReversibleAdhesiveSpecies::MixedSpeciesType* SpeciesHandler::getMixedObject(const LinearViscoelasticFrictionReversibleAdhesiveSpecies*,const LinearViscoelasticFrictionReversibleAdhesiveSpecies*);
template LinearPlasticViscoelasticFrictionReversibleAdhesiveSpecies::MixedSpeciesType* SpeciesHandler::getMixedObject(const LinearPlasticViscoelasticFrictionReversibleAdhesiveSpecies*,const LinearPlasticViscoelasticFrictionReversibleAdhesiveSpecies*);

template LinearViscoelasticFrictionLiquidBridgeWilletSpecies::MixedSpeciesType* SpeciesHandler::getMixedObject(const LinearViscoelasticFrictionLiquidBridgeWilletSpecies*,const LinearViscoelasticFrictionLiquidBridgeWilletSpecies*);
