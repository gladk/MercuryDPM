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

#include "SpeciesHandler.h"
#include "Species/BaseSpecies.h"
#include "Species/LinearViscoelasticSpecies.h"
#include "Species/LinearPlasticViscoelasticSpecies.h"
#include "Species/LinearViscoelasticFrictionSpecies.h"
#include "Species/LinearViscoelasticSlidingFrictionSpecies.h"
#include "Species/LinearPlasticViscoelasticFrictionSpecies.h"
#include "Species/LinearPlasticViscoelasticSlidingFrictionSpecies.h"
#include "DPMBase.h"

///Constructor of the SpeciesHandler class. It creates and empty SpeciesHandler.
SpeciesHandler::SpeciesHandler()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "SpeciesHandler::SpeciesHandler() finished" << std::endl;
#endif
}

/// \param[in] WH The SpeciesHandler that has to be copied.
SpeciesHandler::SpeciesHandler(const SpeciesHandler &other)
//    : BaseHandler<BaseSpecies>()
{
    clear();
    setDPMBase(other.getDPMBase());
    copyContentsFromOtherHandler(other);
    mixedObjects_ = other.getMixedObjects();
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "SpeciesHandler::SpeciesHandler(const SpeciesHandler &PH) finished" << std::endl;
#endif
}

/// \param[in] rhs The BoundaryHandler on the right hand side of the assignment.
SpeciesHandler SpeciesHandler::operator =(const SpeciesHandler& rhs)
{
    if (this != &rhs)
    {
        clear();
        copyContentsFromOtherHandler(rhs);
        mixedObjects_ = rhs.getMixedObjects();
    }
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "SpeciesHandler SpeciesHandler::operator =(const SpeciesHandler& rhs)" << std::endl;
#endif
    ///\todo TW: the assignment operator doesn't copy the mixed objects
    return *this;
}

SpeciesHandler::~SpeciesHandler()
{
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

    for (unsigned int i = 0; i+1 < getNumberOfObjects(); i++)
    {
        is >> type;
        if (type.compare("LinearViscoelasticSpecies") == 0)
        {
            ///\todo TW: should the pushback object be newed? I think so
            LinearViscoelasticSpecies species;
            is >> species;
            mixedObjects_.push_back(&species);
        }
        else if (type.compare("LinearPlasticViscoelasticSpecies") == 0)
        {
            LinearPlasticViscoelasticSpecies species;
            is >> species;
            mixedObjects_.push_back(&species);
        }
        else if (type.compare("LinearViscoelasticSlidingFrictionSpecies") == 0)
        {
            LinearViscoelasticSlidingFrictionSpecies species;
            is >> species;
            mixedObjects_.push_back(&species);
        }
        else if (type.compare("LinearPlasticViscoelasticSlidingFrictionSpecies") == 0)
        {
            LinearPlasticViscoelasticSlidingFrictionSpecies species;
            is >> species;
            mixedObjects_.push_back(&species);
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
BaseSpecies* SpeciesHandler::readOldObject(std::istream& is)
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

template<class U> U* SpeciesHandler::getMixedObject(const U* S, const U* T)
{
    return dynamic_cast<U*>(getMixedObject(S->getIndex(),T->getIndex()));
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

void SpeciesHandler::addObject(BaseSpecies* const O)
{
    BaseHandler<BaseSpecies>::addObject(O);
    for (unsigned int id =0; id+1 < getNumberOfObjects(); id++)
    {
        mixedObjects_.push_back(O->copy());
        mixedObjects_.back()->mix(O,getObject(id));
    }
    O->setHandler(this);
    getDPMBase()->particleHandler.computeAllMasses(O->getIndex());
}

void SpeciesHandler::removeObject(unsigned const int id)
{
    BaseHandler<BaseSpecies>::removeObject(id);
    for (unsigned int id2 =0; id2 < getNumberOfObjects(); id2++)
    {
        mixedObjects_.erase(mixedObjects_.begin()+getMixedId(id, id2));
    }
}

void SpeciesHandler::write(std::ostream& os) const
{
    os << "Species " << getNumberOfObjects() << std::endl;
    std::vector<BaseSpecies*>::const_iterator it2 = mixedObjects_.begin();
    for (std::vector<BaseSpecies*>::const_iterator it = begin(); it != end(); ++it)
    {
        os << (**it) << std::endl;
        for (unsigned int id2 =0; id2 < (*it)->getIndex(); id2++)
        {
            os << (**it2) << std::endl;
            it2++;
        }
    }
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
template LinearViscoelasticSpecies* SpeciesHandler::getMixedObject(const LinearViscoelasticSpecies*,const LinearViscoelasticSpecies*);
template LinearPlasticViscoelasticSpecies* SpeciesHandler::getMixedObject(const LinearPlasticViscoelasticSpecies*,const LinearPlasticViscoelasticSpecies*);
template LinearViscoelasticSlidingFrictionSpecies* SpeciesHandler::getMixedObject(const LinearViscoelasticSlidingFrictionSpecies*,const LinearViscoelasticSlidingFrictionSpecies*);
template LinearPlasticViscoelasticSlidingFrictionSpecies* SpeciesHandler::getMixedObject(const LinearPlasticViscoelasticSlidingFrictionSpecies*,const LinearPlasticViscoelasticSlidingFrictionSpecies*);
template LinearViscoelasticFrictionSpecies* SpeciesHandler::getMixedObject(const LinearViscoelasticFrictionSpecies*,const LinearViscoelasticFrictionSpecies*);
template LinearPlasticViscoelasticFrictionSpecies* SpeciesHandler::getMixedObject(const LinearPlasticViscoelasticFrictionSpecies*,const LinearPlasticViscoelasticFrictionSpecies*);
