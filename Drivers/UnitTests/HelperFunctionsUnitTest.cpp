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

#include "Logger.h"
#include "Math/ExtendedMath.h"
#include "Math/Helpers.h"
#ifndef LOG_MAIN_LEVEL
#define LOG_MAIN_LEVEL
#endif

Logger<LOG_MAIN_LEVEL> logger("Main");

int main(int argc UNUSED, char *argv[] UNUSED)
{
	///todo{Test all helper functions}

	Mdouble mass=2;
    Mdouble k=30000;
    Mdouble disp=40;
    Mdouble tc=helpers::computeCollisionTimeFromKAndDispAndEffectiveMass(k,disp,mass);
    Mdouble r=helpers::computeRestitutionCoefficientFromKAndDispAndEffectiveMass(k,disp,mass);
    if (!mathsFunc::compare(k,helpers::computeKFromCollisionTimeAndDispAndEffectiveMass(tc,disp,mass),1e-10))
    {
    logger.log(Log::FATAL,"Error in helpers::computeKFromCollisionTimeAndDispAndEffectiveMass(%,%,%) return is % and but should be %",tc,disp,mass,helpers::computeKFromCollisionTimeAndDispAndEffectiveMass(tc,disp,mass),k);
    }
    if (!mathsFunc::compare(k,helpers::computeKFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(tc,r,mass),1e-10))
    {
    logger.log(Log::FATAL,"Error in helpers::computeKFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(%,%,%) return is % and but should be %",tc,r,mass,helpers::computeKFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(tc,r,mass),k);
    }
    if (!mathsFunc::compare(k,helpers::computeKFromDispAndRestitutionCoefficientAndEffectiveMass(disp,r,mass),1e-10))
    {
    logger.log(Log::FATAL,"Error in helpers::computeKFromDispAndRestitutionCoefficientAndEffectiveMass(%,%,%) return is % and but should be %",disp,r,mass,helpers::computeKFromDispAndRestitutionCoefficientAndEffectiveMass(disp,r,mass),k);
    }

    if (!mathsFunc::compare(disp,helpers::computeDispFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(tc,r,mass),1e-10))
    {
    logger.log(Log::FATAL,"Error in helpers::computeDispFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(%,%,%) return is % and but should be %",tc,r,mass,helpers::computeDispFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(tc,r,mass),disp);
    }
    if (!mathsFunc::compare(disp,helpers::computeDispFromKAndCollisionTimeAndEffectiveMass(k,tc,mass),1e-10))
    {
    logger.log(Log::FATAL,"Error in helpers::computeDispFromKAndCollisionTimeAndEffectiveMass(%,%,%) return is % and but should be %",k,tc,mass,helpers::computeDispFromKAndCollisionTimeAndEffectiveMass(k,tc,mass),disp);
    }
    if (!mathsFunc::compare(disp,helpers::computeDispFromKAndRestitutionCoefficientAndEffectiveMass(k,r,mass),1e-10))
    {
    logger.log(Log::FATAL,"Error in helpers::computeDispFromKAndRestitutionCoefficientAndEffectiveMass(%,%,%) return is % and but should be %",k,r,mass,helpers::computeDispFromKAndRestitutionCoefficientAndEffectiveMass(k,r,mass),disp);
    }

    if (!mathsFunc::compare(tc,helpers::computeCollisionTimeFromDispAndRestitutionCoefficientAndEffectiveMass(disp,r,mass),1e-10))
    {
    logger.log(Log::FATAL,"Error in helpers::computeCollisionTimeFromDispAndRestitutionCoefficientAndEffectiveMass(%,%,%) return is % and but should be %",disp,r,mass,helpers::computeCollisionTimeFromDispAndRestitutionCoefficientAndEffectiveMass(disp,r,mass),tc);
    }
    if (!mathsFunc::compare(tc,helpers::computeCollisionTimeFromKAndRestitutionCoefficientAndEffectiveMass(k,r,mass),1e-10))
    {
    logger.log(Log::FATAL,"Error in helpers::computeCollisionTimeFromKAndRestitutionCoefficientAndEffectiveMass(%,%,%) return is % and but should be %",k,r,mass,helpers::computeCollisionTimeFromKAndRestitutionCoefficientAndEffectiveMass(k,r,mass),tc);
    }
    if (!mathsFunc::compare(tc,helpers::computeCollisionTimeFromKAndDispAndEffectiveMass(k,disp,mass),1e-10))
    {
    logger.log(Log::FATAL,"Error in helpers::computeCollisionTimeFromKandDispAndEffectiveMass(%,%,%) return is % and but should be %",k,disp,mass,helpers::computeCollisionTimeFromKAndDispAndEffectiveMass(k,disp,mass),tc);
    }

    if (!mathsFunc::compare(r,helpers::computeRestitutionCoefficientFromCollisionTimeAndDispAndEffectiveMass(tc,disp,mass),1e-10))
    {
    logger.log(Log::FATAL,"Error in helpers::computeRestitutionCoefficientFromCollisionTimeAndDispAndEffectiveMass(%,%,%) return is % and but should be %",tc,disp,mass,helpers::computeRestitutionCoefficientFromCollisionTimeAndDispAndEffectiveMass(tc,disp,mass),r);
    }
    if (!mathsFunc::compare(r,helpers::computeRestitutionCoefficientFromKAndCollisionTimeAndEffectiveMass(k,tc,mass),1e-10))
    {
    logger.log(Log::FATAL,"Error in helpers::computeRestitutionCoefficientFromKAndCollisionTimeAndEffectiveMass(%,%,%) return is % and but should be %",k,tc,mass,helpers::computeRestitutionCoefficientFromKAndCollisionTimeAndEffectiveMass(k,tc,mass),r);
    }
    if (!mathsFunc::compare(r,helpers::computeRestitutionCoefficientFromKAndDispAndEffectiveMass(k,disp,mass),1e-10))
    {
    logger.log(Log::FATAL,"Error in helpers::computeRestitutionCoefficientFromKandDispAndEffectiveMass(%,%,%) return is % and but should be %",k,disp,mass,helpers::computeRestitutionCoefficientFromKAndDispAndEffectiveMass(k,disp,mass),r);
    }    
}
