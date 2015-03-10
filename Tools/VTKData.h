//Copyright (c) 2015, The MercuryDPM Developers Team. All rights reserved.
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

#ifndef TOOLS_VTKDATA_H
#define TOOLS_VTKDATA_H

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <functional>
#include <map>
#include <vector>
#include <type_traits>

//Namespace Detail contains black magic.
namespace Detail {
  /*!
  * Type-erased Internal storage class for a single datamember.
  */
  template<typename T>
  class VTKPointDescriptorEntry 
  {
     std::string name_;
    public:
      /*!
      * \brief initialises this data member with given name.
      * \param fieldName Name of the VTK output field
      */
      VTKPointDescriptorEntry(std::string fieldName)
        : name_(fieldName)
      { 
      }
      
      virtual ~VTKPointDescriptorEntry()
      {
      }
      
      /*!
      * \brief writes this VTKData to the given output stream for a single T
      * \param out the output stream
      * \param t the backing dataobject
      */
      virtual void emit(std::ostream& out, const T& t) const = 0;
      /*!
      * \brief Gives the VTKDataType for VTK.
      * \returns the VTK datatype in string format.
      */
      virtual std::string getTypeName() const = 0;
      /*!
      * \brief Returns the name associated with this field
      */
      std::string getName() const
      {
        return name_;
      }
      /*!
      * \brief Returns the number of components in this type
      */
      virtual std::size_t getNumberOfComponents() const = 0;
  };
  
  /*! This function returns the correct VTK data type based on the template argument */
  template<typename V>
  typename std::enable_if<std::is_floating_point<V>::value, std::string>::type
     toVTKDataType()
  {
    return "Float32";
  }
  
  template<typename V>
  typename std::enable_if<std::is_integral<V>::value && std::is_unsigned<V>::value, std::string>::type
     toVTKDataType()
  {
    return "UInt32";
  }
  
  template<typename V>
  typename std::enable_if<std::is_integral<V>::value && std::is_signed<V>::value, std::string>::type
     toVTKDataType()
  {
    return "Int32";
  }
  
  template<typename V>
  typename std::enable_if<std::is_array<V>::value, std::string>::type
     toVTKDataType()
  {
    return toVTKDataType< typename std::remove_extent<V>::type >();
  }
  
  /*! \brief This function actually writes the correct datatype to ostream. */
  template<typename T, typename V>
  typename std::enable_if<std::is_array<V>::value || std::is_pointer<V>::value, void>::type
     emitProxy(std::ostream& out, const T& t, std::size_t nComponents, V T::*member)
  {
    for (std::size_t i = 0; i < nComponents; i++)
    {
      out << (t.*member)[i] << " ";
    }
  }
  
  template<typename T, typename V>
  typename std::enable_if<!(std::is_array<V>::value || std::is_pointer<V>::value), void>::type
     emitProxy(std::ostream& out, const T& t, std::size_t nComponents, V T::*member)
  {
    out << t.*member << " ";
  }
  
  /*! This class contains the typed information about the descriptor. We need to inherit
   * from the type-erased class to be able to put all these into a single array...
   */  
  template<typename T, typename V>
  class VTKPointDescriptorEntryImpl : public VTKPointDescriptorEntry<T>
  {
    const V T::* member_;
    const std::size_t nComponents_;
    public:
      VTKPointDescriptorEntryImpl(std::string name, V T::*member, std::size_t nComponents)
        : VTKPointDescriptorEntry<T>(name), member_(member), nComponents_(nComponents)
      { }
      
      std::string getTypeName() const override
      {
        return toVTKDataType<V>();
      }
      
      void emit(std::ostream& out, const T& t) const override
      {
        emitProxy(out, t, nComponents_, member_);
      }
      
      std::size_t getNumberOfComponents() const override
      {
        return nComponents_;
      }
  };
}

/*! This class serves as a descriptor to be used for defining VTK Output files.
 * Once again, this class contains a lot of black magic.
 *
 * Usage:
 * VTKPointDescriptor<CLASS> descr;
 *  descr.addDescriptor("Name",  & CLASS::Member )
 *       .addDescriptor("Name2", & CLASS::Member2 );
 */
template<typename T>
class VTKPointDescriptor
{
  std::vector< Detail::VTKPointDescriptorEntry<T>* > entries_;
  Detail::VTKPointDescriptorEntry<T>* positionEntry_;
 public:
  VTKPointDescriptor() : positionEntry_(nullptr)
  {
  }
  
  ~VTKPointDescriptor()
  {
    for (auto * d : entries_)
      delete d;
  }
  
  template<typename DATATYPE>
  typename std::enable_if<std::is_array<DATATYPE>::value, VTKPointDescriptor&>::type
     addProperty(std::string name, DATATYPE T::*m, bool isPrimary = false)
  {
    Detail::VTKPointDescriptorEntry<T> * data
       = new Detail::VTKPointDescriptorEntryImpl<T,DATATYPE>(name, m, std::extent<DATATYPE>::value);
    entries_.push_back(data);
    if (isPrimary)
      positionEntry_ = entries_.back();

    return *this;
  }
  
  template<typename DATATYPE>
  typename std::enable_if<!std::is_array<DATATYPE>::value, VTKPointDescriptor&>::type
     addProperty(std::string name, DATATYPE T::*m, bool isPrimary = false)
  {
    Detail::VTKPointDescriptorEntry<T> * data
       = new Detail::VTKPointDescriptorEntryImpl<T,DATATYPE>(name, m, 1);
    entries_.push_back(data);
    if (isPrimary)
      positionEntry_ = entries_.back();

    return *this;
  }

  template<typename DATATYPE>
  VTKPointDescriptor& addProperty(std::string name, DATATYPE T::*m, std::size_t nComponents, bool isPrimary = false)
  {
    Detail::VTKPointDescriptorEntry<T> * data
       = new Detail::VTKPointDescriptorEntryImpl<T,DATATYPE>(name, m, nComponents);
    entries_.push_back(data);
    if (isPrimary)
      positionEntry_ = entries_.back();
    
    return *this;
  }
  
  
  template<typename VT>
  friend class VTKUnstructuredGrid;
};

/* \brief VTKCollection output file writer
*
* This is templated by the VTKDescriptor DESCR.
* \param T the backing output type
*/
class VTKCollection
{
 std::ofstream outFile_;
 bool hasHeader_;
 bool hasFooter_;
 std::size_t recordID_;

 void writeHeader()
 {
   outFile_ <<
     "<?xml version=\"1.0\"?>\n"
     "<VTKFile type=\"Collection\" version=\"0.1\" byte_order=\"LittleEndian\">\n"
     " <Collection>\n";
   hasHeader_ = true;
 }
 
 void writeFooter()
 {
   outFile_ <<
     " </Collection>\n"
     "</VTKFile>\n";
   hasFooter_ = true;
 }
 
 public:
  VTKCollection(std::string fileName)
    : outFile_(fileName), hasHeader_(false), hasFooter_(false), recordID_(0)
  {
  }
  
  ~VTKCollection()
  {
    if (hasHeader_ && !hasFooter_)
      writeFooter();
  }
  
  /* Returns true if the output file is valid and no error bits are set. */
  operator bool() const
  {
    return outFile_.good();
  }
  
  /* Appends the (relative) path to this index file. */
  void append(std::string filename)
  {
    if (!hasHeader_)
      writeHeader();
      
    outFile_ <<
      "<DataSet group=\"\" part=\"0\" timestep=\"" << recordID_ << "\" file=\"" << filename << "\" />\n";
    
    recordID_++;
  }
};

/*! This function writes single output frames.
 *  this requires an filename and a descriptor.
 *  To actually write it, we need a collection.
 */
template<typename T>
class VTKUnstructuredGrid
{
  const VTKPointDescriptor<T>* descriptor_;
  std::ofstream outFile_;
 public:
  /*! Create a new VTK Unstructured grid file.
   * \param filename The name of the output file
   * \param descr The typedescriptor for T
   */
  VTKUnstructuredGrid(std::string filename, const VTKPointDescriptor<T>* descr)
    : descriptor_(descr), outFile_(filename)
  {
  }
  
  /*! Returns true if this VTK output file is valid and has no errorbits set */
  operator bool() const
  {
    return outFile_.good();
  }
  
  /*! Writes out the container C containing T's, which all get written using
   * the information in the descriptor.
   * C should contain the forward iterable traits, as well as a method size()
   * to give the number of elements stored in this container. All STL containers
   * have this property. Yours should too. */
  template<typename C>
  void write(const C& container)
  {
    outFile_ <<
      "<?xml version=\"1.0\"?>\n"
       "<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"LittleEndian\">\n"
       " <UnstructuredGrid>\n"
       "  <Piece NumberOfPoints=\"" << container.size() << "\" NumberOfCells=\"0\">\n"
       "   <Cells>\n"
       "    <DataArray type=\"Int32\" name=\"connectivity\" format=\"ascii\">\n"
       "       0\n"
       "    </DataArray>\n"
       "    <DataArray type=\"Float32\" name=\"offset\" format=\"ascii\">\n"
       "       0\n"
       "    </DataArray>\n"
       "    <DataArray type=\"UInt8\" name=\"types\" format=\"ascii\">\n"
       "       1\n"
       "    </DataArray>\n"
       "   </Cells>\n"
       "   <Points>\n";
    Detail::VTKPointDescriptorEntry<T>* pDescr = descriptor_->positionEntry_;
    outFile_ << "<DataArray type=\"" << pDescr->getTypeName() << "\" "
                "NumberOfComponents=\"" << pDescr->getNumberOfComponents() << "\" "
                "format=\"ascii\">\n";
    for (const T& mem : container)
    {
      pDescr->emit(outFile_, mem);
    }
    outFile_ << "\n</DataArray>\n";
    
    outFile_ <<
      "    </Points>\n"
      "    <PointData>\n";
    
    for (Detail::VTKPointDescriptorEntry<T>* descr : descriptor_->entries_)
    {
      outFile_ << "<DataArray type=\""    << descr->getTypeName() << "\" "
                  "Name=\""               << descr->getName() << "\" "
                  "NumberOfComponents=\"" << descr->getNumberOfComponents() << "\" "
                  "format=\"ascii\">\n";
      for (const T& mem : container)
      {
        descr->emit(outFile_, mem);
      }
      outFile_ << "\n</DataArray>\n";
    }
    outFile_ <<
      "   </PointData>\n"
      "   <CellData/>\n"
      "  </Piece>\n"
      " </UnstructuredGrid>\n"
      "</VTKFile>\n";
  }
};

#endif