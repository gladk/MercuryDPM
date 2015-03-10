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

#include "BaseObject.h"

BaseObject::BaseObject()
{
    index_ = 0;
    id_ = 0;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseObject::BaseObject() finished"<<std::endl;
#endif
}

BaseObject::BaseObject(const BaseObject &p)
{
    index_ = p.index_;
    id_ = p.id_;
#ifdef DEBUG_CONSTRUCTOR
    std::cout<<"BaseObject::BaseObject(const BaseObject &p) finished"<<std::endl;
#endif
}

BaseObject::~BaseObject()
{
#ifdef DEBUG_CONSTRUCTOR
    std::cerr << "BaseBoundary::~BaseBoundary() finished"<<std::endl;
#endif
}

std::ostream& operator <<(std::ostream& os, const BaseObject& o)
{
    o.write(os);
    return os;
}

std::istream& operator >>(std::istream& is, BaseObject &o)
{
    o.read(is);
    return (is);
}

void BaseObject::moveInHandler(const unsigned int index)
{
    index_ = index;
}

void BaseObject::setIndex(const unsigned int index)
{
    index_ = index;
}

void BaseObject::setId(const unsigned int id)
{
    id_ = id;
    ///\todo TW: here we should update BaseHandler::nextId_
}


unsigned int BaseObject::getIndex() const
{
    return index_;
}

unsigned int BaseObject::getId() const
{
    return id_;
}

void BaseObject::read(std::istream& is)
{
    std::string dummy;
    is >> dummy >> id_;
}

void BaseObject::write(std::ostream& os) const
{
    os << getName();
    os << " id " << id_;
}
