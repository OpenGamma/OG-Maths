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

using namespace std;

/**
 *  Unit contains code for SVD node runners
 */
namespace librdag {

template<typename T> void svd_dense_runner(RegContainer& reg, shared_ptr<const OGMatrix<T>> arg)
{
  int4 m = arg->getRows();
  int4 n = arg->getCols();
  int4 lda = m > 1 ? m : 1;
  int4 ldu = lda;
  int4 minmn = m > n ? n : m;
  int4 ldvt = n;
  int4 info = 0;

  T * U = new T[ldu*m];
  T * VT = new T[ldvt*n];
  real8 * S = new real8[minmn];

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
  reg.push_back(OGRealDiagonalMatrix::create(S, m, n, OWNER));
  reg.push_back(makeConcreteDenseMatrix(VT, n, n, OWNER));
}

void *
SVDRunner::run(RegContainer& reg, OGRealScalar::Ptr arg) const
{
  // real space svd is just u=1, s=value, v=1
  reg.push_back(OGRealScalar::create(1.e0));
  reg.push_back(OGRealScalar::create(arg->getValue()));
  reg.push_back(OGRealScalar::create(1.e0));
  return nullptr;
}
void *
SVDRunner::run(RegContainer& reg, OGRealDenseMatrix::Ptr arg) const
{
  svd_dense_runner<real8>(reg, arg);
  return nullptr;
}

void *
SVDRunner::run(RegContainer& reg, OGComplexMatrix::Ptr arg) const
{
  svd_dense_runner<complex16>(reg, arg);
  return nullptr;
}

} // end namespace
