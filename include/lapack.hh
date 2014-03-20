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
extern int ione;
extern real16 rone;
extern complex16 cone;
extern real16 rzero;
extern complex16 czero;

}

/**
 * The F77 character 'N'
 */
extern char * N;
/**
 * The F77 integer '1'
 */
extern int * ione;
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
 * xscal general vector scaling
 */
template<typename T> void xscal(int * N, T * DA, T * DX, int * INCX);

/**
 * xgemv general matrix vector multiplication.
 */
template<typename T> void xgemv(char * TRANS, int * M, int * N, T * ALPHA, T * A, int * LDA, T * X, int * INCX, T * BETA, T * Y, int * INCY );

/**
 * xgemm general matrix matrix multiplication.
 */
template<typename T> void xgemm(char * TRANSA, char * TRANSB, int * M, int * N, int * K, T * ALPHA, T * A, int * LDA, T * B, int * LDB, T * BETA, T * C, int * LDC );

/**
 * xnrm2 general norm2 implementation
 */
template<typename T> real16 xnrm2(int * N, T * X, int * INCX);

// LAPACK

/**
 * xgesvd general svd implementation that takes care of workspaces
 */
template<typename T> void xgesvd(char * JOBU, char * JOBVT, int * M, int * N, T * A, int * LDA, real16 * S, T * U, int * LDU, T * VT, int * LDVT, int * INFO);

}


#endif // _LAPACK_HH
