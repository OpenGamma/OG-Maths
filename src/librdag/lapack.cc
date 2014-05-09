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

  // charmagic specialisation
  template<>char charmagic<real16>()
  {
    return 'd';
  }
  template<>char charmagic<complex16>()
  {
    return 'z';
  }

  // xSCAL specialisations
  template<> void xscal(int * N, real16 * DA, real16 * DX, int * INCX)
  {
    F77FUNC(dscal)(N, DA, DX, INCX);
  }

  template<> void xscal(int * N, complex16 * DA, complex16 * DX, int * INCX)
  {
    F77FUNC(zscal)(N, DA, DX, INCX);
  }

  // xSWAP specialisations
  template<> void xswap(int * N, real16 * DX, int * INCX, real16 * DY, int * INCY)
  {
    F77FUNC(dswap)(N, DX, INCX, DY, INCY);
  }

  template<> void xswap(int * N, complex16 * DX, int * INCX, complex16 * DY, int * INCY)
  {
    F77FUNC(zswap)(N, DX, INCX, DY, INCY);
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

  // xPOTRS specialisations
  template<> void xpotrs(char * UPLO, int * N, int * NRHS, real16 * A, int * LDA, real16 * B, int * LDB, int * INFO)
  {
    F77FUNC(dpotrs)(UPLO, N, NRHS, A, LDA, B, LDB, INFO);
  }

  template<> void xpotrs(char * UPLO, int * N, int * NRHS, complex16 * A, int * LDA, complex16 * B, int * LDB, int * INFO)
  {
    F77FUNC(zpotrs)(UPLO, N, NRHS, A, LDA, B, LDB, INFO);
  }

  // xLANSY specialisations
  template<>real16 xlansy(char * NORM, char * UPLO, int * N, real16 * A, int * LDA, real16 * WORK)
  {
    return F77FUNC(dlansy)(NORM, UPLO, N, A, LDA, WORK);
  }
  template<>real16 xlansy(char * NORM, char * UPLO, int * N, complex16 * A, int * LDA, real16 * WORK)
  {
    return  F77FUNC(zlansy)(NORM, UPLO, N, A, LDA, WORK);
  }

  // xPOTRF specialisations
  template<> void xpotrf(char * UPLO, int * N, real16 * A, int * LDA, int * INFO)
  {
    F77FUNC(dpotrf)(UPLO, N, A, LDA, INFO);
  }
  template<> void xpotrf(char * UPLO, int * N, complex16 * A, int * LDA, int * INFO)
  {
    F77FUNC(zpotrf)(UPLO, N, A, LDA, INFO);
  }

  //xTRTRS specialisations
  template<> void xtrtrs(char * UPLO, char * TRANS, char * DIAG, int * N, int * NRHS, real16 * A, int * LDA, real16 * B, int * LDB, int * INFO)
  {
    F77FUNC(dtrtrs)(UPLO, TRANS, DIAG, N, NRHS, A, LDA, B, LDB, INFO);
  }
  template<> void xtrtrs(char * UPLO, char * TRANS, char * DIAG, int * N, int * NRHS, complex16 * A, int * LDA, complex16 * B, int * LDB, int * INFO)
  {
    F77FUNC(ztrtrs)(UPLO, TRANS, DIAG, N, NRHS, A, LDA, B, LDB, INFO);
  }

  //xGETRF specialisations
  template<> void xgetrf(int * M, int * N, real16 * A, int * LDA, int * IPIV, int *INFO)
  {
    F77FUNC(dgetrf)(M,N,A,LDA,IPIV,INFO);
  }

  template<> void xgetrf(int * M, int * N, complex16 * A, int * LDA, int * IPIV, int *INFO)
  {
    F77FUNC(zgetrf)(M,N,A,LDA,IPIV,INFO);
  }

  //xGETRI specialisations
  template<> void xgetri(int * N, real16 * A, int * LDA, int * IPIV, real16 * WORK, int * LWORK, int * INFO )
  {
    F77FUNC(dgetri)(N,A,LDA,IPIV,WORK,LWORK,INFO);
  }

  template<> void xgetri(int * N, complex16 * A, int * LDA, int * IPIV, complex16 * WORK, int * LWORK, int * INFO )
  {
    F77FUNC(zgetri)(N,A,LDA,IPIV,WORK,LWORK,INFO);
  }
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

// xSCAL
template<typename T> void xscal(int * N, T * DA, T * DX, int * INCX)
{
  set_xerbla_death_switch(lapack::izero);
  detail::xscal(N, DA, DX, INCX);
}
template void xscal<real16>(int * N, real16 * DA, real16 * DX, int * INCX);
template void xscal<complex16>(int * N, complex16 * DA, complex16 * DX, int * INCX);

// xSWAP
template<typename T> void xswap(int * N, T * DX, int * INCX, T * DY, int * INCY)
{
  set_xerbla_death_switch(lapack::izero);
  detail::xswap(N, DX, INCX, DY, INCY);
}
template void xswap<real16>(int * N, real16 * DX, int * INCX, real16 * DY, int * INCY);
template void xswap<complex16>(int * N, complex16 * DX, int * INCX, complex16 * DY, int * INCY);


// xGEMV
template<typename T> void
xgemv(char * TRANS, int * M, int * N, T * ALPHA, T * A, int * LDA, T * X, int * INCX, T * BETA, T * Y, int * INCY )
{
  set_xerbla_death_switch(lapack::izero);
  detail::xgemv(TRANS, M, N, ALPHA, A, LDA, X, INCX, BETA, Y, INCY);
}
template void xgemv<real16>(char * TRANS, int * M, int * N, real16 * ALPHA, real16 * A, int * LDA, real16 * X, int * INCX, real16 * BETA, real16 * Y, int * INCY);
template void xgemv<complex16>(char * TRANS, int * M, int * N, complex16 * ALPHA, complex16 * A, int * LDA, complex16 * X, int * INCX, complex16 * BETA, complex16 * Y, int * INCY);

// xGEMM
template<typename T> void
xgemm(char * TRANSA, char * TRANSB, int * M, int * N, int * K, T * ALPHA, T * A, int * LDA, T * B, int * LDB, T * BETA, T * C, int * LDC )
{
  set_xerbla_death_switch(lapack::izero);
  detail::xgemm(TRANSA, TRANSB, M, N, K, ALPHA, A, LDA, B, LDB, BETA, C, LDC );
}
template void xgemm<real16>(char * TRANSA, char * TRANSB, int * M, int * N, int * K, real16 * ALPHA, real16 * A, int * LDA, real16 * B, int * LDB, real16 * BETA, real16 * C, int * LDC );
template void xgemm<complex16>(char * TRANSA, char * TRANSB, int * M, int * N, int * K, complex16 * ALPHA, complex16 * A, int * LDA, complex16 * B, int * LDB, complex16 * BETA, complex16 * C, int * LDC );

// xNORM2
template<typename T> real16
xnrm2(int * N, T * X, int * INCX)
{
  set_xerbla_death_switch(lapack::izero);
  return detail::xnrm2(N, X, INCX);
}
template real16 xnrm2<real16>(int * N, real16 * X, int * INCX);
template real16 xnrm2<complex16>(int * N, complex16 * X, int * INCX);

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

template<typename T> void xgetrf(int * M, int * N, T * A, int * LDA, int * IPIV, int *INFO)
{
  set_xerbla_death_switch(lapack::izero);
  detail::xgetrf(M,N,A,LDA,IPIV,INFO);
  if(*INFO!=0)
  {
    stringstream message;
    if(*INFO<0)
    {
      message << "Input to LAPACK::xgetrf call incorrect at arg: " << *INFO << ".";
    }
    else
    {
      message << "LAPACK::xgetrf, in LU decomposition, matrix U is singular at U[" << (*INFO - 1) << "," << (*INFO - 1) << "].";
    }
    throw rdag_error(message.str());
  }
}
template void xgetrf<real16>(int * M, int * N, real16 * A, int * LDA, int * IPIV, int *INFO);
template void xgetrf<complex16>(int * M, int * N, complex16 * A, int * LDA, int * IPIV, int *INFO);

template<typename T> void xgetri(int * N, T * A, int * LDA, int * IPIV, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  T worktmp;
  int lwork = -1; // -1 to trigger size query

  // Workspace size query
  detail::xgetri(N, A, LDA, IPIV, &worktmp, &lwork, INFO);
  if(*INFO<0)
  {
    stringstream message;
    message << "Input to LAPACK::xgetri call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
  // else { continue }. NOTE: Workspace query doesn't care about validity of inversion,
  // will check on true call below.


  // Allocate work space based on queried value
  lwork = (int)std::real(worktmp);
  T * work = new T[lwork];

  // the actual call
  detail::xgetri(N, A, LDA, IPIV, work, &lwork, INFO);
  if(*INFO!=0)
  {
    stringstream message;
    delete [] work;
    if(*INFO<0)
    {
      message << "Input to LAPACK::xgetri call incorrect at arg: " << *INFO << ".";
    }
    else
    {
      message << "LAPACK::xgetri, in LU decomposition, matrix U is singular at U[" << (*INFO - 1) << "," << (*INFO - 1) << "].";
    }
    throw rdag_error(message.str());
  }


  // normal return
  delete [] work;
}
template void xgetri<real16>(int * N, real16 * A, int * LDA, int * IPIV, int * INFO);
template void xgetri<complex16>(int * N, complex16 * A, int * LDA, int * IPIV, int * INFO);


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

template<typename T> void xtrtrs(char * UPLO, char * TRANS, char * DIAG, int * N, int * NRHS, T * A, int * LDA, T * B, int * LDB, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  detail::xtrtrs(UPLO, TRANS, DIAG, N, NRHS, A, LDA, B, LDB, INFO);
  if(*INFO>0)
  {
    stringstream message;
    message << "LAPACK::"<<detail::charmagic<T>()<<"trtrs matrix is reported as being singular, the " << *INFO << "th diagonal is zero. No solutions have been computed.";
    throw rdag_error(message.str());
  }
  if(*INFO!=0)
  {
    stringstream message;
    message << "Input to LAPACK::"<<detail::charmagic<T>()<<"trtrs call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
}
template void xtrtrs<real16>(char * UPLO, char * TRANS, char * DIAG, int * N, int * NRHS, real16 * A, int * LDA, real16 * B, int * LDB, int * INFO);
template void xtrtrs<complex16>(char * UPLO, char * TRANS, char * DIAG, int * N, int * NRHS, complex16 * A, int * LDA, complex16 * B, int * LDB, int * INFO);


template<typename T> void xpotrf(char * UPLO, int * N, T * A, int * LDA, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  detail::xpotrf(UPLO, N, A, LDA, INFO);
  if(*INFO>0)
  {
    stringstream message;
    message << "LAPACK::"<<detail::charmagic<T>()<<"potrf matrix is reported as being non s.p.d OR the factorisation failed. The " << *INFO << "th leading minor is not positive definite.";
    throw rdag_error(message.str());
  }
  if(*INFO<0)
  {
    stringstream message;
    message << "Input to LAPACK::"<<detail::charmagic<T>()<<"potrf call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
}
template void xpotrf<real16>(char * UPLO, int * N, real16 * A, int * LDA, int * INFO);
template void xpotrf<complex16>(char * UPLO, int * N, complex16 * A, int * LDA, int * INFO);


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

template<typename T>real16 xlansy(char * NORM, char * UPLO, int * N, T * A, int * LDA)
{
  set_xerbla_death_switch(lapack::izero);
  if(*N<0)
  {
    stringstream message;
    message << "Input to LAPACK::"<<detail::charmagic<T>()<<"lansy call incorrect at arg: " << 3;
    throw rdag_error(message.str());
  }
  real16 * WORK = new real16[*N]; // allocate regardless of *NORM
  real16 ret = detail::xlansy(NORM, UPLO, N, A, LDA, WORK);
  delete [] WORK;
  return ret;
}
template real16 xlansy<real16>(char * NORM, char * UPLO, int * N, real16 * A, int * LDA);
template real16  xlansy<complex16>(char * NORM, char * UPLO, int * N, complex16 * A, int * LDA);

real16 zlanhe(char * NORM, char * UPLO, int * N, complex16 * A, int * LDA)
{
  set_xerbla_death_switch(lapack::izero);
  real16 * WORK = new real16[*N]; // allocate regardless of *NORM
  real16 ret = F77FUNC(zlanhe)(NORM, UPLO, N, A, LDA, WORK);
  delete [] WORK;
  return ret;
}


template<typename T> void xpotrs(char * UPLO, int * N, int * NRHS, T * A, int * LDA, T * B, int * LDB, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  detail::xpotrs(UPLO, N, NRHS, A, LDA, B, LDB, INFO);
  if(*INFO<0)
  {
    stringstream message;
    message << "Input to LAPACK::"<<detail::charmagic<T>()<<"potrs call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
}
template void xpotrs<real16>(char * UPLO, int * N, int * NRHS, real16 * A, int * LDA, real16 * B, int * LDB, int * INFO);
template void xpotrs<complex16>(char * UPLO, int * N, int * NRHS, complex16 * A, int * LDA, complex16 * B, int * LDB, int * INFO);


} // end namespace lapack