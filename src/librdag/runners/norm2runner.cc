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
 *  Unit contains code for NORM2 node runners
 */
namespace librdag {

void *
NORM2Runner::run(RegContainer& reg, OGRealScalar const * arg) const
{
  const OGRealScalar* ret;
  ret = new OGRealScalar(fabs(arg->getValue()));
  reg.push_back(ret);
  return nullptr;
}

template<typename T>
void
norm2_dense_runner(RegContainer& reg, OGMatrix<T> const * arg)
{
  const OGRealScalar* ret = nullptr; // the returned item

  // Matrix in scalar context, i.e. a 1x1 matrix, norm2 is simply abs(value)
  if(arg->getRows()==1 && arg->getCols()==1)
  {
    ret = new OGRealScalar(std::abs(arg->getData()[0]));
  }
  else if(isVector(arg)) // Matrix is a vector, norm2 computed via BLAS xnrm2
  {
    int len = arg->getRows() > arg->getCols() ? arg->getRows(): arg->getCols();
    real16 value = lapack::xnrm2(&len, arg->getData(), lapack::ione);
    ret = new OGRealScalar(value);
  }
  else // Matrix is a full matrix, norm2 computed via LAPACK xgesvd
  {
    int m = arg->getRows();
    int n = arg->getCols();
    int lda = m > 1 ? m : 1;
    int ldu = lda;
    int minmn = m > n ? n : m;
    int ldvt = minmn;
    int info = 0;

    T * U = nullptr; //IGNORED
    T * VT = nullptr; //IGNORED
    real16 * S = new real16[minmn];

    // copy A else it's destroyed
    T * A = new T[m*n];
    std::memcpy(A, arg->getData(), sizeof(T)*m*n);

    // call lapack
    lapack::xgesvd(lapack::N, lapack::N, &m, &n, A, &lda, S, U, &ldu, VT, &ldvt, &info);

    ret = new OGRealScalar(std::real(S[0]));
    delete[] A;
    delete[] S;
  }

  // shove ret into register
  reg.push_back(ret);
}

void *
NORM2Runner::run(RegContainer& reg, OGRealMatrix const * arg) const
{
  norm2_dense_runner(reg, arg);
  return nullptr;
}

void *
NORM2Runner::run(RegContainer& reg, OGComplexMatrix const * arg) const
{
  norm2_dense_runner(reg, arg);
  return nullptr;
}

} // end namespace
