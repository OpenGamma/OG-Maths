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
 *  Unit contains code for CTRANSPOSE node runners
 */
namespace librdag {

void *
CTRANSPOSERunner::run(RegContainer& reg, OGRealScalar const * arg) const
{
  const OGRealScalar* ret;
  ret = new OGRealScalar(arg->getValue());
  reg.push_back(ret);
  return nullptr;
}

template<typename T>
void
ctranspose_dense_runner(RegContainer& reg, OGMatrix<T> const * arg)
{
  const OGTerminal* ret = nullptr; // the returned item

  // Matrix in scalar context, i.e. a 1x1 matrix, transpose is simply value
  if(arg->getRows()==1 && arg->getCols()==1)
  {
    ret = makeConcreteScalar(std::conj(arg->getData()[0]));
  }
  else // Matrix is a full matrix
  {
    int m = arg->getRows();
    int n = arg->getCols();
    int retRows = n, retCols = m;
    T * data = arg->getData();
    T * tmp = new T[m * n];
    int ir;
    for (int i = 0; i < n; i++) {
      ir = i * m;
      for (int j = 0; j < m; j++) {
        tmp[j * n + i] = std::conj(data[ir + j]);
      }
    }
    ret = makeConcreteDenseMatrix(tmp, retRows, retCols, OWNER);
  }

  // shove ret into register
  reg.push_back(ret);
}

void *
CTRANSPOSERunner::run(RegContainer& reg, OGRealMatrix const * arg) const
{
  ctranspose_dense_runner(reg, arg);
  return nullptr;
}

void *
CTRANSPOSERunner::run(RegContainer& reg, OGComplexMatrix const * arg) const
{
  ctranspose_dense_runner(reg, arg);
  return nullptr;
}

} // end namespace
