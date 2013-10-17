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

namespace convert
{

using namespace librdag;

/**
 * DispatchToReal16ArrayOfArrays
 */

DispatchToReal16ArrayOfArrays::~DispatchToReal16ArrayOfArrays()
{
    real16 ** arr = this->getData();
    if(arr)
    {

        for(int i=0; i<this->getRows(); i++)
        {
            delete arr[i];
        }
        delete arr;
    }
}

void DispatchToReal16ArrayOfArrays::visit(librdag::OGExpr const SUPPRESS_UNUSED *thing)
{
    throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGExpr)");
}

void DispatchToReal16ArrayOfArrays::visit(librdag::OGScalar<real16> const  *thing)
{
    this->setData(thing->toReal16ArrayOfArrays());
    this->setRows(1);
    this->setCols(1);
}

void DispatchToReal16ArrayOfArrays::visit(librdag::OGScalar<complex16> const SUPPRESS_UNUSED  *thing)
{
    throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGScalar<complex16>)");
}
void DispatchToReal16ArrayOfArrays::visit(librdag::OGScalar<int> const SUPPRESS_UNUSED  *thing)
{
    throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGScalar<int>)");
}

void DispatchToReal16ArrayOfArrays::visit(librdag::OGMatrix<real16> const *thing)
{
    this->setData(thing->toReal16ArrayOfArrays());
    this->setRows(thing->getRows());
    this->setCols(thing->getCols());
}

void DispatchToReal16ArrayOfArrays::visit(librdag::OGMatrix<complex16> const SUPPRESS_UNUSED *thing)
{
    throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGMatrix<complex16>)");
}
void DispatchToReal16ArrayOfArrays::visit(librdag::OGDiagonalMatrix<real16> const *thing)
{
    this->setData(thing->toReal16ArrayOfArrays());
    this->setRows(thing->getRows());
    this->setCols(thing->getCols());
}
void DispatchToReal16ArrayOfArrays::visit(librdag::OGDiagonalMatrix<complex16> const SUPPRESS_UNUSED  *thing)
{
    throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGDiagonalMatrix<complex16>)");
}

void DispatchToReal16ArrayOfArrays::visit(librdag::OGSparseMatrix<real16> const *thing)
{
    this->setData(thing->toReal16ArrayOfArrays());
    this->setRows(thing->getRows());
    this->setCols(thing->getCols());
}
void DispatchToReal16ArrayOfArrays::visit(librdag::OGSparseMatrix<complex16> const SUPPRESS_UNUSED  *thing)
{
    throw std::runtime_error("DispatchToReal16ArrayOfArrays::visit(librdag::OGSparseMatrix<complex16>)");
}

void DispatchToReal16ArrayOfArrays::setData(real16 ** data)
{
    this->_data = data;
}

void DispatchToReal16ArrayOfArrays::setRows(int rows)
{
    this->rows = rows;
}

void DispatchToReal16ArrayOfArrays::setCols(int cols)
{
    this->cols = cols;
}

real16 ** DispatchToReal16ArrayOfArrays::getData()
{
    return this->_data;
}

int DispatchToReal16ArrayOfArrays::getRows()
{
    return this->rows;
}

int DispatchToReal16ArrayOfArrays::getCols()
{
    return this->cols;
}

/**
 * DispatchToComplex16ArrayOfArrays
 */

DispatchToComplex16ArrayOfArrays::~DispatchToComplex16ArrayOfArrays()
{
    complex16 ** arr = this->getData();
    if(arr)
    {

        for(int i=0; i<this->getRows(); i++)
        {
            delete arr[i];
        }
        delete arr;
    }
}
void DispatchToComplex16ArrayOfArrays::visit(librdag::OGExpr const SUPPRESS_UNUSED *thing)
{
    throw std::runtime_error("DispatchToComplex16ArrayOfArrays::visit(librdag::OGExpr)");
}
void DispatchToComplex16ArrayOfArrays::visit(librdag::OGScalar<complex16> const *thing)
{
    this->setData(thing->toComplex16ArrayOfArrays());
    this->setRows(1);
    this->setCols(1);
}
void DispatchToComplex16ArrayOfArrays::visit(librdag::OGScalar<real16> const SUPPRESS_UNUSED  *thing)
{
    throw std::runtime_error("DispatchToComplex16ArrayOfArrays::visit(librdag::OGScalar<real16>)");
}
void DispatchToComplex16ArrayOfArrays::visit(librdag::OGScalar<int> const SUPPRESS_UNUSED *thing)
{
    throw std::runtime_error("DispatchToComplex16ArrayOfArrays::visit(librdag::OGScalar<int>)");
}
void DispatchToComplex16ArrayOfArrays::visit(librdag::OGMatrix<complex16> const *thing)
{
    printf("Visitor: librdag::OGMatrix<complex16> branch\n");
    this->setData(thing->toComplex16ArrayOfArrays());
    this->setRows(thing->getRows());
    this->setCols(thing->getCols());
}
void DispatchToComplex16ArrayOfArrays::visit(librdag::OGMatrix<real16> const SUPPRESS_UNUSED *thing)
{
    throw std::runtime_error("DispatchToComplex16ArrayOfArrays::visit(librdag::OGMatrix<real16>)");
}
void DispatchToComplex16ArrayOfArrays::visit(librdag::OGDiagonalMatrix<complex16> const *thing)
{
    printf("Visitor: librdag::OGMatrix<complex16> branch\n");
    this->setData(thing->toComplex16ArrayOfArrays());
    this->setRows(thing->getRows());
    this->setCols(thing->getCols());
}
void DispatchToComplex16ArrayOfArrays::visit(librdag::OGDiagonalMatrix<real16> const SUPPRESS_UNUSED *thing)
{
    throw std::runtime_error("DispatchToComplex16ArrayOfArrays::visit(librdag::OGDiagonalMatrix<real16>)");
}
void DispatchToComplex16ArrayOfArrays::visit(librdag::OGSparseMatrix<complex16> const *thing)
{
    printf("Visitor: librdag::OGMatrix<complex16> branch\n");
    this->setData(thing->toComplex16ArrayOfArrays());
    this->setRows(thing->getRows());
    this->setCols(thing->getCols());
}
void DispatchToComplex16ArrayOfArrays::visit(librdag::OGSparseMatrix<real16> const SUPPRESS_UNUSED *thing)
{
    throw std::runtime_error("DispatchToComplex16ArrayOfArrays::visit(librdag::OGSparseMatrix<real16>)");
}
void DispatchToComplex16ArrayOfArrays::setData(complex16 ** data)
{
    this->_data = data;
}
void DispatchToComplex16ArrayOfArrays::setRows(int rows)
{
    this->rows = rows;
}
void DispatchToComplex16ArrayOfArrays::setCols(int cols)
{
    this->cols = cols;
}
complex16 ** DispatchToComplex16ArrayOfArrays::getData()
{
    return this->_data;
}
int DispatchToComplex16ArrayOfArrays::getRows()
{
    return this->rows;
}
int DispatchToComplex16ArrayOfArrays::getCols()
{
    return this->cols;
}

/**
 * DispatchToOGTerminal
 */

// Helpers

/*
 * Converts a real16 * to a java double[]
 * @param env, the JNI environment pointer
 * @param inputData, the real16 array to convert
 * @param len the length of the array
 * @return a jobjectArray which is a double[] equivalent of {@code inputData}
 */
jdoubleArray convertCreal16Arr2JDoubleArr(JNIEnv * env, real16 * inputData, int len)
{
  jdoubleArray returnVal = JVMManager::newDoubleArray(env, len);
  env->SetDoubleArrayRegion(returnVal, 0, len, inputData);
  return returnVal;
}

/*
 * Converts a real16 ** to a java double[][]
 * @param env, the JNI environment pointer
 * @param inputData, the real16 array of arrays to convert
 * @param rows the number of rows in the array
 * @param cols the number of columns in the array
 * @return a jobjectArray which is a double[][] equivalent of {@code inputData}
 */
jobjectArray convertCreal16ArrOfArr2JDoubleArrOfArr(JNIEnv * env, real16 ** inputData, int rows, int cols)
{
  jobjectArray returnVal = JVMManager::newObjectArray(env, rows, JVMManager::getBigDDoubleArrayClazz(), NULL);
  for(int i = 0; i < rows; i++)
  {
    jdoubleArray tmp = JVMManager::newDoubleArray(env, cols);
    env->SetDoubleArrayRegion(tmp, 0, cols, inputData[i]);
    env->SetObjectArrayElement(returnVal, i, tmp);
  }
  return returnVal;
}

jdoubleArray extractRealPartOfComplex16Arr2JDoubleArr(JNIEnv* env, complex16* inputData, int len)
{
  jdoubleArray returnVal = JVMManager::newDoubleArray(env, len);
  real16* reals = new real16[len];
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
  real16* imags = new real16[len];
  for (int i = 0; i < len; ++i)
  {
    imags[i] = std::imag(inputData[i]);
  }
  env->SetDoubleArrayRegion(returnVal, 0, len, imags);
  return returnVal;
}

/**
 *
 */
jobjectArray extractRealPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(JNIEnv * env, complex16 ** inputData, int rows, int cols)
{
  jobjectArray returnVal = JVMManager::newObjectArray(env, rows, JVMManager::getBigDDoubleArrayClazz(), NULL);
  real16 * aRow = new real16[cols];
  for(int i = 0; i < rows; i++)
  {
    jdoubleArray tmp = JVMManager::newDoubleArray(env, cols);
    for(int j = 0; j < cols; j++)
    {
      aRow[j]=std::real(inputData[i][j]);
    }
    env->SetDoubleArrayRegion(tmp, 0, cols, aRow);
    env->SetObjectArrayElement(returnVal, i, tmp);
  }
  delete aRow;
  return returnVal;
}

/**
*
*/
jobjectArray extractImagPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(JNIEnv * env, complex16 ** inputData, int rows, int cols)
{
  jobjectArray returnVal = JVMManager::newObjectArray(env, rows, JVMManager::getBigDDoubleArrayClazz(), NULL);
  real16 * aRow = new real16[cols];
  for(int i = 0; i < rows; i++)
  {
    jdoubleArray tmp = JVMManager::newDoubleArray(env, cols);
    for(int j = 0; j < cols; j++)
    {
      aRow[j]=std::imag(inputData[i][j]);
    }
    env->SetDoubleArrayRegion(tmp, 0, cols, aRow);
    env->SetObjectArrayElement(returnVal, i, tmp);
  }
  delete aRow;
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
DispatchToOGTerminal::visit(librdag::OGScalar<real16> const *thing)
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
DispatchToOGTerminal::visit(librdag::OGMatrix<real16> const *thing)
{
  jclass cls = JVMManager::getOGRealDenseMatrixClazz();
  jmethodID constructor = JVMManager::getOGRealDenseMatrixClazz_init();
  jobjectArray darr = convertCreal16ArrOfArr2JDoubleArrOfArr(_env, thing->toReal16ArrayOfArrays(), thing->getRows(), thing->getCols());
  jobject newobject = _env->NewObject(cls, constructor, darr);
  setObject(newobject);
}

void
DispatchToOGTerminal::visit(librdag::OGMatrix<complex16> const *thing)
{
  jclass cls = JVMManager::getOGComplexDenseMatrixClazz();
  jmethodID constructor = JVMManager::getOGComplexDenseMatrixClazz_init();
  complex16** values = thing->toComplex16ArrayOfArrays();
  int rows = thing->getRows();
  int cols = thing->getCols();
  jobjectArray realPart = extractRealPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(_env, values, rows, cols);
  jobjectArray complexPart = extractImagPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(_env, values, rows, cols);
  jobject newobject = _env->NewObject(cls, constructor, realPart, complexPart);
  setObject(newobject);
}

void
DispatchToOGTerminal::visit(librdag::OGDiagonalMatrix<real16> const *thing)
{
  jclass cls = JVMManager::getOGRealDiagonalMatrixClazz();
  jmethodID constructor = JVMManager::getOGRealDiagonalMatrixClazz_init();
  jdoubleArray darr = convertCreal16Arr2JDoubleArr(_env, thing->toReal16Array(), thing->getDatalen());
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
DispatchToOGTerminal::visit(librdag::OGSparseMatrix<real16> const *thing)
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
  jdoubleArray values = convertCreal16Arr2JDoubleArr(_env, thing->toReal16Array(), datalen);

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
