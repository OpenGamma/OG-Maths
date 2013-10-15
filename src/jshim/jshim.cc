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
#include "visitor.hh"
#include "debug.h"
#include <stdio.h>

using namespace convert;

/*
 * Check for exception
 */
void checkEx(JNIEnv* env)
{
  jthrowable e = env->ExceptionOccurred();
  if (e)
  {
    env->ExceptionDescribe();
    throw convert_error("Java exception thrown in JNI.");
  }
}

#ifdef __cplusplus
extern "C" {
#endif

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

  DEBUG_PRINT("Check for exception before entrypt\n");
  checkEx(env);
  DEBUG_PRINT("Calling entrypt function\n");
  const librdag::OGTerminal* answer = entrypt(chain);
  delete chain;

  DispatchToReal16ArrayOfArrays *visitor = new DispatchToReal16ArrayOfArrays();
  answer->accept(*visitor);

  jobjectArray returnVal = convertCreal16ArrOfArr2JDoubleArrOfArr(env, visitor->getData(), visitor->getRows(), visitor->getCols());

  delete visitor;

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

  DEBUG_PRINT("Check for exception before entrypt\n");
  checkEx(env);
  DEBUG_PRINT("Calling entrypt function\n");
  const librdag::OGTerminal* answer = entrypt(chain);
  delete chain;

  DispatchToComplex16ArrayOfArrays *visitor = new DispatchToComplex16ArrayOfArrays();
  answer->accept(*visitor);

  jobjectArray realPart = extractRealPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(env, visitor->getData(), visitor->getRows(), visitor->getCols());
  jobjectArray complexPart = extractImagPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(env, visitor->getData(), visitor->getRows(), visitor->getCols());
  delete visitor;

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

JNIEXPORT jobject JNICALL
Java_com_opengamma_longdog_materialisers_Materialisers_materialiseToOGTerminal(JNIEnv SUPPRESS_UNUSED *env, jclass SUPPRESS_UNUSED clazz, jobject obj)
{
  DEBUG_PRINT("materialiseToOGTerminal\n");
  DEBUG_PRINT("Calling convert::createExpression\n");
  // convert obj to OGNumeric objs
  librdag::OGNumeric * chain = convert::createExpression(obj);

  DEBUG_PRINT("Check for exception before entrypt\n");
  checkEx(env);
  DEBUG_PRINT("Calling entrypt function\n");
  const librdag::OGTerminal* answer = entrypt(chain);
  delete chain;

  DispatchToOGTerminal *visitor = new DispatchToOGTerminal(env);
  answer->accept(*visitor);
  jobject result = visitor->getObject();
  delete visitor;

  DEBUG_PRINT("Returning\n");
  return result;
}

#ifdef __cplusplus
}
#endif
