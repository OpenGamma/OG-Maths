/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _LAPACK_HH
#define _LAPACK_HH

#include <iostream>
#include "lapack_raw.h"
#include "numerictypes.hh"

/**
 * The lapack namespace contains templated variants of functions that vary <real8,complex16> in their signature. In places the <real8,complex16> versions of a function require different
 * parameters effort is made to give a single interface with e.g. workspace handling taken care of.
 */
namespace lapack
{

/**
 * Enumeration of checks available for input data.
 */
enum class OnInputCheck
{
  /**
   * Indicates that the relevant input data should be checked to ensure it is finite
   * as defined by std::isfinite();
   */
  isfinite,
  /**
   * Indicates that the relevant input data should not be checked.
   */
  nothing
};


namespace detail
{
// constants
extern char N;
extern char A;
extern char T;
extern char L;
extern char U;
extern char D;
extern char ONE;
extern char O;
extern char V;
extern int4 ione;
extern int4 izero;
extern real8 rone;
extern complex16 cone;
extern real8 rzero;
extern complex16 czero;

template<typename T>char charmagic();
template<typename T>std::string xxxgqrcharmagic();


template<typename T> void xscal(int4 * N, T * DA, T * DX, int4 * INCX);
template<typename T>void xswap(int4 * N, T * DX, int4 * INCX, T * DY, int4 * INCY);
template<typename T> void xgemv(char * TRANS, int4 * M, int4 * N, T * ALPHA, T * A, int4 * LDA, T * X, int4 * INCX, T * BETA, T * Y, int4 * INCY );
template<typename T> void xgemm(char * TRANSA, char * TRANSB, int4 * M, int4 * N, int4 * K, T * ALPHA, T * A, int4 * LDA, T * B, int4 * LDB, T * BETA, T * C, int4 * LDC );
template<typename T> real8 xnrm2(int4 * N, T * X, int4 * INCX);
template<typename T> void xpotrs(char * UPLO, int4 * N, int4 * NRHS, T * A, int4 * LDA, T * B, int4 * LDB, int4 * INFO);
template<typename T> real8 xlansy(char * NORM, char * UPLO, int4 * N, T * A, int4 * LDA, real8 * WORK);
template<typename T> void xpotrf(char * UPLO, int4 * N, T * A, int4 * LDA, int4 * INFO);
template<typename T> void xtrtrs(char * UPLO, char * TRANS, char * DIAG, int4 * N, int4 * NRHS, T * A, int4 * LDA, T * B, int4 * LDB, int4 * INFO);
template<typename T> void xgetrf(int4 * M, int4 * N, T * A, int4 * LDA, int4 * IPIV, int4 *INFO);
template<typename T> void xgetri(int4 * N, T * A, int4 * LDA, int4 * IPIV, T * WORK, int4 * LWORK, int4 * INFO );
template<typename T> real8 xlange(char * NORM, int4 * M, int4 * N, T * A, int4 * LDA, real8 * WORK );
template<typename T> void xgetrs(char * TRANS, int4 * N, int4 * NRHS, T * A, int4 * LDA, int4 * IPIV, T * B, int4 * LDB, int4 * INFO);
template<typename T> void xgels(char * TRANS, int4 * M, int4 * N, int4 * NRHS, T * A, int4 * LDA, T * B, int4 * LDB, T * WORK, int4 * LWORK, int4 * INFO );
template<typename T> void xgeqrf(int4 * M, int4 * N, T * A, int4 * LDA, T * TAU, T * WORK, int4 * LWORK, int4 *INFO);
template<typename T> void xxxgqr(int4 * M, int4 * N, int4 * K, T * A, int4 * LDA, T * TAU, T * WORK, int4 * LWORK, int4 * INFO);

/**
 * Template for checking data local to lapack calls for NaN/Inf on input.
 * @tparam T data type, real8/complex16 are accepted.
 * @tparam CHECK what to check.
 * @param data the data to check.
 * @param n the count of \a data.
 */
template<typename T, OnInputCheck CHECK> void checkData(T * data, int4 n);

/**
 * Partial template specialisation (PTS) buffer for routines that need input
 * checking and need partial specialisation due to differences in their Fortran prototypes.
 */
template<typename T, OnInputCheck CHECK> struct PTSBuffer
{
  static void xgesvd(char * JOBU, char * JOBVT, int4 * M, int4 * N, T * A, int4 * LDA, real8 * S, T * U, int4 * LDU, T * VT, int4 * LDVT, int4 * INFO);
};

}

/**
 * The F77 character 'N'
 */
extern char * N;
/**
 * The F77 character 'A'
 */
extern char * A;
/**
 * The F77 character 'T'
 */
extern char * T;
/**
 * The F77 character 'L'
 */
extern char * L;
/**
 * The F77 character 'U'
 */
extern char * U;
/**
 * The F77 character 'D'
 */
extern char * D;
/**
 * The F77 character '1'
 */
extern char * ONE;
/**
 * The F77 character 'O'
 */
extern char * O;
/**
 * The F77 character 'V'
 */
extern char * V;
/**
 * The F77 integer '1'
 */
extern int4 * ione;
/**
 * The F77 integer '0'
 */
extern int4 * izero;
/**
 * The F77 double precision '1.d0'
 */
extern real8 * rone;
/**
 * The F77 complex double precision '(1.d0, 0.d0)'
 */
extern complex16 * cone;
/**
 * The F77 double precision '0.d0'
 */
extern real8 * rzero;
/**
 * The F77 complex double precision '(0.d0, 0.d0)'
 */
extern complex16 * czero;


// BLAS

/**
 * xscal generalised vector scaling
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @param N as BLAS dscal N
 * @param DA data type specific with intent as BLAS dscal DA
 * @param DX data type specific with intent as BLAS dscal DX
 * @param INCX as BLAS dscal INCX
 */
template<typename T> void xscal(int4 * N, T * DA, T * DX, int4 * INCX);

/**
 * xswap generalised vector interchange
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @param N as BLAS dswap N
 * @param DX data type specific with intent as BLAS dswap DX
 * @param INCX as BLAS dswap INCX
 * @param DY data type specific with intent as BLAS dswap DY
 * @param INCY as BLAS dswap INCY
 */
template<typename T>void xswap(int4 * N, T * DX, int4 * INCX, T * DY, int4 * INCY);


/**
 * xgemv generalised matrix vector multiplication.
 * @param TRANS as BLAS dgemv TRANS
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @param M as BLAS dgemv M
 * @param N as BLAS dgemv N
 * @param ALPHA data type specific with intent as BLAS dgemv ALPHA
 * @param A data type specific with intent as BLAS dgemv A
 * @param LDA as BLAS dgemv LDA
 * @param X data type specific with intent as BLAS dgemv X
 * @param INCX as BLAS dgemv INCX
 * @param BETA data type specific with intent as BLAS dgemv BETA
 * @param Y data type specific with intent as BLAS dgemv Y
 * @param INCY as BLAS dgemv INCY
 */
template<typename T> void xgemv(char * TRANS, int4 * M, int4 * N, T * ALPHA, T * A, int4 * LDA, T * X, int4 * INCX, T * BETA, T * Y, int4 * INCY );

/**
 * xgemm generalised matrix matrix multiplication.
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @param TRANSA as BLAS dgemm TRANSA
 * @param TRANSB as BLAS dgemm TRANSB
 * @param M as BLAS dgemm M
 * @param N as BLAS dgemm N
 * @param K as BLAS dgemm K
 * @param ALPHA data type specific with intent as BLAS dgemm ALPHA
 * @param A data type specific with intent as BLAS dgemm A
 * @param LDA as BLAS dgemm LDA
 * @param B data type specific with intent as BLAS dgemm B
 * @param LDB as BLAS dgemm LDB
 * @param BETA data type specific with intent as BLAS dgemm BETA
 * @param C data type specific with intent as BLAS dgemm C
 * @param LDC as BLAS dgemm LDC

 */
template<typename T> void xgemm(char * TRANSA, char * TRANSB, int4 * M, int4 * N, int4 * K, T * ALPHA, T * A, int4 * LDA, T * B, int4 * LDB, T * BETA, T * C, int4 * LDC );

/**
 * xnrm2 generalised 2-norm implementation
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @param N as BLAS dnrm2 N
 * @param X data type specific with intent as BLAS dnrm2 X
 * @param INCX as BLAS dnrm2 INCX
 * @return the 2-norm of X
 */
template<typename T> real8 xnrm2(int4 * N, T * X, int4 * INCX);

// LAPACK

/**
 * xgesvd is a generalised svd implementation that takes care of workspaces
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @tparam CHECK what to assert the input complies with on entry to this routine
 * @param JOBU as LAPACK dgesvd JOBU
 * @param JOBVT as LAPACK dgesvd JOBVT
 * @param M as LAPACK dgesvd M
 * @param N as LAPACK dgesvd N
 * @param A data type specific with intent as LAPACK dgesvd A
 * @param LDA as LAPACK dgesvd LDA
 * @param S as LAPACK dgesvd S
 * @param U as LAPACK dgesvd U
 * @param LDU as LAPACK dgesvd LDU
 * @param VT as LAPACK dgesvd VT
 * @param LDVT as LAPACK dgesvd LDVT
 * @param INFO as LAPACK dgesvd INFO
 * @throws rdag_error on illegal input OR non-convergence
 */
template<typename T, OnInputCheck CHECK> void xgesvd(char * JOBU, char * JOBVT, int4 * M, int4 * N, T * A, int4 * LDA, real8 * S, T * U, int4 * LDU, T * VT, int4 * LDVT, int4 * INFO);

/**
 * xgetrf() computes the LU decomposition using parital pivoting
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @tparam CHECK what to assert the input complies with on entry to this routine
 * @param M as LAPACK dgetrf M
 * @param N as LAPACK dgetrf N
 * @param A data type specific with intent as LAPACK dgetrf A
 * @param LDA as LAPACK dgetrf LDA
 * @param IPIV as LAPACK dgetrf IPIV
 * @param INFO as LAPACK dgetrf INFO
 */
template<typename T, OnInputCheck CHECK> void xgetrf(int4 * M, int4 * N, T * A, int4 * LDA, int4 * IPIV, int4 *INFO);

/**
 * xgetri() computes the inverse of a matrix via LU decomposition
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @param N as LAPACK dgetri N
 * @param A  data type specific with intent as LAPACK dgetri A
 * @param LDA as LAPACK dgetri LDA
 * @param IPIV as LAPACK dgetri IPIV
 * @param INFO as LAPACK dgetri INFO
 */
template<typename T> void xgetri(int4 * N, T * A, int4 * LDA, int4 * IPIV, int4 * INFO );

/**
 * xtrcon general triangular matrix condition number estimate
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @param NORM as LAPACK dtrcon NORM
 * @param UPLO as LAPACK dtrcon UPLO
 * @param DIAG as LAPACK dtrcon DIAG
 * @param N as LAPACK dtrcon N
 * @param A data type specific with intent as LAPACK dtrcon A
 * @param LDA as LAPACK dtrcon LDA
 * @param RCOND as LAPACK dtrcon RCOND
 * @param INFO as LAPACK dtrcon INFO
 * @throws rdag_error on illegal input
 */
template<typename T> void xtrcon(char * NORM, char * UPLO, char * DIAG, int4 * N, T * A, int4 * LDA, real8 * RCOND, int4 * INFO);


/**
 * Solves a triangular system of the form : A * X = B  or  A**T * X = B
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @param UPLO as LAPACK dtrtrs UPLO
 * @param TRANS as LAPACK dtrtrs TRANS
 * @param DIAG as LAPACK dtrtrs DIAG
 * @param N as LAPACK dtrtrs N
 * @param NRHS as LAPACK dtrtrs NRHS
 * @param A data type specific with intent as LAPACK dtrtrs A
 * @param LDA as LAPACK dtrtrs LDA
 * @param B data type specific with intent as LAPACK dtrtrs B
 * @param LDB as LAPACK dtrtrs LDB
 * @param INFO as LAPACK dtrtrs INFO
 * @throws rdag_error on illegal input
 */
template<typename T> void xtrtrs(char * UPLO, char * TRANS, char * DIAG, int4 * N, int4 * NRHS, T * A, int4 * LDA, T * B, int4 * LDB, int4 * INFO);

/**
 * xpotrf() computes the Cholesky factorisation of a Hermitian positive definite matrix A.
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @param UPLO as LAPACK dpotrf UPLO
 * @param N as LAPACK dpotrf N
 * @param A data type specific with intent as LAPACK dpotrf A
 * @param LDA as LAPACK dpotrf LDA
 * @param INFO as LAPACK dpotrf INFO
 * @throws rdag_error on illegal input
 */
template<typename T> void  xpotrf(char * UPLO, int4 * N, T * A, int4 * LDA, int4 * INFO);


/**
 * xpocon() computes the reciprocal condition estimate (in the 1-norm) of a s.p.d. matrix using the factorization computed by xpotrf().
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @param UPLO as LAPACK dpocon UPLO
 * @param N as LAPACK dpocon N
 * @param A data type specific with intent as LAPACK dpocon A
 * @param LDA as LAPACK dpocon LDA
 * @param ANORM as LAPACK dpocon ANORM
 * @param RCOND as LAPACK dpocon RCOND
 * @param INFO as LAPACK dpocon INFO
 */
template<typename T> void xpocon(char * UPLO, int4 * N, T * A, int4 * LDA, real8 * ANORM, real8 * RCOND, int4 * INFO);

/**
 * xlansy() computes condition numbers for a symmetric matrix.
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @param NORM as LAPACK dlansy NORM
 * @param UPLO as LAPACK dlansy UPLO
 * @param N as LAPACK dlansy N
 * @param A data type specific with intent as LAPACK dlansy A
 * @param LDA as LAPACK dlansy LDA
 * @return the NORM as indicated by \a NORM.
 */
template<typename T> real8 xlansy(char * NORM, char * UPLO, int4 * N, T * A, int4 * LDA);

/**
 * zlanhe() computes condition numbers for a Hermitian matrix.
 * @param NORM as LAPACK zlanhe NORM
 * @param UPLO as LAPACK zlanhe UPLO
 * @param N as LAPACK zlanhe N
 * @param A a complex Hermitian matrix with intent as given in LAPACK zlanhe A
 * @param LDA as LAPACK zlanhe LDA
 * @return the NORM as indicated by \a NORM.
 */
real8 zlanhe(char * NORM, char * UPLO, int4 * N, complex16 * A, int4 * LDA);

/**
 * xpotrs() solves s.p.d. systems via Cholesky decomposition as computed by xpotrf().
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @param UPLO as LAPACK dpotrs UPLO.
 * @param N as LAPACK dpotrs N.
 * @param NRHS as LAPACK dpotrs NRHS.
 * @param A data type specific with intent as LAPACK dpotrs A.
 * @param LDA as LAPACK dpotrs LDA.
 * @param B data type specific with intent as LAPACK dpotrs B.
 * @param LDB as LAPACK dpotrs LDB.
 * @param INFO as LAPACK dpotrs INFO.
 */
template<typename T> void xpotrs(char * UPLO, int4 * N, int4 * NRHS, T * A, int4 * LDA, T * B, int4 * LDB, int4 * INFO);

/**
 * xlange() computes matrix norms (one, Inf, Frobenius)
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @param NORM as LAPACK dlange NORM.
 * @param M as LAPACK dlange M.
 * @param N as LAPACK dlange N.
 * @param A data type specific with intent as LAPACK dlange A.
 * @param LDA as LAPACK dlange LDA.
 */
template<typename T> real8 xlange(char * NORM, int4 * M, int4 * N, T * A, int4 * LDA);

/**
 * xgecon() computes reciprocal condition numbers calculated from a LU factorisation as
 * computed by xgetrf().
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @param NORM as LAPACK dgecon NORM.
 * @param N as LAPACK dgecon N.
 * @param A data type specific with intent as LAPACK dgecon A.
 * @param LDA as LAPACK dgecon LDA.
 * @param ANORM as LAPACK dgecon ANORM.
 * @param RCOND as LAPACK dgecon RCOND.
 * @param INFO as LAPACK dgecon INFO.
 */
template<typename T> void xgecon(char * NORM, int4 * N, T * A, int4 * LDA, real8 * ANORM, real8 * RCOND, int4 * INFO);

/**
 * xgetrs() solves linear systems using a LU factorisation computed by xgetrf().
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @param TRANS as LAPACK xgetrs TRANS.
 * @param N as LAPACK xgetrs N.
 * @param NRHS as LAPACK xgetrs NRHS.
 * @param A data type specific with intent as LAPACK xgetrs A.
 * @param LDA as LAPACK xgetrs LDA.
 * @param IPIV as LAPACK xgetrs IPIV.
 * @param B data type specific with intent as LAPACK xgetrs B.
 * @param LDB as LAPACK xgetrs LDB.
 * @param INFO as LAPACK xgetrs INFO.
 */
template<typename T>void xgetrs(char * TRANS, int4 * N, int4 * NRHS, T * A, int4 * LDA, int4 * IPIV, T * B, int4 * LDB, int4 * INFO);

/**
 * xgels() solves {over,under}determined linear systems via QR decomposition.
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @param TRANS as LAPACK dgels TRANS.
 * @param M as LAPACK dgels M.
 * @param N as LAPACK dgels N.
 * @param NRHS as LAPACK dgels NRHS.
 * @param A data type specific with intent as LAPACK dgels A.
 * @param LDA as LAPACK dgels LDA.
 * @param B data type specific with intent as  LAPACK dgels TRANS.
 * @param LDB as LAPACK dgels LDB.
 * @param INFO as LAPACK dgels INFO.
 */
template<typename T> void xgels(char * TRANS, int4 * M, int4 * N, int4 * NRHS, T * A, int4 * LDA, T * B, int4 * LDB, int4 * INFO );

/**
 * xgelsd() solves {over,under}determined linear systems via SV decomposition.
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @param M as LAPACK dgelsd M.
 * @param N as LAPACK dgelsd N.
 * @param NRHS as LAPACK dgelsd NRHS.
 * @param A data type specific with intent as LAPACK dgelsd A.
 * @param LDA as LAPACK dgelsd LDA.
 * @param B data type specific with intent as LAPACK dgelsd TRANS.
 * @param LDB as LAPACK dgelsd LDB.
 * @param S as LAPACK dgelsd S.
 * @param RCOND as LAPACK dgelsd RCOND.
 * @param RANK as LAPACK dgelsd RANK.
 * @param INFO as LAPACK dgels INFO.
 */
template<typename T> void xgelsd(int4 * M, int4 * N, int4 * NRHS, T * A, int4 * LDA, T * B, int4 * LDB, real8 * S, real8 * RCOND, int4 * RANK, int4 * INFO );

/**
 * xgeev() computes eigen{values,vectors}.
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @param JOBVL as LAPACK dgeev JOBVL.
 * @param JOBVR as LAPACK dgeev JOBVR.
 * @param N as LAPACK dgeev N.
 * @param A data type specific with intent as LAPACK dgeev A.
 * @param LDA as LAPACK dgeev LDA.
 * @param W the computed eigenvalues.
 * @param VL data type specific with intent as LAPACK dgeev VL.
 * @param LDVL as LAPACK dgeev LDVL.
 * @param VR data type specific with intent as LAPACK dgeev VR.
 * @param LDVR as LAPACK dgeev LDVR.
 * @param LDVL as LAPACK dgeev INFO.
 */
template<typename T> void xgeev(char * JOBVL, char * JOBVR, int4 * N, T * A, int4 * LDA, complex16 * W, T * VL, int4 * LDVL, T * VR, int4 * LDVR, int4 * INFO);


/**
 * xgeqrf() computes the QR decomposition.
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @tparam CHECK what to assert the input complies with on entry to this routine
 * @param M as LAPACK degqrf M.
 * @param N as LAPACK degqrf N.
 * @param A data type specific with intent as LAPACK degqrf A.
 * @param LDA as LAPACK dgeqrf LDA.
 * @param TAU  data type specific with intent as LAPACK degqrf TAU.
 * @param INFO as LAPACK degqrf INFO.
 */
template<typename T, OnInputCheck CHECK> void xgeqrf(int4 * M, int4 * N, T * A, int4 * LDA, T * TAU, int4 *INFO);


/**
 * xxxgqr() computes the orthogonal Q matrix from elementary reflectors as returned by xgeqrf()
 * @tparam T the type of the underlying data real8 and complex16 are accepted
 * @tparam CHECK what to assert the input complies with on entry to this routine
 * @param M as LAPACK dorgqr M.
 * @param N as LAPACK dorgqr N.
 * @param K as LAPACK dorgqr K.
 * @param A data type specific with intent as LAPACK degqrf A.
 * @param LDA as LAPACK dgeqrf LDA.
 * @param TAU  data type specific with intent as LAPACK degqrf TAU.
 * @param INFO as LAPACK degqrf INFO.
 */
template<typename T, OnInputCheck CHECK> void xxxgqr(int4 * M, int4 * N, int4 * K, T * A, int4 * LDA, T * TAU, int4 * INFO);
}



#endif // _LAPACK_HH
