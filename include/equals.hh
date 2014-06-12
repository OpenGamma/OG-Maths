/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _EQUALS_HH
#define _EQUALS_HH
#include <cfloat>
#include "numerictypes.hh"

namespace librdag {

extern const real8 FuzzyEquals_default_maxabserror;
extern const real8 FuzzyEquals_default_maxrelerror;

/**
 * Bitwise equals, equals via memcmp()
 */
template <typename T>
bool ArrayBitEquals(T * arr1, T * arr2, int count);

extern template bool
ArrayBitEquals(real8 * arr1, real8 * arr2, int count);

extern template bool
ArrayBitEquals(complex16 * arr1, complex16 * arr2, int count);

extern template bool
ArrayBitEquals(int * arr1, int * arr2, int count);

/**
 * Fuzzy equals, equals within tolerance specified
 */
template<typename T>
bool ArrayFuzzyEquals(T * arr1, T * arr2, int count, real8 maxabserror = FuzzyEquals_default_maxabserror, real8 maxrelerror = FuzzyEquals_default_maxrelerror);

extern template bool
ArrayFuzzyEquals(real8 * arr1, real8 * arr2, int count, real8 maxabserror, real8 maxrelerror);
extern template bool
ArrayFuzzyEquals(complex16 * arr1, complex16 * arr2, int count, real8 maxabserror, real8 maxrelerror);

bool SingleValueFuzzyEquals(real8 val1, real8 val2, real8 maxabserror = FuzzyEquals_default_maxabserror, real8 maxrelerror = FuzzyEquals_default_maxrelerror);
bool SingleValueFuzzyEquals(complex16 val1, complex16 val2, real8 maxabserror = FuzzyEquals_default_maxabserror, real8 maxrelerror = FuzzyEquals_default_maxrelerror);



}

#endif // _EQUALS_HH
