/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "debug.h"
#include "warningmacros.h"
#include "com_opengamma_longdog_nativeloader_NativeLibraries.h"
#include "probeinstructionset.h"

#ifdef __cplusplus
extern "C" {
#endif

//
// Helper function.
// Returns the jobject enum associated with a given C land "instructions_available" enum
//
jobject instrEnumAsjobj(JNIEnv * env, jclass containerClass, char * field)
{
  jfieldID jfieldID_enum = (*env)->GetStaticFieldID(env,containerClass,field,"Lcom/opengamma/longdog/nativeloader/SupportedInstructionSet;");
  if(!jfieldID_enum )
  {
    DEBUG_PRINT("Failed to access SupportedInstructionSet %s fieldID!\n",field);
    exit(-1);
  }
  jobject ret = (*env)->GetStaticObjectField(env,containerClass, jfieldID_enum);
  if(!ret )
  {
    DEBUG_PRINT("Failed to get SupportedInstructionSet %s jobject!\n",field);
    exit(-1);
  }
  return ret;
}

/*
 * Class:     com_opengamma_longdog_nativeloader_NativeLibraries
 * Method:    getSupportedInstructionSet
 * Signature: ()Lcom/opengamma/longdog/nativeloader/SupportedInstructionSet;
 */
JNIEXPORT jobject JNICALL Java_com_opengamma_longdog_nativeloader_NativeLibraries_getSupportedInstructionSet
  (JNIEnv * env, jclass SUPPRESS_UNUSED clazz)
{

  jclass jclass_instrEnum = (*env)->FindClass(env,"com/opengamma/longdog/nativeloader/SupportedInstructionSet");
  if(!jclass_instrEnum )
  {
    DEBUG_PRINT("Failed to access SupportedInstructionSet enum!\n");
    exit(-1);
  }

  switch(getSupportedInstructionSet())
  {
    case supports_STANDARD:
      return instrEnumAsjobj(env,jclass_instrEnum,"STANDARD");
    case supports_SSE41:
      return instrEnumAsjobj(env,jclass_instrEnum,"SSE41");
    case supports_SSE42:
      return instrEnumAsjobj(env,jclass_instrEnum,"SSE42");
    case supports_AVX1:
      return instrEnumAsjobj(env,jclass_instrEnum,"AVX1");
    case supports_AVX2:
      return instrEnumAsjobj(env,jclass_instrEnum,"AVX2");
    default:
      DEBUG_PRINT("Failed to find supported instruction set, this is an error!\n");
      exit(-1);
  }
}


// This function attempts to identify the highest possible
// supported instruction set from the sets currently supported
// {standard, SSE42, AVX1, AVX2}.
// It assumes that the CPUID instruction is supported by the CPUID
// although not ideal the assumption is reasonable as the target
// architectures (x86_64 variants) should all have this instruction.
instructions_available getSupportedInstructionSet() {

  // probes of cpuid with %eax=0000_0001h
  enum instructions_available_eax0000_0001h_e
  {
    probe01_SSE_3   = 1<<0,
    probe01_SSE_4_1 = 1<<19,
    probe01_SSE_4_2 = 1<<20,
    probe01_AVX1     = 1<<28
  };

  // probes of cpuid with %eax=0000_0007h
  enum instructions_available_eax0000_0007h_e
  {
    probe07_AVX2     = 1<<5
  };

  // the eax register
  int32_t EAX;
  // contends of the returned register
  int32_t supported;

  // Call cpuid with eax=0x00000001 and get ecx
  EAX = 0x00000001;
  __asm__("cpuid"
        :"=c"(supported)         // %ecx contains large feature flag set
        :"0"(EAX)                // call with 0x1
        :"%eax","%ebx","%edx");  // clobbered

  if(supported & probe01_AVX1) // we have at least AVX1
  {
    EAX = 0x00000007;
    __asm__("cpuid"
          :"=b"(supported)         // %ebx contains feature flag AVX2
          :"0"(EAX)                // call with 0x7
          :"%eax","%ecx","%edx");  // clobbered

    if(supported & probe07_AVX2) // we have at least AVX2
    {
      DEBUG_PRINT("AVX2 SUPPORTED\n");
      return supports_AVX2;
    }
    DEBUG_PRINT("AVX1 SUPPORTED\n");
    return supports_AVX1;
  }
  else if(supported & probe01_SSE_4_2) // we have at least SSE4.2
  {
    DEBUG_PRINT("SSE4.2 SUPPORTED\n");
    return supports_SSE42;
  }
  else if(supported & probe01_SSE_4_1) // we have at least SSE4.1
  {
    DEBUG_PRINT("SSE4.1 SUPPORTED\n");
    return supports_SSE41;
  }
  else // we have nothing specifically useful!
  {
    DEBUG_PRINT("STANDARD SUPPORTED\n");
    return supports_STANDARD;
  }
}


#ifdef __cplusplus
}
#endif