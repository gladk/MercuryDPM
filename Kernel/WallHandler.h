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


#ifndef WALLHANDLER_H
#define WALLHANDLER_H

#include "BaseHandler.h"
#include "Walls/BaseWall.h"

class BaseWall;

/*!
 * \class WallHandler
 * \brief Container to store all BaseWall
 * \details The WallHandler is a container to store all BaseWall. It is implemented by a vector of pointers to BaseWall.
 */
class WallHandler : public BaseHandler<BaseWall>
{
public:
  /*!
   * \brief default constructor, it simply creates an empty WallHandler.
   */
    WallHandler();

  /*!
   * \brief Copy constructor, it copies the WallHandler and all BaseWall it contains.
   */
    WallHandler(const WallHandler& BH);

  /*!
   * \brief Assignment operator.
   */
    WallHandler operator =(const WallHandler& rhs);
    
  /*!
   * \brief Destructor, it simply destructs the WallHandler and all BaseWall it contains.
   */
    ~WallHandler();

  /*!
   * \brief Adds a new BaseWall to the WallHandler.
   */
    void addObject(BaseWall* W);

  /*!
   * \brief Reads BaseWall into the WallHandler from restart data.
   */
    void readObject(std::istream& is);

    void readOldObject(std::istream& is);

    std::string getName() const;

private:
    
};

#endif

