//
// Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
//
// Please see distribution for license.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include "mem.h"
#include "gtest/gtest.h"


TEST(mem_test,check_malloc_aligned)
{
  // function is aligning mallocs to "__ALIGNMENT" byte boundaries.
  size_t size = 1024;
  void * mem = malloc_aligned(size);
  ASSERT_TRUE((size_t)mem%(size_t)__ALIGNMENT==0);
  free_aligned(mem);
}

#ifndef DEBUG
#define RESETDEBUG
#define DEBUG
#endif
#include "mem.h"
TEST(mem_test,check_malloc_aligned_debug)
{
  // function is aligning mallocs to "__ALIGNMENT" byte boundaries.
  size_t size = 1024;
  void * mem = malloc_aligned(size);
  ASSERT_TRUE((size_t)mem%(size_t)__ALIGNMENT==0);
  free_aligned(mem);
}

// NOTE: Very hard to test exits as:
// 1) EIVAL needs an invalid arg, it's #defined as "__ALIGNMENT" which is valid!
// 2) ENOMEM requires a large malloc, we're limited by java's size_t as 4 bytes.

#ifdef RESETDEBUG
#undef DEBUG
#endif
