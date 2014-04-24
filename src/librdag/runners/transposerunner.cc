/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for licence.
 *
 */

#include "dispatch.hh"
#include "runners.hh"
#include "expression.hh"
#include "iss.hh"
#include "terminal.hh"
#include "uncopyable.hh"
#include "lapack.hh"

#include <stdio.h>
#include <complex>
#include <sstream>

/*
 *  Unit contains code for TRANSPOSE node runners
 */
namespace librdag {

void *
TRANSPOSERunner::run(RegContainer& reg, OGRealScalar const * arg) const
{
  const OGRealScalar* ret;
  ret = new OGRealScalar(arg->getValue());
  reg.push_back(ret);
  return nullptr;
}

template<typename T>
void
transpose_dense_runner(RegContainer& reg, OGMatrix<T> const * arg)
{
  const OGTerminal* ret = nullptr; // the returned item

  // Matrix in scalar context, i.e. a 1x1 matrix, transpose is simply value
  if(arg->getRows()==1 && arg->getCols()==1)
  {
    ret = makeConcreteScalar(arg->getData()[0]);
  }
  else // Matrix is a full matrix
  {
    int m = arg->getRows();
    int n = arg->getCols();
    int retRows = n, retCols = m;
    T * data = arg->getData();
    T * tmp = new T[m * n];
    if(isVector(arg))
    {
      int len = m > n ? m : n;
      std::copy(data, data+len, tmp);
    }
    else
    {
      int ir;
      for (int i = 0; i < n; i++) {
        ir = i * m;
        for (int j = 0; j < m; j++) {
          tmp[j * n + i] = data[ir + j];
        }
      }
    }
    ret = makeConcreteDenseMatrix(tmp, retRows, retCols, OWNER);
  }

  // shove ret into register
  reg.push_back(ret);
}

void *
TRANSPOSERunner::run(RegContainer& reg, OGRealMatrix const * arg) const
{
  transpose_dense_runner(reg, arg);
  return nullptr;
}

void *
TRANSPOSERunner::run(RegContainer& reg, OGComplexMatrix const * arg) const
{
  transpose_dense_runner(reg, arg);
  return nullptr;
}

} // end namespace
