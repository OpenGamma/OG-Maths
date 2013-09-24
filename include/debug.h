/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "warningmacros.h"

#ifdef DEBUG
#define DEBUG_PRINT(...) fprintf(stderr, __VA_ARGS__)
#else
#define DEBUG_PRINT(...) do {} while(0)
#endif

// Used by VAL64BIT_PRINT
#define INT64HIGHLOW(val,high,low) \
    low = (long long unsigned int)val & 0x00000000FFFFFFFFULL; \
    high = ((long long unsigned int)val & 0xFFFFFFFF00000000ULL) >> 32

// Print a 64-bit value on all supported platforms
#ifdef __MINGW32__
#define VAL64BIT_PRINT(desc, ptr)                                                 \
  unsigned int SUPPRESS_UNUSED __ptr_print_high, SUPPRESS_UNUSED __ptr_print_low; \
  INT64HIGHLOW(ptr, __ptr_print_high, __ptr_print_low);                           \
  DEBUG_PRINT(desc " is 0x%08x%08x\n", __ptr_print_high, __ptr_print_low);
#else
#define VAL64BIT_PRINT(desc, ptr)                                                 \
  DEBUG_PRINT(desc " is 0x%llx\n", (long long unsigned int)ptr);
#endif
