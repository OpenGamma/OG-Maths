/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "jvmmanager.hh"
#include "jterminals.hh"
#include "jbindings.hh"
#include "exceptions.hh"

namespace convert {

/**
 * helper function to get ints from int getFOO() in java
 */
jint getIntFromVoidJMethod(jmethodID id, jobject obj)
{
  if(id==nullptr)
  {
    throw convert_error("Null pointer for method id.");
  }
  if(obj==nullptr)
  {
    throw convert_error("Null pointer for jobject obj.");
  }
  JNIEnv *env = nullptr;
  JVMManager::getEnv((void **)&env);
  if(env==nullptr)
  {
    throw convert_error("Null pointer for env from JVMManager::getEnv.");
  }
  jint data = 0x7ff00000;
  data = env->CallIntMethod(obj, id);
  return data;
}

/**
 * JOGRealScalar
 */

JOGRealScalar::JOGRealScalar(jobject obj): OGRealScalar(0.0)
{
  // We can't initialise OGRealScalar in its constructor expression list because we will lose the
  // pointer to the raw data in doing that. So we have to first initialise the OGRealScalar to 0.0.

  // Then, we grab the data reference
  _dataRef = bindPrimitiveArrayData<real16, jdoubleArray>(obj, JVMManager::getOGTerminalClazz_getData());

  // So that now we can update the OGRealScalar with the true value.
  _value = static_cast<real16*>(_dataRef)[0];
  this->_backingObject = obj;
}

JOGRealScalar::~JOGRealScalar()
{
  unbindOGArrayData<real16, jdoubleArray>(this->_dataRef, _backingObject);
  this->_backingObject = nullptr;
  this->_dataRef = nullptr;
}

void
JOGRealScalar::debug_print() const
{
  OGRealScalar::debug_print();
}

/**
 * JOGComplexScalar
 */

JOGComplexScalar::JOGComplexScalar(jobject obj): OGComplexScalar(0.0)
{
  // We can't initialise OGComplexScalar in its constructor expression list because we will lose the
  // pointer to the raw data in doing that. So we have to first initialise the OGComplexScalar to 0.0.

  // Then, we grab the data reference
  _dataRef = bindPrimitiveArrayData<complex16, jdoubleArray>(obj, JVMManager::getOGTerminalClazz_getData());

  // So that now we can update the OGComplexScalar with the true value.
  _value = static_cast<complex16 *>(_dataRef)[0];
  this->_backingObject = obj;
}

JOGComplexScalar::~JOGComplexScalar()
{
  unbindOGArrayData<complex16, jdoubleArray>(this->_dataRef, _backingObject);
  this->_backingObject = nullptr;
  this->_dataRef = nullptr;
}

void
JOGComplexScalar::debug_print() const
{
  OGComplexScalar::debug_print();
}

/**
 * JOGIntegerScalar
 */

JOGIntegerScalar::JOGIntegerScalar(jobject obj): OGIntegerScalar(0)
{
  // We can't initialise OGIntegerScalar in its constructor expression list because we will lose the
  // pointer to the raw data in doing that. So we have to first initialise the OGIntegerScalar to 0.

  // Then, we grab the data reference
  _dataRef = bindPrimitiveArrayData<jint, jintArray>(obj, JVMManager::getOGTerminalClazz_getData());

  // So that now we can update the OGIntegeRScalar with the true value.
  _value = reinterpret_cast<int *>(_dataRef)[0];
  this->_backingObject = obj;
}

JOGIntegerScalar::~JOGIntegerScalar()
{
  unbindOGArrayData<jint, jintArray>(this->_dataRef, _backingObject);
  this->_backingObject = nullptr;
  this->_dataRef = nullptr;
}

void
JOGIntegerScalar::debug_print() const
{
  OGIntegerScalar::debug_print();
}

/**
 * JOGRealMatrix
 */

JOGRealMatrix::JOGRealMatrix(jobject obj): OGRealMatrix
  (
    static_cast<real16 *>(bindPrimitiveArrayData<real16, jdoubleArray>(obj, JVMManager::getOGTerminalClazz_getData())),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getRows(), obj)),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getCols(), obj))
  )
{
  this->_backingObject = obj;
}

JOGRealMatrix::~JOGRealMatrix()
{
  unbindOGArrayData<real16, jdoubleArray>(this->getData(), _backingObject);
  this->_backingObject = nullptr;
}

void
JOGRealMatrix::debug_print() const
{
  OGRealMatrix::debug_print();
}

/*
 * JOGComplexMatrix
 */

JOGComplexMatrix::JOGComplexMatrix(jobject obj): OGComplexMatrix
  (
    static_cast<complex16 *>(bindPrimitiveArrayData<complex16, jdoubleArray>(obj,JVMManager::getOGTerminalClazz_getData())),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getRows(), obj)),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getCols(), obj))
  )
{
  this->_backingObject = obj;
}

JOGComplexMatrix::~JOGComplexMatrix() {
  unbindOGArrayData<complex16, jdoubleArray>(this->getData(), _backingObject);
  this->_backingObject = nullptr;
}

void
JOGComplexMatrix::debug_print() const
{
  OGComplexMatrix::debug_print();
}

/**
 * JOGLogicalMatrix
 */

JOGLogicalMatrix::JOGLogicalMatrix(jobject obj): OGLogicalMatrix
  (
    static_cast<real16 *>(bindPrimitiveArrayData<real16, jdoubleArray>(obj, JVMManager::getOGTerminalClazz_getData())),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getRows(), obj)),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getCols(), obj))
  )
{
  this->_backingObject = obj;
}

JOGLogicalMatrix::~JOGLogicalMatrix()
{
  unbindOGArrayData<real16, jdoubleArray>(this->getData(), _backingObject);
  this->_backingObject = nullptr;
}

void
JOGLogicalMatrix::debug_print() const
{
  OGLogicalMatrix::debug_print();
}

/*
 * JOGRealSparseMatrix
 */

JOGRealSparseMatrix::JOGRealSparseMatrix(jobject obj): OGRealSparseMatrix
  (
    reinterpret_cast<int*>(bindPrimitiveArrayData<jint, jintArray>(obj, JVMManager::getOGSparseMatrixClazz_getColPtr())),
    reinterpret_cast<int*>(bindPrimitiveArrayData<jint, jintArray>(obj, JVMManager::getOGSparseMatrixClazz_getRowIdx())),
    static_cast<real16 *>(bindPrimitiveArrayData<real16, jdoubleArray>(obj, JVMManager::getOGTerminalClazz_getData())),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getRows(), obj)),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getCols(), obj))
  )
{
  this->_backingObject = obj;
}

JOGRealSparseMatrix::~JOGRealSparseMatrix()
{
  unbindPrimitiveArrayData<real16, jdoubleArray>(this->getData(), _backingObject, JVMManager::getOGTerminalClazz_getData());
  unbindPrimitiveArrayData<jint, jintArray>(reinterpret_cast<jint*>(this->getRowIdx()), _backingObject, JVMManager::getOGSparseMatrixClazz_getRowIdx());
  unbindPrimitiveArrayData<jint, jintArray>(reinterpret_cast<jint*>(this->getColPtr()), _backingObject, JVMManager::getOGSparseMatrixClazz_getColPtr());
  this->_backingObject = nullptr;
}

void
JOGRealSparseMatrix::debug_print() const
{
  OGRealSparseMatrix::debug_print();
}

real16**
JOGRealSparseMatrix::toReal16ArrayOfArrays() const
{
  // Returning null as no implementation yet.
  double ** foo = NULL;
  return foo;
}

complex16**
JOGRealSparseMatrix::toComplex16ArrayOfArrays() const
{
  // Returning null as no implementation yet.
  complex16 ** foo = NULL;
  return foo;
}

/*
 * JOGComplexSparseMatrix
 */

JOGComplexSparseMatrix::JOGComplexSparseMatrix(jobject obj): OGComplexSparseMatrix
  (
    reinterpret_cast<int*>(bindPrimitiveArrayData<jint, jintArray>(obj, JVMManager::getOGSparseMatrixClazz_getColPtr())),
    reinterpret_cast<int*>(bindPrimitiveArrayData<jint, jintArray>(obj, JVMManager::getOGSparseMatrixClazz_getRowIdx())),
    static_cast<complex16 *>(bindPrimitiveArrayData<complex16, jdoubleArray>(obj, JVMManager::getOGTerminalClazz_getData())),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getRows(), obj)), static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getCols(), obj))
  )
{
  this->_backingObject = obj;
}

JOGComplexSparseMatrix::~JOGComplexSparseMatrix()
{
  unbindPrimitiveArrayData<complex16, jdoubleArray>(this->getData(), _backingObject, JVMManager::getOGTerminalClazz_getData());
  unbindPrimitiveArrayData<jint, jintArray>(reinterpret_cast<jint*>(this->getRowIdx()), _backingObject, JVMManager::getOGSparseMatrixClazz_getRowIdx());
  unbindPrimitiveArrayData<jint, jintArray>(reinterpret_cast<jint*>(this->getColPtr()), _backingObject, JVMManager::getOGSparseMatrixClazz_getColPtr());
  this->_backingObject = nullptr;
}

void
JOGComplexSparseMatrix::debug_print() const
{
  OGComplexSparseMatrix::debug_print();
}

real16**
JOGComplexSparseMatrix::toReal16ArrayOfArrays() const
{
  // Returning null as no implementation yet.
  double ** foo = NULL;
  return foo;
}

complex16**
JOGComplexSparseMatrix::toComplex16ArrayOfArrays() const
{
  // Returning null as no implementation yet.
  complex16 ** foo = NULL;
  return foo;
}

/**
 * JOGRealDiagonalMatrix
 */

JOGRealDiagonalMatrix::JOGRealDiagonalMatrix(jobject obj):OGRealDiagonalMatrix
  (
    static_cast<real16 *>(bindPrimitiveArrayData<real16, jdoubleArray>(obj, JVMManager::getOGTerminalClazz_getData())),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getRows(), obj)),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getCols(), obj))
  )
{
  this->_backingObject = obj;
}

JOGRealDiagonalMatrix::~JOGRealDiagonalMatrix()
{
  unbindOGArrayData<real16, jdoubleArray>(this->getData(), _backingObject);
  this->_backingObject = nullptr;
}

void
JOGRealDiagonalMatrix::debug_print() const
{
  OGRealDiagonalMatrix::debug_print();
}

/*
 * JOGComplexDiagonalMatrix
 */

JOGComplexDiagonalMatrix::JOGComplexDiagonalMatrix(jobject obj):OGComplexDiagonalMatrix
  (
    static_cast<complex16 *>(bindPrimitiveArrayData<complex16, jdoubleArray>(obj, JVMManager::getOGTerminalClazz_getData())),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getRows(), obj)),
    static_cast<int>(getIntFromVoidJMethod(JVMManager::getOGArrayClazz_getCols(), obj))
  )
{
  this->_backingObject = obj;
}

JOGComplexDiagonalMatrix::~JOGComplexDiagonalMatrix()
{
  unbindOGArrayData<complex16, jdoubleArray>(this->getData(), _backingObject);
  this->_backingObject = nullptr;
}

void
JOGComplexDiagonalMatrix::debug_print() const
{
  OGComplexDiagonalMatrix::debug_print();
}

} // namespace convert
