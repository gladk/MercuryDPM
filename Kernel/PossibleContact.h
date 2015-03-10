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
#include "Logger.h"

/*!
 * \brief Class that describes a possible contact between two BaseParticle.
 * \details In this class that describes a possible contact two BaseParticle
 *          we save both of these BaseParticle, the index of this PossibleContact
 *          and the PossibleContact that comes before and after this one
 *          in the linked list, both for the general list and the list for each
 *          of the particles.
 * \todo Change the names so that they follow the code guidelines.
 */
class PossibleContact
{
public:
    /*!
     * \brief Default constructor, sets all pointers to nullptr.
     */
    PossibleContact()
    {
        P1_ = nullptr;
        P2_ = nullptr;
        Next_ = nullptr;
        Prev_ = nullptr;
        Next1_ = nullptr;
        Prev1_ = nullptr;
        Next2_ = nullptr;
        Prev2_ = nullptr;
        index_ = 0;
    }

    /*!
     * \brief Constructor that takes two BaseParticle and sets them to be the BaseParticle associated with this PossibleContact.
     * \param[in] P1 A pointer to the first BaseParticle in this PossibleContact.
     * \param[in] P2 A pointer to the second BaseParticle in this PossibleContact.
     */
    PossibleContact(BaseParticle* P1, BaseParticle* P2)
    {
        P1_ = P1;
        P2_ = P2;
        Next_ = nullptr;
        Prev_ = nullptr;
        Next1_ = nullptr;
        Prev1_ = nullptr;
        Next2_ = nullptr;
        Prev2_ = nullptr;
        index_ = 0;
    }

    /*!
     * \brief Constructor that takes two BaseParticle and the next global PossibleContact in the list and constructs a PossibleContact from that.
     * \param[in] P1 A pointer to the first BaseParticle in this PossibleContact.
     * \param[in] P2 A pointer to the second BaseParticle in this PossibleContact.
     * \param[in] Next A pointer to the PossibleContact that comes after this one in the linked list.
     */
    PossibleContact(BaseParticle* P1, BaseParticle* P2, PossibleContact* Next)
    {
        P1_ = P1;
        P2_ = P2;
        Next_ = Next;
        Prev_ = nullptr;
        if (Next != nullptr)
            index_ = Next->getIndex() + 1;
        else
            index_ = 0;
        
        Next1_ = nullptr;
        Prev1_ = nullptr;
        Next2_ = nullptr;
        Prev2_ = nullptr;
    }

    /*!
     * \brief Constructor that should be used, takes all required information as parameters and sets it in the data-members.
     * \param[in] P1 A pointer to the first BaseParticle in this PossibleContact.
     * \param[in] P2 A pointer to the second BaseParticle in this PossibleContact.
     * \param[in] Next A pointer to the PossibleContact that comes after this one in the linked list.
     * \param[in] Next1 A pointer to the PossibleContact that comes after this one in the linked list associated with the first BaseParticle of this PossibleContact.
     * \param[in] Next2 A pointer to the PossibleContact that comes after this one in the linked list associated with the second BaseParticle of this PossibleContact.
     */
    PossibleContact(BaseParticle* P1, BaseParticle* P2, PossibleContact* Next, PossibleContact* Next1, PossibleContact* Next2)
    {
        P1_ = P1;
        P2_ = P2;
        Next_ = Next;
        Next1_ = Next1;
        Next2_ = Next2;
        Prev_ = 0;
        Prev1_ = 0;
        Prev2_ = 0;
        if (Next != nullptr)
            index_ = Next->getIndex() + 1;
        else
            index_ = 0;
    }
    
    /*!
     * \brief Gets a pointer to the first BaseParticle in this PossibleContact.
     * \return A pointer to the first BaseParticle in this PossibleContact.
     */
    BaseParticle* getP1()
    {
        return P1_;
    }

    /*!
     * \brief Gets a pointer to the second BaseParticle in this PossibleContact.
     * \return A pointer to the second BaseParticle in this PossibleContact.
     */
    BaseParticle* getP2()
    {
        return P2_;
    }

    /*!
     * \brief Given one BaseParticle of the interacting pair, this function gets the other.
     * \param[in] P A pointer to one of the BaseParticle of this PossibleContact.
     * \return The other BaseParticle of this PossibleContact.
     */
    BaseParticle* getOtherParticle(BaseParticle* P)
    {
        if (P1_ == P)
            return P2_;
        else if (P2_ == P)
            return P1_;
        else
        {
            std::cout << "Particle* getOtherParticle(Particle *P), no match for " << P->getIndex() << " posibilities are " << P1_->getIndex() << " and " << P2_->getIndex() << std::endl;
            return nullptr;
        }
    }

    /*!
     * \brief Gets the next PossibleContact in the general linked list of PossibleContact.
     * \return A pointer to the next PossibleContact in the linked list.
     */
    PossibleContact* getNext()
    {
        return Next_;
    }

    /*!
     * \brief Gets the next PossibleContact in the linked list of PossibleContact of the given particle.
     * \param[in] P A pointer to the BaseParticle for which we want to have the next contact of.
     * \return A pointer to the next PossibleContact in the linked list of PossibleContact of the given particle.
     */
    PossibleContact* getNext(BaseParticle* P)
    {
        if (P1_ == P)
            return Next1_;
        else if (P2_ == P)
            return Next2_;
        else
        {
            logger(WARN, "PossibleContact* getNext(Particle* P), no match for %. Possibilities are % and %.", P->getIndex(), P1_->getIndex(), P2_->getIndex());
            return nullptr;
        }
    }

    /*!
     * \brief Gets the previous PossibleContact in the general linked list of PossibleContact.
     * \return A pointer to the previous PossibleContact in the general linked list of PossibleContact.
     */
    PossibleContact* getPrevious()
    {
        return Prev_;
    }

    /*!
     * \brief Gets the previous PossibleContact in the linked list of PossibleContact of the given particle.
     * \param[in] P A pointer to the BaseParticle we want to know the previous PossibleContact for.
     * \return A pointer to the previous PossibleContact in the linked list of PossibleContact of the given particle.
     */
    PossibleContact* getPrevious(BaseParticle* P)
    {
        if (P1_ == P)
            return Prev1_;
        else if (P2_ == P)
            return Prev2_;
        else
        {
            logger(WARN, "PossibleContact* getPrevious(Particle* P), no match for %. Possibilities are % and %.", P->getIndex(), P1_->getIndex(), P2_->getIndex());
            return nullptr;
        }
    }

    /*!
     * \brief Gets the next PossibleContact in the linked list of PossibleContact of the first particle in this PossibleContact.
     * \return A pointer to the next PossibleContact in the linked list of PossibleContact of the first particle in this PossibleContact.
     */
    PossibleContact* getNext1()
    {
        return Next1_;
    }

    /*!
     * \brief Gets the previous PossibleContact in the linked list of PossibleContact of the first particle in this PossibleContact.
     * \return A pointer to the previous PossibleContact in the linked list of PossibleContact of the first particle in this PossibleContact.
     */
    PossibleContact* getPrevious1()
    {
        return Prev1_;
    }

    /*!
     * \brief Gets the next PossibleContact in the linked list of PossibleContact of the second particle in this PossibleContact.
     * \return A pointer to the next PossibleContact in the linked list of PossibleContact of the second particle in this PossibleContact.
     */
    PossibleContact* getNext2()
    {
        return Next2_;
    }

    /*!
     * \brief Gets the previous PossibleContact in the linked list of PossibleContact of the second particle in this PossibleContact.
     * \return A pointer to the previous PossibleContact in the linked list of PossibleContact of the second particle in this PossibleContact.
     */
    PossibleContact* getPrevious2()
    {
        return Prev2_;
    }

    /*!
     * \brief Gets the index of this PossibleContact.
     * \return The index of this PossibleContact.
     */
    int getIndex()
    {
        return index_;
    }
    
    /*!
     * \brief Sets the first particle in this PossibleContact.
     * \param[in] P1 A pointer to the BaseParticle that must be set as the first BaseParticle.
     */
    void setP1(BaseParticle* P1)
    {
        P1_ = P1;
    }

    /*!
     * \brief Sets the second particle in this PossibleContact.
     * \param[in] P2 A pointer to the BaseParticle that must be set as the second BaseParticle.
     */
    void setP2(BaseParticle* P2)
    {
        P2_ = P2;
    }

    /*!
     * \brief Sets the next PossibleContact in the linked list of PossibleContac.
     * \param[in] Next A pointer to the PossibleContact which is the next PossibleContact in the linked list of PossibleContact
     */
    void setNextPosition(PossibleContact* Next)
    {
        Next_ = Next;
    }

    /*!
     * \brief Sets the next PossibleContact in the linked list of PossibleContact of the given particle.
     * \param[in] P A pointer to the BaseParticle for which the next PossibleContact in the linked list must be set for.
     * \param[in] Next A pointer to the PossibleContact which is the next PossibleContact for the given BaseParticle.
     */
    void setNextPosition(BaseParticle* P, PossibleContact* Next)
    {
        if (P1_ == P)
            Next1_ = Next;
        else if (P2_ == P)
            Next2_ = Next;
        else
            logger(WARN, "void setNextPosition(Particle* P,PossibleContact* Next), no match for %. Possibilities are %  and % ", P->getIndex(), P1_->getIndex(), P2_->getIndex());
    }

    /*!
     * \brief Sets the previous PossibleContact in the linked list of PossibleContact of the given particle.
     * \param[in] Prev A pointer to the PossibleContact which is the previous PossibleContact in the linked list of PossibleContact
     */
    void setPreviousPosition(PossibleContact* Prev)
    {
        Prev_ = Prev;
    }

    /*!
     * \brief Sets the previous PossibleContact in the linked list of PossibleContact of the given particle.
     * \param[in] P A pointer to the BaseParticle for which the previous PossibleContact in the linked list must be set for.
     * \param[in] Prev A pointer to the PossibleContact which is the previous PossibleContact for the given BaseParticle.
     */
    void setPreviousPosition(BaseParticle* P, PossibleContact* Prev)
    {
        if (P1_ == P)
            Prev1_ = Prev;
        else if (P2_ == P)
            Prev2_ = Prev;
        else
            logger(WARN, "void setPreviousPosition(Particle* P,PossibleContact* Prev), no match for %. Possibilities are %  and % ", P->getIndex(), P1_->getIndex(), P2_->getIndex());            
    }

    /*!
     * \brief Sets the next PossibleContact in the linked list of PossibleContact of the first particle in this PossibleContact.
     * \param[in] Next1 A pointer to the PossibleContact which is the next PossibleContact for the first BaseParticle.
     */
    void setNextPosition1(PossibleContact* Next1)
    {
        Next1_ = Next1;
    }

    /*!
     * \brief Sets the previous PossibleContact in the linked list of PossibleContact of the first particle in this PossibleContact.
     * \param[in] Prev1 A pointer to the PossibleContact which is the previous PossibleContact for the first BaseParticle.
     */
    void setPreviousPosition1(PossibleContact* Prev1)
    {
        Prev1_ = Prev1;
    }

    /*!
     * \brief Sets the next PossibleContact in the linked list of PossibleContact of the second particle in this PossibleContact.
     * \param[in] Next2 A pointer to the PossibleContact which is the next PossibleContact for the second BaseParticle.
     */
    void setNextPosition2(PossibleContact* Next2)
    {
        Next2_ = Next2;
    }

    /*!
     * \brief Sets the previous PossibleContact in the linked list of PossibleContact of the second particle in this PossibleContact.
     * \param[in] Prev2 A pointer to the PossibleContact which is the previous PossibleContact for the second BaseParticle.
     */
    void setPreviousPosition2(PossibleContact* Prev2)
    {
        Prev2_ = Prev2;
    }
    
    /*!
     * \brief Writes the index of this PossibleContact and the indices of the BaseParticle of this PossibleContact to the given ostream.
     * \param[in,out] os The output stream to which this PossibleContact must be written.
     * \param[in] o The PossibleContact that must be written.
     * \return The output stream to which this PossibleContact has been written.
     */
    friend std::ostream& operator <<(std::ostream& os, const PossibleContact& o)
    {
        os<<o.index_<<" between ("<<o.P1_->getIndex()<<","<<o.P2_->getIndex()<<")";
        return os;
    }

private:
    /*!
     * \brief The index of this PossibleContact.
     */
    int index_;
    /*!
     * \brief A pointer to the first BaseParticle in this PossibleContact.
     */
    BaseParticle* P1_;
    /*!
     * \brief A pointer to the second BaseParticle in this PossibleContact.
     */
    BaseParticle* P2_;
    /*!
     * \brief A pointer to the PossibleContact that comes after this one in the linked list.
     */
    PossibleContact* Next_;
    /*!
     * \brief A pointer to the PossibleContact that comes before this one in the linked list.
     */
    PossibleContact* Prev_;
    /*!
     * \brief A pointer to the PossibleContact that comes after this one in the linked list associated with the first BaseParticle of this PossibleContact.
     */
    PossibleContact* Next1_;
    /*!
     * \brief A pointer to the PossibleContact that comes before this one in the linked list associated with the first BaseParticle of this PossibleContact.
     */
    PossibleContact* Prev1_;
    /*!
     * \brief A pointer to the PossibleContact that comes after this one in the linked list associated with the second BaseParticle of this PossibleContact.
     */
    PossibleContact* Next2_;
    /*!
     * \brief A pointer to the PossibleContact that comes before this one in the linked list associated with the second BaseParticle of this PossibleContact.
     */
    PossibleContact* Prev2_;
    
};

#endif
