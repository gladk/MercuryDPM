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
            index = Next->get_Index() + 1;
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
            index = Next->get_Index() + 1;
        else
            index = 0;
    }
    
    /*!
     * \brief 
     */
    BaseParticle* get_P1()
    {
        return P1;
    }

    /*!
     * \brief 
     */
    BaseParticle* get_P2()
    {
        return P2;
    }

    /*!
     * \brief 
     */
    BaseParticle* get_OtherParticle(BaseParticle *P)
    {
        if (P1 == P)
            return P2;
        else if (P2 == P)
            return P1;
        else
        {
            std::cout << "Particle* get_OtherParticle(Particle *P), no match for " << P->getIndex() << " posibilities are " << P1->getIndex() << " and " << P2->getIndex() << std::endl;
            return 0;
        }
    }

    /*!
     * \brief 
     */
    PossibleContact* get_Next()
    {
        return Next;
    }

    /*!
     * \brief 
     */
    PossibleContact* get_Next(BaseParticle* P)
    {
        if (P1 == P)
            return Next1;
        else if (P2 == P)
            return Next2;
        else
        {
            std::cout << "get_Next(Particle* P), no match for " << P->getIndex() << " posibilities are " << P1->getIndex() << " and " << P2->getIndex() << std::endl;
            return 0;
        }
    }

    /*!
     * \brief 
     */
    PossibleContact* get_Prev()
    {
        return Prev;
    }

    /*!
     * \brief 
     */
    PossibleContact* get_Prev(BaseParticle* P)
    {
        if (P1 == P)
            return Prev1;
        else if (P2 == P)
            return Prev2;
        else
        {
            std::cout << "PossibleContact* get_Prev(Particle* P), no match for " << P->getIndex() << " posibilities are " << P1->getIndex() << " and " << P2->getIndex() << std::endl;
            return 0;
        }
    }

    /*!
     * \brief 
     */
    PossibleContact* get_Next1()
    {
        return Next1;
    }

    /*!
     * \brief 
     */
    PossibleContact* get_Prev1()
    {
        return Prev1;
    }

    /*!
     * \brief 
     */
    PossibleContact* get_Next2()
    {
        return Next2;
    }

    /*!
     * \brief 
     */
    PossibleContact* get_Prev2()
    {
        return Prev2;
    }

    /*!
     * \brief 
     */
    int get_Index()
    {
        return index;
    }
    
    /*!
     * \brief 
     */
    void set_P1(BaseParticle* P1)
    {
        this->P1 = P1;
    }

    /*!
     * \brief 
     */
    void set_P2(BaseParticle* P2)
    {
        this->P2 = P2;
    }

    /*!
     * \brief 
     */
    void set_Next(PossibleContact* Next)
    {
        this->Next = Next;
    }

    /*!
     * \brief 
     */
    void set_Next(BaseParticle* P, PossibleContact* Next)
    {
        if (P1 == P)
            this->Next1 = Next;
        else if (P2 == P)
            this->Next2 = Next;
        else
            std::cout << "void set_Next(Particle* P,PossibleContact* Next), no match for " << P->getIndex() << " posibilities are " << P1->getIndex() << " and " << P2->getIndex() << std::endl;
    }

    /*!
     * \brief 
     */
    void set_Prev(PossibleContact* Prev)
    {
        this->Prev = Prev;
    }

    /*!
     * \brief 
     */
    void set_Prev(BaseParticle* P, PossibleContact* Prev)
    {
        if (P1 == P)
            this->Prev1 = Prev;
        else if (P2 == P)
            this->Prev2 = Prev;
        else
            std::cout << "void set_Prev(Particle* P,PossibleContact* Prev), no match for " << P->getIndex() << " posibilities are " << P1->getIndex() << " and " << P2->getIndex() << std::endl;
    }

    /*!
     * \brief 
     */
    void set_Next1(PossibleContact* Next1)
    {
        this->Next1 = Next1;
    }

    /*!
     * \brief 
     */
    void set_Prev1(PossibleContact* Prev1)
    {
        this->Prev1 = Prev1;
    }

    /*!
     * \brief 
     */
    void set_Next2(PossibleContact* Next2)
    {
        this->Next2 = Next2;
    }

    /*!
     * \brief 
     */
    void set_Prev2(PossibleContact* Prev2)
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
