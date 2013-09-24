/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "terminal.hh"
#include "exceptions.hh"
#include "visitor.hh"

namespace librdag {

/**
 * OGTerminal
 */

real16**
OGTerminal::toReal16ArrayOfArrays() const
{
  throw librdagException();
}

complex16**
OGTerminal::toComplex16ArrayOfArrays() const
{
  throw librdagException();
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

/**
 * OGArray
 */

template<typename T>
void
OGArray<T>::accept(Visitor &v) const
{
  v.visit(this);
}

template<typename T>
T*
OGArray<T>::getData() const
{
  return _data;
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
  if(data==nullptr)
  {
    throw librdagException();
  }
  _data = data;
}

template<typename T>
void
OGArray<T>::setRows(int rows)
{
  if(rows<0)
  {
    throw librdagException();
  }  
  _rows = rows;
}

template<typename T>
void
OGArray<T>::setCols(int cols)
{
  if(cols<0)
  {
    throw librdagException();
  }
  _cols = cols;
}

template<typename T>
void
OGArray<T>::setDatalen(int datalen)
{
  if(datalen<0)
  {
    throw librdagException();
  }
  _datalen = datalen;
}

template class OGArray<real16>;
template class OGArray<complex16>;

/**
 * OGMatrix
 */

template<typename T>
OGMatrix<T>::OGMatrix(T* data, int rows, int cols)
{
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

/**
 * OGDiagonalMatrix
 */

template<typename T>
OGDiagonalMatrix<T>::OGDiagonalMatrix(T* data, int rows, int cols)
{
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

/**
 * OGSparseMatrix
 */

template<typename T>
OGSparseMatrix<T>::OGSparseMatrix(int * colPtr, int * rowIdx, T* data, int rows, int cols)
{
  this->setData(data);
  this->setRows(rows);
  this->setCols(cols);
  this->setDatalen(colPtr[cols]);
  this->setColPtr(colPtr);
  this->setRowIdx(rowIdx);
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
    throw librdagException();
  }
  _colPtr = colPtr;
}

template<typename T>
void
OGSparseMatrix<T>::setRowIdx(int* rowIdx)
{
  if(rowIdx==nullptr)
  {
    throw librdagException();
  }  
  _rowIdx = rowIdx;
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

} // namespace librdag
