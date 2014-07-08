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

  unique_ptr<T[]> Uptr (new T[ldu*m]);
  unique_ptr<T[]> VTptr (new T[ldvt*n]);
  unique_ptr<real8[]> Sptr ( new real8[minmn]);
  T * U = Uptr.get();
  T * VT = VTptr.get();
  real8 * S = Sptr.get();

  // copy A else it's destroyed
  unique_ptr<T[]> Aptr (new T[m*n]);
  T * A = Aptr.get();
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
      throw e;
    }
    else // failed convergence TODO: this will end up in logs (MAT-369) and userland (MAT-370).
    {
      cerr << e.what() << std::endl;
    }
  }

  reg.push_back(makeConcreteDenseMatrix(Uptr.release(), m, m, OWNER));
  reg.push_back(OGRealDiagonalMatrix::create(Sptr.release(), m, n, OWNER));
  reg.push_back(makeConcreteDenseMatrix(VTptr.release(), n, n, OWNER));
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
SVDRunner::run(RegContainer& reg, OGComplexDenseMatrix::Ptr arg) const
{
  svd_dense_runner<complex16>(reg, arg);
  return nullptr;
}

} // end namespace
