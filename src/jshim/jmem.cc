//
// Copyright (C) 2014 - present by OpenGamma Inc. and the OpenGamma group of companies
//
// Please see distribution for license.
//

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <limits.h>
#include "mem.h"
#include "warningmacros.h"
#include "jni.h"
#include "com_opengamma_maths_mem_Stdlib.h"
#define __FAIL_AFTER 100 /* The number of attempts at locating a block of memory with an address such that a jlong can be used for fake pointer arithmetic */

#ifdef __cplusplus
extern "C" {
#endif
 /**
  * This function is a JNI binding function that provides crude access to the POSIX function
  * posix_memalign(3) or if on Windows _aligned_malloc().
  *
  * Its purpose is to allocate byte aligned memory of a specified quantity.
  * The default alignment is defined in {@link mem.c} as 64 which is most suitable for AVX
  * instructions.
  * The value of 64 bytes causes no harm to SSE instructions either as it is a superset of their
  * optimal alignment requirements.
  *
  * The function returns a java "long" which is valid for arithmetic within the bounds of the
  * size of memory allocated.
  * As finding a suitable block of memory with an address to match the above requirement is an
  * iterative process it is possible for this function to fail.
  *
  *
  * Class:     com_opengamma_maths_mem_Stdlib
  * Method:    wrapped_getAlignedMemory
  * Signature: (J)J
  *
  * @param env the JNI environment struct.
  * @param clazz (unused reference but required by the API).
  * @param amount the quantity in bytes of memory required to be allocated.
  * @return the base address of the allocated memory guaranteed safe for pointer
  * arithmetic behaviour (see function docs).
  *
  */
JNIEXPORT jlong JNICALL Java_com_opengamma_maths_mem_Stdlib_wrapped_1allocateAlignedMemory
(JNIEnv SUPPRESS_UNUSED * env, jclass SUPPRESS_UNUSED clazz, jlong amount)
{
  uintptr_t * memptr;
  size_t size = (size_t)amount;
  int attempts = 0;

  // try and find a memory block with an address suitable for pointer arithmetic with a signed long long (64bit jlong)
  memptr = (uintptr_t *) malloc_aligned(size);

  if((unsigned long long)memptr < LLONG_MAX && (unsigned long long)(memptr+amount) > LLONG_MAX)   // arithmetic would overflow jlong
  {
    while((unsigned long long)memptr < LLONG_MAX && (unsigned long long)(memptr+amount) > LLONG_MAX)
    {
      free_aligned(memptr);
      memptr = (uintptr_t *) malloc_aligned(size);
      if(attempts++>__FAIL_AFTER)
      {
        printf("Failed to find clear address space sufficiently large to perform the requested allocation.");
        exit(1);
      }
    }
  }
  return (jlong)memptr;
}

 /**
  * This function is a JNI binding function for accessing the free(3) function on POSIX
  * compliant platforms, on Windows the bound function is _free_aligned().
  *
  * This function can be used to free memory allocated using wrapped_allocateAlignedMemory from java.
  * The address to be freed must be valid in exactly the same manner as in calling free(3).
  *
  * Class:     com_opengamma_maths_mem_Stdlib
  * Method:    wrapped_freeAlignedMemory
  * Signature: (J)I
  *
  * @param env the JNI environment struct.
  * @param clazz (unused reference but required by the API).
  * @param addr the address of the memory to be freed.
  * @return JNI_OK (nothing can be done on failure).
  */
JNIEXPORT jint JNICALL Java_com_opengamma_maths_mem_Stdlib_wrapped_1freeAlignedMemory
(JNIEnv SUPPRESS_UNUSED * env, jclass SUPPRESS_UNUSED clazz, jlong addr)
{
  free_aligned((void *)addr); // nothing we can do if it fails
  return JNI_OK;
}


 /**
  * This function is a JNI binding function for identifying the size of a uintptr_t.
  * 
  * Class:     com_opengamma_maths_mem_Stdlib
  * Method:    wrapped_checkPointerSize
  * Signature: ()I
  * 
  * @param env the JNI environment struct.
  * @param clazz (unused reference but required by the API).
  * @return the size in bytes of a uintptr_t  
  */
JNIEXPORT jint JNICALL Java_com_opengamma_maths_mem_Stdlib_wrapped_1checkPointerSize
(JNIEnv SUPPRESS_UNUSED * env, jclass SUPPRESS_UNUSED clazz)
{
  return (jint)sizeof(uintptr_t);
}

 /**
  * This function is a JNI binding function for incrementing an address assuming the type is uintptr_t.
  * Class:     com_opengamma_maths_mem_Stdlib
  * Method:    wrapped_incrementAddress
  * Signature: (J)L
  * 
  * @param env the JNI environment struct.
  * @param clazz (unused reference but required by the API).
  * @param addr the address to increment.
  * @return {@code addr} incremented assuming the type is unitptr_t.
  */
JNIEXPORT jlong JNICALL Java_com_opengamma_maths_mem_Stdlib_wrapped_1incrementAddress
(JNIEnv SUPPRESS_UNUSED * env, jclass SUPPRESS_UNUSED clazz, jlong addr)
{
  uintptr_t * address = (uintptr_t *)addr;
  return (jlong)++address;
}


#ifdef __cplusplus
}
#endif
