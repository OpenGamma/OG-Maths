/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include <jni.h>
#include "debug.h"
#include "rpath_hack.h"

#ifdef __cplusplus
extern "C" {
#endif

JavaVM *JVMcache; // global JVM pointer cache

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void SUPPRESS_UNUSED *reserved)
{
     DEBUG_PRINT("OnLoad called for rpath_hack\n");
     JVMcache=jvm;

#ifdef __MINGW32__
     rpath_hack();
#endif

     JNIEnv *env=NULL;
     if ((*jvm)->GetEnv(jvm, (void **)&env, JNI_VERSION_1_2)) {
         return JNI_ERR; /* JNI version not supported */
     }
     return JNI_VERSION_1_2;
}

#ifdef __cplusplus
}
#endif