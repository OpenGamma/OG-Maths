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

extern const real16 FuzzyEquals_default_maxabserror;
extern const real16 FuzzyEquals_default_maxrelerror;

/**
 * Bitwise equals, equals via memcmp()
 */
template <typename T>
bool ArrayBitEquals(T * arr1, T * arr2, int count);

extern template bool
ArrayBitEquals(real16 * arr1, real16 * arr2, int count);

extern template bool
ArrayBitEquals(complex16 * arr1, complex16 * arr2, int count);

extern template bool
ArrayBitEquals(int * arr1, int * arr2, int count);

/**
 * Fuzzy equals, equals within tolerance specified
 */
template<typename T>
bool ArrayFuzzyEquals(T * arr1, T * arr2, int count, real16 maxabserror = FuzzyEquals_default_maxabserror, real16 maxrelerror = FuzzyEquals_default_maxrelerror);

extern template bool
ArrayFuzzyEquals(real16 * arr1, real16 * arr2, int count, real16 maxabserror, real16 maxrelerror);
extern template bool
ArrayFuzzyEquals(complex16 * arr1, complex16 * arr2, int count, real16 maxabserror, real16 maxrelerror);

bool SingleValueFuzzyEquals(real16 val1, real16 val2, real16 maxabserror = FuzzyEquals_default_maxabserror, real16 maxrelerror = FuzzyEquals_default_maxrelerror);
bool SingleValueFuzzyEquals(complex16 val1, complex16 val2, real16 maxabserror = FuzzyEquals_default_maxabserror, real16 maxrelerror = FuzzyEquals_default_maxrelerror);



}

#endif // _EQUALS_HH
