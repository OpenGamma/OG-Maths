/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _LAPACK_HH
#define _LAPACK_HH

#define F77FUNC(__FUNC) __FUNC##_

// these lurk in global namespace

// Standard NORM2 implementations.
extern "C" real16 F77FUNC(dnrm2)(int * N, real16 * X, int * INCX);
extern "C" real16 F77FUNC(dznrm2)(int * N, complex16 * X, int * INCX);

// Standard SVD implementations.
extern "C" void F77FUNC(dgesvd)(char * JOBU, char * JOBVT, int * M, int * N, real16 * A, int * LDA, real16 * S, real16 * U, int * LDU, real16 * VT, int * LDVT, real16 * WORK, int * LWORK, int * INFO);
extern "C" void F77FUNC(zgesvd)(char * JOBU, char * JOBVT, int * M, int * N, complex16 * A, int * LDA, real16 * S, complex16 * U, int * LDU, complex16 * VT, int * LDVT, complex16 * WORK, int * LWORK, real16 * RWORK, int * INFO);


#endif // _LAPACK_HH
