/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "jvmmanager.hh"
#include "exceptions.hh"
#include "debug.h"
#include <iostream>

namespace convert {

/**
 * JNI_OnLoad
 */

#ifdef __cplusplus
extern "C"
#endif
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void __attribute__ ((unused)) *reserved)
{
  DEBUG_PRINT("JNI_OnLoad called, initializing JVMManager\n");
  try {
    JVMManager::initialize(jvm);
  }
  catch (convert_error e)
  {
    DEBUG_PRINT("Exception in JNI_OnLoad: %s\n.", e.what());
    return JNI_ERR;
  }
  DEBUG_PRINT("Successfully initialized JVMManager in JNI_OnLoad\n");
  return JNI_VERSION_1_2;
}

/**
 * JVMManager
 */

void
JVMManager::initialize(JavaVM *jvm)
{
  VAL64BIT_PRINT("VM pointer", jvm);

  // Attempt to get env for JNI version 1.2
  int ret = jvm->GetEnv((void **)&_env, JNI_VERSION_1_2);
  if (ret != JNI_OK)
  {
    throw convert_error("Error getting JNI environment.");
  }

  // Set up cached pointers
  _jvm = jvm;
  registerReferences();
}

void
JVMManager::registerReferences()
{
  //
  // REGISTER CLASS REFERENCES
  //

  registerGlobalClassReference("java/lang/Double", &_DoubleClazz);
  registerGlobalClassReference("com/opengamma/longdog/datacontainers/OGNumeric", &_OGNumericClazz);
  registerGlobalClassReference("com/opengamma/longdog/datacontainers/OGTerminal", &_OGTerminalClazz);
  registerGlobalClassReference("com/opengamma/longdog/datacontainers/matrix/OGArray", &_OGArrayClazz);
  registerGlobalClassReference("com/opengamma/longdog/datacontainers/ExprTypeEnum", &_OGExprTypeEnumClazz);
  registerGlobalClassReference("com/opengamma/longdog/datacontainers/matrix/OGSparseMatrix", &_OGSparseMatrixClazz);
  registerGlobalClassReference("com/opengamma/longdog/datacontainers/scalar/OGScalar", &_OGScalarClazz);  
  registerGlobalClassReference("com/opengamma/longdog/datacontainers/lazy/OGExpr", &_OGExprClazz);
  registerGlobalClassReference("[D", &_BigDDoubleArrayClazz);
  registerGlobalClassReference("com/opengamma/longdog/datacontainers/other/ComplexArrayContainer", &_ComplexArrayContainerClazz);


  registerGlobalClassReference("com/opengamma/longdog/datacontainers/scalar/OGRealScalar", &_OGRealScalarClazz);
  registerGlobalClassReference("com/opengamma/longdog/datacontainers/scalar/OGComplexScalar", &_OGComplexScalarClazz);
  registerGlobalClassReference("com/opengamma/longdog/datacontainers/matrix/OGRealDenseMatrix", &_OGRealDenseMatrixClazz);
  registerGlobalClassReference("com/opengamma/longdog/datacontainers/matrix/OGComplexDenseMatrix", &_OGComplexDenseMatrixClazz);
  registerGlobalClassReference("com/opengamma/longdog/datacontainers/matrix/OGRealDiagonalMatrix", &_OGRealDiagonalMatrixClazz);
  registerGlobalClassReference("com/opengamma/longdog/datacontainers/matrix/OGComplexDiagonalMatrix", &_OGComplexDiagonalMatrixClazz);

  //
  // REGISTER METHOD REFERENCES
  //

  registerGlobalMethodReference(&_DoubleClazz, &_DoubleClazz_init, "<init>", "(D)V");
  registerGlobalMethodReference(&_OGNumericClazz, &_OGNumericClazz_getType, "getType", "()Lcom/opengamma/longdog/datacontainers/ExprTypeEnum;");
  registerGlobalMethodReference(&_OGTerminalClazz, &_OGTerminalClazz_getData, "getData",  "()[D");
  registerGlobalMethodReference(&_OGArrayClazz, &_OGArrayClazz_getRows, "getRows",  "()I");
  registerGlobalMethodReference(&_OGArrayClazz, &_OGArrayClazz_getCols, "getCols",  "()I");
  registerGlobalMethodReference(&_OGSparseMatrixClazz, &_OGSparseMatrixClazz_getColPtr, "getColPtr",  "()[I");
  registerGlobalMethodReference(&_OGSparseMatrixClazz, &_OGSparseMatrixClazz_getRowIdx, "getRowIdx",  "()[I");
  registerGlobalMethodReference(&_OGExprClazz, &_OGExprClazz_getExprs, "getExprs",  "()[Lcom/opengamma/longdog/datacontainers/OGNumeric;");
  registerGlobalMethodReference(&_OGExprClazz, &_OGExprClazz_getNExprs, "getNExprs",  "()I");  
  registerGlobalMethodReference(&_ComplexArrayContainerClazz, &_ComplexArrayContainerClazz_ctor_DAoA_DAoA, "<init>","([[D[[D)V");
  registerGlobalMethodReference(&_OGRealScalarClazz, &_OGRealScalarClazz_init, "<init>", "(Ljava/lang/Number;)V");
  registerGlobalMethodReference(&_OGComplexScalarClazz, &_OGComplexScalarClazz_init, "<init>", "(Ljava/lang/Number;Ljava/lang/Number;)V");
  registerGlobalMethodReference(&_OGRealDenseMatrixClazz, &_OGRealDenseMatrixClazz_init, "<init>", "([[D)V");
  registerGlobalMethodReference(&_OGComplexDenseMatrixClazz, &_OGComplexDenseMatrixClazz_init, "<init>", "([[D[[D)V");
  registerGlobalMethodReference(&_OGRealDiagonalMatrixClazz, &_OGRealDiagonalMatrixClazz_init, "<init>", "([DII)V");
  registerGlobalMethodReference(&_OGComplexDiagonalMatrixClazz, &_OGComplexDiagonalMatrixClazz_init, "<init>", "([D[DII)V");

  //
  // REGISTER FIELD REFERENCES
  //

  registerGlobalFieldReference(&_OGExprTypeEnumClazz, &_OGExprTypeEnumClazz__hashdefined, "_hashDefined", "J");
  
}

JavaVM*
JVMManager::getJVM()
{
  return _jvm;
}

void 
JVMManager::registerGlobalFieldReference(jclass * globalRef, jfieldID * fieldToSet, const char * fieldName, const char * fieldSignature)
{
  jfieldID tmp = nullptr;
  tmp = _env->GetFieldID(*globalRef, fieldName, fieldSignature);
  if (tmp == nullptr)
  {
    DEBUG_PRINT("ERROR: field %s() not found.\n",fieldName);
    throw convert_error("field not found");
  }
  else
  {
    *fieldToSet = tmp;    
    DEBUG_PRINT("Field found: %s()\n\t", fieldName);
    VAL64BIT_PRINT("field pointer", fieldToSet);
  }
}

void
JVMManager::registerGlobalMethodReference(jclass * globalRef, jmethodID * methodToSet, const char * methodName, const char * methodSignature)
{
  jmethodID tmp = nullptr;
  tmp = _env->GetMethodID(*globalRef, methodName, methodSignature);
  if (tmp == nullptr)
  {
    DEBUG_PRINT("ERROR: method %s() not found.\n",methodName);
    throw convert_error("Method not found");
  }
  else
  {
    *methodToSet = tmp;
    DEBUG_PRINT("Method found: %s()\n\t", methodName);
    VAL64BIT_PRINT("method pointer", methodToSet);
  }
}

void
JVMManager::registerGlobalClassReference(const char * FQclassname, jclass * globalRef)
{
  jclass tmpClass = nullptr; // tmp class reference
  // find OGNumeric
  tmpClass = _env->FindClass(FQclassname); // find class
  if(tmpClass==nullptr)
  {
    DEBUG_PRINT("Cannot find class %s in JNI_OnLoad.\n", FQclassname);
    throw convert_error("Class not found.");
  }

  *globalRef = nullptr;
  *globalRef = (jclass) (_env->NewGlobalRef(tmpClass));
  if(*globalRef==nullptr)
  {
    DEBUG_PRINT("Cannot create Global reference for %s.\n",FQclassname);
    throw convert_error("Cannot create global reference.");
  }
}

/**
 * JVMManager accessors
 */

jclass JVMManager::getOGNumericClazz()
{ return _OGNumericClazz; }
jclass JVMManager::getOGExprClazz()
{ return _OGExprClazz; }
jclass JVMManager::getOGArrayClazz()
{ return _OGArrayClazz; }
jclass JVMManager::getOGTerminalClazz()
{ return _OGTerminalClazz; }
jclass JVMManager::getOGScalarClazz()
{ return _OGScalarClazz; }
jclass JVMManager::getOGSparseMatrixClazz()
{ return _OGSparseMatrixClazz; }
jclass JVMManager::getBigDDoubleArrayClazz()
{ return _BigDDoubleArrayClazz; }
jclass JVMManager::getComplexArrayContainerClazz()
{ return _ComplexArrayContainerClazz; }
jclass JVMManager::getOGRealScalarClazz()
{ return _OGRealScalarClazz; }
jclass JVMManager::getOGComplexScalarClazz()
{ return _OGComplexScalarClazz; }
jclass JVMManager::getOGRealDenseMatrixClazz()
{ return _OGRealDenseMatrixClazz; }
jclass JVMManager::getOGComplexDenseMatrixClazz()
{ return _OGComplexDenseMatrixClazz; }
jclass JVMManager::getOGRealDiagonalMatrixClazz()
{ return _OGRealDiagonalMatrixClazz; }
jclass JVMManager::getOGComplexDiagonalMatrixClazz()
{ return _OGComplexDiagonalMatrixClazz; }
jclass JVMManager::getOGExprTypeEnumClazz()
{ return _OGExprTypeEnumClazz; }
jmethodID JVMManager::getOGRealScalarClazz_init()
{ return _OGRealScalarClazz_init; }
jmethodID JVMManager::getOGComplexScalarClazz_init()
{ return _OGComplexScalarClazz_init; }
jmethodID JVMManager::getOGRealDenseMatrixClazz_init()
{ return _OGRealDenseMatrixClazz_init; }
jmethodID JVMManager::getOGComplexDenseMatrixClazz_init()
{ return _OGComplexDenseMatrixClazz_init; }
jmethodID JVMManager::getOGRealDiagonalMatrixClazz_init()
{ return _OGRealDiagonalMatrixClazz_init; }
jmethodID JVMManager::getOGComplexDiagonalMatrixClazz_init()
{ return _OGComplexDiagonalMatrixClazz_init; }
jmethodID JVMManager::getOGTerminalClazz_getData()
{ return _OGTerminalClazz_getData; }
jmethodID JVMManager::getOGNumericClazz_getType()
{ return _OGNumericClazz_getType; }
jmethodID JVMManager::getOGExprClazz_getExprs()
{ return _OGExprClazz_getExprs; }
jmethodID JVMManager::getOGExprClazz_getNExprs()
{ return _OGExprClazz_getNExprs; }
jmethodID JVMManager::getOGArrayClazz_getRows()
{ return _OGArrayClazz_getRows; }
jmethodID JVMManager::getOGArrayClazz_getCols()
{ return _OGArrayClazz_getCols; }
jmethodID JVMManager::getOGSparseMatrixClazz_getColPtr()
{ return _OGSparseMatrixClazz_getColPtr; }
jmethodID JVMManager::getOGSparseMatrixClazz_getRowIdx()
{ return _OGSparseMatrixClazz_getRowIdx; }
jmethodID JVMManager::getComplexArrayContainerClazz_ctor_DAoA_DAoA()
{ return _ComplexArrayContainerClazz_ctor_DAoA_DAoA; }
jfieldID JVMManager:: getOGExprTypeEnumClazz__hashdefined()
{ return _OGExprTypeEnumClazz__hashdefined; }


// Instantiation of JVMManager's fields

JavaVM* JVMManager::_jvm = nullptr;
JNIEnv* JVMManager::_env = nullptr;
jclass JVMManager::_DoubleClazz = nullptr;
jclass JVMManager::_OGNumericClazz = nullptr;
jclass JVMManager::_OGExprClazz = nullptr;
jclass JVMManager::_OGArrayClazz = nullptr;
jclass JVMManager::_OGTerminalClazz = nullptr;
jclass JVMManager::_OGScalarClazz = nullptr;
jclass JVMManager::_OGSparseMatrixClazz = nullptr;
jclass JVMManager::_BigDDoubleArrayClazz = nullptr;
jclass JVMManager::_ComplexArrayContainerClazz = nullptr;
jclass JVMManager::_OGExprTypeEnumClazz = nullptr;
jclass JVMManager::_OGRealScalarClazz = nullptr;
jclass JVMManager::_OGComplexScalarClazz= nullptr;
jclass JVMManager::_OGRealDenseMatrixClazz = nullptr;
jclass JVMManager::_OGComplexDenseMatrixClazz = nullptr;
jclass JVMManager::_OGRealDiagonalMatrixClazz = nullptr;
jclass JVMManager::_OGComplexDiagonalMatrixClazz = nullptr;
jmethodID JVMManager::_DoubleClazz_init = nullptr;
jmethodID JVMManager::_OGRealScalarClazz_init = nullptr;
jmethodID JVMManager::_OGComplexScalarClazz_init = nullptr;
jmethodID JVMManager::_OGRealDenseMatrixClazz_init = nullptr;
jmethodID JVMManager::_OGComplexDenseMatrixClazz_init = nullptr;
jmethodID JVMManager::_OGRealDiagonalMatrixClazz_init = nullptr;
jmethodID JVMManager::_OGComplexDiagonalMatrixClazz_init = nullptr;
jmethodID JVMManager::_OGTerminalClazz_getData = nullptr;
jmethodID JVMManager::_OGNumericClazz_getType = nullptr;
jmethodID JVMManager::_OGExprClazz_getExprs = nullptr;
jmethodID JVMManager::_OGExprClazz_getNExprs = nullptr;
jmethodID JVMManager::_OGArrayClazz_getRows = nullptr;
jmethodID JVMManager::_OGArrayClazz_getCols = nullptr;
jmethodID JVMManager::_OGSparseMatrixClazz_getColPtr = nullptr;
jmethodID JVMManager::_OGSparseMatrixClazz_getRowIdx = nullptr;
jmethodID JVMManager::_ComplexArrayContainerClazz_ctor_DAoA_DAoA = nullptr;
jfieldID  JVMManager::_OGExprTypeEnumClazz__hashdefined = nullptr;


// Wrappers to JavaVM and JNIEnv methods


jobjectArray
JVMManager::newObjectArray(JNIEnv *env, jsize len, jclass clazz, jobject init)
{
  jobjectArray ret = env->NewObjectArray(len, clazz, init);
  if (!ret)
  {
    throw convert_error("NewObjectArray call failed.");
  }
  return ret;
}

jdoubleArray
JVMManager::newDoubleArray(JNIEnv *env, jsize len)
{
  jdoubleArray ret = env->NewDoubleArray(len);
  if (!ret)
  {
    throw convert_error("NewDoubleArray call failed.");
  }
  return ret;
}

void
JVMManager::getEnv(void **penv)
{
  jint status = _jvm->AttachCurrentThread(penv, nullptr);
  if (status)
  {
    throw convert_error("Error attaching current thread.");
  }
}

jobject
JVMManager::callObjectMethod(JNIEnv *env, jobject obj, jmethodID methodID, ...)
{
  va_list args;
  va_start(args, methodID);
  jobject dataobj = env->CallObjectMethod(obj, methodID, args);
  va_end(args);
  if (!dataobj)
  {
    throw convert_error("CallObjectMethod failed.");
  }
  return dataobj;
}

/*
 * Make a new double object because there's no autoboxing in JNI.
 */

jobject
JVMManager::newDouble(JNIEnv* env, jdouble v)
{
  return env->NewObject(_DoubleClazz, _DoubleClazz_init, v);
}

} // namespace convert
