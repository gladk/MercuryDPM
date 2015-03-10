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
#ifdef DEBUG_DESTRUCTOR
    std::cout << "BaseObject::~BaseBoundary() finished"<<std::endl;
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
