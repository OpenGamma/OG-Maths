/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "jdispatch.hh"
#include "visitor.hh"
#include "warningmacros.h"
#include "terminal.hh"
#include "expression.hh"
#include "debug.h"

namespace convert
{

using namespace librdag;
using namespace std;

/**
 * Real8AoA
 */

Real8AoA::Real8AoA(const OGNumeric::Ptr& node)
{
  ExprType_t type = node->getType();
  switch(type)
  {
  case REAL_SCALAR_ENUM:
    _data = node->asOGTerminal()->toReal8ArrayOfArrays();
    _rows = 1;
    _cols = 1;
    break;
  case REAL_MATRIX_ENUM:
  case REAL_DIAGONAL_MATRIX_ENUM:
  case REAL_SPARSE_MATRIX_ENUM:
    _data = node->asOGTerminal()->toReal8ArrayOfArrays();
    _rows = node->asOGTerminal()->getRows();
    _cols = node->asOGTerminal()->getCols();
    break;
  default:
    stringstream message;
    message << "Unsupported type for Real8AoA. Type is " << type << ".";
    throw convert_error(message.str());
  }
}

Real8AoA::Real8AoA(Real8AoA&& o): _data{o._data}, _rows{o._rows}, _cols{o._cols}
{
  o._data = nullptr;
}

Real8AoA&
Real8AoA::operator=(Real8AoA&& o)
{
  _data = o._data;
  o._data = nullptr;
  _rows = o._rows;
  _cols = o._cols;
  return *this;
}

Real8AoA::~Real8AoA()
{
  if (_data != nullptr)
  {
    for(size_t i = 0; i < _rows; i++)
    {
        delete[] _data[i];
    }
    delete[] _data;
  }
}

jobjectArray
Real8AoA::toJDoubleAoA(JNIEnv* env) const
{
 jobjectArray returnVal = JVMManager::newObjectArray(env, _rows, JVMManager::getBigDDoubleArrayClazz(), NULL);
  for(size_t i = 0; i < _rows; i++)
  {
    jdoubleArray tmp = JVMManager::newDoubleArray(env, _cols);
    env->SetDoubleArrayRegion(tmp, 0, _cols, _data[i]);
    env->SetObjectArrayElement(returnVal, i, tmp);
  }
  return returnVal;
}


real8**
Real8AoA::getData() const
{
  return _data;
}

size_t
Real8AoA::getRows() const
{
  return _rows;
}

size_t
Real8AoA::getCols() const
{
  return _cols;
}

/**
 * Complex16AoA
 */

Complex16AoA::Complex16AoA(const OGNumeric::Ptr& node)
{
  ExprType_t type = node->getType();
  switch(type)
  {
  case REAL_SCALAR_ENUM:
  case COMPLEX_SCALAR_ENUM:
    _data = node->asOGTerminal()->toComplex16ArrayOfArrays();
    _rows = 1;
    _cols = 1;
    break;
  case REAL_MATRIX_ENUM:
  case COMPLEX_MATRIX_ENUM:
  case COMPLEX_DIAGONAL_MATRIX_ENUM:
  case COMPLEX_SPARSE_MATRIX_ENUM:
    _data = node->asOGTerminal()->toComplex16ArrayOfArrays();
    _rows = node->asOGTerminal()->getRows();
    _cols = node->asOGTerminal()->getCols();
    break;
  default:
    stringstream message;
    message << "Unsupported type for Complex16AoA. Type is " << type << ".";
    throw convert_error(message.str());
  }
}

Complex16AoA::Complex16AoA(Complex16AoA&& o): _data{o._data}, _rows{o._rows}, _cols{o._cols}
{
  o._data = nullptr;
}

Complex16AoA&
Complex16AoA::operator=(Complex16AoA&& o)
{
  _data = o._data;
  o._data = nullptr;
  _rows = o._rows;
  _cols = o._cols;
  return *this;
}

Complex16AoA::~Complex16AoA()
{
  if (_data != nullptr)
  {
    for(size_t i = 0; i < _rows; i++)
    {
        delete[] _data[i];
    }
    delete[] _data;
  }
}

jobjectArray
Complex16AoA::realPartToJDoubleAoA(JNIEnv* env) const
{
  return toJDoubleAoA<std::real>(env);
}

jobjectArray
Complex16AoA::imagPartToJDoubleAoA(JNIEnv* env) const
{
  return toJDoubleAoA<std::imag>(env);
}

template<real8 (F)(const complex16&)>
jobjectArray
Complex16AoA::toJDoubleAoA(JNIEnv* env) const
{
  jobjectArray returnVal = JVMManager::newObjectArray(env, _rows, JVMManager::getBigDDoubleArrayClazz(), NULL);
  real8 * aRow = new real8[_cols];
  for(size_t i = 0; i < _rows; i++)
  {
    jdoubleArray tmp = JVMManager::newDoubleArray(env, _cols);
    for(size_t j = 0; j < _cols; j++)
    {
      aRow[j] = F(_data[i][j]);
    }
    env->SetDoubleArrayRegion(tmp, 0, _cols, aRow);
    env->SetObjectArrayElement(returnVal, i, tmp);
  }
  delete aRow;
  return returnVal;
}

complex16**
Complex16AoA::getData() const
{
  return _data;
}

size_t
Complex16AoA::getRows() const
{
  return _rows;
}

size_t
Complex16AoA::getCols() const
{
  return _cols;
}

/**
 * JavaTerminal
 */

JavaTerminal::JavaTerminal(JNIEnv* env, const OGNumeric::Ptr& node)
{
  ExprType_t type = node->getType();
  switch(type)
  {
  case REAL_SCALAR_ENUM:
    createRealScalar(env, node);
    break;
  case COMPLEX_SCALAR_ENUM:
    createComplexScalar(env, node);
    break;
  case REAL_MATRIX_ENUM:
    createRealMatrix(env, node);
    break;
  case COMPLEX_MATRIX_ENUM:
    createComplexMatrix(env, node);
    break;
  case REAL_DIAGONAL_MATRIX_ENUM:
    createRealDiagonalMatrix(env, node);
    break;
  case COMPLEX_DIAGONAL_MATRIX_ENUM:
    createComplexDiagonalMatrix(env, node);
    break;
  case REAL_SPARSE_MATRIX_ENUM:
    createRealSparseMatrix(env, node);
    break;
  case COMPLEX_SPARSE_MATRIX_ENUM:
    createComplexSparseMatrix(env, node);
    break;
  default:
    stringstream message;
    message << "Unsupported type for JavaTerminal. Type is " << type << ".";
    throw convert_error(message.str());
  }
}

// Helpers

/*
 * Converts a real8 * to a java double[]
 * @param env, the JNI environment pointer
 * @param inputData, the real8 array to convert
 * @param len the length of the array
 * @return a jobjectArray which is a double[] equivalent of {@code inputData}
 */
jdoubleArray convertCreal8Arr2JDoubleArr(JNIEnv * env, real8 * inputData, size_t len)
{
  jdoubleArray returnVal = JVMManager::newDoubleArray(env, len);
  env->SetDoubleArrayRegion(returnVal, 0, len, inputData);
  return returnVal;
}

jdoubleArray extractRealPartOfComplex16Arr2JDoubleArr(JNIEnv* env, complex16* inputData, size_t len)
{
  jdoubleArray returnVal = JVMManager::newDoubleArray(env, len);
  real8* reals = new real8[len];
  for (size_t i = 0; i < len; ++i)
  {
    reals[i] = std::real(inputData[i]);
  }
  env->SetDoubleArrayRegion(returnVal, 0, len, reals);
  return returnVal;
}

jdoubleArray extractComplexPartOfComplex16Arr2JDoubleArr(JNIEnv* env, complex16* inputData, size_t len)
{
  jdoubleArray returnVal = JVMManager::newDoubleArray(env, len);
  real8* imags = new real8[len];
  for (size_t i = 0; i < len; ++i)
  {
    imags[i] = std::imag(inputData[i]);
  }
  env->SetDoubleArrayRegion(returnVal, 0, len, imags);
  return returnVal;
}

void
JavaTerminal::createRealScalar(JNIEnv* env, const OGNumeric::Ptr& node)
{
  jclass cls = JVMManager::getOGRealScalarClazz();
  jmethodID constructor = JVMManager::getOGRealScalarClazz_init();
  jobject value = JVMManager::newDouble(env, node->asOGRealScalar()->getValue());
  _obj = env->NewObject(cls, constructor, value);
}

void
JavaTerminal::createComplexScalar(JNIEnv* env, const OGNumeric::Ptr& node)
{
  jclass cls = JVMManager::getOGComplexScalarClazz();
  jmethodID constructor = JVMManager::getOGComplexScalarClazz_init();
  complex16 value = node->asOGComplexScalar()->getValue();
  jobject real = JVMManager::newDouble(env, value.real());
  jobject imag = JVMManager::newDouble(env, value.imag());
  _obj = env->NewObject(cls, constructor, real, imag);
}

void
JavaTerminal::createRealMatrix(JNIEnv* env, const OGNumeric::Ptr& node)
{
  jclass cls = JVMManager::getOGRealDenseMatrixClazz();
  jmethodID constructor = JVMManager::getOGRealDenseMatrixClazz_init();
  jobjectArray darr = Real8AoA{node}.toJDoubleAoA(env);
  _obj = env->NewObject(cls, constructor, darr);
}

void
JavaTerminal::createComplexMatrix(JNIEnv* env, const OGNumeric::Ptr& node)
{
  jclass cls = JVMManager::getOGComplexDenseMatrixClazz();
  jmethodID constructor = JVMManager::getOGComplexDenseMatrixClazz_init();
  Complex16AoA c = Complex16AoA{node};
  jobjectArray realPart = c.realPartToJDoubleAoA(env);
  jobjectArray imagPart = c.imagPartToJDoubleAoA(env);
  _obj = env->NewObject(cls, constructor, realPart, imagPart);
}

void
JavaTerminal::createRealDiagonalMatrix(JNIEnv* env, const OGNumeric::Ptr& node)
{
  jclass cls = JVMManager::getOGRealDiagonalMatrixClazz();
  jmethodID constructor = JVMManager::getOGRealDiagonalMatrixClazz_init();
  const OGRealDiagonalMatrix::Ptr mat = node->asOGRealDiagonalMatrix();
  jdoubleArray darr = convertCreal8Arr2JDoubleArr(env, mat->toReal8Array(), mat->getDatalen());
  _obj = env->NewObject(cls, constructor, darr, mat->getRows(), mat->getCols());
}

void
JavaTerminal::createComplexDiagonalMatrix(JNIEnv* env, const OGNumeric::Ptr& node)
{
  jclass cls = JVMManager::getOGComplexDiagonalMatrixClazz();
  jmethodID constructor = JVMManager::getOGComplexDiagonalMatrixClazz_init();
  const OGComplexDiagonalMatrix::Ptr mat = node->asOGComplexDiagonalMatrix();
  complex16* values = mat->toComplex16Array();
  size_t datalen = mat->getDatalen();
  jdoubleArray realpart = extractRealPartOfComplex16Arr2JDoubleArr(env, values, datalen);
  jdoubleArray imagpart = extractComplexPartOfComplex16Arr2JDoubleArr(env, values, datalen);
  _obj = env->NewObject(cls, constructor, realpart, imagpart, mat->getRows(), mat->getCols());
}

jint* makeCJintArray(const int4* arr, size_t len)
{
  jint* jintArr = new jint[len];
  for (size_t i = 0; i < len; ++i)
  {
    jintArr[i] = arr[i];
  }
  return jintArr;
}

void
JavaTerminal::createRealSparseMatrix(JNIEnv* env, const OGNumeric::Ptr& node)
{
  const OGRealSparseMatrix::Ptr mat = node->asOGRealSparseMatrix();
  // Column pointer
  size_t colPtrLen = mat->getCols() + 1;
  jintArray jColPtr = JVMManager::newIntArray(env, colPtrLen);
  jint* colPtr = makeCJintArray(mat->getColPtr(), colPtrLen);
  env->SetIntArrayRegion(jColPtr, 0, colPtrLen, colPtr);
  delete[] colPtr;

  // Row index
  size_t datalen = mat->getDatalen();
  jintArray jRowIdx = JVMManager::newIntArray(env, datalen);
  jint* rowIdx = makeCJintArray(mat->getRowIdx(), datalen);
  env->SetIntArrayRegion(jRowIdx, 0, datalen, rowIdx);
  delete[] rowIdx;

  // Values
  jdoubleArray values = convertCreal8Arr2JDoubleArr(env, mat->toReal8Array(), datalen);

  // Call constructor
  jclass cls = JVMManager::getOGRealSparseMatrixClazz();
  jmethodID constructor = JVMManager::getOGRealSparseMatrixClazz_init();
  _obj = env->NewObject(cls, constructor, jColPtr, jRowIdx, values, mat->getRows(), mat->getCols());
}

void
JavaTerminal::createComplexSparseMatrix(JNIEnv* env, const OGNumeric::Ptr& node)
{
  const OGComplexSparseMatrix::Ptr mat = node->asOGComplexSparseMatrix();
  // Column pointer
  size_t colPtrLen = mat->getCols() + 1;
  jintArray jColPtr = JVMManager::newIntArray(env, colPtrLen);
  jint* colPtr = makeCJintArray(mat->getColPtr(), colPtrLen);
  env->SetIntArrayRegion(jColPtr, 0, colPtrLen, colPtr);
  delete[] colPtr;

  // Row index
  size_t datalen = mat->getDatalen();
  jintArray jRowIdx = JVMManager::newIntArray(env, datalen);
  jint* rowIdx = makeCJintArray(mat->getRowIdx(), datalen);
  env->SetIntArrayRegion(jRowIdx, 0, datalen, rowIdx);
  delete[] rowIdx;

  // Values
  complex16* values = mat->toComplex16Array();
  jdoubleArray realpart = extractRealPartOfComplex16Arr2JDoubleArr(env, values, datalen);
  jdoubleArray imagpart = extractComplexPartOfComplex16Arr2JDoubleArr(env, values, datalen);

  // Call constructor
  jclass cls = JVMManager::getOGComplexSparseMatrixClazz();
  jmethodID constructor = JVMManager::getOGComplexSparseMatrixClazz_init();
  _obj = env->NewObject(cls, constructor, jColPtr, jRowIdx, realpart, imagpart, mat->getRows(), mat->getCols());
}

jobject
JavaTerminal::getObject()
{
  return _obj;
}

} // namespace convert
