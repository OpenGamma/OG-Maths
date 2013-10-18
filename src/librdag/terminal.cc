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
#include <iostream>
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

const OGTerminal*
OGTerminal::asOGTerminal() const
{
  return this;
}

bool
OGTerminal::operator==(const OGTerminal&  other) const
{
  return this->equals(&other);
}

bool
OGTerminal::operator!=(const OGTerminal&  other) const
{
  return !(this->equals(&other));
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
OGScalar<T>::equals(const OGTerminal * other) const
{
  if(this->getType()!=other->getType())
  {
    return false;
  }
  if(static_cast<const OGScalar *>(other)->getValue()!=this->_value)
  {
    return false;
  }
  return true;
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

OGNumeric*
OGRealScalar::copy() const
{
  return new OGRealScalar(this->getValue());
}

const OGRealScalar*
OGRealScalar::asOGRealScalar() const
{
  return this;
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

/**
 * OGComplexScalar
 */

OGComplexScalar::OGComplexScalar(complex16 data): OGScalar<complex16>(data) {}

complex16**
OGComplexScalar::toComplex16ArrayOfArrays() const
{
  return this->toArrayOfArrays();
}

OGNumeric*
OGComplexScalar::copy() const
{
  return new OGComplexScalar(this->getValue());
}

const OGComplexScalar*
OGComplexScalar::asOGComplexScalar() const
{
  return this;
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

/**
 * OGIntegerScalar
 */

OGIntegerScalar::OGIntegerScalar(int data): OGScalar<int>(data) {}

OGNumeric*
OGIntegerScalar::copy() const
{
  return new OGIntegerScalar(this->getValue());
}

const OGIntegerScalar*
OGIntegerScalar::asOGIntegerScalar() const
{
  return this;
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

/**
 * OGArray
 */

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
OGArray<T>::equals(const OGTerminal * other) const
{
  if(this->getType()!=other->getType())
  {
    return false;
  }
  const OGArray * typetwiddle = static_cast<const OGArray *>(other);
  if(typetwiddle ->getRows()!=this->getRows())
  {
    return false;
  }
  if(typetwiddle ->getCols()!=this->getCols())
  {
    return false;
  }
  if(!ArrayBitEquals(typetwiddle->getData(),this->getData(),this->getDatalen()))
  {
    return false;
  }
  return true;
}

template class OGArray<real16>;
template class OGArray<complex16>;

/**
 * OGMatrix
 */

template<typename T>
OGMatrix<T>::OGMatrix(int rows, int cols)
{
  this->setRows(rows);
  this->setCols(cols);
  this->setDatalen(rows*cols);
}

template<typename T>
OGMatrix<T>::OGMatrix(T* data, int rows, int cols)
{
  if (data == nullptr)
  {
    throw rdag_error("Null data pointer passed to Matrix constructor");
  }
  this->setData(data);
  this->setRows(rows);
  this->setCols(cols);
  this->setDatalen(rows*cols);
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

template class OGMatrix<real16>;
template class OGMatrix<complex16>;

/**
 * OGRealMatrix
 */

void
OGRealMatrix::debug_print() const
{
  size_t ptr=0;
  printf("\n");
  for(int i = 0 ; i < this->getRows(); i++)
  {
    for(int j = 0 ; j < this->getCols()-1; j++)
    {
      printf("%6.4f, ",this->getData()[ptr++]);
    }
    printf("%6.4f\n",this->getData()[ptr++]);
  }
}

real16**
OGRealMatrix::toReal16ArrayOfArrays() const
{
  return this->toArrayOfArrays();
}

OGNumeric*
OGRealMatrix::copy() const
{
  return new OGRealMatrix(this->getData(), this->getRows(), this->getCols());
}

const OGRealMatrix*
OGRealMatrix::asOGRealMatrix() const
{
  return this;
}

ExprType_t
OGRealMatrix::getType() const
{
  return REAL_MATRIX_ENUM;
}


/**
 * OGComplexMatrix
 */

void
OGComplexMatrix::debug_print() const
{
  size_t ptr=0;
  printf("\n");
  for(int i = 0 ; i < this->getRows(); i++)
  {
    for(int j = 0 ; j < this->getCols()-1; j++)
    {
      printf("%6.4f + %6.4fi, ",this->getData()[ptr].real(),this->getData()[ptr].imag());
      ptr++;
    }
    printf("%6.4f + %6.4fi\n",this->getData()[ptr].real(),this->getData()[ptr].imag());
    ptr++;
  }
}

complex16**
OGComplexMatrix::toComplex16ArrayOfArrays() const
{
  return this->toArrayOfArrays();
}

OGNumeric*
OGComplexMatrix::copy() const
{
  return new OGComplexMatrix(this->getData(), this->getRows(), this->getCols());
}

const OGComplexMatrix*
OGComplexMatrix::asOGComplexMatrix() const
{
  return this;
}

ExprType_t
OGComplexMatrix::getType() const
{
  return COMPLEX_MATRIX_ENUM;
}

/**
 * OGDiagonalMatrix
 */

template<typename T>
OGDiagonalMatrix<T>::OGDiagonalMatrix(int rows, int cols)
{
  this->setRows(rows);
  this->setCols(cols);
  this->setDatalen(rows>cols?cols:rows);
}

template<typename T>
OGDiagonalMatrix<T>::OGDiagonalMatrix(T* data, int rows, int cols)
{
  if (data == nullptr)
  {
    throw rdag_error("Null data passed to DiagonalMatrix constructor");
  }
  this->setData(data);
  this->setRows(rows);
  this->setCols(cols);
  this->setDatalen(rows>cols?cols:rows);
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

OGNumeric*
OGRealDiagonalMatrix::copy() const
{
  return new OGRealDiagonalMatrix(this->getData(), this->getRows(), this->getCols());
}

const OGRealDiagonalMatrix*
OGRealDiagonalMatrix::asOGRealDiagonalMatrix() const
{
  return this;
}

ExprType_t
OGRealDiagonalMatrix::getType() const
{
  return REAL_DIAGONAL_MATRIX_ENUM;
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

OGNumeric*
OGComplexDiagonalMatrix::copy() const
{
  return new OGComplexDiagonalMatrix(this->getData(), this->getRows(), this->getCols());
}

const OGComplexDiagonalMatrix*
OGComplexDiagonalMatrix::asOGComplexDiagonalMatrix() const
{
  return this;
}


ExprType_t
OGComplexDiagonalMatrix::getType() const
{
  return COMPLEX_DIAGONAL_MATRIX_ENUM;
}

/**
 * OGSparseMatrix
 */

template<typename T>
OGSparseMatrix<T>::OGSparseMatrix(int * colPtr, int * rowIdx, T* data, int rows, int cols)
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
OGSparseMatrix<T>::equals(const OGTerminal * other) const
{
  if(!OGArray<T>::equals(other))
  {
    return false;
  }
  const OGSparseMatrix * typetwiddle = static_cast<const OGSparseMatrix<T> *>(other);
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

OGNumeric*
OGRealSparseMatrix::copy() const
{
  return new OGRealSparseMatrix(this->getColPtr(), this->getRowIdx(), this->getData(),
                                this->getRows(), this->getCols());
}

const OGRealSparseMatrix*
OGRealSparseMatrix::asOGRealSparseMatrix() const
{
  return this;
}

ExprType_t
OGRealSparseMatrix::getType() const
{
  return REAL_SPARSE_MATRIX_ENUM;
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

OGNumeric*
OGComplexSparseMatrix::copy() const
{
  return new OGComplexSparseMatrix(this->getColPtr(), this->getRowIdx(), this->getData(),
                                   this->getRows(), this->getCols());
}

const OGComplexSparseMatrix*
OGComplexSparseMatrix::asOGComplexSparseMatrix() const
{
  return this;
}

ExprType_t
OGComplexSparseMatrix::getType() const
{
  return COMPLEX_SPARSE_MATRIX_ENUM;
}

} // namespace librdag
