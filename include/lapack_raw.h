/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

/**
 * Raw "C" style LAPACK bindings
 */

#ifndef _LAPACK_RAW_H
#define _LAPACK_RAW_H

#include "numerictypes.hh"

#define F77FUNC(__FUNC) __FUNC##_

// these lurk in global namespace

// Xerbla handing, these are ISO C bound so no name mangling required
#ifdef __cplusplus
extern "C" 
#endif
void set_xerbla_death_switch(int * value);

#ifdef __cplusplus
extern "C" 
#endif
int get_xerbla_death_switch();

// BLAS
// Standard xSCAL
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(dscal)(int * N, real8 * DA, real8* DX, int * INCX);
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(zscal)(int * N, complex16 * DA, complex16* DX, int * INCX);

// Standard xSWAP
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(dswap)(int * N, real8 * DX, int * INCX, real8 * DY, int * INCY);
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(zswap)(int * N, complex16 * DX, int * INCX, complex16 * DY, int * INCY);


// Standard xGEMV
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(dgemv)(char * TRANS, int * M, int * N, real8 * ALPHA, real8 * A, int * LDA, real8 * X, int * INCX, real8 * BETA, real8 * Y, int * INCY);
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(zgemv)(char * TRANS, int * M, int * N, complex16 * ALPHA, complex16 * A, int * LDA, complex16 * X, int * INCX, complex16 * BETA, complex16 * Y, int * INCY);

// Standard xGEMM
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(dgemm)(char * TRANSA, char * TRANSB, int * M, int * N, int * K, real8 * ALPHA, real8 * A, int * LDA, real8 * B, int * LDB, real8 * BETA, real8 * C, int * LDC );
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(zgemm)(char * TRANSA, char * TRANSB, int * M, int * N, int * K, complex16 * ALPHA, complex16 * A, int * LDA, complex16 * B, int * LDB, complex16 * BETA, complex16 * C, int * LDC );

// Standard NORM2 implementations.
#ifdef __cplusplus
extern "C" 
#endif
real8 F77FUNC(dnrm2)(int * N, real8 * X, int * INCX);
#ifdef __cplusplus
extern "C" 
#endif
real8 F77FUNC(dznrm2)(int * N, complex16 * X, int * INCX);


// LAPACK

// Standard SVD implementations.
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(dgesvd)(char * JOBU, char * JOBVT, int * M, int * N, real8 * A, int * LDA, real8 * S, real8 * U, int * LDU, real8 * VT, int * LDVT, real8 * WORK, int * LWORK, int * INFO);
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(zgesvd)(char * JOBU, char * JOBVT, int * M, int * N, complex16 * A, int * LDA, real8 * S, complex16 * U, int * LDU, complex16 * VT, int * LDVT, complex16 * WORK, int * LWORK, real8 * RWORK, int * INFO);

// Reciprocal condition number estimates
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(dtrcon)(char * NORM, char * UPLO, char * DIAG, int * N, real8 * A, int * LDA, real8 * RCOND, real8 * WORK, int * IWORK, int * INFO);
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(ztrcon)(char * NORM, char * UPLO, char * DIAG, int * N, complex16 * A, int * LDA, real8 * RCOND, complex16 * WORK, real8 * IWORK, int * INFO);

// LUP decomposition
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(dgetrf)(int * M, int * N, real8 * A, int * LDA, int * IPIV, int *INFO);
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(zgetrf)(int * M, int * N, complex16 * A, int * LDA, int * IPIV, int *INFO);

// inverse based on LU
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(dgetri)(int * N, real8 * A, int * LDA, int * IPIV, real8 * WORK, int * LWORK, int * INFO );
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(zgetri)(int * N, complex16 * A, int * LDA, int * IPIV, complex16 * WORK, int * LWORK, int * INFO );

// solves a triangular system of the form : A * X = B  or  A**T * X = B
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(dtrtrs)(char * UPLO, char * TRANS, char * DIAG, int * N, int * NRHS, real8 * A, int * LDA, real8 * B, int * LDB, int * INFO);
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(ztrtrs)(char * UPLO, char * TRANS, char * DIAG, int * N, int * NRHS, complex16 * A, int * LDA, complex16 * B, int * LDB, int * INFO);

// Cholesky factorisation for s.p.d matrices
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(dpotrf)(char * UPLO, int * N, real8 * A, int * LDA, int * INFO);
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(zpotrf)(char * UPLO, int * N, complex16 * A, int * LDA, int * INFO);

// Reciprocal condition estimate in the 1-norm of a Cholesky decomposition as computed by xpotrf
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(dpocon)(char * UPLO, int * N, real8 * A, int * LDA, real8 * ANORM, real8 * RCOND, real8 * WORK, int * IWORK, int * INFO);
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(zpocon)(char * UPLO, int * N, complex16 * A, int * LDA, real8 * ANORM, real8 * RCOND, complex16 * WORK, real8 * RWORK, int * INFO);

// Norm calculators for symmetric matrices
#ifdef __cplusplus
extern "C" 
#endif
real8 F77FUNC(dlansy)(char * NORM, char * UPLO, int * N, real8 * A, int * LDA, real8 * WORK);
#ifdef __cplusplus
extern "C" 
#endif
real8 F77FUNC(zlansy)(char * NORM, char * UPLO, int * N, complex16 * A, int * LDA, real8 * WORK);

// Norm calculator for Hermitian matrix
#ifdef __cplusplus
extern "C" 
#endif
real8 F77FUNC(zlanhe)(char * NORM, char * UPLO, int * N, complex16 * A, int * LDA, real8 * WORK);


// Cholesky based solvers
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(dpotrs)(char * UPLO, int * N, int * NRHS, real8 * A, int * LDA, real8 * B, int * LDB, int * INFO);
#ifdef __cplusplus
extern "C" 
#endif
void F77FUNC(zpotrs)(char * UPLO, int * N, int * NRHS, complex16 * A, int * LDA, complex16 * B, int * LDB, int * INFO);

#endif //_LAPACK_RAW_H