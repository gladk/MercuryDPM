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

#ifndef SCREW_H
#define SCREW_H

#include "BaseWall.h"
#include "Math/Vector.h"

/*!
 * \class Screw
 * \brief This function defines a archimedes screw in the z-direction from a (constant)  starting point, a (constant) length L, a (constant) radius r, a (constant) number or revelations N and a (constant) rotation speed (rev/s)
 *
 * \details q is a new coordinate going from 0 to 1 and t is the time, x=xs+r*cos(2*pi    *(offset+N*q)), y=ys+r*sin(2*pi*(offset+N*q)), z=zs+q*L
 */

class Screw : public BaseWall
{
public:
    
    /*!
     * \brief
     */
    Screw();

    /*!
     * \brief
     */
    Screw(Vec3D Start, double L, double R, double N, double omega, double thickness);

    /*!
     * \brief
     */
    virtual Screw* copy() const;

    /*!
     * \brief
     */
    bool getDistanceAndNormal(const BaseParticle& P, Mdouble& distance, Vec3D& normal_return) const;

    /*!
     * \brief Allows the wall to be moved to a new position (also orthogonal to the normal), and setting the velocity
     */
    void move_time(Mdouble dt);

    /*!
     * \brief reads wall
     */
    void read(std::istream& is);

    /*!
     * \brief
     */
    void oldRead(std::istream& is);

    /*!
     * \brief outputs wall
     */
    void write(std::ostream& os) const;

    /*!
     * \brief Returns the name of the object
     */
    virtual std::string getName() const;

    /*!
     * \brief
     */
    BaseInteraction* getInteractionWith(BaseParticle *P, Mdouble timeStamp, InteractionHandler* interactionHandler);

private:
    Vec3D start_; ///
    /*!
     * \brief
     */
    double l_, maxR_, n_, omega_, offset_, thickness_;
};

#endif
