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

real16 getNaN()
{
  union {
    real16 d;
    unsigned long long int i;
  } nanval;
  nanval.i = 0x7FF1010101010101;
  return nanval.d;
}

OGRealScalarRegister::OGRealScalarRegister(): OGRealScalar(getNaN()), Register() {}

void
OGRealScalarRegister::alloc() {}

void
OGRealScalarRegister::free() {}

/**
 * OGComplexScalarRegister
 */

complex16 getComplexNaN()
{
  union {
    real16 d;
    unsigned long long int i;
  } nanval1, nanval2;
  nanval1.i = 0x7FFDEAD101010101;
  nanval2.i = 0x7FFDEAD202020202;
  return complex16(nanval1.d, nanval2.d);
}

OGComplexScalarRegister::OGComplexScalarRegister(): OGComplexScalar(getComplexNaN()), Register() {}

void
OGComplexScalarRegister::alloc() {}

void
OGComplexScalarRegister::free() {}

/**
 * OGRealMatrixRegister
 */

OGRealMatrixRegister::OGRealMatrixRegister(int rows, int cols)
                     :OGRealMatrix(rows, cols), Register() {}

void OGRealMatrixRegister::alloc()
{
  real16* data = new real16[getDatalen()];
  setData(data);

}

void OGRealMatrixRegister::free()
{
  delete[] getData();
  setData(nullptr);
}

/**
 * OGComplexMatrixRegister
 */

OGComplexMatrixRegister::OGComplexMatrixRegister(int rows, int cols)
                        :OGComplexMatrix(rows, cols), Register() {}
void OGComplexMatrixRegister::alloc()
{
  complex16* data = new complex16[getDatalen()];
  setData(data);

}

void OGComplexMatrixRegister::free()
{
  delete[] getData();
  setData(nullptr);
}

/**
 * OGRealDiagonalMatrixRegister
 */

OGRealDiagonalMatrixRegister::OGRealDiagonalMatrixRegister(int rows, int cols)
                             :OGRealDiagonalMatrix(rows, cols), Register() {}

void OGRealDiagonalMatrixRegister::alloc()
{
  real16* data = new real16[getDatalen()];
  setData(data);

}

void OGRealDiagonalMatrixRegister::free()
{
  delete[] getData();
  setData(nullptr);
}

/**
 * OGComplexDiagonalMatrixRegister
 */

OGComplexDiagonalMatrixRegister::OGComplexDiagonalMatrixRegister(int rows, int cols)
                                :OGComplexDiagonalMatrix(rows, cols), Register() {}
void OGComplexDiagonalMatrixRegister::alloc()
{
  complex16* data = new complex16[getDatalen()];
  setData(data);

}

void OGComplexDiagonalMatrixRegister::free()
{
  delete[] getData();
  setData(nullptr);
}


} // namespace librdag
