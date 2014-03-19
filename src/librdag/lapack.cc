/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "lapack.hh"
#include "exceptions.hh"

using namespace librdag;

namespace lapack {

namespace detail
{
  char N = 'N';
  int ione = 1;
  real16 rone = 1.e0;
  complex16 cone = {1.e0, 0.e0};
  real16 rzero = 0.e0;
  complex16 czero = {0.e0, 0.e0};
}

// f77 constants
char *      N     = &detail::N;
int *       ione  = &detail::ione;
real16 *    rone  = &detail::rone;
complex16 * cone  = &detail::cone;
real16 *    rzero = &detail::rzero;
complex16 * czero = &detail::czero;

// xSCAL specialisations
template<> void xscal(int * N, real16 * DA, real16 * DX, int * INCX)
{
  F77FUNC(dscal)(N, DA, DX, INCX);
}

template<> void xscal(int * N, complex16 * DA, complex16 * DX, int * INCX)
{
  F77FUNC(zscal)(N, DA, DX, INCX);
}

// xGEMV specialisations
template<> void xgemv(char * TRANS, int * M, int * N, real16 * ALPHA, real16 * A, int * LDA, real16 * X, int * INCX, real16 * BETA, real16 * Y, int * INCY )
{
  F77FUNC(dgemv)(TRANS, M, N, ALPHA, A, LDA, X, INCX, BETA, Y, INCY);
}

template<> void xgemv(char * TRANS, int * M, int * N, complex16 * ALPHA, complex16 * A, int * LDA, complex16 * X, int * INCX, complex16 * BETA, complex16 * Y, int * INCY )
{
  F77FUNC(zgemv)(TRANS, M, N, ALPHA, A, LDA, X, INCX, BETA, Y, INCY);
}

// xGEMM specialisations
template<> void
xgemm(char * TRANSA, char * TRANSB, int * M, int * N, int * K, real16 * ALPHA, real16 * A, int * LDA, real16 * B, int * LDB, real16 * BETA, real16 * C, int * LDC )
{
  F77FUNC(dgemm)(TRANSA, TRANSB, M, N, K, ALPHA, A, LDA, B, LDB, BETA, C, LDC );
}

template<> void
xgemm(char * TRANSA, char * TRANSB, int * M, int * N, int * K, complex16 * ALPHA, complex16 * A, int * LDA, complex16 * B, int * LDB, complex16 * BETA, complex16 * C, int * LDC )
{
  F77FUNC(zgemm)(TRANSA, TRANSB, M, N, K, ALPHA, A, LDA, B, LDB, BETA, C, LDC );
}


// xNORM2 specialisations
template<> real16 xnrm2(int * N, real16 * X, int * INCX)
{
  return F77FUNC(dnrm2)(N, X, INCX);
}

template<> real16 xnrm2(int * N, complex16 * X, int * INCX)
{
  return F77FUNC(dznrm2)(N, X, INCX);
}

// xGESVD specialisations
template<>  void xgesvd(char * JOBU, char * JOBVT, int * M, int * N, real16 * A, int * LDA, real16 * S, real16 * U, int * LDU, real16 * VT, int * LDVT, int * INFO)
{
  real16 * tmp = new real16[1]; // buffer, alloc slot of 1 needed as the work space dimension is written here.
  int lwork = -1; // set for query
  real16 * WORK = tmp; // set properly after query

  // work space query
  F77FUNC(dgesvd)(JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT, LDVT, WORK, &lwork, INFO);

  // query complete WORK[0] contains size needed
  lwork = (int)WORK[0];
  WORK = new real16[lwork];

  // full execution
  F77FUNC(dgesvd)(JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT, LDVT, WORK, &lwork, INFO);
  if(*INFO!=0)
  {
    if(*INFO < 0)
    {
      stringstream message;
      message << "Input to LAPACK::dgesvd call incorrect at arg: " << *INFO;
      throw rdag_error(message.str());
    }
    else
    {
      throw rdag_error("LAPACK::dgesvd, internal call to dbdsqr did not converge.");
    }
  }

  delete [] tmp;
  delete [] WORK;
}

template<>  void xgesvd(char * JOBU, char * JOBVT, int * M, int * N, complex16 * A, int * LDA, real16 * S, complex16 * U, int * LDU, complex16 * VT, int * LDVT, int * INFO)
{
  int minmn = *M > *N ? *N : *M; // compute scale for RWORK
  complex16 * tmp = new complex16[1];
  int lwork = -1; // set for query
  real16    * RWORK = new real16[5*minmn];
  complex16 * WORK = tmp; // set properly after query, alloc slot of 1 needed as the work space dimension is written here.
  F77FUNC(zgesvd)(JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT, LDVT, WORK, &lwork, RWORK, INFO);

  // query complete WORK[0] contains size needed
  lwork = (int)(WORK[0].real());
  WORK = new complex16[lwork];

  // full execution
  F77FUNC(zgesvd)(JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT, LDVT, WORK, &lwork, RWORK, INFO);
    if(*INFO!=0)
    {
      if(*INFO < 0)
      {
        stringstream message;
        message << "Input to LAPACK::zgesvd call incorrect at arg: " << *INFO;
        throw rdag_error(message.str());
      }
      else
      {
        throw rdag_error("LAPACK::zgesvd, internal call to zbdsqr did not converge.");
      }
    }

  delete [] tmp;
  delete [] WORK;
  delete [] RWORK;
}



} // end namespace lapack