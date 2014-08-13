/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "iss.hh"

namespace librdag {

bool isScalar(const OGTerminal::Ptr& terminal)
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

bool isMatrix(const OGTerminal::Ptr& terminal)
{
  return !isScalar(terminal);
}

// not sure whether is makes more sense for vectors to include scalar contexts or not
// presently scalar is not a member of vector
bool isVector(const OGTerminal::Ptr& terminal)
{
  if(isScalar(terminal)) return false;
  if((terminal->getRows()==1 && terminal->getCols() > 1) ||(terminal->getRows() > 1 && terminal->getCols() == 1))
  {
    return true;
  }
  return false;
}

bool isReal(const OGTerminal::Ptr& terminal)
{
  if(terminal->asOGRealScalar()!=nullptr)
  {
    return true;
  }
  if(terminal->asOGLogicalMatrix()!=nullptr)
  {
    return true;
  }
  if(terminal->asOGRealDenseMatrix()!=nullptr)
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


bool isComplex(const OGTerminal::Ptr& terminal)
{
  return !isReal(terminal);
}


template<>
bool isfinite(real8 * data, int4 n)
{
  for(int4 k = 0; k < n; k++)
  {
    if(!std::isfinite(data[k]))
    {
      return false;
    }
  }
  return true;
}

template<>
bool isfinite(complex16 * data, int4 n)
{
  for(int4 k = 0; k < n; k++)
  {
    if(!(std::isfinite(std::real(data[k]))||std::isfinite(std::imag(data[k]))))
    {
      return false;
    }
  }
  return true;
}

} // end namespace librdag
