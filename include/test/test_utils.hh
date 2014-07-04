/**
 * Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _TEST_UTILS_H
#define _TEST_UTILS_H

#include <cstddef>
#include <memory>

namespace testutils {

/**
 * Wraps a stack allocated 2D array of any type in a unique_ptr containing an on heap 
 * reference to the stack allocated array that is plumbed in to the stack data.
 * The purpose of this is so that stack allocated data in row major form can be
 * transformed to a \a T** with minimal effort.
 * @param T the type of the stack alocated array.
 * @param S1 the number of "rows" in the stack allocated array.
 * @param S2 the number of "columns" in the stack allocated array.
 * @param foo a stack allocated 2D array.
 * @return a unique_ptr to an on heap T*[] wired up for 2D array access to \a foo
 */
template<typename T, std::size_t S1, std::size_t S2> std::unique_ptr<T*[]> stack2heap(T foo[S1][S2] )
{
  std::unique_ptr<T*[]> up (new T*[S1]);
  T ** bar = up.get();
  for(size_t i = 0; i < S1; i++)
  {
    bar[i] = foo[i];
  }
  return up;
}

}

#endif