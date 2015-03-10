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

/*!
 * \file BaseHandler.h
 *
 * Class BaseHandler
 */
#ifndef BASEHANDLER_H
#define BASEHANDLER_H

#include <iostream>
#include <vector>
#include "Math/Helpers.h"
#include "Logger.h"

///\todo TW: someone help: how do I add a logger here? Guess its implemented. Did you mean something else TW?

class DPMBase;

/*!
 * \class BaseHandler
 * \brief Container to store all objects that one creates in a simulation. This is where a container to store an umpteen number of pointers is created
 * . Its just pointers everywhere ;)
 * \details The BaseHandler allows one to create a container to store all pointer objects of a templated type T 
 * It is implemented by a (protected) vector of pointers to objects of type T. Once the container is created, the BaseHandler
 * also provides the provision to manipulate the pointers i.e. by accessing, adding, deleting and few more operations by using its
 * member methods. 
 */
template<class T>
class BaseHandler
{
public:
    /*!
     * \brief Default BaseHandler constructor, it creates an empty BaseHandler and assigns DPMBase_ to a
     * null pointer.
     */
    BaseHandler();

    /*!
     * \brief Copy constructor, it copies the content of BaseHandler it contains.
     */
    BaseHandler(const BaseHandler<T>& BH);

    /*!
     * \brief Destructor, it destructs the BaseHandler and all Object it contains.
     */
    virtual ~BaseHandler();

    /*!
     * \brief Function that copies the contents (vector of pointers, maxObject_, nextId_, DPMBase_) from one 
     * handler (container) to the other.
     */
    void copyContentsFromOtherHandler(const BaseHandler<T>& BH);

    /*!
     * \brief Creates a copy of a Object and adds it to the BaseHandler.
     */
    template<typename U>
    U* copyAndAddObject(const U& O);

    /*!
     * \brief Creates a copy of a Object and adds it to the BaseHandler.
     */
    template<typename U>
    U* copyAndAddObject(U* O);

    /*!
     * \brief Adds a new Object to the BaseHandler.
     */
    virtual void addObject(T* O);

    /*!
     * \brief Removes an Object from the BaseHandler.
     */
    virtual void removeObject(unsigned const int id);

    /*!
     * \brief Removes the last Object from the BaseHandler.
     */
    void removeLastObject();

    /*!
     * \brief Empties the whole BaseHandler by removing all Objects and setting all other variables to 0.
     */
    void clear();

    /*!
     * \brief Reads Object into the BaseHandler from restart data.
     * \param[in] is The input stream from which the information is read.
     */
    virtual void readObject(std::istream& is) = 0;

    /*!
     *  \brief Reads all objects from restart data.
     */
    void read(std::istream& is);

    /*!
     * \brief Gets a pointer to the Object at the specified index in the BaseHandler. 
     */
    T* getObjectById(const unsigned int id);

    /*!
     * \brief Gets a pointer to the Object at the specified index in the BaseHandler.   
     */
    T* getObject(const unsigned int id);

    /*!
     * \brief Gets a constant pointer to the Object at the specified index in the BaseHandler.
     */
    const T* getObject(const unsigned int id) const;

    /*!
     * \brief Gets a pointer to the last Object in this BaseHandler.
     */
    T* getLastObject();

    /*!
     * \brief Gets a constant pointer to the last Object in this BaseHandler.
     */
    const T* getLastObject() const;

    /*!
     * \brief Gets the number of Object in this BaseHandler. 
     */
    unsigned int getNumberOfObjects() const;

    /*!
     * \brief Gets the storage capacity of this BaseHandler.
     */
    unsigned int getStorageCapacity() const;

    /*!
     * \brief Sets the storage capacity of this BaseHandler.
     */
    void setStorageCapacity(const unsigned int N);

    /*!
     * \brief Gets the begin of the const_iterator over all Object in this BaseHandler.
     */
    const typename std::vector<T*>::const_iterator begin() const;

    /*!
     * \brief Gets the begin of the iterator over all BaseBoundary in this BaseHandler.
     */
    const typename std::vector<T*>::iterator begin();

    /*!
     * \brief Gets the end of the const_iterator over all BaseBoundary in this BaseHandler.
     */
    const typename std::vector<T*>::const_iterator end() const;

    /*!
     * \brief Gets the end of the iterator over all BaseBoundary in this BaseHandler.
     */
    const typename std::vector<T*>::iterator end();

    /*!
     * \brief Sets the problem that is solved using this handler.
     */
    void setDPMBase(DPMBase* DPMBase);

    /*! 
     * \brief Gets the problem that is solved using this handler.
     */
    DPMBase* getDPMBase();

    /*! 
     * \brief Gets the problem that is solved using this handler and does not change the class.
     */
    DPMBase* getDPMBase() const;
    
    /*!\brief Gets the name of this handler.
     * \return A string that contains the name of the handler.
     */
    virtual std::string getName() const = 0;

protected:
    //This should not be private. That's just annoying. @dducks
    /*!
     * \brief The actual list of Object pointers
     * 
     * The list of Object pointers. This handler is responsible for the memory-deallocation
     * of these objects.
     */
    std::vector<T*> objects_;
    
private:
    /*!
     * \brief An integer to keep track of the largest number of objects ever stored in this BaseHandler
     */
    unsigned int maxObjects_;

    /*!
     * \brief identifier for next object created
     */
    unsigned int nextId_;

    /*!
     * \brief A pointer back to the DPMBase class.
     * 
     * Please note that this pointer back to the DPMBase class is a "shared" pointer
     * and should not be deallocated by this class.
     */
    DPMBase* DPMBase_;
};

/*!
 * 
 */
template<class T> BaseHandler<T>::BaseHandler()
{
    DPMBase_ = nullptr;
    clear();
#ifdef DEBUG_CONSTRUCTOR
    logger(DEBUG, "Basehandler<T>::BaseHandler() finished");
    //std::cout << "BaseHandler<T>::BaseHandler() finished" << std::endl;
#endif
}

///\param[in] BH A reference to the BaseHandler that has to be copied.
template<class T> BaseHandler<T>::BaseHandler(const BaseHandler<T>& BH)
{
    DPMBase_ = nullptr;
    clear();
    copyContentsFromOtherHandler(BH);
#ifdef DEBUG_CONSTRUCTOR
    logger(DEBUG,"BaseHandler<T>::BaseHandler(const BaseHandler &BH) finished");
#endif
}

template<class T> BaseHandler<T>::~BaseHandler()
{
    clear();
#ifdef DEBUG_DESTRUCTOR
    logger(DEBUG, "BaseHandler<T>::~BaseHandler() finished");
#endif
}

///\param[in] BH A reference to the BaseHandler of which the objects have to be copied.
template<class T> void BaseHandler<T>::copyContentsFromOtherHandler(const BaseHandler<T>& BH)
{
    for (auto obj : BH.objects_) 
    {
        addObject(obj->copy());
    }
}

///\param[in] BH A reference to the BaseHandler of which the objects have to be copied.
template<class T> template<class U> U* BaseHandler<T>::copyAndAddObject(const U& O)
{
    U* oCopy = O.copy();
    addObject(oCopy);
    return oCopy;
}

///\param[in] O A reference to the Object that has to be copied.
template<class T> template<class U> U* BaseHandler<T>::copyAndAddObject(U* O)
{
    U* oCopy = O->copy();
    addObject(oCopy);
    return oCopy;
}

///\param[in] O A pointer to the object that must be added.
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

/*!
 * This methods removes a particle. This methods invalidates ANY iterators to
 * particles in this container. This method may shuffle the order of objects in this container.
 * \param[in] id An unsigned integer that gives the id of the Object that has to be removed.
 */
template<class T> void BaseHandler<T>::removeObject(unsigned const int id)
{
    if (id >= getNumberOfObjects())
    {
        logger(ERROR, "In: void %::removeOject(const unsigned int id) const, no object exists with index %, number of objects is %", getName(), id, getNumberOfObjects());
        return;
    }
    
    //Okay, this function deletes the particle. Now, the problem is that
    //particles store their position in the handler (@dducks: do they?)
    //which means that we would absolutely destroy performance if we took
    //something out in the middle. Now, what we'll do is swap our particle
    //with the last one (in case it already is, it is invariant); then
    //remove the last one.
    //So, we want the last index.
    std::size_t lastIdx = objects_.size() - 1;
  
    //So, step one, retrieve the pointer
    T* objectToDelete = objects_[id];

    if (id != lastIdx) //Are we not the last object?
    {
      //well.. let's swap.
      T* objectToMove = objects_[lastIdx];

      objects_[id] = objectToMove; //place it back
      objects_[lastIdx] = objectToDelete; //Just to make sure.

      //and notify it of the change.
      objects_[id]->moveInHandler(id);
      //Even though we are going to delete this particle,
      //we still need to keep it consistent.
      objects_[lastIdx]->moveInHandler(lastIdx);
    }
    
    //And _NOW_ we delete it.
    delete objectToDelete;
      
    //And clear it from the backing container.
    objects_.pop_back();
}

template<class T> void BaseHandler<T>::removeLastObject()
{
    if (getNumberOfObjects() == 0)
    {
        logger(WARN, "In: void %::removeLastObject, no Object exists in this BaseHandler.", getName());
        return;
    }
    //Physically removes Object
    delete objects_.back();
    //Remove the (now double) reference to that last Object
    objects_.pop_back();
}

///Delete all objects stored in objects_ and set the maximum number of objects that
///have been in this container to 0, and set the Id of the next object that will be added to 0.
template<class T> void BaseHandler<T>::clear()
{
    
    for (auto obj : objects_) 
    {
        delete obj;
    }
    objects_.clear();
    
    nextId_ = 0;
    maxObjects_ = 0;
}

/// \param[in] is The input stream from which the information is read.
template<class T> void BaseHandler<T>::read(std::istream& is)
{
    clear();
    unsigned int N;
    std::string dummy;
    is >> dummy;
    std::stringstream line(std::stringstream::in | std::stringstream::out);
	helpers::getLineFromStringStream(is, line);
    line >> N;
    logger(VERBOSE, "In %::read(is): reading in % objects.", getName(), N);
    for (unsigned int i = 0; i < N; i++)
    {
        readObject(is);
    }
}

/// \param[in] id The id of the requested Object.
/// \return A pointer to the Object with the correct Id.   
///Gets an object with the identity id. Please note that the object with this identity
///does not have to be at place id in the vector of Object objects_.
template<class T> T* BaseHandler<T>::getObjectById(const unsigned int id)
{
    // Usually, the id and the index into the backing storage matches
    // So check this postion first!
    // dducks: Can't we guarantee more? That should speed up searches. 
    if (id < objects_.size() && objects_[id]->getId() == id)
    {
        return objects_[id]; //There is a hit, return early
    }
    
    for (T* obj : objects_ ) //Search for the correct id, since it wasn't where
    {                        // we expected it. Just use a linear search..
        if (obj->getId() == id) //Found it, so return!
            return obj;
    }
    logger(ERROR, "Object with id % could not be found.", id);
    return nullptr;
}

///\param[in] id the index of the requested Object.
///\return A pointer to the requested Object.  
template<class T> T* BaseHandler<T>::getObject(const unsigned int id)
{
#ifdef NDEBUG
    return objects_[id];
#else
    if (id >= getNumberOfObjects())
    {
        logger(ERROR, "Object couldn't be found because ID (%) is higher than number of objects. (Handler = %)", id, getName());
        return nullptr;
    }
    else
    {
        return objects_[id];
    }
#endif
}

/// \param[in] id the index of the requested Object.
/// \return A constant pointer to the requested Object.
template<class T> const T* BaseHandler<T>::getObject(const unsigned int id) const
{
#ifdef NDEBUG
    return objects_[id];
#else
    if (id >= getNumberOfObjects())
    {
        logger(ERROR, "In: Object* % :getObject(const unsigned int id) const. No Object exist with index % number of objects is %", getName(), id, getNumberOfObjects());
        return nullptr;
    }
    else
    {
        return objects_[id];
    }
#endif
}

///\return A pointer to the last Object in the BaseHandler.
template<class T> T* BaseHandler<T>::getLastObject()
{
    return objects_.back();
}

///\return A constant pointer to the last Object in the BaseHandler.
template<class T> const T* BaseHandler<T>::getLastObject() const
{
    return objects_.back();
}

///\return The number of Objects in this BaseHandler.
///\todo TW: should we change this to size_type?
template<class T> unsigned int BaseHandler<T>::getNumberOfObjects() const
{
    return objects_.size();
}

///\return The storage capacity of this BaseHandler.
template<class T> unsigned int BaseHandler<T>::getStorageCapacity() const
{
    return objects_.capacity();
}

///\param[in] N The storage capacity the BaseHandler will have 
template<class T> void BaseHandler<T>::setStorageCapacity(const unsigned int N)
{
    objects_.reserve(N);
}

///\return A const_iterator pointing to the first Object.
template<class T> const typename std::vector<T*>::const_iterator BaseHandler<T>::begin() const
{
    return objects_.begin();
}

///\return A iterator pointing to the first Object.
template<class T> const typename std::vector<T*>::iterator BaseHandler<T>::begin()
{
    return objects_.begin();
}

///\return A const_iterator pointing to the last BaseBoundary.
template<class T> const typename std::vector<T*>::const_iterator BaseHandler<T>::end() const
{
    return objects_.end();
}

///\return An iterator pointing to the last BaseBoundary.
template<class T> const typename std::vector<T*>::iterator BaseHandler<T>::end()
{
    return objects_.end();
}

///\param[in] DPMBase A pointer to a DPMBase, which is the superclass for all problem descriptions.
template<class T> void BaseHandler<T>::setDPMBase(DPMBase* DPMBase)
{
    DPMBase_ = DPMBase;
}

///\return A pointer to the DPMBase (problem descriptor) that is using this handler.
template<class T> DPMBase* BaseHandler<T>::getDPMBase()
{
#ifdef NDEBUG
    return DPMBase_;
#else
    if (DPMBase_ == nullptr)
    {
        logger(ERROR, "%::getDPMBase(): pointer to DPMBase class is not set.", getName());
    }
    return DPMBase_;
#endif
}

///\return A pointer to the DPMBase (problem descriptor) that is using this handler.
template<class T> DPMBase* BaseHandler<T>::getDPMBase() const
{
#ifdef NDEBUG
    return DPMBase_;
#else
    if (DPMBase_ == nullptr)
    {
        logger(ERROR, "%::getDPMBase(): pointer to DPMBase class is not set.", getName());
    }
    return DPMBase_;
#endif
}
#endif

