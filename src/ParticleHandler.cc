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

#include <limits>

#include "ParticleHandler.h"
#include "Particles/BaseParticle.h"
#include "Particles/DeltaMaxsParticle.h"
#include "Particles/TangentialSpringParticle.h"

///Constructor of the ParticleHandler class. It creates and empty ParticleHandler.
ParticleHandler::ParticleHandler() : BaseHandler<BaseParticle>()
{
    species_=0;
    largestParticle_=0;
    smallestParticle_=0;
    #ifdef CONSTUCTOR_OUTPUT
        std::cerr << "ParticleHandler() finished" << std::endl;
    #endif
}

/// \param[in] PH The ParticleHandler that has te be copied.
ParticleHandler::ParticleHandler(const ParticleHandler &PH)
{
    species_=PH.species_;
    clear();
    for (std::vector<BaseParticle*>::const_iterator it=PH.begin();it!=PH.end();it++)
    {
        addObject((*it)->copy());
    }    
    #ifdef CONSTUCTOR_OUTPUT
        std::cerr << "ParticleHandler(const ParticleHandler &PH) finished" << std::endl;
    #endif
}

/// \param[in] rhs The ParticleHandler on the right hand side of the assignment.
ParticleHandler ParticleHandler::operator = (const ParticleHandler& rhs)
{
    if (this != &rhs)
    {
        clear();
        for (std::vector<BaseParticle*>::const_iterator it=rhs.begin();it!=rhs.end();it++)
        {
            addObject((*it)->copy());
        }
    }
    #ifdef CONSTUCTOR_OUTPUT
        std::cerr << "ParticleHandler operator = (const ParticleHandler& rhs) finished" << std::endl;
    #endif
    return *this;
}

ParticleHandler::~ParticleHandler()
{
    #ifdef DESTRUCTOR_OUTPUT
        std::cout << "WallHandler::~WallHandler() finished" << std::endl;
    #endif
}

/// \param[in] P A pointer to the BaseParticle (or derived class) that has to be added.
/// \todo Also insert the particle in the HGRID
void ParticleHandler::addObject(BaseParticle* P)
{
    //Puts the particle in the Particle list
    BaseHandler<BaseParticle>::addObject(P);
    //Check if this particle has new extrema
    checkExtrema(getLastObject());
    //set the particleHandler pointer
    getLastObject()->setHandler(this);
}

/// \return A pointer to the to the smallest BaseParticle (by interactionRadius) in this ParticleHandler.
BaseParticle* ParticleHandler::getSmallestParticle() const{
    if (!smallestParticle_)
    {
        std::cerr << "Warning: No particles to set get_SmallestParticle()" << std::endl;
    }
    return smallestParticle_;
}

/// \return A pointer to the to the fastest BaseParticle in this ParticleHandler.
BaseParticle* ParticleHandler::getFastestParticle() const
{
    if (!getNumberOfObjects())
    {
        std::cerr << "Warning: No particles to set get_FastestParticle()" << std::endl;
    }
    BaseParticle* p = 0;
    Mdouble maxSpeed = -std::numeric_limits<Mdouble>::max();
    for (std::vector<BaseParticle*>::const_iterator it=begin();it!=end();it++)
    {
        if ((*it)->get_Velocity().GetLength()>maxSpeed)
        {
            maxSpeed=(*it)->get_Velocity().GetLength();
            p = (*it);
        }
    }
    return p;
}

BaseParticle* ParticleHandler::getLowestPositionComponentParticle(const int i) const
{
    if (!getNumberOfObjects())
    {
        std::cerr << "Warning: No getLowestPositionComponentParticle(cons int i)" << std::endl;
    }
    BaseParticle* p = 0;
    Mdouble min = std::numeric_limits<Mdouble>::max();
    for (std::vector<BaseParticle*>::const_iterator it=begin();it!=end();it++)
    {
        if ((*it)->get_Position().getComponent(i)<min) {
            min=(*it)->get_Position().getComponent(i);
            p = (*it);
        }
    }
    return p;
}
BaseParticle* ParticleHandler::getHighestPositionComponentParticle(const int i) const
{
    if (!getNumberOfObjects())
    {
        std::cerr << "Warning: No getHighestPositionComponentParticle(cons int i)" << std::endl;
    }
    BaseParticle* p = 0;
    Mdouble max = -std::numeric_limits<Mdouble>::max();
    for (std::vector<BaseParticle*>::const_iterator it=begin();it!=end();it++)
    {
        if ((*it)->get_Position().getComponent(i)>max) {
            max=(*it)->get_Position().getComponent(i);
            p = (*it);
        }
    }
    return p;
}
BaseParticle* ParticleHandler::getLowestVelocityComponentParticle(const int i) const
{
    if (!getNumberOfObjects())
    {
        std::cerr << "Warning: No getLowestVelocityComponentParticle(cons int i)" << std::endl;
    }
    BaseParticle* p = 0;
    Mdouble min = std::numeric_limits<Mdouble>::max();
    for (std::vector<BaseParticle*>::const_iterator it=begin();it!=end();it++)
    {
        if ((*it)->get_Velocity().getComponent(i)<min) {
            min=(*it)->get_Velocity().getComponent(i);
            p = (*it);
        }
    }
    return p;
}
BaseParticle* ParticleHandler::getHighestVelocityComponentParticle(const int i) const
{
    if (!getNumberOfObjects())
    {
        std::cerr << "Warning: No getHighestVelocityComponentParticle(cons int i)" << std::endl;
    }
    BaseParticle* p = 0;
    Mdouble max = -std::numeric_limits<Mdouble>::max();
    for (std::vector<BaseParticle*>::const_iterator it=begin();it!=end();it++)
    {
        if ((*it)->get_Velocity().getComponent(i)>max) {
            max=(*it)->get_Velocity().getComponent(i);
            p = (*it);
        }
    }
    return p;
}

/// \return A pointer to the to the lightest BaseParticle (by mass) in this ParticleHandler.
BaseParticle* ParticleHandler::getLightestParticle() const{
    if (!getNumberOfObjects())
    {
        std::cerr << "Warning: No particles to set getLightestParticle()" << std::endl;
        throw;
    }
    BaseParticle* p = 0;
    Mdouble minMass = std::numeric_limits<Mdouble>::max();
    for (std::vector<BaseParticle*>::const_iterator it=begin();it!=end();it++)
    {
        if ((*it)->get_Mass()<minMass)
        {
            minMass=(*it)->get_Mass();
            p = (*it);
        }
    }
    return p;
}

/// \return A pointer to the to the largest BaseParticle (by interactionRadius) in this ParticleHandler.
BaseParticle* ParticleHandler::getLargestParticle() const{
    if (!largestParticle_)
    {
        std::cerr << "Warning: No particles to set get_LargestParticle()" << std::endl;
        throw;
    }
    return largestParticle_;
}

void ParticleHandler::clear()
{
    smallestParticle_=0;
    largestParticle_=0;
    BaseHandler<BaseParticle>::clear();
}

/// \param[in] is The input stream from which the information is read.
void ParticleHandler::readObject(std::istream& is)
{
    std::string type;
    is >> type;
    if(type.compare("BP")==0)
    {
        BaseParticle baseParticle;
        is>>baseParticle;
        copyAndAddObject(baseParticle);
    }
    else if(type.compare("TSP")==0)
    {
        TangentialSpringParticle tangentialSpringParticle;
        is>>tangentialSpringParticle;
        copyAndAddObject(tangentialSpringParticle);
    }
    else if(type.compare("DMP")==0)
    {
        DeltaMaxsParticle deltaMaxsParticle;
        is>>deltaMaxsParticle;
        copyAndAddObject(deltaMaxsParticle);
    }  
    else if(isdigit(type[0]))
    {
        TangentialSpringParticle tangentialSpringParticle;
        tangentialSpringParticle.oldRead(is,type);
        copyAndAddObject(tangentialSpringParticle);		
	}
    else
    {
        std::cerr<<"Particle type: "<<type<<" not understood in restart file"<<std::endl;
        exit(-1);
    }
}

/// \param[in] P A pointer to the particle, which properties have to be checked against the ParticleHandlers extrema.
void ParticleHandler::checkExtrema(BaseParticle* P)
{
    if(!largestParticle_||P->get_InteractionRadius()>largestParticle_->get_InteractionRadius())
    {
        largestParticle_=P;
    }
    if(!smallestParticle_||P->get_InteractionRadius()<smallestParticle_->get_InteractionRadius())
    {
        smallestParticle_=P;
    }
}

/// \param[in] P A pointer to the particle, which is going to get deleted.
void ParticleHandler::checkExtremaOnDelete(BaseParticle* P)
{
	if(P==largestParticle_)
	{
		largestParticle_=0;
		Mdouble maxRadius = -std::numeric_limits<Mdouble>::max();
		for (std::vector<BaseParticle*>::const_iterator it=begin();it!=end();it++)
		{
			if ((*it)->get_InteractionRadius()>maxRadius&&P!=(*it))
			{
				maxRadius = (*it)->get_InteractionRadius();
				largestParticle_ = (*it);
			}
		}
	}
	if(P==smallestParticle_)
	{
		smallestParticle_=0;
		Mdouble minRadius = std::numeric_limits<Mdouble>::max();
		for (std::vector<BaseParticle*>::const_iterator it=begin();it!=end();it++)
		{
			if ((*it)->get_InteractionRadius()<minRadius&&P!=(*it))
			{
				minRadius = (*it)->get_InteractionRadius();
				smallestParticle_ = (*it);
			}
		}
	}
}

CSpecies& ParticleHandler::getSpecies(const int i) {
    if (!species_) std::cerr << "B" << species_ << std::endl;
    return (*species_)[i]; }   
void ParticleHandler::setSpecies(std::vector<CSpecies>* species) {species_ = species;}

