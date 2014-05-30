//
// Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
//
// Please see distribution for license.
//

#include <stdlib.h>

#ifndef _MEM_H
#define _MEM_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \def __ALIGNMENT 
 * The default alignment for AVX support
 */
#define __ALIGNMENT 64

/**
 * Allocate vector instruction performance aware aligned memory.
 *
 * This function allocates memory aligned to the default boundary of 64 bytes. This is such
 * that AVX and SSE (as as AVX base address alignment is a superset of SSE) instructions
 * can be generated with hints towards using their aligned variants when accesing memory 
 * allocated via this function. Additionally, cachelines are normally 64 bytes, so we get
 * more hits for shorter vectors this way.
 *
 * @param size the amount of memory to allocate in bytes.
 * @return a pointer to the address of the memory allocated.
 *
 */
void * malloc_aligned(size_t size);

/**
 * Free aligned memory that was allocated using
 * \a malloc_aligned().
 * @param mem the address of the aligned memory to free.
 */
void free_aligned(void * mem);

#ifdef __cplusplus
}
#endif

#endif // _MEM_H
