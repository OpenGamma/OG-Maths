/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _LAPACK_HH
#define _LAPACK_HH

#define F77FUNC(__FUNC) __FUNC##_

// these lurk in global namespace FTW F77
extern "C" real16 F77FUNC(dnrm2)(int * N, real16 * X, int * INCX);

#endif // _LAPACK_HH
