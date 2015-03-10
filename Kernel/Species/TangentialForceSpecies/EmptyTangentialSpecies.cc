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

#include "EmptyTangentialSpecies.h"

EmptyTangentialSpecies::EmptyTangentialSpecies() : BaseSpecies()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"EmptySpecies::EmptySpecies() finished"<<std::endl;
#endif
}

EmptyTangentialSpecies::EmptyTangentialSpecies(const EmptyTangentialSpecies &s)
        : BaseSpecies(s)
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"EmptySpecies::EmptySpecies(const EmptySpecies &p) finished"<<std::endl;
#endif
}

EmptyTangentialSpecies::~EmptyTangentialSpecies()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"EmptySpecies::~EmptySpecies() finished"<<std::endl;
#endif   
}

///EmptySpecies print function, which accepts an os std::stringstream as input. It prints human readable EmptySpecies information to the std::stringstream
void EmptyTangentialSpecies::write(std::ostream& os UNUSED) const
{
}

void EmptyTangentialSpecies::read(std::istream& is UNUSED)
{
}

//the name is set such that the full name does not extend
std::string EmptyTangentialSpecies::getBaseName() const
{
    return "";
}

///create values for mixed species
void EmptyTangentialSpecies::mix(EmptyTangentialSpecies* const S UNUSED, EmptyTangentialSpecies* const T UNUSED)
{
}

bool EmptyTangentialSpecies::getUseAngularDOFs() const
{
    return false;
}
