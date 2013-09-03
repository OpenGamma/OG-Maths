/**
 * Copyright (C) 2012 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _WINPRINT64_H
#define _WINPRINT64_H

#define INT64HIGHLOW(val,high,low) \
    low = (long long unsigned int)val & 0x00000000FFFFFFFFULL; \
    high = ((long long unsigned int)val & 0xFFFFFFFF00000000ULL) >> 32

#endif
