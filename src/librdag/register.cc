/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "register.hh"
#include "exceptions.hh"
#include "debug.h"

namespace librdag {

/**
 * Register
 */

Register::Register()
{
  _refCount = 0;
}

Register::~Register()
{
  if (_refCount != 0)
  {
    DEBUG_PRINT("Possible leak: destruction of Register where _refCount != 0\n");
  }
}

int
Register::incRef()
{
  if (_refCount == 0)
  {
    alloc();
  }

  _refCount++;

  return _refCount;
}

int
Register::decRef()
{
  if (_refCount < 1)
  {
    throw rdag_error("Refcount for register has fallen below zero");
  }

  _refCount--;

  if (_refCount == 0)
  {
    free();
  }

  return _refCount;
}


/**
 * OGRealScalarRegister
 */

OGRealScalarRegister::OGRealScalarRegister(): OGRealScalar(getNaN()), Register() {}

void
OGRealScalarRegister::alloc() {}

void
OGRealScalarRegister::free() {}

OGNumeric * 
OGRealScalarRegister::copy() const
{
  return OGRealScalar::copy();
}

/**
 * OGComplexScalarRegister
 */
OGComplexScalarRegister::OGComplexScalarRegister(): OGComplexScalar(getComplexNaN()), Register() {}

void
OGComplexScalarRegister::alloc() {}

void
OGComplexScalarRegister::free() {}

OGNumeric * 
OGComplexScalarRegister::copy() const
{
  return OGComplexScalar::copy();
}

/**
 * OGRealMatrixRegister
 */

OGRealMatrixRegister::OGRealMatrixRegister(int rows, int cols)
                     :OGRealMatrix(rows, cols), Register() {}

void OGRealMatrixRegister::alloc()
{
  real16* data = new real16[getDatalen()]();
  setData(data);

}

void OGRealMatrixRegister::free()
{
  delete[] getData();
  setData(nullptr);
}

OGNumeric * 
OGRealMatrixRegister::copy() const
{
  return OGRealMatrix::copy();
}

/**
 * OGComplexMatrixRegister
 */

OGComplexMatrixRegister::OGComplexMatrixRegister(int rows, int cols)
                        :OGComplexMatrix(rows, cols), Register() {}
void OGComplexMatrixRegister::alloc()
{
  complex16* data = new complex16[getDatalen()]();
  setData(data);

}

void OGComplexMatrixRegister::free()
{
  delete[] getData();
  setData(nullptr);
}

OGNumeric * 
OGComplexMatrixRegister::copy() const
{
  return OGComplexMatrix::copy();
}

/**
 * OGRealDiagonalMatrixRegister
 */

OGRealDiagonalMatrixRegister::OGRealDiagonalMatrixRegister(int rows, int cols)
                             :OGRealDiagonalMatrix(rows, cols), Register() {}

void OGRealDiagonalMatrixRegister::alloc()
{
  real16* data = new real16[getDatalen()]();
  setData(data);

}

void OGRealDiagonalMatrixRegister::free()
{
  delete[] getData();
  setData(nullptr);
}

OGNumeric * 
OGRealDiagonalMatrixRegister::copy() const
{
  return OGRealDiagonalMatrix::copy();
}

/**
 * OGComplexDiagonalMatrixRegister
 */

OGComplexDiagonalMatrixRegister::OGComplexDiagonalMatrixRegister(int rows, int cols)
                                :OGComplexDiagonalMatrix(rows, cols), Register() {}
void OGComplexDiagonalMatrixRegister::alloc()
{
  complex16* data = new complex16[getDatalen()]();
  setData(data);

}

void OGComplexDiagonalMatrixRegister::free()
{
  delete[] getData();
  setData(nullptr);
}

OGNumeric * 
OGComplexDiagonalMatrixRegister::copy() const
{
  return OGComplexDiagonalMatrix::copy();
}

} // namespace librdag
