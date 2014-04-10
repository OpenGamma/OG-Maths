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
  char A = 'A';
  char T = 'T';
  char L = 'L';
  char U = 'U';
  char D = 'D';
  char O = 'O';
  char ONE = '1';
  int ione = 1;
  int izero = 0;
  real16 rone = 1.e0;
  complex16 cone = {1.e0, 0.e0};
  real16 rzero = 0.e0;
  complex16 czero = {0.e0, 0.e0};
}

// f77 constants
char *      N     = &detail::N;
char *      A     = &detail::A;
char *      T     = &detail::T;
char *      L     = &detail::L;
char *      U     = &detail::U;
char *      D     = &detail::D;
char *      O     = &detail::O;
char *      ONE   = &detail::ONE;
int *       ione  = &detail::ione;
int *       izero  = &detail::izero;
real16 *    rone  = &detail::rone;
complex16 * cone  = &detail::cone;
real16 *    rzero = &detail::rzero;
complex16 * czero = &detail::czero;

// xSCAL specialisations
template<> void xscal(int * N, real16 * DA, real16 * DX, int * INCX)
{
  set_xerbla_death_switch(lapack::izero);
  F77FUNC(dscal)(N, DA, DX, INCX);
}

template<> void xscal(int * N, complex16 * DA, complex16 * DX, int * INCX)
{
  set_xerbla_death_switch(lapack::izero);
  F77FUNC(zscal)(N, DA, DX, INCX);
}

// xGEMV specialisations
template<> void xgemv(char * TRANS, int * M, int * N, real16 * ALPHA, real16 * A, int * LDA, real16 * X, int * INCX, real16 * BETA, real16 * Y, int * INCY )
{
  set_xerbla_death_switch(lapack::izero);
  F77FUNC(dgemv)(TRANS, M, N, ALPHA, A, LDA, X, INCX, BETA, Y, INCY);
}

template<> void xgemv(char * TRANS, int * M, int * N, complex16 * ALPHA, complex16 * A, int * LDA, complex16 * X, int * INCX, complex16 * BETA, complex16 * Y, int * INCY )
{
  set_xerbla_death_switch(lapack::izero);
  F77FUNC(zgemv)(TRANS, M, N, ALPHA, A, LDA, X, INCX, BETA, Y, INCY);
}

// xGEMM specialisations
template<> void
xgemm(char * TRANSA, char * TRANSB, int * M, int * N, int * K, real16 * ALPHA, real16 * A, int * LDA, real16 * B, int * LDB, real16 * BETA, real16 * C, int * LDC )
{
  set_xerbla_death_switch(lapack::izero);
  F77FUNC(dgemm)(TRANSA, TRANSB, M, N, K, ALPHA, A, LDA, B, LDB, BETA, C, LDC );
}

template<> void
xgemm(char * TRANSA, char * TRANSB, int * M, int * N, int * K, complex16 * ALPHA, complex16 * A, int * LDA, complex16 * B, int * LDB, complex16 * BETA, complex16 * C, int * LDC )
{
  set_xerbla_death_switch(lapack::izero);
  F77FUNC(zgemm)(TRANSA, TRANSB, M, N, K, ALPHA, A, LDA, B, LDB, BETA, C, LDC );
}


// xNORM2 specialisations
template<> real16 xnrm2(int * N, real16 * X, int * INCX)
{
  set_xerbla_death_switch(lapack::izero);
  return F77FUNC(dnrm2)(N, X, INCX);
}

template<> real16 xnrm2(int * N, complex16 * X, int * INCX)
{
  set_xerbla_death_switch(lapack::izero);
  return F77FUNC(dznrm2)(N, X, INCX);
}

// xGESVD specialisations
template<>  void xgesvd(char * JOBU, char * JOBVT, int * M, int * N, real16 * A, int * LDA, real16 * S, real16 * U, int * LDU, real16 * VT, int * LDVT, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  real16 * tmp = new real16[1]; // buffer, alloc slot of 1 needed as the work space dimension is written here.
  int lwork = -1; // set for query
  real16 * WORK = tmp; // set properly after query

  // work space query
  F77FUNC(dgesvd)(JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT, LDVT, WORK, &lwork, INFO);

  if(*INFO < 0)
  {
    delete [] tmp;
    stringstream message;
    message << "Input to LAPACK::dgesvd call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }

  // query complete WORK[0] contains size needed
  lwork = (int)WORK[0];
  WORK = new real16[lwork]();

  // full execution
  F77FUNC(dgesvd)(JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT, LDVT, WORK, &lwork, INFO);

  // clean up
  delete [] tmp;
  delete [] WORK;

  if(*INFO!=0)
  {
    throw rdag_error("LAPACK::dgesvd, internal call to dbdsqr did not converge.");
  }
}

template<>  void xgesvd(char * JOBU, char * JOBVT, int * M, int * N, complex16 * A, int * LDA, real16 * S, complex16 * U, int * LDU, complex16 * VT, int * LDVT, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  int minmn = *M > *N ? *N : *M; // compute scale for RWORK
  complex16 * tmp = new complex16[1]; // buffer, alloc slot of 1 needed as the work space dimension is written here.
  int lwork = -1; // set for query
  real16    * RWORK = nullptr;
  complex16 * WORK = tmp; // set properly after query

  // work space query
  F77FUNC(zgesvd)(JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT, LDVT, WORK, &lwork, RWORK, INFO);
  if(*INFO < 0)
  {
    delete [] tmp;
    stringstream message;
    message << "Input to LAPACK::zgesvd call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }

  // query complete WORK[0] contains size needed
  lwork = (int)(WORK[0].real());
  WORK = new complex16[lwork];
  RWORK = new real16[5*minmn];

  // full execution
  F77FUNC(zgesvd)(JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT, LDVT, WORK, &lwork, RWORK, INFO);

  // clean up
  delete [] tmp;
  delete [] WORK;
  delete [] RWORK;

  if(*INFO!=0)
  {
    throw rdag_error("LAPACK::zgesvd, internal call to zbdsqr did not converge.");
  }
}

template<> void xtrcon(char * NORM, char * UPLO, char * DIAG, int * N, real16 * A, int * LDA, real16 * RCOND, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  if(*N<0)
  {
    stringstream message;
    message << "Input to LAPACK::dtrcon call incorrect at arg: " << 4;
    throw rdag_error(message.str());
  }
  real16 * WORK = new real16[ 3 * *N];
  int * IWORK = new int[*N];
  F77FUNC(dtrcon)(NORM, UPLO, DIAG, N, A, LDA, RCOND, WORK, IWORK, INFO);
  delete [] WORK;
  delete [] IWORK;
  if(*INFO!=0)
  {
      stringstream message;
      message << "Input to LAPACK::dtrcon call incorrect at arg: " << *INFO;
      throw rdag_error(message.str());
  }
}


template<> void xtrcon(char * NORM, char * UPLO, char * DIAG, int * N, complex16 * A, int * LDA, real16 * RCOND, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  if(*N<0)
  {
    stringstream message;
    message << "Input to LAPACK::ztrcon call incorrect at arg: " << 4;
    throw rdag_error(message.str());
  }
  complex16 * WORK = new complex16[ 2 * *N];
  real16 * RWORK = new real16[*N];
  F77FUNC(ztrcon)(NORM, UPLO, DIAG, N, A, LDA, RCOND, WORK, RWORK, INFO);
  delete [] WORK;
  delete [] RWORK;
  if(*INFO!=0)
  {
    stringstream message;
    message << "Input to LAPACK::ztrcon call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
}

template<> void xtrtrs(char * UPLO, char * TRANS, char * DIAG, int * N, int * NRHS, real16 * A, int * LDA, real16 * B, int * LDB, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  F77FUNC(dtrtrs)(UPLO, TRANS, DIAG, N, NRHS, A, LDA, B, LDB, INFO);
  if(*INFO>0)
  {
    stringstream message;
    message << "LAPACK::dtrtrs matrix is reported as being singular, the " << *INFO << "th diagonal is zero. No solutions have been computed.";
    throw rdag_error(message.str());
  }
  if(*INFO<0)
  {
    stringstream message;
    message << "Input to LAPACK::dtrtrs call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
}

template<> void xtrtrs(char * UPLO, char * TRANS, char * DIAG, int * N, int * NRHS, complex16 * A, int * LDA, complex16 * B, int * LDB, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  F77FUNC(ztrtrs)(UPLO, TRANS, DIAG, N, NRHS, A, LDA, B, LDB, INFO);
  if(*INFO>0)
  {
    stringstream message;
    message << "LAPACK::dtrtrs matrix is reported as being singular, the " << *INFO << "th diagonal is zero. No solutions have been computed.";
    throw rdag_error(message.str());
  }
  if(*INFO!=0)
  {
    stringstream message;
    message << "Input to LAPACK::ztrtrs call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
}


template<> void xpotrf(char * UPLO, int * N, real16 * A, int * LDA, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  F77FUNC(dpotrf)(UPLO, N, A, LDA, INFO);
  if(*INFO>0)
  {
    stringstream message;
    message << "LAPACK::dpotrf matrix is reported as being non s.p.d OR the factorisation failed. The " << *INFO << "th leading minor is not positive definite.";
    throw rdag_error(message.str());
  }
  if(*INFO<0)
  {
    stringstream message;
    message << "Input to LAPACK::dpotrf call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
}

template<> void xpotrf(char * UPLO, int * N, complex16 * A, int * LDA, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  F77FUNC(zpotrf)(UPLO, N, A, LDA, INFO);
  if(*INFO>0)
  {
    stringstream message;
    message << "LAPACK::zpotrf matrix is reported as being non s.p.d OR the factorisation failed. The " << *INFO << "th leading minor is not positive definite.";
    throw rdag_error(message.str());
  }
  if(*INFO<0)
  {
    stringstream message;
    message << "Input to LAPACK::zpotrf call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
}

template<> void xpocon(char * UPLO, int * N, real16 * A, int * LDA, real16 * ANORM, real16 * RCOND, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  if(*N<0)
  {
    stringstream message;
    message << "Input to LAPACK::dpocon call incorrect at arg: " << 2;
    throw rdag_error(message.str());
  }
  real16 * WORK = new real16[3 * (*N)];
  int * IWORK = new int[(*N)];
  F77FUNC(dpocon)(UPLO, N, A, LDA, ANORM, RCOND, WORK, IWORK, INFO);
  delete [] WORK;
  delete [] IWORK;
  if(*INFO<0)
  {
    stringstream message;
    message << "Input to LAPACK::dpocon call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
}

template<> void xpocon(char * UPLO, int * N, complex16 * A, int * LDA, real16 * ANORM, real16 * RCOND, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  if(*N<0)
  {
    stringstream message;
    message << "Input to LAPACK::zpocon call incorrect at arg: " << 2;
    throw rdag_error(message.str());
  }
  complex16 * WORK = new complex16[2 * *N];
  real16 * RWORK = new real16[*N];
  F77FUNC(zpocon)(UPLO, N, A, LDA, ANORM, RCOND, WORK, RWORK, INFO);
  delete [] WORK;
  delete [] RWORK;
  if(*INFO<0)
  {
    stringstream message;
    message << "Input to LAPACK::dpocon call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
}

template<>real16 xlansy(char * NORM, char * UPLO, int * N, real16 * A, int * LDA)
{
  set_xerbla_death_switch(lapack::izero);
  if(*N<0)
  {
    stringstream message;
    message << "Input to LAPACK::dlansy call incorrect at arg: " << 3;
    throw rdag_error(message.str());
  }
  real16 * WORK = new real16[*N]; // allocate regardless of *NORM
  real16 ret = F77FUNC(dlansy)(NORM, UPLO, N, A, LDA, WORK);
  delete [] WORK;
  return ret;
}

template<>real16 xlansy(char * NORM, char * UPLO, int * N, complex16 * A, int * LDA)
{
  set_xerbla_death_switch(lapack::izero);
  if(*N<0)
  {
    stringstream message;
    message << "Input to LAPACK::zlansy call incorrect at arg: " << 3;
    throw rdag_error(message.str());
  }
  real16 * WORK = new real16[*N]; // allocate regardless of *NORM
  real16 ret = F77FUNC(zlansy)(NORM, UPLO, N, A, LDA, WORK);
  delete [] WORK;
  return ret;
}


real16 zlanhe(char * NORM, char * UPLO, int * N, complex16 * A, int * LDA)
{
  set_xerbla_death_switch(lapack::izero);
  real16 * WORK = new real16[*N]; // allocate regardless of *NORM
  real16 ret = F77FUNC(zlanhe)(NORM, UPLO, N, A, LDA, WORK);
  delete [] WORK;
  return ret;
}


template<> void xpotrs(char * UPLO, int * N, int * NRHS, real16 * A, int * LDA, real16 * B, int * LDB, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  F77FUNC(dpotrs)(UPLO, N, NRHS, A, LDA, B, LDB, INFO);
  if(*INFO<0)
  {
    stringstream message;
    message << "Input to LAPACK::dpotrs call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
}

template<> void xpotrs(char * UPLO, int * N, int * NRHS, complex16 * A, int * LDA, complex16 * B, int * LDB, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  F77FUNC(zpotrs)(UPLO, N, NRHS, A, LDA, B, LDB, INFO);
  if(*INFO<0)
  {
    stringstream message;
    message << "Input to LAPACK::zpotrs call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
}


} // end namespace lapack