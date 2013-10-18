/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */
#include <assert.h>
#include <string.h>
#include "terminal.hh"
#include "warningmacros.h"
#include "equals.hh"
#include <iostream>

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

template <typename T> bool ArrayFuzzyEquals(T * arr1, T * arr2, int count, real16 maxabserror, real16 maxrelerror)
{
  assert(count>=0);
  for(int i=0;i<count;i++)
  {
    if(!SingleValueFuzzyEquals(arr1[i], arr2[i], maxabserror, maxrelerror)) return false;
  }
  // all data ok
  return true;
}

template bool ArrayFuzzyEquals(real16 * arr1, real16 * arr2, int count, real16 maxabserror, real16 maxrelerror);
template bool ArrayFuzzyEquals(complex16 * arr1, complex16 * arr2, int count, real16 maxabserror, real16 maxrelerror);



bool SingleValueFuzzyEquals(real16 val1, real16 val2, real16 maxabserror, real16 maxrelerror)
{
  // IEEE754 nans not comparable, their relation is considered "unordered" sec 5.7.
  if(std::isnan(val1)) return false;
  if(std::isnan(val2)) return false;

  // check if they are below max absolute error bounds (i.e. small in the first place)
  real16 diff = (val1-val2);
  if(std::fabs(diff)>maxabserror) return false;

  // check if they are both 0, else the rel error gets stuck with /0
  if((val1==0.e0)&&(val2==0.e0)) return true;

  // check if they are within a relative error bound, div difference by largest of the 2
  real16 divisor = std::fabs(val1) > std::fabs(val2) ? val1 : val2;
  real16 relerror = std::fabs(diff/divisor);
  if(relerror > maxrelerror) return false;

  // all ok
  return true;
}

bool SingleValueFuzzyEquals(complex16 val1, complex16 val2, real16 maxabserror, real16 maxrelerror)
{
  if(!SingleValueFuzzyEquals(std::real(val1), std::real(val2) , maxabserror, maxrelerror)) return false;
  if(!SingleValueFuzzyEquals(std::imag(val1), std::imag(val2) , maxabserror, maxrelerror)) return false;
  // all ok
  return true;
}

}