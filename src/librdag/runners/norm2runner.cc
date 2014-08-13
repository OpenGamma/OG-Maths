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
 *  Unit contains code for NORM2 node runners
 */
namespace librdag {

void *
NORM2Runner::run(RegContainer& reg, OGRealScalar::Ptr arg) const
{
  OGNumeric::Ptr ret = OGRealScalar::create(fabs(arg->getValue()));
  reg.push_back(ret);
  return nullptr;
}

template<typename T>
void
norm2_dense_runner(RegContainer& reg, shared_ptr<const OGMatrix<T>> arg)
{
  OGNumeric::Ptr ret; // the returned item

  // Matrix in scalar context, i.e. a 1x1 matrix, norm2 is simply abs(value)
  if(arg->getRows()==1 && arg->getCols()==1)
  {
    ret = OGRealScalar::create(std::abs(arg->getData()[0]));
  }
  else if(isVector(arg)) // Matrix is a vector, norm2 computed via BLAS xnrm2
  {
    int4 len = arg->getRows() > arg->getCols() ? arg->getRows(): arg->getCols();
    real8 value = lapack::xnrm2(&len, arg->getData(), lapack::ione);
    ret = OGRealScalar::create(value);
  }
  else // Matrix is a full matrix, norm2 computed via LAPACK xgesvd
  {
    int4 m = arg->getRows();
    int4 n = arg->getCols();
    int4 lda = m > 1 ? m : 1;
    int4 ldu = lda;
    int4 minmn = m > n ? n : m;
    int4 ldvt = minmn;
    int4 info = 0;

    T * U = nullptr; //IGNORED
    T * VT = nullptr; //IGNORED
    real8 * S = new real8[minmn];

    // copy A else it's destroyed
    T * A = new T[m*n];
    std::memcpy(A, arg->getData(), sizeof(T)*m*n);

    // call lapack
    lapack::xgesvd<T,lapack::OnInputCheck::isfinite>(lapack::N, lapack::N, &m, &n, A, &lda, S, U, &ldu, VT, &ldvt, &info);

    ret = OGRealScalar::create(std::real(S[0]));
    delete[] A;
    delete[] S;
  }

  // shove ret into register
  reg.push_back(ret);
}

void *
NORM2Runner::run(RegContainer& reg, OGRealDenseMatrix::Ptr arg) const
{
  norm2_dense_runner<real8>(reg, arg);
  return nullptr;
}

void *
NORM2Runner::run(RegContainer& reg, OGComplexDenseMatrix::Ptr arg) const
{
  norm2_dense_runner<complex16>(reg, arg);
  return nullptr;
}

} // end namespace
