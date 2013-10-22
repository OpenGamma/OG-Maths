/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _NUMERICTYPES_HH
#define _NUMERICTYPES_HH

#include <complex>

using namespace std;

typedef complex<double> complex16;
typedef complex<float> complex8;
typedef double real16;
typedef float real8;

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


#endif /* NUMERICTYPES_HH_ */
