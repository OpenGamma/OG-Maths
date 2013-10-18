/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
#include <assert.h>
#include <string.h>
#include "terminal.hh"
#include "warningmacros.h"

using namespace std;
namespace librdag {

template <typename T> bool ArrayBitEquals(T * arr1, T * arr2, int count)
{
  assert(count>=0);
  size_t len = count*sizeof(T);
  if(!memcmp(arr1,arr2,len))
  {
    return true;
  }
  return false;
}

template bool ArrayBitEquals(real16 * arr1, real16 * arr2, int count);
template bool ArrayBitEquals(complex16 * arr1, complex16 * arr2, int count);
template bool ArrayBitEquals(int * arr1, int * arr2, int count);

}