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
