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

#ifndef CTANGENTIALSPRING_H
#define CTANGENTIALSPRING_H

#include "Math/Vector.h"
#include <algorithm>
#include <vector>
#include <iostream>
#include <iomanip>
#include <functional>

/*!
 * \class CTangentialSpring
 * \brief Stores the tangential spring \f$\vec{\delta}_{ij}^t\f$ needed to compute a tangential elastic force between particles PI, PJ.
 */

class CTangentialSpring
{
public:
    ///constructors
    /*!
     * \brief
     */
    CTangentialSpring()
    {
        delta.setZero();
        RollingSpring.setZero();
        TorsionSpring.setZero();
        SlidingForce.setZero();
        pParticle = -1;
        pWall = -1;
        time = -1;
        sliding = false;
        slidingRolling = false;
        slidingTorsion = false;
    }
    /*!
     * \brief
     */    
    CTangentialSpring(int pParticle_, int pWall_, Mdouble time_)
    {
        delta.setZero();
        RollingSpring.setZero();
        TorsionSpring.setZero();
        SlidingForce.setZero();
        pParticle = pParticle_;
        pWall = pWall_;
        time = time_;
        sliding = false;
        slidingRolling = false;
        slidingTorsion = false;
    }
    /*!
     * \brief copy constructor
     */
    CTangentialSpring(const CTangentialSpring &ts)
    {
        delta = ts.delta;
        RollingSpring = ts.RollingSpring;
        TorsionSpring = ts.TorsionSpring;
        SlidingForce = ts.SlidingForce;
        pParticle = ts.pParticle;
        pWall = ts.pWall;
        time = ts.time;
        sliding = ts.sliding;
        slidingRolling = ts.slidingRolling;
        slidingTorsion = ts.slidingTorsion;
    }
    
    /*!
     * \brief outputs tangential spring
     */
    void write(std::ostream& os)
    {
        os << "delta:" << delta
                << ", particle:" << pParticle
                << ", wall:" << pWall
                << ", time:" << time;
    }
    
    /*!
     * \brief writes spring
     */
    friend inline std::ostream& operator<<(std::ostream& os, const CTangentialSpring &p)
    {
        os << p.delta << " " << p.RollingSpring << " " << p.TorsionSpring << " " << p.pParticle << " " << p.pWall << " " << p.time;
        return os;
    }
    
    /*!
     * \brief reads spring
     */
    friend inline std::istream& operator>>(std::istream& is, CTangentialSpring &p)
    {
        is >> p.delta >> p.RollingSpring >> p.TorsionSpring >> p.pParticle >> p.pWall >> p.time;
        return is;
    }
    
    /*!
     * \brief
     */
    void reverse()
    {
        delta = -delta;
        RollingSpring = -RollingSpring;
        TorsionSpring = -TorsionSpring;
        SlidingForce = -SlidingForce;
    }
    
    /*!
     * \brief stores the overlap
     */
    Vec3D delta;
    /*!
     * \brief stores the rolling spring
     */
    Vec3D RollingSpring;
    /*!
     * \brief stores the torsion spring
     */
    Vec3D TorsionSpring;
    /*!
     * \brief Stores the force (for some non-linear, hysteretic spring models)
     */
    Vec3D SlidingForce;
    /*!
     * \brief A pointer to the particle in contact; nullptr if the contact is with a wall (The other particle is the particle in which CDeltaMax is located)
     */
    int pParticle;
    /*!
     * \brief A pointer to the wall in contact; nullptr if the contact is with a particle (The other particle is the particle in which CDeltaMax is located)
     */
    int pWall;
    /*!
     * \brief stores the last time the tangential spring was read (if it was not read during the last timestep, the contact is old and will be deleted)
     */
    Mdouble time;
    /*!
     * \brief
     */
    bool sliding;
    /*!
     * \brief
     */
    bool slidingRolling;
    /*!
     * \brief
     */
    bool slidingTorsion;
};
/*!
 * \class CTangentialSprings
 * \brief Member variable of #TangentialSpringParticle storing all tangential springs \f$\vec{\delta}_{ij}^t\f$ of particle PI with contacting particles PJ. 
 * 
 * \details A tangential spring between PI and PJ can be stored in either 
 * particle depending on which particle is the first particle in 
 * MD::computeInternalForces.
 * 
 * Geometrically, there can be no more than 13 possible contacts in 3D, so the vector size is limited.
 */
class CTangentialSprings : public std::vector<CTangentialSpring>
{
public:
    
    /*!
     * \brief Construction required for the erase/remove_if stuff
     */
    struct check_spring_time : public std::binary_function<CTangentialSpring, Mdouble, bool>
    {
        bool operator()(const CTangentialSpring a, const Mdouble b) const
        {
            return (a.time < b);
        }
    };
    
    /*!
     * \brief Function selects the tangential spring vector for particle-particle interations (also removed not used springs}
     */
    void to_erase(Mdouble time)
    {
        erase(remove_if(begin(), end(), bind2nd(check_spring_time(), time)), end());
    }
    
    /*!
     * \brief Function selects the tangential spring vector for particle-particle interations (also removed not used springs}
     */
    CTangentialSpring* select_particle_spring(unsigned int P, Mdouble time_, Mdouble dt)
    {
        //Remove_if reconstructs the vector with only elements passing the check_spring_time function
        //Erase removes the end of the vector
        erase(remove_if(begin(), end(), bind2nd(check_spring_time(), time_)), end());
        
        //Loops over all Springs and returns the correct one (if it exists)
        for (CTangentialSprings::iterator it = begin(); it != end(); it++)
        {
            if (it->pParticle == static_cast<int>(P))
            {
                it->time = time_ + dt;
                return &(*it);
            }
        }
        return 0;
    }

    /*!
     * \brief
     */    
    CTangentialSpring* create_new(unsigned int P, Mdouble time_, Mdouble dt)
    {
        //if not, create it
        push_back(CTangentialSpring(static_cast<int>(P), -1, time_ + dt));
        return &back();
    }
    
    /*!
     * \brief
     */
    CTangentialSpring* create_new(unsigned int P, Mdouble time_, Mdouble dt, CTangentialSpring& CTS)
    {
        //if not, create it
        push_back(CTangentialSpring(CTS));
        back().pParticle = static_cast<int>(P);
        back().time = time_ + dt;
        return &back();
    }
    
    /*!
     * \brief
     */
    CTangentialSpring* create_new_wall(unsigned int W, Mdouble time_, Mdouble dt)
    {
        //if not, create it
        push_back(CTangentialSpring(-1, static_cast<int>(W), time_ + dt));
        return &(back());
    }
    
    /*!
     * \brief
     */
    bool has_particle_spring(int P, Mdouble time_)
    {
        //Remove_if reconstructs the vector with only elements passing the check_spring_time function
        //Erase removes the end of the vector
        erase(remove_if(begin(), end(), bind2nd(check_spring_time(), time_)), end());
        //Loops over all Springs and returns the correct one (if it exists)
        for (CTangentialSprings::iterator it = begin(); it != end(); it++)
        {
            if (it->pParticle == P)
            {
                return true;
            }
        }
        //if not, create it
        return false;
    }
    
    /*!
     * \brief Function selects the tangential spring vector for particle-particle interations (also removed not used springs}
     */
    CTangentialSpring* select_wall_spring(unsigned int W, Mdouble time_, Mdouble dt)
    {
        //Remove_if reconstructs the vector with only elements passing the check_spring_time function
        //Erase removes the end of the vector
        erase(remove_if(begin(), end(), bind2nd(check_spring_time(), time_)), end());
        
        //Loops over all Springs ant returns the correct one (if it exists)
        for (CTangentialSprings::iterator it = begin(); it != end(); it++)
        {
            if (it->pWall == static_cast<int>(W))
            {
                it->time = time_ + dt;
                return &(*it);
            }
        }
        
        //if not, create it
        //~ std::cout << "created pw " << time_ << " " << W << std::endl;
        return 0;
    }
    
    /*!
     * \brief Resets the tangential springs
     */
    void reset()
    {
        clear();
        reserve(13);
    }
    
    /*!
     * \brief outputs all current active tangential springs
     */
    void write(std::ostream& os, Mdouble time_)
    {
        os << "Tangential Springs: N=" << size() << std::endl;
        for (CTangentialSprings::iterator it = begin(); it != end(); it++)
            if (it->time >= time_)
            {
                it->write(os);
                os << std::endl;
            }
    }
    
public:
    /*!
     * \brief writes all springs
     */
    friend inline std::ostream& operator<<(std::ostream& os, const CTangentialSprings &p)
    {
        os << p.size();
        for (unsigned int i = 0; i < p.size(); i++)
            os << " " << p[i];
        return os;
    }
    
    /*!
     * \brief reads all springs
     */
    friend inline std::istream& operator>>(std::istream& is, CTangentialSprings &p)
    {
        //should return empty spring if read from is failed
        unsigned int n;
        is >> n;
        if (is.fail())
        {
            p.resize(0);
        }
        else
        {
            p.resize(n);
            for (unsigned int i = 0; i < p.size(); i++)
                is >> p[i];
        }
        return is;
    }
    
};

#endif
