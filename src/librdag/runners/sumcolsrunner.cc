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

using namespace std;

/*
 * Unit contains code for SUMCOLS node runners
 * SUMCOLS essentially performs a summation of the column space of a matrix
 */
namespace librdag {

void *
SUMCOLSRunner::run(RegContainer& reg, OGRealScalar::Ptr arg) const
{
  OGNumeric::Ptr ret = OGRealScalar::create(arg->getValue());
  reg.push_back(ret);
  return nullptr;
}

template<typename T>
void
sumcols_dense_runner(RegContainer& reg, shared_ptr<const OGMatrix<T>> arg)
{
  OGNumeric::Ptr ret; // the returned item

  // see if this is a 1x1
  if(arg->getRows()==1 && arg->getCols()==1)
  {
      ret = makeConcreteScalar(arg->getData()[0]);
  }
  else // Matrix is a full matrix, sumcols computed via standard loops, no direct BLAS calls available
  {
    int4 m = arg->getRows();
    int4 n = arg->getCols();

    // the matrix
    T * mat = arg->getData();

    // the memory in which the result will be placed
    std::unique_ptr<T[]> newData(new T[n]());
    T * ans = newData.get();

    for(int4 i = 0; i < n; i++) // for each col
    {
      const int4 rowStart = i*m;
      for(int4 j = 0; j < m; j++) // for each row
      {
        ans[i]+=mat[rowStart + j]; // reduce:+ into ans
      }
    }


    ret = makeConcreteDenseMatrix(newData.release(), 1, n, OWNER);
  }

  // shove ret into register
  reg.push_back(ret);
}

void *
SUMCOLSRunner::run(RegContainer& reg, OGRealDenseMatrix::Ptr arg) const
{
  sumcols_dense_runner<real8>(reg, arg);
  return nullptr;
}

void *
SUMCOLSRunner::run(RegContainer& reg, OGComplexDenseMatrix::Ptr arg) const
{
  sumcols_dense_runner<complex16>(reg, arg);
  return nullptr;
}

} // end namespace
