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

/// \bug Had to take out two function from Basehandler to get cmake to compile the code, should take to Dinant about this

#ifndef BASEHANDLER_H
#define BASEHANDLER_H

#include <iostream>
#include <vector>


/// \brief Container to store all Object
/// \details The BaseHandler is a container to store all Object. It is implemented by a vector of pointers to the Object.
template <class T>
class BaseHandler
{
public:
    /// \brief Default BaseHandler constructor, it simply creates an empty BaseHandler.
    BaseHandler()
	{
		clear();
		#ifdef CONSTUCTOR_OUTPUT
			std::cout << "BaseHandler::BaseHandler() finished" << std::endl;
		#endif
	}

    /// \brief Copy constructor, it copies the BaseHandler and all Object it contains.
    /// \param[in] BH The BaseHandler that has te be copied.
    BaseHandler(const BaseHandler<T>& BH)
	{
		clear();
		for (typename std::vector<T*>::const_iterator it=BH.begin();it!=BH.end();it++)
		{
			addObject((*it)->copy());
		}
		#ifdef CONSTUCTOR_OUTPUT
			std::cout << "BaseHandler::BaseHandler(const BaseHandler &BH) finished" << std::endl;
		#endif
	}

    /// \brief Assignment operator.
	/// \param[in] rhs The BaseHandler on the right hand side of the assignment.//
 //   BaseHandler operator = (const BaseHandler<T>& rhs)
 //   {
 //       if (this != &rhs)
 //       {
 //           clear();
 //           for (typename std::vector<T*>::const_iterator it=rhs.begin();it!=rhs.end();it++)
 //           {
 //               addObject((*it)->copy());
 //           }
 //       }
 //       #ifdef OPERATOR_OUTPUT
 //           std::cout << "BaseHandler BaseHandler::operator = (const BoundaryHandler& rhs) finished" << std::endl;
 //       #endif
 //       return *this;
 //   }    

    /// \brief Destructor, it simply destructs the BaseHandler and all Object it contains.
    virtual ~BaseHandler()
	{
		clear();
		#ifdef DESTRUCTOR_OUTPUT
			std::cout << "BaseHandler::~BaseHandler() finished" << std::endl;
		#endif
	}

    /// \brief Creates a copy of a Object and adds it to the BaseHandler.
	/// \param[in] O A reference to the Object that has to be copied.
    void copyAndAddObject(const T& O)
	{
		addObject(O.copy());
	}    

    /// \brief Creates a copy of a Object and adds it to the BaseHandler.
    /// \param[in] O A pointer to the Object (or derived class) that has to be copied.
    void copyAndAddObject(const T* O)
	{
		addObject(O->copy());
	}       

    /// \brief Adds a new Object to the BaseHandler.
    virtual void addObject(T* O)
	{
        objects_.push_back(O);
        //Set the index of the particle
        getLastObject()->set_Index(getNumberOfObjects()-1);
        //set the non changing particle identifier
        getLastObject()->set_Id(nextId_);
        //Update Id for next particle
        nextId_++;
        //Update the maximum number of Particles
        if(getNumberOfObjects()>maxObjects_) maxObjects_=getNumberOfObjects();              
	}

    /// \brief Removes a Object from the BaseHandler.
    /// \details The Object at position id is removed by moving the last Object in the vector to the position of id.
	/// \param[in] id The index of which Object has to be removed from the BaseHandler
    virtual void removeObject(unsigned const int id)
	{
		if (id>=getNumberOfObjects())
		{
			std::cerr<<"In: void BaseHandler::removeObject(const unsigned int id) const"<<std::endl;
			std::cerr<<"No Object exist with index "<<id<<" number of objects is "<<getNumberOfObjects()<<std::endl;
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


    /// \brief Removes the last Object from the BaseHandler.
    void removeLastObject()
	{
		if (getNumberOfObjects()==0)
		{
			std::cerr<<"In: void BaseHandler::removeLastObject const"<<std::endl;
			std::cerr<<"No Object exist in this BaseHandler"<<std::endl;
			throw;			
		}
		//Physically removes Object
		delete objects_.back();
		//Remove the (now double) reference to that last Object
		objects_.pop_back();		
	}

    /// \brief Empties the whole BaseHandler by removing all Object.
    void clear()
    {
		objects_.clear();
        nextId_=0;
        maxObjects_=0;        
	}

    /// \brief Reads Object into the BaseHandler from restart data.
    /// \param[in] is The input stream from which the information is read.
    virtual void readObject(std::istream& is)=0;

    /// \brief Gets a pointer to the Object at the specified index in the BaseHandler.
	/// \param[in] id The index of the requested Object.
	/// \return A pointer to the requested Object.    
    T* getObject(const unsigned int id) const
    {
		if (id>=getNumberOfObjects())
		{
			std::cerr<<"In: Object* BaseHandler::getObject(const unsigned int id) const"<<std::endl;
			std::cerr<<"No Object exist with index "<<id<<" number of objects is "<<getNumberOfObjects()<<std::endl;
			throw;			
		}
		else
		{
			return objects_[id];
		}
	}

    /// \brief Gets a pointer to the last Object in this BaseHandler.
    /// \return A pointer to the last Object in the BaseHandler.
    T* getLastObject() const
    {
		return objects_.back();
	}

    /// \brief Gets the number of Object in this BaseHandler.
    /// \return The number of Objects in this BaseHandler.
    unsigned int getNumberOfObjects() const
    {
		return objects_.size();
	}

    /// \brief Gets the storage capacity of this BaseHandler.
    /// \return The storage capacity of this BaseHandler.
    unsigned int getStorageCapacity() const
    {
		return objects_.capacity();
	}

    /// \brief Sets the storage capacity of this BaseHandler.
    /// \param[in] N The storage capacity the BaseHandler will have   
    void setStorageCapacity(const unsigned int N)
    {
		objects_.reserve(N);
	}
    
    /// \brief Gets the begin of the const_iterator over all Object in this BaseHandler.
    /// \return A const_iterator pointing to the first Object.
    const typename std::vector<T*>::const_iterator begin() const
    {
		return objects_.begin();
	}

    /// \brief Gets the begin of the iterator over all BaseBoundary in this BaseHandler.
    /// \return A iterator pointing to the first Object.
    const typename std::vector<T*>::iterator begin()
    {
		return objects_.begin();
	}
    /// \brief Gets the end of the const_iterator over all BaseBoundary in this BaseHandler.
    /// \return A const_iterator pointing to the last BaseBoundary.
    const typename std::vector<T*>::const_iterator end() const
    {
		return objects_.end();
	}

    /// \brief Gets the end of the iterator over all BaseBoundary in this BaseHandler.
    /// \return An iterator pointing to the last BaseBoundary.
    const typename std::vector<T*>::iterator end()
    {
		return objects_.end();
	}

private:
    ///The actual list of Object pointers
    std::vector<T*> objects_;
    
    ///An integer to keep track of the largest number of objects ever stored in this BaseHandler
    unsigned int maxObjects_;

    ///identifier for next object created
    int nextId_;    
};

#endif

