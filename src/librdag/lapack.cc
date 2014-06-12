/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "lapack.hh"
#include "exceptions.hh"

#include <memory>

using namespace librdag;
using namespace std;

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
  real8 rone = 1.e0;
  complex16 cone = {1.e0, 0.e0};
  real8 rzero = 0.e0;
  complex16 czero = {0.e0, 0.e0};

  // charmagic specialisation
  template<>char charmagic<real8>()
  {
    return 'd';
  }
  template<>char charmagic<complex16>()
  {
    return 'z';
  }

  // xSCAL specialisations
  template<> void xscal(int * N, real8 * DA, real8 * DX, int * INCX)
  {
    F77FUNC(dscal)(N, DA, DX, INCX);
  }

  template<> void xscal(int * N, complex16 * DA, complex16 * DX, int * INCX)
  {
    F77FUNC(zscal)(N, DA, DX, INCX);
  }

  // xSWAP specialisations
  template<> void xswap(int * N, real8 * DX, int * INCX, real8 * DY, int * INCY)
  {
    F77FUNC(dswap)(N, DX, INCX, DY, INCY);
  }

  template<> void xswap(int * N, complex16 * DX, int * INCX, complex16 * DY, int * INCY)
  {
    F77FUNC(zswap)(N, DX, INCX, DY, INCY);
  }

  // xGEMV specialisations
  template<> void xgemv(char * TRANS, int * M, int * N, real8 * ALPHA, real8 * A, int * LDA, real8 * X, int * INCX, real8 * BETA, real8 * Y, int * INCY )
  {
    F77FUNC(dgemv)(TRANS, M, N, ALPHA, A, LDA, X, INCX, BETA, Y, INCY);
  }

  template<> void xgemv(char * TRANS, int * M, int * N, complex16 * ALPHA, complex16 * A, int * LDA, complex16 * X, int * INCX, complex16 * BETA, complex16 * Y, int * INCY )
  {
    F77FUNC(zgemv)(TRANS, M, N, ALPHA, A, LDA, X, INCX, BETA, Y, INCY);
  }

  // xGEMM specialisations
  template<> void
  xgemm(char * TRANSA, char * TRANSB, int * M, int * N, int * K, real8 * ALPHA, real8 * A, int * LDA, real8 * B, int * LDB, real8 * BETA, real8 * C, int * LDC )
  {
    F77FUNC(dgemm)(TRANSA, TRANSB, M, N, K, ALPHA, A, LDA, B, LDB, BETA, C, LDC );
  }

  template<> void
  xgemm(char * TRANSA, char * TRANSB, int * M, int * N, int * K, complex16 * ALPHA, complex16 * A, int * LDA, complex16 * B, int * LDB, complex16 * BETA, complex16 * C, int * LDC )
  {
    F77FUNC(zgemm)(TRANSA, TRANSB, M, N, K, ALPHA, A, LDA, B, LDB, BETA, C, LDC );
  }

  // xNORM2 specialisations
  template<> real8 xnrm2(int * N, real8 * X, int * INCX)
  {
    return F77FUNC(dnrm2)(N, X, INCX);
  }

  template<> real8 xnrm2(int * N, complex16 * X, int * INCX)
  {
    return F77FUNC(dznrm2)(N, X, INCX);
  }

  // xPOTRS specialisations
  template<> void xpotrs(char * UPLO, int * N, int * NRHS, real8 * A, int * LDA, real8 * B, int * LDB, int * INFO)
  {
    F77FUNC(dpotrs)(UPLO, N, NRHS, A, LDA, B, LDB, INFO);
  }

  template<> void xpotrs(char * UPLO, int * N, int * NRHS, complex16 * A, int * LDA, complex16 * B, int * LDB, int * INFO)
  {
    F77FUNC(zpotrs)(UPLO, N, NRHS, A, LDA, B, LDB, INFO);
  }

  // xLANSY specialisations
  template<>real8 xlansy(char * NORM, char * UPLO, int * N, real8 * A, int * LDA, real8 * WORK)
  {
    return F77FUNC(dlansy)(NORM, UPLO, N, A, LDA, WORK);
  }
  template<>real8 xlansy(char * NORM, char * UPLO, int * N, complex16 * A, int * LDA, real8 * WORK)
  {
    return  F77FUNC(zlansy)(NORM, UPLO, N, A, LDA, WORK);
  }

  // xPOTRF specialisations
  template<> void xpotrf(char * UPLO, int * N, real8 * A, int * LDA, int * INFO)
  {
    F77FUNC(dpotrf)(UPLO, N, A, LDA, INFO);
  }
  template<> void xpotrf(char * UPLO, int * N, complex16 * A, int * LDA, int * INFO)
  {
    F77FUNC(zpotrf)(UPLO, N, A, LDA, INFO);
  }

  //xTRTRS specialisations
  template<> void xtrtrs(char * UPLO, char * TRANS, char * DIAG, int * N, int * NRHS, real8 * A, int * LDA, real8 * B, int * LDB, int * INFO)
  {
    F77FUNC(dtrtrs)(UPLO, TRANS, DIAG, N, NRHS, A, LDA, B, LDB, INFO);
  }
  template<> void xtrtrs(char * UPLO, char * TRANS, char * DIAG, int * N, int * NRHS, complex16 * A, int * LDA, complex16 * B, int * LDB, int * INFO)
  {
    F77FUNC(ztrtrs)(UPLO, TRANS, DIAG, N, NRHS, A, LDA, B, LDB, INFO);
  }

  //xGETRF specialisations
  template<> void xgetrf(int * M, int * N, real8 * A, int * LDA, int * IPIV, int *INFO)
  {
    F77FUNC(dgetrf)(M,N,A,LDA,IPIV,INFO);
  }

  template<> void xgetrf(int * M, int * N, complex16 * A, int * LDA, int * IPIV, int *INFO)
  {
    F77FUNC(zgetrf)(M,N,A,LDA,IPIV,INFO);
  }

  //xGETRI specialisations
  template<> void xgetri(int * N, real8 * A, int * LDA, int * IPIV, real8 * WORK, int * LWORK, int * INFO )
  {
    F77FUNC(dgetri)(N,A,LDA,IPIV,WORK,LWORK,INFO);
  }

  template<> void xgetri(int * N, complex16 * A, int * LDA, int * IPIV, complex16 * WORK, int * LWORK, int * INFO )
  {
    F77FUNC(zgetri)(N,A,LDA,IPIV,WORK,LWORK,INFO);
  }

  //xLANGE specialisation
  template<> real16 xlange(char * NORM, int * M, int * N, real16 * A, int * LDA, real16 * WORK )
  {
    return F77FUNC(dlange)(NORM, M, N, A, LDA, WORK);
  }

  template<> real16 xlange(char * NORM, int * M, int * N, complex16 * A, int * LDA, real16 * WORK )
  {
    return F77FUNC(zlange)(NORM, M, N, A, LDA, WORK);
  }

  // xGETRS specialisation
  template<> void xgetrs(char * TRANS, int * N, int * NRHS, real16 * A, int * LDA, int * IPIV, real16 * B, int * LDB, int * INFO)
  {
    F77FUNC(dgetrs)(TRANS, N, NRHS, A, LDA, IPIV, B, LDB, INFO);
  }
  template<> void xgetrs(char * TRANS, int * N, int * NRHS, complex16 * A, int * LDA, int * IPIV, complex16 * B, int * LDB, int * INFO)
  {
    F77FUNC(zgetrs)(TRANS, N, NRHS, A, LDA, IPIV, B, LDB, INFO);
  }

  //xGELS specialisation
  template<> void xgels(char * TRANS, int * M, int * N, int * NRHS, real16 * A, int * LDA, real16 * B, int * LDB, real16 * WORK, int * LWORK, int * INFO )
  {
    F77FUNC(dgels)(TRANS, M, N, NRHS, A, LDA, B, LDB, WORK, LWORK, INFO );
  }
  template<> void xgels(char * TRANS, int * M, int * N, int * NRHS, complex16 * A, int * LDA, complex16 * B, int * LDB, complex16 * WORK, int * LWORK, int * INFO )
  {
    F77FUNC(zgels)(TRANS, M, N, NRHS, A, LDA, B, LDB, WORK, LWORK, INFO );
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
real8 *    rone  = &detail::rone;
complex16 * cone  = &detail::cone;
real8 *    rzero = &detail::rzero;
complex16 * czero = &detail::czero;

// xSCAL
template<typename T> void xscal(int * N, T * DA, T * DX, int * INCX)
{
  set_xerbla_death_switch(lapack::izero);
  detail::xscal(N, DA, DX, INCX);
}
template void xscal<real8>(int * N, real8 * DA, real8 * DX, int * INCX);
template void xscal<complex16>(int * N, complex16 * DA, complex16 * DX, int * INCX);

// xSWAP
template<typename T> void xswap(int * N, T * DX, int * INCX, T * DY, int * INCY)
{
  set_xerbla_death_switch(lapack::izero);
  detail::xswap(N, DX, INCX, DY, INCY);
}
template void xswap<real8>(int * N, real8 * DX, int * INCX, real8 * DY, int * INCY);
template void xswap<complex16>(int * N, complex16 * DX, int * INCX, complex16 * DY, int * INCY);


// xGEMV
template<typename T> void
xgemv(char * TRANS, int * M, int * N, T * ALPHA, T * A, int * LDA, T * X, int * INCX, T * BETA, T * Y, int * INCY )
{
  set_xerbla_death_switch(lapack::izero);
  detail::xgemv(TRANS, M, N, ALPHA, A, LDA, X, INCX, BETA, Y, INCY);
}
template void xgemv<real8>(char * TRANS, int * M, int * N, real8 * ALPHA, real8 * A, int * LDA, real8 * X, int * INCX, real8 * BETA, real8 * Y, int * INCY);
template void xgemv<complex16>(char * TRANS, int * M, int * N, complex16 * ALPHA, complex16 * A, int * LDA, complex16 * X, int * INCX, complex16 * BETA, complex16 * Y, int * INCY);

// xGEMM
template<typename T> void
xgemm(char * TRANSA, char * TRANSB, int * M, int * N, int * K, T * ALPHA, T * A, int * LDA, T * B, int * LDB, T * BETA, T * C, int * LDC )
{
  set_xerbla_death_switch(lapack::izero);
  detail::xgemm(TRANSA, TRANSB, M, N, K, ALPHA, A, LDA, B, LDB, BETA, C, LDC );
}
template void xgemm<real8>(char * TRANSA, char * TRANSB, int * M, int * N, int * K, real8 * ALPHA, real8 * A, int * LDA, real8 * B, int * LDB, real8 * BETA, real8 * C, int * LDC );
template void xgemm<complex16>(char * TRANSA, char * TRANSB, int * M, int * N, int * K, complex16 * ALPHA, complex16 * A, int * LDA, complex16 * B, int * LDB, complex16 * BETA, complex16 * C, int * LDC );

// xNORM2
template<typename T> real8
xnrm2(int * N, T * X, int * INCX)
{
  set_xerbla_death_switch(lapack::izero);
  return detail::xnrm2(N, X, INCX);
}
template real8 xnrm2<real8>(int * N, real8 * X, int * INCX);
template real8 xnrm2<complex16>(int * N, complex16 * X, int * INCX);

// xGESVD specialisations
template<>  void xgesvd(char * JOBU, char * JOBVT, int * M, int * N, real8 * A, int * LDA, real8 * S, real8 * U, int * LDU, real8 * VT, int * LDVT, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  real16 tmp;
  int lwork = -1; // set for query

  // work space query
  F77FUNC(dgesvd)(JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT, LDVT, &tmp, &lwork, INFO);

  if(*INFO < 0)
  {
    stringstream message;
    message << "Input to LAPACK::dgesvd call incorrect at arg: " << *INFO;
    message << std::endl << "LDVT is " << *LDVT;
    cout << message.str();
    throw rdag_error(message.str());
  }

  // query complete tmp contains size needed
  lwork = (int)tmp;
  unique_ptr<real16[]> workPtr(new real16[lwork]());
  real16 * WORK = workPtr.get();

  // full execution
  F77FUNC(dgesvd)(JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT, LDVT, WORK, &lwork, INFO);

  if(*INFO!=0)
  {
    throw rdag_error("LAPACK::dgesvd, internal call to dbdsqr did not converge.");
  }
}

template<>  void xgesvd(char * JOBU, char * JOBVT, int * M, int * N, complex16 * A, int * LDA, real8 * S, complex16 * U, int * LDU, complex16 * VT, int * LDVT, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  int minmn = *M > *N ? *N : *M; // compute scale for RWORK
  complex16 tmp;
  int lwork = -1; // set for query
  real16 * RWORK = nullptr;

  // work space query
  F77FUNC(zgesvd)(JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT, LDVT, &tmp, &lwork, RWORK, INFO);
  if(*INFO < 0)
  {
    stringstream message;
    message << "Input to LAPACK::zgesvd call incorrect at arg: " << *INFO;
    message << std::endl << "LDVT is " << *LDVT;
    cout << message.str();
    throw rdag_error(message.str());
  }

  // query complete tmp contains size needed
  lwork = (int)(tmp.real());

  unique_ptr<complex16[]> workPtr(new complex16[lwork]);
  complex16 * WORK = workPtr.get();
  unique_ptr<real16[]> rworkPtr(new real16[5*minmn]);
  RWORK = rworkPtr.get();


  // full execution
  F77FUNC(zgesvd)(JOBU, JOBVT, M, N, A, LDA, S, U, LDU, VT, LDVT, WORK, &lwork, RWORK, INFO);

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
template void xgetrf<real8>(int * M, int * N, real8 * A, int * LDA, int * IPIV, int *INFO);
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
  unique_ptr<T[]> workPtr (new T[lwork]);
  T * work = workPtr.get();

  // the actual call
  detail::xgetri(N, A, LDA, IPIV, work, &lwork, INFO);
  if(*INFO!=0)
  {
    stringstream message;
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

}
template void xgetri<real8>(int * N, real8 * A, int * LDA, int * IPIV, int * INFO);
template void xgetri<complex16>(int * N, complex16 * A, int * LDA, int * IPIV, int * INFO);


template<> void xtrcon(char * NORM, char * UPLO, char * DIAG, int * N, real8 * A, int * LDA, real8 * RCOND, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  if(*N<=0)
  {
    stringstream message;
    message << "Input to LAPACK::dtrcon call incorrect at arg: " << 4;
    throw rdag_error(message.str());
  }

  unique_ptr<real16[]> workPtr (new real16[ 3 * *N]);
  real16 * WORK = workPtr.get();
  unique_ptr<int[]> iworkPtr (new int[*N]);
  int * IWORK = iworkPtr.get();

  F77FUNC(dtrcon)(NORM, UPLO, DIAG, N, A, LDA, RCOND, WORK, IWORK, INFO);
  if(*INFO!=0)
  {
      stringstream message;
      message << "Input to LAPACK::dtrcon call incorrect at arg: " << *INFO;
      throw rdag_error(message.str());
  }
}


template<> void xtrcon(char * NORM, char * UPLO, char * DIAG, int * N, complex16 * A, int * LDA, real8 * RCOND, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  if(*N<0)
  {
    stringstream message;
    message << "Input to LAPACK::ztrcon call incorrect at arg: " << 4;
    throw rdag_error(message.str());
  }

  unique_ptr<complex16[]> workPtr (new complex16[ 2 * *N]);
  complex16 * WORK = workPtr.get();
  unique_ptr<real16[]> rworkPtr (new real16[*N]);
  real16 * RWORK = rworkPtr.get();

  F77FUNC(ztrcon)(NORM, UPLO, DIAG, N, A, LDA, RCOND, WORK, RWORK, INFO);
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
template void xtrtrs<real8>(char * UPLO, char * TRANS, char * DIAG, int * N, int * NRHS, real8 * A, int * LDA, real8 * B, int * LDB, int * INFO);
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
template void xpotrf<real8>(char * UPLO, int * N, real8 * A, int * LDA, int * INFO);
template void xpotrf<complex16>(char * UPLO, int * N, complex16 * A, int * LDA, int * INFO);


template<> void xpocon(char * UPLO, int * N, real8 * A, int * LDA, real8 * ANORM, real8 * RCOND, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  if(*N<=0)
  {
    stringstream message;
    message << "Input to LAPACK::dpocon call incorrect at arg: " << 2;
    throw rdag_error(message.str());
  }
  unique_ptr<real16[]> workPtr (new real16[3 * (*N)]);
  real16 * WORK = workPtr.get();
  unique_ptr<int[]> iworkPtr (new int[(*N)]);
  int * IWORK = iworkPtr.get();

  F77FUNC(dpocon)(UPLO, N, A, LDA, ANORM, RCOND, WORK, IWORK, INFO);
  if(*INFO<0)
  {
    stringstream message;
    message << "Input to LAPACK::dpocon call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
}

template<> void xpocon(char * UPLO, int * N, complex16 * A, int * LDA, real8 * ANORM, real8 * RCOND, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  if(*N<=0)
  {
    stringstream message;
    message << "Input to LAPACK::zpocon call incorrect at arg: " << 2;
    throw rdag_error(message.str());
  }

  unique_ptr<complex16[]> workPtr (new complex16[2 * *N]);
  complex16 * WORK = workPtr.get();
  unique_ptr<real16[]> rworkPtr (new real16[*N]);
  real16 * RWORK = rworkPtr.get();

  F77FUNC(zpocon)(UPLO, N, A, LDA, ANORM, RCOND, WORK, RWORK, INFO);
  if(*INFO<0)
  {
    stringstream message;
    message << "Input to LAPACK::dpocon call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
}

template<typename T>real8 xlansy(char * NORM, char * UPLO, int * N, T * A, int * LDA)
{
  set_xerbla_death_switch(lapack::izero);
  if(*N<=0)
  {
    stringstream message;
    message << "Input to LAPACK::"<<detail::charmagic<T>()<<"lansy call incorrect at arg: " << 3;
    throw rdag_error(message.str());
  }

  unique_ptr<real16[]> workPtr (new real16[*N]);// allocate regardless of *NORM
  real16 * WORK = workPtr.get();
  real16 ret = detail::xlansy(NORM, UPLO, N, A, LDA, WORK);

  return ret;
}
template real8 xlansy<real8>(char * NORM, char * UPLO, int * N, real8 * A, int * LDA);
template real8  xlansy<complex16>(char * NORM, char * UPLO, int * N, complex16 * A, int * LDA);

real8 zlanhe(char * NORM, char * UPLO, int * N, complex16 * A, int * LDA)
{
  set_xerbla_death_switch(lapack::izero);

  unique_ptr<real16[]> workPtr (new real16[*N]);// allocate regardless of *NORM
  real16 * WORK = workPtr.get();
  real16 ret = F77FUNC(zlanhe)(NORM, UPLO, N, A, LDA, WORK);

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
template void xpotrs<real8>(char * UPLO, int * N, int * NRHS, real8 * A, int * LDA, real8 * B, int * LDB, int * INFO);
template void xpotrs<complex16>(char * UPLO, int * N, int * NRHS, complex16 * A, int * LDA, complex16 * B, int * LDB, int * INFO);


template<typename T> real16 xlange(char * NORM, int * M, int * N, T * A, int * LDA)
{
  set_xerbla_death_switch(lapack::izero);
  if(*M<=0)
  {
    stringstream message;
    message << "Input to LAPACK::"<<detail::charmagic<T>()<<"lange call incorrect at arg: " << 2;
    throw rdag_error(message.str());
  }
  unique_ptr<real16[]> workPtr (new real16[*M]);// allocate regardless of *NORM
  real16 * WORK = workPtr.get();
  real16 ret = detail::xlange(NORM, M, N, A, LDA, WORK);
  return ret;
}
template real16 xlange<real16>(char * NORM, int * M, int * N, real16 * A, int * LDA);
template real16 xlange<complex16>(char * NORM, int * M, int * N, complex16 * A, int * LDA);


template<> void xgecon(char * NORM, int * N, real16 * A, int * LDA, real16 * ANORM, real16 * RCOND, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  if(*N<=0)
  {
    stringstream message;
    message << "Input to LAPACK::dgecon call incorrect at arg: " << 2;
    throw rdag_error(message.str());
  }

  unique_ptr<real16[]> workPtr (new real16[4*(*N)]);
  real16 * WORK = workPtr.get();
  unique_ptr<int[]> iworkPtr (new int[*N]);
  int * IWORK = iworkPtr.get();
  F77FUNC(dgecon)(NORM, N, A, LDA, ANORM, RCOND, WORK, IWORK, INFO);

  if(*INFO<0)
  {
    stringstream message;
    message << "Input to LAPACK::dgecon call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
}

template<> void xgecon(char * NORM, int * N, complex16 * A, int * LDA, real16 * ANORM, real16 * RCOND, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  if(*N<=0)
  {
    stringstream message;
    message << "Input to LAPACK::zgecon call incorrect at arg: " << 2;
    throw rdag_error(message.str());
  }

  unique_ptr<complex16[]> workPtr (new complex16[2*(*N)]);
  complex16 * WORK = workPtr.get();
  unique_ptr<real16[]> rworkPtr (new real16[2*(*N)]);
  real16 * RWORK = rworkPtr.get();

  F77FUNC(zgecon)(NORM, N, A, LDA, ANORM, RCOND, WORK, RWORK, INFO);

  if(*INFO<0)
  {
    stringstream message;
    message << "Input to LAPACK::zgecon call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
}


template<typename T> void xgetrs(char * TRANS, int * N, int * NRHS, T * A, int * LDA, int * IPIV, T * B, int * LDB, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  detail::xgetrs(TRANS, N, NRHS, A, LDA, IPIV, B, LDB, INFO);
  if(*INFO<0)
  {
    stringstream message;
    message << "Input to LAPACK::"<<detail::charmagic<T>()<<"getrs call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
}
template void xgetrs<real16>(char * TRANS, int * N, int * NRHS, real16 * A, int * LDA, int * IPIV, real16 * B, int * LDB, int * INFO);
template void xgetrs<complex16>(char * TRANS, int * N, int * NRHS, complex16 * A, int * LDA, int * IPIV, complex16 * B, int * LDB, int * INFO);


template<typename T> void xgels(char * TRANS, int * M, int * N, int * NRHS, T * A, int * LDA, T * B, int * LDB, int * INFO )
{
  set_xerbla_death_switch(lapack::izero);
  T worktmp;
  int lwork = -1; // -1 to trigger size query

  // Workspace size query
  detail::xgels(TRANS, M, N, NRHS, A, LDA, B, LDB, &worktmp, &lwork, INFO);
  if(*INFO<0)
  {
    stringstream message;
    message << "Input to LAPACK::xgels call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
  // else { continue }. NOTE: Workspace query doesn't care about validity of solution,
  // will check on true call below.

  // Allocate work space based on queried value
  lwork = (int)std::real(worktmp);
  unique_ptr<T[]> workPtr (new T[lwork]);
  T * work = workPtr.get();

  // the actual call
  detail::xgels(TRANS, M, N, NRHS, A, LDA, B, LDB, work, &lwork, INFO);
  if(*INFO!=0)
  {
    stringstream message;
    if(*INFO<0)
    {
      message << "Input to LAPACK::xgels call incorrect at arg: " << *INFO << ".";
    }
    else
    {
      message << "LAPACK::xgels, the [" << (*INFO - 1) << " element of the triangular " <<
      "factorisation of A is zero, A does not have full rank, the least squares solution " <<
      "could not be computed";
    }
    throw rdag_error(message.str());
  }
  // normal return
}
template void xgels<real16>(char * TRANS, int * M, int * N, int * NRHS, real16 * A, int * LDA, real16 * B, int * LDB, int * INFO );
template void xgels<complex16>(char * TRANS, int * M, int * N, int * NRHS, complex16 * A, int * LDA, complex16 * B, int * LDB, int * INFO );


template<> void xgelsd(int * M, int * N, int * NRHS, real16 * A, int * LDA, real16 * B, int * LDB, real16 * S, real16 * RCOND, int * RANK, int * INFO )
{
  set_xerbla_death_switch(lapack::izero);
  real16 worktmp;
  int iworktmp;
  int lwork = -1; // -1 to trigger size query

  // Workspace size query
  F77FUNC(dgelsd)(M, N, NRHS, A, LDA, B, LDB, S, RCOND, RANK, &worktmp, &lwork, &iworktmp, INFO);
  if(*INFO<0)
  {
    stringstream message;
    message << "Input to LAPACK::dgelsd call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
  // else { continue }. NOTE: Workspace query doesn't care about validity of solution,
  // will check on true call below.

  // Allocate work space based on queried value
  lwork = (int)(worktmp);

  unique_ptr<real16[]> workPtr (new real16[lwork]);
  real16 * work = workPtr.get();

  unique_ptr<int[]> iworkPtr (new int[iworktmp]);
  int * iwork = iworkPtr.get();

  // the actual call
  F77FUNC(dgelsd)(M, N, NRHS, A, LDA, B, LDB, S, RCOND, RANK, work, &lwork, iwork, INFO);

  if(*INFO!=0)
  {
    stringstream message;
    if(*INFO<0)
    {
      message << "Input to LAPACK::dgelsd call incorrect at arg: " << *INFO << ".";
    }
    else
    {
      message << "LAPACK::dgelsd, SVD computation failed to converge, " << (*INFO) <<
      " elements of the intermediate bi-diagonal form did not converge to zero.";
    }
    throw rdag_error(message.str());
  }

}


template<> void xgelsd(int * M, int * N, int * NRHS, complex16 * A, int * LDA, complex16 * B, int * LDB, real16 * S, real16 * RCOND, int * RANK, int * INFO )
{
  set_xerbla_death_switch(lapack::izero);
  complex16 worktmp;
  real16 rworktmp;
  int iworktmp;
  int lwork = -1; // -1 to trigger size query

  // Workspace size query
  F77FUNC(zgelsd)(M, N, NRHS, A, LDA, B, LDB, S, RCOND, RANK, &worktmp, &lwork, &rworktmp, &iworktmp, INFO);
  if(*INFO<0)
  {
    stringstream message;
    message << "Input to LAPACK::zgelsd call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }
  // else { continue }. NOTE: Workspace query doesn't care about validity of solution,
  // will check on true call below.

  // Allocate work space based on queried value
  lwork = (int)std::real(worktmp);

  unique_ptr<complex16[]> workPtr (new complex16[lwork]);
  unique_ptr<real16[]> rworkPtr (new real16[(int)(rworktmp)]);
  unique_ptr<int[]> iworkPtr (new int[iworktmp]);

  complex16 * work = workPtr.get();
  real16 * rwork = rworkPtr.get();
  int * iwork = iworkPtr.get();

  // the actual call
  F77FUNC(zgelsd)(M, N, NRHS, A, LDA, B, LDB, S, RCOND, RANK, work, &lwork, rwork, iwork, INFO);

  if(*INFO!=0)
  {
    stringstream message;
    if(*INFO<0)
    {
      message << "Input to LAPACK::zgelsd call incorrect at arg: " << *INFO << ".";
    }
    else
    {
      message << "LAPACK::zgelsd, SVD computation failed to converge, " << (*INFO) <<
      " elements of the intermediate bi-diagonal form did not converge to zero.";
    }
    throw rdag_error(message.str());
  }

}


template<> void xgeev(char * JOBVL, char * JOBVR, int * N, real16 * A, int * LDA, complex16 * W, real16 * VL, int * LDVL, real16 * VR, int * LDVR, int * INFO)
{
  set_xerbla_death_switch(lapack::izero);
  real16 worktmp;
  int lwork = -1; // -1 to trigger size query

  if(*N<=0)
  {
    stringstream message;
    message << "Input to LAPACK::dgeev call incorrect at arg: " << 3;
    throw rdag_error(message.str());
  }

  real16 * WR = nullptr;
  real16 * WI = nullptr;

  // Workspace size query
  F77FUNC(dgeev)(JOBVL, JOBVR, N, A, LDA, WR, WI, VL, LDVL, VR, LDVR, &worktmp, &lwork, INFO);
  if(*INFO<0)
  {
    stringstream message;
    message << "Input to LAPACK::dgeev call incorrect at arg: " << *INFO;
    throw rdag_error(message.str());
  }

  std::unique_ptr<real16 []> ptrWR (new real16[*N]);
  WR = (ptrWR).get();
  std::unique_ptr<real16 []> ptrWI (new real16[*N]);
  WI = (ptrWI).get();

    // Allocate work space based on queried value
  lwork = (int)(worktmp);
  std::unique_ptr<real16 []> ptrWORK (new real16[lwork]);
  real16 * work = ptrWORK.get();

  // the actual call
  F77FUNC(dgeev)(JOBVL, JOBVR, N, A, LDA, WR, WI, VL, LDVL, VR, LDVR, work, &lwork, INFO);

  // copy back answer
  for(int i = 0 ; i < *N; i++)
  {
    W[i] = std::complex<real16>(WR[i],WI[i]);
  }

  if(*INFO!=0)
  {
    stringstream message;
    if(*INFO<0)
    {
      message << "Input to LAPACK::dgeev call incorrect at arg: " << *INFO << ".";
    }
    else
    {
      message << "LAPACK::dgeev, QR alg failed. Eigenvalues in elements, " << (*INFO - 1) <<
      ":" << (*N-1) << " have successfully converged, no eigenvectors have been computed.";
    }
    throw rdag_error(message.str());
  }
}



} // end namespace lapack