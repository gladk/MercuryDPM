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
/*!
 * \file BaseHandler.h
 *
 * Class BaseHandler
 */
#ifndef BASEHANDLER_H
#define BASEHANDLER_H

#include <iostream>
#include <vector>
///\todo TW: someone help: how do I add a logger here?

class DPMBase;

/*!
 * \class BaseHandler
 * \brief Container to store all Object
 * \details The BaseHandler is a container to store all Object. It is implemented by a vector of pointers to the Object.
 */
template<class T>
class BaseHandler
{
public:
    /*!
     * \brief Default BaseHandler constructor, it simply creates an empty BaseHandler.
     */
    BaseHandler();

    /*!
     * \brief Copy constructor, it copies the BaseHandler and all Object it contains.
     * \param[in] BH The BaseHandler that has to be copied.
     */
    BaseHandler(const BaseHandler<T>& BH);

    /*!
     * \brief Destructor, it simply destructs the BaseHandler and all Object it contains.
     */
    virtual ~BaseHandler();

    /*!
     * \brief 
     * \param[in]
     */
    void copyContentsFromOtherHandler(const BaseHandler<T>& BH);

    /*!
     * \brief Creates a copy of a Object and adds it to the BaseHandler.
     * \param[in] O A reference to the Object that has to be copied.
     */
    template<typename U>
    U* copyAndAddObject(const U& O);

    /*!
     * \brief Creates a copy of a Object and adds it to the BaseHandler.
     * \param[in] O A pointer to the Object that has to be copied.
     */
    template<typename U>
    U* copyAndAddObject(U* O);

    /*!
     * \brief Adds a new Object to the BaseHandler.
     * \param[in]
     */
    virtual void addObject(T* O);

//    /*!
//     * \brief Creates a copy of a Object and adds it to the BaseHandler.
//     * \param[in] O A pointer to the Object that has to be copied.
//     */
//    template<typename U>
//    U* addNewObject();

    /*!
     * \brief Removes a Object from the BaseHandler.
     * \param[in]
     */
    virtual void removeObject(unsigned const int id);

    /*!
     * \brief Removes the last Object from the BaseHandler.
     */
    void removeLastObject();

    /*!
     * \brief Empties the whole BaseHandler by removing all Objects.
     */
    void clear();

    /*!
     * \brief Reads Object into the BaseHandler from restart data.
     * \param[in] is The input stream from which the information is read.
     */
    virtual void readObject(std::istream& is)=0;

    void read(std::istream& is);

    /*!
     * \brief Gets a pointer to the Object at the specified index in the BaseHandler.
     * \param[in] id the index of the requested Object.
     * \return A pointer to the requested Object.    
     */
    T* getObject(const unsigned int id);

    /*!
     * \brief Gets a constant pointer to the Object at the specified index in the BaseHandler.
     * \param[in] id the index of the requested Object.
     * \return A constant pointer to the requested Object.
     */
    const T* getObject(const unsigned int id) const;

    /*!
     * \brief Gets a pointer to the last Object in this BaseHandler.
     * \return A pointer to the last Object in the BaseHandler.
     */
    T* getLastObject();

    /*!
     * \brief
     * \return
     */
    const T* getLastObject() const;

    /*!
     * \brief Gets the number of Object in this BaseHandler.
     * \return The number of Objects in this BaseHandler.
     */
    unsigned int getNumberOfObjects() const;

    /*!
     * \brief Gets the storage capacity of this BaseHandler.
     * \return The storage capacity of this BaseHandler.
     */
    unsigned int getStorageCapacity() const;

    /*!
     * \brief Sets the storage capacity of this BaseHandler.
     * \param[in] N The storage capacity the BaseHandler will have   
     */
    void setStorageCapacity(const unsigned int N);

    /*!
     * \brief Gets the begin of the const_iterator over all Object in this BaseHandler.
     * \return A const_iterator pointing to the first Object.
     */
    const typename std::vector<T*>::const_iterator begin() const;

    /*!
     * \brief Gets the begin of the iterator over all BaseBoundary in this BaseHandler.
     * \return A iterator pointing to the first Object.
     */
    const typename std::vector<T*>::iterator begin();

    /*!
     * \brief Gets the end of the const_iterator over all BaseBoundary in this BaseHandler.
     * \return A const_iterator pointing to the last BaseBoundary.
     */
    const typename std::vector<T*>::const_iterator end() const;

    /*!
     * \brief Gets the end of the iterator over all BaseBoundary in this BaseHandler.
     * \return An iterator pointing to the last BaseBoundary.
     */
    const typename std::vector<T*>::iterator end();

    /*!
     * \brief
     * \param[in]
     */
    void setDPMBase(DPMBase* DPMBase);

    /*! 
     * \brief
     */
    DPMBase* getDPMBase();

    DPMBase* getDPMBase() const;

    virtual std::string getName() const = 0;

private:
    /*!
     * \brief The actual list of Object pointers
     */
    std::vector<T*> objects_;

    /*!
     * \brief An integer to keep track of the largest number of objects ever stored in this BaseHandler
     */
    unsigned int maxObjects_;

    /*!
     * \brief identifier for next object created
     */
    unsigned int nextId_;

    /*!
     * \brief A pointer back to the DPMBase class
     */
    DPMBase* DPMBase_;
};

template<class T> BaseHandler<T>::BaseHandler()
{
    DPMBase_ = nullptr;
    clear();
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "BaseHandler<T>::BaseHandler() finished" << std::endl;
#endif
}

template<class T> BaseHandler<T>::BaseHandler(const BaseHandler<T>& BH)
{
    DPMBase_ = nullptr;
    clear();
    copyContentsFromOtherHandler(BH);
#ifdef DEBUG_CONSTRUCTOR
    std::cout << "BaseHandler<T>::BaseHandler(const BaseHandler &BH) finished" << std::endl;
#endif
}

template<class T> BaseHandler<T>::~BaseHandler()
{
    clear();
#ifdef DEBUG_DESTRUCTOR
    std::cout << "BaseHandler<T>::~BaseHandler() finished" << std::endl;
#endif
}

template<class T> void BaseHandler<T>::copyContentsFromOtherHandler(const BaseHandler<T>& BH)
{
    for (typename std::vector<T*>::const_iterator it = BH.begin(); it != BH.end(); it++)
    {
        addObject((*it)->copy());
    }
}

template<class T> template<class U> U* BaseHandler<T>::copyAndAddObject(const U& O)
{
    U* oCopy = O.copy();
    addObject(oCopy);
    return oCopy;
}

template<class T> template<class U> U* BaseHandler<T>::copyAndAddObject(U* O)
{
    U* oCopy = O->copy();
    addObject(oCopy);
    return oCopy;
}

template<class T> void BaseHandler<T>::addObject(T* O)
{
    objects_.push_back(O);
    //Set the index of the particle
    getLastObject()->setIndex(getNumberOfObjects() - 1);
    //set the non changing particle identifier
    getLastObject()->setId(nextId_);
    //Update Id for next particle
    nextId_++;
    //Update the maximum number of Particles
    if (getNumberOfObjects() > maxObjects_)
        maxObjects_ = getNumberOfObjects();
}

//template<class T> template<class U> U* BaseHandler<T>::addNewObject()
//{
//    U* object = new U;
//    addObject(object);
//    return object;
//}

template<class T> void BaseHandler<T>::removeObject(unsigned const int id)
{
    if (id >= getNumberOfObjects())
    {
        std::cerr << "In: void " << getName() << "::removeObject(const unsigned int id) const" << std::endl;
        std::cerr << "No Object exist with index " << id << " number of objects is " << getNumberOfObjects() << std::endl;
        throw;
    }

    //Physically remove Object
    delete objects_[id];

    //If the Object was the last Object nothing has to be done, otherwise some additional work is neccesary
    if (getObject(id) != getLastObject())
    {
        //Copy the pointer to the last Object to position id
        objects_[id] = getLastObject();
        objects_[id]->moveInHandler(id);
    }
    //Remove the (now double) reference to that last Object
    objects_.pop_back();
}

template<class T> void BaseHandler<T>::removeLastObject()
{
    if (getNumberOfObjects() == 0)
    {
        std::cerr << "In: void " << getName() << "::removeLastObject" << std::endl;
        std::cerr << "No Object exist in this BaseHandler" << std::endl;
        throw;
    }
    //Physically removes Object
    delete objects_.back();
    //Remove the (now double) reference to that last Object
    objects_.pop_back();
}

template<class T> void BaseHandler<T>::clear()
{
    while (!objects_.empty())
    {
        removeLastObject();
    }
    nextId_ = 0;
    maxObjects_ = 0;
}

template<class T> void BaseHandler<T>::read(std::istream& is)
{
    clear();
    unsigned int N;
    std::string dummy;
    is >> dummy >> N;
    //std::cout << "In " << getName() << "::read(is): reading in " << N << " objects..." << std::endl; //verbose level
    for (unsigned int i = 0; i < N; i++)
        readObject(is);
}

template<class T> T* BaseHandler<T>::getObject(const unsigned int id)
{
#ifdef NDEBUG
    return objects_[id];
#else
    if (id >= getNumberOfObjects())
    {
        std::cerr << "In: Object* " << getName() << "::getObject(const unsigned int id) const" << std::endl;
        std::cerr << "No Object exist with index " << id << " number of objects is " << getNumberOfObjects() << std::endl;
        throw;
    }
    else
    {
        return objects_[id];
    }
#endif
}

template<class T> const T* BaseHandler<T>::getObject(const unsigned int id) const
        {
#ifdef NDEBUG
    return objects_[id];
#else
    if (id >= getNumberOfObjects())
    {
        std::cerr << "In: Object* " << getName() << "::getObject(const unsigned int id) const" << std::endl;
        std::cerr << "No Object exist with index " << id << " number of objects is " << getNumberOfObjects() << std::endl;
        throw;
    }
    else
    {
        return objects_[id];
    }
#endif
}

template<class T> T* BaseHandler<T>::getLastObject()
{
    return objects_.back();
}

template<class T> const T* BaseHandler<T>::getLastObject() const
{
    return objects_.back();
}

template<class T> unsigned int BaseHandler<T>::getNumberOfObjects() const
{
    return objects_.size();
}

template<class T> unsigned int BaseHandler<T>::getStorageCapacity() const
{
    return objects_.capacity();
}

template<class T> void BaseHandler<T>::setStorageCapacity(const unsigned int N)
{
    objects_.reserve(N);
}

template<class T> const typename std::vector<T*>::const_iterator BaseHandler<T>::begin() const
{
    return objects_.begin();
}

template<class T> const typename std::vector<T*>::iterator BaseHandler<T>::begin()
{
    return objects_.begin();
}

template<class T> const typename std::vector<T*>::const_iterator BaseHandler<T>::end() const
{
    return objects_.end();
}

template<class T> const typename std::vector<T*>::iterator BaseHandler<T>::end()
{
    return objects_.end();
}

template<class T> void BaseHandler<T>::setDPMBase(DPMBase* DPMBase)
{
    DPMBase_ = DPMBase;
}

template<class T> DPMBase* BaseHandler<T>::getDPMBase()
{
#ifdef NDEBUG
    return DPMBase_;
#else
    if (DPMBase_ == nullptr)
    {
        std::cerr << getName() << "::getDPMBase(): pointer to DPMBase class is not set." << std::endl;
        exit(-1);
    }
    return DPMBase_;
#endif
}

template<class T> DPMBase* BaseHandler<T>::getDPMBase() const
{
#ifdef NDEBUG
    return DPMBase_;
#else
    if (DPMBase_ == nullptr)
    {
        std::cerr << getName() << "::getDPMBase(): pointer to DPMBase class is not set." << std::endl;
        exit(-1);
    }
    return DPMBase_;
#endif
}
#endif

