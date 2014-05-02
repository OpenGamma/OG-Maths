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
 * The lapack namespace contains templated variants of functions that vary <real16,complex16> in their signature. In places the <real16,complex16> versions of a function require different
 * parameters effort is made to give a single interface with e.g. workspace handling taken care of.
 */
namespace lapack
{

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
extern int ione;
extern int izero;
extern real16 rone;
extern complex16 cone;
extern real16 rzero;
extern complex16 czero;

template<typename T>char charmagic();

template<typename T> void xscal(int * N, T * DA, T * DX, int * INCX);
template<typename T>void xswap(int * N, T * DX, int * INCX, T * DY, int * INCY);
template<typename T> void xgemv(char * TRANS, int * M, int * N, T * ALPHA, T * A, int * LDA, T * X, int * INCX, T * BETA, T * Y, int * INCY );
template<typename T> void xgemm(char * TRANSA, char * TRANSB, int * M, int * N, int * K, T * ALPHA, T * A, int * LDA, T * B, int * LDB, T * BETA, T * C, int * LDC );
template<typename T> real16 xnrm2(int * N, T * X, int * INCX);
template<typename T> void xpotrs(char * UPLO, int * N, int * NRHS, T * A, int * LDA, T * B, int * LDB, int * INFO);
template<typename T> real16 xlansy(char * NORM, char * UPLO, int * N, T * A, int * LDA, real16 * WORK);
template<typename T> void xpotrf(char * UPLO, int * N, T * A, int * LDA, int * INFO);
template<typename T> void xtrtrs(char * UPLO, char * TRANS, char * DIAG, int * N, int * NRHS, T * A, int * LDA, T * B, int * LDB, int * INFO);
template<typename T> void xgetrf(int * M, int * N, T * A, int * LDA, int * IPIV, int *INFO);

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
 * The F77 integer '1'
 */
extern int * ione;
/**
 * The F77 integer '0'
 */
extern int * izero;
/**
 * The F77 double precision '1.d0'
 */
extern real16 * rone;
/**
 * The F77 complex double precision '(1.d0, 0.d0)'
 */
extern complex16 * cone;
/**
 * The F77 double precision '0.d0'
 */
extern real16 * rzero;
/**
 * The F77 complex double precision '(0.d0, 0.d0)'
 */
extern complex16 * czero;


// BLAS

/**
 * xscal generalised vector scaling
 * @param N as BLAS dscal N
 * @param DA data type specific with intent as BLAS dscal DA
 * @param DX data type specific with intent as BLAS dscal DX
 * @param INCX as BLAS dscal INCX
 */
template<typename T> void xscal(int * N, T * DA, T * DX, int * INCX);

/**
 * xswap generalised vector interchange
 * @param N as BLAS dswap N
 * @param DX data type specific with intent as BLAS dswap DX
 * @param INCX as BLAS dswap INCX
 * @param DY data type specific with intent as BLAS dswap DY
 * @param INCY as BLAS dswap INCY
 */
template<typename T>void xswap(int * N, T * DX, int * INCX, T * DY, int * INCY);


/**
 * xgemv generalised matrix vector multiplication.
 * @param TRANS as BLAS dgemv TRANS
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
template<typename T> void xgemv(char * TRANS, int * M, int * N, T * ALPHA, T * A, int * LDA, T * X, int * INCX, T * BETA, T * Y, int * INCY );

/**
 * xgemm generalised matrix matrix multiplication.
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
template<typename T> void xgemm(char * TRANSA, char * TRANSB, int * M, int * N, int * K, T * ALPHA, T * A, int * LDA, T * B, int * LDB, T * BETA, T * C, int * LDC );

/**
 * xnrm2 generalised 2-norm implementation
 * @param N as BLAS dnrm2 N
 * @param X data type specific with intent as BLAS dnrm2 X
 * @param INCX as BLAS dnrm2 INCX
 * @return the 2-norm of X
 */
template<typename T> real16 xnrm2(int * N, T * X, int * INCX);

// LAPACK

/**
 * xgesvd is a generalised svd implementation that takes care of workspaces
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
template<typename T> void xgesvd(char * JOBU, char * JOBVT, int * M, int * N, T * A, int * LDA, real16 * S, T * U, int * LDU, T * VT, int * LDVT, int * INFO);

/**
 * xgetrf() computes the LU decomposition using parital pivoting
 * @param M as LAPACK dgetrf M
 * @param N as LAPACK dgetrf N
 * @param A data type specific with intent as LAPACK dgetrf A
 * @param LDA as LAPACK dgetrf LDA
 * @param IPIV as LAPACK dgetrf IPIV
 * @param INFO as LAPACK dgetrf INFO
 */
template<typename T> void xgetrf(int * M, int * N, T * A, int * LDA, int * IPIV, int *INFO);

/**
 * xtrcon general triangular matrix condition number estimate
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
template<typename T> void xtrcon(char * NORM, char * UPLO, char * DIAG, int * N, T * A, int * LDA, real16 * RCOND, int * INFO);


/**
 * Solves a triangular system of the form : A * X = B  or  A**T * X = B
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
template<typename T> void xtrtrs(char * UPLO, char * TRANS, char * DIAG, int * N, int * NRHS, T * A, int * LDA, T * B, int * LDB, int * INFO);

/**
 * xpotrf() computes the Cholesky factorization of a Hermitian positive definite matrix A.
 * @param UPLO as LAPACK dpotrf UPLO
 * @param N as LAPACK dpotrf N
 * @param A data type specific with intent as LAPACK dpotrf A
 * @param LDA as LAPACK dpotrf LDA
 * @param INFO as LAPACK dpotrf INFO
 * @throws rdag_error on illegal input
 */
template<typename T> void  xpotrf(char * UPLO, int * N, T * A, int * LDA, int * INFO);


/**
 * xpocon() computes the reciprocal condition estimate (in the 1-norm) of a s.p.d. matrix using the factorization computed by xpotrf().
 * @param UPLO as LAPACK dpocon UPLO
 * @param N as LAPACK dpocon N
 * @param A data type specific with intent as LAPACK dpocon A
 * @param LDA as LAPACK dpocon LDA
 * @param ANORM as LAPACK dpocon ANORM
 * @param RCOND as LAPACK dpocon RCOND
 * @param INFO as LAPACK dpocon INFO
 */
template<typename T> void xpocon(char * UPLO, int * N, T * A, int * LDA, real16 * ANORM, real16 * RCOND, int * INFO);

/**
 * xlansy() computes condition numbers for a symmetric matrix.
 * @param NORM as LAPACK dlansy NORM
 * @param UPLO as LAPACK dlansy UPLO
 * @param N as LAPACK dlansy N
 * @param A data type specific with intent as LAPACK dlansy A
 * @param LDA as LAPACK dlansy LDA
 * @return the NORM as indicated by \a NORM.
 */
template<typename T> real16 xlansy(char * NORM, char * UPLO, int * N, T * A, int * LDA);

/**
 * zlanhe() computes condition numbers for a Hermitian matrix.
 * @param NORM as LAPACK zlanhe NORM
 * @param UPLO as LAPACK zlanhe UPLO
 * @param N as LAPACK zlanhe N
 * @param A a complex Hermitian matrix with intent as given in LAPACK zlanhe A
 * @param LDA as LAPACK zlanhe LDA
 * @return the NORM as indicated by \a NORM.
 */
real16 zlanhe(char * NORM, char * UPLO, int * N, complex16 * A, int * LDA);

/**
 * xpotrs() solves s.p.d. systems via Cholesky decomposition as computed by xpotrf().
 * @param UPLO as LAPACK dpotrs UPLO.
 * @param N as LAPACK dpotrs N.
 * @param NRHS as LAPACK dpotrs NRHS.
 * @param A data type specific with intent as LAPACK dpotrs A.
 * @param LDA as LAPACK dpotrs LDA.
 * @param B data type specific with intent as LAPACK dpotrs B.
 * @param LDB as LAPACK dpotrs LDB.
 * @param INFO as LAPACK dpotrs INFO.
 */
template<typename T> void xpotrs(char * UPLO, int * N, int * NRHS, T * A, int * LDA, T * B, int * LDB, int * INFO);

}


#endif // _LAPACK_HH
