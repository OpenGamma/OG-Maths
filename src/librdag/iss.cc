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

// not sure whether is makes more sense for vectors to include scalar contexts or not
// presently scalar is not a member of vector
bool isVector(const OGTerminal * terminal)
{
  if(isScalar(terminal)) return false;
  if((terminal->getRows()==1 && terminal->getCols() > 1) ||(terminal->getRows() > 1 && terminal->getCols() == 1))
  {
    return true;
  }
  return false;
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


bool isScalar(pOGTerminal t)
{
  return isScalar(t.get());
}

bool isMatrix(pOGTerminal t)
{
  return isMatrix(t.get());
}

bool isVector(pOGTerminal t)
{
  return isVector(t.get());
}

bool isReal(pOGTerminal t)
{
  return isReal(t.get());
}

bool isComplex(pOGTerminal t)
{
  return isComplex(t.get());
}

} // end namespace librdag
