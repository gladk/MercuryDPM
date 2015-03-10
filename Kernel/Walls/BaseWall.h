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

#ifndef BASEWALL_H
#define BASEWALL_H

#include "BaseInteractable.h"

class WallHandler;
class BaseParticle;

//Note the getVelocity can for some walls be dependent on which point on the wall is meant.
class BaseWall : public BaseInteractable
{
public:
    /*!
     * \brief
     */
    BaseWall();
    /*!
     * \brief
     */
    BaseWall(const BaseWall &p);
    /*!
     * \brief
     */
    virtual ~BaseWall();
    /*!
     * \brief
     */
    virtual BaseWall* copy() const = 0;
    /*!
     * \brief
     */
    void read(std::istream& is) = 0;
    /*!
     * \brief
     */
    void write(std::ostream& os) const = 0;
    /*!
     * \brief
     */
    virtual void clear();
    /*!
     * \brief
     */
    virtual void move(Mdouble position_);
    /*!
     * \brief
     */
    virtual void move(Vec3D velocity_, Mdouble dt);
    /*!
     * \brief
     */
    virtual void move_time(Mdouble dt);
    /*!
     * \brief
     */
    virtual bool getDistanceAndNormal(const BaseParticle& P, Mdouble& distance, Vec3D& normal_return) const = 0;

    /*!
     * \brief
     */
    void setHandler(WallHandler* handler);
    /*!
     * \brief
     */
    WallHandler* getHandler() const;

    /*!
     * \brief
     */
    void setIndSpecies(unsigned int indSpecies);

private:

    WallHandler* handler_; ///
};
#endif