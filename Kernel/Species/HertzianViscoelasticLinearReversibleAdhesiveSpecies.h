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

#ifndef HERTZIANVISCOELASTICLINEARREVERSIBLEADHESIVESPECIES_H
#define HERTZIANVISCOELASTICLINEARREVERSIBLEADHESIVESPECIES_H

#include "Species.h"
#include "NormalForceSpecies/HertzianViscoelasticNormalSpecies.h"
#include "FrictionForceSpecies/EmptyFrictionSpecies.h"
#include "AdhesiveForceSpecies/LinearReversibleAdhesiveSpecies.h"
typedef Species<HertzianViscoelasticNormalSpecies,EmptyFrictionSpecies,LinearReversibleAdhesiveSpecies> HertzianViscoelasticLinearReversibleAdhesiveSpecies;
typedef MixedSpecies<HertzianViscoelasticNormalSpecies,EmptyFrictionSpecies,LinearReversibleAdhesiveSpecies> MixedHertzianViscoelasticLinearReversibleAdhesiveSpecies;
#endif