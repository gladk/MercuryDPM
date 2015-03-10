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

///Constructor of the SpeciesHandler class. It creates and empty SpeciesHandler.
SpeciesHandler::SpeciesHandler()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "SpeciesHandler::SpeciesHandler() finished" << std::endl;
#endif
}

/// \param[in] WH The SpeciesHandler that has to be copied.
SpeciesHandler::SpeciesHandler(const SpeciesHandler &other)
//    : BaseHandler<ParticleSpecies>()
{
    clear();
    setDPMBase(other.getDPMBase());
    copyContentsFromOtherHandler(other);
//    mixedObjects_ = other.getMixedObjects();
    /* Instead of copying over the mixed objects pointers we reconstruct them.
    ** Currently probably leaking memory of the old ones in the process. Who's
    ** responsible for handling the old species? @dducks
    ** Jira #MDPM-71
    */
    mixedObjects_.clear();
    for (BaseSpecies * mixSpec : other.mixedObjects_) {
      mixedObjects_.push_back(mixSpec->copy());
      mixedObjects_.back()->setHandler(this);
    }
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "SpeciesHandler::SpeciesHandler(const SpeciesHandler &other) finished" << std::endl;
#endif
}

/// \param[in] rhs The BoundaryHandler on the right hand side of the assignment.
SpeciesHandler SpeciesHandler::operator =(const SpeciesHandler& rhs)
{
    if (this != &rhs)
    {
        clear();
        copyContentsFromOtherHandler(rhs);
    //    mixedObjects_ = rhs.getMixedObjects();
        //POTENTIAL LEAK HERE: Jira bug #MDPM-71
        //Who is responsible for memory mgmt of species?
        mixedObjects_.clear();
        for (BaseSpecies * mixSpec : rhs.mixedObjects_) {
            mixedObjects_.push_back(mixSpec->copy());
            mixedObjects_.back()->setHandler(this);
        }
    }

#ifdef DEBUG_CONSTRUCTOR
    std::cout << "SpeciesHandler SpeciesHandler::operator =(const SpeciesHandler& rhs)" << std::endl;
#endif
    ///\todo TW: the assignment operator doesn't copy the mixed objects
    return *this;
}

SpeciesHandler::~SpeciesHandler()
{
    for (BaseSpecies* o : mixedObjects_)
        delete o;
    mixedObjects_.clear();
#ifdef DEBUG_DESTRUCTOR
    std::cout << "SpeciesHandler::~SpeciesHandler() finished" << std::endl;
#endif
}

/// \param[in] is The input stream from which the information is read.
void SpeciesHandler::readObject(std::istream& is)
{
    std::string type;
    is >> type;
    if (type.compare("LinearViscoelasticSpecies") == 0)
    {
        LinearViscoelasticSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type.compare("LinearPlasticViscoelasticSpecies") == 0)
    {
        LinearPlasticViscoelasticSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type.compare("LinearViscoelasticSlidingFrictionSpecies") == 0)
    {
        LinearViscoelasticSlidingFrictionSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type.compare("LinearPlasticViscoelasticSlidingFrictionSpecies") == 0)
    {
        LinearPlasticViscoelasticSlidingFrictionSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type.compare("LinearViscoelasticFrictionSpecies") == 0)
    {
        LinearViscoelasticFrictionSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type.compare("LinearPlasticViscoelasticFrictionSpecies") == 0)
    {
        LinearPlasticViscoelasticFrictionSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type.compare("LinearViscoelasticIrreversibleAdhesiveSpecies") == 0)
    {
        LinearViscoelasticIrreversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type.compare("LinearPlasticViscoelasticIrreversibleAdhesiveSpecies") == 0)
    {
        LinearPlasticViscoelasticIrreversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type.compare("LinearViscoelasticSlidingFrictionIrreversibleAdhesiveSpecies") == 0)
    {
        LinearViscoelasticSlidingFrictionIrreversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type.compare("LinearPlasticViscoelasticSlidingFrictionIrreversibleAdhesiveSpecies") == 0)
    {
        LinearPlasticViscoelasticSlidingFrictionIrreversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type.compare("LinearViscoelasticFrictionIrreversibleAdhesiveSpecies") == 0)
    {
        LinearViscoelasticFrictionIrreversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type.compare("LinearPlasticViscoelasticFrictionIrreversibleAdhesiveSpecies") == 0)
    {
        LinearPlasticViscoelasticFrictionIrreversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }        
    else if (type.compare("LinearViscoelasticReversibleAdhesiveSpecies") == 0)
    {
        LinearViscoelasticReversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type.compare("LinearPlasticViscoelasticReversibleAdhesiveSpecies") == 0)
    {
        LinearPlasticViscoelasticReversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type.compare("LinearViscoelasticSlidingFrictionReversibleAdhesiveSpecies") == 0)
    {
        LinearViscoelasticSlidingFrictionReversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type.compare("LinearPlasticViscoelasticSlidingFrictionReversibleAdhesiveSpecies") == 0)
    {
        LinearPlasticViscoelasticSlidingFrictionReversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type.compare("LinearViscoelasticFrictionReversibleAdhesiveSpecies") == 0)
    {
        LinearViscoelasticFrictionReversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type.compare("LinearPlasticViscoelasticFrictionReversibleAdhesiveSpecies") == 0)
    {
        LinearPlasticViscoelasticFrictionReversibleAdhesiveSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type.compare("LinearViscoelasticFrictionLiquidBridgeWilletSpecies") == 0)
    {
        LinearViscoelasticFrictionLiquidBridgeWilletSpecies species;
        is >> species;
        copyAndAddObject(species);
    }
    else if (type.compare("k") == 0) //for backwards compatibility
    {
        addObject(readOldObject(is));
    }
    else
    {
        std::cerr << "Species type: " << type << " not understood in restart file" << std::endl;
        std::stringstream line(std::stringstream::in | std::stringstream::out);
        helpers::getLineFromStringStream(is, line);
        std::cout << line.str() << std::endl;
        exit(-1);
    }

    //remove the default mixed species
    for (unsigned int i =0; i+1 < getNumberOfObjects(); i++)
    {
        ///\todo TW why does deleting these objects create a segmentation fault
        delete mixedObjects_.back();
        mixedObjects_.pop_back();
    }

    for (unsigned int i = 0; i+1 < getNumberOfObjects(); i++)
    {
        is >> type;
        if (type.compare("LinearViscoelasticMixedSpecies") == 0)
        {
            LinearViscoelasticMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type.compare("LinearPlasticViscoelasticMixedSpecies") == 0)
        {
            LinearPlasticViscoelasticMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type.compare("LinearViscoelasticSlidingFrictionMixedSpecies") == 0)
        {
            LinearViscoelasticSlidingFrictionMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type.compare("LinearPlasticViscoelasticSlidingFrictionMixedSpecies") == 0)
        {
            LinearPlasticViscoelasticSlidingFrictionMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type.compare("LinearViscoelasticFrictionMixedSpecies") == 0)
        {
            LinearViscoelasticFrictionMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type.compare("LinearPlasticViscoelasticFrictionMixedSpecies") == 0)
        {
            LinearPlasticViscoelasticFrictionMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type.compare("LinearViscoelasticIrreversibleAdhesiveMixedSpecies") == 0)
        {
            LinearViscoelasticIrreversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type.compare("LinearPlasticViscoelasticIrreversibleAdhesiveMixedSpecies") == 0)
        {
            LinearPlasticViscoelasticIrreversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type.compare("LinearViscoelasticSlidingFrictionIrreversibleAdhesiveMixedSpecies") == 0)
        {
            LinearViscoelasticSlidingFrictionIrreversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type.compare("LinearPlasticViscoelasticSlidingFrictionIrreversibleAdhesiveMixedSpecies") == 0)
        {
            LinearPlasticViscoelasticSlidingFrictionIrreversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type.compare("LinearViscoelasticFrictionIrreversibleAdhesiveMixedSpecies") == 0)
        {
            LinearViscoelasticFrictionIrreversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type.compare("LinearPlasticViscoelasticFrictionIrreversibleAdhesiveMixedSpecies") == 0)
        {
            LinearPlasticViscoelasticFrictionIrreversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }        
        else if (type.compare("LinearViscoelasticReversibleAdhesiveMixedSpecies") == 0)
        {
            LinearViscoelasticReversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type.compare("LinearPlasticViscoelasticReversibleAdhesiveMixedSpecies") == 0)
        {
            LinearPlasticViscoelasticReversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type.compare("LinearViscoelasticSlidingFrictionReversibleAdhesiveMixedSpecies") == 0)
        {
            LinearViscoelasticSlidingFrictionReversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type.compare("LinearPlasticViscoelasticSlidingFrictionReversibleAdhesiveMixedSpecies") == 0)
        {
            LinearPlasticViscoelasticSlidingFrictionReversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type.compare("LinearViscoelasticFrictionReversibleAdhesiveMixedSpecies") == 0)
        {
            LinearViscoelasticFrictionReversibleAdhesiveMixedSpecies species;
            is >> species;
            mixedObjects_.push_back(species.copy());
        }
        else if (type.compare("LinearPlasticViscoelasticFrictionReversibleAdhesiveMixedSpecies") == 0)
        {
            LinearPlasticViscoelasticFrictionReversibleAdhesiveMixedSpecies species;
            is >> species;
        }
        else if (type.compare("LinearViscoelasticFrictionLiquidBridgeWilletMixedSpecies") == 0)
        {
            LinearViscoelasticFrictionLiquidBridgeWilletMixedSpecies species;
            is >> species;
        }
        else if (type.compare("k") == 0) //for backwards compatibility
        {
            mixedObjects_.push_back(readOldObject(is));
        }
        else
        {
            std::cerr << "Species type: " << type << " not understood in restart file" << std::endl;
            exit(-1);
        }
    }
}

/// \param[in] is The input stream from which the information is read.
ParticleSpecies* SpeciesHandler::readOldObject(std::istream& is)
{
    //read in next line
    std::stringstream line(std::stringstream::in | std::stringstream::out);
    helpers::getLineFromStringStream(is, line);

    //read out each property
    std::string dummy;
    Mdouble density, particleDimension, stiffness, dissipation, slidingFrictionCoefficient, slidingFrictionCoefficientStatic=0, slidingStiffness, slidingDissipation;
    line >> stiffness;
    while (true)
    {
        line >> dummy;
        if (!dummy.compare("disp"))
            line >> dissipation;
        else if (!dummy.compare("rho"))
            line >> density;
        else if (!dummy.compare("kt"))
            line >> slidingStiffness;
        else if (!dummy.compare("dispt"))
            line >> slidingDissipation;
        else if (!dummy.compare("mu"))
            line >> slidingFrictionCoefficient;
        else if (!dummy.compare("mus"))
            line >> slidingFrictionCoefficientStatic;
        else if (!dummy.compare("dim_particle"))
        {
            line >> particleDimension;
            getDPMBase()->setParticleDimensions(particleDimension);
        }
        else if (!dummy.compare("(mixed)"))
        {
            density = 0;
        }
        else
        {
            std::cerr << "Warning: " << dummy << "is not a species property" << std::endl;
            break;
        }
        if (line.eof())
            break;
    }

    //create the correct species
    if (slidingFrictionCoefficient==0)
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
        if (slidingFrictionCoefficientStatic==0)
            slidingFrictionCoefficientStatic = slidingFrictionCoefficient;
        species->setSlidingFrictionCoefficientStatic(slidingFrictionCoefficientStatic);
        return species;
    }
}


unsigned int SpeciesHandler::getMixedId(const unsigned int id1, const unsigned int id2) const
{
    unsigned int maxId = std::max(id1,id2);
    return (maxId*(maxId-1))/2 + std::min(id1,id2);
}

template<class U> typename U::MixedSpeciesType* SpeciesHandler::getMixedObject(const U* S, const U* T)
{
    return dynamic_cast<typename U::MixedSpeciesType*>(getMixedObject(S->getIndex(),T->getIndex()));
}

const std::vector<BaseSpecies*>& SpeciesHandler::getMixedObjects() const
{
    return mixedObjects_;
}

BaseSpecies* SpeciesHandler::getMixedObject(const unsigned int id1, const unsigned int id2)
{
    if (id1==id2)
        return getObject(id1);
    else
    {
        if (std::max(id1,id2) >= getNumberOfObjects())
        {
            std::cerr << "In: Object* SpeciesHandler::getMixedObject(const unsigned int id) const" << std::endl;
            std::cerr << "No Object exist with index " << std::max(id1,id2) << " number of objects is " << getNumberOfObjects() << std::endl;
            throw;
        }
        else
        {
            return mixedObjects_[getMixedId(id1,id2)];
        }        
    }
}

void SpeciesHandler::addObject(ParticleSpecies* const O)
{
    BaseHandler<ParticleSpecies>::addObject(O);
    std::cout << "Part / Mix: "
              << objects_.size() << " / "
              << mixedObjects_.size() << std::endl;
    for (unsigned int id =0; id+1 < getNumberOfObjects(); id++)
    {
        mixedObjects_.push_back(O->copyMixed());
        mixedObjects_.back()->setIndex(id);
        mixedObjects_.back()->setId(getNumberOfObjects()-1);
        mixedObjects_.back()->mix(O,getObject(id));
    }
    O->setHandler(this);
    getDPMBase()->particleHandler.computeAllMasses(O->getIndex());
	getDPMBase()->setRotation(useAngularDOFs());
}

void SpeciesHandler::removeObject(unsigned const int id)
{
    BaseHandler<ParticleSpecies>::removeObject(id);
    for (unsigned int id2 =0; id2 < getNumberOfObjects(); id2++)
    {
        mixedObjects_.erase(mixedObjects_.begin()+getMixedId(id, id2));
    }
	getDPMBase()->setRotation(useAngularDOFs());
}

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
    
    for (ParticleSpecies * spec : objects_) {
      os << *spec << std::endl;
    }
    for (BaseSpecies * spec : mixedObjects_) {
       os << *spec << std::endl;
    }
    */
}

std::string SpeciesHandler::getName() const
{
    return "SpeciesHandler";
}

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

//instantiate templated munctions
template LinearViscoelasticSpecies::MixedSpeciesType * SpeciesHandler::getMixedObject(const LinearViscoelasticSpecies*,const LinearViscoelasticSpecies*);
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
