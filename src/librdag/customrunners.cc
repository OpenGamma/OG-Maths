/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for licence.
 *
 */

#include "dispatch.hh"
#include "customrunners.hh"
#include "expression.hh"
#include "iss.hh"
#include "terminal.hh"
#include "uncopyable.hh"
#include "lapack.hh"

#include <stdio.h>

/**
 * Unit contains code for custom (i.e. non-autogenerated) runners
 */
namespace librdag {

void *
NORM2Runner::run(RegContainer SUPPRESS_UNUSED * reg, OGRealScalar const SUPPRESS_UNUSED * arg) const
{
  const OGRealScalar* ret;
  ret = new OGRealScalar(fabs(arg->getValue()));
  reg->push_back(ret);
  return nullptr;
}
void *
NORM2Runner::run(RegContainer SUPPRESS_UNUSED * reg, OGRealMatrix const SUPPRESS_UNUSED * arg) const
{
  const OGRealScalar* ret = nullptr; // the returned item

  // Matrix in scalar context, i.e. a 1x1 matrix, norm2 is simply abs(value)
  if(arg->getRows()==1 && arg->getCols()==1)
  {
    ret = new OGRealScalar(fabs(arg->getData()[0]));
  }
  else if(isVector(arg)) // Matrix is a vector, norm2 computed via BLAS dnrm2
  {
    int one = 1;
    int len = arg->getRows() > arg->getCols() ? arg->getRows(): arg->getCols();
    real16 value = F77FUNC(dnrm2)(&len, arg->getData(), &one);
    ret = new OGRealScalar(value);
  }
  else // Matrix is a full matrix, norm2 computed via LAPACK dgesvd
  {
    int m = arg->getRows();
    int n = arg->getCols();
    int lda = m > 1 ? m : 1;
    int ldu = lda;
    int minmn = m > n ? n : m;
    int ldvt = minmn;
    int lwork = -1; // set for query
    int info = 0;

    real16 * tmp = new real16[1];
    real16 * U = nullptr; //IGNORED
    real16 * VT = nullptr; //IGNORED
    real16 * S = new real16[minmn];

    real16 * WORK = tmp; // set properly after query, alloc slot of 1 needed as the work space dimension is written here.

    char optN = 'N';

    // copy A else it's destroyed
    real16 * A = new real16[m*n];
    std::memcpy(A, arg->getData(), sizeof(real16)*m*n);

    // query workspace
    F77FUNC(dgesvd)(&optN, &optN, &m, &n, A, &lda, S, U, &ldu, VT, &ldvt, WORK, &lwork, &info);

    // set workspace
    lwork = (int)WORK[0];
    WORK = new real16[lwork];

    // finally, the call
    F77FUNC(dgesvd)(&optN, &optN, &m, &n, A, &lda, S, U, &ldu, VT, &ldvt, WORK, &lwork, &info);
    ret = new OGRealScalar(S[0]);
    delete[] tmp;
    delete[] A;
    delete[] S;
    delete[] WORK;
  }

  // shove ret into register
  reg->push_back(ret);
  return nullptr;
}
void *
NORM2Runner::run(RegContainer SUPPRESS_UNUSED * reg, OGComplexMatrix const SUPPRESS_UNUSED * arg) const
{
  throw rdag_error("Unimplemented unary expression node");
  return nullptr;
}

} // end namespace