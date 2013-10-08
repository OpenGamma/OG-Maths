/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include "com_opengamma_longdog_materialisers_Materialisers.h"
#include "entrypt.hh"
#include "jvmmanager.hh"
#include "expression.hh"
#include "exprtypeenum.h"
#include "exprfactory.hh"
#include "warningmacros.h"
#include "dispatch.hh"
#include "debug.h"
#include <stdio.h>

using namespace convert;

#ifdef __cplusplus
extern "C" {
#endif

//
//
// JNI methods and helpers
//
//

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

/*
 * Class:     com_opengamma_longdog_materialisers_Materialisers
 * Method:    materialiseToJDoubleArrayOfArrays
 * Signature: (Lcom/opengamma/longdog/datacontainers/OGNumeric;)[[D
 */
JNIEXPORT jobjectArray JNICALL Java_com_opengamma_longdog_materialisers_Materialisers_materialiseToJDoubleArrayOfArrays
(JNIEnv *env, jclass SUPPRESS_UNUSED clazz, jobject obj)
{
  DEBUG_PRINT("Entering materialise function\n");

  DEBUG_PRINT("Calling convert::createExpression\n");
  // convert obj to OGNumeric objs
  librdag::OGNumeric * chain = convert::createExpression(obj);

  DEBUG_PRINT("Calling entrypt function\n");
  const librdag::OGTerminal* answer = entrypt(chain);
  delete chain;
  convert::OGTerminalPtrContainer_t<real16>* res = convert::dispatchToReal16ArrayOfArrays(answer);
  jobjectArray returnVal = convertCreal16ArrOfArr2JDoubleArrOfArr(env, res->data, res->rows, res->cols);

  DEBUG_PRINT("Returning\n");
  return returnVal;
}

/*
 * Class:     com_opengamma_longdog_materialisers_Materialisers
 * Method:    materialiseToJComplexArrayContainer
 * Signature: (Lcom/opengamma/longdog/datacontainers/OGNumeric;)Lcom/opengamma/longdog/datacontainers/other/ComplexArrayContainer;
 */
JNIEXPORT jobject JNICALL Java_com_opengamma_longdog_materialisers_Materialisers_materialiseToJComplexArrayContainer
(JNIEnv *env, jclass SUPPRESS_UNUSED clazz, jobject obj)
{
  DEBUG_PRINT("materialiseToJComplexArrayContainer\n");
  DEBUG_PRINT("Entering materialise function\n");

  DEBUG_PRINT("Calling convert::createExpression\n");
  // convert obj to OGNumeric objs
  librdag::OGNumeric * chain = convert::createExpression(obj);

  DEBUG_PRINT("Calling entrypt function\n");
  const librdag::OGTerminal* answer = entrypt(chain);
  delete chain;
  convert::OGTerminalPtrContainer_t<complex16>* res = convert::dispatchToComplex16ArrayOfArrays(answer);

  jobjectArray realPart = extractRealPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(env, res->data, res->rows, res->cols);
  jobjectArray complexPart = extractImagPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(env, res->data, res->rows, res->cols);
  jobject returnVal = env->NewObject(JVMManager::getComplexArrayContainerClazz(),
                                     JVMManager::getComplexArrayContainerClazz_ctor_DAoA_DAoA(),
                                     realPart, complexPart);

  DEBUG_PRINT("Returning\n");
  return returnVal;
}

/*
 * Class:     com_opengamma_longdog_materialisers_Materialisers
 * Method:    materialiseToJBoolean
 * Signature: (Lcom/opengamma/longdog/datacontainers/OGNumeric;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_opengamma_longdog_materialisers_Materialisers_materialiseToJBoolean
(JNIEnv SUPPRESS_UNUSED *env, jclass SUPPRESS_UNUSED clazz, jobject SUPPRESS_UNUSED obj)
{
  jboolean returnVal = 0;
  return returnVal;
}

#ifdef __cplusplus
}
#endif
