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
    throw librdagException();
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

OGRealScalarRegister::OGRealScalarRegister(): OGRealScalar(0.0), Register() {}

void
OGRealScalarRegister::alloc() {}

void
OGRealScalarRegister::free() {}

/**
 * OGComplexScalarRegister
 */

OGComplexScalarRegister::OGComplexScalarRegister(): OGComplexScalar(0.0), Register() {}

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

} // namespace librdag
