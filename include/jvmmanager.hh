/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _JVMMANAGER_HH
#define _JVMMANAGER_HH

#include "jni.h"

namespace convert {

/*
 * Caches static classes methods, fields and the JVM pointer.
 * The JVM Manager gets initialized when JNI_OnLoad() is called.
 */
class JVMManager {
  public:
    static JavaVM* getJVM();
    static void initialize(JavaVM* jvm);
    // Classes, methods, and fields
    static jclass getOGNumericClazz();
    static jclass getOGExprClazz();
    static jclass getOGArrayClazz();
    static jclass getOGTerminalClazz();
    static jclass getOGScalarClazz();
    static jclass getOGSparseMatrixClazz();
    static jclass getBigDDoubleArrayClazz();
    static jclass getComplexArrayContainerClazz();
    static jclass getOGExprTypeEnumClazz();
    static jmethodID getOGTerminalClazz_getData();
    static jmethodID getOGNumericClazz_getType();
    static jmethodID getOGExprClazz_getExprs();
    static jmethodID getOGExprClazz_getNExprs();
    static jmethodID getOGArrayClazz_getRows();
    static jmethodID getOGArrayClazz_getCols();
    static jmethodID getOGSparseMatrixClazz_getColPtr();
    static jmethodID getOGSparseMatrixClazz_getRowIdx();
    static jmethodID getComplexArrayContainerClazz_ctor_DAoA_DAoA();
    static jfieldID  getOGExprTypeEnumClazz__hashdefined();
    // Wrappers for JNIEnv and JavaVM methods
    static jobjectArray newObjectArray(JNIEnv *env, jsize len, jclass clazz, jobject init);
    static jdoubleArray newDoubleArray(JNIEnv *env, jsize len);
    static void attachCurrentThread(void **penv, void *args);
    static jobject callObjectMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...);
  private:
    static void registerReferences();
    static void registerGlobalMethodReference(jclass *globalRef, jmethodID* methodToSet, const char* methodName, const char* methodSignature);
    static void registerGlobalClassReference(const char* FQclassname, jclass *globalRef);
    static JavaVM* _jvm;
    static JNIEnv* _env;
    // Classes, methods, and fields
    static jclass _OGNumericClazz;
    static jclass _OGExprClazz;
    static jclass _OGArrayClazz;
    static jclass _OGTerminalClazz;
    static jclass _OGScalarClazz;
    static jclass _OGSparseMatrixClazz;
    static jclass _BigDDoubleArrayClazz;
    static jclass _ComplexArrayContainerClazz;
    static jclass _OGExprTypeEnumClazz;
    static jmethodID _OGTerminalClazz_getData;
    static jmethodID _OGNumericClazz_getType;
    static jmethodID _OGExprClazz_getExprs;
    static jmethodID _OGExprClazz_getNExprs;
    static jmethodID _OGArrayClazz_getRows;
    static jmethodID _OGArrayClazz_getCols;
    static jmethodID _OGSparseMatrixClazz_getColPtr;
    static jmethodID _OGSparseMatrixClazz_getRowIdx;
    static jmethodID _ComplexArrayContainerClazz_ctor_DAoA_DAoA;
    static jfieldID  _OGExprTypeEnumClazz__hashdefined;
};

} // namespace convert

#endif // _JVMMANAGER_HH
