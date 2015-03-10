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

#include "Helpers.h"
#include <cmath>
#include <sys/stat.h>
#include <sstream>
#include <string>


helpers::KAndDisp helpers::computeKAndDispFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(Mdouble tc, Mdouble r, Mdouble mass)
{
    helpers::KAndDisp ans;
    ans.k = computeKFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(tc, r, mass);
    ans.disp = computeDispFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(tc, r, mass);
    return ans;
}

Mdouble helpers::computeCollisionTimeFromKAndDispAndEffectiveMass(Mdouble k, Mdouble disp, Mdouble mass)
{
    if (k <= 0)
    {
        std::cerr << "Error in helpers::computeCollisionTimeFromKandDispAndEffectiveMass(Mdouble k, Mdouble disp, Mdouble mass) stiffness is not set or has an unexpected value, (stiffness=" << k << ")"<<std::endl;
        exit(-1);
    }
    if (disp < 0)
    {
        std::cerr << "Error in helpers::computeCollisionTimeFromKandDispAndEffectiveMass(Mdouble k, Mdouble disp, Mdouble mass) dissipation is not set or has an unexpected value, (dissipation="<<disp<<")"<<std::endl;
        exit(-1);
    }
    if (mass <= 0)
    {
        std::cerr << "Error in helpers::computeCollisionTimeFromKandDispAndEffectiveMass(Mdouble k, Mdouble disp, Mdouble mass) mass is not set or has an unexpected value (mass="<< mass << ")" << std::endl;
        exit(-1);
    }
    if(4.0*k/mass-mathsFunc::square(disp/mass)<0)
    {
        std::cerr << "Error in helpers::computeCollisionTimeFromKandDispAndEffectiveMass(Mdouble k, Mdouble disp, Mdouble mass) values for stiffness, dissipation and mass lead to an overdamped system (stiffness=" << k <<" dissipation="<<disp<<" mass="<< mass << ")" << std::endl;
        exit(-1);
    }
    return 2.0*constants::pi/std::sqrt(4.0*k/mass-mathsFunc::square(disp/mass));
}

Mdouble helpers::computeRestitutionCoefficientFromKAndDispAndEffectiveMass(Mdouble k, Mdouble disp, Mdouble mass)
{
    if (k <= 0)
    {
        std::cerr << "Error in helpers::computeRestitutionCoefficientFromKandDispAndEffectiveMass(Mdouble k, Mdouble disp, Mdouble mass) stiffness is not set or has an unexpected value, (stiffness=" << k << ")"<<std::endl;
        exit(-1);
    }
    if (disp < 0)
    {
        std::cerr << "Error in helpers::computeRestitutionCoefficientFromKandDispAndEffectiveMass(Mdouble k, Mdouble disp, Mdouble mass) dissipation is not set or has an unexpected value, (dissipation="<<disp<<")"<<std::endl;
        exit(-1);
    }
    if (mass <= 0)
    {
        std::cerr << "Error in helpers::computeRestitutionCoefficientFromKandDispAndEffectiveMass(Mdouble k, Mdouble disp, Mdouble mass) mass is not set or has an unexpected value (mass="<< mass << ")" << std::endl;
        exit(-1);
    }
    if(4.0*mass*k-mathsFunc::square(disp)<0)
    {
        std::cerr << "Error in helpers::computeRestitutionCoefficientFromKandDispAndEffectiveMass(Mdouble k, Mdouble disp, Mdouble mass) values for stiffness, dissipation and mass lead to an overdamped system (stiffness=" << k <<" dissipation="<<disp<<" mass="<< mass << ")" << std::endl;
        exit(-1);
    }
    return std::exp(-disp*constants::pi/std::sqrt(4.0*mass*k-mathsFunc::square(disp)));
}

Mdouble helpers::computeDispFromKAndRestitutionCoefficientAndEffectiveMass(Mdouble k, Mdouble r, Mdouble mass)
{
    if (k <= 0)
    {
        std::cerr << "Error in helpers::computeDispFromKAndRestitutionCoefficientAndEffectiveMass(Mdouble k, Mdouble r, Mdouble mass) stiffness is not set or has an unexpected value, (stiffness=" << k << ")"<<std::endl;
        exit(-1);
    }
    if (r < 0)
    {
        std::cerr << "Error in helpers::computeDispFromKAndRestitutionCoefficientAndEffectiveMass(Mdouble k, Mdouble r, Mdouble mass) restitution coefficient is not set or has an unexpected value, (restitution coefficient="<<r<<")"<<std::endl;
        exit(-1);
    }
    if (mass <= 0)
    {
        std::cerr << "Error in helpers::computeDispFromKAndRestitutionCoefficientAndEffectiveMass(Mdouble k, Mdouble r, Mdouble mass) mass is not set or has an unexpected value (mass="<< mass << ")" << std::endl;
        exit(-1);
    }
    return -2.0*sqrt(mass * k / (constants::sqr_pi + mathsFunc::square(std::log(r)))) * std::log(r);
}

Mdouble helpers::computeCollisionTimeFromKAndRestitutionCoefficientAndEffectiveMass(Mdouble k, Mdouble r, Mdouble mass)
{
    if (k <= 0)
    {
        std::cerr << "Error in helpers::computeCollisionTimeFromKAndRestitutionCoefficientAndEffectiveMass(Mdouble k, Mdouble r, Mdouble mass) stiffness is not set or has an unexpected value, (stiffness=" << k << ")"<<std::endl;
        exit(-1);
    }
    if (r < 0)
    {
        std::cerr << "Error in helpers::computeCollisionTimeFromKAndRestitutionCoefficientAndEffectiveMass(Mdouble k, Mdouble r, Mdouble mass) restitution coefficient is not set or has an unexpected value, (restitution coefficient="<<r<<")"<<std::endl;
        exit(-1);
    }
    if (mass <= 0)
    {
        std::cerr << "Error in helpers::computeCollisionTimeFromKAndRestitutionCoefficientAndEffectiveMass(Mdouble k, Mdouble r, Mdouble mass) mass is not set or has an unexpected value (mass="<< mass << ")" << std::endl;
        exit(-1);
    }
    return sqrt(mass/k*(constants::sqr_pi + mathsFunc::square(std::log(r))));
}

Mdouble helpers::computeDispFromKAndCollisionTimeAndEffectiveMass(Mdouble k, Mdouble tc, Mdouble mass)
{
    if (k <= 0)
    {
        std::cerr << "Error in helpers::computeDispFromKAndCollisionTimeAndEffectiveMass(Mdouble k, Mdouble tc, Mdouble mass) stiffness is not set or has an unexpected value, (stiffness=" << k << ")"<<std::endl;
        exit(-1);
    }
    if (tc <= 0)
    {
        std::cerr << "Error in helpers::computeDispFromKAndCollisionTimeAndEffectiveMass(Mdouble k, Mdouble tc, Mdouble mass) collision time is not set or has an unexpected value, (collision time="<<tc<<")"<<std::endl;
        exit(-1);
    }
    if (mass <= 0)
    {
        std::cerr << "Error in helpers::computeDispFromKAndCollisionTimeAndEffectiveMass(Mdouble k, Mdouble tc, Mdouble mass) mass is not set or has an unexpected value (mass="<< mass << ")" << std::endl;
        exit(-1);
    }
    if(mass*k-constants::sqr_pi*mathsFunc::square(mass/tc)<0)
    {
        std::cerr << "Error in helpers::computeDispFromKAndCollisionTimeAndEffectiveMass(Mdouble k, Mdouble disp, Mdouble mass) values for stiffness, collision time and mass lead to an overdamped system (stiffness=" << k <<" collision time="<<tc<<" mass="<< mass << ")" << std::endl;
        exit(-1);
    }
    return 2.0*std::sqrt(mass*k-constants::sqr_pi*mathsFunc::square(mass/tc));
}

Mdouble helpers::computeRestitutionCoefficientFromKAndCollisionTimeAndEffectiveMass(Mdouble k, Mdouble tc, Mdouble mass)
{
    if (k <= 0)
    {
        std::cerr << "Error in helpers::computeRestitutionCoefficientFromKAndCollisionTimeAndEffectiveMass(Mdouble k, Mdouble tc, Mdouble mass) stiffness is not set or has an unexpected value, (stiffness=" << k << ")"<<std::endl;
        exit(-1);
    }
    if (tc <= 0)
    {
        std::cerr << "Error in helpers::computeRestitutionCoefficientFromKAndCollisionTimeAndEffectiveMass(Mdouble k, Mdouble tc, Mdouble mass) collision time is not set or has an unexpected value, (collision time="<<tc<<")"<<std::endl;
        exit(-1);
    }
    if (mass <= 0)
    {
        std::cerr << "Error in helpers::computeRestitutionCoefficientFromKAndCollisionTimeAndEffectiveMass(Mdouble k, Mdouble tc, Mdouble mass) mass is not set or has an unexpected value (mass="<< mass << ")" << std::endl;
        exit(-1);
    }
    if(k/mass*mathsFunc::square(tc)-constants::sqr_pi<0)
    {
        std::cerr << "Error in helpers::computeRestitutionCoefficientFromKAndCollisionTimeAndEffectiveMass(Mdouble k, Mdouble disp, Mdouble mass) values for stiffness, collision time and mass lead to an overdamped system (stiffness=" << k <<" collision time="<<tc<<" mass="<< mass << ")" << std::endl;
        exit(-1);
    }
    return std::exp(-std::sqrt(k/mass*mathsFunc::square(tc)-constants::sqr_pi));
}

Mdouble helpers::computeDispFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(Mdouble tc, Mdouble r, Mdouble mass)
{
    if (tc <= 0)
    {
        std::cerr << "Error in helpers::computeDispFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(Mdouble tc, Mdouble r, Mdouble mass) collision time is not set or has an unexpected value, (collision time=" << tc << ")"<<std::endl;
        exit(-1);
    }
    if (r < 0)
    {
        std::cerr << "Error in helpers::computeDispFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(Mdouble tc, Mdouble r, Mdouble mass) restitution coefficient not set or has an unexpected value, (restitution coefficient="<<r<<")"<<std::endl;
        exit(-1);
    }
    if (mass <= 0)
    {
        std::cerr << "Error in helpers::computeDispFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(Mdouble tc, Mdouble r, Mdouble mass) mass is not set or has an unexpected value (mass="<< mass << ")" << std::endl;
        exit(-1);
    }
    return -2.0 * mass * std::log(r) / tc;
}

Mdouble helpers::computeKFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(Mdouble tc, Mdouble r, Mdouble mass)
{
    if (tc <= 0)
    {
        std::cerr << "Error in helpers::computeKFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(Mdouble tc, Mdouble r, Mdouble mass) collision time is not set or has an unexpected value, (collision time=" << tc << ")"<<std::endl;
        exit(-1);
    }
    if (r < 0)
    {
        std::cerr << "Error in helpers::computeKFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(Mdouble tc, Mdouble r, Mdouble mass) restitution coefficient not set or has an unexpected value, (restitution coefficient="<<r<<")"<<std::endl;
        exit(-1);
    }
    if (mass <= 0)
    {
        std::cerr << "Error in helpers::computeKFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(Mdouble tc, Mdouble r, Mdouble mass) mass is not set or has an unexpected value (mass="<< mass << ")" << std::endl;
        exit(-1);
    }
    return mass * (mathsFunc::square(constants::pi / tc) + mathsFunc::square(-std::log(r) / tc));
}

Mdouble helpers::computeKFromCollisionTimeAndDispAndEffectiveMass(Mdouble tc, Mdouble disp, Mdouble mass)
{
    if (tc <= 0)
    {
        std::cerr << "Error in helpers::computeKFromCollisionTimeAndDispAndEffectiveMass(Mdouble tc, Mdouble disp, Mdouble mass) collision time is not set or has an unexpected value, (collision time=" << tc << ")"<<std::endl;
        exit(-1);
    }
    if (disp < 0)
    {
        std::cerr << "Error in helpers::computeKFromCollisionTimeAndDispAndEffectiveMass(Mdouble tc, Mdouble disp, Mdouble mass) dissipation not set or has an unexpected value, (dissipation="<< disp <<")"<<std::endl;
        exit(-1);
    }
    if (mass <= 0)
    {
        std::cerr << "Error in helpers::computeKFromCollisionTimeAndDispAndEffectiveMass(Mdouble tc, Mdouble disp, Mdouble mass) mass is not set or has an unexpected value (mass="<< mass << ")" << std::endl;
        exit(-1);
    }
    return 0.25*mathsFunc::square(disp)/mass+constants::sqr_pi*mass/mathsFunc::square(tc);
}

Mdouble helpers::computeRestitutionCoefficientFromCollisionTimeAndDispAndEffectiveMass(Mdouble tc, Mdouble disp, Mdouble mass)
{
    if (tc <= 0)
    {
        std::cerr << "Error in helpers::computeRestitutionCoefficientFromCollisionTimeAndDispAndEffectiveMass(Mdouble tc, Mdouble disp, Mdouble mass) collision time is not set or has an unexpected value, (collision time=" << tc << ")"<<std::endl;
        exit(-1);
    }
    if (disp < 0)
    {
        std::cerr << "Error in helpers::computeRestitutionCoefficientFromCollisionTimeAndDispAndEffectiveMass(Mdouble tc, Mdouble disp, Mdouble mass) dissipation not set or has an unexpected value, (dissipation="<< disp <<")"<<std::endl;
        exit(-1);
    }
    if (mass <= 0)
    {
        std::cerr << "Error in helpers::computeRestitutionCoefficientFromCollisionTimeAndDispAndEffectiveMass(Mdouble tc, Mdouble disp, Mdouble mass) mass is not set or has an unexpected value (mass="<< mass << ")" << std::endl;
        exit(-1);
    }
    return std::exp(-0.5*disp*tc/mass);   
}

Mdouble helpers::computeKFromDispAndRestitutionCoefficientAndEffectiveMass(Mdouble disp, Mdouble r, Mdouble mass)
{
    if (disp <= 0)
    {
        std::cerr << "Error in helpers::computeKFromDispAndRestitutionCoefficientAndEffectiveMass(Mdouble disp, Mdouble r, Mdouble mass) dissipation is not set or has an unexpected value, (dissipation time=" << disp << ")"<<std::endl;
        exit(-1);
    }
    if (r < 0)
    {
        std::cerr << "Error in helpers::computeKFromDispAndRestitutionCoefficientAndEffectiveMass(Mdouble disp, Mdouble r, Mdouble mass) restitution coefficient not set or has an unexpected value, (restitution coefficient="<< r <<")"<<std::endl;
        exit(-1);
    }
    if (mass <= 0)
    {
        std::cerr << "Error in helpers::computeKFromDispAndRestitutionCoefficientAndEffectiveMass(Mdouble disp, Mdouble r, Mdouble mass) mass is not set or has an unexpected value (mass="<< mass << ")" << std::endl;
        exit(-1);
    }
   return 0.25*mathsFunc::square(disp)*(constants::sqr_pi/(mathsFunc::square(std::log(r)))+1.0)/mass;
}

Mdouble helpers::computeCollisionTimeFromDispAndRestitutionCoefficientAndEffectiveMass(Mdouble disp, Mdouble r, Mdouble mass)
{
    if (disp <= 0)
    {
        std::cerr << "Error in helpers::computeCollisionTimeFromDispAndRestitutionCoefficientAndEffectiveMass(Mdouble disp, Mdouble r, Mdouble mass) dissipation is not set or has an unexpected value, (dissipation time=" << disp << ")"<<std::endl;
        exit(-1);
    }
    if (r < 0)
    {
        std::cerr << "Error in helpers::computeCollisionTimeFromDispAndRestitutionCoefficientAndEffectiveMass(Mdouble disp, Mdouble r, Mdouble mass) restitution coefficient not set or has an unexpected value, (restitution coefficient="<< r <<")"<<std::endl;
        exit(-1);
    }
    if (mass <= 0)
    {
        std::cerr << "Error in helpers::computeCollisionTimeFromDispAndRestitutionCoefficientAndEffectiveMass(Mdouble disp, Mdouble r, Mdouble mass) mass is not set or has an unexpected value (mass="<< mass << ")" << std::endl;
        exit(-1);
    }
    return -2.0*mass*std::log(r)/disp;
}
///from Deen...Kuipers2006, eq. 43 and 30
helpers::KAndDispAndKtAndDispt helpers::computeDisptFromCollisionTimeAndRestitutionCoefficientAndTangentialRestitutionCoefficientAndEffectiveMass(Mdouble tc, Mdouble r, Mdouble beta, Mdouble mass)
{
    helpers::KAndDispAndKtAndDispt ans;
    ans.disp = -2.0 * mass * log(r) / tc;
    ans.k = mass * (mathsFunc::square(constants::pi / tc) + mathsFunc::square(ans.disp / (2.0 * mass)));
    ans.kt = 2.0 / 7.0 * ans.k * (mathsFunc::square(constants::pi) + mathsFunc::square(log(beta))) / (mathsFunc::square(constants::pi) + mathsFunc::square(log(r)));
    if (beta != 0.0)
        ans.dispt = -2 * log(beta) * sqrt(1.0 / 7.0 * mass * ans.kt / (mathsFunc::square(constants::pi) + mathsFunc::square(log(beta))));
    else
        ans.dispt = 2. * sqrt(1.0 / 7.0 * mass * ans.kt);
    return ans;
}

Mdouble helpers::getMaximumVelocity(Mdouble k, Mdouble disp, Mdouble radius, Mdouble mass)
{
// note: underestimate based on energy argument,
// Ekin = 2*1/2*m*(v/2)^2 = 1/2*k*(2*r)^2, gives
// return radius * sqrt(8.0*k/mass);
    
// with dissipation, see S. Luding, Collisions & Contacts between two particles, eq 34
    Mdouble w = sqrt(k / mass - mathsFunc::square(disp / (2.0 * mass)));
    Mdouble w0 = sqrt(k / mass);
    Mdouble DispCorrection = exp(-disp / mass / w) * asin(w / w0);
//std::cout << "DispCorrection" << DispCorrection << std::endl;
    return radius * sqrt(8.0 * k / mass) / DispCorrection;
}

unsigned int helpers::getSaveCountFromNumberOfSavesAndTimeMaxAndTimestep(unsigned int numberOfSaves, Mdouble timeMax, Mdouble timestep)
{
    if (numberOfSaves > 0 && timeMax > 0 && timestep > 0)
    {
        return static_cast<unsigned int>(ceil(timeMax / timestep / static_cast<double>(numberOfSaves - 1)));
    }
    else
    {
        std::cerr << "Error in getSaveCountFromNumberOfSavesAndTimeMaxAndTimestep (" << numberOfSaves << "," << timeMax << "," << timestep << ")" << std::endl;
        std::cerr << "Arguments need to be positive" << std::endl;
        exit(-1);
    }
}

unsigned int helpers::getSaveCountFromNumberOfSavesPerTimeUnitAndTimestep(unsigned int numberOfSaves, Mdouble timestep)
{
    if (numberOfSaves > 1 && timestep > 0)
    {
        return static_cast<unsigned int>(ceil(1.0 / timestep / static_cast<double>(numberOfSaves - 1)));
    }
    else
    {
        std::cerr << "Error in getSaveCountFromNumberOfSavesPerTimeUnitAndTimestep (" << numberOfSaves << "," << timestep << ")" << std::endl;
        std::cerr << "Arguments need to be positive" << std::endl;
        exit(-1);
    }
}

void helpers::getLineFromStringStream(std::istream& in, std::stringstream& out)
{
    std::string line_string;
    getline(in, line_string);
    out.str(line_string);
    out.clear();
}

bool helpers::writeToFile(std::string filename, std::string filecontent)
{
    std::fstream file;
    file.open(filename.c_str(), std::ios::out);
    if (file.fail())
    {
        std::cerr << "Error in writeToFile: file could not be opened" << std::endl;
        return false;
    }
    file << filecontent;
    file.close();
    return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///This is a FileExist routine, which is used to test if a run have already need preformed, allows me to plug holes in parm studies.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool helpers::fileExists(std::string strFilename)
{
    struct stat stFileInfo;
    bool blnReturn;
    int intStat;

// Attempt to get the file attributes

    intStat = stat(strFilename.c_str(), &stFileInfo);
    if (intStat == 0)
    {
        // We were able to get the file attributes
        // so the file obviously exists.
        blnReturn = true;
    }
    else
    {
        // We were not able to get the file attributes.
        // This may mean that we don't have permission to
        // access the folder which contains this file. If you
        // need to do that level of checking, lookup the
        // return values of stat which will give you
        // more details on why stat failed.
        blnReturn = false;
    }

    return blnReturn;
}

bool helpers::openFile(std::fstream& file, std::string filename, unsigned int options, std::fstream::openmode mode)
{
    if (options == 1)
    {
        //std::cout << "open " << filename << std::endl;
        file.open(filename.c_str(), mode);
        if (file.fail())
            return false;
    }
    return true;
}

Mdouble helpers::getEffectiveMass(Mdouble mass0, Mdouble mass1)
{
    return mass0 * mass1 / (mass0 + mass1);
}

