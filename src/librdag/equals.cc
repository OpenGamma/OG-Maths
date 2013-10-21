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
#include "debug.h"
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


/**
 * Checks if two double precision floating point numbers are approximately "equal"
 * The parameter "maxabserror" determines the minimum threshold for "equal" in terms
 * of the two numbers being very small in magnitude.
 * The parameter "maxrelerror" determines the minimum threshold for "equal" in terms
 * of the relative magnitude of the numbers. i.e. invariant of the magnitude of the numbers
 * what is the maximum level of magnitude difference acceptable.
 */
bool SingleValueFuzzyEquals(real16 val1, real16 val2, real16 maxabserror, real16 maxrelerror)
{

#ifdef __LOCALDEBUG
#ifdef DEBUG
  DEBUG_PRINT("FuzzyEquals: Comparing %24.16f and %24.16f\n", val1,val2);
#endif
#endif

  // IEEE754 nans not comparable, their relation is considered "unordered" sec 5.7.
  if(std::isnan(val1))
  {
#ifdef __LOCALDEBUG
#ifdef DEBUG
  DEBUG_PRINT("FuzzyEquals: Failed as value 1 is NaN\n");
#endif
#endif
    return false;
  }

  if(std::isnan(val2))
  {
#ifdef __LOCALDEBUG
#ifdef DEBUG
  DEBUG_PRINT("FuzzyEquals: Failed as value 2 is NaN\n");
#endif
#endif
    return false;
  }

// deal with infs in debug mode
#ifdef __LOCALDEBUG
#ifdef DEBUG
  bool val1isinf = std::isinf(val1);
  bool val2isinf = std::isinf(val2);
  if(val1isinf||val2isinf)
  {
    if(val1isinf&&val2isinf)
    {
      if(signbit(val2)==signbit(val1))
      {
        DEBUG_PRINT("FuzzyEquals: Inf Branch. Success as both inf of same sign\n");
        return true;
      }
    }

  DEBUG_PRINT("FuzzyEquals: Inf Branch. Fail, non matching infs\n");
    return false;
  }
#endif
#endif

  if(val1 == val2)
  {
      return true; // (+/-)inf compares == as does (+/-)0.e0
  }

  // check if they are below max absolute error bounds (i.e. small in the first place)
  real16 diff = (val1-val2);
  if(maxabserror>std::fabs(diff))
  {
#ifdef __LOCALDEBUG
#ifdef DEBUG
  DEBUG_PRINT("FuzzyEquals: Match as below diff bounds. maxabserror > diff. (%24.16f >  %24.16f)\n", maxabserror, std::abs(diff));
#endif
#endif
    return true;
  }
#ifdef __LOCALDEBUG
#ifdef DEBUG
  DEBUG_PRINT("FuzzyEquals: Failed as diff > maxabserror. (%24.16f >  %24.16f)\n", std::abs(diff), maxabserror);
#endif
#endif

  // check if they are within a relative error bound, div difference by largest of the 2
  real16 divisor = std::fabs(val1) > std::fabs(val2) ? val1 : val2;
  real16 relerror = std::fabs(diff/divisor);
  if(maxrelerror > relerror)
  {
#ifdef __LOCALDEBUG
#ifdef DEBUG
  DEBUG_PRINT("FuzzyEquals: Match as maxrelerror > relerror. (%24.16f >  %24.16f)\n", maxrelerror, relerror);
#endif
#endif
    return true;
  };

#ifdef __LOCALDEBUG
#ifdef DEBUG
  DEBUG_PRINT("FuzzyEquals: Fail as relerror > maxrelerror. (%24.16f >  %24.16f)\n", relerror, maxrelerror);
#endif
#endif

  return false;
}

bool SingleValueFuzzyEquals(complex16 val1, complex16 val2, real16 maxabserror, real16 maxrelerror)
{
  if(!SingleValueFuzzyEquals(std::real(val1), std::real(val2) , maxabserror, maxrelerror)) return false;
  if(!SingleValueFuzzyEquals(std::imag(val1), std::imag(val2) , maxabserror, maxrelerror)) return false;
  // all ok
  return true;
}

}