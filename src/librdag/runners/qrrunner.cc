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
 *  Unit contains code for QR node runners
 */
namespace librdag {

template<typename T> void qr_dense_runner(RegContainer& reg, shared_ptr<const OGMatrix<T>> arg)
{
  int4 m = arg->getRows();
  int4 n = arg->getCols();
  int4 lda = m;
  int4 minmn = m > n ? n : m;
  int4 mn = m * n;
  int4 info = 0;

  // Size of input
  //   A = [m x n]

  // Sizes of output
  //   Q = [m x m]
  //   R = [m x n]

  // Temporary store of elementary reflector scalings
  unique_ptr<T[]> Tauptr(new T[minmn]());

  // Will become the working copy of "A"
  unique_ptr<T[]> Aptr(new T[mn]());
  T * A = Aptr.get();
  std::copy(arg->getData(), arg->getData()+mn, A);

  // These are used later when we have decided what is "Q" and what is "R".
  T * R;
  unique_ptr<T[]> Rptr;
  unique_ptr<T[]> Qptr;

  // This is the number of columns of Q to compute, at some point we
  // may want to offer a "thin" QR where the columns of Q mapping the
  // null space are not computed.
  int4 k = m;

  // Compute QR factorisation, a compressed form is returned in A
  lapack::xgeqrf<T, lapack::OnInputCheck::isfinite>(&m, &n, A, &lda, Tauptr.get(), &info);

  // 'A' now contains a compound representation of QR stored as:
  // Q as elementary reflectors in lower triangle/trapeziod.
  // R in upper triangle.

  // Extraction phase.
  // We want to return k columns of Q...
  if(m <= n) // if m <= n then Q will fit in the original A and we need to get a new R then populate it.
  {
    // New R
    Rptr = std::move(unique_ptr<T[]>(new T[mn]()));
    R = Rptr.get();

    // fetch in data from decomposed A.
    // first the triangular part
    for(std::size_t d = 0; d < m; d++)
    {
      for(std::size_t e = 0; e <= d; e++)
      {
        R[d*m + e]=Aptr.get()[d*m + e];
      }
    }
    // second the square (memcpy it)
    std::copy(Aptr.get()+m*m, Aptr.get()+m*n, R+m*m);

    // Q takes ownership of A
    Qptr = std::move(Aptr);
  }
  else // Q won't fit in original A, need a new Q and to zero out bottom of R.
  {
    // New Q
    Qptr = std::move(unique_ptr<T[]>(new T[m*m]()));

    // copy A into Q
    std::copy(A, A+mn, Qptr.get());

    // assign A's memory to R
    Rptr = std::move(Aptr);
    R = Rptr.get();

    // zero out bottom of R
    for(std::size_t d = 0; d < n; d++)
    {
      for(std::size_t e = d+1; e < m; e++)
      {
        R[d*m + e]=0.e0;
      }
    }
  }
  // This call is confusing in that is uses m-n in the first few arg names
  // but these aren't the usual rows x cols...
  // The first 3 parameters are:
  // m the order of the original matrix that's been decomposed
  // k the number of columns of the matrix "Q" to compute (max m)
  // nReflectors the number of reflectors whose product defines Q, this is min(k, n)
  int4 nReflectors = std::min(k, n);
  lapack::xxxgqr<T, lapack::OnInputCheck::isfinite>(&m, &k, &nReflectors, Qptr.get(), &m, Tauptr.get(), &info);

  // Else, exception propagates, stack unwinds

  OGNumeric::Ptr cQ = makeConcreteDenseMatrix(Qptr.release(), m, m, OWNER);
  OGNumeric::Ptr cR = makeConcreteDenseMatrix(Rptr.release(), m, n, OWNER);

  reg.push_back(cQ);
  reg.push_back(cR);
}

void *
QRRunner::run(RegContainer& reg, OGRealScalar::Ptr arg) const
{
  // single real space qr is just q=1, r=value
  reg.push_back(OGRealScalar::create(1.e0));
  reg.push_back(OGRealScalar::create(arg->getValue()));
  return nullptr;
}
void *
QRRunner::run(RegContainer& reg, OGRealDenseMatrix::Ptr arg) const
{
  qr_dense_runner<real8>(reg, arg);
  return nullptr;
}

void *
QRRunner::run(RegContainer& reg, OGComplexDenseMatrix::Ptr arg) const
{
  qr_dense_runner<complex16>(reg, arg);
  return nullptr;
}

} // end namespace
