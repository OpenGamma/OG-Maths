/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "jvmmanager.hh"
#include "jterminals.hh"
#include "jbindings.hh"

namespace convert {

/**
 * helper function to get ints from int getFOO() in java
 */
jint getIntFromVoidJMethod(jmethodID id, jobject obj)
{
  JNIEnv *env = NULL;
  JVMManager::attachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  jint data = 0x7ff00000;
  data = env->CallIntMethod(obj, id);
  return data;
}

/**
 * JOGRealScalar
 */

JOGRealScalar::JOGRealScalar(jobject * obj): OGRealScalar
  (
    static_cast<real16 *>(_dataRef = bindPrimitiveArrayData<real16, jdoubleArray>(*obj, JVMManager::getOGTerminalClazz_getData()))[0]
  )
{
  this->_backingObject = obj;
}

JOGRealScalar::~JOGRealScalar()
{
  unbindOGArrayData<real16>(this->_dataRef, *_backingObject);
  this->_backingObject = nullptr;
  this->_dataRef = nullptr;
}

void
JOGRealScalar::debug_print() const
{
  printf("\nJava bound OGRealScalar\n");
  OGRealScalar::debug_print();
}

/**
 * JOGComplexScalar
 */

JOGComplexScalar::JOGComplexScalar(jobject * obj): OGComplexScalar
  (
    static_cast<complex16 *>(_dataRef = bindPrimitiveArrayData<complex16, jdoubleArray>(*obj, JVMManager::getOGTerminalClazz_getData()))[0]
  )
{
  this->_backingObject = obj;
}

JOGComplexScalar::~JOGComplexScalar()
{
  unbindOGArrayData<complex16>(this->_dataRef, *_backingObject);
  this->_backingObject = nullptr;
  this->_dataRef = nullptr;
}

void
JOGComplexScalar::debug_print() const
{
  printf("\nJava bound JOGComplexScalar\n");
  OGComplexScalar::debug_print();
}

/**
 * JOGRealMatrix
 */

JOGRealMatrix::JOGRealMatrix(jobject * obj): OGRealMatrix
  (
    static_cast<real16 *>(bindPrimitiveArrayData<real16, jdoubleArray>(*obj, JVMManager::getOGTerminalClazz_getData())),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getRows(), *obj)),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getCols(), *obj))
  )
{
  this->_backingObject = obj;
}

JOGRealMatrix::~JOGRealMatrix()
{
  unbindOGArrayData<real16>(this->getData(), *_backingObject);
  this->_backingObject = nullptr;
}

void
JOGRealMatrix::debug_print() const
{
  printf("\nJava bound OGRealMatrix\n");
  OGRealMatrix::debug_print();
}

/*
 * JOGComplexMatrix
 */

JOGComplexMatrix::JOGComplexMatrix(jobject * obj): OGComplexMatrix
  (
    static_cast<complex16 *>(bindPrimitiveArrayData<complex16, jdoubleArray>(*obj,JVMManager::getOGTerminalClazz_getData())),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getRows(), *obj)),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getCols(), *obj))
  )
{
  this->_backingObject = obj;
}

JOGComplexMatrix::~JOGComplexMatrix() {
  unbindOGArrayData<complex16>(this->getData(), *_backingObject);
  this->_backingObject = nullptr;
}

void
JOGComplexMatrix::debug_print() const
{
  printf("\nJava bound OGComplexMatrix\n");
  OGComplexMatrix::debug_print();
}

/*
 * JOGRealSparseMatrix
 */

JOGRealSparseMatrix::JOGRealSparseMatrix(jobject * obj): OGRealSparseMatrix
  (
    static_cast<int*>(bindPrimitiveArrayData<int, jintArray>(*obj, JVMManager::getOGSparseMatrixClazz_getColPtr())),
    static_cast<int*>(bindPrimitiveArrayData<int, jintArray>(*obj, JVMManager::getOGSparseMatrixClazz_getRowIdx())),
    static_cast<real16 *>(bindPrimitiveArrayData<real16, jdoubleArray>(*obj, JVMManager::getOGTerminalClazz_getData())),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getRows(), *obj)),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getCols(), *obj))
  )
{
  this->_backingObject = obj;
}

JOGRealSparseMatrix::~JOGRealSparseMatrix()
{
  unbindPrimitiveArrayData<real16, jdoubleArray>(this->getData(), *_backingObject, JVMManager::getOGTerminalClazz_getData());
  unbindPrimitiveArrayData<int, jintArray>(this->getRowIdx(), *_backingObject, JVMManager::getOGSparseMatrixClazz_getRowIdx());
  unbindPrimitiveArrayData<int, jintArray>(this->getColPtr(), *_backingObject, JVMManager::getOGSparseMatrixClazz_getColPtr());
  this->_backingObject = nullptr;
}

void
JOGRealSparseMatrix::debug_print() const
{
  printf("\nJava bound OGRealSparseMatrix\n");
  OGRealSparseMatrix::debug_print();
}

real16**
JOGRealSparseMatrix::toReal16ArrayOfArrays() const
{
  double ** foo = NULL;
  printf("returning null as no impl yet!!!!\n");
  return foo;
}

complex16**
JOGRealSparseMatrix::toComplex16ArrayOfArrays() const
{
  complex16 ** foo = NULL;
  printf("returning null as no impl yet!!!!\n");
  return foo;
}

/*
 * JOGComplexSparseMatrix
 */

JOGComplexSparseMatrix::JOGComplexSparseMatrix(jobject * obj): OGComplexSparseMatrix
  (
    static_cast<int*>(bindPrimitiveArrayData<int, jintArray>(*obj, JVMManager::getOGSparseMatrixClazz_getColPtr())),
    static_cast<int*>(bindPrimitiveArrayData<int, jintArray>(*obj, JVMManager::getOGSparseMatrixClazz_getRowIdx())),
    static_cast<complex16 *>(bindPrimitiveArrayData<complex16, jdoubleArray>(*obj, JVMManager::getOGTerminalClazz_getData())),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getRows(), *obj)), static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getCols(), *obj))
  )
{
  this->_backingObject = obj;
}

JOGComplexSparseMatrix::~JOGComplexSparseMatrix()
{
  unbindPrimitiveArrayData<complex16, jdoubleArray>(this->getData(), *_backingObject, JVMManager::getOGTerminalClazz_getData());
  unbindPrimitiveArrayData<int, jintArray>(this->getRowIdx(), *_backingObject, JVMManager::getOGSparseMatrixClazz_getRowIdx());
  unbindPrimitiveArrayData<int, jintArray>(this->getColPtr(), *_backingObject, JVMManager::getOGSparseMatrixClazz_getColPtr());
  this->_backingObject = nullptr;
}

void
JOGComplexSparseMatrix::debug_print() const
{
  printf("\nJava bound OGComplexSparseMatrix\n");
  OGComplexSparseMatrix::debug_print();
}

real16**
JOGComplexSparseMatrix::toReal16ArrayOfArrays() const
{
  double ** foo = NULL;
  printf("returning null as no impl yet!!!!\n");
  return foo;
}

complex16**
JOGComplexSparseMatrix::toComplex16ArrayOfArrays() const
{
  complex16 ** foo = NULL;
  printf("returning null as no impl yet!!!!\n");
  return foo;
}

/**
 * JOGRealDiagonalMatrix
 */

JOGRealDiagonalMatrix::JOGRealDiagonalMatrix(jobject * obj):OGRealDiagonalMatrix
  (
    static_cast<real16 *>(bindPrimitiveArrayData<real16, jdoubleArray>(*obj, JVMManager::getOGTerminalClazz_getData())),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getRows(), *obj)),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getCols(), *obj))
  )
{
  this->_backingObject = obj;
}

JOGRealDiagonalMatrix::~JOGRealDiagonalMatrix()
{
  unbindOGArrayData<real16>(this->getData(), *_backingObject);
  this->_backingObject = nullptr;
}

void
JOGRealDiagonalMatrix::debug_print() const
{
  printf("\nJava bound JOGRealDiagonalMatrix\n");
  OGRealDiagonalMatrix::debug_print();
}

/*
 * JOGComplexDiagonalMatrix
 */

JOGComplexDiagonalMatrix::JOGComplexDiagonalMatrix(jobject * obj):OGComplexDiagonalMatrix
  (
    static_cast<complex16 *>(bindPrimitiveArrayData<complex16, jdoubleArray>(*obj, JVMManager::getOGTerminalClazz_getData())),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getRows(), *obj)),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getCols(), *obj))
  )
{
  this->_backingObject = obj;
}

JOGComplexDiagonalMatrix::~JOGComplexDiagonalMatrix()
{
  unbindOGArrayData<complex16>(this->getData(), *_backingObject);
  this->_backingObject = nullptr;
}

void
JOGComplexDiagonalMatrix::debug_print() const
{
  printf("\nJava bound JOGComplexDiagonalMatrix\n");
  OGComplexDiagonalMatrix::debug_print();
}

} // namespace convert
