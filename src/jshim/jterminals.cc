/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include <iostream>

#include "jvmmanager.hh"
#include "jterminals.hh"
#include "jbindings.hh"
#include "exceptions.hh"
#include "modifiermacros.h"

using namespace std;

namespace convert {

/**
 * helper function to get size_ts from int getFOO() in java
 */
DLLEXPORT_C size_t getSizeTFromVoidJMethod(jmethodID id, jobject obj)
{
  if (id==nullptr)
  {
    throw convert_error("Null pointer for method id.");
  }
  if (obj==nullptr)
  {
    throw convert_error("Null pointer for jobject obj.");
  }
  
  JNIEnv *env = nullptr;
  JVMManager::getEnv((void **)&env);
  
  if (env==nullptr)
  {
    throw convert_error("Null pointer for env from JVMManager::getEnv.");
  }
  jint data = 0x7ff00000;
  data = env->CallIntMethod(obj, id);
  checkEx(env);
  
  if (data < 0)
  {
    throw convert_error("Negative value in getSizeTFromVoidJMethod");
  }
  
  return static_cast<size_t>(data);
}

/**
 * JOGRealScalar
 */

JOGRealScalar::JOGRealScalar(jobject obj): OGRealScalar(0.0)
{
  // We can't initialise OGRealScalar in its constructor expression list because we will lose the
  // pointer to the raw data in doing that. So we have to first initialise the OGRealScalar to 0.0.

  // Then, we grab the data reference
  _dataRef = bindPrimitiveArrayData<real8, jdoubleArray>(obj, JVMManager::getOGTerminalClazz_getData());

  // So that now we can update the OGRealScalar with the true value.
  _value = static_cast<real8*>(_dataRef)[0];
  this->_backingObject = obj;
}

JOGRealScalar::Ptr
JOGRealScalar::create(jobject obj)
{
  return JOGRealScalar::Ptr{new JOGRealScalar{obj}};
}

JOGRealScalar::~JOGRealScalar()
{
  try {
    unbindOGArrayData<real8, jdoubleArray>(this->_dataRef, _backingObject);
  }
  catch (convert_error e)
  {
    cerr << "Warning (~JOGRealScalar): convert_error thrown when unbinding Java array data" << endl;
  }
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

JOGComplexScalar::Ptr
JOGComplexScalar::create(jobject obj)
{
  return JOGComplexScalar::Ptr{new JOGComplexScalar{obj}};
}



JOGComplexScalar::~JOGComplexScalar()
{
  try {
    unbindOGArrayData<complex16, jdoubleArray>(this->_dataRef, _backingObject);
  }
  catch (convert_error e)
  {
    cerr << "Warning (~JOGComplexScalar): convert_error thrown when unbinding Java array data" << endl;
  }

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
  // We can't initialise OGIntegerScalar in its constructor expression list because we need to
  // get the current env first.
  JNIEnv *env = NULL;
  jint jStatus = 0;
  jStatus=JVMManager::getJVM()->AttachCurrentThread((void **)&env, NULL);  // NOP to get env ptr
  if(jStatus)
  {
    throw convert_error("Thread attach failed");
  }

  // There is no data reference for integer scalars.
  _dataRef = nullptr;


  // Now we can update the OGIntegeRScalar with the true value.
  jmethodID meth = JVMManager::getOGIntegerScalarClazz_getValue();
  _value = env->CallIntMethod(obj, meth);
  checkEx(env);
  this->_backingObject = obj;
}

JOGIntegerScalar::Ptr
JOGIntegerScalar::create(jobject obj)
{
  return JOGIntegerScalar::Ptr{new JOGIntegerScalar{obj}};
}

JOGIntegerScalar::~JOGIntegerScalar()
{
  this->_backingObject = nullptr;
  this->_dataRef = nullptr;
}

void
JOGIntegerScalar::debug_print() const
{
  OGIntegerScalar::debug_print();
}

/**
 * JOGRealDenseMatrix
 */

JOGRealDenseMatrix::JOGRealDenseMatrix(jobject obj): OGRealDenseMatrix
  (
    static_cast<real8 *>(bindPrimitiveArrayData<real8, jdoubleArray>(obj, JVMManager::getOGTerminalClazz_getData())),
    getSizeTFromVoidJMethod(JVMManager::getOGArrayClazz_getRows(), obj),
    getSizeTFromVoidJMethod(JVMManager::getOGArrayClazz_getCols(), obj)
  )
{
  this->_backingObject = obj;
}

JOGRealDenseMatrix::Ptr
JOGRealDenseMatrix::create(jobject obj)
{
  return JOGRealDenseMatrix::Ptr{new JOGRealDenseMatrix{obj}};
}

JOGRealDenseMatrix::~JOGRealDenseMatrix()
{
  try {
    unbindOGArrayData<real8, jdoubleArray>(this->getData(), _backingObject);
  }
  catch (convert_error e)
  {
    cerr << "Warning (~JOGRealDenseMatrix): convert_error thrown when unbinding Java array data" << endl;
  }

  this->_backingObject = nullptr;
}

void
JOGRealDenseMatrix::debug_print() const
{
  OGRealDenseMatrix::debug_print();
}

/*
 * JOGComplexDenseMatrix
 */

JOGComplexDenseMatrix::JOGComplexDenseMatrix(jobject obj): OGComplexDenseMatrix
  (
    static_cast<complex16 *>(bindPrimitiveArrayData<complex16, jdoubleArray>(obj,JVMManager::getOGTerminalClazz_getData())),
    getSizeTFromVoidJMethod(JVMManager::getOGArrayClazz_getRows(), obj),
    getSizeTFromVoidJMethod(JVMManager::getOGArrayClazz_getCols(), obj)
  )
{
  this->_backingObject = obj;
}

JOGComplexDenseMatrix::Ptr
JOGComplexDenseMatrix::create(jobject obj)
{
  return JOGComplexDenseMatrix::Ptr{new JOGComplexDenseMatrix{obj}};
}

JOGComplexDenseMatrix::~JOGComplexDenseMatrix() {
  try {
    unbindOGArrayData<complex16, jdoubleArray>(this->getData(), _backingObject);
  }
  catch (convert_error e)
  {
    cerr << "Warning (~JOGComplexDenseMatrix): convert_error thrown when unbinding Java array data" << endl;
  }

  this->_backingObject = nullptr;
}

void
JOGComplexDenseMatrix::debug_print() const
{
  OGComplexDenseMatrix::debug_print();
}

/**
 * JOGLogicalMatrix
 */

JOGLogicalMatrix::JOGLogicalMatrix(jobject obj): OGLogicalMatrix
  (
    static_cast<real8 *>(bindPrimitiveArrayData<real8, jdoubleArray>(obj, JVMManager::getOGTerminalClazz_getData())),
    getSizeTFromVoidJMethod(JVMManager::getOGArrayClazz_getRows(), obj),
    getSizeTFromVoidJMethod(JVMManager::getOGArrayClazz_getCols(), obj)
  )
{
  this->_backingObject = obj;
}

JOGLogicalMatrix::Ptr
JOGLogicalMatrix::create(jobject obj)
{
  return JOGLogicalMatrix::Ptr{new JOGLogicalMatrix{obj}};
}

JOGLogicalMatrix::~JOGLogicalMatrix()
{
  try {
    unbindOGArrayData<real8, jdoubleArray>(this->getData(), _backingObject);
  }
  catch (convert_error e)
  {
    cerr << "Warning (~JOGLogicalMatrix): convert_error thrown when unbinding Java array data" << endl;
  }

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
    reinterpret_cast<int4*>(bindPrimitiveArrayData<jint, jintArray>(obj, JVMManager::getOGSparseMatrixClazz_getColPtr())),
    reinterpret_cast<int4*>(bindPrimitiveArrayData<jint, jintArray>(obj, JVMManager::getOGSparseMatrixClazz_getRowIdx())),
    static_cast<real8 *>(bindPrimitiveArrayData<real8, jdoubleArray>(obj, JVMManager::getOGTerminalClazz_getData())),
    getSizeTFromVoidJMethod(JVMManager::getOGArrayClazz_getRows(), obj),
    getSizeTFromVoidJMethod(JVMManager::getOGArrayClazz_getCols(), obj)
  )
{
  this->_backingObject = obj;
}

JOGRealSparseMatrix::Ptr
JOGRealSparseMatrix::create(jobject obj)
{
  return JOGRealSparseMatrix::Ptr{new JOGRealSparseMatrix{obj}};
}

JOGRealSparseMatrix::~JOGRealSparseMatrix()
{
  try {
    unbindPrimitiveArrayData<real8, jdoubleArray>(this->getData(), _backingObject, JVMManager::getOGTerminalClazz_getData());
    unbindPrimitiveArrayData<jint, jintArray>(reinterpret_cast<jint*>(this->getRowIdx()), _backingObject, JVMManager::getOGSparseMatrixClazz_getRowIdx());
    unbindPrimitiveArrayData<jint, jintArray>(reinterpret_cast<jint*>(this->getColPtr()), _backingObject, JVMManager::getOGSparseMatrixClazz_getColPtr());
  }
  catch (convert_error e)
  {
    cerr << "Warning (~JOGRealSparseMatrix): convert_error thrown when unbinding Java array data" << endl;
  }

  this->_backingObject = nullptr;
}

void
JOGRealSparseMatrix::debug_print() const
{
  OGRealSparseMatrix::debug_print();
}

real8**
JOGRealSparseMatrix::toReal8ArrayOfArrays() const
{
  throw convert_error("Not implemented");
}

complex16**
JOGRealSparseMatrix::toComplex16ArrayOfArrays() const
{
  throw convert_error("Not implemented");
}

/*
 * JOGComplexSparseMatrix
 */

JOGComplexSparseMatrix::JOGComplexSparseMatrix(jobject obj): OGComplexSparseMatrix
  (
    reinterpret_cast<int4*>(bindPrimitiveArrayData<jint, jintArray>(obj, JVMManager::getOGSparseMatrixClazz_getColPtr())),
    reinterpret_cast<int4*>(bindPrimitiveArrayData<jint, jintArray>(obj, JVMManager::getOGSparseMatrixClazz_getRowIdx())),
    static_cast<complex16 *>(bindPrimitiveArrayData<complex16, jdoubleArray>(obj, JVMManager::getOGTerminalClazz_getData())),
    getSizeTFromVoidJMethod(JVMManager::getOGArrayClazz_getRows(), obj),
    getSizeTFromVoidJMethod(JVMManager::getOGArrayClazz_getCols(), obj)
  )
{
  this->_backingObject = obj;
}

JOGComplexSparseMatrix::Ptr
JOGComplexSparseMatrix::create(jobject obj)
{
  return JOGComplexSparseMatrix::Ptr{new JOGComplexSparseMatrix{obj}};
}

JOGComplexSparseMatrix::~JOGComplexSparseMatrix()
{
  try {
    unbindPrimitiveArrayData<complex16, jdoubleArray>(this->getData(), _backingObject, JVMManager::getOGTerminalClazz_getData());
    unbindPrimitiveArrayData<jint, jintArray>(reinterpret_cast<jint*>(this->getRowIdx()), _backingObject, JVMManager::getOGSparseMatrixClazz_getRowIdx());
    unbindPrimitiveArrayData<jint, jintArray>(reinterpret_cast<jint*>(this->getColPtr()), _backingObject, JVMManager::getOGSparseMatrixClazz_getColPtr());
  }
  catch (convert_error e)
  {
    cerr << "Warning (~JOGComplexSparseMatrix): convert_error thrown when unbinding Java array data" << endl;
  }

  this->_backingObject = nullptr;
}

void
JOGComplexSparseMatrix::debug_print() const
{
  OGComplexSparseMatrix::debug_print();
}

real8**
JOGComplexSparseMatrix::toReal8ArrayOfArrays() const
{
  throw convert_error("Not implemented");
}

complex16**
JOGComplexSparseMatrix::toComplex16ArrayOfArrays() const
{
  throw convert_error("Not implemented");
}

/**
 * JOGRealDiagonalMatrix
 */

JOGRealDiagonalMatrix::JOGRealDiagonalMatrix(jobject obj):OGRealDiagonalMatrix
  (
    static_cast<real8 *>(bindPrimitiveArrayData<real8, jdoubleArray>(obj, JVMManager::getOGTerminalClazz_getData())),
    getSizeTFromVoidJMethod(JVMManager::getOGArrayClazz_getRows(), obj),
    getSizeTFromVoidJMethod(JVMManager::getOGArrayClazz_getCols(), obj)
  )
{
  this->_backingObject = obj;
}

JOGRealDiagonalMatrix::Ptr
JOGRealDiagonalMatrix::create(jobject obj)
{
  return JOGRealDiagonalMatrix::Ptr{new JOGRealDiagonalMatrix{obj}};
}

JOGRealDiagonalMatrix::~JOGRealDiagonalMatrix()
{
  try {
    unbindOGArrayData<real8, jdoubleArray>(this->getData(), _backingObject);
  }
  catch (convert_error e)
  {
    cerr << "Warning (~JOGRealDiagonalMatrix): convert_error thrown when unbinding Java array data" << endl;
  }

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
    getSizeTFromVoidJMethod(JVMManager::getOGArrayClazz_getRows(), obj),
    getSizeTFromVoidJMethod(JVMManager::getOGArrayClazz_getCols(), obj)
  )
{
  this->_backingObject = obj;
}

JOGComplexDiagonalMatrix::Ptr
JOGComplexDiagonalMatrix::create(jobject obj)
{
  return JOGComplexDiagonalMatrix::Ptr{new JOGComplexDiagonalMatrix{obj}};
}

JOGComplexDiagonalMatrix::~JOGComplexDiagonalMatrix()
{
  try {
    unbindOGArrayData<complex16, jdoubleArray>(this->getData(), _backingObject);
  }
  catch (convert_error e)
  {
    cerr << "Warning (~JOGRealDiagonalMatrix): convert_error thrown when unbinding Java array data" << endl;
  }

  this->_backingObject = nullptr;
}

void
JOGComplexDiagonalMatrix::debug_print() const
{
  OGComplexDiagonalMatrix::debug_print();
}

} // namespace convert
