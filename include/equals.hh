/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _EQUALS_HH
#define _EQUALS_HH
#include "terminal.hh"

namespace librdag {

template <typename T>
bool ArrayBitEquals(T * arr1, T * arr2, int count);

extern template bool
ArrayBitEquals(real16 * arr1, real16 * arr2, int count);

extern template bool
ArrayBitEquals(complex16 * arr1, complex16 * arr2, int count);

extern template bool
ArrayBitEquals(int * arr1, int * arr2, int count);

}

#endif // _EQUALS_HH
