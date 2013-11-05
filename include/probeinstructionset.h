//
// Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
//
// Please see distribution for license.
//
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

// The supported instruction set on this machine for use in other functions
typedef enum instructions_available_e
{
  supports_STANDARD,
  supports_SSE42,
  supports_AVX1,
  supports_AVX2
} instructions_available;

// gets the supported instruction set
instructions_available getSupportedInstructionSet();

// gets the instruction enum as its java equivalent enum object
jobject instrEnumAsjobj(JNIEnv * env, jclass containerClass, char * field);

#ifdef __cplusplus
}
#endif