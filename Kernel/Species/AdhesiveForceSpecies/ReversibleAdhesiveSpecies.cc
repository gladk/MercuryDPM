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

#include "ReversibleAdhesiveSpecies.h"

ReversibleAdhesiveSpecies::ReversibleAdhesiveSpecies()
        : BaseSpecies()
{
    adhesionForceMax_ = 0;
    adhesionStiffness_ = 0;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"ReversibleAdhesiveSpecies::ReversibleAdhesiveSpecies() finished"<<std::endl;
#endif
}

ReversibleAdhesiveSpecies::ReversibleAdhesiveSpecies(const ReversibleAdhesiveSpecies &s)
        : BaseSpecies(s)
{
    adhesionForceMax_ = s.adhesionForceMax_;
    adhesionStiffness_ = s.adhesionStiffness_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"ReversibleAdhesiveSpecies::ReversibleAdhesiveSpecies(const ReversibleAdhesiveSpecies &p) finished"<<std::endl;
#endif
}

ReversibleAdhesiveSpecies::~ReversibleAdhesiveSpecies()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"ReversibleAdhesiveSpecies::~ReversibleAdhesiveSpecies() finished"<<std::endl;
#endif   
}

///ReversibleAdhesiveSpecies print function, which accepts an os std::stringstream as input. It prints human readable ReversibleAdhesiveSpecies information to the std::stringstream
void ReversibleAdhesiveSpecies::write(std::ostream& os) const
        {
    os << " adhesionForceMax " << adhesionForceMax_;
    os << " adhesionStiffness " << adhesionStiffness_;
}

void ReversibleAdhesiveSpecies::read(std::istream& is)
{
    std::string dummy;
    is >> dummy >> adhesionForceMax_;
    is >> dummy >> adhesionStiffness_;
}

//the name is set such that the full name does not extend
std::string ReversibleAdhesiveSpecies::getBaseName() const
{
    return "ReversibleAdhesive";
}

///create values for mixed species
void ReversibleAdhesiveSpecies::mix(ReversibleAdhesiveSpecies* const S, ReversibleAdhesiveSpecies* const T)
{
    adhesionForceMax_ = average(S->getAdhesionForceMax(), T->getAdhesionForceMax());
    adhesionStiffness_ = average(S->getAdhesionStiffness(), T->getAdhesionStiffness());
}

Mdouble ReversibleAdhesiveSpecies::getInteractionDistance() const
{
    if (adhesionStiffness_ != 0.0)
        return adhesionForceMax_ / adhesionStiffness_;
    else
    {
        std::cerr << "ReversibleAdhesiveSpecies::getInteractionDistance(): adhesionStiffness cannot be zero" << std::endl;
        exit(-1);
    }
}

///Allows the spring constant to be changed
void ReversibleAdhesiveSpecies::setAdhesionStiffness(Mdouble new_k0)
{
    if (new_k0 >= 0)
        adhesionStiffness_ = new_k0;
    else
    {
        std::cerr << "Error in setAdhesionStiffness" << std::endl;
        exit(-1);
    }
}
///Allows the spring constant to be accessed
Mdouble ReversibleAdhesiveSpecies::getAdhesionStiffness() const
{
    return adhesionStiffness_;
}

///Allows the spring constant to be changed
void ReversibleAdhesiveSpecies::setAdhesionForceMax(Mdouble new_f0)
{
    if (new_f0 >= 0)
        adhesionForceMax_ = new_f0;
    else
    {
        std::cerr << "Error in setAdhesionForceMax" << std::endl;
        exit(-1);
    }
}
///Allows the spring constant to be accessed
Mdouble ReversibleAdhesiveSpecies::getAdhesionForceMax() const
{
    return adhesionForceMax_;
}
