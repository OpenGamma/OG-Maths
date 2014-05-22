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
#include "debug.h"


/**
 *  Unit contains code for SVD node runners
 */
namespace librdag {

template<typename T> void svd_dense_runner(RegContainer& reg, const OGMatrix<T>* arg)
{
  int m = arg->getRows();
  int n = arg->getCols();
  int lda = m > 1 ? m : 1;
  int ldu = lda;
  int minmn = m > n ? n : m;
  int ldvt = n;
  int info = 0;

  T * U = new T[ldu*m];
  T * VT = new T[ldvt*n];
  real16 * S = new real16[minmn];

  // copy A else it's destroyed
  T * A = new T[m*n];
  std::memcpy(A, arg->getData(), sizeof(T)*m*n);

  // call lapack
  try
  {
    lapack::xgesvd(lapack::A, lapack::A, &m, &n, A, &lda, S, U, &ldu, VT, &ldvt, &info);
  }
  catch (exception& e)
  {
    if(info < 0) // illegal arg
    {
      delete[] A;
      delete[] U;
      delete[] VT;
      delete[] S;
      throw e;
    }
    else // failed convergence TODO: this will end up in logs (MAT-369) and userland (MAT-370).
    {
      cerr << e.what() << std::endl;
    }
  }
  delete[] A;

  reg.push_back(makeConcreteDenseMatrix(U, m, m, OWNER));
  reg.push_back(new OGRealDiagonalMatrix(S, m, n, OWNER));
  reg.push_back(makeConcreteDenseMatrix(VT, n, n, OWNER));
}

void *
SVDRunner::run(RegContainer& reg, OGRealScalar const *arg) const
{
  // real space svd is just u=1, s=value, v=1
  reg.push_back(new OGRealScalar(1.e0));
  reg.push_back(new OGRealScalar(arg->getValue()));
  reg.push_back(new OGRealScalar(1.e0));
  return nullptr;
}
void *
SVDRunner::run(RegContainer& reg, OGRealMatrix const *arg) const
{
  svd_dense_runner(reg, arg);
  return nullptr;
}

void *
SVDRunner::run(RegContainer& reg, OGComplexMatrix const *arg) const
{
  svd_dense_runner(reg, arg);
  return nullptr;
}

} // end namespace
