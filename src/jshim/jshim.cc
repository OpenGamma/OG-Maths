/**
 * Copyright (C) 2013 - present by OpenGamma Inc. and the OpenGamma group of companies
 *
 * Please see distribution for license.
 */

#include <sstream>
#include "com_opengamma_longdog_materialisers_Materialisers.h"
#include "entrypt.hh"
#include "jvmmanager.hh"
#include "expression.hh"
#include "exprfactory.hh"
#include "warningmacros.h"
#include "jdispatch.hh"
#include "visitor.hh"
#include "debug.h"
#include <stdio.h>

using namespace convert;
using namespace dogma_exceptions;

jobjectArray javaBacktrace(JNIEnv* env, const BacktraceElement* backtrace, size_t traceSize)
{
  // Create new array of strings for backtrace
  jclass strClass = env->FindClass("java/lang/String");
  jstring empty = env->NewStringUTF("");
  size_t nStr = traceSize * 4; // File name, class, method, line number
  jobjectArray btArray = env->NewObjectArray(nStr, strClass, empty);
  checkEx(env);

  for (size_t i = 0; i < traceSize; i++)
  {
    const BacktraceElement* frame = &backtrace[i];
    jstring className = env->NewStringUTF(frame->getObjectFile().c_str());
    jstring methodName = env->NewStringUTF(frame->getFunction().c_str());
    stringstream ss;
    ss << (long long unsigned) frame->getAddress();
    jstring fileName = env->NewStringUTF(ss.str().c_str());
    jstring lineNo = env->NewStringUTF("1");
    env->SetObjectArrayElement(btArray, ((i * 4) + 0), className);
    env->SetObjectArrayElement(btArray, ((i * 4) + 1), methodName);
    env->SetObjectArrayElement(btArray, ((i * 4) + 2), fileName);
    env->SetObjectArrayElement(btArray, ((i * 4) + 3), lineNo);
    checkEx(env);
  }

  return btArray;
}

void convertExceptionJava(JNIEnv* env, convert_error& e)
{
#ifdef __MINGW32__
  env->ThrowNew(JVMManager::getMathsExceptionNativeConversionClazz(), e.what());
#else
  jstring msg = env->NewStringUTF(e.what());
  jmethodID ctor = env->GetMethodID(JVMManager::getMathsExceptionNativeConversionClazz(), "<init>", "(Ljava/lang/String;[Ljava/lang/String;)V");
  checkEx(env);
  jobjectArray btArray = javaBacktrace(env, e.getBacktrace(), e.getTraceSize());
  jthrowable jex = (jthrowable) env->NewObject(JVMManager::getMathsExceptionNativeConversionClazz(), ctor, msg, btArray);
  env->Throw(jex);
#endif // __MINGW32__
}

void rdagExceptionJava(JNIEnv* env, rdag_error& e)
{
#ifdef __MINGW32__
  env->ThrowNew(JVMManager::getMathsExceptionNativeConversionClazz(), e.what());
#else
  jstring msg = env->NewStringUTF(e.what());
  jmethodID ctor = env->GetMethodID(JVMManager::getMathsExceptionNativeComputationClazz(), "<init>", "(Ljava/lang/String;[Ljava/lang/String;)V");
  checkEx(env);
  jobjectArray btArray = javaBacktrace(env, e.getBacktrace(), e.getTraceSize());
  jthrowable jex = (jthrowable) env->NewObject(JVMManager::getMathsExceptionNativeComputationClazz(), ctor, msg, btArray);
  env->Throw(jex);
#endif // __MINGW32__
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

  librdag::OGNumeric* chain;
  jobjectArray returnVal;

  try
  {
    // convert obj to OGNumeric objs
    DEBUG_PRINT("Calling convert::createExpression\n");
    chain = convert::createExpression(obj);
    DEBUG_PRINT("Check for exception before entrypt\n");
    checkEx(env);
    DEBUG_PRINT("Calling entrypt function\n");
    const librdag::OGTerminal* answer = entrypt(chain);
    DEBUG_PRINT("Returning from entrypt function\n");

    DispatchToReal16ArrayOfArrays *visitor = new DispatchToReal16ArrayOfArrays();
    answer->accept(*visitor);

    returnVal = convertCreal16ArrOfArr2JDoubleArrOfArr(env, visitor->getData(), visitor->getRows(), visitor->getCols());
    delete visitor;
  }
  catch (convert_error e)
  {
    convertExceptionJava(env, e);
    return nullptr;
  }
  catch (rdag_error e)
  {
    rdagExceptionJava(env, e);
    return nullptr;
  }

  delete chain;

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

  librdag::OGNumeric * chain;
  jobject returnVal;

  try
  {
    DEBUG_PRINT("Calling convert::createExpression\n");
    // convert obj to OGNumeric objs
    chain = convert::createExpression(obj);

    DEBUG_PRINT("Check for exception before entrypt\n");
    checkEx(env);
    DEBUG_PRINT("Calling entrypt function\n");
    const librdag::OGTerminal* answer = entrypt(chain);


    DispatchToComplex16ArrayOfArrays *visitor = new DispatchToComplex16ArrayOfArrays();
    answer->accept(*visitor);

    jobjectArray realPart = extractRealPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(env, visitor->getData(), visitor->getRows(), visitor->getCols());
    jobjectArray complexPart = extractImagPartOfCcomplex16ArrOfArr2JDoubleArrOfArr(env, visitor->getData(), visitor->getRows(), visitor->getCols());

    delete visitor;

    returnVal = env->NewObject(JVMManager::getComplexArrayContainerClazz(),
                               JVMManager::getComplexArrayContainerClazz_ctor_DAoA_DAoA(),
                               realPart, complexPart);
  }
  catch (convert_error e)
  {
    convertExceptionJava(env, e);
    return nullptr;
  }
  catch (rdag_error e)
  {
    rdagExceptionJava(env, e);
    return nullptr;
  }

  delete chain;

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
  librdag::OGNumeric * chain;
  jobject result;

  try
  {
  // convert obj to OGNumeric objs
    chain = convert::createExpression(obj);

    DEBUG_PRINT("Check for exception before entrypt\n");
    checkEx(env);
    DEBUG_PRINT("Calling entrypt function\n");
    const librdag::OGTerminal* answer = entrypt(chain);

    DispatchToOGTerminal *visitor = new DispatchToOGTerminal(env);
    answer->accept(*visitor);
    result = visitor->getObject();
    delete visitor;
  }
  catch (convert_error e)
  {
    convertExceptionJava(env, e);
    return nullptr;
  }
  catch (rdag_error e)
  {
    rdagExceptionJava(env, e);
    return nullptr;
  }

  delete chain;

  DEBUG_PRINT("Returning\n");
  return result;
}

#ifdef __cplusplus
}
#endif
