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
    for(int i = 0; i < _rows; i++)
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
  for(int i = 0; i < _rows; i++)
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

int
Real8AoA::getRows() const
{
  return _rows;
}

int
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
    for(int i = 0; i < _rows; i++)
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

template<double (F)(const complex<double>&)>
jobjectArray
Complex16AoA::toJDoubleAoA(JNIEnv* env) const
{
  jobjectArray returnVal = JVMManager::newObjectArray(env, _rows, JVMManager::getBigDDoubleArrayClazz(), NULL);
  real8 * aRow = new real8[_cols];
  for(int i = 0; i < _rows; i++)
  {
    jdoubleArray tmp = JVMManager::newDoubleArray(env, _cols);
    for(int j = 0; j < _cols; j++)
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

int
Complex16AoA::getRows() const
{
  return _rows;
}

int
Complex16AoA::getCols() const
{
  return _cols;
}

/**
 * DispatchToOGTerminal
 */

// Helpers

/*
 * Converts a real8 * to a java double[]
 * @param env, the JNI environment pointer
 * @param inputData, the real8 array to convert
 * @param len the length of the array
 * @return a jobjectArray which is a double[] equivalent of {@code inputData}
 */
jdoubleArray convertCreal8Arr2JDoubleArr(JNIEnv * env, real8 * inputData, int len)
{
  jdoubleArray returnVal = JVMManager::newDoubleArray(env, len);
  env->SetDoubleArrayRegion(returnVal, 0, len, inputData);
  return returnVal;
}

jdoubleArray extractRealPartOfComplex16Arr2JDoubleArr(JNIEnv* env, complex16* inputData, int len)
{
  jdoubleArray returnVal = JVMManager::newDoubleArray(env, len);
  real8* reals = new real8[len];
  for (int i = 0; i < len; ++i)
  {
    reals[i] = std::real(inputData[i]);
  }
  env->SetDoubleArrayRegion(returnVal, 0, len, reals);
  return returnVal;
}

jdoubleArray extractComplexPartOfComplex16Arr2JDoubleArr(JNIEnv* env, complex16* inputData, int len)
{
  jdoubleArray returnVal = JVMManager::newDoubleArray(env, len);
  real8* imags = new real8[len];
  for (int i = 0; i < len; ++i)
  {
    imags[i] = std::imag(inputData[i]);
  }
  env->SetDoubleArrayRegion(returnVal, 0, len, imags);
  return returnVal;
}

DispatchToOGTerminal::DispatchToOGTerminal(JNIEnv* env): _obj(nullptr), _env(env) {}

DispatchToOGTerminal::~DispatchToOGTerminal() {}

void
DispatchToOGTerminal::visit(librdag::OGExpr SUPPRESS_UNUSED const *thing)
{
  throw convert_error("Dispatch of expression to terminal not allowed");
}

void
DispatchToOGTerminal::visit(librdag::OGScalar<real8> const *thing)
{
  jclass cls = JVMManager::getOGRealScalarClazz();
  jmethodID constructor = JVMManager::getOGRealScalarClazz_init();
  jobject value = JVMManager::newDouble(_env, thing->getValue());
  jobject newobject = _env->NewObject(cls, constructor, value);
  setObject(newobject);
}

void
DispatchToOGTerminal::visit(librdag::OGScalar<complex16> const *thing)
{
  jclass cls = JVMManager::getOGComplexScalarClazz();
  jmethodID constructor = JVMManager::getOGComplexScalarClazz_init();
  complex16 value = thing->getValue();
  jobject real = JVMManager::newDouble(_env, value.real());
  jobject imag = JVMManager::newDouble(_env, value.imag());
  jobject newobject = _env->NewObject(cls, constructor, real, imag);
  setObject(newobject);
}

void
DispatchToOGTerminal::visit(librdag::OGScalar<int> SUPPRESS_UNUSED const *thing)
{
  throw convert_error("We do not materialise integer scalars.");
}

void
DispatchToOGTerminal::visit(librdag::OGMatrix<real8> const *thing)
{
  jclass cls = JVMManager::getOGRealDenseMatrixClazz();
  jmethodID constructor = JVMManager::getOGRealDenseMatrixClazz_init();
  jobjectArray darr = Real8AoA{OGNumeric::Ptr{thing->createOwningCopy()}}.toJDoubleAoA(_env);
  jobject newobject = _env->NewObject(cls, constructor, darr);
  setObject(newobject);
}

void
DispatchToOGTerminal::visit(librdag::OGMatrix<complex16> const *thing)
{
  jclass cls = JVMManager::getOGComplexDenseMatrixClazz();
  jmethodID constructor = JVMManager::getOGComplexDenseMatrixClazz_init();
  Complex16AoA c = Complex16AoA{OGNumeric::Ptr{thing->createOwningCopy()}};
  jobjectArray realPart = c.realPartToJDoubleAoA(_env);
  jobjectArray imagPart = c.imagPartToJDoubleAoA(_env);
  jobject newobject = _env->NewObject(cls, constructor, realPart, imagPart);
  setObject(newobject);
}

void
DispatchToOGTerminal::visit(librdag::OGDiagonalMatrix<real8> const *thing)
{
  jclass cls = JVMManager::getOGRealDiagonalMatrixClazz();
  jmethodID constructor = JVMManager::getOGRealDiagonalMatrixClazz_init();
  jdoubleArray darr = convertCreal8Arr2JDoubleArr(_env, thing->toReal8Array(), thing->getDatalen());
  jobject newobject = _env->NewObject(cls, constructor, darr, thing->getRows(), thing->getCols());
  setObject(newobject);
}

void
DispatchToOGTerminal::visit(librdag::OGDiagonalMatrix<complex16> const *thing)
{
  jclass cls = JVMManager::getOGComplexDiagonalMatrixClazz();
  jmethodID constructor = JVMManager::getOGComplexDiagonalMatrixClazz_init();
  complex16* values = thing->toComplex16Array();
  int datalen = thing->getDatalen();
  jdoubleArray realpart = extractRealPartOfComplex16Arr2JDoubleArr(_env, values, datalen);
  jdoubleArray imagpart = extractComplexPartOfComplex16Arr2JDoubleArr(_env, values, datalen);
  jobject newobject = _env->NewObject(cls, constructor, realpart, imagpart, thing->getRows(), thing->getCols());
  setObject(newobject);
}

jint* makeCJintArray(const int* arr, int len)
{
  jint* jintArr = new jint[len];
  for (int i = 0; i < len; ++i)
  {
    jintArr[i] = arr[i];
  }
  return jintArr;
}

void
DispatchToOGTerminal::visit(librdag::OGSparseMatrix<real8> const *thing)
{
  // Column pointer
  int colPtrLen = thing->getCols() + 1;
  jintArray jColPtr = JVMManager::newIntArray(_env, colPtrLen);
  jint* colPtr = makeCJintArray(thing->getColPtr(), colPtrLen);
  _env->SetIntArrayRegion(jColPtr, 0, colPtrLen, colPtr);
  delete[] colPtr;

  // Row index
  int datalen = thing->getDatalen();
  jintArray jRowIdx = JVMManager::newIntArray(_env, datalen);
  jint* rowIdx = makeCJintArray(thing->getRowIdx(), datalen);
  _env->SetIntArrayRegion(jRowIdx, 0, datalen, rowIdx);
  delete[] rowIdx;

  // Values
  jdoubleArray values = convertCreal8Arr2JDoubleArr(_env, thing->toReal8Array(), datalen);

  // Call constructor
  jclass cls = JVMManager::getOGRealSparseMatrixClazz();
  jmethodID constructor = JVMManager::getOGRealSparseMatrixClazz_init();
  jobject newobject = _env->NewObject(cls, constructor, jColPtr, jRowIdx, values, thing->getRows(), thing->getCols());

  // Done
  setObject(newobject);
}

void
DispatchToOGTerminal::visit(librdag::OGSparseMatrix<complex16> const *thing)
{
  // Column pointer
  int colPtrLen = thing->getCols() + 1;
  jintArray jColPtr = JVMManager::newIntArray(_env, colPtrLen);
  jint* colPtr = makeCJintArray(thing->getColPtr(), colPtrLen);
  _env->SetIntArrayRegion(jColPtr, 0, colPtrLen, colPtr);
  delete[] colPtr;

  // Row index
  int datalen = thing->getDatalen();
  jintArray jRowIdx = JVMManager::newIntArray(_env, datalen);
  jint* rowIdx = makeCJintArray(thing->getRowIdx(), datalen);
  _env->SetIntArrayRegion(jRowIdx, 0, datalen, rowIdx);
  delete[] rowIdx;

  // Values
  complex16* values = thing->toComplex16Array();
  jdoubleArray realpart = extractRealPartOfComplex16Arr2JDoubleArr(_env, values, datalen);
  jdoubleArray imagpart = extractComplexPartOfComplex16Arr2JDoubleArr(_env, values, datalen);

  // Call constructor
  jclass cls = JVMManager::getOGComplexSparseMatrixClazz();
  jmethodID constructor = JVMManager::getOGComplexSparseMatrixClazz_init();
  jobject newobject = _env->NewObject(cls, constructor, jColPtr, jRowIdx, realpart, imagpart, thing->getRows(), thing->getCols());

  // Done
  setObject(newobject);
}

jobject
DispatchToOGTerminal::getObject()
{
  return _obj;
}

void
DispatchToOGTerminal::setObject(jobject obj)
{
  _obj = obj;
}



} // namespace convert
