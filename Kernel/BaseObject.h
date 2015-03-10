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
