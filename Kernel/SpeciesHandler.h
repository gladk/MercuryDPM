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


#ifndef SPECIESHANDLER_H
#define SPECIESHANDLER_H

#include "BaseHandler.h"
class BaseSpecies;
class ParticleSpecies;

/// \brief Container to store all ParticleSpecies
/// \details The SpeciesHandler is a container to store all ParticleSpecies. 
/// It is implemented by a vector of pointers to ParticleSpecies.
class SpeciesHandler : public BaseHandler<ParticleSpecies>
{
public:
    /// \brief Default constructor, it creates an empty SpeciesHandler.
    SpeciesHandler();

    /// \brief Copy constructor, it copies the SpeciesHandler and all ParticleSpecies it contains.
    SpeciesHandler(const SpeciesHandler&other);

    /// \brief Copy assignment operator.
    SpeciesHandler operator =(const SpeciesHandler& rhs);
    
    /// \brief Destructor, it destructs the SpeciesHandler and all ParticleSpecies it contains.
    ~SpeciesHandler();

    /// \brief Adds a new ParticleSpecies to the SpeciesHandler.
    virtual void addObject(ParticleSpecies* const S);

    ///\brief Remove the ParticleSpecies with given id.
    void removeObject(unsigned const int id);

    /// \brief Reads Species data into the SpeciesHandler from restart file.
    void readObject(std::istream& is);

    /// \brief Reads ParticleSpecies into the SpeciesHandler from old-style restart data.
    ParticleSpecies* readOldObject(std::istream& is);

    /// \brief Gets the Id of the behaviour between two given species.
    unsigned int getMixedId(const unsigned int id1, const unsigned int id2) const;

    template<typename U> typename U::MixedSpeciesType* getMixedObject(const U* S, const U* T);

//    LinearViscoelasticMixedSpecies* getMixedObject(const LinearViscoelasticSpecies*,const LinearViscoelasticSpecies*);
//    LinearPlasticViscoelasticMixedSpecies* getMixedObject(const LinearPlasticViscoelasticSpecies*,const LinearPlasticViscoelasticSpecies*);
//    LinearViscoelasticSlidingFrictionMixedSpecies* getMixedObject(const LinearViscoelasticSlidingFrictionSpecies*,const LinearViscoelasticSlidingFrictionSpecies*);
//    LinearPlasticViscoelasticSlidingFrictionMixedSpecies* getMixedObject(const LinearPlasticViscoelasticSlidingFrictionSpecies*,const LinearPlasticViscoelasticSlidingFrictionSpecies*);
//    LinearViscoelasticFrictionMixedSpecies* getMixedObject(const LinearViscoelasticFrictionSpecies*,const LinearViscoelasticFrictionSpecies*);
//    LinearPlasticViscoelasticFrictionMixedSpecies* getMixedObject(const LinearPlasticViscoelasticFrictionSpecies*,const LinearPlasticViscoelasticFrictionSpecies*);

    /// \brief Gets the mixed object that is constructed from two given species.
    BaseSpecies* getMixedObject(const unsigned int id1, const unsigned int id2);

    /// \brief Returns a pointer to the vector of all mixed objects.
    const std::vector<BaseSpecies*>& getMixedObjects() const;

    /// \brief Write all the species and mixed species to an output stream.
    virtual void write(std::ostream& os) const;

    /// \brief Returns the name of the handler, namely the string "SpeciesHandler".
    std::string getName() const;

    /// \brief Check if angular DOF have to be used
    bool useAngularDOFs();

private:
    ///The list of pointers to the mixed species
    std::vector<BaseSpecies*> mixedObjects_;
};

#endif

