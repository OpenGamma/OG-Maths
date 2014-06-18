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
void set_xerbla_death_switch(int4 * value);

#ifdef __cplusplus
extern "C"
#endif
int4 get_xerbla_death_switch();

// BLAS
// Standard xSCAL
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(dscal)(int4 * N, real8 * DA, real8* DX, int4 * INCX);
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(zscal)(int4 * N, complex16 * DA, complex16* DX, int4 * INCX);

// Standard xSWAP
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(dswap)(int4 * N, real8 * DX, int4 * INCX, real8 * DY, int4 * INCY);
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(zswap)(int4 * N, complex16 * DX, int4 * INCX, complex16 * DY, int4 * INCY);


// Standard xGEMV
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(dgemv)(char * TRANS, int4 * M, int4 * N, real8 * ALPHA, real8 * A, int4 * LDA, real8 * X, int4 * INCX, real8 * BETA, real8 * Y, int4 * INCY);
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(zgemv)(char * TRANS, int4 * M, int4 * N, complex16 * ALPHA, complex16 * A, int4 * LDA, complex16 * X, int4 * INCX, complex16 * BETA, complex16 * Y, int4 * INCY);

// Standard xGEMM
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(dgemm)(char * TRANSA, char * TRANSB, int4 * M, int4 * N, int4 * K, real8 * ALPHA, real8 * A, int4 * LDA, real8 * B, int4 * LDB, real8 * BETA, real8 * C, int4 * LDC );
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(zgemm)(char * TRANSA, char * TRANSB, int4 * M, int4 * N, int4 * K, complex16 * ALPHA, complex16 * A, int4 * LDA, complex16 * B, int4 * LDB, complex16 * BETA, complex16 * C, int4 * LDC );

// Standard NORM2 implementations.
#ifdef __cplusplus
extern "C"
#endif
real8 F77FUNC(dnrm2)(int4 * N, real8 * X, int4 * INCX);
#ifdef __cplusplus
extern "C"
#endif
real8 F77FUNC(dznrm2)(int4 * N, complex16 * X, int4 * INCX);


// LAPACK

// Standard SVD implementations.
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(dgesvd)(char * JOBU, char * JOBVT, int4 * M, int4 * N, real8 * A, int4 * LDA, real8 * S, real8 * U, int4 * LDU, real8 * VT, int4 * LDVT, real8 * WORK, int4 * LWORK, int4 * INFO);
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(zgesvd)(char * JOBU, char * JOBVT, int4 * M, int4 * N, complex16 * A, int4 * LDA, real8 * S, complex16 * U, int4 * LDU, complex16 * VT, int4 * LDVT, complex16 * WORK, int4 * LWORK, real8 * RWORK, int4 * INFO);

// Reciprocal condition number estimates
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(dtrcon)(char * NORM, char * UPLO, char * DIAG, int4 * N, real8 * A, int4 * LDA, real8 * RCOND, real8 * WORK, int4 * IWORK, int4 * INFO);
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(ztrcon)(char * NORM, char * UPLO, char * DIAG, int4 * N, complex16 * A, int4 * LDA, real8 * RCOND, complex16 * WORK, real8 * IWORK, int4 * INFO);

// LUP decomposition
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(dgetrf)(int4 * M, int4 * N, real8 * A, int4 * LDA, int4 * IPIV, int4 *INFO);
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(zgetrf)(int4 * M, int4 * N, complex16 * A, int4 * LDA, int4 * IPIV, int4 *INFO);

// inverse based on LU
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(dgetri)(int4 * N, real8 * A, int4 * LDA, int4 * IPIV, real8 * WORK, int4 * LWORK, int4 * INFO );
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(zgetri)(int4 * N, complex16 * A, int4 * LDA, int4 * IPIV, complex16 * WORK, int4 * LWORK, int4 * INFO );

// solves a triangular system of the form : A * X = B  or  A**T * X = B
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(dtrtrs)(char * UPLO, char * TRANS, char * DIAG, int4 * N, int4 * NRHS, real8 * A, int4 * LDA, real8 * B, int4 * LDB, int4 * INFO);
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(ztrtrs)(char * UPLO, char * TRANS, char * DIAG, int4 * N, int4 * NRHS, complex16 * A, int4 * LDA, complex16 * B, int4 * LDB, int4 * INFO);

// Cholesky factorisation for s.p.d matrices
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(dpotrf)(char * UPLO, int4 * N, real8 * A, int4 * LDA, int4 * INFO);
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(zpotrf)(char * UPLO, int4 * N, complex16 * A, int4 * LDA, int4 * INFO);

// Reciprocal condition estimate in the 1-norm of a Cholesky decomposition as computed by xpotrf
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(dpocon)(char * UPLO, int4 * N, real8 * A, int4 * LDA, real8 * ANORM, real8 * RCOND, real8 * WORK, int4 * IWORK, int4 * INFO);
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(zpocon)(char * UPLO, int4 * N, complex16 * A, int4 * LDA, real8 * ANORM, real8 * RCOND, complex16 * WORK, real8 * RWORK, int4 * INFO);

// Norm calculators for symmetric matrices
#ifdef __cplusplus
extern "C"
#endif
real8 F77FUNC(dlansy)(char * NORM, char * UPLO, int4 * N, real8 * A, int4 * LDA, real8 * WORK);
#ifdef __cplusplus
extern "C"
#endif
real8 F77FUNC(zlansy)(char * NORM, char * UPLO, int4 * N, complex16 * A, int4 * LDA, real8 * WORK);

// Norm calculator for Hermitian matrix
#ifdef __cplusplus
extern "C"
#endif
real8 F77FUNC(zlanhe)(char * NORM, char * UPLO, int4 * N, complex16 * A, int4 * LDA, real8 * WORK);

// Cholesky based solvers
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(dpotrs)(char * UPLO, int4 * N, int4 * NRHS, real8 * A, int4 * LDA, real8 * B, int4 * LDB, int4 * INFO);
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(zpotrs)(char * UPLO, int4 * N, int4 * NRHS, complex16 * A, int4 * LDA, complex16 * B, int4 * LDB, int4 * INFO);

// Matrix norms, {1,Inf,Frob}
#ifdef __cplusplus
extern "C"
#endif
real8 F77FUNC(dlange)(char * NORM, int4 * M, int4 * N, real8 * A, int4 * LDA, real8 * WORK );
#ifdef __cplusplus
extern "C"
#endif
real8 F77FUNC(zlange)(char * NORM, int4 * M, int4 * N, complex16 * A, int4 * LDA, real8 * WORK );

// Reciprocal condition number calculated from a LU factorisation computed by XGETRF
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(dgecon)(char * NORM, int4 * N, real8 * A, int4 * LDA, real8 * ANORM, real8 * RCOND, real8 * WORK, int4 * IWORK, int4 * INFO);
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(zgecon)(char * NORM, int4 * N, complex16 * A, int4 * LDA, real8 * ANORM, real8 * RCOND, complex16 * WORK, real8 * RWORK, int4 * INFO);

// Solves linear systems using a LU factorisation computed by XGETRF
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(dgetrs)(char * TRANS, int4 * N, int4 * NRHS, real8 * A, int4 * LDA, int4 * IPIV, real8 * B, int4 * LDB, int4 * INFO);
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(zgetrs)(char * TRANS, int4 * N, int4 * NRHS, complex16 * A, int4 * LDA, int4 * IPIV, complex16 * B, int4 * LDB, int4 * INFO);

// Solve {over,under}determined linear systems via QR decomposition
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(dgels)(char * TRANS, int4 * M, int4 * N, int4 * NRHS, real8 * A, int4 * LDA, real8 * B, int4 * LDB, real8 * WORK, int4 * LWORK, int4 * INFO);
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(zgels)(char * TRANS, int4 * M, int4 * N, int4 * NRHS, complex16 * A, int4 * LDA, complex16 * B, int4 * LDB, complex16 * WORK, int4 * LWORK, int4 * INFO);

// Solve {over,under}determined linear systems via SV decomposition
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(dgelsd)(int4 * M, int4 * N, int4 * NRHS, real8 * A, int4 * LDA, real8 * B, int4 * LDB, real8 * S, real8 * RCOND, int4 * RANK, real8 * WORK, int4 * LWORK, int4 * IWORK, int4 * INFO );
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(zgelsd)(int4 * M, int4 * N, int4 * NRHS, complex16 * A, int4 * LDA, complex16 * B, int4 * LDB, real8 * S, real8 * RCOND, int4 * RANK, complex16 * WORK, int4 * LWORK, real8 * RWORK, int4 * IWORK, int4 * INFO);

// Compute eig{values,vectors}
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(dgeev)(char * JOBVL, char * JOBVR, int4 * N, real8 * A, int4 * LDA, real8 * WR, real8 * WI, real8 * VL, int4 * LDVL, real8 * VR, int4 * LDVR, real8 * WORK, int4 * LWORK, int4 * INFO);
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(zgeev)(char * JOBVL, char * JOBVR, int4 * N, complex16 * A, int4 * LDA, complex16 * W, complex16 * VL, int4 * LDVL, complex16 * VR, int4 * LDVR, complex16 * WORK, int4 * LWORK, real8 * RWORK, int4 * INFO);


// Compute QR factorisation
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(dgeqrf)(int4 * M, int4 * N, real8 * A, int4 * LDA, real8 * TAU, real8 * WORK, int4 * LWORK, int4 *INFO);
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(zgeqrf)(int4 * M, int4 * N, complex16 * A, int4 * LDA, complex16 * TAU, complex16 * WORK, int4 * LWORK, int4 *INFO);

// Generate orthogonal Q matrix from elementary reflectors as returned by xgeqrf()
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(dorgqr)(int4 * M, int4 * N, int4 * K, real8 * A, int4 * LDA, real8 * TAU, real8 * WORK, int4 * LWORK, int4 * INFO);
#ifdef __cplusplus
extern "C"
#endif
void F77FUNC(zungqr)(int4 * M, int4 * N, int4 * K, complex16 * A, int4 * LDA, complex16 * TAU, complex16 * WORK, int4 * LWORK, int4 * INFO);

#endif//_LAPACK_RAW_H
