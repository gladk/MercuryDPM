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

#ifndef HELPERS_H
#define HELPERS_H

#include "ExtendedMath.h"
#include <iostream> //std::istream and std::stringstream
#include <fstream> //std::fstream

namespace helpers
{

    /*!
     * \brief return type specifically for fuctions returning k and disp at once
     */
    class KAndDisp
    {
    public:
        Mdouble k;
        Mdouble disp;
    };
    
    /*!
     * \brief Set disp and k such that is matches a given collision time tc and restitution coefficient r for a collision of effective/reduced mass m. 
     */
    KAndDisp computeKAndDispFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(Mdouble tc, Mdouble r, Mdouble mass);

   /*!
    * \brief Calculates the collision time for a given stiffness, dissipation, and effective mass
    */
   Mdouble computeCollisionTimeFromKAndDispAndEffectiveMass(Mdouble k, Mdouble disp, Mdouble mass);
   /*!
    * \brief Calculates the restitution coefficient time for a given stiffness, dissipation, and effective mass
    */
   Mdouble computeRestitutionCoefficientFromKAndDispAndEffectiveMass(Mdouble k, Mdouble disp, Mdouble mass);
   /*!
    * \brief Calculates the dissipation for a given stiffness, restitution coefficient, and effective mass
    */
    Mdouble computeDispFromKAndRestitutionCoefficientAndEffectiveMass(Mdouble k, Mdouble r, Mdouble mass);
    /*!
     * \brief Calculates the collision time for a given stiffness, restitution coefficient, and effective mass
     */
    Mdouble computeCollisionTimeFromKAndRestitutionCoefficientAndEffectiveMass(Mdouble k, Mdouble r, Mdouble mass);
    /*!
     * \brief Calculates the dissipation for a given stiffness, collision time, and effective mass
     */
    Mdouble computeDispFromKAndCollisionTimeAndEffectiveMass(Mdouble k, Mdouble tc, Mdouble mass);
    /*!
     * \brief Calculates the restitution coefficient for a given stiffness, collision time, and effective mass
     */
    Mdouble computeRestitutionCoefficientFromKAndCollisionTimeAndEffectiveMass(Mdouble k, Mdouble tc, Mdouble mass);
    /*!
     * \brief Calculates the dissipation for a given collision time, restitution coefficient, and effective mass
     */
    Mdouble computeDispFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(Mdouble tc, Mdouble r, Mdouble mass);
    /*!
     * \brief Calculates the stiffness for a given collision time, restitution coefficient, and effective mass
     */
    Mdouble computeKFromCollisionTimeAndRestitutionCoefficientAndEffectiveMass(Mdouble tc, Mdouble r, Mdouble mass);
    /*!
     * \brief Calculates the stiffness for a given collision time, dissipation, and effective mass
     */
    Mdouble computeKFromCollisionTimeAndDispAndEffectiveMass(Mdouble tc, Mdouble disp, Mdouble mass);
    /*!
     * \brief Calculates the resitution coefficient for a given collision time, dissipation, and effective mass
     */
    Mdouble computeRestitutionCoefficientFromCollisionTimeAndDispAndEffectiveMass(Mdouble tc, Mdouble disp, Mdouble mass);
    /*!
     * \brief Calculates the stiffness for a given dissipation, restitution coefficient, and effective mass
     */
    Mdouble computeKFromDispAndRestitutionCoefficientAndEffectiveMass(Mdouble disp, Mdouble r, Mdouble mass);
    /*!
     * \brief Calculates the collision time for a given dissipation, restitution coefficient, and effective mass
     */
    Mdouble computeCollisionTimeFromDispAndRestitutionCoefficientAndEffectiveMass(Mdouble disp, Mdouble r, Mdouble mass);

    ///return type specifically for fuctions returning k, disp, kt, dispt at once
    /*!
     * \brief return type specifically for fuctions returning k, disp, kt, dispt at once
     */
    class KAndDispAndKtAndDispt
    {
    public:
        Mdouble k;
        Mdouble disp;
        Mdouble kt;
        Mdouble dispt;
    };
    
    /*!
     * \brief Set disp, k, dispt and kt such that is matches a given collision time tc and a normal andtangential restitution coefficient r, beta for a collision of effective/reduced mass m. From Deen...Kuipers2006, eq. 43 and 30
     */
    KAndDispAndKtAndDispt computeDisptFromCollisionTimeAndRestitutionCoefficientAndTangentialRestitutionCoefficientAndEffectiveMass(Mdouble tc, Mdouble r, Mdouble beta, Mdouble mass);
    
    /*!
     * \brief Calculates the maximum relative velocity allowed for a normal collision of two particles of radius r and particle mass m (for higher velocities particles could pass through each other)
     */
    Mdouble getMaximumVelocity(Mdouble k, Mdouble disp, Mdouble radius, Mdouble mass);

    /*!
     * \brief Use like setSaveCount(helpers::getSaveCountFromNumberOfSavesAndTimeMaxAndTimestep(numberOfSaves, getTimeMax(), getTimeStep()));
     */
    unsigned int getSaveCountFromNumberOfSavesAndTimeMaxAndTimestep(unsigned int numberOfSaves, Mdouble timeMax, Mdouble timestep);

    /*!
     * \brief 
     */
    unsigned int getSaveCountFromNumberOfSavesPerTimeUnitAndTimestep(unsigned int numberOfSaves, Mdouble timestep);
    
    /*!
     * \brief 
     */
    void getLineFromStringStream(std::istream& in, std::stringstream& out);
    
    /*!
     * \brief 
     */
    bool writeToFile(std::string filename, std::string filecontent);

    ///Function to check if a file exists, is used to check if a run has already need done
    bool fileExists(std::string strFilename);

    bool openFile(std::fstream& file, std::string filename, unsigned int options, std::fstream::openmode mode);

    Mdouble getEffectiveMass(Mdouble mass0, Mdouble mass1);

}

#endif
