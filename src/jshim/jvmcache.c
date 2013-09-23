/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include "jvmcache.h"
#include "winprint64.h"
#include "debug.h"

#ifdef __cplusplus
extern "C" {
#endif
  JavaVM *JVMcache = NULL;
  jclass OGNumericClazz = NULL;
  jclass OGExprClazz = NULL;
  jclass OGArrayClazz = NULL;
  jclass OGTerminalClazz = NULL;
  jclass OGScalarClazz = NULL;
  jclass OGSparseMatrixClazz = NULL;
  jclass BigDDoubleArrayClazz = NULL;
  jclass ComplexArrayContainerClazz = NULL;
  jmethodID OGTerminalClazz_getData = NULL;
  jmethodID OGNumericClazz_getType = NULL;
  jmethodID OGExprClazz_getExprs = NULL;
  jmethodID OGExprClazz_getNExprs = NULL;
  jmethodID OGArrayClazz_getRows = NULL;
  jmethodID OGArrayClazz_getCols = NULL;
  jmethodID OGSparseMatrixClazz_getColPtr = NULL;
  jmethodID OGSparseMatrixClazz_getRowIdx = NULL;
  jmethodID ComplexArrayContainerClazz_ctor_DAoA_DAoA = NULL;
  jclass OGExprTypeEnumClazz = NULL;
  jfieldID  OGExprTypeEnumClazz__hashdefined = NULL;
#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
extern "C"
#endif
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void __attribute__ ((unused)) *reserved)
{
  DEBUG_PRINT("OnLoad called, caching VM ptr\n");

  JVMcache=jvm; // set the lib level JVM to the on-load JVM ptr
#ifdef __MINGW32__
  unsigned int high SUPPRESS_UNUSED, low SUPPRESS_UNUSED;
  INT64HIGHLOW(JVMcache, high, low);
  DEBUG_PRINT("vm ptr at 0x%x%x\n", high, low);
#else
  DEBUG_PRINT("vm ptr at 0x%llx\n", (long long unsigned int)JVMcache);
#endif
  JNIEnv *env=NULL;
  if ((*jvm)->GetEnv(jvm, (void **)&env, JNI_VERSION_1_2))
  {
    return JNI_ERR; /* JNI version not supported */
  }

  //
  // REGISTER CLASS REFERENCES
  //


  // static cache all the offsets
  jint jstatus = 0;
  jstatus = registerGlobalClassReference(env, "com/opengamma/longdog/datacontainers/OGNumeric", &OGNumericClazz);
  if(jstatus)
  {
    fprintf(stderr, "ERROR: could not get class pointer. Hard exiting.\n");
    exit(1);
  }

  jstatus = registerGlobalClassReference(env, "com/opengamma/longdog/datacontainers/OGTerminal", &OGTerminalClazz);
  if(jstatus)
  {
    fprintf(stderr, "ERROR: could not get class pointer. Hard exiting.\n");
    exit(1);
  }  
  
  jstatus = registerGlobalClassReference(env, "com/opengamma/longdog/datacontainers/matrix/OGArray", &OGArrayClazz);
  if(jstatus)
  {
    fprintf(stderr, "ERROR: could not get class pointer. Hard exiting.\n");
    exit(1);
  }

  jstatus = registerGlobalClassReference(env, "com/opengamma/longdog/datacontainers/ExprTypeEnum", &OGExprTypeEnumClazz);
  if(jstatus)
  {
    fprintf(stderr, "ERROR: could not get class pointer. Hard exiting.\n");
    exit(1);
  }

  jstatus = registerGlobalClassReference(env, "com/opengamma/longdog/datacontainers/matrix/OGSparseMatrix", &OGSparseMatrixClazz);
  if(jstatus)
  {
    fprintf(stderr, "ERROR: could not get class pointer. Hard exiting.\n");
    exit(1);
  }

  jstatus = registerGlobalClassReference(env, "com/opengamma/longdog/datacontainers/lazy/OGExpr", &OGExprClazz);
  if(jstatus)
  {
    fprintf(stderr, "ERROR: could not get class pointer. Hard exiting.\n");
    exit(1);
  }
    
  jstatus = registerGlobalClassReference(env, "[D", &BigDDoubleArrayClazz);
  if(jstatus)
  {
    fprintf(stderr, "ERROR: could not get class pointer. Hard exiting.\n");
    exit(1);
  }
  
  jstatus = registerGlobalClassReference(env, "com/opengamma/longdog/datacontainers/other/ComplexArrayContainer", &ComplexArrayContainerClazz);
  if(jstatus)
  {
    fprintf(stderr, "ERROR: could not get class pointer. Hard exiting.\n");
    exit(1);
  }

  //
  // REGISTER METHOD REFERENCES
  //

  jstatus = registerGlobalMethodReference(env, &OGNumericClazz, &OGNumericClazz_getType, "getType", "()Lcom/opengamma/longdog/datacontainers/ExprTypeEnum;");
  if(jstatus)
  {
    fprintf(stderr, "ERROR: could not get method pointer. Hard exiting.\n");
    exit(1);
  }

  jstatus = registerGlobalMethodReference(env, &OGTerminalClazz, &OGTerminalClazz_getData, "getData",  "()[D");
  if(jstatus)
  {
    fprintf(stderr, "ERROR: could not get method pointer. Hard exiting.\n");
    exit(1);
  }

  jstatus = registerGlobalMethodReference(env, &OGArrayClazz, &OGArrayClazz_getRows, "getRows",  "()I");
  if(jstatus)
  {
    fprintf(stderr, "ERROR: could not get method pointer. Hard exiting.\n");
    exit(1);
  }

  jstatus = registerGlobalMethodReference(env, &OGArrayClazz, &OGArrayClazz_getCols, "getCols",  "()I");
  if(jstatus)
  {
    fprintf(stderr, "ERROR: could not get method pointer. Hard exiting.\n");
    exit(1);
  }

  jstatus = registerGlobalMethodReference(env, &OGSparseMatrixClazz, &OGSparseMatrixClazz_getColPtr, "getColPtr",  "()[I");
  if(jstatus)
  {
    fprintf(stderr, "ERROR: could not get method pointer. Hard exiting.\n");
    exit(1);
  }

  jstatus = registerGlobalMethodReference(env, &OGSparseMatrixClazz, &OGSparseMatrixClazz_getRowIdx, "getRowIdx",  "()[I");
  if(jstatus)
  {
    fprintf(stderr, "ERROR: could not get method pointer. Hard exiting.\n");
    exit(1);
  }

  jstatus = registerGlobalMethodReference(env, &OGExprClazz, &OGExprClazz_getExprs, "getExprs",  "()[Lcom/opengamma/longdog/datacontainers/OGNumeric;");
  if(jstatus)
  {
    fprintf(stderr, "ERROR: could not get method pointer. Hard exiting.\n");
    exit(1);
  }
  
  jstatus = registerGlobalMethodReference(env, &ComplexArrayContainerClazz, &ComplexArrayContainerClazz_ctor_DAoA_DAoA, "<init>","([[D[[D)V");
  if(jstatus)
  {
    fprintf(stderr, "ERROR: could not get method pointer. Hard exiting.\n");
    exit(1);
  }  

  OGExprTypeEnumClazz__hashdefined = NULL;
  OGExprTypeEnumClazz__hashdefined = (*env)->GetFieldID(env, OGExprTypeEnumClazz, "_hashDefined", "J");
  if (OGExprTypeEnumClazz__hashdefined == 0)
  {
    fprintf(stderr, "ERROR: fieldID not found _hashDefined\n");
    exit(1);
  }

  DEBUG_PRINT("Successful JNI Load\n");

  return JNI_VERSION_1_2;
}

#ifdef __cplusplus
extern "C"
#endif
jint registerGlobalMethodReference(JNIEnv * env, jclass * globalRef, jmethodID * methodToSet, const char * methodName, const char * methodSignature)
{
  jmethodID tmp = NULL;
  tmp = (*env)->GetMethodID(env, *globalRef, methodName, methodSignature);
  *methodToSet = tmp;
  if (methodToSet == 0)
  {
    fprintf(stderr, "ERROR: method not found %s()\n",methodName);
    return 1;
  }
  else
  {
#ifdef __MINGW32__
  unsigned int high SUPPRESS_UNUSED, low SUPPRESS_UNUSED;
  INT64HIGHLOW(methodToSet, high, low);
  DEBUG_PRINT("Method found %s() 0x%x%x\n", methodName, high, low);
#else
  DEBUG_PRINT("Method found %s() 0x%llx\n", methodName, (long long unsigned int)methodToSet);
#endif
  }

  return 0;
}

#ifdef __cplusplus
extern "C"
#endif
jint registerGlobalClassReference(JNIEnv * env, const char * FQclassname, jclass * globalRef)
{
  jint jstatus = 0;
  jclass tmpClass = NULL; // tmp class reference
  // find OGNumeric
  tmpClass = NULL;
  tmpClass = (*env)->FindClass(env,FQclassname); // find class
  if(tmpClass==NULL)
  {
    fprintf(stderr, "Cannot find class %s in JNI_OnLoad.\n", FQclassname);
    return 1;
  }

  *globalRef = NULL;
  *globalRef = (jclass) ((*env)->NewGlobalRef(env,tmpClass));
  if(*globalRef==NULL)
  {
    fprintf(stderr, "Cannot create Global reference for %s in JNI_OnLoad.\n",FQclassname);
    return 1;
  }
  return jstatus;
}
