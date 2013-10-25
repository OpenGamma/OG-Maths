/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "iss.hh"

namespace librdag {

bool isScalar(const OGTerminal * terminal)
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

bool isMatrix(const OGTerminal * terminal)
{
  return !isScalar(terminal);
}

bool isReal(const OGTerminal * terminal)
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

bool isComplex(const OGTerminal * terminal)
{
  return !isReal(terminal);
}

}