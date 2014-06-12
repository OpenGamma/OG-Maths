/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _NUMERICTYPES_HH
#define _NUMERICTYPES_HH

#ifdef __cplusplus
#include <complex>
#else
#include <complex.h>
#endif

#ifdef __cplusplus
typedef std::complex<double> complex16;
typedef std::complex<float> complex8;
typedef double real8;
typedef float real4;
#else
typedef double complex complex16;
typedef float complex complex8;
typedef double real8;
typedef float real4;
#endif

#ifdef __cplusplus
namespace librdag {

real8 getNaN();
complex16 getComplexNaN();

real8 getPosInf();
real8 getNegInf();
complex16 getPosPosComplexInf();
complex16 getPosNegComplexInf();
complex16 getNegPosComplexInf();
complex16 getNegNegComplexInf();

}
#endif

#endif /* NUMERICTYPES_HH_ */
