/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "iss.hh"

namespace librdag {

DLLEXPORT_C bool isScalar(const OGTerminal * terminal)
{
  if(terminal->asOGRealScalar()!=nullptr)
  {
    return true;
  }
  if(terminal->asOGComplexScalar()!=nullptr)
  {
    return true;
  }
  if(terminal->asOGIntegerScalar()!=nullptr)
  {
    return true;
  }
  return false;
}

DLLEXPORT_C bool isMatrix(const OGTerminal * terminal)
{
  return !isScalar(terminal);
}

DLLEXPORT_C bool isReal(const OGTerminal * terminal)
{
  if(terminal->asOGRealScalar()!=nullptr)
  {
    return true;
  }
  if(terminal->asOGLogicalMatrix()!=nullptr)
  {
    return true;
  }
  if(terminal->asOGRealMatrix()!=nullptr)
  {
    return true;
  }
  if(terminal->asOGRealDiagonalMatrix()!=nullptr)
  {
    return true;
  }
  if(terminal->asOGRealSparseMatrix()!=nullptr)
  {
    return true;
  }
  if(terminal->asOGIntegerScalar()!=nullptr)
  {
    return true;
  }
  return false;
}

DLLEXPORT_C bool isComplex(const OGTerminal * terminal)
{
  return !isReal(terminal);
}

}