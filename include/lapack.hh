/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _LAPACK_HH
#define _LAPACK_HH

#include "numerictypes.hh"

#define F77FUNC(__FUNC) __FUNC##_

// these lurk in global namespace


// BLAS
// Standard xSCAL
extern "C" void F77FUNC(dscal)(int * N, real16 * DA, real16* DX, int * INCX);
extern "C" void F77FUNC(zscal)(int * N, complex16 * DA, complex16* DX, int * INCX);

// Standard xGEMV
extern "C" void F77FUNC(dgemv)(char * TRANS, int * M, int * N, real16 * ALPHA, real16 * A, int * LDA, real16 * X, int * INCX, real16 * BETA, real16 * Y, int * INCY);
extern "C" void F77FUNC(zgemv)(char * TRANS, int * M, int * N, complex16 * ALPHA, complex16 * A, int * LDA, complex16 * X, int * INCX, complex16 * BETA, complex16 * Y, int * INCY);

// Standard xGEMM
extern "C" void F77FUNC(dgemm)(char * TRANSA, char * TRANSB, int * M, int * N, int * K, real16 * ALPHA, real16 * A, int * LDA, real16 * B, int * LDB, real16 * BETA, real16 * C, int * LDC );
extern "C" void F77FUNC(zgemm)(char * TRANSA, char * TRANSB, int * M, int * N, int * K, complex16 * ALPHA, complex16 * A, int * LDA, complex16 * B, int * LDB, complex16 * BETA, complex16 * C, int * LDC );

// Standard NORM2 implementations.
extern "C" real16 F77FUNC(dnrm2)(int * N, real16 * X, int * INCX);
extern "C" real16 F77FUNC(dznrm2)(int * N, complex16 * X, int * INCX);


// LAPACK

// Standard SVD implementations.
extern "C" void F77FUNC(dgesvd)(char * JOBU, char * JOBVT, int * M, int * N, real16 * A, int * LDA, real16 * S, real16 * U, int * LDU, real16 * VT, int * LDVT, real16 * WORK, int * LWORK, int * INFO);
extern "C" void F77FUNC(zgesvd)(char * JOBU, char * JOBVT, int * M, int * N, complex16 * A, int * LDA, real16 * S, complex16 * U, int * LDU, complex16 * VT, int * LDVT, complex16 * WORK, int * LWORK, real16 * RWORK, int * INFO);

#endif // _LAPACK_HH
