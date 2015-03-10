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

#ifndef POSSIBLECONTACT_H
#define POSSIBLECONTACT_H

#include "Particles/BaseParticle.h"

/*!
 * \class PossibleContact
 * \brief
 */
class PossibleContact
{
public:
  // constructors
    /*!
     * \brief constructor
     */
    PossibleContact()
    {
        P1 = 0;
        P2 = 0;
        Next = 0;
        Prev = 0;
        Next1 = 0;
        Prev1 = 0;
        Next2 = 0;
        Prev2 = 0;
        index = 0;
    }

    /*!
     * \brief 
     */
    PossibleContact(BaseParticle* P1, BaseParticle* P2)
    {
        this->P1 = P1;
        this->P2 = P2;
    }

    /*!
     * \brief 
     */
    PossibleContact(BaseParticle* P1, BaseParticle* P2, PossibleContact* Next)
    {
        this->P1 = P1;
        this->P2 = P2;
        this->Next = Next;
        this->Prev = 0;
        if (Next)
            index = Next->getIndex() + 1;
        else
            index = 0;
    }

    /*!
     * \brief 
     */
    PossibleContact(BaseParticle* P1, BaseParticle* P2, PossibleContact* Next, PossibleContact* Next1, PossibleContact* Next2)
    {
        this->P1 = P1;
        this->P2 = P2;
        this->Next = Next;
        this->Next1 = Next1;
        this->Next2 = Next2;
        this->Prev = 0;
        this->Prev1 = 0;
        this->Prev2 = 0;
        if (Next)
            index = Next->getIndex() + 1;
        else
            index = 0;
    }
    
    /*!
     * \brief 
     */
    BaseParticle* getP1()
    {
        return P1;
    }

    /*!
     * \brief 
     */
    BaseParticle* getP2()
    {
        return P2;
    }

    /*!
     * \brief 
     */
    BaseParticle* getOtherParticle(BaseParticle *P)
    {
        if (P1 == P)
            return P2;
        else if (P2 == P)
            return P1;
        else
        {
            std::cout << "Particle* getOtherParticle(Particle *P), no match for " << P->getIndex() << " posibilities are " << P1->getIndex() << " and " << P2->getIndex() << std::endl;
            return 0;
        }
    }

    /*!
     * \brief 
     */
    PossibleContact* getNext()
    {
        return Next;
    }

    /*!
     * \brief 
     */
    PossibleContact* getNext(BaseParticle* P)
    {
        if (P1 == P)
            return Next1;
        else if (P2 == P)
            return Next2;
        else
        {
            std::cout << "getNext(Particle* P), no match for " << P->getIndex() << " posibilities are " << P1->getIndex() << " and " << P2->getIndex() << std::endl;
            return 0;
        }
    }

    /*!
     * \brief 
     */
    PossibleContact* getPrevious()
    {
        return Prev;
    }

    /*!
     * \brief 
     */
    PossibleContact* getPrevious(BaseParticle* P)
    {
        if (P1 == P)
            return Prev1;
        else if (P2 == P)
            return Prev2;
        else
        {
            std::cout << "PossibleContact* getPrevious(Particle* P), no match for " << P->getIndex() << " posibilities are " << P1->getIndex() << " and " << P2->getIndex() << std::endl;
            return 0;
        }
    }

    /*!
     * \brief 
     */
    PossibleContact* getNext1()
    {
        return Next1;
    }

    /*!
     * \brief 
     */
    PossibleContact* getPrevious1()
    {
        return Prev1;
    }

    /*!
     * \brief 
     */
    PossibleContact* getNext2()
    {
        return Next2;
    }

    /*!
     * \brief 
     */
    PossibleContact* getPrevious2()
    {
        return Prev2;
    }

    /*!
     * \brief 
     */
    int getIndex()
    {
        return index;
    }
    
    /*!
     * \brief 
     */
    void setP1(BaseParticle* P1)
    {
        this->P1 = P1;
    }

    /*!
     * \brief 
     */
    void setP2(BaseParticle* P2)
    {
        this->P2 = P2;
    }

    /*!
     * \brief 
     */
    void setNextPosition(PossibleContact* Next)
    {
        this->Next = Next;
    }

    /*!
     * \brief 
     */
    void setNextPosition(BaseParticle* P, PossibleContact* Next)
    {
        if (P1 == P)
            this->Next1 = Next;
        else if (P2 == P)
            this->Next2 = Next;
        else
            std::cout << "void setNextPosition(Particle* P,PossibleContact* Next), no match for " << P->getIndex() << " posibilities are " << P1->getIndex() << " and " << P2->getIndex() << std::endl;
    }

    /*!
     * \brief 
     */
    void setPreviousPosition(PossibleContact* Prev)
    {
        this->Prev = Prev;
    }

    /*!
     * \brief 
     */
    void setPreviousPosition(BaseParticle* P, PossibleContact* Prev)
    {
        if (P1 == P)
            this->Prev1 = Prev;
        else if (P2 == P)
            this->Prev2 = Prev;
        else
            std::cout << "void setPreviousPosition(Particle* P,PossibleContact* Prev), no match for " << P->getIndex() << " posibilities are " << P1->getIndex() << " and " << P2->getIndex() << std::endl;
    }

    /*!
     * \brief 
     */
    void setNextPosition1(PossibleContact* Next1)
    {
        this->Next1 = Next1;
    }

    /*!
     * \brief 
     */
    void setPreviousPosition1(PossibleContact* Prev1)
    {
        this->Prev1 = Prev1;
    }

    /*!
     * \brief 
     */
    void setNextPosition2(PossibleContact* Next2)
    {
        this->Next2 = Next2;
    }

    /*!
     * \brief 
     */
    void setPreviousPosition2(PossibleContact* Prev2)
    {
        this->Prev2 = Prev2;
    }
    
    /*!
     * \brief
     * \details
     */
    friend std::ostream& operator <<(std::ostream& os, const PossibleContact& o)
    {
        std::cout<<o.index<<" between ("<<o.P1->getIndex()<<","<<o.P2->getIndex()<<")";
        return os;
    }

private:
    int index;///
    BaseParticle* P1;///
    BaseParticle* P2;///
    PossibleContact* Next;///
    PossibleContact* Prev;///
    PossibleContact* Next1;///
    PossibleContact* Prev1;///
    PossibleContact* Next2;///
    PossibleContact* Prev2;///
    
};

#endif
