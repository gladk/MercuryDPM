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


#ifndef BOUNDARYHANDLER_H
#define BOUNDARYHANDLER_H

#include "BaseHandler.h"
#include "Boundaries/BaseBoundary.h"

/*!
 * \class BoundaryHandler
 * \brief Container to store pointers to all BaseBoundary objects
 * \details The BoundaryHandler is a container to store all BaseBoundary. It is implemented by a vector of pointers to BaseBoundary.
 */
class BoundaryHandler : public BaseHandler<BaseBoundary>
{
public:
    /*!
     * \brief Default constructor, it creates an empty BoundaryHandler.
     */
    BoundaryHandler();

    /*!
     * \brief Constructor that copies all BaseBoundary it contains and sets the other variables to 0/nullptr.
     */
    BoundaryHandler(const BoundaryHandler& BH);

    /*!
     * \brief Assignment operator, copies only the vector of BaseBoundary and sets the other variables to 0/nullptr.
     */
    BoundaryHandler operator=(const BoundaryHandler& rhs);
    
    /*!
     * \brief Destructor, it destructs the BoundaryHandler and all BaseBoundary it contains.
     */
    ~BoundaryHandler();

    /*!
     * \brief Adds a BaseBoundary to the BoundaryHandler. 
     */
    void addObject(BaseBoundary* P);

    /*!
     * \brief Reads BaseBoundary into the BoundaryHandler from restart data. 
     */
    void readObject(std::istream& is);

    /*!
     *  \brief Reads a periodic boundary from old-style restart data.    
     */ 
    void readOldObject(std::istream& is);

    /*!
     *  \brief Returns the name of the handler, namely the string "BoundaryHandler".
     */
    std::string getName() const;
};

#endif

