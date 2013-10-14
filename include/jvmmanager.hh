/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#ifndef _JVMMANAGER_HH
#define _JVMMANAGER_HH
#include "modifiermacros.h"

#include "modifiermacros.h"

// if this is defined the user must provide sufficient and suitable structs to 
// fake the parts of the JVM interface used.
#ifdef _FAKE_JNI_H
#include "fake_jni.h"
#else
#include "jni.h"
#endif

namespace convert {

/*
 * Caches static classes methods, fields and the JVM pointer.
 * The JVM Manager gets initialized when JNI_OnLoad() is called.
 */
class JVMManager {
  public:
    DLLEXPORT_C static JavaVM* getJVM();
    DLLEXPORT_C static void initialize(JavaVM* jvm);
    // Classes, methods, and fields
    DLLEXPORT_C static jclass getOGNumericClazz();
    DLLEXPORT_C static jclass getOGExprClazz();
    DLLEXPORT_C static jclass getOGArrayClazz();
    DLLEXPORT_C static jclass getOGTerminalClazz();
    DLLEXPORT_C static jclass getOGScalarClazz();
    DLLEXPORT_C static jclass getOGSparseMatrixClazz();
    DLLEXPORT_C static jclass getBigDDoubleArrayClazz();
    DLLEXPORT_C static jclass getComplexArrayContainerClazz();
    DLLEXPORT_C static jclass getOGExprTypeEnumClazz();
    DLLEXPORT_C static jclass getOGRealScalarClazz();
    DLLEXPORT_C static jclass getOGComplexScalarClazz();
    DLLEXPORT_C static jclass getOGRealDenseMatrixClazz();
    DLLEXPORT_C static jclass getOGComplexDenseMatrixClazz();
    DLLEXPORT_C static jclass getOGRealDiagonalMatrixClazz();
    DLLEXPORT_C static jclass getOGComplexDiagonalMatrixClazz();
    DLLEXPORT_C static jmethodID getOGRealScalarClazz_init();
    DLLEXPORT_C static jmethodID getOGComplexScalarClazz_init();
    DLLEXPORT_C static jmethodID getOGRealDenseMatrixClazz_init();
    DLLEXPORT_C static jmethodID getOGComplexDenseMatrixClazz_init();
    DLLEXPORT_C static jmethodID getOGRealDiagonalMatrixClazz_init();
    DLLEXPORT_C static jmethodID getOGComplexDiagonalMatrixClazz_init();
    DLLEXPORT_C static jmethodID getOGTerminalClazz_getData();
    DLLEXPORT_C static jmethodID getOGNumericClazz_getType();
    DLLEXPORT_C static jmethodID getOGExprClazz_getExprs();
    DLLEXPORT_C static jmethodID getOGExprClazz_getNExprs();
    DLLEXPORT_C static jmethodID getOGArrayClazz_getRows();
    DLLEXPORT_C static jmethodID getOGArrayClazz_getCols();
    DLLEXPORT_C static jmethodID getOGSparseMatrixClazz_getColPtr();
    DLLEXPORT_C static jmethodID getOGSparseMatrixClazz_getRowIdx();
    DLLEXPORT_C static jmethodID getComplexArrayContainerClazz_ctor_DAoA_DAoA();
    DLLEXPORT_C static jfieldID  getOGExprTypeEnumClazz__hashdefined();
    // Wrappers for JNIEnv and JavaVM methods
    DLLEXPORT_C static jobjectArray newObjectArray(JNIEnv *env, jsize len, jclass clazz, jobject init);
    DLLEXPORT_C static jdoubleArray newDoubleArray(JNIEnv *env, jsize len);
    DLLEXPORT_C static jobject newDouble(JNIEnv* env, jdouble v);
    DLLEXPORT_C static void getEnv(void **penv);
    DLLEXPORT_C static jobject callObjectMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...);
  private:
    static void registerReferences();
    static void registerGlobalClassReference(const char* FQclassname, jclass *globalRef);
    static void registerGlobalMethodReference(jclass *globalRef, jmethodID* methodToSet, const char* methodName, const char* methodSignature);
    static void registerGlobalFieldReference(jclass *globalRef, jfieldID* fieldIDToSet, const char* fieldIDName, const char* fieldIDSignature);    
    static JavaVM* _jvm;
    thread_local static JNIEnv* _env;
    // Classes, methods, and fields
    static jclass _DoubleClazz;
    static jclass _OGNumericClazz;
    static jclass _OGExprClazz;
    static jclass _OGArrayClazz;
    static jclass _OGTerminalClazz;
    static jclass _OGScalarClazz;
    static jclass _OGSparseMatrixClazz;
    static jclass _BigDDoubleArrayClazz;
    static jclass _ComplexArrayContainerClazz;
    static jclass _OGExprTypeEnumClazz;
    static jclass _OGRealScalarClazz;
    static jclass _OGComplexScalarClazz;
    static jclass _OGRealDenseMatrixClazz;
    static jclass _OGComplexDenseMatrixClazz;
    static jclass _OGRealDiagonalMatrixClazz;
    static jclass _OGComplexDiagonalMatrixClazz;
    static jmethodID _DoubleClazz_init;
    static jmethodID _OGRealScalarClazz_init;
    static jmethodID _OGComplexScalarClazz_init;
    static jmethodID _OGRealDenseMatrixClazz_init;
    static jmethodID _OGComplexDenseMatrixClazz_init;
    static jmethodID _OGRealDiagonalMatrixClazz_init;
    static jmethodID _OGComplexDiagonalMatrixClazz_init;
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
