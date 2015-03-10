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
