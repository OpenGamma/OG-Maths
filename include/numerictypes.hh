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
using namespace std;
#endif

#ifdef __cplusplus
typedef complex<double> complex16;
typedef complex<float> complex8;
typedef double real16;
typedef float real8;
#else
typedef double complex complex16;
typedef float complex complex8;
typedef double real16;
typedef float real8;
#endif

#ifdef __cplusplus
namespace librdag {

real16 getNaN();
complex16 getComplexNaN();

real16 getPosInf();
real16 getNegInf();
complex16 getPosPosComplexInf();
complex16 getPosNegComplexInf();
complex16 getNegPosComplexInf();
complex16 getNegNegComplexInf();

}
#endif

#endif /* NUMERICTYPES_HH_ */
