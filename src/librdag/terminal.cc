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

pOGTerminal
OGTerminal::asOGTerminal() const
{
  return static_pointer_cast<const OGTerminal, const OGNumeric>(shared_from_this());
}

bool
OGTerminal::operator==(const pOGTerminal& other) const
{
  return this->equals(other);
}

bool
OGTerminal::operator!=(const pOGTerminal& other) const
{
  return !(this->equals(other));
}

detail::FuzzyCompareOGTerminalContainer&
OGTerminal::operator~(void) const
{
  return this->getFuzzyContainer();
}

bool OGTerminal::operator==(const detail::FuzzyCompareOGTerminalContainer& thing) const
{
  return this->fuzzyequals(pOGTerminal{thing.getTerminal()});
}

bool OGTerminal::operator!=(const detail::FuzzyCompareOGTerminalContainer& thing) const
{
  return !(this->fuzzyequals(pOGTerminal{thing.getTerminal()}));
}

OGTerminal::OGTerminal()
{
  //_fuzzyref = new detail::FuzzyCompareOGTerminalContainer(asOGTerminal());
  _converter = new ConvertTo();
}

OGTerminal::~OGTerminal()
{
  if (_fuzzyref != nullptr)
  {
    delete _fuzzyref;
  }
  delete _converter;
}

detail::FuzzyCompareOGTerminalContainer&
OGTerminal::getFuzzyContainer() const{
  if (_fuzzyref == nullptr)
  {
    _fuzzyref = new detail::FuzzyCompareOGTerminalContainer(asOGTerminal());
  }
  return *_fuzzyref;
}

const ConvertTo *
OGTerminal::getConvertTo() const
{
  return this->_converter;
}

bool
OGTerminal::mathsequals(const OGTerminal * other) const
{
  return this->mathsequals(other, FuzzyEquals_default_maxabserror, FuzzyEquals_default_maxrelerror);
}

bool
OGTerminal::mathsequals(pOGTerminal other) const
{
  return mathsequals(other.get());
}

bool
OGTerminal::mathsequals(const OGTerminal * other, real16 maxabserror, real16 maxrelerror) const
{
  pOGTerminal thisconv; 
  pOGTerminal otherconv;
  bool ret = false;
  if((this->getRows()!=other->getRows())||(this->getCols()!=other->getCols()))
  {
    return ret;
  }
  if(isReal(this)&&isReal(other))
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
OGTerminal::mathsequals(pOGTerminal other, real16 maxabserror, real16 maxrelerror) const
{
  return mathsequals(other.get(), maxabserror, maxrelerror);
}

bool
OGTerminal::operator%(const OGTerminal& other) const
{
  return this->mathsequals(&other);
}



/**
 * FuzzyCompareOGTerminalContainer
 */
detail::FuzzyCompareOGTerminalContainer::FuzzyCompareOGTerminalContainer(const pOGTerminal& terminal): _terminal{terminal} {}

detail::FuzzyCompareOGTerminalContainer::~FuzzyCompareOGTerminalContainer(){}

const weak_ptr<const OGTerminal>
detail::FuzzyCompareOGTerminalContainer::getTerminal() const
{
  return _terminal;
}


/**
 * OGScalar
 */

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
OGScalar<T>::equals(pOGTerminal other) const
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
OGScalar<T>::fuzzyequals(const pOGTerminal& other) const
{
  return this->fuzzyequals(other,FuzzyEquals_default_maxabserror,FuzzyEquals_default_maxrelerror);
}

template<typename T>
bool
OGScalar<T>::fuzzyequals(const pOGTerminal& other, real16 maxabserror, real16 maxrelerror) const
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
pOGRealMatrix
OGScalar<T>::asFullOGRealMatrix() const
{
  throw rdag_error("Cannot asFullOGRealMatrix() template OGScalar<T> class.");
}

template<typename T>
pOGComplexMatrix
OGScalar<T>::asFullOGComplexMatrix() const
{
  throw rdag_error("Cannot asFullOGComplexMatrix() template OGScalar<T> class.");
}

template<typename T>
OGTerminal *
OGScalar<T>::createOwningCopy() const
{
  throw rdag_error("Cannot createOwningCopy() template OGScalar<T> class.");
}

template<typename T>
OGTerminal *
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
pOGNumeric
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

pOGNumeric
OGRealScalar::copy() const
{
  return pOGNumeric{new OGRealScalar(this->getValue())};
}

pOGRealScalar
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

pOGRealMatrix
OGRealScalar::asFullOGRealMatrix() const
{
  return getConvertTo()->convertToOGRealMatrix(asOGRealScalar());
}

pOGComplexMatrix
OGRealScalar::asFullOGComplexMatrix() const
{
  return getConvertTo()->convertToOGComplexMatrix(asOGRealScalar());
}

OGTerminal *
OGRealScalar::createOwningCopy() const
{
  return new OGRealScalar(this->getValue());
}

OGTerminal *
OGRealScalar::createComplexOwningCopy() const
{
  return new OGComplexScalar(this->getValue());
}

/**
 * OGComplexScalar
 */

OGComplexScalar::OGComplexScalar(complex16 data): OGScalar<complex16>(data) {}

complex16**
OGComplexScalar::toComplex16ArrayOfArrays() const
{
  return this->toArrayOfArrays();
}

pOGNumeric
OGComplexScalar::copy() const
{
  return pOGNumeric{new OGComplexScalar(this->getValue())};
}

pOGComplexScalar
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

pOGRealMatrix
OGComplexScalar::asFullOGRealMatrix() const
{
  throw rdag_error("Cannot represent complex data in linear memory of type real16");
}

pOGComplexMatrix
OGComplexScalar::asFullOGComplexMatrix() const
{
  return getConvertTo()->convertToOGComplexMatrix(asOGComplexScalar());
}

OGTerminal *
OGComplexScalar::createOwningCopy() const
{
  return new OGComplexScalar(this->getValue());
}

OGTerminal *
OGComplexScalar::createComplexOwningCopy() const
{
  return this->createOwningCopy();
}


/**
 * OGIntegerScalar
 */

OGIntegerScalar::OGIntegerScalar(int data): OGScalar<int>(data) {}

pOGNumeric
OGIntegerScalar::copy() const
{
  return pOGNumeric{new OGIntegerScalar(this->getValue())};
}

pOGIntegerScalar
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

pOGRealMatrix
OGIntegerScalar::asFullOGRealMatrix() const
{
  return getConvertTo()->convertToOGRealMatrix(asOGIntegerScalar());
}

pOGComplexMatrix
OGIntegerScalar::asFullOGComplexMatrix() const
{
  return getConvertTo()->convertToOGComplexMatrix(asOGIntegerScalar());
}

OGTerminal *
OGIntegerScalar::createOwningCopy() const
{
  return new OGIntegerScalar(this->getValue());
}

OGTerminal *
OGIntegerScalar::createComplexOwningCopy() const
{
  return new OGComplexScalar(this->getValue());
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
OGArray<T>::fundamentalsEqual(const pOGTerminal& other) const
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
OGArray<T>::equals(pOGTerminal other) const
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
OGArray<T>::fuzzyequals(const pOGTerminal& other,real16 maxabserror,real16 maxrelerror) const
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
OGArray<T>::fuzzyequals(const pOGTerminal& other) const
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
pOGNumeric
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
pOGRealMatrix
OGArray<T>::
asFullOGRealMatrix() const
{
  // this is a default backstop impl whilst waiting for a traited version
  throw rdag_error("Cannot convert asFullOGRealMatrix(), concrete implementation class required for operation.");
}

template<typename T>
pOGComplexMatrix
OGArray<T>::
asFullOGComplexMatrix() const
{
  // this is a default backstop impl whilst waiting for a traited version
  throw rdag_error("Cannot convert asFullOGComplexMatrix(), concrete implementation class required for operation.");
}

template<typename T>
OGTerminal *
OGArray<T>::
createOwningCopy() const
{
  // this is a default backstop impl whilst waiting for a traited version
  throw rdag_error("Cannot createOwningCopy(), concrete implementation class required for operation.");
}

template<typename T>
OGTerminal *
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
pOGNumeric
OGMatrix<T>::copy() const
{
  return pOGNumeric{new OGMatrix<T>(this->getData(), this->getRows(), this->getCols())};
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
OGTerminal *
OGMatrix<T>::createOwningCopy() const
{
  T * newdata =  new T[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  return new OGMatrix<T>(newdata, this->getRows(), this->getCols(), OWNER);
}

template<typename T>
pOGComplexMatrix
OGMatrix<T>::asFullOGComplexMatrix() const
{
  complex16 * newdata = new complex16[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  return pOGComplexMatrix{new OGComplexMatrix(newdata,this->getRows(),this->getCols(),OWNER)};
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

pOGNumeric
OGRealMatrix::copy() const
{
  return pOGNumeric{new OGRealMatrix(this->getData(), this->getRows(), this->getCols())};
}

pOGRealMatrix
OGRealMatrix::asOGRealMatrix() const
{
  return static_pointer_cast<const OGRealMatrix, const OGNumeric>(shared_from_this());
}

ExprType_t
OGRealMatrix::getType() const
{
  return REAL_MATRIX_ENUM;
}

pOGRealMatrix
OGRealMatrix::asFullOGRealMatrix() const
{
  real16 * ret = new real16[this->getDatalen()];
  memcpy(ret,this->getData(),this->getDatalen()*sizeof(real16));
  return pOGRealMatrix{new OGRealMatrix(ret,this->getRows(),this->getCols(),OWNER)};
}

pOGComplexMatrix
OGRealMatrix::asFullOGComplexMatrix() const
{
  return getConvertTo()->convertToOGComplexMatrix(asOGRealMatrix());
}

OGTerminal *
OGRealMatrix::createOwningCopy() const
{
  real16 * newdata =  new real16[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  return new OGRealMatrix(newdata, this->getRows(), this->getCols(), OWNER);
}

OGTerminal *
OGRealMatrix::createComplexOwningCopy() const
{
  complex16 * newdata =  new complex16[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  return new OGComplexMatrix(newdata, this->getRows(), this->getCols(), OWNER);
}

/**
 * Logical Matrix
 */
pOGLogicalMatrix
OGLogicalMatrix::asOGLogicalMatrix() const
{
  return static_pointer_cast<const OGLogicalMatrix, const OGNumeric>(shared_from_this());
}


/**
 * OGComplexMatrix
 */

complex16**
OGComplexMatrix::toComplex16ArrayOfArrays() const
{
  return this->toArrayOfArrays();
}

pOGNumeric
OGComplexMatrix::copy() const
{
  return pOGNumeric{new OGComplexMatrix(this->getData(), this->getRows(), this->getCols())};
}

pOGComplexMatrix
OGComplexMatrix::asOGComplexMatrix() const
{
  return static_pointer_cast<const OGComplexMatrix, const OGNumeric>(shared_from_this());
}

ExprType_t
OGComplexMatrix::getType() const
{
  return COMPLEX_MATRIX_ENUM;
}

pOGRealMatrix
OGComplexMatrix::asFullOGRealMatrix() const
{
  throw rdag_error("Cannot represent complex data in linear memory of type real16");
}

pOGComplexMatrix
OGComplexMatrix::asFullOGComplexMatrix() const
{
  int len = this->getDatalen();
  complex16 * data = this->getData();
  complex16 * ret = new complex16[len];
  memcpy(ret,data,len*sizeof(complex16));
  return pOGComplexMatrix{new OGComplexMatrix(ret,this->getRows(),this->getCols(),OWNER)};
}

OGTerminal *
OGComplexMatrix::createOwningCopy() const
{
  complex16 * newdata =  new complex16[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  return new OGComplexMatrix(newdata, this->getRows(), this->getCols(), OWNER);
}

OGTerminal *
OGComplexMatrix::createComplexOwningCopy() const
{
  return this->createOwningCopy();
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

pOGNumeric
OGRealDiagonalMatrix::copy() const
{
  return pOGNumeric{new OGRealDiagonalMatrix(this->getData(), this->getRows(), this->getCols())};
}

pOGRealDiagonalMatrix
OGRealDiagonalMatrix::asOGRealDiagonalMatrix() const
{
  return static_pointer_cast<const OGRealDiagonalMatrix, const OGNumeric>(shared_from_this());
}

ExprType_t
OGRealDiagonalMatrix::getType() const
{
  return REAL_DIAGONAL_MATRIX_ENUM;
}

pOGRealMatrix
OGRealDiagonalMatrix::asFullOGRealMatrix() const
{
    return getConvertTo()->convertToOGRealMatrix(asOGRealDiagonalMatrix());
}

pOGComplexMatrix
OGRealDiagonalMatrix::asFullOGComplexMatrix() const
{
  return getConvertTo()->convertToOGComplexMatrix(asOGRealDiagonalMatrix());
}

OGTerminal *
OGRealDiagonalMatrix::createOwningCopy() const
{
  real16 * newdata =  new real16[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  return new OGRealDiagonalMatrix(newdata, this->getRows(), this->getCols(), OWNER);
}

OGTerminal *
OGRealDiagonalMatrix::createComplexOwningCopy() const
{
  complex16 * newdata =  new complex16[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  return new OGComplexDiagonalMatrix(newdata, this->getRows(), this->getCols(), OWNER);
}


/**
 * OGComplexDiagonalMatrix
 */

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

pOGNumeric
OGComplexDiagonalMatrix::copy() const
{
  return pOGNumeric{new OGComplexDiagonalMatrix(this->getData(), this->getRows(), this->getCols())};
}

pOGComplexDiagonalMatrix
OGComplexDiagonalMatrix::asOGComplexDiagonalMatrix() const
{
  return static_pointer_cast<const OGComplexDiagonalMatrix, const OGNumeric>(shared_from_this());
}


ExprType_t
OGComplexDiagonalMatrix::getType() const
{
  return COMPLEX_DIAGONAL_MATRIX_ENUM;
}

pOGRealMatrix
OGComplexDiagonalMatrix::asFullOGRealMatrix() const
{
  throw rdag_error("Cannot represent complex data in linear memory of type real16");
}

pOGComplexMatrix
OGComplexDiagonalMatrix::asFullOGComplexMatrix() const
{
  return getConvertTo()->convertToOGComplexMatrix(asOGComplexDiagonalMatrix());
}


OGTerminal *
OGComplexDiagonalMatrix::createOwningCopy() const
{
  complex16 * newdata =  new complex16[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  return new OGComplexDiagonalMatrix(newdata, this->getRows(), this->getCols(), OWNER);
}

OGTerminal *
OGComplexDiagonalMatrix::createComplexOwningCopy() const
{
  return this->createOwningCopy();
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
OGSparseMatrix<T>::equals(pOGTerminal other) const
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
OGSparseMatrix<T>::fuzzyequals(const pOGTerminal& other, real16 maxabserror, real16 maxrelerror) const
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
OGSparseMatrix<T>::fuzzyequals(const pOGTerminal& other) const
{
  return fuzzyequals(other, FuzzyEquals_default_maxabserror, FuzzyEquals_default_maxrelerror);
}

template class OGSparseMatrix<real16>;
template class OGSparseMatrix<complex16>;

/**
 * OGRealSparseMatrix
 */

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

pOGNumeric
OGRealSparseMatrix::copy() const
{
  return pOGNumeric{new OGRealSparseMatrix(this->getColPtr(), this->getRowIdx(), this->getData(),
                                           this->getRows(), this->getCols())};
}

pOGRealSparseMatrix
OGRealSparseMatrix::asOGRealSparseMatrix() const
{
  return static_pointer_cast<const OGRealSparseMatrix, const OGNumeric>(shared_from_this());
}

ExprType_t
OGRealSparseMatrix::getType() const
{
  return REAL_SPARSE_MATRIX_ENUM;
}

pOGRealMatrix
OGRealSparseMatrix::asFullOGRealMatrix() const
{
  return getConvertTo()->convertToOGRealMatrix(asOGRealSparseMatrix());
}

pOGComplexMatrix
OGRealSparseMatrix::asFullOGComplexMatrix() const
{
  return getConvertTo()->convertToOGComplexMatrix(asOGRealSparseMatrix());
}

OGTerminal *
OGRealSparseMatrix::createOwningCopy() const
{
  real16 * newdata =  new real16[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  int * newColPtr = new int[this->getCols()+1];
  std::copy(this->getColPtr(), this->getColPtr()+this->getCols()+1, newColPtr);
  int * newRowIdx = new int[this->getDatalen()];
  std::copy(this->getRowIdx(), this->getRowIdx()+this->getDatalen(), newRowIdx);
  return new OGRealSparseMatrix(newColPtr, newRowIdx, newdata, this->getRows(), this->getCols(), OWNER);
}

OGTerminal *
OGRealSparseMatrix::createComplexOwningCopy() const
{
  complex16 * newdata =  new complex16[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  int * newColPtr = new int[this->getCols()+1];
  std::copy(this->getColPtr(), this->getColPtr()+this->getCols()+1, newColPtr);
  int * newRowIdx = new int[this->getDatalen()];
  std::copy(this->getRowIdx(), this->getRowIdx()+this->getDatalen(), newRowIdx);
  return new OGComplexSparseMatrix(newColPtr, newRowIdx, newdata, this->getRows(), this->getCols(), OWNER);
}

/**
 * OGComplexSparseMatrix
 */

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

pOGNumeric
OGComplexSparseMatrix::copy() const
{
  return pOGNumeric{new OGComplexSparseMatrix(this->getColPtr(), this->getRowIdx(), this->getData(),
                                              this->getRows(), this->getCols())};
}

pOGComplexSparseMatrix
OGComplexSparseMatrix::asOGComplexSparseMatrix() const
{
  return static_pointer_cast<const OGComplexSparseMatrix, const OGNumeric>(shared_from_this());
}

ExprType_t
OGComplexSparseMatrix::getType() const
{
  return COMPLEX_SPARSE_MATRIX_ENUM;
}

pOGRealMatrix
OGComplexSparseMatrix::asFullOGRealMatrix() const
{
  throw rdag_error("Cannot represent complex data in linear memory of type real16");
}

pOGComplexMatrix
OGComplexSparseMatrix::asFullOGComplexMatrix() const
{
  return getConvertTo()->convertToOGComplexMatrix(asOGComplexSparseMatrix());
}

OGTerminal *
OGComplexSparseMatrix::createOwningCopy() const
{
  complex16 * newdata =  new complex16[this->getDatalen()];
  std::copy(this->getData(), this->getData()+this->getDatalen(), newdata);
  int * newColPtr = new int[this->getCols()+1];
  std::copy(this->getColPtr(), this->getColPtr()+this->getCols()+1, newColPtr);
  int * newRowIdx = new int[this->getDatalen()];
  std::copy(this->getRowIdx(), this->getRowIdx()+this->getDatalen(), newRowIdx);
  return new OGComplexSparseMatrix(newColPtr, newRowIdx, newdata, this->getRows(), this->getCols(), OWNER);
}

OGTerminal *
OGComplexSparseMatrix::createComplexOwningCopy() const
{
  return this->createOwningCopy();
}


// Concrete template factory for dense matrices
template<>
pOGNumeric makeConcreteDenseMatrix(real16 * data, int rows, int cols, DATA_ACCESS access)
{
  return pOGNumeric{new OGRealMatrix(data, rows, cols, access)};
}

template<>
pOGNumeric makeConcreteDenseMatrix(complex16 * data, int rows, int cols, DATA_ACCESS access)
{
  return pOGNumeric{new OGComplexMatrix(data, rows, cols, access)};
}

// Concrete template factory for scalars
template<>
pOGNumeric makeConcreteScalar(real16 data)
{
  return pOGNumeric{new OGRealScalar(data)};
}

template<>
pOGNumeric makeConcreteScalar(complex16 data)
{
  return pOGNumeric{new OGComplexScalar(data)};
}

} // namespace librdag
