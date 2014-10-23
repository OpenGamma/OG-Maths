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
 * Unit contains code for SUMROWS node runners
 * SUMROWS essentially performs a summation of the row space of a matrix
 */
namespace librdag {

void *
SUMROWSRunner::run(RegContainer& reg, OGRealScalar::Ptr arg) const
{
  OGNumeric::Ptr ret = OGRealScalar::create(arg->getValue());
  reg.push_back(ret);
  return nullptr;
}

template<typename T>
void
sumrows_dense_runner(RegContainer& reg, shared_ptr<const OGMatrix<T>> arg)
{
  OGNumeric::Ptr ret; // the returned item

  if(arg->getCols()==1 && arg->getRows()==1) // is a 1x1
  {
      ret = makeConcreteScalar(arg->getData()[0]);
  }
  else // Matrix is a full matrix, sumrows computed via standard loops, no direct BLAS calls available
  {
    int4 m = arg->getRows();
    int4 n = arg->getCols();

    // the result space
    std::unique_ptr<T[]> newData(new T[arg->getRows()]);
    T * ans = newData.get();

    // the matrix
    T * mat = arg->getData();

    // copy in first col as base for reduction
    std::copy(mat, mat + m, ans);

    for(int4 i = 1; i < n; i++) // for each col, loop peel to account for copy
    {
      const int4 rowStart = i*m;
      for(int4 j = 0; j < m; j++) // for each row
      {
        ans[j]+=mat[rowStart + j]; // reduce row element wise into ans
      }
    }

    ret = makeConcreteDenseMatrix(newData.release(), m, 1, OWNER);
  }

  // shove ret into register
  reg.push_back(ret);
}

void *
SUMROWSRunner::run(RegContainer& reg, OGRealDenseMatrix::Ptr arg) const
{
  sumrows_dense_runner<real8>(reg, arg);
  return nullptr;
}

void *
SUMROWSRunner::run(RegContainer& reg, OGComplexDenseMatrix::Ptr arg) const
{
  sumrows_dense_runner<complex16>(reg, arg);
  return nullptr;
}

} // end namespace
