/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _LAPACK_HH
#define _LAPACK_HH

#define F77FUNC(__FUNC) __FUNC##_

// these lurk in global namespace
extern "C" real16 F77FUNC(dnrm2)(int * N, real16 * X, int * INCX);

extern "C" real16 F77FUNC(dgesvd)(char * JOBU, char * JOBVT, int * M, int * N, real16 * A, int * LDA, real16 * S, real16 * U, int * LDU, real16 * VT, int * LDVT, real16 * WORK, int * LWORK, int * INFO);


#endif // _LAPACK_HH
