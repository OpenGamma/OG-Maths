/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include <string.h>
#include "terminal.hh"
#include "exceptions.hh"
#include "visitor.hh"
#include "warningmacros.h"
#include "equals.hh"
#include "iss.hh"
#include "debug.h"
#include "convertto.hh"
#include <iostream>
#include <iostream>
#include <iomanip>

namespace librdag {

/**
 * OGTerminal
 */

real16*
OGTerminal::toReal16Array() const
{
  throw rdag_error("OGTerminal::toReal16Array() called but not implemented");
}

complex16*
OGTerminal::toComplex16Array() const
{
  throw rdag_error("OGTerminal::toComplex16Array() called but not implemented");
}

real16**
OGTerminal::toReal16ArrayOfArrays() const
{
  throw rdag_error("OGTerminal::toReal16ArrayOfArrays() called but not implemented");
}

complex16**
OGTerminal::toComplex16ArrayOfArrays() const
{
  throw rdag_error("OGTerminal::toComplex16ArrayOfArrays() called but not implemented");
}

OGTerminal::Ptr
OGTerminal::asOGTerminal() const
{
  return static_pointer_cast<const OGTerminal, const OGNumeric>(shared_from_this());
}

bool
OGTerminal::operator==(const OGTerminal::Ptr& other) const
{
  return this->equals(other);
}

bool
OGTerminal::operator!=(const OGTerminal::Ptr& other) const
{
  return !(this->equals(other));
}

detail::FuzzyCompareOGTerminalContainer
OGTerminal::operator~(void) const
{
  return detail::FuzzyCompareOGTerminalContainer(asOGTerminal());
}

bool OGTerminal::operator==(const detail::FuzzyCompareOGTerminalContainer& thing) const
{
  return this->fuzzyequals(OGTerminal::Ptr{thing.getTerminal()});
}

bool OGTerminal::operator!=(const detail::FuzzyCompareOGTerminalContainer& thing) const
{
  return !(this->fuzzyequals(OGTerminal::Ptr{thing.getTerminal()}));
}

OGTerminal::OGTerminal()
{
  _converter = new ConvertTo();
}

OGTerminal::~OGTerminal()
{
  delete _converter;
}

const ConvertTo *
OGTerminal::getConvertTo() const
{
  return this->_converter;
}

bool
OGTerminal::mathsequals(const OGTerminal::Ptr& other) const
{
  return mathsequals(other, FuzzyEquals_default_maxabserror, FuzzyEquals_default_maxrelerror);
}

bool
OGTerminal::mathsequals(const OGTerminal::Ptr& other, real16 maxabserror, real16 maxrelerror) const
{
  OGTerminal::Ptr thisconv; 
  OGTerminal::Ptr otherconv;
  bool ret = false;
  if((this->getRows()!=other->getRows())||(this->getCols()!=other->getCols()))
  {
    return ret;
  }
  if(isReal(static_pointer_cast<const OGTerminal, const OGNumeric>(shared_from_this())) && isReal(other))
  {
    otherconv = other->asFullOGRealMatrix();
    thisconv = this->asFullOGRealMatrix();
  }
  else
  {
    otherconv = other->asFullOGComplexMatrix();
    thisconv = this->asFullOGComplexMatrix();
  }
  if(thisconv->fuzzyequals(otherconv, maxabserror, maxrelerror))
  {
    ret = true;
  }
  return ret;
}

bool
OGTerminal::operator%(const OGTerminal::Ptr& other) const
{
  return this->mathsequals(other);
}



/**
 * FuzzyCompareOGTerminalContainer
 */
detail::FuzzyCompareOGTerminalContainer::FuzzyCompareOGTerminalContainer(const OGTerminal::Ptr& terminal): _terminal{terminal} {}

detail::FuzzyCompareOGTerminalContainer::~FuzzyCompareOGTerminalContainer(){}

const shared_ptr<const OGTerminal>
detail::FuzzyCompareOGTerminalContainer::getTerminal() const
{
  return _terminal;
}


/**
 * OGScalar
 */

template<typename T>
typename OGScalar<T>::Ptr
OGScalar<T>::create(T data)
{
  return OGScalar<T>::Ptr{new OGScalar<T>{data}};
}

template<typename T>
OGScalar<T>::OGScalar(T data)
{
  this->_value=data;
}

template<typename T>
int
OGScalar<T>::getRows() const
{
  return 1;
}

template<typename T>
int
OGScalar<T>::getCols() const
{
  return 1;
}

template<typename T>
int
OGScalar<T>::getDatalen() const
{
  return 1;
}

template<typename T>
void
OGScalar<T>::accept(Visitor &v) const
{
  v.visit(this);
}

template<typename T>
T
OGScalar<T>::getValue() const
{
  return this->_value;
}

template<typename T>
T**
OGScalar<T>::toArrayOfArrays() const
{
  T ** tmp = new T * [1];
  tmp[0] = new T[1];
  tmp[0][0] = this->getValue();
  return tmp;
}

template<typename T>
bool
OGScalar<T>::equals(const OGTerminal::Ptr& other) const
{
  if(this->getType()!=other->getType())
  {
    return false;
  }
  if(static_pointer_cast<const OGScalar<T>, const OGTerminal>(other)->getValue()!=this->_value)
  {
    return false;
  }
  return true;
}

template<typename T>
bool
OGScalar<T>::fuzzyequals(const OGTerminal::Ptr& other) const
{
  return this->fuzzyequals(other,FuzzyEquals_default_maxabserror,FuzzyEquals_default_maxrelerror);
}

template<typename T>
bool
OGScalar<T>::fuzzyequals(const OGTerminal::Ptr& other, real16 maxabserror, real16 maxrelerror) const
{
  if(this->getType()!=other->getType())
  {
    return false;
  }
  if(!SingleValueFuzzyEquals(static_pointer_cast<const OGScalar<T>, const OGTerminal>(other)->getValue(),this->_value,maxabserror,maxrelerror))
  {
    return false;
  }
  return true;
}

template<typename T>
OGRealMatrix::Ptr
OGScalar<T>::asFullOGRealMatrix() const
{
  throw rdag_error("Cannot asFullOGRealMatrix() template OGScalar<T> class.");
}

template<typename T>
OGComplexMatrix::Ptr
OGScalar<T>::asFullOGComplexMatrix() const
{
  throw rdag_error("Cannot asFullOGComplexMatrix() template OGScalar<T> class.");
}

template<typename T>
OGTerminal::Ptr
OGScalar<T>::createOwningCopy() const
{
  throw rdag_error("Cannot createOwningCopy() template OGScalar<T> class.");
}

template<typename T>
OGTerminal::Ptr
OGScalar<T>::createComplexOwningCopy() const
{
  throw rdag_error("Cannot createComplexOwningCopy() template OGScalar<T> class.");
}

template<typename T>
void
OGScalar<T>::debug_print() const
{
  cout << "OGScalar<T>: " << setprecision(__DEBUG_PRECISION) << this->getValue() << endl;
}

template<typename T>
OGNumeric::Ptr
OGScalar<T>::copy() const
{
  throw rdag_error("Cannot copy() template OGScalar<T> class.");
}


template class OGScalar<real16>;
template class OGScalar<complex16>;
template class OGScalar<int>;

/**
 * OGRealScalar
 */

OGRealScalar::OGRealScalar(real16 data): OGScalar<real16>(data) {}

OGRealScalar::Ptr
OGRealScalar::create(real16 data)
{
  return OGRealScalar::Ptr{new OGRealScalar{data}};
}

real16**
OGRealScalar::toReal16ArrayOfArrays() const
{
  return this->toArrayOfArrays();
}

complex16**
OGRealScalar::toComplex16ArrayOfArrays() const
{
  complex16 ** tmp = new complex16 * [1];
  tmp[0] = new complex16[1];
  tmp[0][0] = this->getValue();
  return tmp;
}

OGNumeric::Ptr
OGRealScalar::copy() const
{
  return OGNumeric::Ptr{new OGRealScalar(this->getValue())};
}

OGRealScalar::Ptr
OGRealScalar::asOGRealScalar() const
{
  return static_pointer_cast<const OGRealScalar, const OGNumeric>(shared_from_this());
}

void
OGRealScalar::debug_print() const
{
  printf("OGRealScalar(%f)\n", this->getValue());
}

ExprType_t
OGRealScalar::getType() const
{
  return REAL_SCALAR_ENUM;
}

OGRealMatrix::Ptr
OGRealScalar::asFullOGRealMatrix() const
{
  return getConvertTo()->convertToOGRealMatrix(asOGRealScalar());
}

OGComplexMatrix::Ptr
OGRealScalar::asFullOGComplexMatrix() const
{
  return getConvertTo()->convertToOGComplexMatrix(asOGRealScalar());
}

OGTerminal::Ptr
OGRealScalar::createOwningCopy() const
{
  return OGRealScalar::create(this->getValue());
}

OGTerminal::Ptr
OGRealScalar::createComplexOwningCopy() const
{
  return OGComplexScalar::create(this->getValue());
}

/**
 * OGComplexScalar
 */

OGComplexScalar::OGComplexScalar(complex16 data): OGScalar<complex16>(data) {}

OGComplexScalar::Ptr
OGComplexScalar::create(complex16 data)
{
  return OGComplexScalar::Ptr{new OGComplexScalar{data}};
}

complex16**
OGComplexScalar::toComplex16ArrayOfArrays() const
{
  return this->toArrayOfArrays();
}

OGNumeric::Ptr
OGComplexScalar::copy() const
{
  return OGNumeric::Ptr{new OGComplexScalar(this->getValue())};
}

OGComplexScalar::Ptr
OGComplexScalar::asOGComplexScalar() const
{
  return static_pointer_cast<const OGComplexScalar, const OGNumeric>(shared_from_this());
}

void
OGComplexScalar::debug_print() const
{
  printf("OGComplexScalar(%f+%fi)\n", this->getValue().real(), this->getValue().imag());
}

ExprType_t
OGComplexScalar::getType() const
{
  return COMPLEX_SCALAR_ENUM;
}

OGRealMatrix::Ptr
OGComplexScalar::asFullOGRealMatrix() const
{
  throw rdag_error("Cannot represent complex data in linear memory of type real16");
}

OGComplexMatrix::Ptr
OGComplexScalar::asFullOGComplexMatrix() const
{
  return getConvertTo()->convertToOGComplexMatrix(asOGComplexScalar());
}

OGTerminal::Ptr
OGComplexScalar::createOwningCopy() const
{
  return OGComplexScalar::create(this->getValue());
}

OGTerminal::Ptr
OGComplexScalar::createComplexOwningCopy() const
{
  return createOwningCopy();
}


/**
 * OGIntegerScalar
 */

OGIntegerScalar::OGIntegerScalar(int data): OGScalar<int>(data) {}

OGIntegerScalar::Ptr
OGIntegerScalar::create(int data)
{
  return OGIntegerScalar::Ptr{new OGIntegerScalar{data}};
}

OGNumeric::Ptr
OGIntegerScalar::copy() const
{
  return OGNumeric::Ptr{new OGIntegerScalar(this->getValue())};
}

OGIntegerScalar::Ptr
OGIntegerScalar::asOGIntegerScalar() const
{
  return static_pointer_cast<const OGIntegerScalar, const OGNumeric>(shared_from_this());
}

void
OGIntegerScalar::debug_print() const
{
  printf("OGIntegerScalar(%d)\n", this->getValue());
}

ExprType_t
OGIntegerScalar::getType() const
{
  return INTEGER_SCALAR_ENUM;
}

OGRealMatrix::Ptr
OGIntegerScalar::asFullOGRealMatrix() const
{
  return getConvertTo()->convertToOGRealMatrix(asOGIntegerScalar());
}

OGComplexMatrix::Ptr
OGIntegerScalar::asFullOGComplexMatrix() const
{
  return getConvertTo()->convertToOGComplexMatrix(asOGIntegerScalar());
}

OGTerminal::Ptr
OGIntegerScalar::createOwningCopy() const
{
  return OGIntegerScalar::create(this->getValue());
}

OGTerminal::Ptr
OGIntegerScalar::createComplexOwningCopy() const
{
  return OGComplexScalar::create(this->getValue());
}


/**
 * OGArray
 */

template<typename T>
OGArray<T>::~OGArray()
{
  if(this->getDataAccess() == OWNER)
  {
    delete [] this->getData();
  }
}

template<typename T>
T*
OGArray<T>::getData() const
{
  return _data;
}

template<typename T>
T*
OGArray<T>::toArray() const
{
  T* tmp = new T[_datalen];
  memcpy (tmp, _data, sizeof(T)*_datalen );
  return tmp;
}

template<typename T>
int
OGArray<T>::getRows() const
{
  return _rows;
}

template<typename T>
int
OGArray<T>::getCols() const
{
  return _cols;
}

template<typename T>
int
OGArray<T>::getDatalen() const
{
  return _datalen;
}

template<typename T>
DATA_ACCESS
OGArray<T>::getDataAccess() const
{
  return _data_access;
}

template<typename T>
void
OGArray<T>::setData(T * data)
{
  _data = data;
}

template<typename T>
void
OGArray<T>::setRows(int rows)
{
  if(rows<0)
  {
    throw rdag_error("Attempt to set rows to a negative value");
  }
  _rows = rows;
}

template<typename T>
void
OGArray<T>::setCols(int cols)
{
  if(cols<0)
  {
    throw rdag_error("Attempt to set cols to a negative value");
  }
  _cols = cols;
}

template<typename T>
void
OGArray<T>::setDatalen(int datalen)
{
  if(datalen<0)
  {
    throw rdag_error("Attempt to set dalaten to a negative value");
  }
  _datalen = datalen;
}

template<typename T>
bool
OGArray<T>::fundamentalsEqual(const OGTerminal::Ptr& other) const
{
  if(this->getType()!=other->getType())
  {
    return false;
  }
  const shared_ptr<const OGArray<T>> typetwiddle = static_pointer_cast<const OGArray<T>, const OGTerminal>(other);
  if(typetwiddle ->getRows()!=this->getRows())
  {
    return false;
  }
  if(typetwiddle ->getCols()!=this->getCols())
  {
    return false;
  }
  return true;
}

template<typename T>
bool
OGArray<T>::fundamentalsEqual(const OGTerminal* other) const
{
  if(this->getType()!=other->getType())
  {
    return false;
  }
  const OGArray<T>* typetwiddle = static_cast<const OGArray<T>*>(other);
  if(typetwiddle ->getRows()!=this->getRows())
  {
    return false;
  }
  if(typetwiddle ->getCols()!=this->getCols())
  {
    return false;
  }
  return true;
}


template<typename T>
bool
OGArray<T>::equals(const OGTerminal::Ptr& other) const
{
  if(!fundamentalsEqual(other)) return false;
  const shared_ptr<const OGArray> typetwiddle = static_pointer_cast<const OGArray, const OGTerminal>(other);
  if(!ArrayBitEquals(typetwiddle->getData(),this->getData(),this->getDatalen()))
  {
    return false;
  }
  return true;
}

template<typename T>
bool
OGArray<T>::fuzzyequals(const OGTerminal::Ptr& other,real16 maxabserror,real16 maxrelerror) const
{
  if(!fundamentalsEqual(other)) return false;
  const shared_ptr<const OGArray<T>> typetwiddle = static_pointer_cast<const OGArray<T>, const OGTerminal>(other);
  if(!ArrayFuzzyEquals(typetwiddle->getData(),this->getData(),this->getDatalen(),maxabserror,maxrelerror))
  {
    return false;
  }
  return true;
}

template<typename T>
bool
OGArray<T>::fuzzyequals(const OGTerminal::Ptr& other) const
{
  return fuzzyequals(other, FuzzyEquals_default_maxabserror, FuzzyEquals_default_maxrelerror);
}

template<typename T>
void
OGArray<T>::setDataAccess(DATA_ACCESS access_spec)
{
  _data_access = access_spec;
}

template<typename T>
void
OGArray<T>::
debug_print() const
{
  throw rdag_error("Cannot debug_print abstract OGArray<T> class.");
}

template<typename T>
OGNumeric::Ptr
OGArray<T>::
copy() const
{
  throw rdag_error("Cannot copy abstract OGArray<T> class.");
}

template<typename T>
void
OGArray<T>::
accept(Visitor SUPPRESS_UNUSED &v) const
{
  throw rdag_error("Cannot visit abstract OGArray<T> class.");
}

template<typename T>
OGRealMatrix::Ptr
OGArray<T>::
asFullOGRealMatrix() const
{
  // this is a default backstop impl whilst waiting for a traited version
  throw rdag_error("Cannot convert asFullOGRealMatrix(), concrete implementation class required for operation.");
}

template<typename T>
OGComplexMatrix::Ptr
OGArray<T>::
asFullOGComplexMatrix() const
{
  // this is a default backstop impl whilst waiting for a traited version
  throw rdag_error("Cannot convert asFullOGComplexMatrix(), concrete implementation class required for operation.");
}

template<typename T>
OGTerminal::Ptr
OGArray<T>::
createOwningCopy() const
{
  // this is a default backstop impl whilst waiting for a traited version
  throw rdag_error("Cannot createOwningCopy(), concrete implementation class required for operation.");
}

template<typename T>
OGTerminal::Ptr
OGArray<T>::
createComplexOwningCopy() const
{
  // this is a default backstop impl whilst waiting for a traited version
  throw rdag_error("Cannot createComplexOwningCopy(), concrete implementation class required for operation.");
}

template class OGArray<real16>;
template class OGArray<complex16>;

/**
 * OGMatrix
 */

template<typename T>
OGMatrix<T>::OGMatrix(T* data, int rows, int cols, DATA_ACCESS access_spec)
{
  if (data == nullptr)
  {
    throw rdag_error("Null data pointer passed to Matrix constructor");
  }
  this->setData(data);
  this->setRows(rows);
  this->setCols(cols);
  this->setDatalen(rows*cols);
  this->setDataAccess(access_spec);
}

template<typename T>
typename OGMatrix<T>::Ptr
OGMatrix<T>::create(T* data, int rows, int cols, DATA_ACCESS access_spec)
{
  return OGMatrix<T>::Ptr{new OGMatrix<T>{data, rows, cols, access_spec}};
}

template<typename T>
void
OGMatrix<T>::accept(Visitor &v) const
{
  v.visit(this);
}

template<typename T>
T**
OGMatrix<T>::toArrayOfArrays() const
{
  int const rows = this->getRows();
  int const cols = this->getCols();
  T * const data = this->getData();
  T ** tmp = new T * [rows];
  for(int i=0; i < rows; i++)
  {
    tmp[i] = new T [cols];
    for(int j = 0; j < cols; j++)
    {
      tmp[i][j] = data[j*rows+i];
    }
  }
  return tmp;
}

template<typename T>
OGNumeric::Ptr
OGMatrix<T>::copy() const
{
  return OGMatrix<T>::create(this->getData(), this->getRows(), this->getCols());
}

template<typename T>
void
OGMatrix<T>::debug_print() const
{
  cout << std::endl << "OGMatrix<T>:" << std::endl;
  cout << std::endl << "OWNING STATUS:" << this->getDataAccess() << std::endl;
  int rows = this->getRows();
  for(int i = 0 ; i < rows; i++)
  {
    for(int j = 0 ; j < this->getCols()-1; j++)
    {
      cout << setprecision(__DEBUG_PRECISION) << this->getData()[j*this->getRows()+i] << ", ";
    }
    cout << setprecision(__DEBUG_PRECISION) << this->getData()[(this->getCols() - 1)*this->getRows()+i] << std::endl;
  }
}

template<typename T>
OGTerminal::Ptr
OGMatrix<T>::createOwningCopy() const
{
  T * newdata =  new T[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  return OGMatrix<T>::create(newdata, this->getRows(), this->getCols(), OWNER);
}

template<typename T>
OGComplexMatrix::Ptr
OGMatrix<T>::asFullOGComplexMatrix() const
{
  complex16 * newdata = new complex16[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  return OGComplexMatrix::create(newdata,this->getRows(),this->getCols(),OWNER);
}

template<typename T>
complex16 **
OGMatrix<T>::toComplex16ArrayOfArrays() const
{
  int const rows = this->getRows();
  int const cols = this->getCols();
  T * const data = this->getData();
  complex16 ** tmp = new complex16 * [rows];
  for(int i=0; i < rows; i++)
  {
    tmp[i] = new complex16 [cols];
    for(int j = 0; j < cols; j++)
    {
      tmp[i][j] = data[j*rows+i];
    }
  }
  return tmp;
}

template<>
real16 **
OGMatrix<real16>::toReal16ArrayOfArrays() const
{
  int const rows = this->getRows();
  int const cols = this->getCols();
  real16 * const data = this->getData();
  real16 ** tmp = new real16 * [rows];
  for(int i=0; i < rows; i++)
  {
    tmp[i] = new real16 [cols];
    for(int j = 0; j < cols; j++)
    {
      tmp[i][j] = data[j*rows+i];
    }
  }
  return tmp;
}

template<>
real16 **
OGMatrix<complex16>::toReal16ArrayOfArrays() const
{
  throw rdag_error("Error in in partial template specialisation for OGMatrix<complex16>::toReal16ArrayOfArrays(). Cannot convert a matrix backed by complex16 type to a real16 type.");
}

template<>
ExprType_t
OGMatrix<real16>::getType() const
{
  return REAL_MATRIX_ENUM;
}

template<>
ExprType_t
OGMatrix<complex16>::getType() const
{
  return COMPLEX_MATRIX_ENUM;
}

template class OGMatrix<real16>;
template class OGMatrix<complex16>;

/**
 * OGRealMatrix
 */

OGNumeric::Ptr
OGRealMatrix::copy() const
{
  return create(this->getData(), this->getRows(), this->getCols());
}

OGRealMatrix::Ptr
OGRealMatrix::create(real16* data, int rows, int cols, DATA_ACCESS access_spec)
{
  return OGRealMatrix::Ptr{new OGRealMatrix{data, rows, cols, access_spec}};
}

OGRealMatrix::Ptr
OGRealMatrix::asOGRealMatrix() const
{
  return static_pointer_cast<const OGRealMatrix, const OGNumeric>(shared_from_this());
}

ExprType_t
OGRealMatrix::getType() const
{
  return REAL_MATRIX_ENUM;
}

OGRealMatrix::Ptr
OGRealMatrix::asFullOGRealMatrix() const
{
  real16 * ret = new real16[this->getDatalen()];
  memcpy(ret,this->getData(),this->getDatalen()*sizeof(real16));
  return OGRealMatrix::create(ret,this->getRows(),this->getCols(),OWNER);
}

OGComplexMatrix::Ptr
OGRealMatrix::asFullOGComplexMatrix() const
{
  return getConvertTo()->convertToOGComplexMatrix(asOGRealMatrix());
}

OGTerminal::Ptr
OGRealMatrix::createOwningCopy() const
{
  real16 * newdata =  new real16[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  return OGRealMatrix::create(newdata, this->getRows(), this->getCols(), OWNER);
}

OGTerminal::Ptr
OGRealMatrix::createComplexOwningCopy() const
{
  complex16 * newdata =  new complex16[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  return OGComplexMatrix::create(newdata, this->getRows(), this->getCols(), OWNER);
}

/**
 * Logical Matrix
 */
OGLogicalMatrix::Ptr
OGLogicalMatrix::create(real16* data, int rows, int cols, DATA_ACCESS access_spec)
{
  return OGLogicalMatrix::Ptr{new OGLogicalMatrix{data, rows, cols, access_spec}};
}

OGLogicalMatrix::Ptr
OGLogicalMatrix::asOGLogicalMatrix() const
{
  return static_pointer_cast<const OGLogicalMatrix, const OGNumeric>(shared_from_this());
}


/**
 * OGComplexMatrix
 */

OGComplexMatrix::Ptr
OGComplexMatrix::create(complex16* data, int rows, int cols, DATA_ACCESS access_spec)
{
  return OGComplexMatrix::Ptr{new OGComplexMatrix{data, rows, cols, access_spec}};
}


complex16**
OGComplexMatrix::toComplex16ArrayOfArrays() const
{
  return this->toArrayOfArrays();
}

OGNumeric::Ptr
OGComplexMatrix::copy() const
{
  return create(this->getData(), this->getRows(), this->getCols());
}

OGComplexMatrix::Ptr
OGComplexMatrix::asOGComplexMatrix() const
{
  return static_pointer_cast<const OGComplexMatrix, const OGNumeric>(shared_from_this());
}

ExprType_t
OGComplexMatrix::getType() const
{
  return COMPLEX_MATRIX_ENUM;
}

OGRealMatrix::Ptr
OGComplexMatrix::asFullOGRealMatrix() const
{
  throw rdag_error("Cannot represent complex data in linear memory of type real16");
}

OGComplexMatrix::Ptr
OGComplexMatrix::asFullOGComplexMatrix() const
{
  int len = this->getDatalen();
  complex16 * data = this->getData();
  complex16 * ret = new complex16[len];
  memcpy(ret,data,len*sizeof(complex16));
  return OGComplexMatrix::create(ret,this->getRows(),this->getCols(),OWNER);
}

OGTerminal::Ptr
OGComplexMatrix::createOwningCopy() const
{
  complex16 * newdata =  new complex16[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  return OGComplexMatrix::create(newdata, this->getRows(), this->getCols(), OWNER);
}

OGTerminal::Ptr
OGComplexMatrix::createComplexOwningCopy() const
{
  return createOwningCopy();
}

/**
 * OGDiagonalMatrix
 */

template<typename T>
OGDiagonalMatrix<T>::OGDiagonalMatrix(T* data, int rows, int cols, DATA_ACCESS access_spec)
{
  if (data == nullptr)
  {
    throw rdag_error("Null data passed to DiagonalMatrix constructor");
  }
  this->setData(data);
  this->setRows(rows);
  this->setCols(cols);
  this->setDatalen(rows>cols?cols:rows);
  this->setDataAccess(access_spec);
}

template<typename T>
typename OGDiagonalMatrix<T>::Ptr
OGDiagonalMatrix<T>::create(T* data, int rows, int cols, DATA_ACCESS access_spec)
{
  return OGDiagonalMatrix<T>::Ptr{new OGDiagonalMatrix<T>{data, rows, cols, access_spec}};
}

template<typename T>
void
OGDiagonalMatrix<T>::accept(Visitor &v) const
{
  v.visit(this);
}

template<typename T>
T**
OGDiagonalMatrix<T>::toArrayOfArrays() const
{
  int const rows = this->getRows();
  int const cols = this->getCols();
  T * const data = this->getData();
  int const datalen = this->getDatalen();
  T ** tmp = new T * [rows];
  for(int i=0; i < rows; i++)
  {
    tmp[i] = new T [cols];
    for(int j=0; j < cols; j++)
    {
      if(i == j && i < datalen)
      {
        tmp[i][j] = data[i];
      }
      else
      {
        tmp[i][j] = 0.e0;
      }
    }
  }
  return tmp;
}

template class OGDiagonalMatrix<real16>;
template class OGDiagonalMatrix<complex16>;

/**
 * OGRealDiagonalMatrix
 */

OGRealDiagonalMatrix::Ptr
OGRealDiagonalMatrix::create(real16* data, int rows, int cols, DATA_ACCESS access_spec)
{
  return OGRealDiagonalMatrix::Ptr{new OGRealDiagonalMatrix{data, rows, cols, access_spec}};
}

void
OGRealDiagonalMatrix::debug_print() const
{
  size_t ptr=0;
  printf("\n");
  for(int i = 0 ; i < this->getRows(); i++)
  {
    for(int j = 0 ; j < this->getCols()-1; j++)
    {
      if(j==i && i < this->getDatalen())
      {
        printf("%6.4f, ",this->getData()[ptr++]);
      }
      else
      {
        printf("%6.4f, ",0.e0);
      }
    }
    if(i==this->getCols()-1 && i < this->getDatalen())
    {
      printf("%6.4f\n",this->getData()[ptr++]);
    }
    else
    {
      printf("%6.4f\n",0.e0);
    }
  }
}

real16*
OGRealDiagonalMatrix::toReal16Array() const
{
  return this->toArray();
}

real16**
OGRealDiagonalMatrix::toReal16ArrayOfArrays() const
{
  return this->toArrayOfArrays();
}

OGNumeric::Ptr
OGRealDiagonalMatrix::copy() const
{
  return create(this->getData(), this->getRows(), this->getCols());
}

OGRealDiagonalMatrix::Ptr
OGRealDiagonalMatrix::asOGRealDiagonalMatrix() const
{
  return static_pointer_cast<const OGRealDiagonalMatrix, const OGNumeric>(shared_from_this());
}

ExprType_t
OGRealDiagonalMatrix::getType() const
{
  return REAL_DIAGONAL_MATRIX_ENUM;
}

OGRealMatrix::Ptr
OGRealDiagonalMatrix::asFullOGRealMatrix() const
{
    return getConvertTo()->convertToOGRealMatrix(asOGRealDiagonalMatrix());
}

OGComplexMatrix::Ptr
OGRealDiagonalMatrix::asFullOGComplexMatrix() const
{
  return getConvertTo()->convertToOGComplexMatrix(asOGRealDiagonalMatrix());
}

OGTerminal::Ptr
OGRealDiagonalMatrix::createOwningCopy() const
{
  real16 * newdata =  new real16[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  return OGRealDiagonalMatrix::create(newdata, this->getRows(), this->getCols(), OWNER);
}

OGTerminal::Ptr
OGRealDiagonalMatrix::createComplexOwningCopy() const
{
  complex16 * newdata =  new complex16[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  return OGComplexDiagonalMatrix::create(newdata, this->getRows(), this->getCols(), OWNER);
}


/**
 * OGComplexDiagonalMatrix
 */

OGComplexDiagonalMatrix::Ptr
OGComplexDiagonalMatrix::create(complex16* data, int rows, int cols, DATA_ACCESS access_spec)
{
  return OGComplexDiagonalMatrix::Ptr{new OGComplexDiagonalMatrix{data, rows, cols, access_spec}};
}

void
OGComplexDiagonalMatrix::debug_print() const
{
  size_t ptr=0;
  printf("\n");
  for(int i = 0 ; i < this->getRows(); i++)
  {
    for(int j = 0 ; j < this->getCols()-1; j++)
    {
      if(j==i && i < this->getDatalen())
      {
        printf("%6.4f + %6.4fi, ",this->getData()[ptr].real(),this->getData()[ptr].imag());
        ptr++;
      }
      else
      {
        printf("%6.4f + %6.4fi, ",0.e0,0.e0);
      }
    }
    if(i==this->getCols()-1 && i < this->getDatalen())
    {
      printf("%6.4f + %6.4fi\n",this->getData()[ptr].real(),this->getData()[ptr].imag());
      ptr++;
    }
    else
    {
      printf("%6.4f + %6.4fi\n",0.e0,0.e0);
    }
  }
}

complex16*
OGComplexDiagonalMatrix::toComplex16Array() const
{
  return this->toArray();
}

complex16**
OGComplexDiagonalMatrix::toComplex16ArrayOfArrays() const
{
  return this->toArrayOfArrays();
}

OGNumeric::Ptr
OGComplexDiagonalMatrix::copy() const
{
  return OGComplexDiagonalMatrix::create(this->getData(), this->getRows(), this->getCols());
}

OGComplexDiagonalMatrix::Ptr
OGComplexDiagonalMatrix::asOGComplexDiagonalMatrix() const
{
  return static_pointer_cast<const OGComplexDiagonalMatrix, const OGNumeric>(shared_from_this());
}


ExprType_t
OGComplexDiagonalMatrix::getType() const
{
  return COMPLEX_DIAGONAL_MATRIX_ENUM;
}

OGRealMatrix::Ptr
OGComplexDiagonalMatrix::asFullOGRealMatrix() const
{
  throw rdag_error("Cannot represent complex data in linear memory of type real16");
}

OGComplexMatrix::Ptr
OGComplexDiagonalMatrix::asFullOGComplexMatrix() const
{
  return getConvertTo()->convertToOGComplexMatrix(asOGComplexDiagonalMatrix());
}


OGTerminal::Ptr
OGComplexDiagonalMatrix::createOwningCopy() const
{
  complex16 * newdata =  new complex16[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  return OGComplexDiagonalMatrix::create(newdata, this->getRows(), this->getCols(), OWNER);
}

OGTerminal::Ptr
OGComplexDiagonalMatrix::createComplexOwningCopy() const
{
  return createOwningCopy();
}



/**
 * OGSparseMatrix
 */

template<typename T>
OGSparseMatrix<T>::OGSparseMatrix(int * colPtr, int * rowIdx, T* data, int rows, int cols, DATA_ACCESS access_spec)
{
  if (data == nullptr)
  {
    throw rdag_error("Null data passed to SparseMatrix constructor");
  }
  this->setData(data);
  this->setRows(rows);
  this->setCols(cols);
  this->setColPtr(colPtr);
  this->setRowIdx(rowIdx);
  this->setDatalen(colPtr[cols]); // must go last to catch null on colptr happens
  this->setDataAccess(access_spec);
}

template<typename T>
typename OGSparseMatrix<T>::Ptr
OGSparseMatrix<T>::create(int* colPtr, int* rowIdx, T* data, int rows, int cols, DATA_ACCESS access_spec)
{
  return OGSparseMatrix<T>::Ptr{new OGSparseMatrix<T>{colPtr, rowIdx, data, rows, cols, access_spec}};
}

template<typename T>
OGSparseMatrix<T>::~OGSparseMatrix()
{
  if(this->getDataAccess() == OWNER)
  {
    delete[] this->getColPtr();
    delete[] this->getRowIdx();
  }
}

template<typename T>
void
OGSparseMatrix<T>::accept(Visitor &v) const
{
  v.visit(this);
}

template<typename T>
int*
OGSparseMatrix<T>::getColPtr() const
{
  return _colPtr;
}

template<typename T>
int*
OGSparseMatrix<T>::getRowIdx() const
{
  return _rowIdx;
}

template<typename T>
void
OGSparseMatrix<T>::setColPtr(int* colPtr)
{
  if(colPtr==nullptr)
  {
    throw rdag_error("Attempt to set colPtr to null");
  }
  _colPtr = colPtr;
}

template<typename T>
void
OGSparseMatrix<T>::setRowIdx(int* rowIdx)
{
  if(rowIdx==nullptr)
  {
    throw rdag_error("Attempt to set rowIdx to null");
  }  
  _rowIdx = rowIdx;
}

template<typename T>
T** 
OGSparseMatrix<T>::toArrayOfArrays() const{
  int const rows = this->getRows();
  int const cols = this->getCols();
  int * const colPtr = this->getColPtr();
  int * const rowIdx = this->getRowIdx();
  T * const data = this->getData();
  T ** tmp = new T * [rows];
  for(int i=0; i < rows; i++)
  {
    tmp[i] = new T [cols];
    memset(tmp[i],0,(size_t)sizeof(T)*cols);
  }
  for (int ir = 0; ir < cols; ir++)
  {
      for (int i = colPtr[ir]; i < colPtr[ir + 1]; i++)
      {
        tmp[rowIdx[i]][ir] = data[i];
      }
  }
  return tmp;
}

template<typename T>
bool
OGSparseMatrix<T>::equals(const OGTerminal::Ptr& other) const
{
  if(!OGArray<T>::equals(other))
  {
    return false;
  }
  shared_ptr<const OGSparseMatrix<T>> typetwiddle = static_pointer_cast<const OGSparseMatrix<T>, const OGTerminal>(other);
  if(!ArrayBitEquals(typetwiddle->getColPtr(),this->getColPtr(),this->getCols()+1))
  {
    return false;
  }
  if(!ArrayBitEquals(typetwiddle->getRowIdx(),this->getRowIdx(),this->getDatalen()))
  {
    return false;
  }
  return true;
}

template<typename T>
bool
OGSparseMatrix<T>::fuzzyequals(const OGTerminal::Ptr& other, real16 maxabserror, real16 maxrelerror) const
{
  if(!OGArray<T>::fuzzyequals(other,maxabserror,maxrelerror))
  {
    return false;
  }
  const shared_ptr<const OGSparseMatrix<T>> typetwiddle = static_pointer_cast<const OGSparseMatrix<T>, const OGTerminal>(other);
  if(!ArrayBitEquals(typetwiddle->getColPtr(),this->getColPtr(),this->getCols()+1))
  {
    return false;
  }
  if(!ArrayBitEquals(typetwiddle->getRowIdx(),this->getRowIdx(),this->getDatalen()))
  {
    return false;
  }
  return true;
}

template<typename T>
bool
OGSparseMatrix<T>::fuzzyequals(const OGTerminal::Ptr& other) const
{
  return fuzzyequals(other, FuzzyEquals_default_maxabserror, FuzzyEquals_default_maxrelerror);
}

template class OGSparseMatrix<real16>;
template class OGSparseMatrix<complex16>;

/**
 * OGRealSparseMatrix
 */

OGRealSparseMatrix::Ptr
OGRealSparseMatrix::create(int* colPtr, int* rowIdx, real16* data, int rows, int cols, DATA_ACCESS access_spec)
{
  return OGRealSparseMatrix::Ptr{new OGRealSparseMatrix{colPtr, rowIdx, data, rows, cols, access_spec}};
}

void
OGRealSparseMatrix::debug_print() const
{
  double nnz = 100.e0 * this->getDatalen() / (this->getRows() * this->getCols());
  printf("\nOGRealSparseMatrix\n");
  printf("[nnz density = %4.2f. rows = %d, columns = %d]\n", nnz, this->getRows(), this->getCols());
  int * colPtr = this->getColPtr();
  for (int ir = 0; ir < this->getCols(); ir++)
  {
    for (int i = colPtr[ir]; i < colPtr[ir + 1]; i++)
    {
      printf("(%d,%d) = %6.4f\n",this->getRowIdx()[i],ir,this->getData()[i]);
    }
  }
}

real16*
OGRealSparseMatrix::toReal16Array() const
{
  return this->toArray();
}

real16**
OGRealSparseMatrix::toReal16ArrayOfArrays() const
{
  return this->toArrayOfArrays();
}

OGNumeric::Ptr
OGRealSparseMatrix::copy() const
{
  return OGRealSparseMatrix::create(this->getColPtr(), this->getRowIdx(), this->getData(),
                                    this->getRows(), this->getCols());
}

OGRealSparseMatrix::Ptr
OGRealSparseMatrix::asOGRealSparseMatrix() const
{
  return static_pointer_cast<const OGRealSparseMatrix, const OGNumeric>(shared_from_this());
}

ExprType_t
OGRealSparseMatrix::getType() const
{
  return REAL_SPARSE_MATRIX_ENUM;
}

OGRealMatrix::Ptr
OGRealSparseMatrix::asFullOGRealMatrix() const
{
  return getConvertTo()->convertToOGRealMatrix(asOGRealSparseMatrix());
}

OGComplexMatrix::Ptr
OGRealSparseMatrix::asFullOGComplexMatrix() const
{
  return getConvertTo()->convertToOGComplexMatrix(asOGRealSparseMatrix());
}

OGTerminal::Ptr
OGRealSparseMatrix::createOwningCopy() const
{
  real16 * newdata =  new real16[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  int * newColPtr = new int[this->getCols()+1];
  std::copy(this->getColPtr(), this->getColPtr()+this->getCols()+1, newColPtr);
  int * newRowIdx = new int[this->getDatalen()];
  std::copy(this->getRowIdx(), this->getRowIdx()+this->getDatalen(), newRowIdx);
  return OGRealSparseMatrix::create(newColPtr, newRowIdx, newdata, this->getRows(), this->getCols(), OWNER);
}

OGTerminal::Ptr
OGRealSparseMatrix::createComplexOwningCopy() const
{
  complex16 * newdata =  new complex16[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  int * newColPtr = new int[this->getCols()+1];
  std::copy(this->getColPtr(), this->getColPtr()+this->getCols()+1, newColPtr);
  int * newRowIdx = new int[this->getDatalen()];
  std::copy(this->getRowIdx(), this->getRowIdx()+this->getDatalen(), newRowIdx);
  return OGComplexSparseMatrix::create(newColPtr, newRowIdx, newdata, this->getRows(), this->getCols(), OWNER);
}

/**
 * OGComplexSparseMatrix
 */

OGComplexSparseMatrix::Ptr
OGComplexSparseMatrix::create(int* colPtr, int* rowIdx, complex16* data, int rows, int cols, DATA_ACCESS access_spec)
{
  return OGComplexSparseMatrix::Ptr{new OGComplexSparseMatrix{colPtr, rowIdx, data, rows, cols, access_spec}};
}

void
OGComplexSparseMatrix::debug_print() const
{
  double nnz = 100.e0 * this->getDatalen() / (double)(this->getRows() * this->getCols());
  printf("\nOGComplexSparseMatrix\n");
  printf("[nnz density = %4.2f. rows = %d, columns = %d]\n", nnz, this->getRows(), this->getCols());
  int * colPtr = this->getColPtr();
  for (int ir = 0; ir < this->getCols(); ir++)
  {
    for (int i = colPtr[ir]; i < colPtr[ir + 1]; i++)
    {
      printf("(%d,%d) = %6.4f + %6.4fi \n",this->getRowIdx()[i],ir,this->getData()[i].real(),this->getData()[i].imag());
    }
  }
}

complex16*
OGComplexSparseMatrix::toComplex16Array() const
{
  return this->toArray();
}

complex16**
OGComplexSparseMatrix::toComplex16ArrayOfArrays() const
{
  return this->toArrayOfArrays();
}

OGNumeric::Ptr
OGComplexSparseMatrix::copy() const
{
  return OGComplexSparseMatrix::create(this->getColPtr(), this->getRowIdx(), this->getData(),
                                       this->getRows(), this->getCols());
}

OGComplexSparseMatrix::Ptr
OGComplexSparseMatrix::asOGComplexSparseMatrix() const
{
  return static_pointer_cast<const OGComplexSparseMatrix, const OGNumeric>(shared_from_this());
}

ExprType_t
OGComplexSparseMatrix::getType() const
{
  return COMPLEX_SPARSE_MATRIX_ENUM;
}

OGRealMatrix::Ptr
OGComplexSparseMatrix::asFullOGRealMatrix() const
{
  throw rdag_error("Cannot represent complex data in linear memory of type real16");
}

OGComplexMatrix::Ptr
OGComplexSparseMatrix::asFullOGComplexMatrix() const
{
  return getConvertTo()->convertToOGComplexMatrix(asOGComplexSparseMatrix());
}

OGTerminal::Ptr
OGComplexSparseMatrix::createOwningCopy() const
{
  complex16 * newdata =  new complex16[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  int * newColPtr = new int[this->getCols()+1];
  std::copy(this->getColPtr(), this->getColPtr()+this->getCols()+1, newColPtr);
  int * newRowIdx = new int[this->getDatalen()];
  std::copy(this->getRowIdx(), this->getRowIdx()+this->getDatalen(), newRowIdx);
  return OGComplexSparseMatrix::create(newColPtr, newRowIdx, newdata, this->getRows(), this->getCols(), OWNER);
}

OGTerminal::Ptr
OGComplexSparseMatrix::createComplexOwningCopy() const
{
  return createOwningCopy();
}


// Concrete template factory for dense matrices
template<>
OGNumeric::Ptr makeConcreteDenseMatrix(real16 * data, int rows, int cols, DATA_ACCESS access)
{
  return OGRealMatrix::create(data, rows, cols, access);
}

template<>
OGNumeric::Ptr makeConcreteDenseMatrix(complex16 * data, int rows, int cols, DATA_ACCESS access)
{
  return OGComplexMatrix::create(data, rows, cols, access);
}

// Concrete template factory for scalars
template<>
OGNumeric::Ptr makeConcreteScalar(real16 data)
{
  return OGRealScalar::create(data);
}

template<>
OGNumeric::Ptr makeConcreteScalar(complex16 data)
{
  return OGComplexScalar::create(data);
}

} // namespace librdag
