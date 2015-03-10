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

#ifndef BASEOBJECT_H
#define BASEOBJECT_H

#include <iostream>
    /*!
     * \class BaseObject
     * \brief 
     * \details
     */
class BaseObject
{
public:
    /*!
     * \brief Default constructor
     */
    BaseObject();

    /*!
     * \brief Copy constructor, copies all the objects BaseObject contains
     */
    BaseObject(const BaseObject &p);

    /*!
     * \brief Destructor
     */
    virtual ~BaseObject();

    /*!
     * \brief 
     * \details
     */
    friend std::ostream& operator <<(std::ostream& os, const BaseObject& o);

    /*!
     * \brief 
     * \details
     */
    friend std::istream& operator >>(std::istream& is, BaseObject& o);

    /*!
     * \brief 
     * \param[in]
     */
    virtual void read(std::istream& is) = 0;

    /*!
     * \brief 
     * \param[in]
     */
    virtual void write(std::ostream& os) const = 0;

    /*!
     * \brief 
     * \return
     */
    virtual std::string getName() const = 0;

    /*!
     * \brief 
     * \param[in]
     */
    virtual void moveInHandler(const unsigned int index);

    /*!
     * \brief 
     * \param[in]
     */
    void setIndex(const unsigned int index);

    /*!
     * \brief 
     * \param[in]
     */
    void setId(const unsigned int id);

    /*!
     * \brief 
     * \return
     */
    unsigned int getIndex() const;

    /*!
     * \brief 
     * \return
     */
    unsigned int getId() const;

private:
    /*!
     * \brief location in BaseHandler::objects_
     */
    unsigned int index_;

    /*!
     * \brief unique identifier within handler (remains constant even if particle is moved)
     */
    unsigned int id_;

    /*!
     * \todo put the handler_ back in
     */
};
#endif
